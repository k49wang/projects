#include <queue>
#include <iostream>
#include <algorithm>
#include "tree.hpp"

size_t Node::getID() const { return m_id; }
void Node::setID(size_t id) { m_id = id; }

size_t Node::getCount() const { return m_count; }
void Node::incrementCount(size_t count) { m_count += count; }
void Node::decrementCount(size_t count) { m_count -= count; }

std::shared_ptr<Node> Node::getFirst() const { return m_first; }
void Node::setFirst(std::shared_ptr<Node> node) { m_first = node; }

std::shared_ptr<Node> Node::getPrev() const { return m_prev; }
void Node::setPrev(std::shared_ptr<Node> node) { m_prev = node; }

std::shared_ptr<Node> Node::getNext() const { return m_next; }
void Node::setNext(std::shared_ptr<Node> node) { m_next = node; }

std::shared_ptr<Node> Node::getCachePrev() const { return m_cachePrev; }
void Node::setCachePrev(std::shared_ptr<Node> node) { m_cachePrev = node; }

std::shared_ptr<Node> Node::getCacheNext() const { return m_cacheNext; }
void Node::setCacheNext(std::shared_ptr<Node> node) { m_cacheNext = node; }

Tree::Tree() {
    m_nextIDs.emplace(0);
}

size_t Tree::getSmallestID() {
    auto iter = m_nextIDs.begin();
    size_t ret = *iter;
    m_nextIDs.erase(iter);
    if (m_nextIDs.size() == 0) m_nextIDs.emplace(ret + 1);
    return ret;
}

void Tree::addToFrontCache(std::shared_ptr<Node> node) {
    node->setCacheNext(m_cacheHead);
    auto prevNode = m_cacheHead;
    if (m_cacheHead) {
        m_cacheHead->setCachePrev(node);
    }
    m_cacheHead = node;
}

void Tree::removeFromCache(std::shared_ptr<Node> node) {
    auto prevNode = node->getCachePrev();
    auto nextNode = node->getCacheNext();
    if (prevNode) {
        prevNode->setCacheNext(nextNode);
    }
    if (nextNode) {
        nextNode->setCachePrev(prevNode);
    }
    node->setCacheNext(nullptr);
    node->setCachePrev(nullptr);
}

void Tree::clear() {
    m_root = nullptr;
    m_cacheHead = nullptr;
    m_nodes.clear();
    m_nextIDs.clear();
    m_nextIDs.emplace(0);
}

void Tree::addNodeByParentID(int parentID) {
    if (parentID < 0) {
        clear();
        m_root = std::make_shared<Node>();
        m_root->setID(getSmallestID());
        m_nodes[0] = m_root;
        addToFrontCache(m_root);
    } else {
        auto parentNode = m_nodes[parentID];
        size_t newID = getSmallestID();
        auto prevNode = parentNode;
        auto currNode = parentNode->getFirst();
        while (currNode) {
            if (currNode->getID() > newID) break;
            prevNode = currNode;
            currNode = currNode->getNext();
        }
        std::shared_ptr<Node> newNode = std::make_shared<Node>();
        newNode->setID(newID);
        if (prevNode == parentNode) {
            prevNode->setFirst(newNode);
        } else {
            prevNode->setNext(newNode);
        }
        newNode->setPrev(prevNode);
        newNode->setNext(currNode);
        if (currNode) {
            currNode->setPrev(newNode);
        }
        m_nodes[newID] = newNode;
        addToFrontCache(newNode);
        
        while (parentNode) {
            parentNode->incrementCount(1);
            parentNode = getParentInternal(parentNode);
        }
    }
}

std::shared_ptr<Node> Tree::getParentInternal(std::shared_ptr<Node> node) {
    auto prevNode = node->getPrev();
    while (prevNode) {
        if (prevNode->getFirst() == node) return prevNode;
        node = prevNode;
        prevNode = prevNode->getPrev();
    }
    return nullptr;
}

