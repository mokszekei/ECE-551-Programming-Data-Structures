#include <math.h> /* fmod */
#include <stdio.h>

#include <cstdlib>
#include <exception>
#include <fstream>  // Save to local file.
#include <iomanip>  // for std::setprecision(2)
#include <iostream>
#include <vector>

class invalid_input : public std::exception {
  const char * what() const throw() { return "invalid correlation input"; }
};

class Triangle;

class Node {
 private:
  int id;
  double x;
  double y;
  //std::vector<Triangle> list_of_triangle;  //length no longer than 3

 public:
  Node(){};
  Node(int idd, double x_cor, double y_cor) : id(idd), x(x_cor), y(y_cor){};
  int getid() { return id; }
  double getx() { return x; }
  double gety() { return y; }
  Node & operator=(const Node & rhs) {
    id = rhs.id;
    x = rhs.x;
    y = rhs.y;
    return *this;
  };
  ~Node(){};
};

class Edge {
 private:
  std::vector<Node> edge;  //lenght=2
  //std::vector<Triangle> list_of_triangle;  //length no longer than 2
 public:
  void appendNode(Node node) { edge.push_back(node); }
  std::vector<Node> getEdge() { return edge; }
  ~Edge(){};
};

class Triangle {
 private:
  std::vector<Node> nodes;  //lenght=3
  std::vector<Edge> edges;  //length=2

 public:
  Triangle(){};
  Node & operator[](int index) { return nodes[index]; };
  void appendNode(Node node) { nodes.push_back(node); }
  void appendEdge(Edge edge) { edges.push_back(edge); }

  std::vector<Node> getNodes() { return nodes; };
  std::vector<Edge> getEdges() { return edges; };

  //If node inside the triangle, return 1； if outside return 0
  int IsPointInTriangle(Node node) {
    Node nodeA = nodes[0];
    Node nodeB = nodes[1];
    Node nodeC = nodes[2];
    std::vector<double> PA;
    PA.push_back(nodeA.getx() - node.getx());
    PA.push_back(nodeA.gety() - node.gety());
    std::vector<double> PB;
    PB.push_back(nodeB.getx() - node.getx());
    PB.push_back(nodeB.gety() - node.gety());
    std::vector<double> PC;
    PC.push_back(nodeC.getx() - node.getx());
    PC.push_back(nodeC.gety() - node.gety());
    double t1 = PA[0] * PB[1] - PA[1] * PB[0];
    double t2 = PB[0] * PC[1] - PB[1] * PC[0];
    double t3 = PC[0] * PA[1] - PC[1] * PA[0];
    if (t1 * t2 >= 0 && t1 * t3 >= 0) {
      return 1;
    }
    else {
      return 0;
    }
  };
  //double calculateMinimumAngle(Node node) {};

  ~Triangle(){};
};

class Surface {
 private:
  double max_x;
  double max_y;
  std::vector<Node> all_nodes;

