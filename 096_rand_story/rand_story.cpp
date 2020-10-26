#include "rand_story.h"

#include <cstdlib>
#include <exception>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

int main(int argc, char ** argv) {
  if (argc != 4) {
    std::cerr << "Usage: inputFile" << std::endl;
    exit(EXIT_FAILURE);
  }
  std::ifstream f1(argv[1]);
  if (!f1) {
    std::cerr << "fail to open " << argv[1] << std::endl;
    exit(EXIT_FAILURE);
  }
  std::ifstream f2(argv[2]);
  if (!f2) {
    std::cerr << "fail to open " << argv[1] << std::endl;
    exit(EXIT_FAILURE);
  };
  std::string seed = argv[3];
  if (!isdigit(seed[0])) {
    std::cerr << "in valid seed " << argv[1] << std::endl;
    exit(EXIT_FAILURE);
  };

  Story story;
  unsigned n = std::stoi(argv[3]);
  try {
    story.read_category(f2);
    story.tellStory(f1, n);
    story.delet();
  }
  catch (std::exception & e) {
    std::cout << "An exception (" << e.what() << ") occured!\n";
  }
  return (EXIT_SUCCESS);
}
