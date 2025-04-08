#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <syslog.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "file_scanner.h"

static int verbose = 0;

void scan_directory(const char *dir_path, char **fragments, int fragment_count) {
    struct dirent *entry;
    DIR *dp = opendir(dir_path);

    if (dp == NULL) {
        syslog(LOG_ERR, "Could not open directory: %s", dir_path);
        return;
    }

    while ((entry = readdir(dp)) != NULL) {
        if (entry->d_type == DT_DIR) {
            if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
                char new_path[PATH_MAX];
                snprintf(new_path, sizeof(new_path), "%s/%s", dir_path, entry->d_name);
                scan_directory(new_path, fragments, fragment_count);
            }
        } else {
            for (int i = 0; i < fragment_count; i++) {
                if (strstr(entry->d_name, fragments[i]) != NULL) {
                    syslog(LOG_INFO, "Found: %s/%s", dir_path, entry->d_name);
                    if (verbose) {
                        printf("Found: %s/%s\n", dir_path, entry->d_name);
                    }
                    break;
                }
            }
        }
    }

    closedir(dp);
}

void initiate_scan(char **fragments, int fragment_count) {
    const char *root_path = "/";
    scan_directory(root_path, fragments, fragment_count);
}

void set_verbosity(int level) {
    verbose = level;
}