class Vector2D {
 private:
  double x;
  double y;

 public:
  Vector2D() {
    x = 0;
    y = 0;
  }
  Vector2D(double x_cor, double y_cor) {
    x = x_cor;
    y = y_cor;
  }
  //Vector2D() : x(0), y(0) {}
  //Vector2D(double a, double b) : x(a), y(b) {}
  void initVector(double init_x, double init_y);
  double getMagnitude() const;
  Vector2D operator+(const Vector2D & rhs) const;
  Vector2D & operator+=(const Vector2D & rhs);
  double dot(const Vector2D & rhs) const;
  void print() const;
};
