#include <map>
#include <string>
#include "word_loader.h"

#ifndef CROSSLANGUAGEMATCH_SRC_FILE_LOADER_H_
#define CROSSLANGUAGEMATCH_SRC_FILE_LOADER_H_

namespace cross_language_match {

class FileWordLoader : public WordLoader {
 public:
  FileWordLoader(std::string file_path);
  ~FileWordLoader();
  WordLoader::InputError ParseAndLoadIntoMap() override;
 protected:
  std::istream &OpenInputStream() override;
  void CloseInputStream() override;
 private:
  std::string file_path_;
  std::ifstream *file_stream_;
};

}
#endif //CROSSLANGUAGEMATCH_SRC_FILE_LOADER_H_
