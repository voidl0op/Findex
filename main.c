#include "crawl.h"

int main(void) {
  int code = crawling("files.txt");
  if (code != 0)
    fprintf(stderr, "finished with error code %d\n", code);
  return code != 0;
}
