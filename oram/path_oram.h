//
// Created by weber on 30.06.2017.
//

#ifndef ORAMEVALUATOR_PATH_ORAM_H
#define ORAMEVALUATOR_PATH_ORAM_H

#include "binary_tree_oram.h"
#include "../helper.h"
#include <cassert>
#include <utility>

class Path : public TreeInterface {
protected:
    uint16_t s;
    LinearScanOram* stash{};
public:
    Path(uint64_t m, uint64_t b, uint16_t B, uint16_t c, uint16_t s, std::string type) :
            TreeInterface(m, b, (uint16_t) (myLog2(m)-1), b+2* myLog2(m), B, c, std::move(type)), s(s) {}

    Path(uint64_t m, uint64_t b, uint16_t B, uint16_t c, uint16_t s) :
            TreeInterface(m, b, (uint16_t) (myLog2(m)-1), b+2* myLog2(m), B, c, "Path ORAM"), s(s) {}

    ~Path() override {
        delete stash;
    }
    void build(uint16_t counter) override;
    void build() override;
    virtual Path* createMap(uint64_t newM);
    virtual bool recursionCond(uint16_t counter) {
        return counter > 0 && m > 8*c;          // d = log(m)-1 and log(d) is used
    }
    LinearScanOram* createBuckets();
    outType& c_LCA(uint64_t b);
    outType& c_RAR(uint64_t b);
    outType& c_acc(uint64_t b);
    outType& c_init(bool values);
    virtual outType& c_addAndEvict();
};

class PathSC : public Path {
public:
    PathSC(uint64_t m, uint64_t b, uint16_t B, uint16_t c, uint16_t s) : Path(m, b, B, c, s, "PathSC ORAM") {
        // B has to be power of two otherwise c_offset would need to include multiplier
        assert(isPowerOfTwo(B));
    }
    ~PathSC() { }
    PathSC* createMap(uint64_t newM);
    outType& c_addAndEvict();
    outType& c_dud(uint64_t m, uint64_t b);
    outType& c_offset(uint64_t m, uint64_t b);
    outType& sort1();
    outType& sort23();
};

class Scoram : public Path {
public:
    Scoram(uint64_t m, uint64_t b, uint16_t B, uint16_t c, uint16_t s) : Path(m, b, B, c, s, "SCORAM") {}
    ~Scoram() { }
    Scoram* createMap(uint64_t newM);
    outType& c_addAndEvict();
    outType& c_cPut();
    outType& c_minLCA(uint64_t m, uint64_t b);
    outType& c_RDP();
    outType& c_GPP();
};

class Coram : public Path {
public:
    Coram(uint64_t m, uint64_t b, uint16_t B, uint16_t c, uint16_t s) : Path(m, b, B, c, s, "Circuit ORAM") {}
    ~Coram() { }
    Coram* createMap(uint64_t newM) override;
    outType& c_minLCA(uint64_t m, uint64_t b);
    outType& c_PDStash();
    outType& c_PDStage();
    outType& c_PD();
    outType& c_PT();
    outType& c_evictOnceW();
    outType& c_evictOnce();
    outType& c_addAndEvict() override;
};

#endif //ORAMEVALUATOR_PATH_ORAM_H
