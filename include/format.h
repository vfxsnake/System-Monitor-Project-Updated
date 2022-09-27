#ifndef FORMAT_H
#define FORMAT_H

#include <string>

namespace Format {
std::string ElapsedTime(long times);
std::string ZeroPadding(std::string &source, int padding_size=2);
};                                    // namespace Format

#endif