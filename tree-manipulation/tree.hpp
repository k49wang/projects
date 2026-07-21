#ifndef __TREE_HPP__
#define __TREE_HPP__

#include <memory>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <set>

class Node {
    // member fields
    size_t m_id = 0;
    size_t m_count = 1;

    // for tree-use
    std::shared_ptr<Node> m_first = nullptr;
    std::shared_ptr<Node> m_next = nullptr;
    std::shared_ptr<Node> m_prev = nullptr;

    // for caching purposes
    std::shared_ptr<Node> m_cachePrev = nullptr;
    std::shared_ptr<Node> m_cacheNext = nullptr;

public:
    
    size_t getID() const;
    void setID(size_t id);

    size_t getCount() const;
    void incrementCount(size_t count);
    void decrementCount(size_t count);

    std::shared_ptr<Node> getFirst() const;
    void setFirst(std::shared_ptr<Node> node);

    std::shared_ptr<Node> getNext() const;
    void setNext(std::shared_ptr<Node> node);

    std::shared_ptr<Node> getPrev() const;
    void setPrev(std::shared_ptr<Node> node);

    std::shared_ptr<Node> getCachePrev() const;
    void setCachePrev(std::shared_ptr<Node> node);

    std::shared_ptr<Node> getCacheNext() const;
    void setCacheNext(std::shared_ptr<Node> node);
};

class Tree {
    std::shared_ptr<Node> m_root = nullptr;

    // keep track of all smallest available IDs
    std::set<size_t> m_nextIDs;
    // get the next smallest available ID
    size_t getSmallestID();

    std::shared_ptr<Node> m_cacheHead = nullptr;
    void addToFrontCache(std::shared_ptr<Node> node);
    void moveToFrontCache(std::shared_ptr<Node> node, std::unordered_set<std::shared_ptr<Node>>& uniqueNodes);
    void removeFromCache(std::shared_ptr<Node> node);
    
    // keep track of all existing nodes with id as the key
    std::unordered_map<size_t, std::shared_ptr<Node>> m_nodes;

    std::shared_ptr<Node> getParentInternal(std::shared_ptr<Node> node);
    std::shared_ptr<Node> getParent(std::shared_ptr<Node> n1, std::shared_ptr<Node> n2, std::unordered_set<std::shared_ptr<Node>>& uniqueNodes) const;

    void clear();
    void removeNodeByAddress(std::shared_ptr<Node> node, size_t& removed);

public:
    Tree();

    void addNodeByParentID(int parentID);
    void removeNodeByID(size_t id, size_t& removed);
    std::vector<std::vector<size_t>> getAllIDs() const;
    std::vector<size_t> getIDsByCreationTime() const;
    std::vector<size_t> getIDsWithDepth(size_t depth) const;
    bool areCousins(size_t id1, size_t id2, size_t& count) const;
    size_t getCount(size_t id) const;

};
#endif
