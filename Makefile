CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -Werror -g
COVERAGE_FLAGS = --coverage

TARGET = taskforge

SRCS = $(wildcard *.cpp)

OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS) *.gcda *.gcno *.gcov coverage.html

coverage: clean
	mkdir -p coverage
	$(CXX) $(CXXFLAGS) $(COVERAGE_FLAGS) -o $(TARGET) $(SRCS)
	./$(TARGET)
	gcovr --filter '$(CURDIR)/.*\.cpp$$' --html-details -o coverage/coverage.html
	@echo "Coverage report: coverage/coverage.html"

cleandoc:
	rm -r /docs

valgrind:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(TARGET)

zip:
	zip -r taskforge.zip . -x "*.o" "*.gcda" "*.gcno" "*.gcov" "taskforge" "coverage.html"

.PHONY: all clean
