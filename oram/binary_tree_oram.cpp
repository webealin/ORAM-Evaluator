//
// Created by weber on 30.06.2017.
//
#include "binary_tree_oram.h"

/**
 * construction of the structure using recursive trees until depth would be smaller 12 (GKK)
 */
void TreeInterface::build() {
    auto newM = (uint64_t) ceil((double) m / c);

    if(d >= 12) {
        map = createMap(newM);
        ((TreeInterface*) map)->build();
    }
    else map = createLSMap(newM);

    buckets = createBuckets();
}

/**
 * construction of the structure using counter steps before utilizing Linear Scan ORAM
 * @param counter
 */
void TreeInterface::build(uint16_t counter) {
    auto newM = (uint64_t) ceil((double) m / c);

    if(recursionCond(counter)) {
        map = createMap(newM);
        ((TreeInterface*) map)->build((uint16_t) (counter-1));
    }
    else map = createLSMap(newM);

    buckets = createBuckets();
}

/**
 * initialization of tree-based ORAM
 * @param values: true if the values are in place during initialization
 * @return costs for initialization of tree-based ORAM
 */
outType& TreeInterface::c_init(bool values) {
    // (2^(d+1)-1)*initBO(B, bb)+m*rand(d)+initMap(m/c, b)
    outType& out = ((uint64_t) pow(2, d+1)-1) * buckets->c_init(false) + map->c_init(false);

    if(values) {
        //outType& acc = c_acc(b);
        //multiplyRounds(acc, m);
        //addRounds(out, acc.rounds);
        //return out + m * acc;
        return out + m*c_acc(b);
    }
    return out;
}

/**
 * eviction phase of tree-based ORAMs
 * @return costs for eviction phase
 */
outType& TreeInterface::c_evict() {
    // sum(2 to d-1: 2*rand(i))+(2*d-3)(pop_BO(B, bb)+dChild(bb)+2*add_BO(B, bb))
    return (2*d-3)*(buckets->c_pop()+c_condSwap(bb)+2*buckets->c_add());        // TODO summe (wegen dem rand)
}

outType& c_dIdx(uint16_t b) {
    auto* out = new outType;
    *out = {0, b, 1};
    return *out;
}

/**
 * lookup and map update for tree-based ORAM
 * @return costs for lookup and map update
 */
outType& TreeInterface::c_LUMU() {
    //dIdx(myLog2(m))+dBIdx+Read_LS(c, log(m))+Write_LS(c, log(m))+acc_Map(m/c, b)
    //outType& acc = map->c_acc(c*d);
    //outType& out = c_dIdx(d) + TrivialLinearScan::read(c, d) + TrivialLinearScan::write(c, d) + acc;
    //addRounds(out, 1 + acc.rounds);
    //return out;
    return /*c_dIdx(d) +*/ TrivialLinearScan::c_read(c, d) + TrivialLinearScan::c_write(c, d) + map->c_acc(c*d);
}

/**
 * lookup and map update for GKK binary-tree ORAM
 * @return costs for lookup and map update
 */
outType& BinaryTreeGKK::c_LUMU() {
    //dIdx(myLog2(m))+dBIdx+Read_LS(c, log(m))+Write_LS(c, log(m))+acc_Map(m/c, b)
    return TrivialLinearScan::c_read(c, d) + TrivialLinearScan::c_write(c, d) + map->c_acc(b);
}

outType& c_dPath(uint16_t b) {
    auto* out = new outType;
    *out = {0, b, 1};
    return *out;
}

/**
 * ReadAndRemove: reads and removes element with the given id from the ORAM
 * @param b: number of bit to read during RAR (always includes payload and isDummy)
 * @return costs for RAR using b-bit
 */
outType& TreeInterface::c_RAR(uint64_t b) {
    //LUMU(m, b)+rand(d)+dPath+d*RAR_BO(B, b+1, d)
    //outType& out = c_LUMU()+c_rand(d)+c_dPath(d)+d*buckets->c_RAR(b+1);
    //addRounds(out, 3);  // d Path and share conversions of RAR
    //return out;
    return c_LUMU()+c_rand(d)/*+c_dPath(d)*/+d*buckets->c_RAR(b+1);
}

/**
 * ReadAndRemove: reads and removes element with the given id from the GKK-ORAM
 * @param b: number of bit to read during RAR, only payload here
 * @return costs for RAR using b-bit
 */
outType& BinaryTreeGKK::c_RAR(uint64_t b) {
    //LUMU(m, b)+rand(d)+dPath+d*RAR_BO(B, b+d, d)
    return c_LUMU()+c_rand(d)+d*buckets->c_RAR(b+d);
}

/**
 * Add: writes whole bb bit element to the first empty slot
 * @return costs for adding one element
 */
outType& TreeInterface::c_add() {
    //add_BO(B, bb)+evict(m, b)
    return buckets->c_add()-c_B2Y(B, bb)+c_evict();
}

/**
 * reads b-bit from one of the elements in the array
 * @param b: number of bit to read, only payload here
 * @return costs for accessing b-bit
 */
outType& TreeInterface::c_acc(uint64_t b) {
    //RAR(m, b)+add(m, b)
    return c_RAR(b)+c_add();
}

/**
 * amortized costs per access on tree-based ORAM
 * @param noAcc: number of accesses on the ORAM
 * @param values: true if the values are in place during initialization
 * @return amortized costs per access
 */
outType& TreeInterface::c_amortized(uint16_t noAcc, bool values) {
    // init(m, b) / a + acc(m, b)
    return c_init(values)/noAcc + c_acc(b);
}