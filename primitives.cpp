//
// library of building blocks that contains count of linear gates
// and number of bits to transfer [byte] / rounds of interactions
// values are calculated depending on entry in settings file
// Created by weber on 30.06.2017.
//

#include "primitives.h"

/**
 * costs for a 2-input linear gate
 * @return costs (gates, traffic, rounds) for one gate
 */
outType& c_lin_gate() {
    auto* out = new outType;
    *out = {1, 256, 0};
    return *out;
}

/**
 * conversion of Yao to Boolean sharing is for free
 * @param m number of elements to share
 * @param b bit per element to share
 * @return costs (gates, traffic, rounds) for Y2B share conversion
 */
outType& c_Y2B(uint64_t m, uint64_t b) {
    auto* out = new outType;
    *out = {0, 0, 0};
    return *out;
}

/**
 *
 * @param m
 * @param b
 * @return
 */
outType& c_B2Y(uint64_t m, uint64_t b) {
    auto* out = new outType;
    *out = {0, m*256*b, 2};
    return *out;
}

/**
 * costs for establishing a Yao Share
 * @param m number of elements to share
 * @param b bitwidth per element
 * @return costs /gates, traffic, rounds) for establishing a Yao share
 */
outType& c_yaoShare(uint64_t m, uint64_t b) {
    auto* out = new outType;
    *out = {0, 256*m*b, 2};
    return *out;
}

/**
 * costs for establishing a Boolean Share
 * @param m number of elements to share
 * @param b bitwidth per element
 * @return costs /gates, traffic, rounds) for establishing a Boolean share
 */
outType& c_booleanShare(uint64_t m, uint64_t b) {
    auto* out = new outType;
    *out = {0, m*b, 1};
    return *out;
}

/**
 * costs for a b-bit 2:1 multiplexer
 * @param b: number of bits to represent the two elements
 * @return costs (gates, traffic, rounds) for the multiplexer
 */
outType& c_mux(uint64_t b) {
    assert(b != 0);
    auto* out = new outType;
    *out = {b, 256*b, 0};
    return *out;
}

/**
 * costs for a b-bit m:1 tree multiplexer
 * @param m: number of elements to connect via the multiplexer
 * @param b: number of bits of the respective elements
 * @return costs (gates, traffic, rounds) for the multiplexer tree
 */
outType& c_mux(uint64_t m, uint64_t b) {
    assert(m != 0);
    // (m-1)*c_mux(b)
    return (m-1)*c_mux(b);
}

/**
 * costs for a b-bit m+1:1 multiplexer chain of m elements and one dummy element
 * @param m: number of elements to connect excluding the dummy
 * @param b: number of bits of the respective elements
 * @return costs (gates, traffic, rounds) for the multiplexer chain
 */
outType& c_mux_chain(uint64_t m, uint64_t b) {
    assert(m != 0);
    // m*c_mux(b)
    return m*c_mux(b);
}

/**
 * costs for adding two b-bit numbers (excluding carry out)
 * @param b: number of bits of the two numbers
 * @return costs (gates, traffic, rounds) of addition
 */
outType& c_adder(uint64_t b) {
    assert(b > 1);
    auto* out = new outType;
    *out = {(b-1), 256*(b-1), 0};
    return *out;
}

/**
 * costs for adding two b-bit numbers (including carry out)
 * @param b: number of bits of the two numbers
 * @return costs (gates, traffic, rounds) of addition
 */
outType& c_adder_carry(uint64_t b) {
    assert(b != 0);
    auto* out = new outType;
    *out = {b, 256*b, 0};
    return *out;
}

/**
 * costs for equality comparison of two b-bit numbers
 * @param b: number of bits of the two numbers
 * @return costs (gates, traffic, rounds) of equality comparison
 */
outType& c_comp_eq(uint64_t b) {
    assert(b != 0);
    // b-1 OR
    auto* out = new outType;
    *out = {(b-1), 256*(b-1), 0};
    return *out;
}

