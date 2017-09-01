//
// Created by weber on 03.07.2017.
//

#ifndef ORAMEVALUATOR_TREE_FACTORY_H
#define ORAMEVALUATOR_TREE_FACTORY_H

#include "binary_tree_oram.h"
#include "path_oram.h"
#include "optimized_square_root.h"
#include "../paper_benchmarks.h"
#include <cassert>

class ORAMFactory {
public:
    static inline BinaryTree* create_BT(uint64_t m, uint64_t b, uint16_t B, uint16_t c, uint16_t counter, bool dynamicBuckets) {
        BinaryTree* ret = new BinaryTree(m, b, B, c, dynamicBuckets);
        ret->build(counter);
        return ret;
    }

    static inline void acc_BT(uint16_t d, uint64_t b, uint16_t B, uint16_t c, uint16_t counter, bool dynamicBuckets) {
        BinaryTree* oram = create_BT((uint64_t) pow(2, d), b, B, c, counter, dynamicBuckets);
        outType& out = oram->c_acc(b);
        std::cout << "BT: d = " << d << " b = " << b << " B = " << B << " c = " << c << ": " << out << std::endl;
        delete &out;
        delete oram;
    }

    static inline BinaryTree* create_BT(uint64_t m, uint64_t b, uint16_t B, uint16_t c, uint16_t counter) {
        BinaryTree* ret = new BinaryTree(m, b, B, c);
        ret->build(counter);
        return ret;
    }

    static inline BinaryTreeGKK* create_GKK_BT(uint64_t m, uint64_t b, uint16_t c) {
        BinaryTreeGKK* ret = new BinaryTreeGKK(m, b, c);
        ret->build();
        return ret;
    }

    static inline void acc_GKK_BT(uint16_t d, uint64_t b, uint16_t c) {
        BinaryTreeGKK* oram = create_GKK_BT((uint64_t) pow(2, d), b, c);
        outType& out = oram->c_acc(b);
        std::cout << "GKK-BT: d = " << d << " b = " << b << " c = " << c << ": " << out << std::endl;
        delete &out;
        delete oram;
    }

    static inline Path* create_Path(std::string type, uint64_t m, uint64_t b, uint16_t B, uint16_t c, uint16_t stash, uint16_t counter) {
        Path* ret;

        if(type.compare("Path") == 0)
            ret = new Path(m, b, B, c, stash);

        else if(type.compare("PathSC") == 0)
            ret = new PathSC(m, b, B, c, stash);

        else if(type.compare("Scoram") == 0)
            ret = new Scoram(m, b, B, c, stash, 4);

        else if(type.compare("SQR_CORAM") == 0)
            ret = new SQR_CORAM(m, b, B, c, stash);

        else {
            assert(type.compare("CORAM") == 0);
            ret = new Coram(m, b, B, c, stash);
        }
        ret->build(counter);
        return ret;
    }

    static inline void acc_Path(std::string type, uint16_t d, uint64_t b, uint16_t B, uint16_t c, uint16_t stash, uint16_t counter) {
        Path* oram = create_Path(type, (uint64_t) pow(2, d), b, B, c, stash, counter);
        outType& out = oram->c_acc(b);
        std::cout << type << ": d = " << d << " b = " << b << " B = " << B << " c = " << c << " stash = " << stash << ": " << out << std::endl;
        delete &out;
        delete oram;
    }

    static inline OSquareRoot* create_OSQR(uint64_t m, uint64_t b, uint16_t c, uint16_t counter) {
        OSquareRoot* ret = new OSquareRoot(m, b, c);
        ret->build(counter);
        return ret;
    }

    static inline void acc_OSQR(uint16_t d, uint64_t b, uint16_t c, uint16_t counter) {
        OSquareRoot* oram = create_OSQR((uint64_t) pow(2, d), b, c, counter);
        outType& out = oram->c_acc(b);
        std::cout << "Optimized Square-Root ORAM: d = " << d << " b = " << b << " c = " << c << ": " << out << std::endl;
        delete &out;
        delete oram;
    }

    static inline void acc_OSQR(uint32_t noAcc, uint16_t d, uint64_t b, uint16_t c, uint16_t counter) {
        OSquareRoot* oram = create_OSQR((uint64_t) pow(2, d), b, c, counter);
        outType& out = oram->c_acc(noAcc, b);
        std::cout << "Optimized Square-Root ORAM: d = " << d << " b = " << b << " c = " << c << ": " << out << std::endl;
        delete &out;
        delete oram;
    }

    static inline void acc_LSO_old(uint32_t noAcc, uint16_t d, uint64_t b) {
        outType& out = noAcc*TrivialLinearScan::c_write_old((uint64_t) pow(2, d), b);
        std::cout << "Linear Scan ORAM (old): " << out << std::endl;
        delete &out;
    }

    static inline MixedORAM* create_Mixed_ORAM(uint32_t noAcc, bool values, uint64_t m, uint64_t b, uint16_t B, uint16_t c, uint16_t stash, uint16_t counter) {
        MixedORAM* ret = new MixedORAM(noAcc, values, m, b, B, c, stash);
        ret->build();
        return ret;
    }

    static inline void acc_Mixed_ORAM(uint32_t noAcc, bool values, uint16_t d, uint64_t b, uint16_t B, uint16_t c, uint16_t stash, uint16_t counter) {
        MixedORAM* oram = create_Mixed_ORAM(noAcc, values, (uint64_t) pow(2, d), b, B, c, stash, counter);
        outType& out = oram->c_acc(b);
        std::cout << "Mixed-ORAM: d = " << d << " b = " << b << " B = " << B << " c = " << c << " stash = " << stash << " noAcc: " << noAcc << ": " << out << std::endl;
        delete &out;
        delete oram;
    }
};

#endif //ORAMEVALUATOR_TREE_FACTORY_H
