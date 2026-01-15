# Source Layout and Tools

This folder holds the implementation for each helper tool and its reusable
library logic. For each tool, the code is split into two pieces:

- `*_main.cpp`: command-line interface (argument parsing, error handling).
- `*.cpp`: reusable library logic that can be called from other analyses.

The public headers live under `include/hep_helper/` and are the entry points
for linking against the library.

## Tools

`hep_list_root_fields` writes branch names and types to a CSV file.

```sh
./build/hep_list_root_fields file.root output.csv [tree_name]
```

Notes:

- `tree_name` defaults to `Events`.
- Output columns: `tree,branch,type`.

`hep_dump_root_event` writes the first (0th) event to a text file.

```sh
./build/hep_dump_root_event file.root output.txt [tree_name]
```

Notes:

- `tree_name` defaults to `Events`.
- Output lines follow the pattern `field (type) = value`.
