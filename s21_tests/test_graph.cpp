#include <gtest/gtest.h>

#include <fstream>
#include <vector>

#include "../s21_graph/graph.h"

namespace s21 {
class GraphTest : public ::testing::Test {
 protected:
  void SetUp() override {
    createValidGraphFile();
    createInvalidGraphFile();
    createNegativeWeightFile();
    createNonNumericFile();
    createNonSquareFile();
    createExtraDataFile();
    createDirectedGraphFile();
    createWeightedGraphFile();
    createNegativeSizeFile();
    createInvalidDataInFile();
  }

  void TearDown() override {
    remove("valid_graph.txt");
    remove("invalid_graph.txt");
    remove("negative_weight.txt");
    remove("non_numeric.txt");
    remove("non_square.txt");
    remove("extra_data.txt");
    remove("directed_graph.txt");
    remove("weighted_graph.txt");
    remove("test_export.dot");
    remove("directed_export.dot");
    remove("negative_size.txt");
    remove("invalid_data_file.txt");
  }

  void createValidGraphFile() {
    std::ofstream file("valid_graph.txt");
    file << "3\n";
    file << "0 1 2\n";
    file << "1 0 3\n";
    file << "2 3 0\n";
    file.close();
  }

  void createInvalidGraphFile() {
    std::ofstream file("invalid_graph.txt");
    file << "3\n";
    file << "0 1 2\n";
    file << "1 0\n";
    file.close();
  }

  void createNegativeWeightFile() {
    std::ofstream file("negative_weight.txt");
    file << "3\n";
    file << "0 1 -2\n";
    file << "1 0 3\n";
    file << "2 3 0\n";
    file.close();
  }

  void createNonNumericFile() {
    std::ofstream file("non_numeric.txt");
    file << "3\n";
    file << "0 a 2\n";
    file << "1 0 3\n";
    file << "2 3 0\n";
    file.close();
  }

  void createNonSquareFile() {
    std::ofstream file("non_square.txt");
    file << "3\n";
    file << "0 1 2 4\n";
    file << "1 0 3\n";
    file << "2 3 0\n";
    file.close();
  }

  void createExtraDataFile() {
    std::ofstream file("extra_data.txt");
    file << "3\n";
    file << "0 1 2\n";
    file << "1 0 3\n";
    file << "2 3 0\n";
    file << "extra data\n";
    file.close();
  }

  void createDirectedGraphFile() {
    std::ofstream file("directed_graph.txt");
    file << "3\n";
    file << "0 1 0\n";
    file << "0 0 1\n";
    file << "1 0 0\n";
    file.close();
  }

  void createWeightedGraphFile() {
    std::ofstream file("weighted_graph.txt");
    file << "4\n";
    file << "0 5 0 2\n";
    file << "5 0 3 0\n";
    file << "0 3 0 7\n";
    file << "2 0 7 0\n";
    file.close();
  }

  void createNegativeSizeFile() {
    std::ofstream file("negative_size.txt");
    file << "-3\n";
    file << "0 5 0 2\n";
    file << "5 0 3 0\n";
    file << "0 3 0 7\n";
    file << "2 0 7 0\n";
    file.close();
  }

