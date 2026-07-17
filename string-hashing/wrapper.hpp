#ifndef __WRAPPER_HPP__
#define __WRAPPER_HPP__

#include <memory>
#include <vector>

class StringHashMap;

class Wrapper {
    std::shared_ptr<StringHashMap> m_map;
public:
    Wrapper();
    size_t addWrapper(const std::string& s);
    size_t findWrapper(const std::string& s);
    std::vector<std::pair<std::string, std::pair<size_t, size_t>>> collectWrapper() const;
};

#endif
