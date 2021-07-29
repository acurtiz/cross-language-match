#include <map>
#include <string>
#include "word_loader.h"

#ifndef CROSSLANGUAGEMATCH_SRC_STRING_WORD_LOADER_H_
#define CROSSLANGUAGEMATCH_SRC_STRING_WORD_LOADER_H_

namespace cross_language_match {

class StringWordLoader : public WordLoader {
 public:
  StringWordLoader(std::string raw_string);
  std::map<std::string, std::string> *GetWordPairs() override;
 private:
  std::string raw_string_;
};

}

#endif //CROSSLANGUAGEMATCH_SRC_STRING_WORD_LOADER_H_
