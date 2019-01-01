#pragma once
#include <string>
#include <vector>

namespace vvv {
namespace helpers {

inline std::string joinStringsWith(const std::vector<std::string>& v,
                                   const std::string& delimiter)
{
    std::string ret;
    const auto numStrings = v.size();
    const auto lastIndex = numStrings - 1;
    for (size_t i = 0; i < numStrings; ++i) {
        ret += v[i];
        if (i != lastIndex)
            ret += delimiter;
    }

    return ret;
}

inline void split(const std::string& input, char delimiter,
                  std::vector<std::string>& ret)
{
    using namespace std;
    size_t last_pos = 0;
    while (true) {
        const size_t current_pos = input.find(delimiter, last_pos);
        if (current_pos == string::npos) {
            const string substr =
                input.substr(last_pos, input.size() - last_pos);
            ret.push_back(substr);
            break;
        }
        else {
            const string substr =
                input.substr(last_pos, current_pos - last_pos);
            ret.push_back(substr);
            last_pos = current_pos + 1;
        }
    }
}

inline std::vector<std::string> split(const std::string& str,
                                      char delimiter = ' ')
{
    std::vector<std::string> ret;
    split(str, delimiter, ret);
    return ret;
}
} // namespace helpers
} // namespace vvv
