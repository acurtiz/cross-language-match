#include <map>
#include <string>

#ifndef CROSSLANGUAGEMATCH_SRC_FILE_LOADER_H_
#define CROSSLANGUAGEMATCH_SRC_FILE_LOADER_H_

namespace cross_language_match {

class WordPairFileLoader {
 public:
  WordPairFileLoader(std::string file_path);
  ~WordPairFileLoader();
  std::map<std::string, std::string> *GetWordPairs();
 private:
  std::map<std::string, std::string> *word_pairs_;
};

}
#endif //CROSSLANGUAGEMATCH_SRC_FILE_LOADER_H_
