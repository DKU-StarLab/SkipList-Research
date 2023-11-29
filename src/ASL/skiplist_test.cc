#include <iostream>
#include <chrono>
#include "skiplist.h"
#include <set>
#include <random>
#include <assert.h>

#include <fstream>
#include <string>
#include <vector>

#include "zipf.h"
#include "latest-generator.h"

typedef std::chrono::high_resolution_clock Clock;

void fb(SkipList<Key> &sl) {
    std::string file_path = "../dataset/keys/fb.txt";
    std::ifstream file(file_path);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << file_path << std::endl;
    }
    std::vector<uint64_t> numbers;
    std::string line;

    while (std::getline(file, line)) {
        uint64_t number = std::stoull(line);
        numbers.push_back(number);
    }
    file.close();

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distr(0, numbers.size());

    auto w_start = Clock::now();
    for (std::size_t i = 0; i < numbers.size(); i++) {
        Key key = numbers[distr(gen)];
        printf("Insert - %lu\n", key);
        sl.Insert_Test2(key);
    }
    auto w_end = Clock::now();
    printf("After Insert\n");
    auto r_start = Clock::now();
    for (std::size_t i = 0; i < numbers.size(); i++) {
        Key key = numbers[distr(gen)];
        sl.Contains(key);
    }
    auto r_end = Clock::now();
    
    float r_time, w_time;
    r_time = std::chrono::duration_cast<std::chrono::nanoseconds>(r_end - r_start).count() * 0.001;
    w_time = std::chrono::duration_cast<std::chrono::nanoseconds>(w_end - w_start).count() * 0.001;
    printf("\n[Real-fb] W = %.lf, R = %.lf\n", w_time, r_time);
}

void books(SkipList<Key> &sl) {
    std::string file_path = "../dataset/keys/books.txt";
    std::ifstream file(file_path);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << file_path << std::endl;
    }
    std::vector<uint64_t> numbers;
    std::string line;

    while (std::getline(file, line)) {
        uint64_t number = std::stoull(line);
        numbers.push_back(number);
    }
    file.close();
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distr(0, numbers.size());

    auto w_start = Clock::now();
    for (std::size_t i = 0; i < numbers.size(); i++) {
        Key key = numbers[distr(gen)];
        printf("Insert - %lu\n", key);
        sl.Insert_Test2(key);
    }
    auto w_end = Clock::now();
    printf("After Insert\n");
    auto r_start = Clock::now();
    for (std::size_t i = 0; i < numbers.size(); i++) {
        Key key = numbers[distr(gen)];
        sl.Contains(key);
    }
    auto r_end = Clock::now();
    
    float r_time, w_time;
    r_time = std::chrono::duration_cast<std::chrono::nanoseconds>(r_end - r_start).count() * 0.001;
    w_time = std::chrono::duration_cast<std::chrono::nanoseconds>(w_end - w_start).count() * 0.001;
    printf("\n[Real-books] W = %.lf, R = %.lf\n", w_time, r_time);
}

void wiki(SkipList<Key> &sl) {
    std::string file_path = "../dataset/keys/wiki.txt";
    std::ifstream file(file_path);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << file_path << std::endl;
    }
    std::vector<uint64_t> numbers;
    std::string line;
    
    while (std::getline(file, line)) {
        uint64_t number = std::stoull(line);
        numbers.push_back(number);
    }
    file.close();

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distr(0, numbers.size());

    auto w_start = Clock::now();
    for (std::size_t i = 0; i < numbers.size(); i++) {
        Key key = numbers[distr(gen)];
        sl.Insert_Test2(key);
    }
    auto w_end = Clock::now();
    printf("After Insert\n");
    auto r_start = Clock::now();
    for (std::size_t i = 0; i < numbers.size(); i++) {
        Key key = numbers[distr(gen)];
        sl.Contains(key);
    }
    auto r_end = Clock::now();
    
    float r_time, w_time;
    r_time = std::chrono::duration_cast<std::chrono::nanoseconds>(r_end - r_start).count() * 0.001;
    w_time = std::chrono::duration_cast<std::chrono::nanoseconds>(w_end - w_start).count() * 0.001;
    printf("\n[Real-wiki] W = %.lf, R = %.lf\n", w_time, r_time);
}

