#include <string>
#include <map>
#include <boost/format.hpp>
#include "word_loader/word_loader.h"

namespace cross_language_match {

WordLoader::InputError WordLoader::ParseAndLoadIntoMap() {

  word_pairs_ = std::map<std::string, std::string>();

  std::istream &input_stream = OpenInputStream();

  std::string line;
  while (getline(input_stream, line)) {
    std::size_t comma_occurrences = std::count(line.begin(), line.end(), ',');
    if (comma_occurrences == 0) {
      return WordLoader::InputError::LINE_CONTAINS_NO_COMMA;
    } else if (comma_occurrences > 1) {
      return WordLoader::InputError::LINE_CONTAINS_MORE_THAN_ONE_COMMA;
    }

    std::size_t comma_index = line.find(',');
    if (comma_index == std::string::npos) {
      throw std::runtime_error(
          boost::str(boost::format("Improperly formatted file, line without comma found: %1%") % line)
      );
    }

    std::string left_word = line.substr(0, comma_index);
    std::string right_word = line.substr(comma_index + 1);

    word_pairs_.insert(std::pair<std::string, std::string>(left_word, right_word));

  }

  CloseInputStream();

  return WordLoader::InputError::NONE;

}

std::map<std::string, std::string> WordLoader::GetWordPairMap() {
  return word_pairs_;
}

}

