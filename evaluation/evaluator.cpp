//
// Created by weber on 09.07.2017.
//

#include "evaluator.h"

outType Evaluator::find_LinearScan(uint32_t noRead, uint32_t noWrite, uint64_t m, uint64_t b) {
    outType& out = addWR(multiplyWR(noRead, TrivialLinearScan::c_read(m, b)),
                         multiplyWR(noWrite, TrivialLinearScan::c_write(m, b)));
    outType ret = {out.gates, out.traffic, out.rounds};
    delete &out;
    return ret;
}

void Evaluator::print_LinearScan(uint32_t noRead, uint32_t noWrite, uint64_t m, uint64_t b) {
    std::cout << "Linear Scan: " << find_LinearScan(noRead, noWrite, m, b) << std::endl;
}

Evaluator::btSettings Evaluator::find_best_BT(uint32_t noAcc, bool values, uint64_t m, uint64_t b, evalParam bParam,
                                               btFunc acc) {
    btSettings minSettings;

    for (uint16_t B = bParam.min; B <= bParam.max; B = B * bParam.step_m + bParam.step_p)
        find_best_BT(noAcc, values, m, b, B, minSettings, acc);

    return minSettings;
}

void Evaluator::find_best_BT(uint32_t noAcc, bool values, uint64_t m, uint64_t b, uint16_t B, btSettings& minSettings,
                                              btFunc acc) {
    for (uint16_t c = 2; c <= myLog2(m); c *= 2)
        find_best_BT(noAcc, values, m, b, B, c, minSettings, acc);
}

void Evaluator::find_best_BT(uint32_t noAcc, bool values, uint64_t m, uint64_t b, uint16_t B, uint16_t  c, btSettings& minSettings,
                             btFunc acc) {
    for (uint16_t count = 1; count <= floor(log(m) / log(c)); count++) {
        outType &out = multiplyWR(noAcc, acc(noAcc, values, m, b, B, c, count));
        if (out < *minSettings.out) {
            *minSettings.out = {out.gates, out.traffic, out.rounds};
            minSettings = {B, c, count, minSettings.out};
        }
        delete &out;
    }
}

void Evaluator::print_best_BT(uint32_t noAcc, bool values, uint64_t m, uint64_t b, evalParam bParam,
                              btFunc acc) {
    clock_t start = clock();
    btSettings minSettings = find_best_BT(noAcc, values, m, b, bParam, acc);
    float elapsed = (float) (clock() - start) / CLOCKS_PER_SEC;
    std::cout << "Binary Tree ORAM: B: " << minSettings.B << " c: " << minSettings.c << " count: " << minSettings.count
              << " out: " << minSettings.out << " time needed: " << elapsed << std::endl;
    delete minSettings.out;
}

Evaluator::pathSettings Evaluator::find_best_Path(uint32_t noAcc, bool values, uint64_t m, uint64_t b, evalParam bParam,
                                                  evalParam sParam, pathFunc acc) {

    pathSettings minSettings = {UINT16_MAX, new btSettings};

    for(uint16_t stash = sParam.min; stash <= sParam.max; stash = stash*sParam.step_m + sParam.step_p)
        for(uint16_t B = bParam.min; B <= bParam.max; B = B*bParam.step_m + bParam.step_p)
            find_best_Path(noAcc, values, m, b, B, stash, minSettings, acc);


    return minSettings;
}

Evaluator::pathSettings Evaluator::find_best_Path(uint32_t noAcc, bool values, uint64_t m, uint64_t b, uint16_t B,
                                                  uint16_t stash, pathSettings& minSettings, pathFunc acc) {

    uint16_t d = myLog2(m);

    for (uint16_t c = 2; c <= d; c *= 2) {
        for (uint16_t count = 1; count <= floor(log(m)/log(c)); count++) {
            outType& out = multiplyWR(noAcc, acc(noAcc, values, m, b, B, c, stash, count));
            if (out < *minSettings.bt->out) {
                *minSettings.bt->out = {out.gates, out.traffic, out.rounds};
                *minSettings.bt = {B, c, count, minSettings.bt->out};
                minSettings = {stash, minSettings.bt};
            }
            delete &out;
        }
    }
    return minSettings;
}

void Evaluator::print_best_Path(uint32_t noAcc, bool values, uint64_t m, uint64_t b, std::string type, evalParam bParam, evalParam sParam,
                                                  pathFunc acc) {
    clock_t start = clock();
    pathSettings minSettings = find_best_Path(noAcc, values, m, b, bParam, sParam, acc);
    float elapsed = (float)(clock() - start) / CLOCKS_PER_SEC;
    std::cout << type << ": B: " << minSettings.bt->B << " c: " << minSettings.bt->c << " count: " << minSettings.bt->count << " stash: " << minSettings.stash << " out: " << minSettings.bt->out << " time needed: " << elapsed << std::endl;
    delete minSettings.bt->out;
    delete minSettings.bt;
}

