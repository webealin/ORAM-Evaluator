//
// Created by weber on 27.07.2017.
//

#include "plotter.h"

void Plotter::plot_Elements(evalParam mParam, uint64_t b) {
    CSVFileWriter* writer = new CSVFileWriter("test");
    clock_t start = clock();

    for(uint64_t m = mParam.min; m <= mParam.max; m = m*mParam.step_m + mParam.step_p) {
        writer->addLine(m);
        uint16_t d = myLog2(m);

        writer->addOutType(&find_LinearScanORAM(1, m, b));

        btSettings* minBTS = find_best_BT(1, m, b, "Binary Tree ORAM - fast", evalParam{d, (uint16_t) (10*d), 1, d}, acc_BT_fast);
        writer->addOutType(minBTS->out);
        delete minBTS;
        //writer->addOutType(find_LinearScanORAM(1, m, b));
        //writer->addOutType(find_LinearScanORAM(1, m, b));
    }

    float elapsed = (float)(clock() - start) / CLOCKS_PER_SEC;
    std::cout << "Plotted all - time needed: " << elapsed << std::endl;
}

void Plotter::plot_Bitwidth(uint64_t m, evalParam bParam) {
    clock_t start = clock();
    for(uint64_t b = bParam.min; b <= bParam.max; b = b*bParam.step_m + bParam.step_p) {

    }

    float elapsed = (float)(clock() - start) / CLOCKS_PER_SEC;
    std::cout << "Plotted all - time needed: " << elapsed << std::endl;
}

