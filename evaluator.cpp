//
// Created by weber on 09.07.2017.
//

#include "evaluator.h"
#include "fast_calculator.h"

outType& Evaluator::find_LinearScan(uint32_t noRead, uint32_t noWrite, uint64_t m, uint64_t b) {
    outType out = noRead * LinearScan::read(m, b) + noWrite * LinearScan::write(m, b);
    std::cout << "Linear Scan: " << out << std::endl;
    return out;
}

outType& Evaluator::find_LinearScanORAM(uint32_t noAcc, uint64_t m, uint64_t b) {
    LinearScanOram* oram = (new LinearScanOram(m, b));
    outType& out = noAcc*oram->c_acc(b);
    std::cout << "Linear Scan ORAM: " << out << std::endl;
    delete oram;
    return out;
}

Evaluator::btSettings* Evaluator::find_best_BT(uint32_t noAcc, uint64_t m, uint64_t b, std::string type, evalParam bParam,
                             uint64_t (*acc)(uint64_t m, uint64_t b, uint16_t B, uint16_t c, uint16_t count)) {
    clock_t start = clock();
    btSettings* minSettings = new btSettings;
    *minSettings = {0, 0, 0, UINT64_MAX};
    uint16_t d = myLog2(m);

    for(uint16_t B = bParam.min; B <= bParam.max; B = B*bParam.step_m + bParam.step_p) {
        for(uint16_t c = 2; c <= d; c *= 2) {
            for(uint16_t count = 1; count <= floor(log(m)/log(c)); count++) {
                uint64_t out = noAcc * acc(m, b, B, c, count);
                if(out < minSettings->out)
                    *minSettings = {B, c, count, out};
            }
        }
    }
    float elapsed = (float)(clock() - start) / CLOCKS_PER_SEC;
    std::cout << type << ": B: " << minSettings->B << " c: " << minSettings->c << " count: " << minSettings->count << " out: " << minSettings->out << " time needed: " << elapsed << std::endl;
    return minSettings;
}

uint64_t acc_BT_slow(uint64_t m, uint64_t b, uint16_t B, uint16_t c, uint16_t count) {
    BinaryTree* oram = TreeFactory::create_BT(m, b, B, c, count);
    uint64_t out = oram->c_acc(b).gates;
    delete oram;
    return out;
}

uint64_t acc_BT_fast(uint64_t m, uint64_t b, uint16_t B, uint16_t c, uint16_t count) {
    return c_acc_BT(m, b, B, count, c).gates;
}

Evaluator::pathSettings* Evaluator::find_best_Path(uint32_t noAcc, uint64_t m, uint64_t b, std::string type, evalParam bParam, evalParam sParam,
                               uint64_t (*acc)(uint64_t newM, uint64_t newB, uint16_t B, uint16_t c, uint16_t stash, uint16_t count)) {
    clock_t start = clock();
    btSettings* minBTSettings = new btSettings;
    pathSettings* minSettings = new pathSettings;
    minSettings->bt->out = UINT64_MAX;
    uint16_t d = myLog2(m);

    for(uint16_t stash = sParam.min; stash <= sParam.max; stash = stash*sParam.step_m + sParam.step_p) {
        for(uint16_t B = bParam.min; B <= bParam.max; B = B*bParam.step_m + bParam.step_p) {
            for (uint16_t c = 2; c <= d; c *= 2) {
                for (uint16_t count = 1; count <= floor(log(m)/log(c)); count++) {
                    uint64_t access = acc(m, b, B, c, stash, count);
                    uint64_t out = noAcc * access;

                    if (out < minSettings->bt->out && myLog2(noAcc) + myLog2(access) < 64) {
                        *minBTSettings = {B, c, count, out};
                        *minSettings = {minBTSettings, stash};
                    }
                }
            }
        }
    }
    float elapsed = (float)(clock() - start) / CLOCKS_PER_SEC;
    std::cout << type << ": B: " << minSettings->bt->B << " c: " << minSettings->bt->c << " count: " << minSettings->bt->count << " stash: " << minSettings->stash << " out: " << minSettings->bt->out << " time needed: " << elapsed << std::endl;
    return minSettings;
}

uint64_t acc_Path_slow(uint64_t m, uint64_t b, uint16_t B, uint16_t c, uint16_t stash, uint16_t count) {
    Path* path = TreeFactory::create_Path("Path", m, b, B, c, stash, count);
    uint64_t out = path->c_acc(b).gates;
    delete path;
    return out;
}

uint64_t acc_Path_fast(uint64_t m, uint64_t b, uint16_t B, uint16_t c, uint16_t stash, uint16_t count) {
    return c_acc_Path(m, b, B, count, stash, c).gates;
}

uint64_t acc_PathSC_slow(uint64_t m, uint64_t b, uint16_t B, uint16_t c, uint16_t stash, uint16_t count) {
    PathSC* path = (PathSC*) TreeFactory::create_Path("PathSC", m, b, B, c, stash, count);
    uint64_t out = path->c_acc(b).gates;
    delete path;
    return out;
}

