//
// Created by weber on 03.07.2017.
//

#ifndef ORAMEVALUATOR_TREE_FACTORY_H
#define ORAMEVALUATOR_TREE_FACTORY_H

#include "binary_tree_oram.h"
#include "path_oram.h"
#include <cassert>

class TreeFactory {
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
            ret = new Scoram(m, b, B, c, stash);

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
};

#endif //ORAMEVALUATOR_TREE_FACTORY_H
