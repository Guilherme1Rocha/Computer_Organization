# Multilevel Cache Hierarchy  

## Introduction  
Multilevel cache hierarchies are a fundamental component of modern computer systems, with the first levels of cache integrated into the processor chip. The objective of this project is to develop a memory hierarchy consisting of up to two levels of cache.  

## Development Environment  
The simulator must be developed in the C programming language and should target an x86-64 Linux environment. The only allowed dependencies are those provided by the GNU C Library (glibc), such as `stdio.h` and `stdlib.h`.  

The project must be compiled using `make` or `make all`, and no warnings should be present during the compilation process. Additionally, the simulator must be capable of running on laboratory computers using a Linux operating system for testing purposes.  

Students are permitted to modify the `Makefile` and introduce additional build targets to facilitate development. However, the compiler flags (CFLAGS) must remain unchanged.  

## Tasks  

All implementations must be configurable via the `Cache.h` header file. Students are not allowed to remove any predefined constants from this file. Each task must be delivered with its corresponding header (`.h`) and source code (`.c`) files in a separate directory, as specified below.  

### 1. Direct-Mapped L1 Cache  
- Develop a memory hierarchy with a direct-mapped L1 cache consisting of multiple lines.  
- Use the parameters provided in the `Cache.h` constants file.  
- Copy `SimpleCache.c` and `SimpleCache.h` to `L1Cache.c` and `L1Cache.h`, respectively, and modify them to implement the L1 cache.  

### 2. Direct-Mapped L2 Cache  
- Implement a direct-mapped L2 cache with parameters specified in the `Cache.h` constants file.  
- Integrate the Direct-Mapped L1 Cache developed in the previous task to form a two-level cache hierarchy.  

### 3. 2-Way Set Associative L2 Cache  
- Modify the L2 cache developed in the previous task to implement a two-way set-associative L2 cache.  
- Ensure that all other parameters, including `L2Size`, remain unchanged.  
- The Direct-Mapped L1 Cache from the first task must still be used in the two-level cache hierarchy.  