 public:
  Surface(){};
  Node & operator[](int index) { return all_nodes[index]; };
  void readfile(std::ifstream & f) {
    std::string line;
    std::getline(f, line);
    max_x = std::stod(line);
    std::getline(f, line);
    max_y = std::stod(line);
    int id = 0;
    while (std::getline(f, line)) {
      std::size_t p1 = 0;
      std::size_t p2 = 0;
      p1 = line.find(" ", 0);
      p2 = line.find(" ", p1 + 1);
      if (p2 == std::string::npos) {
        throw invalid_input();
      }
      std::string x = line.substr(p1 + 1, p2 - p1 - 1);
      double x_cor = std::stod(x);
      int a = p2 + 1;
      int b = line.length() - p2 - 1;
      std::string y = line.substr(a, b);
      double y_cor = std::stod(y);
      Node node(id, x_cor, y_cor);
      all_nodes.push_back(node);
      id++;
    }
    //Now start to creat new nodes
    double width = all_nodes[3].getx();
    double height = all_nodes[3].gety();
    int nx;
    double realx;
    int ny;
    double realy;

    if (fmod(width, max_x) != 0) {
      nx = width / max_x + 1;
      realx = width / nx;
    }
    else {
      realx = max_x;
      nx = width / max_x;
    }
    if (fmod(height, max_y) != 0) {
      ny = height / max_y + 1;
      realy = height / (ny);
    }
    else {
      realy = max_y;
      ny = height / max_y;
    }
    std::vector<double> x_arr;
    for (int i = 0; i < nx + 1; i++) {
      x_arr.push_back(i * realx);
    }
    std::vector<double> y_arr;
    for (int i = 0; i < ny + 1; i++) {
      y_arr.push_back(i * realy);
    }
    int x = 0;
    for (int i = 1; i < ny; i++) {
      int lastid = all_nodes[all_nodes.size() - 1].getid();
      Node node(lastid + 1, x, y_arr[i]);
      all_nodes.push_back(node);
    }

    for (int i = 1; i < nx; i++) {
      for (int j = 0; j < ny + 1; j++) {
        int lastid = all_nodes[all_nodes.size() - 1].getid();
        Node node(lastid + 1, x_arr[i], y_arr[j]);
        all_nodes.push_back(node);
      }
    }

    x = x_arr[x_arr.size() - 1];
    for (int i = 1; i < ny; i++) {
      int lastid = all_nodes[all_nodes.size() - 1].getid();
      Node node(lastid + 1, x, y_arr[i]);
      all_nodes.push_back(node);
    }
  };
  Node getNode(int i) { return all_nodes[i]; };
  std::vector<Node> getAllNodes() { return all_nodes; };
  void print_nodes() {
    std::cout << "$nodes" << '\n';
    for (std::size_t i = 0; i < all_nodes.size(); i++) {
      std::cout << std::setprecision(3) << all_nodes[i].getid() + 1 << ' '
                << std::setprecision(3) << all_nodes[i].getx() << ' '
                << std::setprecision(3) << all_nodes[i].gety() << "\n";
    }
  }
  ~Surface(){};
};

class Triangulation {
 private:
  std::vector<Triangle> triangles;
  std::vector<Node> mesh_nodes;
  std::vector<std::vector<int> > elements;  //3 columns

 public:
  Triangulation(){};
  Triangle & operator[](int index) { return triangles[index]; };
  std::vector<Node> getMeshNodes() { return mesh_nodes; };
  void appendNode(Node node) { mesh_nodes.push_back(node); }

  void constructTriangle(std::vector<int> & ids) {
    Triangle temp_tri;
    for (int i = 0; i < 3; i++) {
      temp_tri.appendNode(mesh_nodes[ids[i]]);
    }

    for (int i = 0; i < 3; i++) {
      for (int j = i + 1; j < 3; j++) {
        Edge temp_edge;
        temp_edge.appendNode(mesh_nodes[ids[i]]);
        temp_edge.appendNode(mesh_nodes[ids[j]]);
        temp_tri.appendEdge(temp_edge);
      }
    }
    triangles.push_back(temp_tri);
  };

