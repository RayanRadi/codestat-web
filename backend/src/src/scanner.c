// scanner.c
// Recursively scans directories and sends .c/.h files to analyzer.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

#include "scanner.h"
#include "analyzer/analyzer.h"

void scan_directory(const char *path) {
    DIR *dir = opendir(path);
    if (!dir) {
        perror("opendir");
        return;
    }

    struct dirent *entry;
    char fullpath[1024];

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        snprintf(fullpath, sizeof(fullpath), "%s/%s", path, entry->d_name);

        struct stat statbuf;
        if (stat(fullpath, &statbuf) == -1) {
            perror("stat");
            continue;
        }

        if (S_ISDIR(statbuf.st_mode)) {
            scan_directory(fullpath);  // Recurse into subdirectories
        } else if (S_ISREG(statbuf.st_mode)) {
            if (strstr(fullpath, ".c") || strstr(fullpath, ".h") || strstr(fullpath, ".java")) {
                AnalysisResult dummy = {0};
                analyze_file(fullpath, &dummy);  // LOC handled in analyzer
            }
        }
    }

    closedir(dir);
}
