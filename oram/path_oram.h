//
// Created by weber on 30.06.2017.
//

#ifndef ORAMEVALUATOR_PATH_ORAM_H
#define ORAMEVALUATOR_PATH_ORAM_H

#include "binary_tree_oram.h"
#include "../helper.h"
#include "assert.h"

class Path : public TreeInterface {
protected:
    uint16_t s;
    LinearScanOram* stash;
public:
    Path(uint64_t m, uint64_t b, uint16_t B, uint16_t c, uint16_t s, std::string type) :
            TreeInterface(m, b, (uint16_t) (myLog2(m)-1), b+2* myLog2(m), B, c, type), s(s) {}

    Path(uint64_t m, uint64_t b, uint16_t B, uint16_t c, uint16_t s) :
            TreeInterface(m, b, (uint16_t) (myLog2(m)-1), b+2* myLog2(m), B, c, "Path ORAM"), s(s) {}
    virtual ~Path() {
        delete stash;
    }
    void build(uint16_t counter);
    void build();
    virtual Path* createMap(uint64_t newM);
    LinearScanOram* createBuckets();
    uint64_t c_LCA(uint64_t b);
    uint64_t c_RAR(uint64_t b);
    uint64_t c_acc(uint64_t b);
    uint64_t c_init(bool values);
    virtual uint64_t c_addAndEvict();
};

class PathSC : public Path {
public:
    PathSC(uint64_t m, uint64_t b, uint16_t B, uint16_t c, uint16_t s) : Path(m, b, B, c, s, "PathSC ORAM") {
        // B has to be power of two otherwise c_offset would need to include multiplier
        assert(isPowerOfTwo(B));
    }
    ~PathSC() { }
    PathSC* createMap(uint64_t newM);
    uint64_t c_addAndEvict();
    uint64_t c_dud(uint64_t m, uint64_t b);
    uint64_t c_offset(uint64_t m, uint64_t b);
    uint64_t sort1();
    uint64_t sort23();
};

class Scoram : public Path {
public:
    Scoram(uint64_t m, uint64_t b, uint16_t B, uint16_t c, uint16_t s) : Path(m, b, B, c, s, "SCORAM") {}
    ~Scoram() { }
    Scoram* createMap(uint64_t newM);
    uint64_t c_addAndEvict();
    uint64_t c_cPut();
    uint64_t c_minLCA(uint64_t m, uint64_t b);
    uint64_t c_RDP();
    uint64_t c_GPP();
};

class Coram : public Path {
public:
    Coram(uint64_t m, uint64_t b, uint16_t B, uint16_t c, uint16_t s) : Path(m, b, B, c, s, "Circuit ORAM") {}
    ~Coram() { }
    Coram* createMap(uint64_t newM);
    uint64_t c_minLCA(uint64_t m, uint64_t b);
    uint64_t c_PDStash();
    uint64_t c_PDStage();
    uint64_t c_PD();
    uint64_t c_PT();
    uint64_t c_evictOnceW();
    uint64_t c_evictOnce();
    uint64_t c_addAndEvict();
};

#endif //ORAMEVALUATOR_PATH_ORAM_H