  void generateFirst3Elements() {
    std::vector<int> ids;
    int myints[] = {0, 1, 4};
    ids.assign(myints, myints + 3);
    elements.push_back(ids);
    this->constructTriangle(ids);

    std::vector<int> ids2;
    int myints2[] = {2, 3, 4};
    ids2.assign(myints2, myints2 + 3);
    elements.push_back(ids2);
    this->constructTriangle(ids2);

    std::vector<int> ids3;
    int myints3[] = {1, 3, 4};
    ids3.assign(myints3, myints3 + 3);
    elements.push_back(ids3);
    this->constructTriangle(ids3);
  }
  void addElement(Node & node) {
    Node temp = node;
    int n = node.getid();
    //First check whether node is on the edge of rectangle;
    double xy = mesh_nodes[0].getx();
    double x = mesh_nodes[1].getx();
    double y = mesh_nodes[2].gety();
    // inside rectangle
    if (node.getx() != xy && node.getx() != x && node.gety() != xy && node.gety() != y) {
      //find which triangle it is in
      for (std::size_t i = 0; i < triangles.size(); i++) {
        if (triangles[i].IsPointInTriangle(node) == 1) {
          // In this situation, 3 new triangle is created.
          int n1 = triangles[i].getNodes()[0].getid();
          int n2 = triangles[i].getNodes()[1].getid();
          int n3 = triangles[i].getNodes()[2].getid();
          std::vector<int> ids;
          int myints[] = {n1, n, n2};
          ids.assign(myints, myints + 3);
          elements.push_back(ids);
          this->constructTriangle(ids);

          std::vector<int> ids2;
          int myints2[] = {n1, n, n3};
          ids2.assign(myints2, myints2 + 3);
          elements.push_back(ids2);
          this->constructTriangle(ids2);

          std::vector<int> ids3;

          int myints3[] = {n2, n, n3};
          ids3.assign(myints3, myints3 + 3);
          elements.push_back(ids3);
          this->constructTriangle(ids3);

          // Delete the outside triangle
          triangles.erase(triangles.begin() + i);
          elements.erase(elements.begin() + i);
          break;
        }
      }
    }
    // on the rectangle In this situation, 2 new triangle is created
    else if (node.getx() == xy || node.getx() == x || node.gety() == xy ||
             node.gety() == y) {
      for (std::size_t i = 0; i < triangles.size(); i++) {
        if (triangles[i].IsPointInTriangle(node) >= 1) {
          int n1 = triangles[i].getNodes()[0].getid();
          int n2 = triangles[i].getNodes()[1].getid();
          int n3 = triangles[i].getNodes()[2].getid();
          if ((triangles[i].getNodes()[1].getx() == node.getx() &&
               triangles[i].getNodes()[2].getx() == node.getx()) ||
              (triangles[i].getNodes()[1].gety() == node.gety() &&
               triangles[i].getNodes()[2].gety() == node.gety()))

          {
            std::vector<int> ids;
            int myints[] = {n1, n, n2};
            ids.assign(myints, myints + 3);
            elements.push_back(ids);
            this->constructTriangle(ids);

            std::vector<int> ids2;
            int myints2[] = {n1, n, n3};
            ids2.assign(myints2, myints2 + 3);
            elements.push_back(ids2);
            this->constructTriangle(ids2);
          }

          if ((triangles[i].getNodes()[0].getx() == node.getx() &&
               triangles[i].getNodes()[2].getx() == node.getx()) ||
              (triangles[i].getNodes()[0].gety() == node.gety() &&
               triangles[i].getNodes()[2].gety() == node.gety())) {
            std::vector<int> ids;
            int myints[] = {n2, n, n1};
            ids.assign(myints, myints + 3);
            elements.push_back(ids);
            this->constructTriangle(ids);

            std::vector<int> ids2;
            int myints2[] = {n2, n, n3};
            ids2.assign(myints2, myints2 + 3);
            elements.push_back(ids2);
            this->constructTriangle(ids2);
          }

          if ((triangles[i].getNodes()[1].getx() == node.getx() &&
               triangles[i].getNodes()[0].getx() == node.getx()) ||
              (triangles[i].getNodes()[1].gety() == node.gety() &&
               triangles[i].getNodes()[0].gety() == node.gety())) {
            std::vector<int> ids;
            int myints[] = {n3, n, n2};
            ids.assign(myints, myints + 3);
            elements.push_back(ids);
            this->constructTriangle(ids);

            std::vector<int> ids2;
            int myints2[] = {n3, n, n1};
            ids2.assign(myints2, myints2 + 3);
            elements.push_back(ids2);
            this->constructTriangle(ids2);
          }
          triangles.erase(triangles.begin() + i);
          elements.erase(elements.begin() + i);
          break;
        }
      }
    }
  };

  void printElements() {
    std::cout << "$elements" << '\n';
    for (std::size_t i = 0; i < elements.size(); i++) {
      std::cout << i + 1 << ' ' << elements[i][0] + 1 << ' ' << elements[i][1] + 1 << ' '
                << elements[i][2] + 1 << "\n";
    }
  }
  ~Triangulation(){};
};
