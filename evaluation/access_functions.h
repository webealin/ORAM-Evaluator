//
// Created by alina on 31.08.17.
//

#ifndef ORAMEVALUATOR_ACCESS_FUNCTIONS_H
#define ORAMEVALUATOR_ACCESS_FUNCTIONS_H


#include "../types/outType.h"
#include "../oram/path_oram.h"
#include "../fast_calculator.h"
#include "../oram/oram_factory.h"

inline outType& acc_BT_slow(uint32_t noAcc, bool dummy, uint64_t m, uint64_t b, uint16_t B, uint16_t c, uint16_t count) {
    BinaryTree* oram = ORAMFactory::create_BT(m, b, B, c, count);
    outType& out = oram->c_acc(b);
    delete oram;
    return out;
}

inline outType& BT_slow(uint32_t noAcc, bool values, uint64_t m, uint64_t b, uint16_t B, uint16_t c, uint16_t count) {
    BinaryTree* oram = ORAMFactory::create_BT(m, b, B, c, count);
    outType& out = addWR(oram->c_init(values), multiplyWR(noAcc, oram->c_acc(b)));
    delete oram;
    return out;
}

inline outType& BT_slow_amortized(uint32_t noAcc, bool values, uint64_t m, uint64_t b, uint16_t B, uint16_t c, uint16_t count) {
    BinaryTree* oram = ORAMFactory::create_BT(m, b, B, c, count);
    outType& out = oram->c_amortized(noAcc, values);
    delete oram;
    return out;
}

inline outType& acc_BT_fast(uint32_t noAcc, bool dummy, uint64_t m, uint64_t b, uint16_t B, uint16_t c, uint16_t count) {
    return c_acc_BT(m, b, B, count, c);
}

inline outType& BT_fast(uint32_t noAcc, bool values, uint64_t m, uint64_t b, uint16_t B, uint16_t c, uint16_t count) {
    return c_BT(noAcc, values, m, b, B, count, c);
}

inline outType& acc_Path_slow(uint32_t noAcc, bool dummy, uint64_t m, uint64_t b, uint16_t B, uint16_t c, uint16_t stash, uint16_t count) {
    Path* path = ORAMFactory::create_Path("Path", m, b, B, c, stash, count);
    outType& out = path->c_acc(b);
    delete path;
    return out;
}

inline outType& Path_slow(uint32_t noAcc, bool values, uint64_t m, uint64_t b, uint16_t B, uint16_t c, uint16_t stash, uint16_t count) {
    Path* oram = ORAMFactory::create_Path("Path", m, b, B, c, stash, count);
    outType& out = addWR(oram->c_init(values), multiplyWR(noAcc, oram->c_acc(b)));
    delete oram;
    return out;
}

inline outType& Path_slow_amortized(uint32_t noAcc, bool values, uint64_t m, uint64_t b, uint16_t B, uint16_t c, uint16_t stash, uint16_t count) {
    Path* oram = ORAMFactory::create_Path("Path", m, b, B, c, stash, count);
    outType& out = oram->c_amortized(noAcc, values);
    delete oram;
    return out;
}

inline outType& acc_Path_fast(uint32_t noAcc, bool dummy, uint64_t m, uint64_t b, uint16_t B, uint16_t c, uint16_t stash, uint16_t count) {
    return c_acc_Path(m, b, B, count, stash, c);
}

inline outType& Path_fast(uint32_t noAcc, bool values, uint64_t m, uint64_t b, uint16_t B, uint16_t c, uint16_t stash, uint16_t count) {
    return c_Path(noAcc, values, m, b, B, count, stash, c);
}

inline outType& acc_PathSC_slow(uint32_t noAcc, bool dummy, uint64_t m, uint64_t b, uint16_t B, uint16_t c, uint16_t stash, uint16_t count) {
    PathSC* path = (PathSC*) ORAMFactory::create_Path("PathSC", m, b, B, c, stash, count);
    outType& out = path->c_acc(b);
    delete path;
    return out;
}

inline outType& PathSC_slow(uint32_t noAcc, bool values, uint64_t m, uint64_t b, uint16_t B, uint16_t c, uint16_t stash, uint16_t count) {
    PathSC* oram = (PathSC*) ORAMFactory::create_Path("PathSC", m, b, B, c, stash, count);
    outType& out = addWR(oram->c_init(values), multiplyWR(noAcc, oram->c_acc(b)));
    delete oram;
    return out;
}

inline outType& PathSC_slow_amortized(uint32_t noAcc, bool values, uint64_t m, uint64_t b, uint16_t B, uint16_t c, uint16_t stash, uint16_t count) {
    PathSC* oram = (PathSC*) ORAMFactory::create_Path("PathSC", m, b, B, c, stash, count);
    outType& out = oram->c_amortized(noAcc, values);
    delete oram;
    return out;
}

inline outType& acc_PathSC_fast(uint32_t noAcc, bool dummy, uint64_t m, uint64_t b, uint16_t B, uint16_t c, uint16_t stash, uint16_t count) {
    return c_acc_PathSC(m, b, B, count, stash, c);
}

inline outType& PathSC_fast(uint32_t noAcc, bool values, uint64_t m, uint64_t b, uint16_t B, uint16_t c, uint16_t stash, uint16_t count) {
    return c_PathSC(noAcc, values, m, b, B, count, stash, c);
}

