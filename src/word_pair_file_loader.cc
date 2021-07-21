#include <map>
#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <fmt/format.h>
#include "word_pair_file_loader.h"

namespace cross_language_match {

std::map<std::string, std::string> *WordPairFileLoader::GetWordPairs(std::string file_path) {

  std::map<std::string, std::string> *word_pairs = new std::map<std::string, std::string>();

  std::string line;
  std::ifstream file(file_path);
  if (file.is_open()) {
    while (getline(file, line)) {

      std::size_t comma_occurrences = std::count(line.begin(), line.end(), ',');
      if (comma_occurrences != 1) {
        throw std::runtime_error(fmt::format("Expected one comma per line, found line with {} occurrences. "
                                             "Line: '{}'",
                                             comma_occurrences,
                                             line));
      }

      std::size_t comma_index = line.find(',');
      if (comma_index == std::string::npos) {
        throw std::runtime_error(fmt::format("Improperly formatted file, line without comma found: {}", line));
      }

      std::string left_word = line.substr(0, comma_index);
      std::string right_word = line.substr(comma_index + 1);

      word_pairs->insert(std::pair<std::string, std::string>(left_word, right_word));

    }
    file.close();
  } else {
    throw std::runtime_error(fmt::format("Unable to open file {}", file_path));
  }

  return word_pairs;

}

}
