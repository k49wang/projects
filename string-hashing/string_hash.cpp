#include <algorithm>
#include <functional>
#include <stdio.h>
#include "string_hash.hpp"

#define CAP 29

Node::Node(const std::string& word, size_t probes):
    word{word}, probes{probes}, next{nullptr} { }

StringHashMap::StringHashMap() {
    for (size_t i = 0; i < CAP; i++) stringMap[i] = nullptr;
}

size_t StringHashMap::add(const std::string& s) {
    if (m_words.find(s) != m_words.end()) return 0;
    std::hash<std::string> hasher;
    size_t pos = hasher(s) % CAP;

    std::shared_ptr<Node> newNode = std::make_shared<Node>(s, 1);
    newNode->next = stringMap[pos];
    stringMap[pos] = newNode;
    m_words[s] = pos;
    return 1;
}

size_t StringHashMap::find(const std::string& s) {
    if (m_words.find(s) == m_words.end()) return 0;

    std::hash<std::string> hasher;
    size_t pos = hasher(s) % CAP;
    size_t probes = 0;
    auto currNode = stringMap[m_words[s]];
    while (currNode) {
        probes += 1;
        if (currNode->word == s) break;
        currNode = currNode->next;
    }
    return probes;
}

std::vector<std::pair<std::string, std::pair<size_t, size_t>>> StringHashMap::collectAllString() const {
    std::vector<std::pair<std::string, std::pair<size_t, size_t>>> res;
    for (size_t i = 0; i < CAP; i++) {
        auto currNode = stringMap[i];
        
        while (currNode) {
            std::pair<size_t, size_t> p1 = {currNode->probes, i};
            std::pair<std::string, std::pair<size_t, size_t>> p = {currNode->word, p1};
            res.emplace_back(p);
            currNode = currNode->next;
        }
    }

    std::sort(res.begin(), res.end(),
        [](const std::pair<std::string, std::pair<size_t, size_t>>& p1, const std::pair<std::string, std::pair<size_t, size_t>>& p2) {
            if (p1.second.first == p2.second.first) return p1.first < p2.first;
            return p1.second.first < p2.second.first;
    });
    return res;
}

void StringHashMap::print() const {
    for (size_t i = 0; i < CAP; i++) {
        printf("Bucket %2ld: ", i);
        auto currNode = stringMap[i];
        if (currNode == nullptr) {
            printf("EMPTY\n");
        } else {
            printf("[%s, %ld]", currNode->word.c_str(), currNode->probes);
            currNode = currNode->next;
            while (currNode) {
                printf(" -> [%s, %ld]", currNode->word.c_str(), currNode->probes);
                currNode = currNode->next;
            }
            printf("\n");
        }
    }
}