//
// Created by weber on 27.07.2017.
//

#include "plotter.h"

void Plotter::plot(uint16_t noAcc, bool values, uint16_t d, uint64_t b, CSVFileWriter* writer, funcTypes func) {
    auto m = (uint64_t) pow(2, d);

    writer->addOutType(find_LinearScan(noAcc, 0, m, b));
    writer->addOutType(find_LinearScan(0, noAcc, m, b));
    writer->addOutType(find_best_BT(noAcc, values, m, b, StandardBTB(d), func.btF));
    writer->addOutType(find_best_Path(noAcc, values, m, b, "Path ORAM", StandardPathB(d), StandardPathS(), func.pathF));
    writer->addOutType(find_best_Path(noAcc, values, m, b, "Path-SC ORAM", StandardPathSCB(d), StandardPathS(), func.pathSCF));
    writer->addOutType(find_best_Path(noAcc, values, m, b, "SCORAM", StandardScoramB(d), StandardScoramS(d), func.SCORAMF));
    writer->addOutType(find_best_Path(noAcc, values, m, b, "Circuit ORAM", StandardPathB(d), StandardPathS(), func.CORAMF));

    auto acc = (uint32_t) (floor(sqrt(m * d)));     // TODO
    writer->addOutType(divideWR(*find_best_OSQR(acc, values, m, b, func.OSQRF).out, acc));        // TODO mixed ORAM!
    std::cout << "Plotted d = " << d << std::endl;
}

void Plotter::plot_acc_elements(evalParam dParam, uint64_t b, const std::string& filename) {
    CSVFileWriter* writer = new CSVFileWriter(filename);
    writer->addHeader("d, Linear Scan Read, Linear Scan Write, Binary Tree ORAM, Path ORAM, Path-SC, SCORAM, Circuit ORAM, Optimized SQR ORAM");
    clock_t start = clock();

    for(uint16_t d = dParam.min; d <= dParam.max; d = d*dParam.step_m + dParam.step_p) {
        writer->addLine(d);
        plot(1, false, d, b, writer, acc_slow_func);                // TODO: umstellen auf fast
    }

    writer->writeFiles();
    delete writer;
    float elapsed = (float)(clock() - start) / CLOCKS_PER_SEC;
    std::cout << "Plotted all - time needed: " << elapsed << std::endl;
}

void Plotter::plot_elements(uint16_t noAcc, bool values, evalParam dParam, uint64_t b, const std::string& filename) {
    CSVFileWriter* writer = new CSVFileWriter(filename);
    writer->addHeader("d, Linear Scan Read, Linear Scan Write, Binary Tree ORAM, Path ORAM, Path-SC, SCORAM, Circuit ORAM, Optimized SQR ORAM");
    clock_t start = clock();

    for(uint16_t d = dParam.min; d <= dParam.max; d = d*dParam.step_m + dParam.step_p) {
        writer->addLine(d);
        plot(noAcc, values, d, b, writer, slow_func);                // TODO: umstellen auf fast
    }

    writer->writeFiles();
    delete writer;
    float elapsed = (float)(clock() - start) / CLOCKS_PER_SEC;
    std::cout << "Plotted all - time needed: " << elapsed << std::endl;
}

void Plotter::plot_acc_bitwidth(uint64_t m, evalParam bParam, const std::string& filename) {
    CSVFileWriter* writer = new CSVFileWriter(filename);
    writer->addHeader("b, Linear Scan Read, Linear Scan Write, Binary Tree ORAM, Path ORAM, Path-SC, SCORAM, Circuit ORAM, Optimized SQR ORAM");
    clock_t start = clock();
    for(uint64_t b = bParam.min; b <= bParam.max; b = b*bParam.step_m + bParam.step_p) {
        writer->addLine(b);
        uint16_t d = myLog2(m);
        plot(1, false, d, b, writer, acc_slow_func);                // TODO: umstellen auf fast
    }
    writer->writeFiles();
    delete writer;
    float elapsed = (float)(clock() - start) / CLOCKS_PER_SEC;
    std::cout << "Plotted all - time needed: " << elapsed << std::endl;
}


void Plotter::plot_bitwidth(uint16_t noAcc, bool values, uint64_t m, evalParam bParam, const std::string& filename) {
    CSVFileWriter* writer = new CSVFileWriter(filename);
    writer->addHeader("b, Linear Scan Read, Linear Scan Write, Binary Tree ORAM, Path ORAM, Path-SC, SCORAM, Circuit ORAM, Optimized SQR ORAM");
    clock_t start = clock();
    for(uint64_t b = bParam.min; b <= bParam.max; b = b*bParam.step_m + bParam.step_p) {
        writer->addLine(b);
        uint16_t d = myLog2(m);
        plot(noAcc, values, d, b, writer, slow_func);                // TODO: umstellen auf fast
    }
    writer->writeFiles();
    delete writer;
    float elapsed = (float)(clock() - start) / CLOCKS_PER_SEC;
    std::cout << "Plotted all - time needed: " << elapsed << std::endl;
}