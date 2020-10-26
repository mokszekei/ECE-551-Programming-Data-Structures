#include "parse.hpp"

#include <exception>
// any other headers you need
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <vector>

class invalid_input : public std::exception {
  // override what() here
  const char * what() const throw() { return "invalid_input"; }
};

item_t * parseLine(const std::string & line) {
  // write me

  item_t * item = new item_t;
  std::string key = line.substr(0, line.find(':'));
  item->first = key;
  std::vector<std::string> values;
  std::string temp = line.substr(line.find(':') + 1);
  size_t len = temp.length();
  size_t count = 1;
  if (len == 0) {
    std::string value = "";
    values.push_back(value);
  }
  else if (len != 0) {
    for (size_t i = 0; i < len; i++) {
      if (temp[i] == ',') {
        count++;
      }
    }
    while (count != 0) {
      if (count == 1) {
        values.push_back(temp);
        count--;
      }
      if (count != 1) {
        values.push_back(temp.substr(0, temp.find(',')));
        temp = temp.substr(temp.find(',') + 1);
        count--;
      }
    }
  }
  item->second = values;
  return item;
}

void check(std::string string) {
  int count = 0;
  int len = string.length();
  for (int i = 0; i < len; i++) {
    if (string[i] == ':') {
      count++;
      if (string[i + 1] != '\0' && string[i + 2] != '\0') {
        if (string[i + 1] != ' ' || string[i + 2] == ',') {
          throw invalid_input();
        }
      }
    }
  }
  if (count != 1 && string != "") {
    throw invalid_input();
  }
  for (int i = 0; i < len; i++) {
    if (string[i] == ',') {
      if (string[i + 2] == '\0' || string[i + 1] != ' ' || string[i + 2] == ',') {
        throw invalid_input();
      }
    }
  }
}

int main(int argc, char ** argv) {
  std::ifstream file;

  std::string strings;
  std::vector<item_t *> ans;
  size_t num = 0;
  int k = 0;
  if (argc == 2) {
    file.open(argv[1]);
    if (file.fail()) {
      std::cerr << "cannot read from file" << std::endl;
      return EXIT_FAILURE;
    }
    while (!file.eof()) {
      std::getline(file, strings);
      try {
        check(strings);
        if (strings.size() > 0) {
          ans.push_back(parseLine(strings));
          std::cout << "key= " << ans[k]->first << '\n' << " values =";
          size_t n = 0;
          n = ans[k]->second.size();
          for (size_t i = 0; i < n - 1; i++) {
            std::cout << ans[k]->second[i] << ',';
          }
          std::cout << ans[k]->second[n - 1] << '\n';
          k++;
        }
      }
      catch (std::exception & e) {
        std::cout << "An exception (" << e.what() << ") occured!\n";
      }
    }
  }
  else {
    std::cerr << "no input file" << std::endl;
    return EXIT_FAILURE;
  }
  num = ans.size();
  for (size_t i = 0; i < num; i++) {
    ans[i]->second.clear();
    delete ans[i];
  }
  ans.clear();

  file.close();
  return EXIT_SUCCESS;
}
