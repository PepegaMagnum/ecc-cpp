//
// Created by shen on 15.06.2026.
//

#include "../include/TimingStats.h"

double timedComputeLog(RhoPollard& rho, Point& P, Point& Q,
                       mpz_t result, TimingStats& stats) {
    auto start = std::chrono::steady_clock::now();
    rho.computeLog(P, Q, result);
    auto end = std::chrono::steady_clock::now();

    std::chrono::duration<double, std::milli> elapsed = end - start;
    double ms = elapsed.count();
    stats.add(ms);
    return ms;
}
