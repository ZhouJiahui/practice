#include <iostream>
#include <string>

void operator"" _hello(char const* s, std::size_t l) {
  std::cout << std::string(s, l) << std::endl;
}

void operator"" _world(long double v) {
  std::cout << v << std::endl;
} 

int main() {
  "abc"_hello;
  1.23_world;
}
