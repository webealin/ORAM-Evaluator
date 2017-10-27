//
// Created by weber on 30.06.2017.
//

#include "path_oram.h"
#include "../evaluation/evaluator.h"

void Path::build(uint16_t counter) {
    TreeInterface::build(counter);
    stash = new LinearScan(s, b+d+1, (uint16_t)(d+1), false);
}


/**
 * helper operation for constructing new Linear Scan ORAM buckets
 * @return new constructed bucket ORAMs
 */
LinearScanOram* Path::createBuckets() {
    return new LinearScanOram(B, b+d+1, (uint16_t)(d+1), false);
}

/**
 * initialization of path-based ORAM (additional initialization of stash)
 * @param values: true if the values are in place during initialization
 * @return costs for initialization of path-based ORAM
 */
outType& Path::c_init(bool values) {
    return TreeInterface::c_init(values) + stash->c_init(false);
}

/**
 * helper operation for constructing new Path ORAM map
 * @param newM: number of elements to store in map
 * @return new constructed Path ORAM map
 */
ORAM* Path::createMap(uint64_t newM) {
    return new Path(newM, c*d, B, c, s);
}

/**
 * Lowest Common Ancestor: determine level of bottommost bucket a block can be placed
 * @param b: bit width of leaf indexes
 * @return costs for determining LCA
 */
outType& Path::c_LCA(uint64_t b) {
    // LeadingZeroCount(b)
    return c_LZC(b);
}

/**
 * reads b-bit from one of the elements in the array
 * @param b: number of bit to read, only payload here
 * @return costs for accessing b-bit
 */
outType& Path::c_acc(uint64_t b) {
    //RAR(m, b)+a&e(m, b)
    return c_RAR(b) + c_addAndEvict();
}

/**
 * ReadAndRemove: reads and removes element with the given id from the ORAM
 * @param b: number of bit to read during RAR (always includes payload and isDummy)
 * @return costs for RAR using b-bit
 */
outType& Path::c_RAR(uint64_t b) {
    // LUMU(m, b)+rand(d)+dPath+d*(RAR_BO(B, b+1, d) - Y2B(B, b+d+1))
    return TreeInterface::c_RAR(b) - d*(c_Y2B(B, b+d+1)) - c_yaoShare(1, bb) + stash->c_RAR(b);
}

/**
 * Eviction of naive Path ORAM
 * @return costs for eviction
 */
outType& Path::c_addAndEvict() {
    // B2Y(s, bb) + B2Y(Bd, d) + (Bd+s)*(LCA(d) + MuxChain(Bd+s, bb) + d(3B + compMag(log2(d))) + Y2B(Bd + s, bb)
    outType& gates = (B*d+s)*(c_LCA(d)+c_mux_chain(B*d+s, bb)+d*(B*3*c_lin_gate() + c_comp_mag(myLog2(d)))-2*c_lin_gate());
    return c_B2Y(s, bb) + c_B2Y(B*d, d) + gates + c_Y2B(B*d+s, bb);
}

                        /***************************************/
                        /**********      PATH-SC      **********/
                        /***************************************/

/**
* helper operation for constructing new PathSC ORAM map
* @param newM: number of elements to store in map
* @return new constructed PathSC ORAM map
*/
PathSC* PathSC::createMap(uint64_t newM) {
    return new PathSC(newM, c*d, B, c, s);
}

/**
 * determine unnecessary dummy elements in a sorted array
 * @param m: number of elements to compare
 * @param b: bit width per element
 * @return costs for determining unnecessary dummies
 */
outType& PathSC::c_dud(uint64_t m, uint64_t b){
    // dDup(m, b) + (m-1)*mux(b)
    return c_dDup(m, b)+(m-1)*c_mux(b);
}

/**
 * determining the offset
 * @param m: number of elements for which offset should be calculated
 * @param b: number of bits to represent offset
 * @return costs for determining the offset
 */
