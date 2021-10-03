
#include "include/home_work_1.hxx"
#include <iostream>

using namespace std::literals;

int main(int argc, char const *argv[]) {
  std::string file_path;
  auto max_words = 100U;
  auto min_size = 5U;
  auto out_dir = "./"s;

  for (auto k = 1; k < argc; ++k) {
    std::string arg = argv[k];
    if (arg == "-f"s) {
      file_path = argv[++k];
    } else if (arg == "-d"s) {
      out_dir = argv[++k];
    } else if (arg == "-w"s) {
      max_words = std::stoi(argv[++k]);
    } else if (arg == "-s"s) {
      min_size = std::stoi(argv[++k]);
    } else {
      std::cerr << "unknown option: " << arg << std::endl;
      std::cerr
          << "usage: " << argv[0]
          << " [-f <file>] [-w <max-words>] [-s <min-size>] [-d <output path>]"
          << std::endl;
      return 1;
    }
  }

  // Reading words from file
  auto start_time = time_now();
  auto word_map = hm1::get_words(file_path);
  auto time_get_words = duration(time_now() - start_time);
  std::cout << "Time reading words: " << std::setprecision(5)
            << time_get_words / 1000000.0 << 's' << std::endl;

  if(word_map.empty()) {
    return -1;
  }

  // Filtering the maximum frequency from words list
  start_time = time_now();
  hm1::filter_words_data(word_map,
                         [&max_words](const hm1::WordData &word_data) {
                           return word_data.second <= max_words;
                         });
  auto time_filter_max_words = duration(time_now() - start_time);
  std::cout << "Time filter max frequency: " << std::setprecision(5)
            << time_filter_max_words / 1000000.0 << 's' << std::endl;

  // Filtering the minimum size from words list
  start_time = time_now();
  hm1::filter_words_data(word_map, [&min_size](const hm1::WordData &word_data) {
    return word_data.first.size() <= min_size;
  });
  auto time_filter_min_size = duration(time_now() - start_time);
  std::cout << "Time filter min size: " << std::setprecision(5)
            << time_filter_min_size / 1000000.0 << 's' << std::endl;

  // Sorting words based on decending frequency
  start_time = time_now();
  hm1::sort_words_data(word_map, [](const hm1::WordData &word_data_1,
                                    const hm1::WordData &word_data_2) {
    return word_data_1.second > word_data_2.second;
  });
  auto time_sort_words_data = duration(time_now() - start_time);
  std::cout << "Time sort decending freq: " << std::setprecision(5)
            << time_sort_words_data / 1000000.0 << 's' << std::endl;

  // Writing words into tag cloud format
  start_time = time_now();
  hm1::create_tag_cloud(
      word_map, "/home/yar/cpp_course_2021/homework1/data/tag_cloud.html");
  auto time_create_tag_cloud = duration(time_now() - start_time);
  std::cout << "Time create tag cloud: " << std::setprecision(5)
            << time_create_tag_cloud / 1000000.0 << 's' << std::endl;

  std::cout << "Time total: " << std::setprecision(5)
            << (time_get_words + time_filter_max_words + time_filter_min_size +
                time_sort_words_data + time_create_tag_cloud) /
                   1000000.0
            << 's' << std::endl;

  return 0;
}