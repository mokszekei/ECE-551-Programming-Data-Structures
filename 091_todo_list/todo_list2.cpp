#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <vector>

#include "task.hpp"

class invalid_input : public std::exception {
  // override what() here
  const char * what() const throw() { return "invalid_input"; }
};

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

TaskDep * parseLine(const std::string & line) {
  // write me
  task_t t = line.substr(0, line.find(':'));
  TaskDep * ans = new TaskDep(t);

  std::list<task_t> pres;
  std::string temp = line.substr(line.find(':') + 1);
  size_t len = temp.length();
  size_t count = 1;
  if (len == 0) {
    task_t pre = "";
    //pres.push_back(pre);
    ans->addPredecessor(pre);
  }
  else if (len != 0) {
    for (size_t i = 0; i < len; i++) {
      if (temp[i] == ',') {
        count++;
      }
    }
    while (count != 0) {
      if (count >= 2) {
        task_t pre = temp.substr(0, temp.find(','));
        //  pres.push_back(pre);
        ans->addPredecessor(pre);
        temp = temp.substr(temp.find(',') + 1);
        count--;
      }
      if (count == 1) {
        task_t pre = temp;
        //pres.push_back(pre);
        ans->addPredecessor(pre);
        count--;
      }
    }
  }

  return ans;
}
void print(std::list<std::string> & list) {
  size_t k = 1;
  size_t n = list.size();
  std::list<std::string>::iterator it = list.begin();
  while (k < n) {
    std::cout << *it << ", ";
    k++;
    ++it;
  }
  std::cout << *it;
}

int check_list(task_t s, std::list<task_t> & list) {
  if (list.empty()) {
    return 0;
  }
  std::list<task_t>::iterator it = list.begin();
  while (it != list.end()) {
    if (*it == s) {
      return 1;
    }
    ++it;
  }
  return 0;
}

std::list<task_t>::iterator find_position(std::list<task_t> & list,
                                          const std::list<task_t> & pres) {
  std::list<task_t>::iterator it_l1 = list.begin();
  std::list<task_t>::iterator it_l2 = list.begin();

  std::list<task_t>::const_iterator it = pres.begin();

  while (it != pres.end()) {
    it_l2 = std::find(it_l1, list.end(), *it);
    if (it_l2 != list.end()) {
      it_l1 = it_l2;
    }
  }
  return it_l1;
}
std::list<task_t> make_ans(std::list<TaskDep *> read_list) {
  std::list<task_t> ans;
  std::list<TaskDep *>::iterator it = read_list.begin();
  size_t count = 0;
  size_t n_task = read_list.size();
  while (ans.size() < n_task) {
    while (it != read_list.end()) {
      if ((*it)->getPredecessors().empty()) {
        ans.push_back((*it)->getTask());
      }
      else {
        std::list<task_t>::iterator itt = (*it)->getPredecessors().begin();
        size_t i = 0;
        while (itt != (*it)->getPredecessors().end()) {
          if (check_list(*itt, ans) == 1) {
            i++;
          }
          ++itt;
        }
        if (i == (*it)->getPredecessors().size()) {
          // insert the the latter predecessor:
          /// this is wrong ans.push_back((*it)->getTask());
          std::list<task_t>::iterator pos = find_position(ans, (*it)->getPredecessors());
          ans.insert(pos, (*it)->getTask());
        }
      }
      ++it;
    }

    if (ans.size() > count) {
      count = ans.size();
      it = read_list.begin();
    }
    else {
      std::cerr << "the dependencies do not allow a todo list to be created" << std::endl;
      throw std::exception();
    }
  }
  return ans;
}

int main(int argc, char ** argv) {
  // open file for reading
  // make dependency list of tasks from input file
  // order list according to dependencies
  // print todo list
  std::ifstream file;
  std::string strings;
  std::list<TaskDep *> read_list;
  std::list<task_t> ans;

  if (argc != 2) {
    std::cerr << "no input file" << std::endl;
    return EXIT_FAILURE;
  }

  file.open(argv[1]);
  if (file.fail()) {
    std::cerr << "cannot read this file" << std::endl;
    return EXIT_FAILURE;
  }
  while (!file.eof()) {
    std::getline(file, strings);
    try {
      check(strings);
      if (strings.size() > 0) {
        read_list.push_back(parseLine(strings));
      }
    }
    catch (std::exception & e) {
      std::cerr << "An exception (" << e.what() << ") occured!\n";
    }
  }
  try {
    std::list<task_t> ans = make_ans(read_list);
  }
  catch (std::exception & e) {
    return EXIT_FAILURE;
  }

  print(ans);

  ///delete read_list
  std::list<TaskDep *>::iterator it = read_list.begin();
  while (it != read_list.end()) {
    TaskDep * temp = *it;
    temp->getPredecessors().erase(temp->getPredecessors().begin(),
                                  temp->getPredecessors().end());
    --it;
  }
  read_list.erase(read_list.begin(), read_list.end());

  // delete ans;

  ans.erase(ans.begin(), ans.end());

  file.close();

  return EXIT_SUCCESS;
}
