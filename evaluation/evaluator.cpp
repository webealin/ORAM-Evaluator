//
// Created by weber on 09.07.2017.
//

#include "evaluator.h"

minSettings Evaluator::find_LinearScan(uint32_t noRead, uint32_t noWrite, uint64_t m, uint64_t b) {
    outType& out = addWR(multiplyWR(noRead, TrivialLinearScan::c_read(m, b)),
                         multiplyWR(noWrite, TrivialLinearScan::c_write(m, b)));
    return minSettings("Trivial Linear Scan", 0, 0, &out);
}

void Evaluator::print_LinearScan(uint32_t noRead, uint32_t noWrite, uint64_t m, uint64_t b) {
    minSettings settings = find_LinearScan(noRead, noWrite, m, b);
    std::cout << "Linear Scan: " << *settings.out << std::endl;
    delete settings.out;
}

btSettings Evaluator::find_best_BT(uint32_t noAcc, bool values, uint64_t m, uint64_t b, evalParam bParam,
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
            minSettings = {"Binary Tree ORAM", B, c, count, minSettings.out};
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

pathSettings Evaluator::find_best_Path(uint32_t noAcc, bool values, uint64_t m, uint64_t b, std::string type,
                                       evalParam bParam, evalParam sParam, pathFunc acc) {
    pathSettings minSettings;
    //std::cout << "d: " << myLog2(m) << " bParam min: " << bParam.min << " bParam max: " << bParam.max << " sParam min: " << sParam.min << " sParam max: " << sParam.max << std::endl;
    for(uint16_t stash = sParam.min; stash <= sParam.max; stash = stash*sParam.step_m + sParam.step_p)
        for(uint16_t B = bParam.min; B <= bParam.max; B = B*bParam.step_m + bParam.step_p)
            find_best_Path(noAcc, values, m, b, type, B, stash, minSettings, acc);


    return minSettings;
}

pathSettings Evaluator::find_best_Path(uint32_t noAcc, bool values, uint64_t m, uint64_t b, std::string type, uint16_t B,
                                                  uint16_t stash, pathSettings& minSettings, pathFunc acc) {
    uint16_t d = myLog2(m);

    for (uint16_t c = 2; c <= d; c *= 2) {
        for (uint16_t count = 1; count <= floor(log(m)/log(c)); count++) {
            outType& out = multiplyWR(noAcc, acc(noAcc, values, m, b, B, c, stash, count));
            //std::cout << "B: " << B << " stash: " << stash << " c: " << c << " count: " << count << " out: " << out << std::endl;
            if (out < *minSettings.out) {
                *minSettings.out = {out.gates, out.traffic, out.rounds};
                minSettings = {type, B, c, count, stash, minSettings.out};
            }
            delete &out;
        }
    }
    return minSettings;
}

void Evaluator::print_best_Path(uint32_t noAcc, bool values, uint64_t m, uint64_t b, std::string type, evalParam bParam, evalParam sParam,
                                                  pathFunc acc) {
    clock_t start = clock();
    pathSettings min = find_best_Path(noAcc, values, m, b, type, bParam, sParam, acc);
    float elapsed = (float)(clock() - start) / CLOCKS_PER_SEC;
    std::cout << type << ": B: " << min.B << " c: " << min.c << " count: " << min.count << " stash: " << min.stash << " out: " << min.out << " time needed: " << elapsed << std::endl;
    delete min.out;
}

minSettings Evaluator::find_best_OSQR(uint32_t noAcc, bool values, uint64_t m, uint64_t b, minSettings& min, sqrFunc acc) {

    for (uint16_t c = 2; c <= myLog2(m); c *= 2) {
        for (uint16_t count = 1; count <= floor(log(m)/log(c)); count++) {
            outType& out = 1*acc(noAcc, values, m, b, c, count);
            if (out < *min.out) {
                *min.out = {out.gates, out.traffic, out.rounds};
                min = {"Optimized Square Root ORAM", c, count, min.out};
            }
            delete &out;
        }
    }
    return min;
}

minSettings Evaluator::find_best_OSQR(uint32_t noAcc, bool values, uint64_t m, uint64_t b, sqrFunc acc) {
    minSettings min;
    find_best_OSQR(noAcc, values, m, b, min, acc);
    return min;
}

void Evaluator::print_best_OSQR(uint32_t noAcc, bool values, uint64_t m, uint64_t b, sqrFunc acc) {
    clock_t start = clock();
    minSettings min;
    find_best_OSQR(noAcc, values, m, b, min, acc);
    float elapsed = (float)(clock() - start) / CLOCKS_PER_SEC;
    std::cout << "Optimized Square-Root ORAM: c: " << min.c << " count: " << min.count << " out: " << min.out << " time needed: " << elapsed << std::endl;
    delete min.out;
}

pathSettings Evaluator::find_best_MIXO(uint32_t noAcc, bool values, uint64_t m, uint64_t b, uint16_t B,
                                       uint16_t stash, pathSettings& minSettings, MixedORAM::evalMap& seen) {

    for (uint16_t c = 2; c <= myLog2(m); c *= 2) {
        for (uint16_t count = 1; count <= floor(log(m)/log(c)); count++) {
            MixedORAM* oram = ORAMFactory::create_Mixed_ORAM(noAcc, values, m, b, B, c, stash, count, seen);
            outType& out = multiplyWR(noAcc, oram->c_amortized(noAcc, values));
            delete oram;

            if (out < *minSettings.out) {
                *minSettings.out = {out.gates, out.traffic, out.rounds};
                minSettings = {"Mixed ORAM", B, c, count, stash, minSettings.out};
            }
            delete &out;
        }
    }
    return minSettings;
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
    //print_best_Path(noRead+noWrite, values, m, b, "Mixed ORAM", StandardPathB(d), StandardPathS(), Mixed_ORAM_slow);
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

minSettings Evaluator::evaluate(uint32_t noSecretRead, uint32_t noConstRead, uint32_t noSecretWrite, uint32_t noConstWrite,
                         bool values, uint64_t m, uint64_t b) {
    uint32_t noAcc = noSecretRead + noConstRead + noSecretWrite + noConstWrite;

    funcTypes func = slow_func;             // TODO
    minSettings lsRes = find_LinearScan(noSecretRead, noSecretWrite, m, b);
    std::cout << "\nTrivial Linear Scan result: " << *lsRes.out << std::endl;
    pathSettings coramRes = find_best_Path(noAcc, values, m, b, "Circuit ORAM", StandardPathB(myLog2(m)), StandardPathS(), func.CORAMF);
    std::cout << "Circuit ORAM result: " << *coramRes.out << std::endl;
    minSettings osqrRes = find_best_OSQR(noAcc, values, m, b, func.OSQRF);
    std::cout << "Optimized Square Root ORAM result: " << *osqrRes.out << "\n" << std::endl;

    if(*lsRes.out < *coramRes.out && *lsRes.out < *osqrRes.out) {
        delete coramRes.out;
        delete osqrRes.out;
        return lsRes;
    }

    if(*coramRes.out < *osqrRes.out) {
        delete lsRes.out;
        delete osqrRes.out;
        return coramRes;
    }

    delete coramRes.out;
    delete lsRes.out;
    return osqrRes;
}
