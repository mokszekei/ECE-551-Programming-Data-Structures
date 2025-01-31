#include "IntArray.h"

#include <assert.h>

#include <ostream>

IntArray::IntArray() : data(NULL), numElements(0) {
}

IntArray::IntArray(int n) : data(new int[n]), numElements(n) {
}

IntArray::IntArray(const IntArray & rhs) :
    data(new int[rhs.numElements]),
    numElements(rhs.numElements) {
  for (int i = 0; i < numElements; i++) {
    data[i] = rhs.data[i];
  }
}
IntArray::~IntArray() {
  delete[] data;
}

IntArray & IntArray::operator=(const IntArray & rhs) {
  if (this != &rhs) {
    int * temp = new int[rhs.numElements];
    numElements = rhs.numElements;
    for (int i = 0; i < numElements; i++) {
      temp[i] = rhs.data[i];
    }
    delete[] data;
    data = temp;
  }
  return *this;
}

const int & IntArray::operator[](int index) const {
  assert(index < numElements);
  int & temp = data[index];
  return temp;
}
int & IntArray::operator[](int index) {
  assert(index < numElements);
  int & temp = data[index];
  return temp;
}

int IntArray::size() const {
  return numElements;
}

bool IntArray::operator==(const IntArray & rhs) const {
  if (numElements != rhs.numElements) {
    return false;
  }
  for (int i = 0; i < numElements; i++) {
    if (data[i] != rhs.data[i]) {
      return false;
    }
  }
  return true;
}

bool IntArray::operator!=(const IntArray & rhs) const {
  return !(*this == rhs);
}

std::ostream & operator<<(std::ostream & s, const IntArray & rhs) {
  s << '{';
  for (int i = 0; i < rhs.size(); i++) {
    s << rhs.data[i];
    if (i + 1 != rhs.size()) {
      s << ',';
    }
  }
  s << '}';
  return s;
}
