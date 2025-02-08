# Cache Analysis and Optimization

## Introduction

This project aims to achieve two objectives:  
1. **Characterizing Computer Caches** – Determining the specifications of a computer’s L1 and L2 caches through performance analysis.  
2. **Optimizing Program Performance** – Utilizing cache knowledge to enhance the efficiency of a given matrix multiplication program.  

The assignment leverages the PAPI (Performance Application Programming Interface) library to access hardware performance counters available on modern microprocessors.

### Setup Instructions  

1. **Build the PAPI Library**  
   - Navigate to the source directory:  
     ```sh
     cd papi-X.X.X/src
     ```
   - Compile the library:  
     ```sh
     ./configure
     make
     ```
   - This process generates the `papilib.a` library and utility tools under `src/utils/`.  

2. **Check Available PAPI Events**  
   - Run the following command to verify available events on the target machine:  
     ```sh
     ./src/utils/papi_avail
     ```

## Exercises

### **1. Cache Size Estimation**  
- Use the provided `spark.c` program to estimate the L1 and L2 cache sizes.  
- Execute the program multiple times to ensure statistical significance.  
- Record and analyze the cache capacity results.  

### **2. Cache Characterization**  
Modify the provided C code to measure cache parameters by analyzing different data access patterns.  

- **Parameters to determine:**  
  - Cache capacity  
  - Cache block size  
  - L1 cache miss penalty  

- **Execution Steps:**  
  - Run the program with varying array sizes and strides.  
  - Analyze cache miss behavior.  
  - Generate performance charts.  

### **3. Cache Modeling**  

#### **3.1 L1 Cache Analysis**  
- Run the `cm1.c` program and measure L1 cache misses.  
- Determine:  
  - L1 cache size  
  - Block size  
  - Associativity  

#### **3.2 L2 Cache Analysis**  
- Modify `cm1.c` to track L2 cache misses (use `PAPI_L2_DCM` event).  
- Measure:  
  - L2 cache size  
  - Block size  
  - Associativity  

### **4. Matrix Multiplication Optimization**  

#### **4.1 Baseline Implementation**  
- Analyze the performance of the `mm1.c` program.  
- Evaluate L1 cache hit-rate.  

#### **4.2 Optimization 1: Matrix Transposition**  
- Modify the program to transpose `mul2` before multiplication (`mm2.c`).  
- Measure performance improvements.  
- Compare cache hit-rates with the baseline implementation.  

#### **4.3 Optimization 2: Blocked Matrix Multiplication**  
- Implement a **tiled** version (`mm3.c`) to improve cache locality.  
- Tune the block size to match cache characteristics.  
- Compare results with previous implementations.  

### **5. Performance Evaluation**  
- Analyze speedups and hit-rate improvements.  
- Measure L1 and L2 cache behavior using PAPI events (`PAPI_L2_DCH`, `PAPI_L2_DCM`).  
- Compare optimizations and justify performance improvements. 

## Check answers in `Lab_2.pdf`
