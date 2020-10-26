#include "matplotlibcpp.h"
namespace plt = matplotlibcpp;

#include <math.h> /* fmod */
#include <stdio.h>

#include <cstdlib>
#include <exception>
#include <fstream>  // Save to local file.
#include <iomanip>  // for std::setprecision(2)
#include <iostream>
#include <vector>

#include "mesh.h"

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
  Surface surface;
  Triangulation triangulation;
  try {
    surface.readfile(f);
    surface.print_nodes();
    for (std::size_t i = 0; i < surface.getAllNodes().size(); i++) {
      Node temp = surface.getAllNodes()[i];
      triangulation.appendNode(temp);
    }
    std::vector<int> ids;
    int myints[] = {0, 1, 4};
    ids.assign(myints, myints + 3);

    //说明以下code可行
    /*
    Triangle temp_tri;
    for (int i = 0; i < 3; i++) {
      temp_tri.appendNode(surface[ids[i]]);
    }

    for (int i = 0; i < 3; i++) {
      for (int j = i + 1; j < 3; j++) {
        Edge temp_edge;
        temp_edge.appendNode(surface[ids[i]]);
        temp_edge.appendNode(surface[ids[j]]);
        temp_tri.appendEdge(temp_edge);
      }
    }
      */
    //换成 reference input就好了！
    /*
    triangulation.constructTriangle(ids);
    Triangle tri = triangulation[0];
    */

    triangulation.generateFirst3Elements();

    for (std::size_t i = 5; i < surface.getAllNodes().size(); i++) {
      Node temp = surface.getAllNodes()[i];
      triangulation.addElement(temp);
      triangulation.printElements();
    }
    triangulation.printElements();
  }

  catch (std::exception & e) {
    std::cout << "An exception (" << e.what() << ") occured!\n";
  }

  ///plot the mesh
  plt::plot({1, 3, 2, 4});
  plt::show();

  return (EXIT_SUCCESS);
}
