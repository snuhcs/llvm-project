//===- SummaryExtractorRegistryTest.cpp -----------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "clang/Analysis/Scalable/Serialization/SerializationFormatRegistry.h"
#include "clang/Analysis/Scalable/TUSummary/TUSummary.h"
#include "llvm/ADT/IntrusiveRefCntPtr.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/Support/VirtualFileSystem.h"
#include "llvm/Support/VirtualOutputBackends.h"
#include "gtest/gtest.h"
#include <memory>

using namespace llvm;
using namespace clang;
using namespace ssaf;

static auto valuesOf(
    decltype(std::declval<vfs::InMemoryOutputBackend>().getBuffers()) Range) {
  std::map<std::string, std::string> Result;
  for (const auto &[K, V] : Range) {
    [[maybe_unused]] bool Inserted =
        Result.try_emplace(K, V.str().str()).second;
    assert(Inserted);
  }
  return Result;
}

static auto valuesOf(ArrayRef<std::pair<StringRef, StringRef>> Range) {
  std::map<std::string, std::string> Result;
  for (const auto &[K, V] : Range) {
    [[maybe_unused]] bool Inserted =
        Result.try_emplace(K.str(), V.str()).second;
    assert(Inserted);
  }
  return Result;
}

namespace {

TEST(SerializationFormatRegistryTest, isFormatRegistered) {
  EXPECT_FALSE(isFormatRegistered("Non-existent-format"));
  EXPECT_TRUE(isFormatRegistered("MockSerializationFormat"));
}

TEST(SerializationFormatRegistryTest, EnumeratingRegistryEntries) {
  auto Formats = SerializationFormatRegistry::entries();
  ASSERT_EQ(std::distance(Formats.begin(), Formats.end()), 1U);
  EXPECT_EQ(Formats.begin()->getName(), "MockSerializationFormat");
}

TEST(SerializationFormatRegistryTest, Roundtrip) {
  StringLiteral FancyAnalysisFileData = "FancyAnalysisData{\n"
                                        "  SomeInternalList: zed, vayne, lux\n"
                                        "}\n";

  auto Inputs = makeIntrusiveRefCnt<vfs::InMemoryFileSystem>();
  Inputs->addFile("input/analyses.txt", /*ModificationTime=*/{},
                  MemoryBuffer::getMemBufferCopy("FancyAnalysis\n"));
  Inputs->addFile("input/FancyAnalysis.special", /*ModificationTime=*/{},
                  MemoryBuffer::getMemBufferCopy(FancyAnalysisFileData));
  auto Outputs = makeIntrusiveRefCnt<vfs::InMemoryOutputBackend>();

  std::unique_ptr<SerializationFormat> Format =
      makeFormat(Inputs, Outputs, "MockSerializationFormat");
  ASSERT_TRUE(Format);

  TUSummary LoadedSummary = Format->readTUSummary("input");
  Format->writeTUSummary(LoadedSummary, "output");

  EXPECT_EQ(valuesOf({
                {"log", "finished reading summaries\n"
                        "written summary for FancyAnalysis\n"
                        "finished writing summaries\n"},
                {"output/FancyAnalysis.special", FancyAnalysisFileData},
                {"output/analyses.txt", "FancyAnalysis\n"},
            }),
            valuesOf(Outputs->getBuffers()));
}

} // namespace
