#include <map>
#include <string>

#ifndef CROSSLANGUAGEMATCH_SRC_FILE_LOADER_H_
#define CROSSLANGUAGEMATCH_SRC_FILE_LOADER_H_

namespace cross_language_match {

class WordPairFileLoader {
 public:
  static std::map<std::string, std::string> *GetWordPairs(std::string file_path);
};

}
#endif //CROSSLANGUAGEMATCH_SRC_FILE_LOADER_H_
