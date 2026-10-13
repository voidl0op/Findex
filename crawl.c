#define _DEFAULT_SOURCE
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#define EODIR 1
#define ECDIR -1

static int crawl(const char *path) {
    DIR *directory = opendir(path);
    if (directory == NULL) return EODIR;

    struct dirent *entry;
    while ((entry = readdir(directory)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        char full[5096];
        snprintf(full, sizeof(full), "%s/%s", path, entry->d_name);

        if (entry->d_type == DT_REG) {
            printf("%s\n", full);
        } else if (entry->d_type == DT_DIR) {
            printf("%s\n", full);
            crawl(full);
        }
    }

    if (closedir(directory) == -1) return ECDIR;
    return 0;
}

int crawling() {
    return crawl("/home");
}