#pragma once

#include <string>

namespace hep_helper {

void WriteRootFieldsCsv(const std::string& file_path,
                        const std::string& output_path,
                        const std::string& tree_name = "Events");

}  // namespace hep_helper
