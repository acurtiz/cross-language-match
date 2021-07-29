#include <map>
#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <boost/format.hpp>
#include "file_word_loader.h"

namespace cross_language_match {

FileWordLoader::FileWordLoader(std::string file_path) {
  file_path_ = file_path;
}

std::map<std::string, std::string> *FileWordLoader::GetWordPairs() {

  std::map<std::string, std::string> *word_pairs = new std::map<std::string, std::string>();

  std::string line;
  std::ifstream file(file_path_);
  if (!file.is_open()) {
    throw std::runtime_error(boost::str(boost::format("Unable to open file %1") % file_path_));
  }

  ExtractLineFromStreamIntoMap(&file, word_pairs);

  file.close();

  return word_pairs;

}

}
