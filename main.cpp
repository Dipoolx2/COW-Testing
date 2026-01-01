#include <iostream>
#include <sys/resource.h>
#include <unistd.h>
#include <cstdlib>
#include <sys/mman.h>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <malloc|mmap>\n";
        return 1;
    }

    std::string method = argv[1];
    size_t page_size = sysconf(_SC_PAGESIZE);
    size_t total_size = 16 * 1024 * 1024; // 16 MB
    size_t num_pages = total_size / page_size;

    char* buffer = nullptr;

    if (method == "malloc") {
        buffer = (char*) malloc(total_size);
        if (!buffer) {
            perror("malloc");
            return 1;
        }
    } else if (method == "mmap") {
        buffer = (char*) mmap(nullptr, total_size, PROT_READ | PROT_WRITE,
                              MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        if (buffer == MAP_FAILED) {
            perror("mmap");
            return 1;
        }
    }

    struct rusage usage_before, usage_after;
    getrusage(RUSAGE_SELF, &usage_before);

    std::cout << "Page size: " << page_size << "\n";
    std::cout << "Expected change in page faults: " << num_pages << "\n";
    std::cout << "Minor page faults before writes: " << usage_before.ru_minflt << "\n";
    std::cout << "Major page faults before writes: " << usage_before.ru_majflt << "\n";

    // Write to each page
    for (size_t i = 0; i < num_pages; ++i) {
        buffer[i * page_size] = 42;
    }

    getrusage(RUSAGE_SELF, &usage_after);
    std::cout << "Minor page faults after writes: " << usage_after.ru_minflt << "\n";
    std::cout << "Major page faults after writes: " << usage_after.ru_majflt << "\n";
    std::cout << "Minor page faults during writes: " 
              << (usage_after.ru_minflt - usage_before.ru_minflt) << "\n";
    std::cout << "Major page faults during writes: " 
              << (usage_after.ru_majflt - usage_before.ru_majflt) << "\n";

    if (method == "malloc") {
        free(buffer);
    } else if (method == "mmap") {
        munmap(buffer, total_size);
    }

    return 0;
}

