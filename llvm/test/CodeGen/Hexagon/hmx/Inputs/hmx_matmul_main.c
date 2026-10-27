/*
 * QuRT + hexagon-sim harness for HMX MAC recipes.
 *
 * VTCM via HAP_request_VTCM. Tiles are 2048-byte aligned. Reads recipe.txt
 * plus the named tile/bias files from the --usefs working directory, calls
 * hmx_mac_tile, byte-compares the drain to the golden, prints PASS <checksum>.
 *
 * recipe.txt keys (whitespace-separated):
 *   has_bias <0|1>
 *   bias_bytes <n>          (required if has_bias=1; typically 256)
 *   a_bytes / b_bytes / c_bytes <n>
 *   bias_file / a_file / b_file / c_file <name>
 */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <qurt.h>
#include <qurt_hmx.h>

enum { kAlign = 2048, kVtcmBytes = 32768, kMaxTile = 8192 };

/* Every recipe .ll exports this symbol. bias may be unused (ub/f8). */
void hmx_mac_tile(void *bias, void *A, void *B, void *C);

void *HAP_request_VTCM(unsigned int size, unsigned int single_page_flag);

typedef struct {
  int has_bias;
  unsigned bias_bytes;
  unsigned a_bytes;
  unsigned b_bytes;
  unsigned c_bytes;
  char bias_file[64];
  char a_file[64];
  char b_file[64];
  char c_file[64];
} Recipe;

static int read_file(const char *path, void *dst, unsigned nbytes) {
  FILE *f = fopen(path, "rb");
  if (!f) {
    printf("FAIL open %s\n", path);
    return -1;
  }
  size_t n = fread(dst, 1, nbytes, f);
  fclose(f);
  if (n != nbytes) {
    printf("FAIL read %s got %u want %u\n", path, (unsigned)n, nbytes);
    return -1;
  }
  return 0;
}

static unsigned checksum_u16(const void *p, unsigned nbytes) {
  const uint16_t *w = (const uint16_t *)p;
  unsigned cs = 0;
  unsigned n = nbytes / 2u;
  for (unsigned i = 0; i < n; ++i)
    cs ^= (unsigned)w[i];
  /* Odd trailing byte (if any). */
  if (nbytes & 1u)
    cs ^= ((const uint8_t *)p)[nbytes - 1u];
  return cs;
}

static int load_recipe(Recipe *r) {
  memset(r, 0, sizeof(*r));
  r->has_bias = 0;
  r->bias_bytes = 256;
  r->a_bytes = r->b_bytes = r->c_bytes = 2048;
  strcpy(r->bias_file, "identity_bias.bin");
  strcpy(r->a_file, "tile_a.bin");
  strcpy(r->b_file, "tile_b.bin");
  strcpy(r->c_file, "tile_c_expected.bin");

  FILE *f = fopen("recipe.txt", "r");
  if (!f) {
    puts("FAIL open recipe.txt");
    return -1;
  }
  char key[64], val[64];
  while (fscanf(f, "%63s %63s", key, val) == 2) {
    if (!strcmp(key, "has_bias"))
      r->has_bias = (int)strtol(val, NULL, 0);
    else if (!strcmp(key, "bias_bytes"))
      r->bias_bytes = (unsigned)strtoul(val, NULL, 0);
    else if (!strcmp(key, "a_bytes"))
      r->a_bytes = (unsigned)strtoul(val, NULL, 0);
    else if (!strcmp(key, "b_bytes"))
      r->b_bytes = (unsigned)strtoul(val, NULL, 0);
    else if (!strcmp(key, "c_bytes"))
      r->c_bytes = (unsigned)strtoul(val, NULL, 0);
    else if (!strcmp(key, "bias_file"))
      snprintf(r->bias_file, sizeof(r->bias_file), "%s", val);
    else if (!strcmp(key, "a_file"))
      snprintf(r->a_file, sizeof(r->a_file), "%s", val);
    else if (!strcmp(key, "b_file"))
      snprintf(r->b_file, sizeof(r->b_file), "%s", val);
    else if (!strcmp(key, "c_file"))
      snprintf(r->c_file, sizeof(r->c_file), "%s", val);
    else {
      printf("FAIL unknown recipe key %s\n", key);
      fclose(f);
      return -1;
    }
  }
  fclose(f);
  if (r->a_bytes > kMaxTile || r->b_bytes > kMaxTile || r->c_bytes > kMaxTile ||
      r->bias_bytes > kMaxTile) {
    puts("FAIL tile/bias too large");
    return -1;
  }
  return 0;
}

int main(void) {
  Recipe recipe;
  if (load_recipe(&recipe))
    return 3;

  void *vtcm = HAP_request_VTCM(kVtcmBytes, /*single_page=*/1);
  if (!vtcm) {
    puts("FAIL HAP_request_VTCM");
    return 2;
  }

  uint8_t *base =
      (uint8_t *)((((uintptr_t)vtcm) + (kAlign - 1u)) & ~((uintptr_t)(kAlign - 1u)));
  void *bias = base;
  void *A = base + kAlign;
  void *B = base + 2 * kAlign;
  void *C = base + 3 * kAlign;
  uint8_t expected[kMaxTile];

  if (recipe.has_bias) {
    if (read_file(recipe.bias_file, bias, recipe.bias_bytes))
      return 3;
  } else {
    memset(bias, 0, recipe.bias_bytes ? recipe.bias_bytes : 256);
  }
  if (read_file(recipe.a_file, A, recipe.a_bytes) ||
      read_file(recipe.b_file, B, recipe.b_bytes) ||
      read_file(recipe.c_file, expected, recipe.c_bytes)) {
    return 3;
  }
  memset(C, 0, recipe.c_bytes);

  int rc = qurt_hmx_lock2(QURT_HMX_NON_SHARED_LOCK);
  if (rc != 0) {
    printf("FAIL qurt_hmx_lock2 rc=%d\n", rc);
    return 1;
  }

  hmx_mac_tile(bias, A, B, C);

  qurt_hmx_unlock2(QURT_HMX_NON_SHARED_UNLOCK);

  unsigned mism = 0;
  const uint8_t *got = (const uint8_t *)C;
  for (unsigned i = 0; i < recipe.c_bytes; ++i) {
    if (got[i] != expected[i])
      ++mism;
  }
  unsigned cs = checksum_u16(C, recipe.c_bytes);
  if (mism) {
    printf("FAIL mismatch_bytes=%u checksum=0x%x\n", mism, cs);
    return 4;
  }
  printf("PASS 0x%x\n", cs);
  fflush(stdout);
  return 0;
}
