//
// Created by weber on 09.07.2017.
//

#ifndef ORAMEVALUATOR_EVALUATOR_H
#define ORAMEVALUATOR_EVALUATOR_H

#include "../oram/tree_factory.h"
#include <ctime>
#include "../trivial_linear_scan.h"
#include "../fast_calculator.h"

class Evaluator {
public:
    struct btSettings {
        uint16_t B;
        uint16_t c;
        uint16_t count;
        outType* out;
    };

    struct pathSettings {
        uint16_t stash;
        btSettings* bt;
    };

    struct evalParam {
        uint16_t min;
        uint16_t max;
        uint16_t step_m;
        uint16_t step_p;
    };

protected:
    outType find_LinearScan(uint32_t noRead, uint32_t noWrite, uint64_t m, uint64_t b);
    void print_LinearScan(uint32_t noRead, uint32_t noWrite, uint64_t m, uint64_t b);

    outType find_LinearScanORAM(uint32_t noAcc, uint64_t m, uint64_t b);
    void print_LinearScanORAM(uint32_t noAcc, uint64_t m, uint64_t b);

    Evaluator::btSettings find_best_BT(uint32_t noAcc, uint64_t m, uint64_t b, evalParam bParam,
            outType& (*acc)(uint64_t newM, uint64_t newB, uint16_t B, uint16_t c, uint16_t count));
    void print_best_BT_fast(uint32_t noAcc, uint64_t m, uint64_t b, evalParam bParam);
    void print_best_BT_slow(uint32_t noAcc, uint64_t m, uint64_t b, evalParam bParam);

    Evaluator::pathSettings find_best_Path(uint32_t noAcc, uint64_t m, uint64_t b, evalParam bParam, evalParam sParam,
                                   outType& (*acc)(uint64_t newM, uint64_t newB, uint16_t B, uint16_t c, uint16_t stash, uint16_t count));
    void print_best_Path(uint32_t noAcc, uint64_t m, uint64_t b, std::string type, evalParam bParam, evalParam sParam,
                              outType& (*acc)(uint64_t newM, uint64_t newB, uint16_t B, uint16_t c, uint16_t stash, uint16_t count));
public:
    void evaluate_exact(uint32_t noRead, uint32_t noWrite, uint64_t m, uint64_t b);
    void evaluate_exact_fast(uint32_t noRead, uint32_t noWrite, uint64_t m, uint64_t b);
};

inline outType& acc_BT_slow(uint64_t m, uint64_t b, uint16_t B, uint16_t c, uint16_t count) {
    BinaryTree* oram = TreeFactory::create_BT(m, b, B, c, count);
    outType& out = oram->c_acc(b);
    delete oram;
    return out;
}

inline outType& acc_BT_fast(uint64_t m, uint64_t b, uint16_t B, uint16_t c, uint16_t count) {
    return c_acc_BT(m, b, B, count, c);
}

inline outType& acc_Path_slow(uint64_t m, uint64_t b, uint16_t B, uint16_t c, uint16_t stash, uint16_t count) {
    Path* path = TreeFactory::create_Path("Path", m, b, B, c, stash, count);
    outType& out = path->c_acc(b);
    delete path;
    return out;
}

inline outType& acc_Path_fast(uint64_t m, uint64_t b, uint16_t B, uint16_t c, uint16_t stash, uint16_t count) {
    return c_acc_Path(m, b, B, count, stash, c);
}

inline outType& acc_PathSC_slow(uint64_t m, uint64_t b, uint16_t B, uint16_t c, uint16_t stash, uint16_t count) {
    PathSC* path = (PathSC*) TreeFactory::create_Path("PathSC", m, b, B, c, stash, count);
    outType& out = path->c_acc(b);
    delete path;
    return out;
}

inline outType& acc_PathSC_fast(uint64_t m, uint64_t b, uint16_t B, uint16_t c, uint16_t stash, uint16_t count) {
    return c_acc_PSC(m, b, B, count, stash, c);
}

inline outType& acc_SCORAM_slow(uint64_t m, uint64_t b, uint16_t B, uint16_t c, uint16_t stash, uint16_t count) {
    Scoram* path = (Scoram*) TreeFactory::create_Path("Scoram", m, b, B, c, stash, count);
    outType& out = path->c_acc(b);
    delete path;
    return out;
}

inline outType& acc_SCORAM_fast(uint64_t m, uint64_t b, uint16_t B, uint16_t c, uint16_t stash, uint16_t count) {
    return c_acc_SCORAM(m, b, B, count, stash, c);
}

inline outType& acc_CORAM_slow(uint64_t m, uint64_t b, uint16_t B, uint16_t c, uint16_t stash, uint16_t count) {
    Coram* path = (Coram*) TreeFactory::create_Path("CORAM", m, b, B, c, stash, count);
    outType& out = path->c_acc(b);
    delete path;
    return out;
}

inline outType& acc_CORAM_fast(uint64_t m, uint64_t b, uint16_t B, uint16_t c, uint16_t stash, uint16_t count) {
    return c_acc_CORAM(m, b, B, count, stash, c);
}

#endif //ORAMEVALUATOR_EVALUATOR_H
