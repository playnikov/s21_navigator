#ifndef S21_GRAPH
#define S21_GRAPH

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace s21 {

class Graph {
 private:
  std::vector<std::vector<int>> adjacency_matrix_;
  int vertex_count_;

 public:
  Graph() : vertex_count_(0) {}

  explicit Graph(int vertices) : vertex_count_(vertices) {
    adjacency_matrix_.resize(vertices, std::vector<int>(vertices, 0));
  }

  bool LoadGraphFromFile(const std::string& filename);
  bool ExportGraphToDot(const std::string& filename) const;

  int GetVertexCount() const { return vertex_count_; }

  const std::vector<std::vector<int>>& GetAdjacencyMatrix() const {
    return adjacency_matrix_;
  }

  int GetWeight(int from, int to) const;
  bool IsDirected() const;
  void SetAdjacencyMatrix(const std::vector<std::vector<int>>& matrix);
};

}  // namespace s21

#endif