outType& PathSC::c_offset(uint64_t m, uint64_t b) {
    // m*(compMag(b)+2*adder(b)+mux(b))
    return m*(c_comp_mag(b) + 2*c_adder(b)+c_mux(b));
}

/**
 * sort all elements of current stash according to level returned by LCA operation
 * @return costs for sorting by log(d) bit level
 */
outType& PathSC::sort1() {
    // (Bd+s)*LCA(d)+sort(Bd+s, bb, log(d))
    return (B*d+s)*c_LCA(d)+c_sort(B*d+s, bb, myLog2(d));
}

/**
 * determine offset and sort all elements of current stash plus new dummies two times according to offset
 * @return costs for sorting by log(d) bit offset
 */
outType& PathSC::sort23() {
    // offset(Bd+s, log(Bd)) + DUD(2Bd+s, log(Bd)) + 2*sort(2Bd+s, bb, log(Bd))
    return c_offset(B*d+s, myLog2(B*d)) + c_dud((uint64_t) 2*B*d+s, myLog2(B*d)) + 2*c_sort((uint64_t) 2*B*d+s, bb, myLog2(B*d));
}

/**
 * eviction of Path SC ORAM
 * @return costs for eviction of Path SC ORAM
 */
outType& PathSC::c_addAndEvict() {
    // B2Y(s, bb) + B2Y(Bd, d) + sort1(d, s, bb) + sort2(d, s, bb) + Y2B(Bd+s, bb)
    return sort1() + sort23();
}

                        /**************************************/
                        /**********      SCORAM      **********/
                        /**************************************/

/**
 * helper operation for constructing new Path ORAM map
 * @param newM: number of elements to store in map
 * @return new constructed Path ORAM map
 */
Scoram* Scoram::createMap(uint64_t newM) {
    return new Scoram(newM, c*d, B, c, s, alpha);
}

/**
 * reads b-bit from one of the elements in the array
 * @param b: number of bit to read, only payload here
 * @return costs for accessing b-bit
 */
outType& Scoram::c_acc(uint64_t b) {
    //RAR(m, b)+a&e(m, b)
    return addWR(c_RAR(b), c_addAndEvict());
}

/**
 * ReadAndRemove: reads and removes element with the given id from the ORAM
 * @param b: number of bit to read during RAR (always includes payload and isDummy)
 * @return costs for RAR using b-bit
 */
outType& Scoram::c_RAR(uint64_t b) {
    // LUMU(m, b)+rand(d)+dPath+d*(RAR_BO(B, b+1, d) - Y2B(B, b+d+1))
    return Path::c_RAR(b) + d*(c_Y2B(B, b+d+1));
}

/**
 * determine if block should be dropped now
 * @return costs for determining if block should be dropped
 */
outType& Scoram::c_cPut() {
    // 2*AND + OR
    return 3*c_lin_gate();
}

/**
 * helper operation for condition of conditional ReadAndRemove operation
 * @return costs for condition of cRAR operation
 */
outType& c_condRAR() {
    // AND
    return c_lin_gate();
}

/**
 * helper operation for condition of conditional Add operation
 * @return costs for condition of cAdd operation
 */
outType& c_condAdd() {
    // AND + OR
    return 2*c_lin_gate();
}

/**
 * Minimum Lowest Common Ancestor: determine block that can be placed nearest to leaves and its destination level
 * @param m: number of elements for which LCA has to be determined
 * @param b: bit width of leaf indexes
 * @return costs for determining Minimum LCA
 */
outType& Scoram::c_minLCA(uint64_t m, uint64_t b) {
    // m(b-1)*OR + (m-1)*(compMag(b+1) + AND + mux(2b+3)
    return m*(b-1)*c_lin_gate() + (m-1)*(c_comp_mag(b+1) + 1*c_lin_gate() + c_mux(2*b+3));
}

/**
 * Reverse Dropping Pass: push block from stash that can be placed deepest in bottommost possible bucket
 * @return costs for Reverse Dropping Pass
 */
