// function_count.c
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "function_count.h"

int count_functions(const char *filepath) {
    FILE *file = fopen(filepath, "r");
    if (!file) return 0;

    int count = 0;
    char line[1024];

    while (fgets(line, sizeof(line), file)) {
        // Skip lines that likely aren't function declarations
        if (strstr(line, "if (") || strstr(line, "for (") || 
            strstr(line, "while (") || strstr(line, "switch ("))
            continue;

        // Check if line contains '(', ')', and '{' suggesting a function definition
        if (strchr(line, '(') && strchr(line, ')') && strchr(line, '{')) {
            count++;
        }
    }

    fclose(file);
    return count;
}

const char* get_blast_radius_label(int func_count) {
    if (func_count <= 2) return "LOW";
    if (func_count <= 10) return "MEDIUM";
    return "HIGH";
}