Evaluator::sqrSettings Evaluator::find_best_OSQR(uint32_t noAcc, bool values, uint64_t m, uint64_t b, sqrFunc acc) {

    sqrSettings minSettings = {UINT16_MAX, UINT16_MAX, new outType};
    for (uint16_t c = 2; c <= myLog2(m); c *= 2) {
        for (uint16_t count = 1; count <= floor(log(m)/log(c)); count++) {
            outType& out = 1*acc(noAcc, values, m, b, c, count);
            if (out < *minSettings.out) {
                *minSettings.out = {out.gates, out.traffic, out.rounds};
                minSettings = {c, count, minSettings.out};
            }
            delete &out;
        }
    }
    return minSettings;
}

void Evaluator::print_best_OSQR(uint32_t noAcc, bool values, uint64_t m, uint64_t b, sqrFunc acc) {
    clock_t start = clock();
    sqrSettings minSettings = find_best_OSQR(noAcc, values, m, b, acc);
    float elapsed = (float)(clock() - start) / CLOCKS_PER_SEC;
    std::cout << "Optimized Square-Root ORAM: c: " << minSettings.c << " count: " << minSettings.count << " out: " << minSettings.out << " time needed: " << elapsed << std::endl;
    delete minSettings.out;
}

void Evaluator::evaluate(uint32_t noRead, uint32_t noWrite, bool values, uint64_t m, uint64_t b, uint16_t d, funcTypes func) {
    uint32_t noAcc = noRead + noWrite;

    print_LinearScan(noRead, noWrite, m, b);
    print_best_BT(noAcc, values, m, b, StandardBTB(d), func.btF);
    print_best_Path(noAcc, values, m, b, "Path ORAM", StandardPathB(d), StandardPathS(), func.pathF);
    print_best_Path(noAcc, values, m, b, "PathSC ORAM", StandardPathSCB(d), StandardPathS(), func.pathSCF);
    print_best_Path(noAcc, values, m, b, "SCORAM", StandardScoramB(d), StandardScoramS(d), func.SCORAMF);
    print_best_Path(noAcc, values, m, b, "Circuit ORAM", StandardPathB(d), StandardPathS(), func.CORAMF);
    print_best_OSQR(noAcc, values, m, b, func.OSQRF);
}

void Evaluator::evaluate_acc_exact(uint32_t noRead, uint32_t noWrite, uint64_t m, uint64_t b) {
    uint16_t d = myLog2(m);
    std::cout << "Evaluate Access: noRead: " << noRead << " noWrite: " << noWrite << " d: " << d << " b: " << b << std::endl;
    evaluate(noRead, noWrite, false, m, b, d, acc_slow_func);
    // does not make sense for Mixed ORAM
    std::cout << std::endl;
}

void Evaluator::evaluate_exact(uint32_t noRead, uint32_t noWrite, bool values, uint64_t m, uint64_t b) {
    uint16_t d = myLog2(m);
    std::cout << "Evaluate: noRead: " << noRead << " noWrite: " << noWrite << " values: " << values << " d: " << d << " b: " << b << std::endl;
    evaluate(noRead, noWrite, values, m, b, d, slow_func);
    print_best_Path(noRead+noWrite, values, m, b, "Mixed ORAM", StandardPathB(d), StandardPathS(), Mixed_ORAM_slow);
    std::cout << std::endl;
}

void Evaluator::evaluate_acc_exact_fast(uint32_t noRead, uint32_t noWrite, uint64_t m, uint64_t b) {
    uint16_t d = myLog2(m);
    std::cout << "Evaluate Access fast: noRead: " << noRead << " noWrite: " << noWrite << " d: " << d << " b: " << b << std::endl;
    evaluate(noRead, noWrite, false, m, b, d, acc_fast_func);
    // does not make sense for Mixed ORAM
    std::cout << std::endl;
}

void Evaluator::evaluate_exact_fast(uint32_t noRead, uint32_t noWrite, bool values, uint64_t m, uint64_t b) {
    uint16_t d = myLog2(m);
    std::cout << "Evaluate fast: noRead: " << noRead << " noWrite: " << noWrite << " d: " << d << " b: " << b << std::endl;
    evaluate(noRead, noWrite, values, m, b, d, fast_func);
    //print_best_Path(noRead+noWrite, values, m, b, "Mixed ORAM", StandardPathB(d), StandardPathS(), Mixed_ORAM_fast);      // TODO
    std::cout << std::endl;
}