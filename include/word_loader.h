#include <istream>
#include <map>

#ifndef CROSSLANGUAGEMATCH_INCLUDE_WORD_LOADER_H_
#define CROSSLANGUAGEMATCH_INCLUDE_WORD_LOADER_H_

namespace cross_language_match {

class WordLoader {
 public:
  std::map<std::string, std::string> *GetWordPairs();
 protected:
  virtual std::istream &OpenInputStream() = 0;
  virtual void CloseInputStream() = 0;
};

}
#endif //CROSSLANGUAGEMATCH_INCLUDE_WORD_LOADER_H_
