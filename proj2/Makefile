# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude
LDFLAGS = -lgtest_main -lgtest -pthread -lexpat

# Directories
SRC_DIR = src
TESTSRC_DIR = testsrc
INCLUDE_DIR = include
OBJ_DIR = obj
BIN_DIR = bin

# Source files for each executable

## teststrutils
STRUTILS_SRC = $(SRC_DIR)/StringUtils.cpp
STRUTILS_TEST_SRC = $(TESTSRC_DIR)/StringUtilsTest.cpp

## teststrdatasource
STRDATASRC_SRC = $(SRC_DIR)/StringDataSource.cpp
STRDATASRC_TEST_SRC = $(TESTSRC_DIR)/StringDataSourceTest.cpp

## teststrdatasink
STRDATASINK_SRC = $(SRC_DIR)/StringDataSink.cpp
STRDATASINK_TEST_SRC = $(TESTSRC_DIR)/StringDataSinkTest.cpp

## testdsv
DSV_SRC1 = $(SRC_DIR)/DSVReader.cpp
DSV_SRC2 = $(SRC_DIR)/DSVWriter.cpp
DSV_TEST_SRC = $(TESTSRC_DIR)/DSVTest.cpp

## testxml
XML_SRC1 = $(SRC_DIR)/XMLReader.cpp
XML_SRC2 = $(SRC_DIR)/XMLWriter.cpp
XML_TEST_SRC = $(TESTSRC_DIR)/XMLTest.cpp

# Object files
STRUTILS_OBJS = $(OBJ_DIR)/StringUtils.o $(OBJ_DIR)/StringUtilsTest.o
STRDATASRC_OBJS = $(OBJ_DIR)/StringDataSource.o $(OBJ_DIR)/StringDataSourceTest.o
STRDATASINK_OBJS = $(OBJ_DIR)/StringDataSink.o $(OBJ_DIR)/StringDataSinkTest.o
DSV_OBJS = $(OBJ_DIR)/DSVReader.o $(OBJ_DIR)/DSVWriter.o $(OBJ_DIR)/DSVTest.o
XML_OBJS = $(OBJ_DIR)/XMLReader.o $(OBJ_DIR)/XMLWriter.o $(OBJ_DIR)/XMLTest.o

# Executable targets
TEST_STRUTILS = $(BIN_DIR)/teststrutils
TEST_STRDATASRC = $(BIN_DIR)/teststrdatasource
TEST_STRDATASINK = $(BIN_DIR)/teststrdatasink
TEST_DSV = $(BIN_DIR)/testdsv
TEST_XML = $(BIN_DIR)/testxml

# Default target: build all executables
all: $(TEST_STRUTILS) $(TEST_STRDATASRC) $(TEST_STRDATASINK) $(TEST_DSV) $(TEST_XML)

# Create obj and bin
$(OBJ_DIR) $(BIN_DIR):
	mkdir -p $@

# Compile source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile test files
$(OBJ_DIR)/%Test.o: $(TESTSRC_DIR)/%Test.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Link obj and test files
# teststrutils: StringUtils and its tests
$(TEST_STRUTILS): $(STRUTILS_OBJS) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# teststrdatasource: StringDataSource and its tests
$(TEST_STRDATASRC): $(STRDATASRC_OBJS) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# teststrdatasink: StringDataSink and its tests
$(TEST_STRDATASINK): $(STRDATASINK_OBJS) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# testdsv: DSV reader/writer and its tests
$(TEST_DSV): $(DSV_OBJS) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# testxml: XML reader/writer and its tests
$(TEST_XML): $(XML_OBJS) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Target to run all tests
tests: all
	./$(TEST_STRUTILS)
	./$(TEST_STRDATASRC)
	./$(TEST_STRDATASINK)
	./$(TEST_DSV)
	./$(TEST_XML)

# Clean: remove the obj and bin directories
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

.PHONY: all tests clean