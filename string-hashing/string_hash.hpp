#ifndef __STRING_HASH_HPP
#define __STRING_HASH_HPP

#include <vector>
#include <string>
#include <cstddef>
#include <memory>
#include <unordered_map>

#define CAP 29

struct Node {
    std::string word;
    size_t probes;
    std::shared_ptr<Node> next;

    Node(const std::string& word, size_t probes);
};

class StringHashMap {
private:
    std::unordered_map<std::string, size_t> m_words;
    std::shared_ptr<Node> stringMap[CAP];

public:
    StringHashMap();
    size_t add(const std::string& s);
    size_t find(const std::string& s);
    std::vector<std::pair<std::string, std::pair<size_t, size_t>>> collectAllString() const;
    void print() const;
};

#endif
