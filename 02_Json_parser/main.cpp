#include <iostream>
#include <fstream>

int main(int argc, char** argv) {
  char buf[1024];
  std::string file_name = argv[1];
  std::fstream file(std::string(file_name), std::ios::in | std::ios::out);
  
  if (!file) {
      std::cout << "Open file " << file_name << " for read failed\n";
      return 1;
  }
  char c{};
  while (file.get(c)){
    switch (c)
    {
      case ':':
        file.seekg(-1, std::ios::cur);
        file << "###";  
        file.seekg(file.tellg(), std::ios::beg);

        break;
    }
  } return 0;
}

