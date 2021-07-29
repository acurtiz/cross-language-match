#include <istream>

#ifndef CROSSLANGUAGEMATCH_INCLUDE_WORD_LOADER_H_
#define CROSSLANGUAGEMATCH_INCLUDE_WORD_LOADER_H_

namespace cross_language_match {

class WordLoader {
 public:
  virtual std::map<std::string, std::string> *GetWordPairs() = 0;
 protected:
  void ExtractLineFromStreamIntoMap(std::istream *input_stream,
                                    std::map<std::string, std::string> *destination_map);

};

}
#endif //CROSSLANGUAGEMATCH_INCLUDE_WORD_LOADER_H_
