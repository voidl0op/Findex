#include <stdio.h>
#include <stdlib.h>

void find_path(const char* file);
void open_path(const char* path);

int main() {
  find_path("thing.c");
  return 0;
}

void find_path(const char* file) {
  char cmd[4096];
  snprintf(cmd, sizeof(cmd), "find . -name %s", file);
  FILE *fp = popen(cmd, "r");
  pclose(fp);
}

void open_path(const char* path) {
  char cmd[4096];
  snprintf(cmd, sizeof(cmd), "xdg-open \"%s\"", path);
  system(cmd);
}
