//
// Created by weber on 09.07.2017.
//

#include "evaluator.h"
#include "../fast_calculator.h"

outType& Evaluator::find_LinearScan(uint32_t noRead, uint32_t noWrite, uint64_t m, uint64_t b) {
    outType& out = noRead * TrivialLinearScan::read(m, b) + noWrite * TrivialLinearScan::write(m, b);
    std::cout << "Linear Scan: " << out << std::endl;
    return out;
}

outType& Evaluator::find_LinearScanORAM(uint32_t noAcc, uint64_t m, uint64_t b) {
    auto* oram = (new LinearScanOram(m, b));
    outType& out = noAcc*oram->c_acc(b);
    std::cout << "Linear Scan ORAM: " << out << std::endl;
    delete oram;
    return out;
}

Evaluator::btSettings* Evaluator::find_best_BT(uint32_t noAcc, uint64_t m, uint64_t b, std::string type, evalParam bParam,
                             outType& (*acc)(uint64_t m, uint64_t b, uint16_t B, uint16_t c, uint16_t count)) {
    clock_t start = clock();
    auto *minSettings = new btSettings;         // TODO: Settings vllt einfach nicht als Pointer speichern? ^^
    *minSettings = {0, 0, 0, new outType};
    minSettings->out->gates = UINT64_MAX;
    uint16_t d = myLog2(m);

    for (uint16_t B = bParam.min; B <= bParam.max; B = B * bParam.step_m + bParam.step_p) {
        for (uint16_t c = 2; c <= d; c *= 2) {
            for (uint16_t count = 1; count <= floor(log(m) / log(c)); count++) {
                /*outType& accOut = */
                outType &out = noAcc * acc(m, b, B, c, count);
                if (out < *minSettings->out) {
                    delete &(minSettings->out);
                    *minSettings = {B, c, count, &out};
                } else delete &out;
            }
        }
    }
    float elapsed = (float) (clock() - start) / CLOCKS_PER_SEC;
    std::cout << type << ": B: " << minSettings->B << " c: " << minSettings->c << " count: " << minSettings->count
              << " out: " << *(minSettings->out) << " time needed: " << elapsed << std::endl;
    return minSettings;
}

outType& acc_BT_slow(uint64_t m, uint64_t b, uint16_t B, uint16_t c, uint16_t count) {
    BinaryTree* oram = TreeFactory::create_BT(m, b, B, c, count);
    outType& out = oram->c_acc(b);
    delete oram;
    return out;
}

outType& acc_BT_fast(uint64_t m, uint64_t b, uint16_t B, uint16_t c, uint16_t count) {
    return c_acc_BT(m, b, B, count, c);
}

