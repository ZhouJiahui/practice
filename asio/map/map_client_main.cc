#include "map_client.h"
#include <strings.h>
#include <iostream>
#include <vector>

int main(int argc, char **argv) {
  if (argc < 3) {
    std::cerr << "Usage: map_client <ip> <port>\n";
    return 1;
  }
  try {
    std::string str;
    while (std::getline(std::cin, str)) {
      MapClient mapClient(argv[1], argv[2]);
      std::vector<std::string> words;
      StringSplit(str, " ", words);
      if (words.size() == 2 && strcasecmp(words[0].c_str(), ::GET.c_str()) == 0) {
        std::cout << mapClient.Get(words[1]) << std::endl;
      } else if (words.size() == 3 && strcasecmp(words[0].c_str(), ::SET.c_str()) == 0) {
        std::cout << mapClient.Set(words[1], words[2]) << std::endl;
      } else {
        std::cout << "unknown input." << std::endl;
      }
    }
  } catch (std::exception& e) {
    std::cerr << "Exception: " << e.what() << "\n";
  }
  return 0;

}
