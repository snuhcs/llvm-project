//===- MockSerializationFormat.cpp ----------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "Registries/MockSerializationFormat.h"
#include "clang/Analysis/Scalable/Model/BuildNamespace.h"
#include "clang/Analysis/Scalable/Model/EntityName.h"
#include "clang/Analysis/Scalable/Model/SummaryName.h"
#include "clang/Analysis/Scalable/Serialization/SerializationFormat.h"
#include "clang/Analysis/Scalable/Serialization/SerializationFormatRegistry.h"
#include "clang/Analysis/Scalable/TUSummary/TUSummary.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/Support/Error.h"
#include "llvm/Support/VirtualOutputFile.h"
#include <cassert>
#include <functional>
#include <memory>
#include <set>

using namespace clang;
using namespace ssaf;

MockSerializationFormat::MockSerializationFormat(
    llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem> FS,
    llvm::IntrusiveRefCntPtr<llvm::vfs::OutputBackend> OutputMgr)
    : SerializationFormat(FS, OutputMgr) {
  for (const auto &FormatInfoEntry : llvm::Registry<FormatInfo>::entries()) {
    std::unique_ptr<FormatInfo> Info = FormatInfoEntry.instantiate();
    bool Inserted = FormatInfos.try_emplace(Info->ForSummary, *Info).second;
    if (!Inserted) {
      llvm::report_fatal_error(
          "Format info was already registered for summary name: " +
          Info->ForSummary.str());
    }
  }
}

TUSummary MockSerializationFormat::readTUSummary(llvm::StringRef Path) {
  BuildNamespace NS(BuildNamespaceKind::CompilationUnit, "Mock.cpp");
  TUSummary Summary(NS);

  auto ManifestFile = FS->getBufferForFile(Path + "/analyses.txt");
  assert(ManifestFile); // TODO Handle error.
  llvm::StringRef ManifestFileContent = (*ManifestFile)->getBuffer();

  llvm::SmallVector<llvm::StringRef, 5> Analyses;
  ManifestFileContent.split(Analyses, /*Separator=*/"\n", /*MaxSplit=*/-1,
                            /*KeepEmpty=*/false);

  for (llvm::StringRef Analysis : Analyses) {
    SummaryName Name(Analysis.str());
    auto InputFile = FS->getBufferForFile(Path + "/" + Name.str() + ".special");
    assert(InputFile);
    auto InfoIt = FormatInfos.find(Name);
    if (InfoIt == FormatInfos.end()) {
      llvm::report_fatal_error(
          "No FormatInfo was registered for summary name: " + Name.str());
    }
    const auto &InfoEntry = InfoIt->second;
    assert(InfoEntry.ForSummary == Name);

    SpecialFileRepresentation Repr{(*InputFile)->getBuffer().str()};
    auto &Table = getIdTableForDeserialization(Summary);

    std::unique_ptr<EntitySummary> Result = InfoEntry.Deserialize(Repr, Table);
    if (!Result) // TODO: Handle error.
      continue;

    EntityId FooId = Table.getId(EntityName{"c:@F@foo", "", /*Namespace=*/{}});
    auto &IdMappings = getData(Summary).try_emplace(Name).first->second;
    [[maybe_unused]] bool Inserted =
        IdMappings.try_emplace(FooId, std::move(Result)).second;
    assert(Inserted);
  }

  llvm::vfs::OutputFile LogFile = llvm::cantFail(OutputMgr->createFile("log"));
  LogFile << "finished reading summaries\n";
  llvm::cantFail(LogFile.keep());
  return Summary;
}

void MockSerializationFormat::writeTUSummary(const TUSummary &Summary,
                                             llvm::StringRef OutputDir) {
  llvm::vfs::OutputFile LogFile = llvm::cantFail(OutputMgr->createFile("log"));

  std::set<SummaryName> Analyses;
  for (const auto &[SummaryName, EntityMappings] : getData(Summary)) {
    [[maybe_unused]] bool Inserted = Analyses.insert(SummaryName).second;
    assert(Inserted);
    for (const auto &Data : llvm::make_second_range(EntityMappings)) {
      auto InfoIt = FormatInfos.find(SummaryName);
      if (InfoIt == FormatInfos.end()) {
        llvm::report_fatal_error(
            "There was no FormatInfo registered for summary name '" +
            SummaryName.str() + "'");
      }
      const auto &InfoEntry = InfoIt->second;
      assert(InfoEntry.ForSummary == SummaryName);

      auto Output = InfoEntry.Serialize(*Data, *this);

      llvm::vfs::OutputFile AnalysisOutputFile =
          llvm::cantFail(OutputMgr->createFile(OutputDir + "/" +
                                               SummaryName.str() + ".special"));
      AnalysisOutputFile << Output.MockRepresentation;
      llvm::cantFail(AnalysisOutputFile.keep());
      LogFile << "written summary for " << SummaryName.str() << "\n";
    }
  }
  llvm::vfs::OutputFile ManifestFile =
      llvm::cantFail(OutputMgr->createFile(OutputDir + "/analyses.txt"));

  interleave(map_range(Analyses, std::mem_fn(&SummaryName::str)), ManifestFile,
             "\n");
  ManifestFile << "\n";
  llvm::cantFail(ManifestFile.keep());

  LogFile << "finished writing summaries\n";
  llvm::cantFail(LogFile.keep());
}

static SerializationFormatRegistry::Add<MockSerializationFormat>
    RegisterFormat("MockSerializationFormat",
                   "A serialization format for testing");
