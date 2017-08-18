//
// Created by weber on 30.06.2017.
//

#ifndef ORAMEVALUATOR_BINARY_TREE_ORAM_H
#define ORAMEVALUATOR_BINARY_TREE_ORAM_H

#include "oram.h"

#include <utility>
#include "linear_scan_oram.h"

/**
 * interface for all tree-based constructions
 */
class TreeInterface : public extendedORAM {
protected:
    uint16_t B;                 // size of buckets
    uint16_t d;                 // depth of tree
    uint16_t c;                 // packing factor
    ORAM* map{};                  // ORAM that stores map
    LinearScanOram* buckets{};    // placeholder ORAM for buckets TODO: Interface für BucketORAM anlegen
public:
    TreeInterface(uint64_t m, uint64_t b, uint16_t d, uint64_t bb, uint16_t B, uint16_t c, const std::string &type) :
            extendedORAM(m, b, bb, type), c(c), B(B), d(d) { }
    virtual void build();
    virtual void build(uint16_t counter);

    ~TreeInterface() override {
        delete map;
        delete buckets;
    }
    virtual ORAM* createMap(uint64_t newM) = 0;
    virtual bool recursionCond(uint16_t counter) = 0;   // TODO: umstellen. Vllt einfach zwei Arten Tree, eine mit counter und eine ohne?

    inline virtual LinearScanOram* createBuckets() {
        return new LinearScanOram(B, b+d+1, d, false);   // note that LSO sets bb to b+d (adds vid for itself)!
    }

    inline virtual TrivialLinearScan* createLSMap(uint64_t newM) {
        return new TrivialLinearScan(newM, c*d);
    }

    outType& c_init(bool values) override;
    outType& c_acc(uint64_t b) override;
    outType& c_amortized(uint16_t noAcc, bool values) override;

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
    BinaryTree(uint64_t m, uint64_t b, uint16_t B, uint16_t c, bool dynamicBuckets) :
            TreeInterface(m, b, myLog2(m), b+2*myLog2(m)+1, B, c, "Binary Tree ORAM"), dynamicBuckets(dynamicBuckets) { }
    BinaryTree(uint64_t m, uint64_t b, uint16_t B, uint16_t c) :
            TreeInterface(m, b, myLog2(m), b+2*myLog2(m)+1, B, c, "Binary Tree ORAM"), dynamicBuckets(false) { }

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
    BinaryTreeGKK(uint64_t m, uint64_t b, uint16_t c) :
            TreeInterface(m, b, myLog2(m), b+2*myLog2(m), (uint16_t) 2*myLog2(m), c, "GKK Binary Tree ORAM") {}

    outType& c_RAR(uint64_t b) override;
    outType& c_LUMU() override;

    inline bool recursionCond(uint16_t counter) override {
        return false;           // TODO remove
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