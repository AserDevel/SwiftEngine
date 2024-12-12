# Compiler and flags
CXX = g++
CXXFLAGS = -Iinclude -std=c++17

# Libraries
LIB = -lSDL2 -lSDL2_image -lGL -ldl

# Output binary
BIN = swift

# Directories
SRC_DIR = src
BUILD_DIR = build
GLAD_DIR = $(SRC_DIR)/glad
GRAPHICS_DIR = $(SRC_DIR)/graphics
MANAGERS_DIR = $(SRC_DIR)/managers
SYSTEMS_DIR = $(SRC_DIR)/systems
TEST_DIR = tests

# Source files
LINALG_SRC = $(wildcard $(LINALG_DIR)/*.cpp)
GLAD_SRC = $(wildcard $(GLAD_DIR)/*.c)
GRAPHICS_SRC = $(wildcard $(GRAPHICS_DIR)/*.cpp)
MANAGERS_SRC = $(wildcard $(MANAGERS_DIR)/*.cpp)
SYSTEMS_SRC = $(wildcard $(SYSTEMS_DIR)/*.cpp)
SRC = $(GLAD_SRC) $(GRAPHICS_SRC) $(MANAGERS_SRC) $(SYSTEMS_SRC)

# Object files
OBJ = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRC)))

# Test files
TEST_SRC = $(wildcard $(TEST_DIR)/*.cpp)
TEST_OBJ = $(patsubst $(TEST_DIR)/%.cpp, $(BUILD_DIR)/tests/%.o, $(TEST_SRC))
TEST_BIN = $(BUILD_DIR)/test_runner

# Default target
all: $(BIN)

# Linking step
$(BIN): main.cpp $(OBJ)
	$(CXX) -o $@ $^ $(LIB) $(CXXFLAGS)

# Compile source files to object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile test files
$(BUILD_DIR)/tests/%.o: $(TEST_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Build test binary
$(TEST_BIN): $(TEST_OBJ) $(OBJ)
	$(CXX) -o $@ $^ $(LIB) $(CXXFLAGS)

# Run the program
run: $(BIN)
	./$(BIN)

# Run tests
test: $(TEST_BIN)
	./$(TEST_BIN)

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR) $(BIN)