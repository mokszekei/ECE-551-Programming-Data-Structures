#include <sstream>
#include <string>

//Parent class does not necessarily need to declare private field,
class Expression {
 public:
  virtual std::string toString() const = 0;
  virtual ~Expression() {}
};

class NumExpression : public Expression {
 private:
  long num;

 public:
  NumExpression(long n) : num(n) {}
  virtual std::string toString() const {
    std::stringstream s;
    s << num;
    return s.str();
  }
};

class AllExpression : public Expression {
 private:
  const char * op;
  Expression * lhs;
  Expression * rhs;

 public:
  AllExpression(const char * o, Expression * l, Expression * r) : op(o), lhs(l), rhs(r) {}
  virtual std::string toString() const {
    std::stringstream s;
    s << '(' << lhs->toString() << ' ' << op << ' ' << rhs->toString() << ')';
    return s.str();
  }
  virtual ~AllExpression() {
    delete lhs;
    delete rhs;
  }
};

class PlusExpression : public AllExpression {
 public:
  PlusExpression(Expression * l, Expression * r) : AllExpression("+", l, r) {}
};
