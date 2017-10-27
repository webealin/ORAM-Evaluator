//
// Created by weber on 30.06.2017.
//

#ifndef ORAMEVALUATOR_PATH_ORAM_H
#define ORAMEVALUATOR_PATH_ORAM_H

#include "binary_tree_oram.h"
#include "../helper.h"
#include "../types/minSettings.h"
#include <cassert>
#include <utility>
#include <unordered_map>

class Path : public TreeInterface {
protected:
    uint16_t s;
    LinearScan* stash{};
public:
    Path(uint64_t m, uint64_t b, uint16_t B, uint16_t c, uint16_t s) : ORAM(m, b, b+2*myLog2(m), "Path ORAM"),
                                                                       TreeInterface((uint16_t) (myLog2(m)-1), B, c),
                                                                       s(s) { }

    ~Path() override {
        delete stash;
    }
    void build(uint16_t counter) override;

    ORAM* createMap(uint64_t newM) override;

    inline bool recursionCond(uint16_t counter) override {
        return counter > 0 && m > 8*c;          // d = log(m)-1 and log(d) is used
    }
    LinearScanOram* createBuckets() override;

    outType& c_init(bool values) override;
    outType& c_acc(uint64_t b) override;
    outType& c_RAR(uint64_t b) override;

    outType& c_LCA(uint64_t b);
    virtual outType& c_addAndEvict();
};

class PathSC : public Path {
public:
    PathSC(uint64_t m, uint64_t b, uint16_t B, uint16_t c, uint16_t s) : ORAM(m, b, b+2*myLog2(m), "PathSC ORAM"),
                                                                         Path(m, b, B, c, s) {
        // B has to be power of two otherwise c_offset would need to include multiplier
        assert(isPowerOfTwo(B));
    }
    PathSC* createMap(uint64_t newM) override;
    outType& c_addAndEvict() override;

    outType& c_dud(uint64_t m, uint64_t b);
    outType& c_offset(uint64_t m, uint64_t b);
    outType& sort1();
    outType& sort23();
};

class Scoram : public Path {
protected:
    uint8_t alpha;
public:
    Scoram(uint64_t m, uint64_t b, uint16_t B, uint16_t c, uint16_t s, uint8_t alpha) : ORAM(m, b, b+2*myLog2(m), "SCORAM"),
                                                                                        Path(m, b, B, c, s), alpha(alpha) { }
    Scoram* createMap(uint64_t newM) override;

    outType& c_acc(uint64_t b) override;
    outType& c_RAR(uint64_t b) override;
    outType& c_addAndEvict() override;

    outType& c_cPut();
    outType& c_minLCA(uint64_t m, uint64_t b);
    outType& c_RDP();
    outType& c_GPP();
};

class Coram : public Path {
public:
    Coram(uint64_t m, uint64_t b, uint16_t B, uint16_t c, uint16_t s) : ORAM(m, b, b+2*myLog2(m), "Circuit ORAM"),
                                                                        Path(m, b, B, c, s) { }
    void build(uint16_t counter) override;

    ORAM* createMap(uint64_t newM) override;
    outType& c_addAndEvict() override;

    outType& c_RAR(uint64_t b) override;

    outType& c_minLCA(uint64_t m, uint64_t b);
    outType& c_PDStash();
    outType& c_PDStage();
    outType& c_PD();
    outType& c_PT();
    outType& c_evictOnceW();
    outType& c_evictOnce();
};

// only used for verification of SQR paper
class SQR_CORAM: public Coram {
public:
    SQR_CORAM(uint64_t m, uint64_t b, uint16_t B, uint16_t c, uint16_t s) : ORAM(m, b, b+2*myLog2(m), "Circuit ORAM"),
                                                                            Coram(m, b, B, c, s) {}

    inline bool recursionCond(uint16_t counter) override {
        return counter > 0 && m >= pow(2, 8);
    }
};

class MixedORAM : public Coram {
public:
    struct params {
        uint64_t m;
        uint64_t b;
        params(uint64_t m, uint64_t b) : m(m), b(b) { }
    };

    struct paramsHash {
        std::size_t operator()(const params& p) const {
            return std::hash<uint64_t>()(p.m) ^ (std::hash<uint64_t>()(p.b) << 1);
        }
    };

    struct paramsEqual {
        bool operator()(const params& lhs, const params& rhs) const {
            return lhs.m == rhs.m && lhs.b == rhs.b;
        }
    };

    typedef std::unordered_map<params, minSettings*, paramsHash, paramsEqual> evalMap;

protected:
    uint32_t noAcc;
    bool values;
    evalMap& seen;
public:
    MixedORAM(uint32_t noAcc, bool values, uint64_t m, uint64_t b, uint16_t B, uint16_t c, uint16_t s, evalMap& seen) :
            ORAM(m, b, b+2*myLog2(m), "Mixed ORAM"), Coram(m, b, B, c, s), noAcc(noAcc), values(values), seen(seen) {
        std::cout << "new Mixed ORAM m: " <<  m << " b: " << b << std::endl;
        for(auto &it : seen)
            std::cout << "seen: m: " << it.first.m << " b: " << it.first.b << std::endl;
    }

    ORAM* createMap(uint64_t newM) override;
};

#endif //ORAMEVALUATOR_PATH_ORAM_H
