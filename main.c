#include <stdio.h>
#include <stdlib.h>
#include "file.h"

int main() {
  char file[256];
  printf("Enter the file wanted: ");
  find_path(fgets(file, sizeof(file), stdin));
  return 0;
}
