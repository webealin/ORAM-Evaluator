//
// Created by weber on 30.06.2017.
//

#ifndef ORAMEVALUATOR_BINARY_TREE_ORAM_H
#define ORAMEVALUATOR_BINARY_TREE_ORAM_H

#include "oram.h"

#include <utility>
#include "linear_scan_oram.h"
#include "map_based_oram.h"

/**
 * interface for all tree-based constructions
 */
class TreeInterface : public MapBasedORAM, public extendedORAM {
protected:
    uint16_t B;                   // size of buckets
    uint16_t d;                   // depth of tree
    LinearScanOram* buckets{};    // placeholder ORAM for buckets
public:
    TreeInterface(uint16_t d, uint16_t B, uint16_t c) : extendedORAM(), MapBasedORAM(c), B(B), d(d) { }
    void build(uint16_t counter) override;

    ~TreeInterface() override {
        delete buckets;
    }

    inline virtual LinearScanOram* createBuckets() {
        return new LinearScanOram(B, b+d+1, d, false);   // note that LSO sets bb to b+d (adds vid for itself)!
    }

    outType& c_init(bool values) override;
    outType& c_acc(uint64_t b) override;
    outType& c_amortized(uint32_t noAcc, bool values) override;

    outType& c_RAR(uint64_t b) override;
    outType& c_add() override;

    outType& c_evict();
    virtual outType& c_LUMU();
};

/**
 * binary-tree construction as implemented in CORAM paper
 */
class BinaryTree : public TreeInterface {
private:
    bool dynamicBuckets;        // use dynamic bucket sizes in maps
public:
    BinaryTree(uint64_t m, uint64_t b, uint16_t B, uint16_t c, bool dBuckets) :
            ORAM(m, b, b+2*myLog2(m)+1, "Binary Tree ORAM"), TreeInterface(myLog2(m), B, c), dynamicBuckets(dBuckets) { }
    BinaryTree(uint64_t m, uint64_t b, uint16_t B, uint16_t c) : ORAM(m, b, b+2*myLog2(m)+1, "Binary Tree ORAM"),
                                                                 TreeInterface(myLog2(m), B, c), dynamicBuckets(false) { }

    inline bool recursionCond(uint16_t counter) override {
        return counter > 0 && m > c;
    }

    inline BinaryTree* createMap(uint64_t newM) override {
        if(dynamicBuckets)
            return new BinaryTree(newM, c*d, (uint16_t) 2*myLog2(m), c, dynamicBuckets);
        return new BinaryTree(newM, c*d, B, c, dynamicBuckets);
    }
};

/**
 * binary-tree construction as implemented in GKK paper
 */
class BinaryTreeGKK : public TreeInterface {
public:
    BinaryTreeGKK(uint64_t m, uint64_t b, uint16_t c) : ORAM(m, b, b+2*myLog2(m), "GKK Binary Tree ORAM"),
            TreeInterface(myLog2(m), (uint16_t) 2*myLog2(m), c) { }

    outType& c_RAR(uint64_t b) override;
    outType& c_LUMU() override;
    inline void build() {
        TreeInterface::build(UINT16_MAX);
    }

    inline bool recursionCond(uint16_t counter) override {
        return d >= 12;
    }

    inline BinaryTreeGKK* createMap(uint64_t newM) override {
        return new BinaryTreeGKK(newM, b, c);
    }

    inline TrivialLinearScan* createLSMap(uint64_t newM) override {
        return new TrivialLinearScan(newM, b);
    }

    inline LinearScanOram* createBuckets() override {
        return new LinearScanOram(B, b+d, d, true);   // note that LSO sets bb to b+d (adds vid for itself)!
    }
};

#endif //ORAMEVALUATOR_BINARY_TREE_ORAM_H