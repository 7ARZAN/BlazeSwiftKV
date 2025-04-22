BlazeSwiftKV: High-Performance Key-Value Store
Project Name
BlazeSwiftKV
The name "BlazeSwiftKV" combines the essence of speed ("Blaze" and "Swift") with the core functionality of a key-value store ("KV"). It’s professional, concise, and appealing to developers on GitHub, conveying high performance and efficiency while remaining memorable and relevant to the project’s focus on rapid key-value operations.
Overview
BlazeSwiftKV is a high-performance, in-memory key-value store implemented in C, designed for rapid insertion and lookup of key-value pairs. It uses a hash table with open addressing and quadratic probing for collision resolution, optimized for speed with arena-based memory allocation and minimal memory overhead. The program reads input from stdin, processes key-value pairs, and handles queries efficiently, making it ideal for competitive programming or performance-critical applications.
Features

Efficient Hash Table: Utilizes a hash table with a fixed size (TABLE_SIZE = 1,048,576) and buckets of size 8, using the DJB2 hashing algorithm and a secondary hash (h2) for probing.
Arena-Based Memory Allocation: Employs a custom arena allocator to minimize memory fragmentation and improve allocation speed for strings.
Dynamic Resizing: Automatically resizes the hash table when the load factor exceeds 0.75, doubling the capacity to maintain performance.
Buffered I/O: Reads input in chunks (BUFFER_SIZE = 65,536) for efficient processing of large datasets.
Query Processing: Supports fast lookups with a simple query interface, printing the associated value or "Not found" if the key is absent.
Input Generator: Includes a Python script (input_creator.py) to generate test input files with up to 1,000,000 key-value pairs and queries.

How It Works
BlazeSwiftKV operates in two phases:

Insertion Phase:

Reads key-value pairs from stdin in the format:
key
value


Keys and values are stored in the hash table using arena-based allocation.

If the load factor exceeds 0.75, the table is resized to maintain performance.

Input terminates with an empty line.



Query Phase:

Reads query keys from stdin.
For each key, outputs the associated value if found, or key: Not found. if not.
Continues until EOF or an error occurs.



File Structure

Core Implementation:

hotrace.c: Contains the main hash table insertion logic with quadratic probing.
hash_methods.c: Implements the DJB2 hashing algorithm and secondary hash function.
hr_search.c: Handles key lookups with recursive probing.
hr_resize.c: Manages hash table resizing and rehashing.
init.c: Initializes the hash table and allocates memory.
hr_tools.c: Provides arena-based string duplication and buffered line reading.
hr_utils.c: Utility functions for string operations (ft_strlen, ft_memcpy, ft_strcmp).
main.c: Orchestrates input processing and query handling.


Header:

hotrace.h: Defines constants, structures, and function prototypes.


Build and Testing:

Makefile: Compiles the program with optimizations (-O3, -Ofast, etc.) and generates test input.
input_creator.py: Generates a test file (hotrace.txt) with random key-value pairs and queries.



Compilation and Usage
Prerequisites

C compiler (e.g., gcc or cc)
Python 3 (for generating test input)
POSIX-compliant system (for read and write syscalls)

Build
make

This compiles the program, creates the BlazeSwiftKV executable, and generates hotrace.txt.
Run
./BlazeSwiftKV < hotrace.txt

Or provide custom input via stdin.
Clean Up
make fclean

Removes the executable, object files, and test input.
Optimizations

Compiler Flags: Uses -O3, -Ofast, -march=native, and -funroll-loops for maximum performance.
Arena Allocation: Reduces memory fragmentation and allocation overhead.
Quadratic Probing: Minimizes clustering in the hash table.
Buffered Reading: Processes input in large chunks to reduce system calls.

Example Input/Output
Input (hotrace.txt)
animal0
sound0
animal1
sound1
...

animal0
unknown0
animal1

Output
sound0
unknown0: Not found.
sound1

Potential Use Cases

Competitive programming challenges requiring fast key-value lookups.
Lightweight in-memory caching systems.
Educational tool for learning hash table implementations and memory management.

Contributing
Contributions are welcome! Feel free to submit pull requests or open issues on GitHub for bug fixes, performance improvements, or new features.
License
This project is licensed under the MIT License.
