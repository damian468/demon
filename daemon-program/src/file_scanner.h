#ifndef FILE_SCANNER_H
#define FILE_SCANNER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <syslog.h>
#include <unistd.h>

// Function to initiate the scanning process
void scan_files(const char *path, char **fragments, int fragment_count, int verbose);

#endif // FILE_SCANNER_H