#include "graph.h"

namespace s21 {

bool Graph::LoadGraphFromFile(const std::string& filename) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    throw std::runtime_error("Cannot open file: " + filename);
  }

  int size;
  file >> size;

  if (size <= 0) {
    throw std::runtime_error("Invalid graph size");
  }

  adjacency_matrix_.resize(size, std::vector<int>(size, 0));
  vertex_count_ = size;

  for (int i = 0; i < size; ++i) {
    for (int j = 0; j < size; ++j) {
      if (!(file >> adjacency_matrix_[i][j])) {
        throw std::runtime_error("Invalid data in file");
      }
      if (adjacency_matrix_[i][j] < 0) {
        throw std::runtime_error("Negative weights are not allowed");
      }
    }
  }

  int extra;
  if (file >> extra) {
    throw std::runtime_error("File contains extra data beyond matrix");
  }

  file.close();
  return true;
}

bool Graph::ExportGraphToDot(const std::string& filename) const {
  std::ofstream file(filename);
  if (!file.is_open()) {
    throw std::runtime_error("Cannot open file: " + filename);
  }

  bool directed = IsDirected();

  if (directed) {
    file << "digraph G {\n";
  } else {
    file << "graph G {\n";
  }

  for (int i = 1; i <= vertex_count_; ++i) {
    file << "  " << i << ";\n";
  }

  for (int i = 0; i < vertex_count_; ++i) {
    for (int j = (directed ? 0 : i + 1); j < vertex_count_; ++j) {
      int weight = adjacency_matrix_[i][j];

      if (weight > 0) {
        if (directed) {
          file << "  " << (i + 1) << " -> " << (j + 1);
        } else {
          file << "  " << (i + 1) << " -- " << (j + 1);
        }

        if (weight != 1) {
          file << " [label=\"" << weight << "\"]";
        }
        file << ";\n";
      }
    }
  }

  file << "}\n";
  file.close();

  return true;
}

int Graph::GetWeight(int from, int to) const {
  if (from < 1 || from > vertex_count_ || to < 1 || to > vertex_count_) {
    throw std::out_of_range("Vertex index out of range");
  }
  return adjacency_matrix_[from - 1][to - 1];
}

void Graph::SetAdjacencyMatrix(const std::vector<std::vector<int>>& matrix) {
  if (matrix.empty()) {
    vertex_count_ = 0;
    adjacency_matrix_.clear();
    return;
  }

  size_t size = matrix.size();
  for (const auto& row : matrix) {
    if (row.size() != size) {
      throw std::invalid_argument("Matrix must be square");
    }
  }

  vertex_count_ = size;
  adjacency_matrix_ = matrix;
}

bool Graph::IsDirected() const {
  bool flag = false;
  for (int i = 0; i < vertex_count_ && !flag; ++i) {
    for (int j = i + 1; j < vertex_count_ && !flag; ++j) {
      if (adjacency_matrix_[i][j] != adjacency_matrix_[j][i]) {
        flag = true;
      }
    }
  }
  return flag;
}

}  // namespace s21