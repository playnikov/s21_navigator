#include "graph_algorithms.h"

#include <climits>
#include <cmath>

namespace s21 {

std::vector<int> GraphAlgorithms::DepthFirstSearch(Graph& graph,
                                                   int start_vertex) {
  if (start_vertex < 1 || start_vertex > graph.GetVertexCount()) {
    throw std::out_of_range("Invalid start vertex");
  }

  int n = graph.GetVertexCount();
  std::vector<bool> visited(n, false);
  std::vector<int> result;
  Stack<int> stack;

  stack.push(start_vertex - 1);

  while (!stack.empty()) {
    int current = stack.top();
    stack.pop();

    if (!visited[current]) {
      visited[current] = true;
      result.push_back(current + 1);

      for (int i = n - 1; i >= 0; --i) {
        if (graph.GetWeight(current + 1, i + 1) > 0 && !visited[i]) {
          stack.push(i);
        }
      }
    }
  }

  return result;
}

std::vector<int> GraphAlgorithms::BreadthFirstSearch(Graph& graph,
                                                     int start_vertex) {
  if (start_vertex < 1 || start_vertex > graph.GetVertexCount()) {
    throw std::out_of_range("Invalid start vertex");
  }

  int n = graph.GetVertexCount();
  std::vector<bool> visited(n, false);
  std::vector<int> result;
  Queue<int> queue;

  visited[start_vertex - 1] = true;
  queue.push(start_vertex - 1);

  while (!queue.empty()) {
    int current = queue.front();
    queue.pop();
    result.push_back(current + 1);

    for (int i = 0; i < n; ++i) {
      if (graph.GetWeight(current + 1, i + 1) > 0 && !visited[i]) {
        visited[i] = true;
        queue.push(i);
      }
    }
  }

  return result;
}

int GraphAlgorithms::GetShortestPathBetweenVertices(Graph& graph, int vertex1,
                                                    int vertex2) {
  int n = graph.GetVertexCount();
  if (vertex1 < 1 || vertex1 > n || vertex2 < 1 || vertex2 > n) {
    throw std::out_of_range("Invalid vertex indices");
  }

  std::vector<int> dist(n, INT_MAX);
  std::vector<bool> visited(n, false);

  dist[vertex1 - 1] = 0;

  bool flag = true;

  for (int i = 0; i < n - 1 && flag; i++) {
    int min_dist = INT_MAX;
    int min_index = -1;

    for (int v = 0; v < n; ++v) {
      if (!visited[v] && dist[v] <= min_dist) {
        min_dist = dist[v];
        min_index = v;
      }
    }

    if (min_index == -1) {
      flag = false;
    } else {
      visited[min_index] = true;

      for (int v = 0; v < n; ++v) {
        int weight = graph.GetWeight(min_index + 1, v + 1);
        if (!visited[v] && weight > 0 && dist[min_index] != INT_MAX &&
            dist[min_index] + weight < dist[v]) {
          dist[v] = dist[min_index] + weight;
        }
      }
    }
  }

  return (dist[vertex2 - 1] == INT_MAX) ? 0 : dist[vertex2 - 1];
}

std::vector<std::vector<int>>
GraphAlgorithms::GetShortestPathsBetweenAllVertices(Graph& graph) {
  int n = graph.GetVertexCount();
  std::vector<std::vector<int>> dist(n, std::vector<int>(n, 0));
  const auto& matrix = graph.GetAdjacencyMatrix();

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (i == j) {
        dist[i][j] = 0;
      } else if (matrix[i][j] > 0) {
        dist[i][j] = matrix[i][j];
      } else {
        dist[i][j] = INT_MAX;
      }
    }
  }

  for (int k = 0; k < n; k++) {
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        if (dist[i][k] != INT_MAX && dist[k][j] != INT_MAX &&
            dist[i][k] + dist[k][j] < dist[i][j]) {
          dist[i][j] = dist[i][k] + dist[k][j];
        }
      }
    }
  }

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (dist[i][j] == INT_MAX) {
        dist[i][j] = 0;
      }
    }
  }

  return dist;
}

std::vector<std::vector<int>> GraphAlgorithms::GetLeastSpanningTree(
    Graph& graph) {
  int n = graph.GetVertexCount();
  const auto& matrix = graph.GetAdjacencyMatrix();

  std::vector<std::vector<int>> mst(n, std::vector<int>(n, 0));
  std::vector<bool> in_mst(n, false);
  std::vector<int> key(n, INT_MAX);
  std::vector<int> parent(n, -1);

  key[0] = 0;

  bool flag = true;

  for (int i = 0; i < n - 1 && flag; i++) {
    int min_key = INT_MAX;
    int u = -1;

    for (int v = 0; v < n; v++) {
      if (!in_mst[v] && key[v] < min_key) {
        min_key = key[v];
        u = v;
      }
    }

    if (u == -1) {
      flag = false;
    } else {
      in_mst[u] = true;

      for (int v = 0; v < n; v++) {
        int weight = matrix[u][v];
        if (weight > 0 && !in_mst[v] && weight < key[v]) {
          key[v] = weight;
          parent[v] = u;
        }
      }
    }
  }

  for (int i = 0; i < n; i++) {
    if (parent[i] != -1) {
      int weight = matrix[i][parent[i]];
      mst[i][parent[i]] = weight;
      mst[parent[i]][i] = weight;
    }
  }

  return mst;
}

TsmResult GraphAlgorithms::SolveTravelingSalesmanProblem(Graph& graph) {
  int n = graph.GetVertexCount();
  if (n < 2) {
    throw std::runtime_error("Graph must have at least 2 vertices");
  }

  std::vector<int> bfs_result = BreadthFirstSearch(graph, 1);
  if (static_cast<int>(bfs_result.size()) != n) {
    throw std::runtime_error("Graph is not connected");
  }

  const int MAX_ITERATIONS = 100;
  const int NUM_ANTS = 50;
  const double ALPHA = 1.0;
  const double BETA = 2.0;
  const double EVA = 0.5;
  const double Q = 100.0;

  std::vector<std::vector<double>> pheromones(n, std::vector<double>(n, 1.0));

  TsmResult best_result;
  best_result.distance = std::numeric_limits<double>::max();

  for (int i = 0; i < MAX_ITERATIONS; i++) {
    std::vector<std::vector<double>> local_update(
        n, std::vector<double>(n, 0.0));
    std::vector<Ant> ants = InitializeAnts(n, NUM_ANTS);
  }
}

std::vector<Ant> GraphAlgorithms::InitializeAnts(int num_vertices,
                                                 int num_ants) {
  std::vector<Ant> ants;
  ants.reserve(num_ants);

  for (int i = 0; i < num_ants; i++) {
    ants.emplace_back(num_vertices);
  }

  return ants;
}

}  // namespace s21