void osm(SkipList<Key> &sl) {
    std::string file_path = "../dataset/keys/osm.txt";
    std::ifstream file(file_path);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << file_path << std::endl;
    }
    std::vector<uint64_t> numbers;
    std::string line;

    while (std::getline(file, line)) {
        uint64_t number = std::stoull(line);
        numbers.push_back(number);
    }
    file.close();

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distr(0, numbers.size());

    auto w_start = Clock::now();
    for (std::size_t i = 0; i < numbers.size(); i++) {
        Key key = numbers[distr(gen)];
        sl.Insert_Test2(key);
    }
    auto w_end = Clock::now();
    printf("After Insert\n");
    auto r_start = Clock::now();
    for (std::size_t i = 0; i < numbers.size(); i++) {
        Key key = numbers[distr(gen)];
        sl.Contains(key);
    }
    auto r_end = Clock::now();
    
    float r_time, w_time;
    r_time = std::chrono::duration_cast<std::chrono::nanoseconds>(r_end - r_start).count() * 0.001;
    w_time = std::chrono::duration_cast<std::chrono::nanoseconds>(w_end - w_start).count() * 0.001;
    printf("\n[Real-osm] W = %.lf, R = %.lf\n", w_time, r_time);
}

void YCSBWorkloadA(const int write, const int run, SkipList<Key> &sl) {
    init_zipf_generator(0, write);

    float w_time, r_time;

    auto w_start = Clock::now();
    for (int i = 0; i < write; i++) {
        Key key = (nextValue() % (write)) + 1;
        sl.Insert_Test2(key);
    }
    auto w_end = Clock::now();
    w_time = std::chrono::duration_cast<std::chrono::nanoseconds>(w_end - w_start).count() * 0.001;
    printf("\n[YCSB-Load] Time = %.lf\n", w_time);

    auto r_start = Clock::now();
    for (int i = 0; i < run; i++) {
        Key Zkey = (nextValue() % (run)) + 1;

        int next_op = rand() % 100;

        if (next_op < 50) {
            sl.Contains(Zkey);
        } else {
            sl.Insert_Test2(Zkey);
        }
    }
    auto r_end = Clock::now();
    r_time = std::chrono::duration_cast<std::chrono::nanoseconds>(r_end - r_start).count() * 0.001;
    printf("\n[YCSB-A] Run Time = %.lf\n", r_time);
}

void YCSBWorkloadB(const int write, const int run, SkipList<Key> &sl) {
    init_zipf_generator(0, write);

    float w_time, r_time;

    auto w_start = Clock::now();
    for (int i = 0; i < write; i++) {
        Key key = (nextValue() % (write)) + 1;
        sl.Insert_Test2(key);
    }
    auto w_end = Clock::now();
    w_time = std::chrono::duration_cast<std::chrono::nanoseconds>(w_end - w_start).count() * 0.001;
    printf("\n[YCSB-Load] Time = %.lf\n", w_time);

    auto r_start = Clock::now();
    for (int i = 0; i < run; i++) {
        Key Zkey = (nextValue() % (run)) + 1;

        int next_op = rand() % 100;

        if (next_op < 95) {
            sl.Contains(Zkey);
        } else {
            sl.Insert_Test2(Zkey);
        }
    }
    auto r_end = Clock::now();
    r_time = std::chrono::duration_cast<std::chrono::nanoseconds>(r_end - r_start).count() * 0.001;
    printf("\n[YCSB-B] Run Time = %.lf\n", r_time);
}

void YCSBWorkloadC(const int write, const int run, SkipList<Key> &sl) {
    init_zipf_generator(0, write);

    float w_time, r_time;

    auto w_start = Clock::now();
    for (int i = 0; i < write; i++) {
        Key key = (nextValue() % (write)) + 1;
        sl.Insert_Test2(key);
    }
    auto w_end = Clock::now();
    w_time = std::chrono::duration_cast<std::chrono::nanoseconds>(w_end - w_start).count() * 0.001;
    printf("\n[YCSB-Load] Time = %.lf\n", w_time);

    auto r_start = Clock::now();
    for (int i = 0; i < run; i++) {
        Key Zkey = (nextValue() % (run)) + 1;
        sl.Contains(Zkey);
    }
    auto r_end = Clock::now();
    r_time = std::chrono::duration_cast<std::chrono::nanoseconds>(r_end - r_start).count() * 0.001;
    printf("\n[YCSB-C] Run Time = %.lf\n", r_time);
}

