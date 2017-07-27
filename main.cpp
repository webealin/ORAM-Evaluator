#include <iostream>
#include "fast_calculator.h"
#include "oram/tree_factory.h"
#include "linear_scan.h"
#include "time.h"
#include "evaluator.h"
Settings::ToCalc Settings::toCalc = Settings::GATES;

void test_GKK() {
    clock_t start = clock();
    // GKK Benchmarks: use old formulas, no Bucketsize, TO_CALC 1, no counter and no dynamic blocksize
    BinaryTreeGKK* oram = TreeFactory::create_GKK_BT((uint64_t) pow(2, 20), 512, 16);
    std::cout << "GKK BT: d = 20: " << oram->c_acc(512) << std::endl;
    delete oram;

    oram = TreeFactory::create_GKK_BT((uint64_t) pow(2, 19), 512, 16);
    std::cout << "GKK BT: d = 19: " << oram->c_acc(512) << std::endl;
    delete oram;

    oram = TreeFactory::create_GKK_BT((uint64_t) pow(2, 18), 512, 16);
    std::cout << "GKK BT: d = 18: " << oram->c_acc(512) << std::endl;
    delete oram;

    oram = TreeFactory::create_GKK_BT((uint64_t) pow(2, 17), 512, 16);
    std::cout << "GKK BT: d = 17: " << oram->c_acc(512) << std::endl;
    delete oram;

    oram = TreeFactory::create_GKK_BT((uint64_t) pow(2, 16), 512, 16);
    std::cout << "GKK BT: d = 16: " << oram->c_acc(512) << std::endl;
    delete oram;

    float elapsed = (float)(clock() - start) / CLOCKS_PER_SEC;
    std::cout << "time needed: " << elapsed << "\n" << std::endl;
}

void test_BT() {
    clock_t start = clock();
    // Circuit ORAM Benchmarks: use new formulas, bucketsize 120, TO_CALC 1, counter(1) and dynamic blocksize,
    for(uint16_t c = 2; c <= 64; c *= 2) {
        BinaryTree* oram = TreeFactory::create_BT((uint64_t) pow(2, 30), 32, 120, c, 8, false);
        std::cout << "BT: c = " << c << ": " << oram->c_acc(32) << std::endl;
        delete oram;
    }
    float elapsed = (float)(clock() - start) / CLOCKS_PER_SEC;
    std::cout << "time needed: " << elapsed << "\n" << std::endl;
}

void test_path() {
    clock_t start = clock();
    // Circuit ORAM Benchmarks: use new formulas, stashsize 89, TO_CALC 1, counter and dynamic blocksize,
    for(uint16_t c = 2; c <= 64; c *= 2) {
        Path* oram = TreeFactory::create_Path("Path", (unsigned long) pow(2, 30), 32, 4, c, 89, 8);
        std::cout << "Path: c = " << c << ": " << oram->c_acc(32) << std::endl;
        delete oram;
    }
    float elapsed = (float)(clock() - start) / CLOCKS_PER_SEC;
    std::cout << "time needed: " << elapsed << "\n" << std::endl;
}

void test_pathSC() {
    clock_t start = clock();
    // Circuit ORAM Benchmarks: use new formulas, stashsize 89, TO_CALC 1, counter and dynamic blocksize,
    for(uint16_t c = 2; c <= 64; c *= 2) {
        PathSC* oram = (PathSC*) TreeFactory::create_Path("PathSC", (unsigned long) pow(2, 30), 32, 4, c, 89, 8);
        std::cout << "PathSC: c = " << c << ": " << oram->c_acc(32) << std::endl;
        delete oram;
    }
    float elapsed = (float)(clock() - start) / CLOCKS_PER_SEC;
    std::cout << "time needed: " << elapsed << "\n" << std::endl;
}

void test_SCORAM() {
    clock_t start = clock();
    // Circuit ORAM Benchmarks: use new formulas, stashsize 141, TO_CALC 1, counter and dynamic blocksize,
    for(uint16_t c = 2; c <= 64; c *= 2) {
        Path* oram = TreeFactory::create_Path("Scoram", (unsigned long) pow(2, 30), 32, 6, c, 141, 8);
        std::cout << "SCORAM: c = " << c << ": " << oram->c_acc(32) << std::endl;
        delete oram;
    }
    float elapsed = (float)(clock() - start) / CLOCKS_PER_SEC;
    std::cout << "time needed: " << elapsed << "\n" << std::endl;
}

void test_CORAM() {
    clock_t start = clock();
    // Circuit ORAM Benchmarks: use new formulas, stashsize 141, TO_CALC 1, counter and dynamic blocksize
    for(uint16_t c = 2; c <= 64; c *= 2) {
        Path* oram = TreeFactory::create_Path("CORAM", (unsigned long) pow(2, 30), 32, 4, c, 141, 8);
        std::cout << "CORAM: c = " << c << ": " << oram->c_acc(32) << std::endl;
        delete oram;
    }
    float elapsed = (float)(clock() - start) / CLOCKS_PER_SEC;
    std::cout << "time needed: " << elapsed << "\n" << std::endl;
}

void test_paper_benchmarks() {
    clock_t start = clock();
    std::cout << "Benchmarks: " << std::endl;
    test_GKK();
    test_BT();
    test_BT_formulas();
    test_path();
    test_pathSC();
    test_SCORAM();
    test_CORAM();
    float elapsed = (float)(clock() - start) / CLOCKS_PER_SEC;
    std::cout << "time needed: " << elapsed << "\n" << std::endl;
}

void evaluate() {
    Evaluator* eval = new Evaluator();
    eval->evaluate_exact(1, 0, (uint64_t) pow(2, 30), 32);
    eval->evaluate_exact_fast(1, 0, (uint64_t) pow(2, 30), 32);
    //eval->evaluate_exact(10, 10, (uint64_t) pow(2, 30), 32);
    //eval->evaluate_exact(10, 10, (uint64_t) pow(2, 30), 512);
    //eval->evaluate_exact(10, 10, (uint64_t) pow(2, 60), 512);
    //eval->evaluate_exact_fast(10, 10, (uint64_t) pow(2, 60), 512);
    delete eval;
}

int main() {
    //test_paper_benchmarks();
    evaluate();
    return 0;
}