  void createInvalidDataInFile() {
    std::ofstream file("invalid_data_file.txt");
    file << "4\n";
    file << "0 5 0 2\n";
    file << "5 g 3 0\n";
    file << "0 3 0 7\n";
    file << "2 0 7 0\n";
    file.close();
  }
};

TEST_F(GraphTest, DefaultConstructor) {
  Graph g;
  EXPECT_EQ(g.GetVertexCount(), 0);
  EXPECT_TRUE(g.GetAdjacencyMatrix().empty());
}

TEST_F(GraphTest, ParameterizedConstructor) {
  Graph g(5);
  EXPECT_EQ(g.GetVertexCount(), 5);

  const auto& matrix = g.GetAdjacencyMatrix();
  EXPECT_EQ(matrix.size(), 5);

  for (const auto& row : matrix) {
    EXPECT_EQ(row.size(), 5);
    for (int val : row) {
      EXPECT_EQ(val, 0);
    }
  }
}

TEST_F(GraphTest, ZeroSizeConstructor) {
  Graph g(0);
  EXPECT_EQ(g.GetVertexCount(), 0);
  EXPECT_TRUE(g.GetAdjacencyMatrix().empty());
}

TEST_F(GraphTest, LoadValidGraph) {
  Graph g;
  EXPECT_TRUE(g.LoadGraphFromFile("valid_graph.txt"));
  EXPECT_EQ(g.GetVertexCount(), 3);

  const auto& matrix = g.GetAdjacencyMatrix();
  EXPECT_EQ(matrix[0][0], 0);
  EXPECT_EQ(matrix[0][1], 1);
  EXPECT_EQ(matrix[0][2], 2);
  EXPECT_EQ(matrix[1][0], 1);
  EXPECT_EQ(matrix[1][1], 0);
  EXPECT_EQ(matrix[1][2], 3);
  EXPECT_EQ(matrix[2][0], 2);
  EXPECT_EQ(matrix[2][1], 3);
  EXPECT_EQ(matrix[2][2], 0);
}

TEST_F(GraphTest, LoadNonExistentFile) {
  Graph g;
  EXPECT_THROW(g.LoadGraphFromFile("nonexistent.txt"), std::runtime_error);
}

TEST_F(GraphTest, LoadInvalidData) {
  Graph g;
  EXPECT_THROW(g.LoadGraphFromFile("invalid_data.txt"), std::runtime_error);
}

TEST_F(GraphTest, LoadInvalidDataFile) {
  Graph g;
  EXPECT_THROW(g.LoadGraphFromFile("invalid_data_file.txt"),
               std::runtime_error);
}

TEST_F(GraphTest, LoadInvalidSize) {
  Graph g;
  EXPECT_THROW(g.LoadGraphFromFile("invalid_size.txt"), std::runtime_error);
}

TEST_F(GraphTest, LoadNegativeWeight) {
  Graph g;
  EXPECT_THROW(g.LoadGraphFromFile("negative_weight.txt"), std::runtime_error);
}

TEST_F(GraphTest, LoadNonSquareMatrix) {
  Graph g;
  EXPECT_THROW(g.LoadGraphFromFile("non_square.txt"), std::runtime_error);
}

TEST_F(GraphTest, LoadNegativeSize) {
  Graph g;
  EXPECT_THROW(g.LoadGraphFromFile("negative_size.txt"), std::runtime_error);
}

TEST_F(GraphTest, GetWeight) {
  Graph g(3);

  std::vector<std::vector<int>> matrix = {{0, 5, 10}, {5, 0, 15}, {10, 15, 0}};
  g.SetAdjacencyMatrix(matrix);

  EXPECT_EQ(g.GetWeight(1, 1), 0);
  EXPECT_EQ(g.GetWeight(1, 2), 5);
  EXPECT_EQ(g.GetWeight(1, 3), 10);
  EXPECT_EQ(g.GetWeight(2, 1), 5);
  EXPECT_EQ(g.GetWeight(2, 2), 0);
  EXPECT_EQ(g.GetWeight(2, 3), 15);
  EXPECT_EQ(g.GetWeight(3, 1), 10);
  EXPECT_EQ(g.GetWeight(3, 2), 15);
  EXPECT_EQ(g.GetWeight(3, 3), 0);
}

TEST_F(GraphTest, GetWeightInvalidIndices) {
  Graph g(3);

  EXPECT_THROW(g.GetWeight(0, 1), std::out_of_range);
  EXPECT_THROW(g.GetWeight(1, 0), std::out_of_range);
  EXPECT_THROW(g.GetWeight(4, 1), std::out_of_range);
  EXPECT_THROW(g.GetWeight(1, 4), std::out_of_range);
  EXPECT_THROW(g.GetWeight(0, 0), std::out_of_range);
}

TEST_F(GraphTest, IsDirected) {
  Graph g;

  std::vector<std::vector<int>> undirected = {{0, 1, 1}, {1, 0, 1}, {1, 1, 0}};
  g.SetAdjacencyMatrix(undirected);
  EXPECT_FALSE(g.IsDirected());

  g.LoadGraphFromFile("directed_graph.txt");
  EXPECT_TRUE(g.IsDirected());

  Graph empty;
  EXPECT_FALSE(empty.IsDirected());
}

TEST_F(GraphTest, ExportUndirectedGraphToDot) {
  Graph g;

  g.LoadGraphFromFile("valid_graph.txt");

  EXPECT_TRUE(g.ExportGraphToDot("test_export.dot"));

  std::ifstream file("test_export.dot");
  EXPECT_TRUE(file.is_open());

  std::string content((std::istreambuf_iterator<char>(file)),
                      std::istreambuf_iterator<char>());

  EXPECT_NE(content.find("graph G {"), std::string::npos);
  EXPECT_NE(content.find("1;"), std::string::npos);
  EXPECT_NE(content.find("2;"), std::string::npos);
  EXPECT_NE(content.find("3;"), std::string::npos);
  EXPECT_NE(content.find("1 -- 2"), std::string::npos);
  EXPECT_NE(content.find("1 -- 3"), std::string::npos);
  EXPECT_NE(content.find("2 -- 3"), std::string::npos);

  file.close();
}

TEST_F(GraphTest, ExportDirectedGraphToDot) {
  Graph g;
  g.LoadGraphFromFile("directed_graph.txt");

  EXPECT_TRUE(g.ExportGraphToDot("directed_export.dot"));

  std::ifstream file("directed_export.dot");
  EXPECT_TRUE(file.is_open());

  std::string content((std::istreambuf_iterator<char>(file)),
                      std::istreambuf_iterator<char>());

  EXPECT_NE(content.find("digraph G {"), std::string::npos);
  EXPECT_NE(content.find("1 -> 2"), std::string::npos);
  EXPECT_NE(content.find("2 -> 3"), std::string::npos);
  EXPECT_NE(content.find("3 -> 1"), std::string::npos);

  file.close();
}

TEST_F(GraphTest, ExportWeightedGraphToDot) {
  Graph g;
  g.LoadGraphFromFile("weighted_graph.txt");

  EXPECT_TRUE(g.ExportGraphToDot("weighted_export.dot"));

  std::ifstream file("weighted_export.dot");
  EXPECT_TRUE(file.is_open());

  std::string content((std::istreambuf_iterator<char>(file)),
                      std::istreambuf_iterator<char>());

  EXPECT_NE(content.find("1 -- 2 [label=\"5\"]"), std::string::npos);
  EXPECT_NE(content.find("1 -- 4 [label=\"2\"]"), std::string::npos);
  EXPECT_NE(content.find("2 -- 3 [label=\"3\"]"), std::string::npos);
  EXPECT_NE(content.find("3 -- 4 [label=\"7\"]"), std::string::npos);

  file.close();
}

TEST_F(GraphTest, ExportEmptyGraphToDot) {
  Graph g;

  EXPECT_TRUE(g.ExportGraphToDot("empty_export.dot"));

  std::ifstream file("empty_export.dot");
  EXPECT_TRUE(file.is_open());

  std::string content((std::istreambuf_iterator<char>(file)),
                      std::istreambuf_iterator<char>());

  EXPECT_NE(content.find("graph G {"), std::string::npos);
  EXPECT_EQ(content.find(";"), std::string::npos);
  file.close();
  remove("empty_export.dot");
}

TEST_F(GraphTest, ExportToInvalidFile) {
  Graph g;
  g.LoadGraphFromFile("valid_graph.txt");

  EXPECT_THROW(g.ExportGraphToDot("/nonexistent/directory/file.dot"),
               std::runtime_error);
}

TEST_F(GraphTest, SetAdjacencyMatrixEmpty) {
  Graph g(5);

  std::vector<std::vector<int>> empty;
  g.SetAdjacencyMatrix(empty);

  EXPECT_EQ(g.GetVertexCount(), 0);
  EXPECT_TRUE(g.GetAdjacencyMatrix().empty());

  const auto& matrix = g.GetAdjacencyMatrix();
  EXPECT_EQ(matrix.size(), 0);
}

TEST_F(GraphTest, SetAdjacencyMatrixNonSquare) {
  Graph g;

  std::vector<std::vector<int>> nonSquare1 = {{0, 1, 2}, {1, 0}};

  EXPECT_THROW(g.SetAdjacencyMatrix(nonSquare1), std::invalid_argument);

  EXPECT_EQ(g.GetVertexCount(), 0);

  std::vector<std::vector<int>> nonSquare2 = {
      {0, 1, 2, 3}, {1, 0, 1, 0}, {2, 1, 0, 1}};

  EXPECT_THROW(g.SetAdjacencyMatrix(nonSquare2), std::invalid_argument);

  EXPECT_EQ(g.GetVertexCount(), 0);
}

}  // namespace s21