void YCSBWorkloadD(const int write, const int run, SkipList<Key> &sl) {
    init_zipf_generator(0, write);

    float w_time, r_time;

    auto w_start = Clock::now();
    for (int i = 0; i < write; i++) {
        Key key = (nextValue() % (write)) + 1;
        sl.Insert_Test2(key);
    }
    auto w_end = Clock::now();
    w_time = std::chrono::duration_cast<std::chrono::nanoseconds>(w_end - w_start).count() * 0.001;
    printf("\n[YCSB-Load] Time = %.lf\n", w_time);

    auto r_start = Clock::now();
    for (int i = 0; i < run; i++) {
        Key Zkey = (nextValue() % (run)) + 1;

        int next_op = rand() % 100;

        if (next_op < 95) {
            sl.Contains(Zkey);
        } else {
            sl.Insert_Test2(Zkey);
        }
    }
    auto r_end = Clock::now();
    r_time = std::chrono::duration_cast<std::chrono::nanoseconds>(r_end - r_start).count() * 0.001;
    printf("\n[YCSB-D] Run Time = %.lf\n", r_time);
}

void YCSBWorkloadE(const int write, const int run, SkipList<Key> &sl) {
    init_zipf_generator(0, write);

    float w_time, r_time;

    auto w_start = Clock::now();
    for (int i = 0; i < write; i++) {
        Key key = (nextValue() % (write)) + 1;
        sl.Insert_Test2(key);
    }
    auto w_end = Clock::now();
    w_time = std::chrono::duration_cast<std::chrono::nanoseconds>(w_end - w_start).count() * 0.001;
    printf("\n[YCSB-Load] Time = %.lf\n", w_time);

    auto r_start = Clock::now();
    for (int i = 0; i < run; i++) {
        Key Zkey = (nextValue() % (run)) + 1;

        int next_op = rand() % 100;

        if (next_op < 95) {
            sl.Scan(Zkey, 100);
        } else {
            sl.Insert_Test2(Zkey);
        }
    }
    auto r_end = Clock::now();
    r_time = std::chrono::duration_cast<std::chrono::nanoseconds>(r_end - r_start).count() * 0.001;
    printf("\n[YCSB-E] Run Time = %.lf\n", r_time);
}

void YCSBWorkloadF(const int write, const int run, SkipList<Key> &sl) {
    init_zipf_generator(0, write);

    float w_time, r_time;

    auto w_start = Clock::now();
    for (int i = 0; i < write; i++) {
        Key key = (nextValue() % (write)) + 1;
        sl.Insert_Test2(key);
    }
    auto w_end = Clock::now();
    w_time = std::chrono::duration_cast<std::chrono::nanoseconds>(w_end - w_start).count() * 0.001;
    printf("\n[YCSB-Load] Time = %.lf\n", w_time);

    auto r_start = Clock::now();
    for (int i = 0; i < run; i++) {
        Key Zkey = (nextValue() % (run)) + 1;

        int next_op = rand() % 100;

        if (next_op < 50) {
            sl.Contains(Zkey);
        } else {
            sl.Contains(Zkey);
            sl.Insert_Test2(Zkey);
        }
    }
    auto r_end = Clock::now();
    r_time = std::chrono::duration_cast<std::chrono::nanoseconds>(r_end - r_start).count() * 0.001;
    printf("\n[YCSB-F] Run Time = %.lf\n", r_time);
}

void Zipfian(const int write, const int read, SkipList<Key> &sl) {
    init_zipf_generator(0, write);

    auto w_start = Clock::now();
    for(int i = 1; i <= write; i++) {
        Key key = nextValue() % write;        
        sl.Insert_Test2(key);
    }
    auto w_end = Clock::now();
    printf("After Insert\n");
    auto r_start = Clock::now();
    for(int i = 1; i <= read; i++) {
        Key key = nextValue() % read;
        sl.Contains(key);
    }
    auto r_end = Clock::now();

    float r_time, w_time;
    r_time = std::chrono::duration_cast<std::chrono::nanoseconds>(r_end - r_start).count() * 0.001;
    w_time = std::chrono::duration_cast<std::chrono::nanoseconds>(w_end - w_start).count() * 0.001;
    printf("\n[Zipfian] W = %.lf, R = %.lf\n", w_time, r_time);
}

