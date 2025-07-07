#include <stdio.h>
#include <string.h>
#include "analyzer.h"
#include "loc.h"
#include "complexity.h"
#include "function_count.h"

static int total_loc = 0;
static int files_scanned = 0;
static FILE *json_out = NULL;

void analyze_file(const char *filepath, AnalysisResult *result) {
    LOCResult loc = count_loc(filepath);
    result->total_lines = loc.total_lines;
    result->code_lines = loc.code_lines;
    result->blank_lines = loc.blank_lines;

    int function_count = count_functions(filepath);
    const char* blast = get_blast_radius_label(function_count);
    const char* worst = get_worst_complexity(filepath);

    if (!json_out) {
        json_out = fopen("report.json", "w");
        fprintf(json_out, "[\n");
    }

    if (files_scanned > 0)
        fprintf(json_out, ",\n");

    fprintf(json_out,
        "  {\n"
        "    \"file\": \"%s\",\n"
        "    \"total_lines\": %d,\n"
        "    \"code_lines\": %d,\n"
        "    \"blank_lines\": %d,\n"
        "    \"functions\": %d,\n"
        "    \"blast_radius\": \"%s\",\n"
        "    \"worst_complexity\": \"%s\"\n"
        "  }",
        filepath, loc.total_lines, loc.code_lines, loc.blank_lines,
        function_count, blast, worst
    );

    total_loc += loc.total_lines;
    files_scanned++;
}

void print_total_loc_to_json(const char *summary_path) {
    if (json_out) {
        fprintf(json_out, ",\n  {\n");
        fprintf(json_out,
            "    \"summary\": {\n"
            "      \"total_loc\": %d,\n"
            "      \"files_scanned\": %d\n"
            "    }\n"
            "  }\n");
        fprintf(json_out, "]\n");
        fclose(json_out);
        json_out = NULL;
    }
}
