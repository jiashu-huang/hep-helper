#include "hep_helper/root_event.h"

#include <fstream>
#include <memory>
#include <stdexcept>
#include <string>

#include "TBranch.h"
#include "TCollection.h"
#include "TFile.h"
#include "TLeaf.h"
#include "TTree.h"

namespace hep_helper {
namespace {

std::string LeafDisplayName(const TBranch* branch, const TLeaf* leaf) {
  if (!leaf) {
    return "unknown";
  }

  const char* leaf_name = leaf->GetName();
  if (!leaf_name || leaf_name[0] == '\0') {
    return "unknown";
  }

  if (!branch) {
    return leaf_name;
  }

  const char* branch_name = branch->GetName();
  if (branch_name && branch_name[0] != '\0' &&
      std::string(leaf_name) != branch_name) {
    return std::string(branch_name) + "." + leaf_name;
  }

  return leaf_name;
}

std::string LeafTypeName(const TLeaf* leaf) {
  if (!leaf) {
    return "unknown";
  }

  const char* type_name = leaf->GetTypeName();
  if (type_name && type_name[0] != '\0') {
    return type_name;
  }

  return "unknown";
}

std::string LeafValueString(TLeaf* leaf) {
  if (!leaf) {
    return "";
  }

  int ndata = leaf->GetNdata();
  if (ndata <= 1) {
    const char* value = leaf->GetValueString(0);
    return value ? value : "";
  }

  std::string result = "[";
  for (int i = 0; i < ndata; ++i) {
    if (i > 0) {
      result += ", ";
    }
    const char* value = leaf->GetValueString(i);
    if (value) {
      result += value;
    }
  }
  result += "]";
  return result;
}

}  // namespace

void WriteFirstRootEventTxt(const std::string& file_path,
                            const std::string& output_path,
                            const std::string& tree_name) {
  std::unique_ptr<TFile> file(TFile::Open(file_path.c_str(), "READ"));
  if (!file || file->IsZombie()) {
    throw std::runtime_error("Failed to open ROOT file: " + file_path);
  }

  TTree* tree = nullptr;
  file->GetObject(tree_name.c_str(), tree);
  if (!tree) {
    throw std::runtime_error("TTree not found: " + tree_name +
                             " in file: " + file_path);
  }

  if (tree->GetEntries() <= 0) {
    throw std::runtime_error("TTree is empty: " + tree_name +
                             " in file: " + file_path);
  }

  tree->GetEntry(0);

  std::ofstream out(output_path);
  if (!out) {
    throw std::runtime_error("Failed to open output file: " + output_path);
  }

  out << "Tree: " << tree->GetName() << "\n";
  out << "Entry: 0\n";

  auto* branches = tree->GetListOfBranches();
  if (!branches) {
    return;
  }

  TIter next_branch(branches);
  while (auto* obj = next_branch()) {
    auto* branch = static_cast<TBranch*>(obj);
    if (!branch) {
      continue;
    }

    auto* leaves = branch->GetListOfLeaves();
    if (!leaves || leaves->GetEntries() == 0) {
      const char* class_name = branch->GetClassName();
      if (class_name && class_name[0] != '\0') {
        out << branch->GetName() << " (" << class_name << ") = <object>\n";
      } else {
        out << branch->GetName() << " = <no leaves>\n";
      }
      continue;
    }

    TIter next_leaf(leaves);
    while (auto* leaf_obj = next_leaf()) {
      auto* leaf = static_cast<TLeaf*>(leaf_obj);
      out << LeafDisplayName(branch, leaf) << " ("
          << LeafTypeName(leaf) << ") = " << LeafValueString(leaf) << "\n";
    }
  }
}

}  // namespace hep_helper
