#include <istream>
#include <map>

#ifndef CROSSLANGUAGEMATCH_INCLUDE_WORD_LOADER_H_
#define CROSSLANGUAGEMATCH_INCLUDE_WORD_LOADER_H_

namespace cross_language_match {

class WordLoader {

 public:
  enum InputError {
    NONE,
    LINE_CONTAINS_MORE_THAN_ONE_COMMA,
    LINE_CONTAINS_NO_COMMA,
    FILE_NOT_FOUND
  };
  virtual InputError ParseAndLoadIntoMap();
  std::map<std::string, std::string> GetWordPairMap();

 protected:
  virtual std::istream &OpenInputStream() = 0;
  virtual void CloseInputStream() = 0;

 private:
  std::map<std::string, std::string> word_pairs_;

};

}
#endif //CROSSLANGUAGEMATCH_INCLUDE_WORD_LOADER_H_
