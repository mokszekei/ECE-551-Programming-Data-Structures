#ifndef __PARSE_HPP__
#define __PARSE_HPP__

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <vector>
typedef std::pair<std::string, std::vector<std::string> > item_t;

item_t * parseLine(const std::string & line);

class invalid_input : public std::exception {
  // override what() here
 public:
  item_t * item;
  invalid_input(item_t * i) : item(i) {}
  virtual const char * what() const throw() { return "invalid_input"; }
};

typedef invalid_input myex;
#endif
