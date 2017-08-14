// Linear Scan ORAM
// Created by weber on 30.06.2017.
//
#include "linear_scan_oram.h"

/**
 * initialization of Linear Scan ORAM
 * @param values: true if the values are in place during initialization
 * @return costs for initialization of LSO
 */
outType& LinearScanOram::c_init(bool values) {
    // if values, use Yao to Boolean conversion
    if(values)
        return c_Y2B(m, bb);
    // else establish Boolean sharing
    return c_booleanShare(m, bb);
}

/**
 * reads b-bit from one of the elements in the array
 * @param b: number of bit to read, excluding the virtual index (b2)
 * @return costs for accessing b-bit
 */
outType& LinearScanOram::c_acc(uint64_t b) {
    // B2Y(m, b+b2) + m(mux(b)+compEq(b2))+Y2B(m, b+b2)
    return c_B2Y(m, b+b2) + (m*(c_mux(b)+c_comp_eq(b2))) + c_Y2B(m, b+b2);
}

/**
 * ReadAndRemove: reads and removes element with the given id from the ORAM
 * @param b: number of bit to read during RAR (always includes payload and isDummy)
 * @return costs for RAR using b-bit
 */
outType& LinearScanOram::c_RAR(uint64_t b) {
    if(useOldFormula)
        // B2Y(m, b+b2) + m*(2*compEq(b2)+AND+OR+mux(b)) + Y2B(m, b+b2)
        return c_B2Y(m, b + b2) + m * (2*c_comp_eq(b2) + 2*c_lin_gate() + c_mux(b)) + c_Y2B(m, b + b2);

    // B2Y(m, b+b2) + m*(compEq(b2)+AND+OR+mux(b)) + Y2B(m, b+b2)
    return c_B2Y(m, b+b2)+m*(c_comp_eq(b2) + 2*c_lin_gate() + c_mux(b))+c_Y2B(m, b+b2);
}

/**
 * conditional ReadAndRemove: additional starting condition is used
 * @param b: number of bit to read during RAR (always includes payload and isDummy)
 * @param cond: pointer to costs for condition to use
 * @return costs for conditional RAR using b-bit and condition cond
 */
outType& LinearScanOram::c_cRAR(uint64_t b, outType& (*cond)()) {
    return c_RAR(b)+m*cond();
}

/**
 * Add: writes whole bb bit element to the first empty slot
 * @return costs for adding one element
 */
outType& LinearScanOram::c_add() {
    if(useOldFormula)
        // B2Y(m, bb) + (m-1)(AND+OR)+m*(compEq(b2)+mux(bb)) + Y2B(m, bb)
        return c_B2Y(m, bb) + (m-1)*2*c_lin_gate() + m*(c_comp_eq(b2) + c_mux(bb)) + c_Y2B(m, bb);
    // B2Y(m, bb) + (m-1)(AND+OR)+m*mux(bb) + Y2B(m, bb)
    return c_B2Y(m, bb) + (m-1)*2*c_lin_gate() + m*c_mux(bb) + c_Y2B(m, bb);
}

/**
 * Conditional Add: writes whole bb bit element to the first empty slot, if condition is true
 * @param cond: pointer to costs for condition to use
 * @return costs for conditionally adding one element
 */
outType& LinearScanOram::c_cAdd(outType& (*cond)()) {
    return c_add()+cond();
}

/**
 * Pop: removes first non-dummy element
 * @return costs for pop of one element
 */
outType& LinearScanOram::c_pop() {
    if(useOldFormula)
        // B2Y(m, bb) + (m-1)(2*OR+AND)+m*(compEq(b2)+mux(bb)) + Y2B(m, bb)
        return c_B2Y(m, bb) + (m-1)*3*c_lin_gate() + m*(c_comp_eq(b2) + c_mux(bb)) + c_Y2B(m, bb);
    // B2Y(m, bb) + (m-1)(2*OR+AND)+m*mux(bb) + Y2B(m, bb)
    return c_B2Y(m, bb) + (m-1)*3*c_lin_gate() + m*(c_mux(bb) + c_Y2B(m, bb));
}

/**
 * Conditional Pop: removes first non-dummy element, if condition is true
 * @param cond: pointer to costs for condition to use
 * @return costs for conditional pop of one element
 */
outType& LinearScanOram::c_cPop(outType& (*cond)()) {
    return c_pop()+cond();
}

/**
 * amortized costs per access on Linear Scan ORAM
 * @param noAcc: number of accesses on the ORAM
 * @param values: true if the values are in place during initialization
 * @return amortized costs per access
 */
outType& LinearScanOram::c_amortized(uint16_t noAcc, bool values) {
    return (c_init(values) / noAcc) + c_acc(b);
}

/**
 * reads b-bit from one of the elements in the array
 * @param b: number of bit to read, excluding the virtual index (b2)
 * @return costs for accessing b-bit
 */
outType& LinearScan::c_acc(uint64_t b) {
    // m(mux(b)+compEq(b2))
    //outType& out = ((uint64_t) m*(c_mux(b)+c_comp_eq(b2)));
    //return out;
    return m*(c_mux(b)+c_comp_eq(b2));
}

outType& LinearScan::c_init(bool values) {
    if(values) {
        auto* out = new outType;
        *out = {0, 0, 0};
        return *out;
    }
    return c_yaoShare(m, bb);
}

outType& LinearScan::c_amortized(uint16_t noAcc, bool values) {
    std::cout << "error: this function should not be invoked" << std::endl;
    auto* out = new outType;     // TODO
    *out = {0, 0, 0};
    return *out;
}

outType& LinearScan::c_RAR(uint64_t b) {
    std::cout << "error: this function should not be invoked" << std::endl;
    auto* out = new outType;     // TODO
    *out = {0, 0, 0};
    return *out;
}

outType& LinearScan::c_add() {
    std::cout << "error: this function should not be invoked" << std::endl;
    auto* out = new outType;     // TODO
    *out = {0, 0, 0};
    return *out;
}
