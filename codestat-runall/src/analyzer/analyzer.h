#ifndef ANALYZER_H
#define ANALYZER_H

typedef struct {
    int total_lines;
    int code_lines;
    int blank_lines;
} AnalysisResult;

void analyze_file(const char *filepath, AnalysisResult *result);
void print_total_loc_to_json(const char *summary_path);

#endif
