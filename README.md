# Advent of Code 2024
This repository contains my solutions to the Advent of Code 2024 problems, implemented in C++.

## Repository Structure

- `input/`: Contains input files for each day's challenge (e.g., `day01.txt`, `day02.txt`).
- `dayXX/`: Each day's solution is placed in its respective directory (e.g., `day01/day01a.cpp` and `day01/day01b.cpp` for parts 1 and 2 of Day 1).
- `output/`: Compiled executables will be placed here after building the respective solutions.
- `Makefile`: A Makefile to easily compile the solution for a specific day and part.

## How to Compile and Run

### Compile a Solution
To compile and run a specific solution for a day, use the following command:
```console
make day=XX part=Y
```
- `XX`: The day number (e.g., `01` for Day 1).
- `Y`: The part of the challenge (`a` or `b`).

**Example:**
```console
make day=17 part=b
```
This will compile the solution for Day 17, Part 2, and place the executable in the `output/` directory.

### Run the Solution
Once compiled, you can run the executable and pass the corresponding input file as a parameter:
```console
./output/dayXX_partY input/dayXX.txt
```

**Example:**
```console
./output/day17_partb input/day17.txt
```
This will execute the compiled program for Day 17, Part 2, using the input data stored in `input/day17.txt`.

### Clean Build Files
To clean up all generated output files, use:
```console
make clean
```

## Notes 
- Solutions are implemented in C++17 and require a compatible compiler, such as `g++`.