void Tree::removeNodeByAddress(std::shared_ptr<Node> node, size_t& removed) {
    auto parentNode = getParentInternal(node);
    auto prevNode = node->getPrev();
    std::queue<std::shared_ptr<Node>> q;
    std::unordered_set<size_t> ids;
    q.emplace(node);
    while (!q.empty()) {
        auto currNode = q.front();
        q.pop();
        auto currChild = currNode->getFirst();
        ids.emplace(currNode->getID());
        while (currChild) {
            q.emplace(currChild);
            currChild = currChild->getNext();
        }
    }
    // removes all nodes
    removed = node->getCount();
    for (const auto& id : ids) {
        auto removedNode = m_nodes[id];
        removeFromCache(removedNode);
        m_nextIDs.emplace(id);
        m_nodes.erase(id);
    }
    auto nextNode = node->getNext();
    auto firstChild = prevNode->getFirst();
    if (firstChild == node) {
        prevNode->setFirst(nextNode);
    } else {
        prevNode->setNext(nextNode);
    }
    if (nextNode) {
        nextNode->setPrev(prevNode);
    }
    while (parentNode) {
        parentNode->decrementCount(removed);
        parentNode = getParentInternal(parentNode);;
    }
}

void Tree::removeNodeByID(size_t id, size_t& removed) {
    if (m_nodes.find(id) == m_nodes.end()) return;
    if (m_root->getID() == id) {
        removed = m_root->getCount();
        clear();
    } else {
        removeNodeByAddress(m_nodes[id], removed);
    }
}

std::vector<std::vector<size_t>> Tree::getAllIDs() const {
    std::vector<std::vector<size_t>> ret;
    if (m_root == nullptr) return ret;

    auto currNode = m_root;
    std::queue<std::pair<std::shared_ptr<Node>, size_t>> q;
    q.emplace(currNode, 0);
    while (!q.empty()) {
        auto currNode = q.front().first;
        size_t currDepth = q.front().second;
        q.pop();
        size_t currID = currNode->getID();

        auto currChild = currNode->getFirst();
        while (currChild) {
            q.emplace(currChild, currDepth + 1);
            currChild = currChild->getNext();
        }
        if (currDepth == ret.size()) {
            std::vector<size_t> newVec = {currID};
            ret.emplace_back(newVec);
        } else {
            ret.back().emplace_back(currID);
        }
    }
    return ret;
}

std::vector<size_t> Tree::getIDsByCreationTime() const {
    std::vector<size_t> ret;
    auto currNode = m_cacheHead;
    while (currNode) {
        ret.emplace_back(currNode->getID());
        currNode = currNode->getCacheNext();
    }
    return ret;
}

std::vector<size_t> Tree::getIDsWithDepth(size_t depth) const {
    std::vector<size_t> ret;
    if (m_root == nullptr) return ret;

    auto currNode = m_root;
    std::queue<std::pair<std::shared_ptr<Node>, size_t>> q;
    q.emplace(currNode, 0);
    while (!q.empty()) {
        auto currNode = q.front().first;
        size_t currDepth = q.front().second;
        q.pop();
        size_t currID = currNode->getID();
        if (currDepth == depth) {
            ret.emplace_back(currID);
        } else {
            auto currChild = currNode->getFirst();
            while (currChild) {
                q.emplace(currChild, currDepth + 1);
                currChild = currChild->getNext();
            }
        }
    }
    std::sort(ret.begin(), ret.end(), [](const size_t& s1, const size_t& s2) { return s1 < s2; });
    return ret;
}

std::shared_ptr<Node> Tree::getParent(std::shared_ptr<Node> n1, std::shared_ptr<Node> n2, std::unordered_set<std::shared_ptr<Node>>& uniqueNodes) const {
    if (n1 == nullptr) return nullptr;
    auto prevNode = n1->getPrev();
    uniqueNodes.emplace(n1);
    if (prevNode == n2) return prevNode;
    while (prevNode) {
        auto firstNode = prevNode->getFirst();
        uniqueNodes.emplace(prevNode);
        if (firstNode == n1) break;
        n1 = prevNode;
        prevNode = prevNode->getPrev();
        if (prevNode == n2) return prevNode;
    }
    return prevNode;
}

bool Tree::areCousins(size_t id1, size_t id2, size_t& count) const {
    return false;
}

size_t Tree::getCount(size_t id) const {
    auto node = m_nodes.find(id)->second;
    return node->getCount();
}
