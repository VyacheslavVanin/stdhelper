#pragma once
#include "actionlist.hpp"
#include <functional>
#include <string>
#include <unordered_map>

namespace vvv {

template <typename K, typename T>
class ActionMap {
public:
    using key_type = K;

    void addAction(const key_type& name, const std::function<T>& f)
    {
        actions[name].addAction(f);
    }

    template <typename... Args>
    void invoke(const key_type& name, Args&... args) const
    {
        const auto& it = actions.find(name);
        if (it == actions.end())
            return;
        it->second(args...);
    }

    const ActionList<T>& operator[](const key_type& name) const
    {
        return actions.at(name);
    }

private:
    std::unordered_map<key_type, ActionList<T>> actions = {};
};

} // namespace vvv
