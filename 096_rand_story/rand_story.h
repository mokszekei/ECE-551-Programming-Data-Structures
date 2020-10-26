#include <cstdlib>
#include <exception>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

class invalid_input : public std::exception {
  // override what() here
  const char * what() const throw() { return "invalid_input"; }
};

class Story {
 private:
  std::map<std::string, std::vector<std::string> > category;

  //std::vector<std::string> story;

 public:
  // helper functions to read in the story
  void tellStory(std::ifstream & f, unsigned n) {
    std::srand(n);

    std::string line;
    while (std::getline(f, line)) {
      std::string item;
      std::size_t p1;
      std::size_t p2 = 0;
      p1 = line.find('_');
      while (p1 != std::string::npos) {
        item = line.substr(p2, p1 - p2);
        std::cout << item;
        p2 = p1 + 1;
        p1 = line.find('_', p2);
        std::string key = line.substr(p2, p1 - p2);
        if (isdigit(key[0])) {
          int size = category["used"].size();
          int k = std::stoi(key);
          if (size < k) {
            throw invalid_input();
          }

          std::cout << category["used"][category["used"].size() - std::stoi(key)];
          category["used"].push_back(
              category["used"][category["used"].size() - std::stoi(key)]);
        }
        else {
          int v1 = std::rand() % category[key].size();
          std::cout << category[key][v1];
          category["used"].push_back(category[key][v1]);
        }
        p2 = p1 + 1;
        p1 = line.find('_', p2);
      }
      item = line.substr(p2, line.size() - p2);
      std::cout << item;
      if (line[line.size() - 1] != '.') {
        std::cout << "\n";
      }
    }
  }

  // generate the map of categories
  void read_category(std::ifstream & f) {
    std::string line;
    while (std::getline(f, line)) {
      std::string key = line.substr(0, line.find(':'));
      std::string value =
          line.substr(line.find(':') + 1, line.size() - line.find(':') - 1);
      category[key].push_back(value);
    }
  }
  //replacing a category with a random word from that category.
  // std::map<std::string, std::vector<std::string> > rand_category(
  //   std::map<std::string, std::vector<std::string> > category){};
  // constructor
  Story(){};
  //publid tellStory method, which takes an unsigned integer for the random seed.
  //void tellStory(unsigned n){};
  void delet(void) { category.clear(); }
};
