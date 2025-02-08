# Instruction Level Parallelism

## Overview

This project aims to provide a hands-on experience in analyzing and optimizing the operation of a pipelined computer architecture using the **WinMIPS64** simulator (version 1.57).

## Environment

**WinMIPS64** is a simulator and visual debugger for a subset of the **MIPS64 Instruction Set Architecture (ISA)**. It allows users to observe instruction execution through the pipeline stages, identify stalls, and inspect registers and memory. The simulator is configured to analyze pipeline behavior under different conditions, including the presence and absence of data forwarding.

### Key Features of WinMIPS64:
- **Graphical visualization** of the pipeline stages.
- **Step-by-step execution** for detailed analysis.
- **Hazard detection and handling** for RAW, WAW, and WAR dependencies.
- **Configurable settings**, including forwarding and branch delay slots.

## Application Program

The provided program performs a mathematical operation using three arrays: `A`, `B`, and `C`. The computation follows this formula: C[i] = A[i] + B[i] * A[i];


The corresponding **MIPS64 assembly code** initializes the data and iterates over the arrays to perform the required computation. The program executes using a loop, and its efficiency is evaluated by analyzing pipeline behavior.

## Procedure

### 1. Execution Without Data Forwarding
- Load and execute the program in **WinMIPS64** without enabling data forwarding.
- Observe pipeline stalls and record instruction execution stages.
- Compute **Cycles Per Instruction (CPI)** and analyze branch prediction policy.

### 2. Enabling Data Forwarding
- Re-run the program with **data forwarding enabled**.
- Record execution stages and identify remaining stalls.
- Compute performance improvement compared to the base case.

### 3. Source Code Optimization: Reordering Instructions
- Apply **instruction reordering** to minimize data and structural hazards.
- Re-evaluate the program execution and calculate speedup.

### 4. Loop Unrolling Optimization
- Unroll the loop to reduce control hazards and improve execution speed.
- Compare execution profiles and measure performance gains.

### 5. Using Branch Delay Slot Optimization
- Modify the code to **utilize the branch delay slot**.
- Enable delay slots in **WinMIPS64 settings** and analyze performance.
- Measure speedup against the original unoptimized program.

## Performance Evaluation

Throughout the assignment, various optimization techniques will be applied to the program. The performance improvements will be quantified by:
- **Instruction execution timelines** (pipeline stages: F, D, Xn, M, W).
- **Number of stalls introduced**.
- **Cycles Per Instruction (CPI) comparison**.
- **Speedup calculations** relative to the unoptimized version.

## Requirements

- **WinMIPS64 version 1.57**
- **Assembly source code (`prog.s`)**
- **Basic understanding of MIPS64 ISA and pipeline concepts**

## Check answers in Lab_3.pdf