uint64_t acc_PathSC_fast(uint64_t m, uint64_t b, uint16_t B, uint16_t c, uint16_t stash, uint16_t count) {
    return c_acc_PSC(m, b, B, count, stash, c).gates;
}

uint64_t acc_SCORAM_slow(uint64_t m, uint64_t b, uint16_t B, uint16_t c, uint16_t stash, uint16_t count) {
    Scoram* path = (Scoram*) TreeFactory::create_Path("Scoram", m, b, B, c, stash, count);
    uint64_t out = path->c_acc(b).gates;
    delete path;
    return out;
}

uint64_t acc_SCORAM_fast(uint64_t m, uint64_t b, uint16_t B, uint16_t c, uint16_t stash, uint16_t count) {
    return c_acc_SCORAM(m, b, B, count, stash, c).gates;
}

uint64_t acc_CORAM_slow(uint64_t m, uint64_t b, uint16_t B, uint16_t c, uint16_t stash, uint16_t count) {
    Coram* path = (Coram*) TreeFactory::create_Path("CORAM", m, b, B, c, stash, count);
    uint64_t out = path->c_acc(b).gates;
    delete path;
    return out;
}

uint64_t acc_CORAM_fast(uint64_t m, uint64_t b, uint16_t B, uint16_t c, uint16_t stash, uint16_t count) {
    return c_acc_CORAM(m, b, B, count, stash, c).gates;
}

void Evaluator::evaluate_exact(uint32_t noRead, uint32_t noWrite, uint64_t m, uint64_t b) {
    clock_t start = clock();
    uint16_t d = myLog2(m);

    find_LinearScan(noRead, noWrite, m, b);
    find_LinearScanORAM((noRead+noWrite), m, b);

    btSettings* minBTS = find_best_BT(noRead+noWrite, m, b, "Binary Tree ORAM - slow", evalParam{d, (uint16_t) (10*d), 1, d}, acc_BT_slow);
    delete minBTS;
    pathSettings* minPath = find_best_Path(noRead+noWrite, m, b, "Path ORAM - slow", evalParam{4, d, 1, 2}, evalParam{d, (uint16_t) 5*d, 1, 1}, acc_Path_slow);
    delete minPath->bt;
    delete minPath;

    pathSettings* minPathSC = find_best_Path(noRead+noWrite, m, b, "PathSC ORAM - slow", evalParam{4, d, 2, 0}, evalParam{d, (uint16_t) 5*d, 1, 1}, acc_PathSC_slow);
    delete minPathSC->bt;
    delete minPathSC;

    pathSettings* minSCORAM = find_best_Path(noRead+noWrite, m, b, "SCORAM - slow", evalParam{6, d, 1, 2}, evalParam{d, (uint16_t) 5*d, 1, 1}, acc_SCORAM_slow);
    delete minSCORAM->bt;
    delete minSCORAM;

    pathSettings* minCORAM = find_best_Path(noRead+noWrite, m, b, "Circuit ORAM - slow", evalParam{4, d, 1, 2}, evalParam{d, (uint16_t) 5*d, 1, 1}, acc_CORAM_slow);
    delete minCORAM->bt;
    delete minCORAM;

    float elapsed = (float)(clock() - start) / CLOCKS_PER_SEC;
    std::cout << "time needed: " << elapsed << "\n" << std::endl;
}

void Evaluator::evaluate_exact_fast(uint32_t noRead, uint32_t noWrite, uint64_t m, uint64_t b) {
    clock_t start = clock();
    uint16_t d = myLog2(m);

    find_LinearScan(noRead, noWrite, m, b);
    find_LinearScanORAM((noRead+noWrite), m, b);

    btSettings* minBTS = find_best_BT(noRead+noWrite, m, b, "Binary Tree ORAM - fast", evalParam{d, (uint16_t) (10*d), 1, d}, acc_BT_fast);
    delete minBTS;
    pathSettings* minPath = find_best_Path(noRead+noWrite, m, b, "Path ORAM - fast", evalParam{4, d, 1, 2}, evalParam{d, (uint16_t) 5*d, 1, 1}, acc_Path_fast);
    delete minPath->bt;
    delete minPath;

    pathSettings* minPathSC = find_best_Path(noRead+noWrite, m, b, "PathSC ORAM - fast", evalParam{4, d, 2, 0}, evalParam{d, (uint16_t) 5*d, 1, 1}, acc_PathSC_fast);
    delete minPathSC->bt;
    delete minPathSC;

    pathSettings* minSCORAM = find_best_Path(noRead+noWrite, m, b, "SCORAM - fast", evalParam{6, d, 1, 2}, evalParam{d, (uint16_t) 5*d, 1, 1}, acc_SCORAM_fast);
    delete minSCORAM->bt;
    delete minSCORAM;

    pathSettings* minCORAM = find_best_Path(noRead+noWrite, m, b, "Circuit ORAM - fast", evalParam{4, d, 1, 2}, evalParam{d, (uint16_t) 5*d, 1, 1}, acc_CORAM_fast);
    delete minCORAM->bt;
    delete minCORAM;
    float elapsed = (float)(clock() - start) / CLOCKS_PER_SEC;
    std::cout << "time needed: " << elapsed << "\n" << std::endl;
}