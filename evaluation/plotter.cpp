//
// Created by weber on 27.07.2017.
//

#include "plotter.h"

void Plotter::plot_Elements(evalParam dParam, uint64_t b) {
    CSVFileWriter* writer = new CSVFileWriter("test");
    clock_t start = clock();

    for(uint16_t d = dParam.min; d <= dParam.max; d = d*dParam.step_m + dParam.step_p) {
        std::cout << "Plotted d = " << d << std::endl;
        writer->addLine(d);
        auto m = (uint64_t) pow(2, d);

        //writer->addOutType(find_LinearScan(1, 0, m, b));
        //writer->addOutType(find_LinearScan(0, 1, m, b));
        writer->addOutType(find_best_BT(1, m, b, evalParam{d, static_cast<uint16_t>(10*d), 1, d}, acc_BT_fast).out);
        writer->addOutType(find_best_Path(1, m, b, evalParam{4, d, 1, 2}, evalParam{d, static_cast<uint16_t>(5*d), 1, 1}, acc_Path_fast).bt->out);
        writer->addOutType(find_best_Path(1, m, b, evalParam{4, d, 2, 0}, evalParam{d, static_cast<uint16_t>(5*d), 1, 1}, acc_PathSC_fast).bt->out);
        writer->addOutType(find_best_Path(1, m, b, evalParam{6, d, 1, 2}, evalParam{d, static_cast<uint16_t>(5*d), 1, 1}, acc_SCORAM_fast).bt->out);
        writer->addOutType(find_best_Path(1, m, b, evalParam{4, d, 1, 2}, evalParam{d, static_cast<uint16_t>(5*d), 1, 1}, acc_CORAM_fast).bt->out);
    }
    writer->writeFiles();
    delete writer;
    float elapsed = (float)(clock() - start) / CLOCKS_PER_SEC;
    std::cout << "Plotted all - time needed: " << elapsed << std::endl;
}

void Plotter::plot_Bitwidth(uint64_t m, evalParam bParam) {
    CSVFileWriter* writer = new CSVFileWriter("test");
    clock_t start = clock();
    for(uint64_t b = bParam.min; b <= bParam.max; b = b*bParam.step_m + bParam.step_p) {
        uint16_t d = myLog2(m);
        writer->addLine(d);

        writer->addOutType(find_LinearScan(1, 0, m, b));
        writer->addOutType(find_LinearScan(0, 1, m, b));
        writer->addOutType(find_best_BT(1, m, b, evalParam{d, (uint16_t) (10*d), 1, d}, acc_BT_fast).out);
        writer->addOutType(find_best_Path(1, m, b, evalParam{4, d, 1, 2}, evalParam{d, static_cast<uint16_t>(5*d), 1, 1}, acc_Path_fast).bt->out);
        writer->addOutType(find_best_Path(1, m, b, evalParam{4, d, 2, 0}, evalParam{d, static_cast<uint16_t>(5*d), 1, 1}, acc_PathSC_fast).bt->out);
        writer->addOutType(find_best_Path(1, m, b, evalParam{6, d, 1, 2}, evalParam{d, static_cast<uint16_t>(5*d), 1, 1}, acc_SCORAM_fast).bt->out);
        writer->addOutType(find_best_Path(1, m, b, evalParam{4, d, 1, 2}, evalParam{d, static_cast<uint16_t>(5*d), 1, 1}, acc_CORAM_fast).bt->out);
    }
    writer->writeFiles();
    delete writer;
    float elapsed = (float)(clock() - start) / CLOCKS_PER_SEC;
    std::cout << "Plotted all - time needed: " << elapsed << std::endl;
}
