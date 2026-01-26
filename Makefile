CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -Werror -I.
TARGET = graph_app
LIB_GRAPH = s21_graph.a
LIB_ALGORITHMS = s21_graph_algorithms.a
LIB_CONTAINERS = s21_containers.a

BUILD_DIR = build

GRAPH_DIR = s21_graph/
ALGORITHMS_DIR = s21_graph_algorithms/
CONTAINERS_DIR = s21_containers/

GRAPH_SRCS = $(GRAPH_DIR)/graph.cpp
ALGORITHMS_SRCS = $(ALGORITHMS_DIR)/graph_algorithms.cpp
CONTAINERS_SRCS = $(CONTAINERS_DIR)/stack.cpp $(CONTAINERS_DIR)/queue.cpp

MAIN_SRC = main.cpp

GRAPH_OBJS = $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(GRAPH_SRCS))
ALGORITHMS_OBJS = $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(ALGORITHMS_SRCS))
CONTAINERS_OBJS = $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(CONTAINERS_SRCS))
MAIN_OBJ = $(BUILD_DIR)/main.o

HEADERS = $(GRAPH_DIR)/graph.h $(ALGORITHMS_DIR)/graph_algorithms.h \
          $(CONTAINERS_DIR)/stack.h $(CONTAINERS_DIR)/queue.h

$(shell mkdir -p $(BUILD_DIR)/$(GRAPH_DIR) $(BUILD_DIR)/$(ALGORITHMS_DIR) $(BUILD_DIR)/$(CONTAINERS_DIR))

all: $(TARGET)

clean: 
	rm -rf $(BUILD_DIR) $(TARGET) *.a *.dot *.png

test:

s21_graph: $(LIB_GRAPH)

s21_graph_algorithms: $(LIB_ALGORITHMS)

.PHONY: all clean test run s21_graph s21_graph_algorithms

$(TARGET): $(MAIN_OBJ) $(LIB_GRAPH) $(LIB_ALGORITHMS) $(LIB_CONTAINERS)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(LIB_GRAPH): $(GRAPH_OBJS)
	ar rcs $@ $^

$(LIB_ALGORITHMS): $(ALGORITHMS_OBJS)
	ar rcs $@ $^

$(LIB_CONTAINERS): $(CONTAINERS_OBJS)
	ar rcs $@ $^

$(BUILD_DIR)/%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

s21_containers: $(LIB_CONTAINERS)
