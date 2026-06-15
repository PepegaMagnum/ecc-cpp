#include <chrono>
#include <iostream>
#include <limits>
#include <include/RhoPollard.h>

class TimingStats {
    double m_total = 0.0;
    double m_min   = std::numeric_limits<double>::max();
    double m_max   = 0.0;
    size_t m_count = 0;

public:
    void add(double ms) {
        m_total += ms;
        if (ms < m_min) m_min = ms;
        if (ms > m_max) m_max = ms;
        ++m_count;
    }

    double average() const { return m_count ? m_total / m_count : 0.0; }
    double min()     const { return m_count ? m_min : 0.0; }
    double max()     const { return m_max; }
    size_t count()   const { return m_count; }
    double total()   const { return m_total; }

    void report() const {
        std::cout << "  runs: "  << m_count
                  << " | avg: "  << average() << " ms"
                  << " | min: "  << min()     << " ms"
                  << " | max: "  << max()     << " ms"
                  << " | total: "<< m_total   << " ms" << std::endl;
    }
};

// Times one computeLog call, records it in stats, and returns the elapsed ms.
double timedComputeLog(RhoPollard& rho, Point& P, Point& Q,
                       mpz_t result, TimingStats& stats);