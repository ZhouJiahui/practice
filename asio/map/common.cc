#include "common.h"

void StringSplit(const std::string& str, const std::string& delimiter, std::vector<std::string>& words) {
  size_t pos1 = 0;
  size_t pos2 = 0;
  while((pos2 = str.find(delimiter, pos1)) != std::string::npos) {
    words.push_back(str.substr(pos1, pos2 - pos1));
    pos1 = pos2 + delimiter.size(); 
  } 
  if (!str.empty()) {
    words.push_back(str.substr(pos1, pos2));
  }
}

bool StringStartWith(const std::string& str, const std::string& word) {
  size_t i = 0;
  for(; i < word.size(); ++i) {
    if (i >= str.size() || str[i] != word[i]) {
      break;
    }
  }
  return i == word.size();
}
