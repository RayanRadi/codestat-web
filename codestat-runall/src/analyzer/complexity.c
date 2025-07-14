#define _GNU_SOURCE
#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

#define MAX_LINE 1024

static char worst_complexity[64] = "O(1)";

const char* get_worst_complexity(const char *filepath) {
    char command[1024];
    snprintf(command, sizeof(command),
        "PYTHONPATH=$HOME/.local/lib/python3.10/site-packages /usr/bin/python3 \"/home/rayan/Personal Projects/codestat-web/backend/ai_complexity.py\" \"%s\"",
        filepath
    );

    FILE *fp = popen(command, "r");
    if (!fp) {
        perror("popen failed");
        strcpy(worst_complexity, "AI_ERROR");
        return worst_complexity;
    }

    if (fgets(worst_complexity, sizeof(worst_complexity), fp) == NULL) {
        strcpy(worst_complexity, "AI_ERROR");
    }

    pclose(fp);
    worst_complexity[strcspn(worst_complexity, "\n")] = 0;

    // Fallback if AI output is empty
    if (strlen(worst_complexity) == 0) {
        strcpy(worst_complexity, "O(1)");
    }

    return worst_complexity;
}