inline outType& acc_SCORAM_slow(uint32_t noAcc, bool dummy, uint64_t m, uint64_t b, uint16_t B, uint16_t c, uint16_t stash, uint16_t count) {
    Scoram* path = (Scoram*) ORAMFactory::create_Path("Scoram", m, b, B, c, stash, count);
    outType& out = path->c_acc(b);
    delete path;
    return out;
}

inline outType& SCORAM_slow(uint32_t noAcc, bool values, uint64_t m, uint64_t b, uint16_t B, uint16_t c, uint16_t stash, uint16_t count) {
    Scoram* oram = (Scoram*) ORAMFactory::create_Path("Scoram", m, b, B, c, stash, count);
    outType& out = addWR(oram->c_init(values), multiplyWR(noAcc, oram->c_acc(b)));
    delete oram;
    return out;
}

inline outType& SCORAM_slow_amortized(uint32_t noAcc, bool values, uint64_t m, uint64_t b, uint16_t B, uint16_t c, uint16_t stash, uint16_t count) {
    Scoram* oram = (Scoram*) ORAMFactory::create_Path("Scoram", m, b, B, c, stash, count);
    outType& out = oram->c_amortized(noAcc, values);
    delete oram;
    return out;
}

inline outType& acc_SCORAM_fast(uint32_t noAcc, bool dummy, uint64_t m, uint64_t b, uint16_t B, uint16_t c, uint16_t stash, uint16_t count) {
    return c_acc_SCORAM(m, b, B, count, stash, c);
}

inline outType& SCORAM_fast(uint32_t noAcc, bool values, uint64_t m, uint64_t b, uint16_t B, uint16_t c, uint16_t stash, uint16_t count) {
    return c_SCORAM(noAcc, values, m, b, B, count, stash, c);
}

inline outType& acc_CORAM_slow(uint32_t noAcc, bool dummy, uint64_t m, uint64_t b, uint16_t B, uint16_t c, uint16_t stash, uint16_t count) {
    Coram* path = (Coram*) ORAMFactory::create_Path("CORAM", m, b, B, c, stash, count);
    outType& out = path->c_acc(b);
    delete path;
    return out;
}

inline outType& CORAM_slow(uint32_t noAcc, bool values, uint64_t m, uint64_t b, uint16_t B, uint16_t c, uint16_t stash, uint16_t count) {
    Coram* oram = (Coram*) ORAMFactory::create_Path("CORAM", m, b, B, c, stash, count);
    outType& out = addWR(oram->c_init(values), multiplyWR(noAcc, oram->c_acc(b)));
    delete oram;
    return out;
}

inline outType& CORAM_slow_amortized(uint32_t noAcc, bool values, uint64_t m, uint64_t b, uint16_t B, uint16_t c, uint16_t stash, uint16_t count) {
    Coram* oram = (Coram*) ORAMFactory::create_Path("CORAM", m, b, B, c, stash, count);
    outType& out = oram->c_amortized(noAcc, values);
    delete oram;
    return out;
}

inline outType& acc_CORAM_fast(uint32_t noAcc, bool dummy, uint64_t m, uint64_t b, uint16_t B, uint16_t c, uint16_t stash, uint16_t count) {
    return c_acc_CORAM(m, b, B, count, stash, c);
}

inline outType& CORAM_fast(uint32_t noAcc, bool values, uint64_t m, uint64_t b, uint16_t B, uint16_t c, uint16_t stash, uint16_t count) {
    return c_CORAM(noAcc, values, m, b, B, count, stash, c);
}

inline outType& acc_OSQR_slow(uint32_t noAcc, bool dummy, uint64_t m, uint64_t b, uint16_t c, uint16_t count) {
    OSquareRoot* oram = ORAMFactory::create_OSQR(m, b, c, count);
    outType& out = oram->c_acc(noAcc, b);
    delete oram;
    return out;
}

inline outType& OSQR_slow(uint32_t noAcc, bool values, uint64_t m, uint64_t b, uint16_t c, uint16_t count) {
    OSquareRoot* oram = ORAMFactory::create_OSQR(m, b, c, count);
    outType& out = addWR(oram->c_init(values), multiplyWR(noAcc, oram->c_acc(b)));
    delete oram;
    return out;
}

inline outType& OSQR_slow_amortized(uint32_t noAcc, bool values, uint64_t m, uint64_t b, uint16_t c, uint16_t count) {
    OSquareRoot* oram = ORAMFactory::create_OSQR(m, b, c, count);
    outType& out = oram->c_amortized(noAcc, values);
    delete oram;
    return out;
}

inline outType& acc_OSQR_fast(uint32_t noAcc, bool dummy, uint64_t m, uint64_t b, uint16_t c, uint16_t count) {
    return c_acc_SQRORAM(noAcc, m, b, c, count);
}

inline outType& OSQR_fast(uint32_t noAcc, bool values, uint64_t m, uint64_t b, uint16_t c, uint16_t count) {
    return c_SQRORAM(noAcc, values, m, b, count, c);
}

inline outType& Mixed_ORAM_slow(uint32_t noAcc, bool values, uint64_t m, uint64_t b, uint16_t B, uint16_t c, uint16_t stash, uint16_t count) {
    MixedORAM* oram = ORAMFactory::create_Mixed_ORAM(noAcc, values, m, b, B, c, stash, count);
    outType& out = oram->c_amortized(noAcc, values);
    delete oram;
    return out;
}

#endif //ORAMEVALUATOR_ACCESS_FUNCTIONS_H
