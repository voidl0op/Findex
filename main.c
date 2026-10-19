#include <stdio.h>
#include <stdlib.h>
#include "file.h"

int main() {
  char file[256];
  FILE* fp = fopen("text.txt", "r");
  printf("Enter the file wanted: ");
  find_path(fgets(file, sizeof(file), stdin));
  open_path(get_path(fp, 1));
  return 0;
}
