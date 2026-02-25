CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -Werror -I.
COVERAGE_FLAGS = -fprofile-arcs -ftest-coverage
TARGET = graph_app
TEST_TARGET = run_tests
LIB_GRAPH = s21_graph.a
LIB_ALGORITHMS = s21_graph_algorithms.a
LIB_CONTAINERS = s21_containers.a
LIB_TEST =  -lgtest -lgtest_main -lpthread -lgcov

BUILD_DIR = build

GRAPH_DIR = s21_graph/
ALGORITHMS_DIR = s21_graph_algorithms/
CONTAINERS_DIR = s21_containers/
TESTS_DIR = s21_tests/
REPORT_DIR = report/

GRAPH_SRCS = $(GRAPH_DIR)/graph.cpp
ALGORITHMS_SRCS = $(ALGORITHMS_DIR)/graph_algorithms.cpp
CONTAINERS_SRCS = $(CONTAINERS_DIR)/stack.cpp $(CONTAINERS_DIR)/queue.cpp
TESTS_SRCS = $(wildcard $(TESTS_DIR)/*.cpp)

MAIN_SRC = main.cpp

GRAPH_OBJS = $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(GRAPH_SRCS))
ALGORITHMS_OBJS = $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(ALGORITHMS_SRCS))
CONTAINERS_OBJS = $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(CONTAINERS_SRCS))
TEST_OBJS = $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(TESTS_SRCS))
MAIN_OBJ = $(BUILD_DIR)/main.o

HEADERS = $(GRAPH_DIR)/graph.h $(ALGORITHMS_DIR)/graph_algorithms.h \
          $(CONTAINERS_DIR)/stack.h $(CONTAINERS_DIR)/queue.h

$(shell mkdir -p $(BUILD_DIR) $(BUILD_DIR)/$(GRAPH_DIR) $(BUILD_DIR)/$(ALGORITHMS_DIR) $(BUILD_DIR)/$(CONTAINERS_DIR) $(BUILD_DIR)/$(TESTS_DIR))

all: $(TARGET)

clean: 
	rm -rf $(BUILD_DIR) $(TARGET) *.a *.dot *.png $(TEST_TARGET) *.gcda *.gcno *.info $(COVERAGE_DIR)

test: $(TEST_TARGET)
	./$(TEST_TARGET)

gcov_report: test
	@mkdir -p $(REPORT_DIR)
	./$(TEST_TARGET)
	@gcovr --filter="$(GRAPH_DIR)" \
		--filter="$(ALGORITHMS_DIR)" \
		--exclude="$(TESTS_DIR)" \
		--exclude="$(CONTAINERS_DIR)" \
		--exclude-throw-branches \
		--html-details -o $(REPORT_DIR)/coverage_report.html \
		--html-medium-threshold-line 80 \
		--html-high-threshold-line 90 \
		--html-medium-threshold-branch 50 \
		--html-high-threshold-branch 70 \
		--print-summary | tee $(REPORT_DIR)/coverage_summary.txt
	@coverage=$$(gcovr --filter="$(GRAPH_DIR)" \
		--filter="$(ALGORITHMS_DIR)" \
		--exclude="$(TESTS_DIR)" \
		--exclude="$(CONTAINERS_DIR)" \
		--print-summary | grep -oP 'lines: \K\d+\.\d+%' | head -1 | sed 's/%//'); \
	if [ -z "$$coverage" ]; then \
		echo "Не удалось определить покрытие"; \
	elif [ $$(echo "$$coverage < 80" | bc -l) -eq 1 ]; then \
		echo "❌ Покрытие строк меньше 80%: $$coverage%"; \
		exit 1; \
	else \
		echo "✅ Покрытие строк: $$coverage%"; \
	fi

s21_graph: $(LIB_GRAPH)

s21_graph_algorithms: $(LIB_ALGORITHMS)

.PHONY: all clean test run s21_graph s21_graph_algorithms

$(TARGET): $(MAIN_OBJ) $(LIB_GRAPH) $(LIB_ALGORITHMS) $(LIB_CONTAINERS)
	$(CXX) $(CXXFLAGS) $^ -o $@ 

$(TEST_TARGET): CXXFLAGS += $(COVERAGE_FLAGS)
$(TEST_TARGET): $(TEST_OBJS) $(GRAPH_OBJS) $(ALGORITHMS_OBJS) $(CONTAINERS_OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LIB_TEST)

$(LIB_GRAPH): $(GRAPH_OBJS)
	ar rcs $@ $^

$(LIB_ALGORITHMS): $(ALGORITHMS_OBJS)
	ar rcs $@ $^

$(LIB_CONTAINERS): $(CONTAINERS_OBJS)
	ar rcs $@ $^

$(BUILD_DIR)/%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

s21_containers: $(LIB_CONTAINERS)

format:
	cp ../materials/linters/.clang-format ./
	find ./ $(GRAPH_DIR) $(GRAPH_DIR) $(ALGORITHMS_DIR) $(ALGORITHMS_DIR) $(CONTAINERS_DIR)  $(CONTAINERS_DIR) $(TEST_DIR) -name '*.h' -o -name '*.cpp' | xargs clang-format -i
	rm -rf .clang-format

style:
	cp ../materials/linters/.clang-format ./
	find ./ $(GRAPH_DIR) $(GRAPH_DIR) $(ALGORITHMS_DIR) $(ALGORITHMS_DIR) $(CONTAINERS_DIR)  $(CONTAINERS_DIR) $(TEST_DIR) -name '*.h' -o -name '*.cpp' | xargs clang-format -n --Werror
	rm -rf .clang-format