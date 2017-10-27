//
// Created by weber on 26.10.2017.
//

#include "testOram.h"
void testOram::build(uint16_t counter) {
    TreeInterface::build(counter);
    stash = new LinearScanOram(s, b+d+1, d, false);
}

/**
 * helper operation for constructing new Circuit ORAM map
 * @param newM: number of elements to store in map
 * @return new constructed Circuit ORAM map
 */
ORAM* testOram::createMap(uint64_t newM) {
    return new testOram(newM, c*d, B, c, s);
}

/**
 * ReadAndRemove: reads and removes element with the given id from the ORAM
 * @param b: number of bit to read during RAR (always includes payload and isDummy)
 * @return costs for RAR using b-bit
 */
outType& testOram::c_RAR(uint64_t b) {
    // LUMU(m, b)+rand(d)+dPath+d*(RAR_BO(B, b+1, d) - Y2B(B, b+d+1))
    return TreeInterface::c_RAR(b) - c_yaoShare(1, bb) + stash->c_RAR(b);
}

/**
 * Optimized Minimum LCA: determine block that can be placed nearest to leaves and its destination level
 * @param m: number of elements for which LCA has to be determined
 * @param b: bit width of leaf indexes
 * @return costs for determining Minimum LCA
 */
outType& testOram::c_minLCA(uint64_t m, uint64_t b) {
    // b*AND + log(b)*(compEq(b) + mux(b+log(b))) + add(log(b))
    outType& lca = 2*c_adder(b) + b*c_lin_gate() + myLog2(b)*(c_comp_eq(b) + c_mux(b+myLog2(b)) + c_adder(myLog2(b)));

    // m*(LCA(b) + AND + compMag(log(b)) + mux(b+log(b)))
    return m*(lca + 1*c_lin_gate() + c_comp_mag(myLog2(b)) + c_mux(b+myLog2(b)));
}

/**
 * Prepare Deepest Array - Stash: scan stash to find element that can be pushed deepest
 * @return costs for preparing deepest array for stash elements
 */
outType& testOram::c_PDStash() {
    // (s-1)*OR + minLCA(s, d) + mux(2*log(d))
    return (s-1)*c_lin_gate()+c_minLCA(s, d)+c_mux((uint16_t) 2*myLog2(d));
}

/**
 * Prepare Deepest Array - Stage: scan bucket to find element that can be pushed deepest
 * @return costs for preparing deepest array for bucket elements
 */
outType& testOram::c_PDStage() {
    // minLCA(B, d) + mux(log(d)) +
    return c_minLCA(B, d) + c_mux(myLog2(d))+2*c_comp_mag(myLog2(d))+c_mux((uint16_t) 2*myLog2(d));
}

/**
 * Prepare Deepest Array
 * @return costs for preparation of deepest array
 */
outType& testOram::c_PD() {
    // PDStash(d) + d*PDStage(d)
    return c_PDStash()+d*c_PDStage();
}

/**
 * Prepare Target Array
 * @return costs for preparation of target array
 */
outType& testOram::c_PT() {
    // d*(4*compEq(log(d)) + 5*mux(log(d)) + 2*AND + B*OR)
    return d*(4*c_comp_eq(myLog2(d))+5*c_mux(myLog2(d))+(2+B)*c_lin_gate());
}

/**
 * eviction on single block
 * @return costs for eviction on single block
 */
outType& testOram::c_evictOnceW() {
    // AND + compEq(log(d)) + mux(bb + log(d)) + Add_LSO(B, bb) + mux(1)
    return 1*c_lin_gate() + c_comp_eq(myLog2(d)) + c_mux(bb+myLog2(d)) + buckets->c_add() + c_mux(1);
}

/**
 * eviction of one block
 * @return
 */
outType& testOram::c_evictOnce() {
    // d*(evictOnceW + compEq(log(d)) + RAR_LSO(B, b+d+1, d) + mux(bb+log(d)))
    return d*(c_evictOnceW() + c_comp_eq(myLog2(d)) + buckets->c_RAR(b+d) + c_mux(bb+myLog2(d)));
}

/**
 * eviction of Circuit ORAM, evicts two single paths
 * @return costs for eviction of Circuit ORAM
 */
outType& testOram::c_addAndEvict() {
    return 2*(c_Y2B(B*d+s, bb) + c_PD() + c_PT() + c_evictOnce() + c_B2Y(B*d+s, bb));
}


/**
 * helper operation for constructing new Linear Scan ORAM buckets
 * @return new constructed bucket ORAMs
 */
LinearScanOram* testOram::createBuckets() {
    return new LinearScanOram(B, b+d+1, d, false);
}

/**
 * initialization of path-based ORAM (additional initialization of stash)
 * @param values: true if the values are in place during initialization
 * @return costs for initialization of path-based ORAM
 */
outType& testOram::c_init(bool values) {
    return TreeInterface::c_init(values) + stash->c_init(false);
}

/**
 * reads b-bit from one of the elements in the array
 * @param b: number of bit to read, only payload here
 * @return costs for accessing b-bit
 */
outType& testOram::c_acc(uint64_t b) {
    //RAR(m, b)+a&e(m, b)
    return c_RAR(b) + c_addAndEvict();
}