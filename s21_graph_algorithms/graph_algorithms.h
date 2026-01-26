#ifndef S21_GRAPH_ALGORITHMS_H
#define S21_GRAPH_ALGORITHMS_H

#include <limits>
#include <vector>

#include "../s21_containers/queue.h"
#include "../s21_containers/stack.h"
#include "../s21_graph/graph.h"

namespace s21 {

struct TsmResult {
  std::vector<int> vertices;
  double distance;

  TsmResult() : distance(0.0) {}
  TsmResult(const std::vector<int>& vertices, double distance)
      : vertices(vertices), distance(distance) {}
};

struct Ant {
  std::vector<int> path;
  std::vector<int> visited;

  double path_length;

  Ant(int num_vertices) : visited(num_vertices, false), path_length(0.0) {}
};

class GraphAlgorithms {
 private:
  std::vector<Ant> InitializeAnts(int num_vertices, int num_ants);
 public:
  // Part 1: Обход графа в глубину и в ширину
  std::vector<int> DepthFirstSearch(Graph& graph, int start_vertex);
  std::vector<int> BreadthFirstSearch(Graph& graph, int start_vertex);

  // Part 2: Поиск кратчайших путей в графе
  int GetShortestPathBetweenVertices(Graph& graph, int vertex1, int vertex2);
  std::vector<std::vector<int>> GetShortestPathsBetweenAllVertices(
      Graph& graph);

  // Part 3: Поиск минимального остовного дерева
  std::vector<std::vector<int>> GetLeastSpanningTree(Graph& graph);

  // Part 4: Задача коммивояжера
  TsmResult SolveTravelingSalesmanProblem(Graph& graph);
};
}  // namespace s21

#endif