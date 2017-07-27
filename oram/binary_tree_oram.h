//
// Created by weber on 30.06.2017.
//

#ifndef ORAMEVALUATOR_BINARY_TREE_ORAM_H
#define ORAMEVALUATOR_BINARY_TREE_ORAM_H

#include "oram.h"
#include "linear_scan_oram.h"

/**
 * interface for all tree-based constructions
 */
class TreeInterface : public ORAM {
protected:
    uint16_t B;                 // size of buckets
    uint16_t d;                 // depth of tree
    uint16_t c;                 // packing factor
    ORAM* map;                  // ORAM that stores map
    LinearScanOram* buckets;    // placeholder ORAM for buckets TODO: Interface für BucketORAM anlegen
public:
    TreeInterface(uint64_t m, uint64_t b, uint16_t d, uint64_t bb, uint16_t B, uint16_t c, std::string type) :
            ORAM(m, b, bb, type), c(c), B(B), d(d) { }
    virtual void build();
    virtual void build(uint16_t counter);
    virtual ~TreeInterface() {
        delete map;
        delete buckets;
    }
    virtual ORAM* createMap(uint64_t newM) = 0;

    inline virtual LinearScanOram* createBuckets() {
        return new LinearScanOram(B, b+d+1, d, false);   // note that LSO sets bb to b+d (adds vid for itself)!
    }

    inline virtual LinearScanOram* createLSMap(uint64_t newM) {
        return new LinearScanOram(newM, c*d, myLog2(newM), false);
    }

    outType& c_evict();
    outType& c_add();
    virtual outType& c_init(bool values);
    virtual outType& c_LUMU();
    virtual outType& c_RAR(uint64_t b);
    virtual outType& c_acc(uint64_t b);
    outType& c_amortized(uint16_t noAcc, bool values);
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
    inline ~BinaryTree() { }

    inline BinaryTree* createMap(uint64_t newM) {
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
    ~BinaryTreeGKK() { }
    outType& c_RAR(uint64_t b);
    outType& c_LUMU();

    inline BinaryTreeGKK* createMap(uint64_t newM) {
        return new BinaryTreeGKK(newM, b, c);
    }

    inline LinearScanOram* createLSMap(uint64_t newM) {
        return new LinearScanOram(newM, b, myLog2(newM), true);
    }

    inline LinearScanOram* createBuckets() {
        return new LinearScanOram(B, b+d, d, true);   // note that LSO sets bb to b+d (adds vid for itself)!
    }
};

#endif //ORAMEVALUATOR_BINARY_TREE_ORAM_H