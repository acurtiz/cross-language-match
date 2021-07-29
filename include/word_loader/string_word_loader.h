#include <map>
#include <string>
#include <sstream>
#include <istream>
#include "word_loader.h"

#ifndef CROSSLANGUAGEMATCH_SRC_STRING_WORD_LOADER_H_
#define CROSSLANGUAGEMATCH_SRC_STRING_WORD_LOADER_H_

namespace cross_language_match {

class StringWordLoader : public WordLoader {
 public:
  StringWordLoader(std::string raw_string);
  ~StringWordLoader();
 protected:
  std::istream &OpenInputStream() override;
  void CloseInputStream() override;
 private:
  std::string raw_string_;
  std::stringstream *stringstream_;

};

}

#endif //CROSSLANGUAGEMATCH_SRC_STRING_WORD_LOADER_H_
