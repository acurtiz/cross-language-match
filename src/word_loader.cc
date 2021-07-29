#include <string>
#include <map>
#include <boost/format.hpp>
#include "word_loader.h"

namespace cross_language_match {

std::map<std::string, std::string> *WordLoader::GetWordPairs() {

  std::map<std::string, std::string> *word_pairs = new std::map<std::string, std::string>();

  std::istream &input_stream = OpenInputStream();

  std::string line;
  while (getline(input_stream, line)) {
    std::size_t comma_occurrences = std::count(line.begin(), line.end(), ',');
    if (comma_occurrences != 1) {
      throw std::runtime_error(
          boost::str(boost::format(
              "Expected one comma per line, found line with %1 occurrences. Line: '%2'") % comma_occurrences % line)
      );
    }

    std::size_t comma_index = line.find(',');
    if (comma_index == std::string::npos) {
      throw std::runtime_error(
          boost::str(boost::format("Improperly formatted file, line without comma found: %1") % line)
      );
    }

    std::string left_word = line.substr(0, comma_index);
    std::string right_word = line.substr(comma_index + 1);

    word_pairs->insert(std::pair<std::string, std::string>(left_word, right_word));

  }

  CloseInputStream();

  return word_pairs;

}

}

