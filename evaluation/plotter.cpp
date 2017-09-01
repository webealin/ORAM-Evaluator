//
// Created by weber on 27.07.2017.
//

#include "plotter.h"

void Plotter::plot(uint32_t noAcc, bool values, uint16_t d, uint64_t b, CSVFileWriter* writer, funcTypes func) {
    writer->addLine(d);
    auto m = (uint64_t) pow(2, d);

    writer->addOutType(find_LinearScan(noAcc, 0, m, b));
    writer->addOutType(find_LinearScan(0, noAcc, m, b));
    writer->addOutType(find_best_BT(noAcc, values, m, b, StandardBTB(d), func.btF));
    writer->addOutType(find_best_Path(noAcc, values, m, b, StandardPathB(d), StandardPathS(), func.pathF));
    writer->addOutType(find_best_Path(noAcc, values, m, b, StandardPathSCB(d), StandardPathS(), func.pathSCF));
    writer->addOutType(find_best_Path(noAcc, values, m, b, StandardScoramB(d), StandardScoramS(d), func.SCORAMF));
    writer->addOutType(find_best_Path(noAcc, values, m, b, StandardPathB(d), StandardPathS(), func.CORAMF));
    writer->addOutType(find_best_OSQR(noAcc, values, m, b, func.OSQRF));
    std::cout << "Plotted d = " << d << std::endl;
}

void Plotter::plot_acc_elements(uint32_t noAcc, evalParam dParam, uint64_t b, const std::string& filename) {
    CSVFileWriter* writer = new CSVFileWriter(filename);
    writer->addHeader("d, Linear Scan Read, Linear Scan Write, Binary Tree ORAM, Path ORAM, Path-SC, SCORAM, Circuit ORAM, Optimized SQR ORAM");
    clock_t start = clock();

    for(uint16_t d = dParam.min; d <= dParam.max; d = d*dParam.step_m + dParam.step_p)
        plot(noAcc, false, d, b, writer, acc_slow_func);                // TODO: umstellen auf fast

    writer->writeFiles();
    delete writer;
    float elapsed = (float)(clock() - start) / CLOCKS_PER_SEC;
    std::cout << "Plotted all - time needed: " << elapsed << std::endl;
}

void Plotter::plot_elements(uint32_t noAcc, bool values, evalParam dParam, uint64_t b, const std::string& filename) {
    CSVFileWriter* writer = new CSVFileWriter(filename);
    writer->addHeader("d, Linear Scan Read, Linear Scan Write, Binary Tree ORAM, Path ORAM, Path-SC, SCORAM, Circuit ORAM, Optimized SQR ORAM");
    clock_t start = clock();

    for(uint16_t d = dParam.min; d <= dParam.max; d = d*dParam.step_m + dParam.step_p)
        plot(noAcc, values, d, b, writer, slow_func);                // TODO: umstellen auf fast

    writer->writeFiles();
    delete writer;
    float elapsed = (float)(clock() - start) / CLOCKS_PER_SEC;
    std::cout << "Plotted all - time needed: " << elapsed << std::endl;
}

void Plotter::plot_acc_bitwidth(uint32_t noAcc, uint64_t m, evalParam bParam, const std::string& filename) {
    CSVFileWriter* writer = new CSVFileWriter(filename);
    writer->addHeader("b, Linear Scan Read, Linear Scan Write, Binary Tree ORAM, Path ORAM, Path-SC, SCORAM, Circuit ORAM, Optimized SQR ORAM");
    clock_t start = clock();
    for(uint64_t b = bParam.min; b <= bParam.max; b = b*bParam.step_m + bParam.step_p) {
        uint16_t d = myLog2(m);
        plot(noAcc, false, d, b, writer, acc_slow_func);                // TODO: umstellen auf fast
    }
    writer->writeFiles();
    delete writer;
    float elapsed = (float)(clock() - start) / CLOCKS_PER_SEC;
    std::cout << "Plotted all - time needed: " << elapsed << std::endl;
}


void Plotter::plot_bitwidth(uint32_t noAcc, bool values, uint64_t m, evalParam bParam, const std::string& filename) {
    CSVFileWriter* writer = new CSVFileWriter(filename);
    writer->addHeader("b, Linear Scan Read, Linear Scan Write, Binary Tree ORAM, Path ORAM, Path-SC, SCORAM, Circuit ORAM, Optimized SQR ORAM");
    clock_t start = clock();
    for(uint64_t b = bParam.min; b <= bParam.max; b = b*bParam.step_m + bParam.step_p) {
        uint16_t d = myLog2(m);
        plot(noAcc, values, d, b, writer, slow_func);                // TODO: umstellen auf fast
    }
    writer->writeFiles();
    delete writer;
    float elapsed = (float)(clock() - start) / CLOCKS_PER_SEC;
    std::cout << "Plotted all - time needed: " << elapsed << std::endl;
}


void Plotter::plot_breakeven_points(evalParam dParam, evalParam bParam, const std::string& filename) {
    CSVFileWriter* writer = new CSVFileWriter(filename);
    clock_t start = clock();

    std::cout << "Plotter::plot_breakeven_points: not implemented yet!" << std::endl;
    /*for(uint16_t d = dParam.min; d <= dParam.max; d = d*dParam.step_m + dParam.step_p) {
        for(uint16_t b = bParam.min; b <= bParam.max; b = b*bParam.step_m + bParam.step_p) {
            // TODO
        }
    }*/
    writer->writeFiles();
    delete writer;
    float elapsed = (float)(clock() - start) / CLOCKS_PER_SEC;
    std::cout << "Plotted all - time needed: " << elapsed << std::endl;
}