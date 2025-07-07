#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include "scanner.h"
#include "analyzer/analyzer.h"
#include "analyzer/complexity.h"
#include "analyzer/loc.h"
#include "analyzer/function_count.h"

static void to_lowercase(char *str) {
    for (int i = 0; str[i]; i++) str[i] = tolower((unsigned char)str[i]);
}

// Recursively search for a file under any Test-Subject subfolder
static int find_and_analyze_file(const char *start_dir, const char *target_name) {
    DIR *dir = opendir(start_dir);
    if (!dir) return 0;

    struct dirent *entry;
    char path[1024];

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;

        snprintf(path, sizeof(path), "%s/%s", start_dir, entry->d_name);

        struct stat statbuf;
        if (stat(path, &statbuf) == -1) continue;

        if (S_ISDIR(statbuf.st_mode)) {
            if (strstr(entry->d_name, "Test-Subject") || strstr(start_dir, "Test-Subject")) {
                if (find_and_analyze_file(path, target_name)) {
                    closedir(dir);
                    return 1;
                }
            } else {
                find_and_analyze_file(path, target_name);
            }
        } else if (S_ISREG(statbuf.st_mode)) {
            if (strcmp(entry->d_name, target_name) == 0) {
                LOCResult result = count_loc(path);
                const char* worst = get_worst_complexity(path);
                int function_count = count_functions(path);
                const char* blast_label = get_blast_radius_label(function_count);

                printf("{\n");
                printf("  \"file\": \"%s\",\n", path);
                printf("  \"total_lines\": %d,\n", result.total_lines);
                printf("  \"code_lines\": %d,\n", result.code_lines);
                printf("  \"blank_lines\": %d,\n", result.blank_lines);
                printf("  \"functions\": %d,\n", function_count);
                printf("  \"blast_radius\": \"%s\",\n", blast_label);
                printf("  \"worst_complexity\": \"%s\"\n", worst);
                printf("}\n");

                closedir(dir);
                return 1;
            }
        }
    }

    closedir(dir);
    return 0;
}

void run_command_interface() {
    char input[256];

    printf("\ncodestat interactive mode (type 'help' for commands, 'exit' to quit)\n");

    while (1) {
        printf("> ");
        if (!fgets(input, sizeof(input), stdin)) break;

        input[strcspn(input, "\n")] = 0;
        char input_copy[256];
        strcpy(input_copy, input);
        to_lowercase(input_copy);

        if (strcmp(input_copy, "exit") == 0) {
            break;

        } else if (strcmp(input_copy, "help") == 0) {
            printf("Available commands:\n");
            printf("  scanfile <filename> - Analyze a specific file buried in any Test-Subject folder\n");
            printf("  help                - Show this help message\n");
            printf("  exit                - Exit the program\n");

        } else if (strncmp(input_copy, "scanfile ", 9) == 0) {
            char *filename = input + 9;
            if (!find_and_analyze_file(".", filename)) {
                printf("File '%s' not found in any Test-Subject folder.\n", filename);
            }

        } else {
            printf("Unknown command: %s\n", input);
        }
    }
}
