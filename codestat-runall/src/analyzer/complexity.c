#define _GNU_SOURCE
#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

#define MAX_FUNCS 100
#define MAX_LINE 1024

typedef struct {
    char name[128];
    char complexity[64];
} FunctionInfo;

const char* call_ai_on_function(const char* func_body) {
    FILE *tmp = fopen("temp_func.txt", "w");
    if (!tmp) return "AI_ERROR";
    fprintf(tmp, "%s", func_body);
    fclose(tmp);

    FILE *fp = popen("PYTHONPATH=$HOME/.local/lib/python3.10/site-packages /usr/bin/python3 /home/rayan/Personal\\ Projects/codestat-web/backend/ai_complexity.py temp_func.txt", "r");
    if (!fp) return "AI_ERROR";

    static char result[128];
    if (fgets(result, sizeof(result), fp) == NULL) {
        strcpy(result, "AI_ERROR");
    }

    pclose(fp);
    remove("temp_func.txt");
    result[strcspn(result, "\n")] = 0;
    return result;
}

const char* estimate_complexity(const char *body, const char *func_name, int *needs_ai) {
    *needs_ai = 1;
    return NULL;
}

static char worst_complexity[64] = "O(1)";

const char* get_worst_complexity(const char *filepath) {
    FILE *f = fopen(filepath, "r");
    if (!f) {
        perror("fopen");
        return "ERROR";
    }

    char line[MAX_LINE];
    char func_body[10000] = {0};
    FunctionInfo functions[MAX_FUNCS];
    int func_count = 0;
    int recording = 0;
    int brace_count = 0;
    char current_func[128] = "";

    while (fgets(line, sizeof(line), f)) {
        if (!recording) {
            if (strstr(line, "(") && strstr(line, ")") && strchr(line, '{')) {
                sscanf(line, "%127s", current_func);
                recording = 1;
                brace_count = 1;
                strcpy(func_body, line);
            }
        } else {
            strcat(func_body, line);
            for (int i = 0; line[i]; i++) {
                if (line[i] == '{') brace_count++;
                if (line[i] == '}') brace_count--;
            }

            if (brace_count == 0) {
                int needs_ai = 0;
                const char *result = estimate_complexity(func_body, current_func, &needs_ai);
                if (needs_ai) {
                    fprintf(stderr, "Calling AI for function: %s\n", current_func);
                    result = call_ai_on_function(func_body);
                }

                strcpy(functions[func_count].name, current_func);
                strcpy(functions[func_count].complexity, result);
                func_count++;

                func_body[0] = '\0';
                recording = 0;
            }
        }
    }

    fclose(f);

    const char *ranking[] = {"O(1)", "O(n)", "O(n^2)", "O(2^n)", "AI_ERROR"};
    int worst_rank = 0;
    for (int i = 0; i < func_count; i++) {
        for (int r = 0; r < 5; r++) {
            if (strcmp(functions[i].complexity, ranking[r]) == 0 && r > worst_rank) {
                worst_rank = r;
            }
        }
    }

    strcpy(worst_complexity, ranking[worst_rank]);
    return worst_complexity;
}

void analyze_file_complexity(const char *filepath) {
    get_worst_complexity(filepath);
    printf("File: %s\n", filepath);
    printf("  Worst Time Complexity: %s\n", worst_complexity);
}
