#include <iostream>
#include <unistd.h>

int main() {
  const char *link = "/proc/self/exe";
  char buf[1024];
  int ret = readlink(link, buf, 1024);
  if (ret != -1) {
    std::cout << buf << std::endl;
  }
  return 0;
}
