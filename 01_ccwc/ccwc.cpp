#include <iostream>
#include <locale>
#include <unistd.h>

int main(int argc, char *argv[]) {
  char *file_name = nullptr;
  struct {
    bool c = false;
    bool l = false;
    bool w = false;
    bool m = false;
  } __attribute__((aligned(4))) flags;
  struct {
    unsigned c = 0;
    unsigned l = 0;
    unsigned w = 0;
    unsigned m = 0;
  } __attribute__((aligned(16))) counter;

  int opt = -1;
  while ((opt = getopt(argc, argv, "clwm")) != -1) {
    switch (opt) {
    case 'c':
      flags.c = true;
      break;
    case 'l':
      flags.l = true;
      break;
    case 'w':
      flags.w = true;
      break;
    case 'm':
      flags.m = true;
      break;
    case '?':
      std::cout << "Unknown flag: " << optopt << std::endl;
      return EXIT_FAILURE;
    default:
      return EXIT_FAILURE;
    }
  }
  for (; optind < argc; ++optind) {
    if (file_name != nullptr) {
      std::cout << "Too many arguments: You have to give just one file name."
                << std::endl;
      return EXIT_FAILURE;
    }
    file_name = argv[optind];
  }

  FILE *fd = nullptr;
  if (file_name == nullptr) {
    fd = stdin;
  } else {
    fd = fopen(file_name, "r");
  }

  if (fd == nullptr) {
    std::cout << "Error opening file." << std::endl;
    return EXIT_FAILURE;
  }

  setlocale(LC_CTYPE, "");
  wint_t wc;
  bool in_word = false;
  char buf[8];
  while (WEOF != (wc = fgetwc(fd))) {
    counter.m++;
    counter.c += wctomb(buf, wc); // TODO: buffer some characters
    counter.l += wc == L'\n';

    bool wc_is_space = iswspace(wc)!=0;
    if (in_word) {
      if (wc_is_space) {
        in_word = false;
      }
    } else if (not wc_is_space) {
      in_word = true, counter.w++;
    }
  }

  fclose(fd);
  if (not(flags.l or flags.w or flags.c or flags.m)) {
    flags.w = flags.l = flags.c = true;
  }
  if (flags.l) {
    std::cout << "  " << counter.l;
  }
  if (flags.w) {
    std::cout << "  " << counter.w;
  }
  if (flags.c) {
    std::cout << "  " << counter.c;
  }
  if (flags.m) {
    std::cout << "  " << counter.m;
  }
  if (file_name) {
    std::cout << "  " << file_name;
  }
  std::cout << std::endl;
  return EXIT_SUCCESS;
}
