#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <vector>

#include "parse.hpp"
#include "task.hpp"

using namespace std;

// found in answers.
bool findp(const list<string> & l, const string & s) {
  for (auto i : l) {
    if (i == s)
      return true;
  }
  return false;
}

// all pres exist.
bool toAdd(const list<string> & pres, const list<string> & ans) {
  for (auto i : pres) {
    if (!findp(ans, i))
      return false;
  }
  return true;
}

// remove the added tasks.
void rmTask(list<TaskDep> & l, vector<string> added) {
  for (string i : added) {
    for (auto it = l.begin(); it != l.end(); it++) {
      if (it->getTask() == i) {
        l.erase(it);
        break;
      }
    }
  }
}

void addToAns(TaskDep t, list<string> & ans) {
  if (t.getPredecessors().empty()) {
    ans.push_front(t.getTask());
    return;
  }
  auto it = ans.begin();
  for (string i : t.getPredecessors()) {
    auto temp = find(it, ans.end(), i);
    if (temp != ans.end())
      it = temp;
  }
  ans.insert(++it, t.getTask());
  return;
}

// one iteration thru tasks
void addTask(list<TaskDep> & l, list<string> & ans) {
  vector<string> added;
  for (auto i : l) {
    if (toAdd(i.getPredecessors(), ans)) {
      //      ans.push_back(i.getTask());
      addToAns(i, ans);
      added.push_back(i.getTask());
    }
  }
  if (added.empty()) {
    cout << "Can't make a todo list" << endl;
    exit(EXIT_FAILURE);
  }
  rmTask(l, added);
}

int main(int argc, char ** argv) {
  // open file for reading
  // make dependency list of tasks from input file
  // order list according to dependencies
  // print todo list

  if (argc != 2) {
    std::cerr << "Usage: inputFile" << std::endl;
    exit(EXIT_FAILURE);
  }
  string filename = string(argv[1]);
  std::ifstream f(filename);
  if (!f) {
    std::cerr << "fail to open " << filename << std::endl;
    exit(EXIT_FAILURE);
  }
  std::string line;
  list<TaskDep> tasks;
  while (std::getline(f, line)) {
    item_t * item = NULL;
    try {
      item = parseLine(line);
    }
    catch (const myex & e) {
      std::cerr << e.what() << std::endl;
      delete e.item;
      exit(EXIT_FAILURE);
    }
    tasks.push_back(TaskDep(item->first));
    for (unsigned long i = 0; i < item->second.size(); i++) {
      tasks.back().addPredecessor((item->second)[i]);
    }
    delete item;
  }

  list<string> answer;

  while (!tasks.empty()) {
    addTask(tasks, answer);
  }

  bool flag = true;
  for (auto i : answer) {
    if (flag) {
      cout << i;
      flag = false;
    }
    else
      cout << ", " << i;
  }
  cout << endl;

  return EXIT_SUCCESS;
}
