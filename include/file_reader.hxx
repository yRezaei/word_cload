#include "error_helper.hxx"
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>

using namespace std::literals;

class FileReader {
private:
  std::ifstream file;

public:
  FileReader(const std::string &file_name,
             helper::ErrorCallback error_callback) {
    if (std::filesystem::exists(file_name)) {
      file.open(file_name);
    } else {
      error_callback("ERROR -> Couldn't open the file: "s + file_name);
    }
  }

  class iterator {
  private:
    std::ifstream *file = nullptr;
    bool eof = false;
    std::string current_line;

  private:
    std::string read_line() {
      std::string line;
      std::getline(*file, line);
      eof = !file->good();
      return line;
    }

  public:
    iterator() : eof{true} {}
    iterator(std::ifstream *file) : file{file} { current_line = read_line(); }

  public:
    void operator++() { current_line = read_line(); }
    std::string operator*() const { return current_line; }
    bool operator!=(const iterator &itr) const {
      return (*this).eof != itr.eof;
    }
  };

  iterator begin() { return {&file}; }
  iterator end() { return {}; }
};
