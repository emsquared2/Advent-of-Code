# =========================================================
#  Makefile for Advent of Code multi-year C++ solutions
#
#  Usage:
#     make year=YYYY day=XX part=a|b
#
#  Example:
#     make year=2024 day=01 part=a
#
#  This compiles:
#     2024/day01/day01a.cpp
#
#  And outputs:
#     2024/output/day01_parta
# =========================================================

# Compiler settings
CXX = g++
CXXFLAGS = -Wall -std=c++17

# Parameters expected from command line
year ?=
day  ?=
part ?=

# Skip argument checks when running help or clean
ifeq ($(filter help clean,$(MAKECMDGOALS)),)
  # Required argument checks
  ifeq ($(year),)
    $(error Missing argument: year=YYYY)
  endif
  ifeq ($(day),)
    $(error Missing argument: day=XX)
  endif
  ifeq ($(part),)
    $(error Missing argument: part=a|b)
  endif
endif

# Construct paths based on inputs
SRC_FILE = $(year)/day$(day)/day$(day)$(part).cpp
OUT_DIR  = $(year)/output
OUT_FILE = $(OUT_DIR)/day$(day)_part$(part)

# Default target: compile the selected file
all: $(OUT_FILE)

# Build rule: compile to output directory
$(OUT_FILE): $(SRC_FILE)
	@mkdir -p $(OUT_DIR)
	$(CXX) $(CXXFLAGS) $< -o $@

# Remove output directory for a specific year
clean:
ifeq ($(year),)
	$(error Provide a year: make clean year=YYYY)
endif
	@rm -rf $(OUT_DIR)
	@echo "Cleaned $(OUT_DIR)"

# Display help
help:
	@echo "Advent of Code Makefile"
	@echo ""
	@echo "Usage:"
	@echo "  make year=YYYY day=XX part=a|b"
	@echo ""
	@echo "Examples:"
	@echo "  make year=2024 day=01 part=a"
	@echo "  make year=2023 day=15 part=b"
	@echo ""
	@echo "Source location:"
	@echo "  YEAR/dayXX/dayXX[a|b].cpp"
	@echo ""
	@echo "Output:"
	@echo "  YEAR/output/dayXX_part[a|b]"
	@echo ""

.PHONY: all clean help
