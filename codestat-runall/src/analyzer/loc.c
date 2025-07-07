#include <stdio.h>
#include <ctype.h>
#include "loc.h"

LOCResult count_loc(const char *filepath) {
    FILE *file = fopen(filepath, "r");
    LOCResult result = {0};

    if (!file) {
        perror("fopen failed");
        return result;
    }

    char line[1024];

    while (fgets(line, sizeof(line), file)) {
        result.total_lines++;

        int only_whitespace = 1;
        for (int i = 0; line[i] != '\0'; i++) {
            if (!isspace((unsigned char)line[i])) {
                only_whitespace = 0;
                break;
            }
        }

        if (only_whitespace) {
            result.blank_lines++;
        } else {
            result.code_lines++;
        }
    }

    fclose(file);
    return result;
}