outType& Scoram::c_RDP() {
    // return minLCA(s, d) + RAR_LSO(s, b+d+1, d+1) + B*d*(cPut + mux(bb)) + Add_LSO(s, bb, d+1);
    return c_minLCA(s, d) + stash->c_RAR(b+d+1)+B*d*(c_cPut()+c_mux(bb)) + stash->c_cAdd(c_condAdd);
}

/**
 * Greedy Push Pass: push block on path that can be placed deepest in bottommost possible bucket
 * @return costs for Greedy Push Pass
 */
outType& Scoram::c_GPP() {
    // d * (minLCA(B, d) + compMag(d+1) + (B-1)*OR + cRAR_LSO(B, b+d+1, d+1) + cAdd_LSO(B, bb, d+1));
    return d*(c_minLCA(B, d) + c_comp_mag(d+1) + (B-1)*c_lin_gate() + buckets->c_cRAR(b+d+1, c_condRAR) + buckets->c_cAdd(c_condAdd));
}

/**
 * eviction of SCORAM, uses flush operation alpha times
 * @return costs for eviction of SCORAM
 */
outType& Scoram::c_addAndEvict() {
    // alpha*(rand(d) + dPath + B2Y(Bd, bb) + RDP(m, b) + GPP(m, b) + Y2B(Bd, bb))
    return multiplyWR(alpha, (c_rand(d) + c_B2Y(B*d, bb) + c_RDP() + c_GPP() + c_Y2B(B*d, bb)));
}

                        /********************************************/
                        /**********      Circuit ORAM      **********/
                        /********************************************/

void Coram::build(uint16_t counter) {
    TreeInterface::build(counter);
    stash = new LinearScanOram(s, b+d+1, myLog2(m), false);
}

/**
 * helper operation for constructing new Circuit ORAM map
 * @param newM: number of elements to store in map
 * @return new constructed Circuit ORAM map
 */
ORAM* Coram::createMap(uint64_t newM) {
    return new Coram(newM, c*d, B, c, s);
}

LinearScanOram* Coram::createBuckets() {
    return new LinearScanOram(B, b+d+1, myLog2(m), false);
}

/**
 * ReadAndRemove: reads and removes element with the given id from the ORAM
 * @param b: number of bit to read during RAR (always includes payload and isDummy)
 * @return costs for RAR using b-bit
 */
outType& Coram::c_RAR(uint64_t b) {
    // LUMU(m, b)+rand(d)+dPath+d*(RAR_BO(B, b+1, d) - Y2B(B, b+d+1))
    return Path::c_RAR(b) + d*(c_Y2B(B, b+myLog2(m)));
}

/**
 * Optimized Minimum LCA: determine block that can be placed nearest to leaves and its destination level
 * @param m: number of elements for which LCA has to be determined
 * @param b: bit width of leaf indexes
 * @return costs for determining Minimum LCA
 */
outType& Coram::c_minLCA(uint64_t m, uint64_t b) {
    // b*AND + log(b)*(compEq(b) + mux(b+log(b))) + add(log(b))
    outType& lca = 2*c_adder(b) + b*c_lin_gate() + myLog2(b)*(c_comp_eq(b) + c_mux(b+myLog2(b)) + c_adder(myLog2(b)));

    // m*(LCA(b) + AND + compMag(log(b)) + mux(b+log(b)))
    return m*(lca + 1*c_lin_gate() + c_comp_mag(myLog2(b)) + c_mux(b+myLog2(b)));
}

/**
 * Prepare Deepest Array - Stash: scan stash to find element that can be pushed deepest
 * @return costs for preparing deepest array for stash elements
 */
outType& Coram::c_PDStash() {
    // (s-1)*OR + minLCA(s, d) + mux(2*log(d))
    return (s-1)*c_lin_gate()+c_minLCA(s, d)+c_mux((uint16_t) 2*myLog2(d));
}

/**
 * Prepare Deepest Array - Stage: scan bucket to find element that can be pushed deepest
 * @return costs for preparing deepest array for bucket elements
 */
