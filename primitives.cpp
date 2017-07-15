//
// library of building blocks that contains count of linear gates
// and number of bits to transfer [byte] / rounds of interactions
// values are calculated depending on entry in settings file
// Created by weber on 30.06.2017.
//

#include "primitives.h"

/**
 *
 * @param m
 * @param b
 * @return
 */
uint64_t c_Y2B(uint64_t m, uint64_t b) {
    switch(Settings::toCalc) {
        case Settings::TRAFFIC:
            return 0;               // TODO
        case Settings::ROUNDS:
            return 0;               // TODO
        default: return 0;
    }
}

/**
 *
 * @param m
 * @param b
 * @return
 */
uint64_t c_B2Y(uint64_t m, uint64_t b) {
    switch(Settings::toCalc) {
        case Settings::TRAFFIC:
            return 0;               // TODO
        case Settings::ROUNDS:
            return 0;               // TODO
        default: return 0;
    }
}

uint64_t c_sShare(uint64_t m, uint64_t b, bool values) {
    switch(Settings::toCalc) {
        case Settings::TRAFFIC:
            return 0;               // TODO
        case Settings::ROUNDS:
            return 0;               // TODO
        default: return 0;
    }
}

/**
 * costs for a b-bit 2:1 multiplexer
 * @param b: number of bits to represent the two elements
 * @return costs (gates, traffic, rounds) for the multiplexer
 */
uint64_t c_mux(uint64_t b) {
    switch(Settings::toCalc) {
        case Settings::TRAFFIC:
            return 32*b;
        case Settings::ROUNDS:
            return 4*b;
        default: return b;
    }
}

/**
 * costs for a b-bit m:1 tree multiplexer
 * @param m: number of elements to connect via the multiplexer
 * @param b: number of bits of the respective elements
 * @return costs (gates, traffic, rounds) for the multiplexer tree
 */
uint64_t c_mux(uint64_t m, uint64_t b) {
    // (m-1)*c_mux(b)
    uint64_t gates = (m-1)*c_mux(b);
    switch(Settings::toCalc) {
        case Settings::TRAFFIC:
            return 32*gates;
        case Settings::ROUNDS:
            return 4*gates;
        default: return gates;
    }
}

/**
 * costs for a b-bit m+1:1 multiplexer chain of m elements and one dummy element
 * @param m: number of elements to connect excluding the dummy
 * @param b: number of bits of the respective elements
 * @return costs (gates, traffic, rounds) for the multiplexer chain
 */
uint64_t c_mux_chain(uint64_t m, uint64_t b) {
    // m*c_mux(b)
    uint64_t gates = m*c_mux(b);
    switch(Settings::toCalc) {
        case Settings::TRAFFIC:
            return 32*gates;
        case Settings::ROUNDS:
            return 4*gates;
        default: return m*b;
    }
}

/**
 * costs for adding two b-bit numbers (excluding carry out)
 * @param b: number of bits of the two numbers
 * @return costs (gates, traffic, rounds) of addition
 */
uint64_t c_adder(uint64_t b) {
    switch(Settings::toCalc) {
        case Settings::TRAFFIC:
            return 32*(b-1);
        case Settings::ROUNDS:
            return 4*(b-1);
        default: return b-1;
    }
}

/**
 * costs for adding two b-bit numbers (including carry out)
 * @param b: number of bits of the two numbers
 * @return costs (gates, traffic, rounds) of addition
 */
uint64_t c_adder_carry(uint64_t b) {
    switch(Settings::toCalc) {
        case Settings::TRAFFIC:
            return 32*b;
        case Settings::ROUNDS:
            return 4*b;
        default: return b;
    }
}

/**
 * costs for equality comparison of two b-bit numbers
 * @param b: number of bits of the two numbers
 * @return costs (gates, traffic, rounds) of equality comparison
 */
uint64_t c_comp_eq(uint64_t b) {
    // b-1 OR
    switch(Settings::toCalc) {
        case Settings::TRAFFIC:
            return 32*(b-1);
        case Settings::ROUNDS:
            return 4*(b-1);
        default: return b-1;
    }
}

/**
 * costs for magnitude comparison of two b-bit numbers
 * @param b: number of bits of the two numbers
 * @return costs (gates, traffic, rounds) of magnitude comparison
 */
uint64_t c_comp_mag(uint64_t b) {
    // add carry
    switch(Settings::toCalc) {
        case Settings::TRAFFIC:
            return 32*c_adder_carry(b);
        case Settings::ROUNDS:
            return 4*c_adder_carry(b);
        default: return c_adder_carry(b);
    }
}

/**
 * costs for counting leading zeros of a b-bit number
 * @param b: number of bits to represent number
 * @return costs (gates, traffic, rounds) for counting leading zeros
 */
