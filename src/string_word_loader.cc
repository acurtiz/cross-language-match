#include <map>
#include <sstream>
#include "string_word_loader.h"

namespace cross_language_match {

StringWordLoader::StringWordLoader(std::string raw_string) {
  raw_string_ = raw_string;
  stringstream_ = nullptr;
}

StringWordLoader::~StringWordLoader() {
  CloseInputStream();
}

std::istream &StringWordLoader::OpenInputStream() {
  stringstream_ = new std::stringstream();
  stringstream_->str(raw_string_);
  return *stringstream_;

}

void StringWordLoader::CloseInputStream() {
  delete stringstream_;
  stringstream_ = nullptr;
}

}