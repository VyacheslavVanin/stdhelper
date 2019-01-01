#pragma once
#include <sstream>
#include <string>

namespace vvv {
namespace helper {

namespace impl {
void format(std::ostream& str, const std::string& fmt, size_t pos)
{
    const char* cpos = fmt.c_str() + pos;
    str << cpos;
}

template <typename T, typename... Args>
void format(std::ostream& str, const std::string& fmt, size_t pos,
            const T& value, Args&&... args)
{
    const auto s = fmt.size();
    const auto del_pos = fmt.find('@', pos);
    const char* cpos = fmt.c_str() + pos;
    if (del_pos == std::string::npos) {
        str.write(cpos, s - pos);
        return;
    }

    str.write(cpos, del_pos - pos);
    str << value;
    format(str, fmt, del_pos + 1, std::forward<Args>(args)...);
}
} // namespace impl

template <typename T, typename... Args>
void format(std::ostream& str, const std::string& fmt, const T& value,
            Args&&... args)
{
    impl::format(str, fmt, 0, value, std::forward<Args>(args)...);
}

template <typename T, typename... Args>
std::string format(const std::string& fmt, const T& value, Args&&... args)
{
    std::stringstream ss;
    impl::format(ss, fmt, 0, value, std::forward<Args>(args)...);
    return ss.str();
}

} // namespace helper
} // namespace vvv
