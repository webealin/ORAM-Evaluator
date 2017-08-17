//
// Created by weber on 09.07.2017.
//

#include "evaluator.h"

outType Evaluator::find_LinearScan(uint32_t noRead, uint32_t noWrite, uint64_t m, uint64_t b) {
    outType& out = addWR(multiplyWR(noRead, TrivialLinearScan::c_read(m, b)), multiplyWR(noWrite,
                                                                                         TrivialLinearScan::c_write(m,
                                                                                                                    b)));
    outType ret = {out.gates, out.traffic, out.rounds};
    delete &out;
    return ret;
}

void Evaluator::print_LinearScan(uint32_t noRead, uint32_t noWrite, uint64_t m, uint64_t b) {
    std::cout << "Linear Scan: " << find_LinearScan(noRead, noWrite, m, b) << std::endl;
}

outType Evaluator::find_LinearScanORAM(uint32_t noAcc, uint64_t m, uint64_t b) {
    auto* oram = new LinearScanOram(m, b);
    outType& out = multiplyWR(noAcc, oram->c_acc(b));
    delete oram;
    outType ret = {out.gates, out.traffic, out.rounds};
    delete &out;
    return ret;
}

void Evaluator::print_LinearScanORAM(uint32_t noAcc, uint64_t m, uint64_t b) {
    std::cout << "Linear Scan ORAM: " << find_LinearScanORAM(noAcc, m, b) << std::endl;
}

Evaluator::btSettings Evaluator::find_best_BT(uint32_t noAcc, uint64_t m, uint64_t b, evalParam bParam,
                                               outType& (*acc)(uint64_t m, uint64_t b, uint16_t B, uint16_t c, uint16_t count)) {
    outType minOut = {UINT64_MAX, UINT64_MAX, UINT16_MAX};
    btSettings minSettings = {UINT16_MAX, UINT16_MAX, UINT16_MAX, &minOut};
    uint16_t d = myLog2(m);

    for (uint16_t B = bParam.min; B <= bParam.max; B = B * bParam.step_m + bParam.step_p) {
        for (uint16_t c = 2; c <= d; c *= 2) {
            for (uint16_t count = 1; count <= floor(log(m) / log(c)); count++) {
                outType &out = multiplyWR(noAcc, acc(m, b, B, c, count));
                if (out < minOut) {
                    minOut = {out.gates, out.traffic, out.rounds};
                    minSettings = {B, c, count, &minOut};
                }
                delete &out;
            }
        }
    }
    return minSettings;
}

void Evaluator::print_best_BT_slow(uint32_t noAcc, uint64_t m, uint64_t b, evalParam bParam) {
    clock_t start = clock();
    btSettings minSettings = find_best_BT(noAcc, m, b, bParam, acc_BT_slow);
    outType out = *(minSettings.out);
    float elapsed = (float) (clock() - start) / CLOCKS_PER_SEC;
    std::cout << "Binary Tree ORAM: B: " << minSettings.B << " c: " << minSettings.c << " count: " << minSettings.count
              << " out: " << out << " time needed: " << elapsed << std::endl;
}

void Evaluator::print_best_BT_fast(uint32_t noAcc, uint64_t m, uint64_t b, evalParam bParam) {
    clock_t start = clock();
    btSettings minSettings = find_best_BT(noAcc, m, b, bParam, acc_BT_fast);
    outType out = *(minSettings.out);
    float elapsed = (float) (clock() - start) / CLOCKS_PER_SEC;
    std::cout << "Binary Tree ORAM: B: " << minSettings.B << " c: " << minSettings.c << " count: " << minSettings.count
              << " out: " << out << " time needed: " << elapsed << std::endl;
}