uint64_t c_LZC(uint64_t b) {
    // (b-1)*OR + sum (i=0, log(b)): (b/2^(i+1))*c_add(i)
    uint64_t gates = (uint64_t)(floor((double) b/2))* myLog2(b)+b-1;     // TODO: Formel Unsinn. Neu machen!

    switch(Settings::toCalc) {
        case Settings::TRAFFIC:
            return 32*gates;
        case Settings::ROUNDS:
            return 4*gates;
        default: return gates;
    }
}

/**
 * decodes a b-bit value into one-hot-code
 * @param b: number of bits to represent value
 * @return costs (gates, traffic, rounds) for decoding
 */
uint64_t c_decode(uint64_t b) {
    uint64_t powed = (uint64_t) pow(2, b);      // TODO: nötig?
    switch(Settings::toCalc) {
        case Settings::TRAFFIC:
            return 64*powed;
        case Settings::ROUNDS:
            return 8*powed;
        default: return 2*powed;
    }
}

/**
 * costs for swapping two b-bit elements if the select input is true
 * @param b: number of bits of elements to swap
 * @return costs (gates, traffic, rounds) for conditionally swapping two b-bit values
 */
uint64_t c_condSwap(uint64_t b) {
    switch(Settings::toCalc) {
        case Settings::TRAFFIC:
            return 32*b;
        case Settings::ROUNDS:
            return 4*b;
        default: return b;
    }
}

/**
 * costs for one comparison element, returns maximum and minimum element of two b-bit input elements
 * @param b: number of bits of elements
 * @param b2: number of bits to represent the tag to sort by
 * @return costs (gates, traffic, rounds) for one comparison element for b-bit elements with b2-bit tags
 */
uint64_t c_compEl(uint64_t b, uint64_t b2) {
    // CompMag(b2)+CondSwap(b)
    uint64_t gates = c_comp_mag(b2) + c_condSwap(b);
    switch(Settings::toCalc) {
        case Settings::TRAFFIC:
            return 32*gates;
        case Settings::ROUNDS:
            return 4*gates;
        default: return gates;
    }
}

/**
 * costs for sorting m b-bit elements by their b2 bit tag
 * @param m: number of elements to sort
 * @param b: number of bits of elements
 * @param b2: number of bits to represent the tag to sort by
 * @return costs (gates, traffic, rounds) fort sorting
 */
uint64_t c_sort(uint64_t m, uint64_t b, uint16_t b2) {
    // compEl(b, b2)*((1/4)*m*(log^2(m)-log(m))+m-1)
    uint64_t rounded = (uint64_t) ceil((double) m/4);
    uint64_t powed = (uint64_t) pow(myLog2(m),2);
    uint64_t gates = c_compEl(b, b2)*(rounded*(powed- myLog2(m))+m-1);

    switch(Settings::toCalc) {
        case Settings::TRAFFIC:
            return 32*gates;
        case Settings::ROUNDS:
            return 4*gates;
        default: return gates;
    }
    //return (b+b2)*(30*m+myLog2(m));
}

/**
 * costs for shuffling m b-bit elements using given control bits
 * @param m: number of elements to shuffle
 * @param b: number of bits of elements
 * @return costs (gates, traffic, rounds) to shuffle the elements
 */
uint64_t c_shuffle(uint64_t m, uint64_t b) {
    // condSwap(b) * (m * log2(m) - m + 1)
    uint64_t gates = c_condSwap(b) * (m * myLog2(m) - m + 1);

    switch(Settings::toCalc) {
        case Settings::TRAFFIC:
            return 32*gates;
        case Settings::ROUNDS:
            return 4*gates;
        default: return gates;
    }
}
// TODO: hier fehlt irgendwas wegen der inputs. steht im Revisiting Paper

/**
 * costs for comparing each element to its neighbours to determine duplicates
 * @param m: number of elements to filter
 * @param b: number of bit to compare by
 * @return costs (gates, traffic, rounds) for filtering duplicates
 */
uint64_t c_dDup(uint64_t m, uint64_t b) {
    // (m-1)*compEq(b)
    uint64_t gates = (m-1)*c_comp_eq(b);

    switch(Settings::toCalc) {
        case Settings::TRAFFIC:
            return 32*gates;
        case Settings::ROUNDS:
            return 4*gates;
        default: return gates;
    }
}

/**
 * costs for returning an arbitrary b-bit value
 * @param b number of bit for random value
 * @return costs (gates, traffic, rounds) for creating an arbitrary b-bit value
 */
uint64_t c_rand(uint64_t b) {
    std::cout << "was zur hölle passiert hier???" << std::endl;
    switch(Settings::toCalc) {
        case Settings::TRAFFIC:
            return 0;               // TODO
        case Settings::ROUNDS:
            return 0;               // TODO
        default: return 0;
    }
}