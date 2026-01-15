#include "hep_helper/root_fields.h"

#include <fstream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include "TBranch.h"
#include "TClass.h"
#include "TDirectory.h"
#include "TFile.h"
#include "TKey.h"
#include "TLeaf.h"
#include "TTree.h"

namespace hep_helper {
namespace {

void CollectTrees(TDirectory* dir, std::vector<TTree*>* out) {
  if (!dir || !out) {
    return;
  }

  TIter next_key(dir->GetListOfKeys());
  while (auto* key = static_cast<TKey*>(next_key())) {
    const char* class_name = key->GetClassName();
    TClass* cls = TClass::GetClass(class_name);
    if (!cls) {
      continue;
    }

    if (cls->InheritsFrom(TTree::Class())) {
      TTree* tree = nullptr;
      dir->GetObject(key->GetName(), tree);
      if (tree) {
        out->push_back(tree);
      }
    }

    if (cls->InheritsFrom(TDirectory::Class())) {
      TDirectory* subdir = nullptr;
      dir->GetObject(key->GetName(), subdir);
      if (subdir) {
        CollectTrees(subdir, out);
      }
    }
  }
}

std::string EscapeCsv(const std::string& value) {
  if (value.find_first_of(",\"\n\r") == std::string::npos) {
    return value;
  }

  std::string escaped;
  escaped.reserve(value.size() + 2);
  escaped.push_back('"');
  for (char ch : value) {
    if (ch == '"') {
      escaped.push_back('"');
      escaped.push_back('"');
    } else {
      escaped.push_back(ch);
    }
  }
  escaped.push_back('"');
  return escaped;
}

std::string BranchType(TBranch* branch) {
  if (!branch) {
    return "unknown";
  }

  const char* class_name = branch->GetClassName();
  if (class_name && class_name[0] != '\0') {
    return class_name;
  }

  const TLeaf* leaf = branch->GetLeaf(branch->GetName());
  if (!leaf) {
    auto* leaves = branch->GetListOfLeaves();
    if (leaves && leaves->GetEntries() > 0) {
      leaf = static_cast<const TLeaf*>(leaves->At(0));
    }
  }

  if (leaf) {
    const char* type_name = leaf->GetTypeName();
    if (type_name && type_name[0] != '\0') {
      return type_name;
    }
  }

  return "unknown";
}

}  // namespace

void WriteRootFieldsCsv(const std::string& file_path,
                        const std::string& output_path,
                        const std::string& tree_name) {
  std::unique_ptr<TFile> file(TFile::Open(file_path.c_str(), "READ"));
  if (!file || file->IsZombie()) {
    throw std::runtime_error("Failed to open ROOT file: " + file_path);
  }

  std::vector<TTree*> trees;
  if (!tree_name.empty()) {
    TTree* tree = nullptr;
    file->GetObject(tree_name.c_str(), tree);
    if (!tree) {
      throw std::runtime_error("TTree not found: " + tree_name +
                               " in file: " + file_path);
    }
    trees.push_back(tree);
  } else {
    CollectTrees(file.get(), &trees);
  }

  if (trees.empty()) {
    throw std::runtime_error("No TTree found in file: " + file_path);
  }

  std::ofstream out(output_path);
  if (!out) {
    throw std::runtime_error("Failed to open output file: " + output_path);
  }

  out << "tree,branch,type\n";

  for (TTree* tree : trees) {
    if (!tree) {
      continue;
    }

    auto* branches = tree->GetListOfBranches();
    if (!branches) {
      continue;
    }

    TIter next_branch(branches);
    while (auto* obj = next_branch()) {
      auto* branch = static_cast<TBranch*>(obj);
      out << EscapeCsv(tree->GetName()) << ','
          << EscapeCsv(branch->GetName()) << ','
          << EscapeCsv(BranchType(branch)) << '\n';
    }
  }
}

}  // namespace hep_helper
