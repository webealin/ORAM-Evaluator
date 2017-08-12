//
// Created by weber on 09.07.2017.
//

#ifndef ORAMEVALUATOR_EVALUATOR_H
#define ORAMEVALUATOR_EVALUATOR_H

#include "../oram/tree_factory.h"
#include "time.h"
#include "../trivial_linear_scan.h"

class Evaluator {
public:
    struct btSettings {
        uint16_t B;
        uint16_t c;
        uint16_t count;
        outType* out;
        inline ~btSettings() {
            delete out;
        }
    };

    struct pathSettings {
        uint16_t stash;
        btSettings* bt;
        inline ~pathSettings() {
            delete bt;
        }
    };

    struct evalParam {
        uint16_t min;
        uint16_t max;
        uint16_t step_m;
        uint16_t step_p;
    };

private:
    outType& find_LinearScan(uint32_t noRead, uint32_t noWrite, uint64_t m, uint64_t b);
    outType& find_LinearScanORAM(uint32_t noAcc, uint64_t m, uint64_t b);

    Evaluator::btSettings* find_best_BT(uint32_t noAcc, uint64_t m, uint64_t b, std::string type, evalParam bParam,
            outType& (*acc)(uint64_t newM, uint64_t newB, uint16_t B, uint16_t c, uint16_t count));

    Evaluator::pathSettings find_best_Path(uint32_t noAcc, uint64_t m, uint64_t b, std::string type, evalParam bParam, evalParam sParam,
                                   outType& (*acc)(uint64_t newM, uint64_t newB, uint16_t B, uint16_t c, uint16_t stash, uint16_t count));
public:
    void evaluate_exact(uint32_t noRead, uint32_t noWrite, uint64_t m, uint64_t b);
    void evaluate_exact_fast(uint32_t noRead, uint32_t noWrite, uint64_t m, uint64_t b);
};

#endif //ORAMEVALUATOR_EVALUATOR_H