/**
 * costs for magnitude comparison of two b-bit numbers
 * @param b: number of bits of the two numbers
 * @return costs (gates, traffic, rounds) of magnitude comparison
 */
outType& c_comp_mag(uint64_t b) {
    // add carry
    return c_adder_carry(b);
}

/**
 * costs for counting leading zeros of a b-bit number
 * @param b: number of bits to represent number
 * @return costs (gates, traffic, rounds) for counting leading zeros
 */
outType& c_LZC(uint64_t b) {
    assert(b != 0);
    // (b-1)*OR + sum (i=0, log(b)): (b/2^(i+1))*c_add(i)
    uint64_t gates = (uint64_t)(floor((double) b/2))* myLog2(b)+b-1;     // TODO: Formel Unsinn. Neu machen!
    auto* out = new outType;
    *out = {gates, 256*gates, 0};
    return *out;
}

/**
 * decodes a b-bit value into one-hot-code
 * @param b: number of bits to represent value
 * @return costs (gates, traffic, rounds) for decoding
 */
outType& c_decode(uint64_t b) {
    auto powed = (uint64_t) pow(2, b);      // TODO: nötig?
    auto* out = new outType;
    *out = {2*powed, 512*powed, 0};
    return *out;
}

/**
 * costs for swapping two b-bit elements if the select input is true
 * @param b: number of bits of elements to swap
 * @return costs (gates, traffic, rounds) for conditionally swapping two b-bit values
 */
outType& c_condSwap(uint64_t b) {
    assert(b != 0);
    auto* out = new outType;
    *out = {b, 256*b, 0};
    return *out;
}

/**
 * costs for one comparison element, returns maximum and minimum element of two b-bit input elements
 * @param b: number of bits of elements
 * @param b2: number of bits to represent the tag to sort by
 * @return costs (gates, traffic, rounds) for one comparison element for b-bit elements with b2-bit tags
 */
outType& c_compEl(uint64_t b, uint64_t b2) {
    // CompMag(b2)+CondSwap(b)
    return c_comp_mag(b2) + c_condSwap(b);
}

/**
 * costs for sorting m b-bit elements by their b2 bit tag
 * @param m: number of elements to sort
 * @param b: number of bits of elements
 * @param b2: number of bits to represent the tag to sort by
 * @return costs (gates, traffic, rounds) fort sorting
 */
outType& c_sort(uint64_t m, uint64_t b, uint16_t b2) {
    // compEl(b, b2)*((1/4)*m*(log^2(m)-log(m))+m-1)
    auto rounded = (uint64_t) ceil((double) m/4);
    auto powed = (uint64_t) pow(myLog2(m),2);
    return (rounded*(powed-myLog2(m))+m-1)*c_compEl(b, b2);
    //return (b+b2)*(30*m+myLog2(m));
}

/**
 * costs for shuffling m b-bit elements using given control bits
 * @param m: number of elements to shuffle
 * @param b: number of bits of elements
 * @return costs (gates, traffic, rounds) to shuffle the elements
 */
outType& c_shuffle(uint64_t m, uint64_t b) {
    // condSwap(b) * (m * log2(m) - m + 1)
    return (m * myLog2(m) - m + 1)*c_condSwap(b);
}
// TODO: hier fehlt irgendwas wegen der inputs. steht im Revisiting Paper

/**
 * costs for comparing each element to its neighbours to determine duplicates
 * @param m: number of elements to filter
 * @param b: number of bit to compare by
 * @return costs (gates, traffic, rounds) for filtering duplicates
 */
outType& c_dDup(const uint64_t m, const uint64_t b) {
    assert(m > 1);
    // (m-1)*compEq(b)
    return (m-1)*c_comp_eq(b);
}

/**
 * costs for returning an arbitrary b-bit value
 * @param b number of bit for random value
 * @return costs (gates, traffic, rounds) for creating an arbitrary b-bit value
 */
outType& c_rand(uint64_t b) {
    auto* out = new outType;
    *out = {0, 2*b, 2};
    return *out;
}