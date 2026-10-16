#include <stdio.h>
#include <stdlib.h>
#include "file.h"

int main() {
  char file[256];
  printf("Enter the file wanted: ");
  printf("%s\n",find_path(fgets(file, sizeof(file), stdin)));
  open_path(find_path(file));
  return 0;
}
