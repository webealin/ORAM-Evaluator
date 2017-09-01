//
// Created by weber on 09.07.2017.
//

#ifndef ORAMEVALUATOR_EVALUATOR_H
#define ORAMEVALUATOR_EVALUATOR_H

#include "../oram/oram_factory.h"
#include <ctime>
#include "../fast_calculator.h"
#include "../oram/optimized_square_root.h"
#include "../types/outType.h"
#include "../types/evalParam.h"
#include "../settings.h"
#include "access_functions.h"

class Evaluator {
public:
    struct btSettings {
        uint16_t B;
        uint16_t c;
        uint16_t count;
        outType* out;
        btSettings() : B(UINT16_MAX), c(UINT16_MAX), count(UINT16_MAX), out(new outType) { }
        btSettings(uint16_t B, uint16_t c, uint16_t count, outType* out) : B(B), c(c), count(count), out(out) { }
    };

    struct pathSettings {
        uint16_t stash;
        btSettings* bt;
    };

    struct sqrSettings {
        uint16_t c;
        uint16_t count;
        outType* out;
    };

    typedef outType& (*btFunc)(uint32_t noAcc, bool values, uint64_t m, uint64_t b, uint16_t B, uint16_t c, uint16_t count);
    typedef outType& (*pathFunc)(uint32_t noAcc, bool values, uint64_t m, uint64_t b, uint16_t B, uint16_t c, uint16_t stash, uint16_t count);
    typedef outType& (*sqrFunc)(uint32_t noAcc, bool values, uint64_t m, uint64_t b, uint16_t c, uint16_t count);

    struct funcTypes {
        btFunc btF;
        pathFunc pathF;
        pathFunc pathSCF;
        pathFunc SCORAMF;
        pathFunc CORAMF;
        sqrFunc OSQRF;
    };

    funcTypes acc_slow_func {acc_BT_slow, acc_Path_slow, acc_PathSC_slow, acc_SCORAM_slow, acc_CORAM_slow, acc_OSQR_slow};
    funcTypes slow_func {BT_slow, Path_slow, PathSC_slow, SCORAM_slow, CORAM_slow, OSQR_slow};
    funcTypes acc_fast_func {acc_BT_fast, acc_Path_fast, acc_PathSC_fast, acc_SCORAM_fast, acc_CORAM_fast, acc_OSQR_fast};
    funcTypes fast_func {BT_fast, Path_fast, PathSC_fast, SCORAM_fast, CORAM_fast, OSQR_fast};

    outType find_LinearScan(uint32_t noRead, uint32_t noWrite, uint64_t m, uint64_t b);

    btSettings find_best_BT(uint32_t noAcc, bool values, uint64_t m, uint64_t b, evalParam bParam, btFunc acc);
    void find_best_BT(uint32_t noAcc, bool values, uint64_t m, uint64_t b, uint16_t B, btSettings &minSettings, btFunc acc);
    void find_best_BT(uint32_t noAcc, bool values, uint64_t m, uint64_t b, uint16_t B, uint16_t c, btSettings &minSettings, btFunc acc);

    pathSettings find_best_Path(uint32_t noAcc, bool values, uint64_t m, uint64_t b, evalParam bParam, evalParam sParam, pathFunc acc);
    pathSettings find_best_Path(uint32_t noAcc, bool values, uint64_t m, uint64_t b, uint16_t B, uint16_t stash, pathSettings& minSettings, pathFunc acc);

    sqrSettings find_best_OSQR(uint32_t noAcc, bool values, uint64_t m, uint64_t b, sqrFunc acc);

    void evaluate_acc_exact(uint32_t noRead, uint32_t noWrite, uint64_t m, uint64_t b);
    void evaluate_acc_exact_fast(uint32_t noRead, uint32_t noWrite, uint64_t m, uint64_t b);
    void evaluate_exact(uint32_t noRead, uint32_t noWrite, bool values, uint64_t m, uint64_t b);
    void evaluate_exact_fast(uint32_t noRead, uint32_t noWrite, bool values, uint64_t m, uint64_t b);

private:
    void print_LinearScan(uint32_t noRead, uint32_t noWrite, uint64_t m, uint64_t b);
    void print_best_BT(uint32_t noAcc, bool values, uint64_t m, uint64_t b, evalParam bParam, btFunc acc);
    void print_best_Path(uint32_t noAcc, bool values, uint64_t m, uint64_t b, std::string type, evalParam bParam, evalParam sParam, pathFunc acc);
    void print_best_OSQR(uint32_t noAcc, bool values, uint64_t m, uint64_t b, sqrFunc acc);

    void evaluate(uint32_t noRead, uint32_t noWrite, bool values, uint64_t m, uint64_t b, uint16_t d, funcTypes func);
};

#endif //ORAMEVALUATOR_EVALUATOR_H
