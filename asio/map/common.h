#ifndef PRACTISE_ASIO_COMMON_H_
#define PRACTISE_ASIO_COMMON_H_
#include <string>
#include <vector>

const static std::string GET = "GET";
const static std::string SET = "SET";
const std::string DELIMITER = "*";
const int MAX_LENGTH = 128;

void StringSplit(const std::string& str, const std::string& delimiter, std::vector<std::string>& words);
 
bool StringStartWith(const std::string& str, const std::string& word);

#endif // PRACTISE_ASIO_COMMON_H_
