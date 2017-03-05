#include <iostream>
#include "graphLoader.h"
#include "graph.h"
#include "edge.h"

using namespace std;

int main(int args, char * argv[])
{
  cout << "Start of application. Number of arguments: " << args << endl;

  if (args < 2) return 1;

  Graph * graph = GraphLoader::LoadGraph(argv[1]);

  return 0;
}
