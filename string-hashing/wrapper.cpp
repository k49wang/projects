#include "wrapper.hpp"
#include "string_hash.hpp"

Wrapper::Wrapper() {
    m_map = std::make_shared<StringHashMap>();
}

size_t Wrapper::addWrapper(const std::string& s) {
    return m_map->add(s);
}

size_t Wrapper::findWrapper(const std::string& s) {
    return m_map->find(s);
}
std::vector<std::pair<std::string, std::pair<size_t, size_t>>> Wrapper::collectWrapper() const {
    return m_map->collectAllString();
}
