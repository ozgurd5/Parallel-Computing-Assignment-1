# Parallel Computing — Sorting Algorithms

This repository contains C/MPI applications developed for the "Parallel Computing" course, exploring the performance characteristics of five classical sorting algorithms in both serial and distributed-memory parallel settings.

The project is primarily built upon serial implementations of five sorting algorithms, an MPI-based parallel framework that uses a tree-based merge strategy to combine locally sorted chunks, and Python-based performance visualization.

## 📂 Table of Contents and File Descriptions

The project consists of 3 main stages and files:

### 1. Serial Sorting Implementations (common.c / serial.c)
Classical implementations of five sorting algorithms in C, executed on a single core for baseline timing.
* **Algorithms:**
  * `Quick Sort` — average O(n log n), Lomuto partitioning scheme.
  * `Merge Sort` — O(n log n), divide-and-conquer with auxiliary buffers.
  * `Bitonic Sort` — O(n log² n), sorting-network based; pads the input with `INT_MAX` up to the next power of 2 when needed.
  * `Selection Sort` — O(n²), input-independent comparison count.
  * `Bubble Sort` — O(n²), includes an early-exit optimization when no swaps occur in a pass.
* **Features:**
  * Microsecond-precision timing via `gettimeofday()` wrapped in `get_time()`.
  * Each algorithm operates on a copy of the input so subsequent algorithms get the same data (`copy_array`).
  * Every result is verified with `is_sorted()` after execution.

### 2. MPI Parallel Sorting with Tree-Based Merge (parallel.c)
MPI-based distributed parallel framework that executes all five algorithms in parallel by combining local sorts with a tree-based merge across ranks.
* **Workflow:**
  * **Distribution:** Rank 0 splits the N-element array with `MPI_Scatter`; each of the `p` ranks receives an `n/p`-element chunk.
  * **Local Sort:** Each rank sorts its own chunk independently using the chosen serial algorithm. No inter-rank communication occurs during this step.
  * **Tree Merge:** In `log₂(p)` stages, ranks pair up as sender/receiver. The sender forwards its sorted chunk and its length; the receiver merges the two sorted pieces into one twice-as-large sorted piece. After `log₂(p)` stages rank 0 holds the fully sorted array.
  * **Verification:** The final array is validated on rank 0 with `is_sorted()` before the measurement is reported.
* **Timing:** Uses `MPI_Wtime()` with `MPI_Barrier` synchronization before each measurement; the maximum elapsed time across ranks is reported via `MPI_Reduce(MPI_MAX)` so reported times reflect true wall-clock duration.
* **Constraint:** Requires that `N % p == 0`; otherwise the program reports an error and exits.

### 3. Performance Analysis and Visualization (plot.py / rapor.md)
Python script and detailed Turkish report for performance comparison and speedup analysis.
* **Outputs (plot.py):**
  * `images/seri_sureleri.png` — serial running times versus N on a log-log scale.
  * `images/seri_vs_paralel_100k.png` and `images/seri_vs_paralel_1000k.png` — grouped bar charts comparing serial against parallel runs (p = 2, 4, 8) at N = 100.000 and N = 1.000.000.
  * `images/speedup_100k.png` and `images/speedup_1000k.png` — speedup curves (S = T_serial / T_parallel) for each algorithm against the ideal S = p line.
* **Report (rapor.md):** Full theoretical discussion, complexity analysis against measured results, speedup and efficiency tables for p = 2, 4, 8, super-linear speedup explanation for O(n²) algorithms, and a bottleneck analysis covering cache sharing, memory bandwidth, and communication overhead.

## 🚀 Installation and Execution

To build and run the project you need a C compiler, CMake, and a Microsoft MPI installation. The plotting step additionally requires Python 3 with `matplotlib` and `numpy`.

**Dependencies:**
* C compiler (tested with MinGW GCC)
* CMake 3.20 or newer
* Microsoft MPI SDK and runtime (https://www.microsoft.com/en-us/download/details.aspx?id=105289)
* Python 3 with `matplotlib` and `numpy` (only required for the plots)

After cloning the repository, build and run via the terminal:

```bash
# Build the executable
cmake -B build
cmake --build build

# Run with p processors (p must divide N defined in common.h)
mpiexec -n 8 build/Parallel_Computing_Assignment.exe

# Generate the performance graphs
pip install matplotlib numpy
python plot.py
```

The array size is controlled by `#define N` in `common.h`. By default `main.c` runs the serial benchmark on rank 0 and the parallel benchmark on all ranks during a single invocation; comment out either call to run only one.
