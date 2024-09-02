#include <codecvt>
#include <fstream>
#include <iosfwd>
#include <iostream>
#include <locale>
#include <regex>
#include <string>
#include <sys/stat.h>
#include <unistd.h>

long GetFileSize(std::string filename) {
  struct stat stat_buf;
  int rc = stat(filename.c_str(), &stat_buf);
  return rc == 0 ? stat_buf.st_size : -1;
}

int main(int argc, char **argv) {
  bool done = false;
  for (int i = 0; i < argc; ++i) {
    if (argv[i] == std::string("-c")) {
      std::string file_name = argv[i + 1];
      int word_count = 0;
      auto file_handler = std::ifstream(file_name);
      word_count = GetFileSize(file_name);
      std::cout << "\t" << word_count << " " << file_name << '\n';
      done = true;
    } else if (argv[i] == std::string("-l")) {
      std::string line;
      std::string file_name = argv[i + 1];
      int line_count = 0;
      auto file_handler = std::ifstream(file_name);
      while (getline(file_handler, line))
        line_count++;
      std::cout << "\t" << line_count << " " << file_name << '\n';
      done = true;
    } else if (argv[i] == std::string("-w")) {
      std::string file_name = argv[i + 1];
      std::string line;
      int word_count = 0;
      auto file_handler = std::ifstream(file_name);
      while (getline(file_handler, line)) {
        // std::regex
        // word_regex("([\\wàêœ§ŭ’éŭÉÈÊËÇÀÂÔÙÛÎÏÄÖÜŒœÆéèêëçàâôùûîïäöüæ]+([-'./\\ŭ’,ÉÈÊËÇÀÂÔÙÛÎÏÄÖÜŒœÆéèêëçàâôùûîïäöüæ—:]*\\w*)*)");
        std::regex word_regex("\\S+");
        auto words_begin =
            std::sregex_iterator(line.begin(), line.end(), word_regex);
        auto words_end = std::sregex_iterator();
        word_count += std::distance(words_begin, words_end);
      }
      std::cout << "\t" << word_count << " " << file_name << '\n';
      done = true;
    } else if (argv[i] == std::string("-m")) {
      std::string file_name = argv[i + 1];
      FILE *fd = fopen("/home/abdoe/dev/cc/wc/test.txt", "r");
      // std::cout << file_name.c_str() << std::endl;
      std::ifstream file_handler(file_name);
      if (!file_handler) {
        std::cerr << "Error: Could not open file " << file_name << std::endl;
        return 1;
      }
      std::string line;
      int char_count = 0;

      setlocale(LC_CTYPE, "");
      wint_t wc;
      while (WEOF != (fgetwc(fd))) {
        char_count++;
      }

      std::cout << "\t" << char_count << " " << file_name << '\n';
      done = true;
    }
  }
  if (not done)
    std::cout << "Something went wrong!" << '\n';
  return 0;
}
