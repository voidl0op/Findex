#include <stdio.h>
#include <stdlib.h>

void find_path(const char* file) {
  char cmd[4096];
  char* buffer = malloc(4096);
  FILE* fptr = fopen("text.txt", "w");

  snprintf(cmd, sizeof(cmd), "/bin/find /home -name %s", file);
  FILE *fp = popen(cmd, "r");

  while(fgets(buffer, 4096, fp) != NULL) fprintf(fptr, buffer);
  fclose(fptr);
  pclose(fp);
}

void open_path(const char* path) {
  char cmd[4096];
  snprintf(cmd, sizeof(cmd), "xdg-open \"%s\"", path);
  system(cmd);
}
