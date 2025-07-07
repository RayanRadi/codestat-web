#include <stdio.h>
#include <stdlib.h>
#include "scanner.h"
#include "analyzer/analyzer.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <directory>\n", argv[0]);
        return 1;
    }

    // Run analysis silently
    scan_directory(argv[1]);
    print_total_loc_to_json("report.json");

    return 0;
}
