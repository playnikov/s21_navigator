#include "graph_algorithms.h"

#include <climits>
#include <cmath>

namespace s21 {

std::vector<int> GraphAlgorithms::DepthFirstSearch(Graph &graph,
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

std::vector<int> GraphAlgorithms::BreadthFirstSearch(Graph &graph,
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

int GraphAlgorithms::GetShortestPathBetweenVertices(Graph &graph, int vertex1,
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
GraphAlgorithms::GetShortestPathsBetweenAllVertices(Graph &graph) {
  int n = graph.GetVertexCount();
  std::vector<std::vector<int>> dist(n, std::vector<int>(n, 0));
  const auto &matrix = graph.GetAdjacencyMatrix();

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
    Graph &graph) {
  int n = graph.GetVertexCount();
  const auto &matrix = graph.GetAdjacencyMatrix();

  std::vector<std::vector<int>> mst(n, std::vector<int>(n, 0));

  if (n > 1) {
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
  }

  return mst;
}

std::vector<Ant> GraphAlgorithms::InitializeAnts(int num_vertices,
                                                 int num_ants) {
  std::vector<Ant> ants;
  ants.reserve(num_ants);

  for (int i = 0; i < num_ants; i++) {
    Ant ant(num_vertices);

    int start_vertex = i % num_vertices;

    ant.path.push_back(start_vertex);
    ant.visited[start_vertex] = true;
    ants.push_back(ant);
  }

  return ants;
}

void GraphAlgorithms::UpdatePheromones(
    std::vector<std::vector<double>> &pheromones, const std::vector<Ant> &ants,
    double evaporation_rate, double Q) {
  size_t n = pheromones.size();

  for (size_t i = 0; i < n; i++) {
    for (size_t j = 0; j < n; j++) {
      if (i != j) {
        pheromones[i][j] *= (1.0 - evaporation_rate);
        if (pheromones[i][j] < 0.001) {
          pheromones[i][j] = 0.001;
        }
      }
    }
  }

  for (const auto &ant : ants) {
    if (ant.path.size() == (n + 1) && ant.path_length > 0) {
      double delta = Q / ant.path_length;
      for (size_t i = 0; i < ant.path.size() - 1; ++i) {
        int from = ant.path[i];
        int to = ant.path[i + 1];
        if (from >= 0 && from < static_cast<int>(n) && to >= 0 &&
            to < static_cast<int>(n)) {
          pheromones[from][to] += delta;
          pheromones[to][from] += delta;
        }
      }
    }
  }
}

int GraphAlgorithms::SelectNextVertex(
    const Ant &ant, const std::vector<std::vector<double>> &pheromones,
    const std::vector<std::vector<int>> &distances, double alpha, double beta,
    std::mt19937 &gen) {
  int current = ant.path.back();
  int n = distances.size();

  int selected_vertex = -1;
  bool flag = true;

  std::vector<double> probabilities(n, 0.0);
  double sum = 0.0;

  for (int i = 0; i < n; ++i) {
    if (!ant.visited[i] && distances[current][i] > 0) {
      double pheromone = std::pow(pheromones[current][i], alpha);
      double heuristic = std::pow(1.0 / distances[current][i], beta);
      probabilities[i] = pheromone * heuristic;
      sum += probabilities[i];
    }
  }

  if (sum > 0.0) {
    for (int i = 0; i < n; ++i) {
      probabilities[i] /= sum;
    }

    std::uniform_real_distribution<double> dist(0.0, 1.0);
    double r = dist(gen);
    double cumulative = 0.0;

    for (int i = 0; i < n && flag; ++i) {
      if (!ant.visited[i] && distances[current][i] > 0) {
        cumulative += probabilities[i];
        if (r <= cumulative) {
          selected_vertex = i;
          flag = false;
        }
      }
    }

    for (int i = 0; i < n && flag; ++i) {
      if (!ant.visited[i] && distances[current][i] > 0) {
        selected_vertex = i;
        flag = false;
      }
    }
  }

  return selected_vertex;
}

bool GraphAlgorithms::IsHamiltonianCycle(const std::vector<int> &path,
                                         int num_vertices) {
  bool is_cycle = true;

  if (static_cast<int>(path.size()) != num_vertices + 1) {
    is_cycle = false;
  } else if (path.front() != path.back()) {
    is_cycle = false;
  } else {
    std::vector<bool> visited(num_vertices, false);
    for (size_t i = 0; i < path.size() - 1 && is_cycle; ++i) {
      int v = path[i] - 1;
      if (v < 0 || v >= num_vertices || visited[v]) {
        is_cycle = false;
      }
      visited[v] = true;
    }

    for (size_t j = 0; j < visited.size() && is_cycle; j++) {
      if (!visited[j]) is_cycle = false;
    }
  }

  return is_cycle;
}

TsmResult GraphAlgorithms::SolveTravelingSalesmanProblem(Graph &graph) {
  int n = graph.GetVertexCount();
  const auto &distances = graph.GetAdjacencyMatrix();

  CheckGraphForTSP(n, distances);

  const int num_ants = std::max(n * 2, 10);
  const int max_iterations = 100;
  const double alpha = 1.0;
  const double beta = 2.0;
  const double evaporation_rate = 0.5;
  const double Q = 100.0;

  std::vector<std::vector<double>> pheromones = InitializePheromones(n);

  std::random_device rd;
  std::mt19937 gen(rd());

  TsmResult best_result;
  best_result.distance = std::numeric_limits<double>::max();
  best_result.vertices = std::vector<int>();

  for (int iteration = 0; iteration < max_iterations; ++iteration) {
    std::vector<Ant> ants = InitializeAnts(n, num_ants);

    for (Ant &ant : ants) {
      BuildAntPath(ant, n, pheromones, distances, alpha, beta, gen);
      if (static_cast<int>(ant.path.size()) == n) {
        CompleteAntPath(ant, distances, best_result);
      }
    }

    UpdatePheromones(pheromones, ants, evaporation_rate, Q);
  }

  if (best_result.vertices.empty()) {
    throw std::runtime_error("Could not find a Hamiltonian cycle in the graph");
  }

  return best_result;
}

void GraphAlgorithms::CheckGraphForTSP(
    int n, const std::vector<std::vector<int>> &distances) {
  if (n < 2) {
    throw std::runtime_error("Graph must have at least 2 vertices for TSP");
  }
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      if (i != j && distances[i][j] == 0) {
        throw std::runtime_error(
            "Graph must be complete for TSP (all edges must have positive "
            "weights)");
      }
    }
  }
}

std::vector<std::vector<double>> GraphAlgorithms::InitializePheromones(int n) {
  std::vector<std::vector<double>> pheromones(n, std::vector<double>(n, 1.0));
  for (int i = 0; i < n; ++i) {
    pheromones[i][i] = 0.0;
  }
  return pheromones;
}

void GraphAlgorithms::BuildAntPath(
    Ant &ant, int n, const std::vector<std::vector<double>> &pheromones,
    const std::vector<std::vector<int>> &distances, double alpha, double beta,
    std::mt19937 &gen) {
  bool valid_path = true;
  for (int step = 1; step < n && valid_path; ++step) {
    int next = SelectNextVertex(ant, pheromones, distances, alpha, beta, gen);
    if (next == -1) {
      ant.path.clear();
      ant.path_length = 0.0;
      valid_path = false;
    } else {
      ant.path.push_back(next);
      ant.visited[next] = true;
      int prev = ant.path[ant.path.size() - 2];
      ant.path_length += distances[prev][next];
    }
  }
}

void GraphAlgorithms::CompleteAntPath(
    Ant &ant, const std::vector<std::vector<int>> &distances,
    TsmResult &best_result) {
  int last = ant.path.back();
  int first = ant.path.front();

  if (distances[last][first] > 0) {
    ant.path.push_back(first);

    ant.path_length += distances[last][first];

    if (ant.path_length < best_result.distance) {
      best_result.distance = ant.path_length;
      best_result.vertices.clear();

      for (int v : ant.path) {
        best_result.vertices.push_back(v + 1);
      }
    }
  }
}

}  // namespace s21