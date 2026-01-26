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

  file.close();
  return true;
}

bool Graph::ExportGraphToDot(const std::string& filename) const {
  std::ofstream file(filename);
  if (!file.is_open()) {
    throw std::runtime_error("Cannot open file: " + filename);
  }

  return true;
}

int Graph::GetWeight(int from, int to) const {
    if (from < 1 || from > vertex_count_ || to < 1 || to > vertex_count_) {
        throw std::out_of_range("Vertex index out of range");
    }
    return adjacency_matrix_[from - 1][to - 1];
}

}  // namespace s21