outType& Coram::c_PDStage() {
    // minLCA(B, d) + mux(log(d)) +
    return c_minLCA(B, d) + c_mux(myLog2(d))+2*c_comp_mag(myLog2(d))+c_mux((uint16_t) 2*myLog2(d));
}

/**
 * Prepare Deepest Array
 * @return costs for preparation of deepest array
 */
outType& Coram::c_PD() {
    // PDStash(d) + d*PDStage(d)
    return c_PDStash()+d*c_PDStage();
}

/**
 * Prepare Target Array
 * @return costs for preparation of target array
 */
outType& Coram::c_PT() {
    // d*(4*compEq(log(d)) + 5*mux(log(d)) + 2*AND + B*OR)
    return d*(4*c_comp_eq(myLog2(d))+5*c_mux(myLog2(d))+(2+B)*c_lin_gate());
}

/**
 * eviction on single block
 * @return costs for eviction on single block
 */
outType& Coram::c_evictOnceW() {
    // AND + compEq(log(d)) + mux(bb + log(d)) + Add_LSO(B, bb) + mux(1)s
    return 1*c_lin_gate() + c_comp_eq(myLog2(d)) + c_mux(bb+myLog2(d)) + buckets->c_add() + c_mux(1);
}

/**
 * eviction of one block
 * @return
 */
outType& Coram::c_evictOnce() {
    // d*(evictOnceW + compEq(log(d)) + RAR_LSO(B, b+d, d) + mux(bb+log(d)))
    return d*(c_evictOnceW() + c_comp_eq(myLog2(d)) + buckets->c_RAR(b+myLog2(m)) + c_mux(bb+myLog2(d)));
}

/**
 * eviction of Circuit ORAM, evicts two single paths
 * @return costs for eviction of Circuit ORAM
 */
outType& Coram::c_addAndEvict() {
    return 2*(c_Y2B(B*d+s, bb) + c_PD() + c_PT() + c_evictOnce() + c_B2Y(B*d+s, bb));
}

ORAM *MixedORAM::createMap(uint64_t newM) {
    std::cout << "\nMixedORAM::createMap: m: " << m << " b: " << b << std::endl;
    std::cout << "before:" << std::endl;
    for(auto &it : seen)
        std::cout << "seen: m: " << it.first.m << " b: " << it.first.b << std::endl;

    auto* eval = new Evaluator();
    auto* thisNewMap = new params(newM, c*d);

    // check if we already evaluated this situation
    auto it = seen.find(*thisNewMap);
    if(it != seen.end())
        std::cout << "gefunden!!" << std::endl;

    // buckets and stash of same size!
    auto* coramOut = new pathSettings();
    eval->find_best_MIXO(noAcc, values, newM, c*d, B, s, *coramOut, seen);

    auto* sqrOut = new minSettings();
    eval->find_best_OSQR(noAcc, values, newM, c*d, *sqrOut, acc_OSQR_slow);

    if(needsTime(*coramOut->out) < needsTime(*sqrOut->out)) {
        seen[*thisNewMap] = coramOut;
        delete sqrOut->out;
        delete sqrOut;
        std::cout << "after - CORAM:" << std::endl;
        for(auto &it : seen)
            std::cout << "seen: m: " << it.first.m << " b: " << it.first.b << std::endl;
        return new MixedORAM(noAcc, values, newM, c * d, B, coramOut->c, s, seen);
    }
    seen[*thisNewMap] = sqrOut;
    std::cout << "after - SQR:" << std::endl;
    for(auto &it : seen)
        std::cout << "seen: m: " << it.first.m << " b: " << it.first.b << std::endl;
    delete coramOut->out;
    delete coramOut;
    return new OSquareRoot(newM, c*d, c);
}

// was will ich?
// - prüfen ab welcher Größe SQR besser als CORAM
// - also auf jedem Level prüfen SQR vs. noch einmal CORAM außen