Evaluator::pathSettings Evaluator::find_best_Path(uint32_t noAcc, uint64_t m, uint64_t b, evalParam bParam, evalParam sParam,
                               outType& (*acc)(uint64_t newM, uint64_t newB, uint16_t B, uint16_t c, uint16_t stash, uint16_t count)) {
    outType minOut = {UINT64_MAX, UINT64_MAX, UINT16_MAX};
    btSettings minBTSettings = {UINT16_MAX, UINT16_MAX, UINT16_MAX, &minOut};
    pathSettings minSettings = {UINT16_MAX, &minBTSettings};
    uint16_t d = myLog2(m);

    for(uint16_t stash = sParam.min; stash <= sParam.max; stash = stash*sParam.step_m + sParam.step_p) {
        for(uint16_t B = bParam.min; B <= bParam.max; B = B*bParam.step_m + bParam.step_p) {
            for (uint16_t c = 2; c <= d; c *= 2) {
                for (uint16_t count = 1; count <= floor(log(m)/log(c)); count++) {
                    outType& out = multiplyWR(noAcc, acc(m, b, B, c, stash, count));
                    if (out < minOut) {
                        minOut = {out.gates, out.traffic, out.rounds};
                        minBTSettings = {B, c, count, &minOut};
                        minSettings = {stash, &minBTSettings};
                    }
                    delete &out;
                }
            }
        }
    }
    return minSettings;
}

void Evaluator::print_best_Path(uint32_t noAcc, uint64_t m, uint64_t b, std::string type, evalParam bParam, evalParam sParam,
                                                  outType& (*acc)(uint64_t newM, uint64_t newB, uint16_t B, uint16_t c, uint16_t stash, uint16_t count)) {
    clock_t start = clock();
    pathSettings minSettings = find_best_Path(noAcc, m, b, bParam, sParam, acc);
    outType out = *(minSettings.bt->out);
    float elapsed = (float)(clock() - start) / CLOCKS_PER_SEC;
    std::cout << type << ": B: " << minSettings.bt->B << " c: " << minSettings.bt->c << " count: " << minSettings.bt->count << " stash: " << minSettings.stash << " out: " << out << " time needed: " << elapsed << std::endl;
}


void Evaluator::evaluate_exact(uint32_t noRead, uint32_t noWrite, uint64_t m, uint64_t b) {
    uint16_t d = myLog2(m);

    print_LinearScan(noRead, noWrite, m, b);
    print_best_BT_slow(noRead + noWrite, m, b, evalParam{d, static_cast<uint16_t>(10*d), 1, d});
    print_best_Path(noRead+noWrite, m, b, "Path ORAM", evalParam{4, d, 1, 2}, evalParam{d, static_cast<uint16_t>(5*d), 1, 1}, acc_Path_slow);
    print_best_Path(noRead+noWrite, m, b, "PathSC ORAM", evalParam{4, d, 2, 0}, evalParam{d, static_cast<uint16_t>(5*d), 1, 1}, acc_PathSC_slow);
    print_best_Path(noRead+noWrite, m, b, "SCORAM", evalParam{6, d, 1, 2}, evalParam{d, static_cast<uint16_t>(5*d), 1, 1}, acc_SCORAM_slow);
    print_best_Path(noRead+noWrite, m, b, "Circuit ORAM", evalParam{4, d, 1, 2}, evalParam{d, static_cast<uint16_t>(5*d), 1, 1}, acc_CORAM_slow);
}

void Evaluator::evaluate_exact_fast(uint32_t noRead, uint32_t noWrite, uint64_t m, uint64_t b) {
    uint16_t d = myLog2(m);

    print_LinearScan(noRead, noWrite, m, b);
    print_best_BT_fast(noRead + noWrite, m, b, evalParam{d, static_cast<uint16_t>(10*d), 1, d});
    print_best_Path(noRead+noWrite, m, b, "Path ORAM", evalParam{4, d, 1, 2}, evalParam{d, static_cast<uint16_t>(5*d), 1, 1}, acc_Path_fast);
    print_best_Path(noRead+noWrite, m, b, "PathSC ORAM", evalParam{4, d, 2, 0}, evalParam{d, static_cast<uint16_t>(5*d), 1, 1}, acc_PathSC_fast);
    print_best_Path(noRead+noWrite, m, b, "SCORAM", evalParam{6, d, 1, 2}, evalParam{d, static_cast<uint16_t>(5*d), 1, 1}, acc_SCORAM_fast);
    print_best_Path(noRead+noWrite, m, b, "Circuit ORAM", evalParam{4, d, 1, 2}, evalParam{d, static_cast<uint16_t>(5*d), 1, 1}, acc_CORAM_fast);
}