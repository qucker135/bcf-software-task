#ifndef __STATISTICS_HPP__
#define __STATISTICS_HPP__

#include <stdint.h>
#include <ostream>

using std::ostream;

typedef struct Statistics{
    uint64_t files;
    uint64_t non_empty_lines;
    uint64_t empty_lines;
    uint64_t words;
    uint64_t letters;
    friend ostream& operator<<(ostream& os, const Statistics& st);
    Statistics operator+(const Statistics&);
    Statistics& operator+=(const Statistics&);
    bool operator==(const Statistics&);
}Statistics;

#endif