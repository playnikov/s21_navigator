
#include <gtest/gtest.h>

#include <algorithm>
#include <fstream>
#include <vector>

#include "../s21_graph_algorithms/graph_algorithms.h"

namespace s21 {

class GraphAlgorithmsTest : public ::testing::Test {
 protected:
  void SetUp() override {
    createSimpleGraph();
    createCompleteGraph4();
    createCompleteGraph5();
    createDisconnectedGraph();
    createDirectedGraph();
    createWeightedGraph();
    createGraphForMST();
    createGraphForTSP();
    createInvalidGraphForTSP();
  }

  void TearDown() override {
    remove("simple_graph.txt");
    remove("complete_4.txt");
    remove("complete_5.txt");
    remove("disconnected_graph.txt");
    remove("directed_graph.txt");
    remove("weighted_graph.txt");
    remove("mst_graph.txt");
    remove("tsp_graph.txt");
    remove("invalid_tsp_graph.txt");
  }

  void createSimpleGraph() {
    std::ofstream file("simple_graph.txt");
    file << "4\n";
    file << "0 1 1 0\n";
    file << "1 0 1 1\n";
    file << "1 1 0 0\n";
    file << "0 1 0 0\n";
    file.close();
  }

  void createCompleteGraph4() {
    std::ofstream file("complete_4.txt");
    file << "4\n";
    file << "0 10 15 20\n";
    file << "10 0 35 25\n";
    file << "15 35 0 30\n";
    file << "20 25 30 0\n";
    file.close();
  }

  void createCompleteGraph5() {
    std::ofstream file("complete_5.txt");
    file << "5\n";
    file << "0 12 18 25 30\n";
    file << "12 0 22 28 20\n";
    file << "18 22 0 32 24\n";
    file << "25 28 32 0 26\n";
    file << "30 20 24 26 0\n";
    file.close();
  }

  void createDisconnectedGraph() {
    std::ofstream file("disconnected_graph.txt");
    file << "5\n";
    file << "0 1 0 0 0\n";
    file << "1 0 0 0 0\n";
    file << "0 0 0 1 1\n";
    file << "0 0 1 0 1\n";
    file << "0 0 1 1 0\n";
    file.close();
  }

  void createDirectedGraph() {
    std::ofstream file("directed_graph.txt");
    file << "3\n";
    file << "0 1 0\n";
    file << "0 0 1\n";
    file << "1 0 0\n";
    file.close();
  }

  void createWeightedGraph() {
    std::ofstream file("weighted_graph.txt");
    file << "4\n";
    file << "0 5 0 2\n";
    file << "5 0 3 0\n";
    file << "0 3 0 7\n";
    file << "2 0 7 0\n";
    file.close();
  }

  void createGraphForMST() {
    std::ofstream file("mst_graph.txt");
    file << "5\n";
    file << "0 2 0 6 0\n";
    file << "2 0 3 8 5\n";
    file << "0 3 0 0 7\n";
    file << "6 8 0 0 9\n";
    file << "0 5 7 9 0\n";
    file.close();
  }

  void createGraphForTSP() {
    std::ofstream file("tsp_graph.txt");
    file << "4\n";
    file << "0 10 15 20\n";
    file << "10 0 35 25\n";
    file << "15 35 0 30\n";
    file << "20 25 30 0\n";
    file.close();
  }

  void createInvalidGraphForTSP() {
    std::ofstream file("invalid_tsp_graph.txt");
    file << "4\n";
    file << "0 10 0 20\n";
    file << "10 0 35 0\n";
    file << "0 35 0 30\n";
    file << "20 0 30 0\n";
    file.close();
  }

