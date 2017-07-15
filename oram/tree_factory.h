//
// Created by weber on 03.07.2017.
//

#ifndef ORAMEVALUATOR_TREE_FACTORY_H
#define ORAMEVALUATOR_TREE_FACTORY_H

#include "binary_tree_oram.h"
#include "path_oram.h"
#include "assert.h"

class TreeFactory {
public:
    static inline BinaryTree* create_BT(uint64_t m, uint64_t b, uint16_t B, uint16_t c, uint16_t counter, bool dynamicBuckets) {
        BinaryTree* ret = new BinaryTree(m, b, B, c, dynamicBuckets);
        ret->build(counter);
        return ret;
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
};

#endif //ORAMEVALUATOR_TREE_FACTORY_H
