#include <fcntl.h>
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

  int fd = -1;
  if (file_name == nullptr) {
    fd = STDIN_FILENO; // stdin;
  } else {
    fd = open(file_name, O_RDONLY);
  }

  if (fd == -1) {
    std::cout << "Error opening file." << std::endl;
    return EXIT_FAILURE;
  }
  std::setlocale(LC_ALL, "en_US.utf8");
  const int _1K = 4 * 1024;
  char buffer[_1K] = {0};
  ssize_t bytes_read = 0;
  bool in_word = false;
  std::mbstate_t state = std::mbstate_t();

  while ((bytes_read = read(fd, buffer, _1K)) > 0) { // Read 1KB chunks
    const char *buffer_pointer = buffer;
    int len = 0;
    wchar_t wc = L'0';
    counter.c += bytes_read;

    while (bytes_read > 0) {
      len = std::mbrtowc(&wc, buffer_pointer, bytes_read, &state);

      if (len > 0) { // Valid multibytes
        counter.m++;
        counter.l += (wc == L'\n'); 

        const bool wc_is_space = (bool)iswspace(wc);
        if (in_word) {
          if (wc_is_space) {
            in_word = false; // End of a word
          }
        } else if (!wc_is_space) {
          in_word = true;
          counter.w++; // Start of a new word
        }

        // Move buffer pointer forward by the number of bytes consumed
        buffer_pointer += len;
        bytes_read -= len;
      } else if (len == -2) {
        // Incomplete multibyte character. Break to get the next chunk of input.
        break;
      } else if (len == -1) {
        // Invalid multibyte sequence, skip one byte and continue
        ++buffer_pointer;
        --bytes_read;
        state = std::mbstate_t(); // Reset state
      }
    }
  }

  close(fd);
  if (not(flags.l or flags.w or flags.c or flags.m)) {
    flags.w = flags.l = flags.c = true;
  }
  if (flags.l) {
    std::cout << "  " << counter.l;
  }
  if (flags.w) {
    std::cout << "  " << counter.w;
  }
  if (flags.m) {
    std::cout << "  " << counter.m;
  }
  if (flags.c) {
    std::cout << "  " << counter.c;
  }
  if (file_name) {
    std::cout << "  " << file_name;
  }
  std::cout << std::endl;

  return EXIT_SUCCESS;
}

// void print_counters()
