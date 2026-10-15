#define _DEFAULT_SOURCE
#include <stdio.h>
#include <dirent.h>
#include <string.h>

#define EODIR 1   /* Error opening dir */
#define ECDIR -1  /* Error closing dir */
#define EOFILE 2  /* Error opening output file */

static int crawler(const char* path, FILE* out) {
  DIR* directory = opendir(path);
  if (directory == NULL) return EODIR;

  int code = 0;
  struct dirent* entry;
  while ((entry = readdir(directory)) != NULL) {
    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;

    char full[4096];
    snprintf(full, sizeof(full), "%s/%s", path, entry->d_name);

    if (entry->d_type == DT_REG) {
      fprintf(out, "%s\n", full);
    } else if (entry->d_type == DT_DIR) {
      fprintf(out, "%s\n", full);
      int rc = crawler(full, out);
      if (rc != 0 && code == 0) code = rc;
    }
  }
  if (closedir(directory) == -1 && code == 0) code = ECDIR;
  return code;
}

int crawling(const char* outfile) {
  FILE* out = fopen(outfile, "w");
  if (out == NULL) return EOFILE;
  int code = crawler("/home", out);
  if (fclose(out) != 0 && code == 0) code = EOFILE;
  return code;
}
