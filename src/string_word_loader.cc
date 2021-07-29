#include <map>
#include <string>
#include <sstream>
#include <boost/format.hpp>
#include "string_word_loader.h"

namespace cross_language_match {

StringWordLoader::StringWordLoader(std::string raw_string) {
  raw_string_ = raw_string;
}
std::map<std::string, std::string> *StringWordLoader::GetWordPairs() {

  std::map<std::string, std::string> *word_pairs = new std::map<std::string, std::string>();

  std::stringstream stream = std::stringstream();
  stream.str(raw_string_);

  ExtractLineFromStreamIntoMap(&stream, word_pairs);

  return word_pairs;
}

}