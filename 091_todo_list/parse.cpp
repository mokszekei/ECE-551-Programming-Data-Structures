#include "parse.hpp"

#include <exception>
// any other headers you need

item_t * parseLine(const std::string & line) {
  // write me
  unsigned long p1 = line.find_first_of(":");
  item_t * item = new item_t();

  // if colon is found
  if (p1 != std::string::npos) {
    if (p1 == 0)
      throw myex(item);
    item->first = line.substr(0, p1);
    // if no values
    if (p1 ==
        line.length() - 1 /* || (p1 == line.length() - 2 && line[p1 + 1] == ' ')*/) {
      return item;
    }
    else if (p1 == line.length() - 2 && line[p1 + 1] == ' ')
      throw myex(item);
  }
  else
    throw myex(item);

  if (line[p1 + 1] != ' ') {
    throw myex(item);
  }
  p1 += 2;
  unsigned long p2 = line.find(",", p1);
  // if comma is found
  while (p2 != std::string::npos) {
    if (p2 == line.length() - 1)
      throw myex(item);
    else if (line[++p2] != ' ')
      throw myex(item);
    std::string v = line.substr(p1, p2 - p1 - 1);
    if (v.empty())
      throw myex(item);
    else
      item->second.push_back(v);
    p1 = p2 + 1;
    p2 = line.find(',', p1);
  }
  std::string v = line.substr(p1);
  if (v.empty())
    throw myex(item);
  else
    item->second.push_back(v);
  return item;
}

/*
int main(int argc, char ** argv) {
  if (argc != 2) {
    std::cerr << "Usage: inputFile" << std::endl;
    exit(EXIT_FAILURE);
  }
  std::ifstream f(argv[1]);
  if (!f) {
    std::cerr << "fail to open " << argv[1] << std::endl;
    exit(EXIT_FAILURE);
  }
  std::string line;
  while (std::getline(f, line)) {
    item_t * item = NULL;
    try {
      item = parseLine(line);
    }
    catch (const myex & e) {
      std::cerr << e.what() << std::endl;
      delete e.item;
      continue;
    }
    std::cout << "key = " << item->first << std::endl;
    std::cout << "  values = ";
    std::vector<std::string>::iterator it;
    if (!item->second.empty()) {
      it = item->second.begin();
      std::cout << *it;
      it++;
    }
    while (it != item->second.end()) {
      std::cout << "; " << *it;
      ++it;
    }
    std::cout << std::endl;
    delete item;
  }
  f.close();
  return 0;
}
*/
