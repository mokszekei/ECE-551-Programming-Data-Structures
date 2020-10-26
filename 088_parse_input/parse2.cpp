#include <exception>

#include "parse.hpp"
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
  item_t * ans = new item_t;
  std::string key = line.substr(0, line.find(':'));
  ans->first = key;
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
      if (count >= 2) {
        std::string value = temp.substr(0, temp.find(','));
        values.push_back(value);
        temp = temp.substr(temp.find(',') + 1);
        count--;
      }
      if (count == 1) {
        std::string value = temp;
        values.push_back(temp);
        count--;
      }
    }
  }
  ans->second = values;
  return ans;
}
/*                                                                                                  
void check(std::string string) {                                                                    
  int count = 0;                                                                                    
  int len = string.length();                                                                        
  for (int i = 0; i < len; i++) {                                                                   
    if (string[i] == ':') {                                                                         
      count++;                                                                                      
    }                                                                                               
  }                                                                                                 
  if (count != 1 && string != "") {                                                                 
    throw std::exception();                                                                         
  }                                                                                                 
  for (int i = 0; i < len; i++) {                                                                   
    if (string[i] == ',') {                                                                         
      if (string[i + 1] != ' ') {                                                                   
        throw std::exception();                                                                     
      }                                                                                             
    }                                                                                               
  }                                                                                                 
}                                                                                                   
*/
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
/*                                                                                                  
int main(int argc, char ** argv) {                                                                  
  std::ifstream read;                                                                               
  std::string strings;                                                                              
  std::vector<item_t *> ans;                                                                        
  if (argc == 2) {                                                                                  
    read.open(argv[1]);                                                                             
    if (read.fail()) {                                                                              
      std::cerr << "cannot read from file" << std::endl;                                            
      return EXIT_FAILURE;                                                                          
    }                                                                                               
    while (!read.eof()) {                                                                           
      std::getline(read, strings);                                                                  
      //std::cout << strings << "\n";                                                               
      try {                                                                                         
        check(strings);                                                                             
      }                                                                                             
      catch (std::exception & e) {                                                                  
        std::cerr << "An exception (" << e.what() << " ) occured!\n";                               
      }                                                                                             
      if (strings.size() > 0) {                                                                     
        ans.push_back(parseLine(strings));                                                          
      }                                                                                             
    }                                                                                               
  }                                                                                                 
  else {                                                                                            
    std::cerr << "no input file" << std::endl;                                                      
    return EXIT_FAILURE;                                                                            
  }                                                                                                 
  size_t num = 0;                                                                                   
  num = ans.size();                                                                                 
  for (size_t i = 0; i < num; i++) {                                                                
    std::cout << "key = " << ans[i]->first << "\n"                                                  
              << "  values =";                                                                      
    size_t sub_num = 0;                                                                             
    sub_num = ans[i]->second.size();                                                                
    for (size_t j = 0; j < sub_num - 1; j++) {                                                      
      std::cout << ans[i]->second[j] << ";";                                                        
    }                                                                                               
    std::cout << ans[i]->second[sub_num - 1] << "\n";                                               
  }                                                                                                 
  num = ans.size();                                                                                 
  for (size_t i = 0; i < num; i++) {                                                                
    ans[i]->second.clear();                                                                         
    delete ans[i];                                                                                  
  }                                                                                                 
  ans.clear();                                                                                      
  read.close();                                                                                     
  return EXIT_SUCCESS;                                                                              
}                                                                                                   
*/
int main(int argc, char ** argv) {
  std::ifstream read;
  std::string strings;
  std::vector<item_t *> ans;
  size_t num = 0;
  int k = 0;
  if (argc == 2) {
    read.open(argv[1]);
    if (read.fail()) {
      std::cerr << "cannot read from file" << std::endl;
      return EXIT_FAILURE;
    }
    while (!read.eof()) {
      std::getline(read, strings);
      try {
        check(strings);
        if (strings.size() > 0) {
          ans.push_back(parseLine(strings));
          std::cout << "key = " << ans[k]->first << "\n"
                    << "  values =";
          size_t sub_num = 0;
          sub_num = ans[k]->second.size();
          for (size_t j = 0; j < sub_num - 1; j++) {
            std::cout << ans[k]->second[j] << ";";
          }
          std::cout << ans[k]->second[sub_num - 1] << "\n";
          k++;
        }
      }
      catch (std::exception & e) {
        std::cerr << "An exception (" << e.what() << " ) occured!\n";
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
  read.close();
  return EXIT_SUCCESS;
}
