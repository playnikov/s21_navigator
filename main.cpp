#include <iostream>
#include <vector>

#include "s21_graph/graph.h"
#include "s21_graph_algorithms/graph_algorithms.h"

void PrintMenu();
void PrintVector(const std::vector<int>& vec);
void PrintMatrix(const std::vector<std::vector<int>>& matrix);

using namespace s21;

int main() {
  Graph graph;
  GraphAlgorithms algorithms;

  int choice;

  std::string filename;

  do {
    PrintMenu();
    std::cin >> choice;

    try {
      switch (choice) {
        case 1:
          std::cout << "Enter filename: ";
          std::cin >> filename;
          graph.LoadGraphFromFile(filename);
          std::cout << "Graph loaded successfully. Vertices: "
                    << graph.GetVertexCount() << "\n";
          graph.PrintMatrix();
          break;

        case 2:
          if (graph.GetVertexCount() == 0) {
            std::cout << "Please load a graph\n";
          } else {
            int start_vertex;
            std::cout << "Enter start vertex (1-based): ";
            std::cin >> start_vertex;
            auto result = algorithms.DepthFirstSearch(graph, start_vertex);
            std::cout << "DFS result: ";
            PrintVector(result);
          }
          break;

        case 3:
          if (graph.GetVertexCount() == 0) {
            std::cout << "Please load a graph\n";
          } else {
            int start_vertex;
            std::cout << "Enter start vertex (1-based): ";
            std::cin >> start_vertex;
            auto result = algorithms.BreadthFirstSearch(graph, start_vertex);
            std::cout << "BFS result: ";
            PrintVector(result);
          }
          break;

        case 4:
          if (graph.GetVertexCount() == 0) {
            std::cout << "Please load a graph\n";
          } else {
            int v1, v2;
            std::cout << "Enter vertex 1: ";
            std::cin >> v1;
            std::cout << "Enter vertex 2: ";
            std::cin >> v2;
            int distance =
                algorithms.GetShortestPathBetweenVertices(graph, v1, v2);
            std::cout << "Shortest path distance: " << distance << "\n";
          }
          break;

        case 5:
          if (graph.GetVertexCount() == 0) {
            std::cout << "Please load a graph\n";
          } else {
            auto result = algorithms.GetShortestPathsBetweenAllVertices(graph);
            std::cout << "Shortest paths matrix:\n";
            PrintMatrix(result);
          }
          break;
        
        case 6:
          if (graph.GetVertexCount() == 0) {
            std::cout << "Please load a graph\n";
          } else {
            auto result = algorithms.GetLeastSpanningTree(graph);
            std::cout << "Minimum spanning tree adjacency matrix:\n";
            PrintMatrix(result);
          }
          break;

        case 0:
          std::cout << "Goodbye!\n";
          break;
        default:
          std::cout << "Invalid choice!\n";
      }
    } catch (const std::exception& e) {
      std::cerr << e.what() << '\n';
    }

  } while (choice != 0);

  return 0;
}

void PrintMenu() {
  std::cout << "1. Load the original graph from a file\n";
  std::cout << "2. Traverse the graph in breadth and print the result to the "
               "console\n";
  std::cout
      << "3. Traverse the graph in depth and print the result to the console\n";
  std::cout << "4. Find the shortest path between any two vertices and print "
               "the result to the console\n";
  std::cout << "5. Find the shortest paths between all pairs of vertices in "
               "the graph and print the result matrix to the console\n";
  std::cout << "6. Search for the minimum spanning tree in the graph and print "
               "the resulting adjacency matrix to the console\n";
  std::cout << "7. Solve the Salesman problem, with output of the resulting "
               "route and its length to the console\n";
  std::cout << "0. Exit\n";
  std::cout << "Enter your choice: ";
}

void PrintVector(const std::vector<int>& vec) {
  std::cout << "[";
  for (size_t i = 0; i < vec.size(); ++i) {
    std::cout << vec[i];
    if (i < vec.size() - 1) std::cout << " ";
  }
  std::cout << "]\n";
}

void PrintMatrix(const std::vector<std::vector<int>>& matrix) {
  for (const auto& row : matrix) {
    for (const auto& val : row) {
      std::cout << val << " ";
    }
    std::cout << "\n";
  }
}