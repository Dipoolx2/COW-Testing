# Copy-on-write page fault testing
Simple script that tests the number of page faults caused by initial memory mapping to COW zero buffers. Script works on macOS and Linux, not on Windows.

Tests available both through explicit memory mapping and malloc.
Usage: ```./cow_test <malloc|mmap>```.

Tested with c++17 on macOS Sequoia 15.5.

Compiled with: ```gcc -O2 -std=c++17 main.cpp -lstdc++ -o cow_test```

### Results yielded with mode ```mmap```:
- Page size: 16384
- Expected change in page faults: 1024
- Minor page faults before writes: 307
- Major page faults before writes: 1
- Minor page faults after writes: 1332
- Major page faults after writes: 1
- Minor page faults during writes: 1025
- Major page faults during writes: 0

### Results yielded with mode ```malloc```:
- Page size: 16384
- Expected change in page faults: 1024
- Minor page faults before writes: 305
- Major page faults before writes: 1
- Minor page faults after writes: 1330
- Major page faults after writes: 1
- Minor page faults during writes: 1025
- Major page faults during writes: 0