  double calculatePathLength(const std::vector<int> &path, Graph &graph) {
    double length = 0.0;
    for (size_t i = 0; i < path.size() - 1; ++i) {
      length += graph.GetWeight(path[i], path[i + 1]);
    }
    return length;
  }
};

TEST_F(GraphAlgorithmsTest, DepthFirstSearch_ValidStart) {
  Graph graph;
  graph.LoadGraphFromFile("simple_graph.txt");
  GraphAlgorithms algorithms;

  std::vector<int> result = algorithms.DepthFirstSearch(graph, 1);

  EXPECT_EQ(result.size(), 4);
  EXPECT_EQ(result[0], 1);

  std::vector<int> expected = {1, 2, 3, 4};
  bool valid =
      std::is_permutation(result.begin(), result.end(), expected.begin());
  EXPECT_TRUE(valid);
}

TEST_F(GraphAlgorithmsTest, DepthFirstSearch_InvalidStart) {
  Graph graph;
  graph.LoadGraphFromFile("simple_graph.txt");
  GraphAlgorithms algorithms;

  EXPECT_THROW(algorithms.DepthFirstSearch(graph, 0), std::out_of_range);
  EXPECT_THROW(algorithms.DepthFirstSearch(graph, 5), std::out_of_range);
}

TEST_F(GraphAlgorithmsTest, DepthFirstSearch_EmptyGraph) {
  Graph graph;
  GraphAlgorithms algorithms;

  EXPECT_THROW(algorithms.DepthFirstSearch(graph, 1), std::out_of_range);
}

TEST_F(GraphAlgorithmsTest, BreadthFirstSearch_ValidStart) {
  Graph graph;
  graph.LoadGraphFromFile("simple_graph.txt");
  GraphAlgorithms algorithms;

  std::vector<int> result = algorithms.BreadthFirstSearch(graph, 1);

  EXPECT_EQ(result.size(), 4);
  EXPECT_EQ(result[0], 1);

  std::vector<int> expected = {1, 2, 3, 4};
  bool valid =
      std::is_permutation(result.begin(), result.end(), expected.begin());
  EXPECT_TRUE(valid);
}

TEST_F(GraphAlgorithmsTest, BreadthFirstSearch_InvalidStart) {
  Graph graph;
  graph.LoadGraphFromFile("simple_graph.txt");
  GraphAlgorithms algorithms;

  EXPECT_THROW(algorithms.BreadthFirstSearch(graph, 0), std::out_of_range);
  EXPECT_THROW(algorithms.BreadthFirstSearch(graph, 5), std::out_of_range);
}

TEST_F(GraphAlgorithmsTest, BreadthFirstSearch_DisconnectedGraph) {
  Graph graph;
  graph.LoadGraphFromFile("disconnected_graph.txt");
  GraphAlgorithms algorithms;

  std::vector<int> result = algorithms.BreadthFirstSearch(graph, 1);
  EXPECT_EQ(result.size(), 2);
  EXPECT_EQ(result[0], 1);
  EXPECT_EQ(result[1], 2);

  result = algorithms.BreadthFirstSearch(graph, 3);
  EXPECT_EQ(result.size(), 3);
  EXPECT_EQ(result[0], 3);
  EXPECT_TRUE((result[1] == 4 && result[2] == 5) ||
              (result[1] == 5 && result[2] == 4));
}

TEST_F(GraphAlgorithmsTest, GetShortestPathBetweenVertices_ValidPath) {
  Graph graph;
  graph.LoadGraphFromFile("weighted_graph.txt");
  GraphAlgorithms algorithms;

  int distance = algorithms.GetShortestPathBetweenVertices(graph, 1, 3);
  EXPECT_EQ(distance, 8);

  distance = algorithms.GetShortestPathBetweenVertices(graph, 1, 4);
  EXPECT_EQ(distance, 2);
}

TEST_F(GraphAlgorithmsTest, GetShortestPathBetweenVertices_InvalidVertices) {
  Graph graph;
  graph.LoadGraphFromFile("weighted_graph.txt");
  GraphAlgorithms algorithms;

  EXPECT_THROW(algorithms.GetShortestPathBetweenVertices(graph, 0, 1),
               std::out_of_range);
  EXPECT_THROW(algorithms.GetShortestPathBetweenVertices(graph, 1, 5),
               std::out_of_range);
  EXPECT_THROW(algorithms.GetShortestPathBetweenVertices(graph, 0, 0),
               std::out_of_range);
}

TEST_F(GraphAlgorithmsTest, GetShortestPathBetweenVertices_SameVertex) {
  Graph graph;
  graph.LoadGraphFromFile("weighted_graph.txt");
  GraphAlgorithms algorithms;

  int distance = algorithms.GetShortestPathBetweenVertices(graph, 2, 2);
  EXPECT_EQ(distance, 0);
}

TEST_F(GraphAlgorithmsTest, GetShortestPathBetweenVertices_NoPath) {
  Graph graph;
  graph.LoadGraphFromFile("disconnected_graph.txt");
  GraphAlgorithms algorithms;

  int distance = algorithms.GetShortestPathBetweenVertices(graph, 1, 3);
  EXPECT_EQ(distance, 0);
}

TEST_F(GraphAlgorithmsTest, GetShortestPathsBetweenAllVertices_Complete) {
  Graph graph;
  graph.LoadGraphFromFile("complete_4.txt");
  GraphAlgorithms algorithms;

  auto matrix = algorithms.GetShortestPathsBetweenAllVertices(graph);

  EXPECT_EQ(matrix.size(), 4);
  EXPECT_EQ(matrix[0].size(), 4);

  EXPECT_EQ(matrix[0][0], 0);
  EXPECT_EQ(matrix[0][1], 10);
  EXPECT_EQ(matrix[0][2], 15);
  EXPECT_EQ(matrix[0][3], 20);

  EXPECT_EQ(matrix[1][0], 10);
  EXPECT_EQ(matrix[2][0], 15);
  EXPECT_EQ(matrix[3][0], 20);
}

TEST_F(GraphAlgorithmsTest, GetShortestPathsBetweenAllVertices_Weighted) {
  Graph graph;
  graph.LoadGraphFromFile("weighted_graph.txt");
  GraphAlgorithms algorithms;

  auto matrix = algorithms.GetShortestPathsBetweenAllVertices(graph);

  EXPECT_EQ(matrix.size(), 4);

  EXPECT_EQ(matrix[0][2], 8);
  EXPECT_EQ(matrix[2][0], 8);

  EXPECT_EQ(matrix[1][3], 7);
  EXPECT_EQ(matrix[3][1], 7);
}

TEST_F(GraphAlgorithmsTest, GetShortestPathsBetweenAllVertices_Disconnected) {
  Graph graph;
  graph.LoadGraphFromFile("disconnected_graph.txt");
  GraphAlgorithms algorithms;

  auto matrix = algorithms.GetShortestPathsBetweenAllVertices(graph);

  EXPECT_EQ(matrix[0][2], 0);
  EXPECT_EQ(matrix[2][0], 0);
}

TEST_F(GraphAlgorithmsTest, GetLeastSpanningTree_Valid) {
  Graph graph;
  graph.LoadGraphFromFile("mst_graph.txt");
  GraphAlgorithms algorithms;

  auto mst = algorithms.GetLeastSpanningTree(graph);

  EXPECT_EQ(mst.size(), 5);

  int edge_count = 0;
  for (int i = 0; i < 5; ++i) {
    for (int j = i + 1; j < 5; ++j) {
      if (mst[i][j] > 0) {
        edge_count++;
        EXPECT_TRUE(mst[i][j] == mst[j][i]);
      }
    }
  }
  EXPECT_EQ(edge_count, 4);

  EXPECT_TRUE(mst[0][1] == 2 || mst[0][1] == 0);
  EXPECT_TRUE(mst[1][2] == 3 || mst[1][2] == 0);
}

TEST_F(GraphAlgorithmsTest, GetLeastSpanningTree_EmptyGraph) {
  Graph graph;
  GraphAlgorithms algorithms;

  auto mst = algorithms.GetLeastSpanningTree(graph);
  EXPECT_TRUE(mst.empty());
}

TEST_F(GraphAlgorithmsTest, GetLeastSpanningTree_SingleVertex) {
  Graph graph(1);
  GraphAlgorithms algorithms;

  auto mst = algorithms.GetLeastSpanningTree(graph);
  EXPECT_EQ(mst.size(), 1);
  EXPECT_EQ(mst[0][0], 0);
}

TEST_F(GraphAlgorithmsTest, SolveTravelingSalesmanProblem_Valid) {
  Graph graph;
  graph.LoadGraphFromFile("tsp_graph.txt");
  GraphAlgorithms algorithms;

  TsmResult result = algorithms.SolveTravelingSalesmanProblem(graph);

  EXPECT_FALSE(result.vertices.empty());
  EXPECT_EQ(result.vertices.size(), 5);

  EXPECT_TRUE(algorithms.IsHamiltonianCycle(result.vertices, 4));
  EXPECT_GT(result.distance, 0);

  double calculated_length = calculatePathLength(result.vertices, graph);
  EXPECT_DOUBLE_EQ(result.distance, calculated_length);
}

TEST_F(GraphAlgorithmsTest, SolveTravelingSalesmanProblem_Complete5) {
  Graph graph;
  graph.LoadGraphFromFile("complete_5.txt");
  GraphAlgorithms algorithms;

  TsmResult result = algorithms.SolveTravelingSalesmanProblem(graph);

  EXPECT_FALSE(result.vertices.empty());
  EXPECT_EQ(result.vertices.size(), 6);
  EXPECT_TRUE(algorithms.IsHamiltonianCycle(result.vertices, 5));
  EXPECT_GT(result.distance, 0);

  double calculated_length = calculatePathLength(result.vertices, graph);
  EXPECT_DOUBLE_EQ(result.distance, calculated_length);
}

TEST_F(GraphAlgorithmsTest, SolveTravelingSalesmanProblem_InvalidGraph) {
  Graph graph;
  graph.LoadGraphFromFile("invalid_tsp_graph.txt");
  GraphAlgorithms algorithms;

  EXPECT_THROW(algorithms.SolveTravelingSalesmanProblem(graph),
               std::runtime_error);
}

TEST_F(GraphAlgorithmsTest, SolveTravelingSalesmanProblem_TooSmallGraph) {
  Graph graph(1);
  GraphAlgorithms algorithms;

  EXPECT_THROW(algorithms.SolveTravelingSalesmanProblem(graph),
               std::runtime_error);
}

TEST_F(GraphAlgorithmsTest, SolveTravelingSalesmanProblem_DisconnectedGraph) {
  Graph graph;
  graph.LoadGraphFromFile("disconnected_graph.txt");
  GraphAlgorithms algorithms;

  EXPECT_THROW(algorithms.SolveTravelingSalesmanProblem(graph),
               std::runtime_error);
}

TEST_F(GraphAlgorithmsTest, MultipleAlgorithms_Consistency) {
  Graph graph;
  graph.LoadGraphFromFile("complete_4.txt");
  GraphAlgorithms algorithms;

  auto bfs_1 = algorithms.BreadthFirstSearch(graph, 1);
  auto bfs_2 = algorithms.BreadthFirstSearch(graph, 1);
  EXPECT_EQ(bfs_1, bfs_2);

  auto dfs_1 = algorithms.DepthFirstSearch(graph, 1);
  auto dfs_2 = algorithms.DepthFirstSearch(graph, 1);
  EXPECT_EQ(dfs_1, dfs_2);

  int path_1 = algorithms.GetShortestPathBetweenVertices(graph, 1, 3);
  int path_2 = algorithms.GetShortestPathBetweenVertices(graph, 1, 3);
  EXPECT_EQ(path_1, path_2);

  auto tsp_1 = algorithms.SolveTravelingSalesmanProblem(graph);
  auto tsp_2 = algorithms.SolveTravelingSalesmanProblem(graph);
  EXPECT_EQ(tsp_1.vertices.size(), tsp_2.vertices.size());
  EXPECT_DOUBLE_EQ(tsp_1.distance, tsp_2.distance);
}

TEST_F(GraphAlgorithmsTest, EdgeCases_ZeroWeightEdges) {
  std::ofstream file("zero_weight_graph.txt");
  file << "3\n";
  file << "0 0 5\n";
  file << "0 0 0\n";
  file << "5 0 0\n";
  file.close();

  Graph graph;
  graph.LoadGraphFromFile("zero_weight_graph.txt");
  GraphAlgorithms algorithms;

  auto dfs = algorithms.DepthFirstSearch(graph, 1);
  EXPECT_EQ(dfs.size(), 2);

  int dist = algorithms.GetShortestPathBetweenVertices(graph, 1, 2);
  EXPECT_EQ(dist, 0);

  remove("zero_weight_graph.txt");
}

TEST_F(GraphAlgorithmsTest, LargeGraph_Performance) {
  std::ofstream file("large_graph.txt");
  int n = 50;
  file << n << "\n";
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      if (i == j)
        file << "0 ";
      else
        file << (rand() % 10 + 1) << " ";
    }
    file << "\n";
  }
  file.close();

  Graph graph;
  graph.LoadGraphFromFile("large_graph.txt");
  GraphAlgorithms algorithms;

  auto start = std::chrono::high_resolution_clock::now();
  auto dfs = algorithms.DepthFirstSearch(graph, 1);
  auto end = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

  EXPECT_EQ(dfs.size(), n);
  EXPECT_LT(duration.count(), 1000);

  start = std::chrono::high_resolution_clock::now();
  auto bfs = algorithms.BreadthFirstSearch(graph, 1);
  end = std::chrono::high_resolution_clock::now();
  duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

  EXPECT_EQ(bfs.size(), n);
  EXPECT_LT(duration.count(), 1000);

  start = std::chrono::high_resolution_clock::now();
  auto paths = algorithms.GetShortestPathsBetweenAllVertices(graph);
  end = std::chrono::high_resolution_clock::now();
  duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

  EXPECT_EQ(paths.size(), n);
  EXPECT_LT(duration.count(), 5000);

  remove("large_graph.txt");
}

}  // namespace s21
