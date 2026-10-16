#include <stdio.h>
#include <stdlib.h>

char* find_path(const char* file) {
  char cmd[4096];
  char* buffer = malloc(4096);
  snprintf(cmd, sizeof(cmd), "/bin/find /home -name %s", file);
  FILE *fp = popen(cmd, "r");
  fgets(buffer, 4096, fp);
  pclose(fp);
  return buffer;
}

void open_path(const char* path) {
  char cmd[4096];
  snprintf(cmd, sizeof(cmd), "xdg-open \"%s\"", path);
  system(cmd);
}
