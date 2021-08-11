#include <fstream>
#include <algorithm>
#include <boost/format.hpp>
#include "word_loader/file_word_loader.h"

namespace cross_language_match {

FileWordLoader::FileWordLoader(std::string file_path) {
  file_path_ = file_path;
  file_stream_ = nullptr;
}

FileWordLoader::~FileWordLoader() {
  CloseInputStream();
}

std::istream &FileWordLoader::OpenInputStream() {

  file_stream_ = new std::ifstream(file_path_);
  if (!file_stream_->is_open()) {
    throw std::runtime_error(boost::str(boost::format("Unable to open file %1%") % file_path_));
  }

  return *file_stream_;

}

void FileWordLoader::CloseInputStream() {
  if (file_stream_ != nullptr) {
    file_stream_->close();
  }
  delete file_stream_;
  file_stream_ = nullptr;
}

WordLoader::InputError FileWordLoader::ParseAndLoadIntoMap() {

  std::ifstream temp_stream = std::ifstream(file_path_);
  if (!temp_stream.good()) {
    return WordLoader::InputError::FILE_NOT_FOUND;
  }

  return WordLoader::ParseAndLoadIntoMap();

}

}
