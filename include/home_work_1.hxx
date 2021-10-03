#ifndef STRING_UTIL_HXX_
#define STRING_UTIL_HXX_

#include "file_reader.hxx"
#include <algorithm>
#include <chrono>
#include <functional>
#include <iomanip>
#include <iostream>
#include <random>
#include <regex>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace hm1 {
using namespace std::literals;

using WordData = std::pair<std::string, unsigned>;
using WordsDataList = std::vector<WordData>;
using TimePoint = std::chrono::high_resolution_clock::time_point;

#define duration(a)                                                            \
  std::chrono::duration_cast<std::chrono::microseconds>(a).count()
#define time_now() std::chrono::high_resolution_clock::now()

template <typename F, typename... Args>
double time_it(F func, Args &&... args) {
  auto start_time = time_now();
  func(std::forward<Args>(args)...);
  return duration(time_now() - start_time);
}

WordsDataList get_words(const std::string &file_path) {
  auto word_map = std::unordered_map<std::string, unsigned>{};
  bool read_failed = false;
  auto file_exist_callback = [&read_failed](const std::string &error) {
    std::cerr << error << std::endl;
    read_failed = true;
  };

  if (!read_failed) {
    for (auto line : FileReader{file_path, file_exist_callback}) {
      line = std::regex_replace(line, std::regex("[^\\da-zA-Z\\s]"), " ");
      std::stringstream stream(line);
      for (std::string word; stream >> word;) {
        if (word_map.find(word) == word_map.end()) {
          if (word.size() > 1) {
            word_map[word] = 1;
          }
        } else {
          word_map[word]++;
        }
      }
    }
  }

  return WordsDataList{word_map.begin(), word_map.end()};
}

void filter_words_data(WordsDataList &word_map,
                       std::function<bool(const WordData &)> filter_func) {
  word_map.erase(std::remove_if(word_map.begin(), word_map.end(), filter_func),
                 word_map.end());
}

void sort_words_data(
    WordsDataList &word_map,
    std::function<bool(const WordData &, const WordData &)> compare_func) {
  std::sort(word_map.begin(), word_map.end(), compare_func);
}

void write_to_file(const WordsDataList &word_map,
                   const std::string &file_path) {
  std::ofstream out_file;
  out_file.open(file_path);
  if (out_file.is_open()) {
    for (const auto &wd : word_map)
      out_file << '"' << wd.first << "\" : "s << wd.second << '\n';
    out_file.flush();
    out_file.close();
  } else {
    std::cerr << "ERROR -> can not open the file with path: \"" << file_path
              << "\" to write words in it." << std::endl;
  }
}

auto randColor(std::mt19937 &gen) -> std::string {
  auto val = std::uniform_int_distribution<>{0, 255};
  auto HEX = [&val, &gen]() {
    auto buf = std::ostringstream{};
    buf << "#" << std::hex << (val(gen) << 16 | val(gen) << 8 | val(gen));
    return buf.str();
  };
  return HEX();
}

void create_tag_cloud(WordsDataList &words_data_list,
                      const std::string &file_path) {
  auto maxFreq = words_data_list.front().second;
  auto minFreq = words_data_list.back().second;
  auto maxFontSize = 90.0f;
  auto minFontSize = 1.0f;
  auto scale = (maxFontSize - minFontSize) / (maxFreq - minFreq);

  auto rd = std::random_device{};
  auto gen = std::mt19937{rd()};
  std::shuffle(words_data_list.begin(), words_data_list.end(), gen);

  std::ofstream html_file;
  html_file.open(file_path);
  if (html_file.is_open()) {
    html_file << "<html>\n"s 
              << "  <head>\n"s
              << "    <title>Embedded Style Sample</title>\n"
	            << "    <link rel=\"stylesheet\" type=\"text/css\" href=\"main.css\" />\n"s
              << "  </head>\n"s
              << "  <body>\n"s
              << "    <ul class=\"cloud\" role=\"navigation\" aria-label=\"Webdev word cloud\">\n";
    {
      for (auto &wd : words_data_list) {
        html_file << "    <li><a href=\"#\" data-weight=\"" << static_cast<int>(scale * wd.second) << "\">" << wd.first << "</a></li>"s << '\n';
      }
    }

    html_file << "  </body>\n"s
              << "</html>"s;

    html_file.close();
  } else {
    std::cerr << "ERROR -> can not open the file with path: \"" << file_path
              << "\" to write tag cloud in it." << std::endl;
  }
}

} // namespace hm1

#endif // end of STRING_UTIL_HXX_