Evaluator::pathSettings Evaluator::find_best_Path(uint32_t noAcc, uint64_t m, uint64_t b, std::string type, evalParam bParam, evalParam sParam,
                               outType& (*acc)(uint64_t newM, uint64_t newB, uint16_t B, uint16_t c, uint16_t stash, uint16_t count)) {
    clock_t start = clock();
    outType minOut = {UINT64_MAX, UINT64_MAX, UINT16_MAX};
    btSettings minBTSettings = {UINT16_MAX, UINT16_MAX, UINT16_MAX, &minOut};
    pathSettings minSettings = {UINT16_MAX, &minBTSettings};
    uint16_t d = myLog2(m);

    for(uint16_t stash = sParam.min; stash <= sParam.max; stash = stash*sParam.step_m + sParam.step_p) {
        for(uint16_t B = bParam.min; B <= bParam.max; B = B*bParam.step_m + bParam.step_p) {
            for (uint16_t c = 2; c <= d; c *= 2) {
                for (uint16_t count = 1; count <= floor(log(m)/log(c)); count++) {
                    outType& out = noAcc*acc(m, b, B, c, stash, count);
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
    float elapsed = (float)(clock() - start) / CLOCKS_PER_SEC;
    std::cout << type << ": B: " << minSettings.bt->B << " c: " << minSettings.bt->c << " count: " << minSettings.bt->count << " stash: " << minSettings.stash << " out: " << *(minSettings.bt->out) << " time needed: " << elapsed << std::endl;
    return minSettings;
}

outType& acc_Path_slow(uint64_t m, uint64_t b, uint16_t B, uint16_t c, uint16_t stash, uint16_t count) {
    Path* path = TreeFactory::create_Path("Path", m, b, B, c, stash, count);
    outType& out = path->c_acc(b);
    delete path;
    return out;
}

outType& acc_Path_fast(uint64_t m, uint64_t b, uint16_t B, uint16_t c, uint16_t stash, uint16_t count) {
    return c_acc_Path(m, b, B, count, stash, c);
}

outType& acc_PathSC_slow(uint64_t m, uint64_t b, uint16_t B, uint16_t c, uint16_t stash, uint16_t count) {
    PathSC* path = (PathSC*) TreeFactory::create_Path("PathSC", m, b, B, c, stash, count);
    outType& out = path->c_acc(b);
    delete path;
    return out;
}

outType& acc_PathSC_fast(uint64_t m, uint64_t b, uint16_t B, uint16_t c, uint16_t stash, uint16_t count) {
    return c_acc_PSC(m, b, B, count, stash, c);
}

outType& acc_SCORAM_slow(uint64_t m, uint64_t b, uint16_t B, uint16_t c, uint16_t stash, uint16_t count) {
    Scoram* path = (Scoram*) TreeFactory::create_Path("Scoram", m, b, B, c, stash, count);
    outType& out = path->c_acc(b);
    delete path;
    return out;
}

outType& acc_SCORAM_fast(uint64_t m, uint64_t b, uint16_t B, uint16_t c, uint16_t stash, uint16_t count) {
    return c_acc_SCORAM(m, b, B, count, stash, c);
}

outType& acc_CORAM_slow(uint64_t m, uint64_t b, uint16_t B, uint16_t c, uint16_t stash, uint16_t count) {
    Coram* path = (Coram*) TreeFactory::create_Path("CORAM", m, b, B, c, stash, count);
    outType& out = path->c_acc(b);
    delete path;
    return out;
}

outType& acc_CORAM_fast(uint64_t m, uint64_t b, uint16_t B, uint16_t c, uint16_t stash, uint16_t count) {
    return c_acc_CORAM(m, b, B, count, stash, c);
}

void Evaluator::evaluate_exact(uint32_t noRead, uint32_t noWrite, uint64_t m, uint64_t b) {
    std::cout << "Evaluate exact - slow" << std::endl;
    uint16_t d = myLog2(m);

    /*outType& linearScan = find_LinearScan(noRead, noWrite, m, b);
    delete &linearScan;

    outType& linearScanOram = find_LinearScanORAM((noRead+noWrite), m, b);
    delete &linearScanOram;*/

    for(int i = 0; i < 10; i++) {
        btSettings *minBTS = find_best_BT(noRead + noWrite, m, b, "Binary Tree ORAM",
                                          evalParam{d, static_cast<uint16_t>(5 * d), 1, d}, acc_BT_slow);
        delete minBTS;
    }

    //find_best_Path(noRead+noWrite, m, b, "Path ORAM", evalParam{4, d, 1, 2}, evalParam{d,  static_cast<uint16_t>(5*d), 1, 1}, acc_Path_slow);

    /*pathSettings* minPathSC = find_best_Path(noRead+noWrite, m, b, "PathSC ORAM", evalParam{4, d, 2, 0}, evalParam{d,  static_cast<uint16_t>(5*d), 1, 1}, acc_PathSC_slow);
    delete minPathSC->bt;
    delete minPathSC;

    pathSettings* minSCORAM = find_best_Path(noRead+noWrite, m, b, "SCORAM", evalParam{6, d, 1, 2}, evalParam{d,  static_cast<uint16_t>(5*d), 1, 1}, acc_SCORAM_slow);
    delete minSCORAM->bt;
    delete minSCORAM;

    pathSettings* minCORAM = find_best_Path(noRead+noWrite, m, b, "Circuit ORAM", evalParam{4, d, 1, 2}, evalParam{d,  static_cast<uint16_t>(5*d), 1, 1}, acc_CORAM_slow);
    delete minCORAM->bt;
    delete minCORAM;*/
}

void Evaluator::evaluate_exact_fast(uint32_t noRead, uint32_t noWrite, uint64_t m, uint64_t b) {
    uint16_t d = myLog2(m);

    find_LinearScan(noRead, noWrite, m, b);
    find_LinearScanORAM((noRead+noWrite), m, b);

    btSettings* minBTS = find_best_BT(noRead+noWrite, m, b, "Binary Tree ORAM - fast", evalParam{d, static_cast<uint16_t>(5*d), 1, d}, acc_BT_fast);
    delete minBTS;

    /*pathSettings* minPath = find_best_Path(noRead+noWrite, m, b, "Path ORAM - fast", evalParam{4, d, 1, 2}, evalParam{d, static_cast<uint16_t>(5*d), 1, 1}, acc_Path_fast);
    delete minPath->bt;
    delete minPath;

    pathSettings* minPathSC = find_best_Path(noRead+noWrite, m, b, "PathSC ORAM - fast", evalParam{4, d, 2, 0}, evalParam{d,  static_cast<uint16_t>(5*d), 1, 1}, acc_PathSC_fast);
    delete minPathSC->bt;
    delete minPathSC;

    pathSettings* minSCORAM = find_best_Path(noRead+noWrite, m, b, "SCORAM - fast", evalParam{6, d, 1, 2}, evalParam{d,  static_cast<uint16_t>(5*d), 1, 1}, acc_SCORAM_fast);
    delete minSCORAM->bt;
    delete minSCORAM;

    pathSettings* minCORAM = find_best_Path(noRead+noWrite, m, b, "Circuit ORAM - fast", evalParam{4, d, 1, 2}, evalParam{d,  static_cast<uint16_t>(5*d), 1, 1}, acc_CORAM_fast);
    delete minCORAM->bt;
    delete minCORAM;*/
}