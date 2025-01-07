# Compiler and Flags
CXX = g++
CXXFLAGS = -Wall -std=c++17

# Directories
OUTPUT_DIR = output
DAY_DIR_PREFIX = day

# File names and targets
SRC_FILE = $(DAY_DIR_PREFIX)$(day)/$(DAY_DIR_PREFIX)$(day)$(part).cpp
OUT_FILE = $(OUTPUT_DIR)/day$(day)_part$(part)

# Default target
all: $(OUT_FILE)

# Build target (compiles the source file into the executable)
$(OUT_FILE): $(SRC_FILE)
	@mkdir -p $(OUTPUT_DIR)
	$(CXX) $(CXXFLAGS) $< -o $@

# Clean up generated files
clean:
	@rm -rf $(OUTPUT_DIR)
	@echo "Cleaned up output directory."

# Help message to show usage
help:
	@echo "Usage: make day=XX part=a|b"
	@echo "Example: make day=01 part=a"
	@echo "Description:"
	@echo "  Compiles dayXX/XXa.cpp or dayXX/XXb.cpp and places the executable in the 'output' directory."

.PHONY: all clean help
