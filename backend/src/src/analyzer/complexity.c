#define _GNU_SOURCE
#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static char worst_complexity[64] = "O(1)";

const char* get_worst_complexity(const char *filepath) {
    char command[1024];
    snprintf(command, sizeof(command),
        "env python3 ./ai_complexity.py \"%s\"", filepath);  // Uses env to pass API key

    FILE *fp = popen(command, "r");
    if (!fp) {
        perror("popen failed");
        return "AI_ERROR";
    }

    char buffer[128];
    if (fgets(buffer, sizeof(buffer), fp) == NULL) {
        pclose(fp);
        return "AI_ERROR";
    }

    pclose(fp);

    buffer[strcspn(buffer, "\n")] = 0; // Strip newline
    strncpy(worst_complexity, buffer, sizeof(worst_complexity) - 1);
    worst_complexity[sizeof(worst_complexity) - 1] = '\0';

    return worst_complexity;
}
