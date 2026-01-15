#include "hep_helper/root_fields.h"

#include <exception>
#include <iostream>
#include <string>

int main(int argc, char** argv) {
  if (argc < 3) {
    std::cerr << "Usage: hep_list_root_fields file.root output.csv [tree_name]\n";
    return 2;
  }

  std::string tree_name = "Events";
  if (argc > 3) {
    tree_name = argv[3];
  }

  try {
    hep_helper::WriteRootFieldsCsv(argv[1], argv[2], tree_name);
  } catch (const std::exception& ex) {
    std::cerr << "Error: " << ex.what() << "\n";
    return 1;
  }

  return 0;
}