void Uniform(const int write, const int read, SkipList<Key> &sl) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distr(0, write);

    auto w_start = Clock::now();
    for(int i = 1; i <= write; i++) {
        Key key = distr(gen)+1;        
        sl.Insert_Test2(key);
    }
    auto w_end = Clock::now();
    printf("After Insert\n");
    auto r_start = Clock::now();
    for(int i = 1; i <= read; i++) {
        Key key = distr(gen)+1;
        sl.Contains(key);
    }
    auto r_end = Clock::now();

    float r_time, w_time;
    r_time = std::chrono::duration_cast<std::chrono::nanoseconds>(r_end - r_start).count() * 0.001;
    w_time = std::chrono::duration_cast<std::chrono::nanoseconds>(w_end - w_start).count() * 0.001;
    printf("\n[Uniform] W = %.lf, R = %.lf\n", w_time, r_time);
}

void Sequential(const int write, const int read, SkipList<Key> &sl) {

    auto w_start = Clock::now();
    for(int i = 1; i <= write; i++) {
        sl.Insert_Test2(i);
    }
    auto w_end = Clock::now();
    printf("After Insert\n");
    auto r_start = Clock::now();
    for(int i = 1; i <= read; i++) {
        sl.Contains(i);
    }
    auto r_end = Clock::now();

    float r_time, w_time;
    r_time = std::chrono::duration_cast<std::chrono::nanoseconds>(r_end - r_start).count() * 0.001;
    w_time = std::chrono::duration_cast<std::chrono::nanoseconds>(w_end - w_start).count() * 0.001;
    printf("\n[Sequential] W = %.lf, R = %.lf\n", w_time, r_time);
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << "[Write Count] [Read Count] [Benchmark]\n";
        std::cerr << "Synthetic Benchmarks: 0-Sequential 1-Uniform 2-Zipfian 3~8-YCSB(A~F) \n";
        std::cerr << "Real-World Benchmarks: 9-fb 10-books 11-wiki 12-osm \n";
        return 1;
    }
    const int W = std::atoi(argv[1]);
    const int R = std::atoi(argv[2]);
    const int B = std::atoi(argv[3]);

    SkipList<Key> sl;

    if (B == 0) { // Sequential benchmark
        printf("\n[Sequential Benchmark in progress ...ing]\n\n");
        Sequential(W, R, sl);
    } else if (B == 1) { // Uniform benchmark;
        printf("\n[Uniform Benchmark in progress ...ing]\n\n");
        Uniform(W, R, sl);
    } else if (B == 2) { // Zipfian benchmark
        printf("\n[Zipfian Benchmark in progress ...ing]\n\n");
        Zipfian(W, R, sl);
    } else if (B == 3) { // YCSB-A benchmark
        printf("\n[YCSB-A Benchmark in progress ...ing]\n\n");
        YCSBWorkloadA(W, R, sl);
    } else if (B == 4) { // YCSB-B benchmark
        printf("\n[YCSB-B Benchmark in progress ...ing]\n\n");
        YCSBWorkloadB(W, R, sl);
    } else if (B == 5) { // YCSB-C benchmark
        printf("\n[YCSB-C Benchmark in progress ...ing]\n\n");
        YCSBWorkloadC(W, R, sl);
    } else if (B == 6) { // YCSB-D benchmark
        printf("\n[YCSB-D Benchmark in progress ...ing]\n\n");
        YCSBWorkloadD(W, R, sl);
    } else if (B == 7) { // YCSB-E benchmark
        printf("\n[YCSB-E Benchmark in progress ...ing]\n\n");
        YCSBWorkloadE(W, R, sl);
    } else if (B == 8) { // YCSB-F benchmark
        printf("\n[YCSB-F Benchmark in progress ...ing]\n\n");
        YCSBWorkloadF(W, R, sl);
    } else if (B == 9) { // Real-world fb benchmark
        printf("\n[Real-World Dataset (fb) Benchmark in progress ...ing]\n\n");
        fb(sl);
    } else if (B == 10) { // Real-world books benchmark
        printf("\n[Real-World Dataset (books) Benchmark in progress ...ing]\n\n");
        books(sl);
    } else if (B == 11) { // Real-world wiki benchmark
        printf("\n[Real-World Dataset (wiki) Benchmark in progress ...ing]\n\n");
        wiki(sl);
    } else if (B == 12) { // Real-world osm benchmark
        printf("\n[Real-World Dataset (osm) Benchmark in progress ...ing]\n\n");
        osm(sl);
    }
    
    //sl.Print();

    return 0;
}
