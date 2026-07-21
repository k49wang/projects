#ifndef __TREE_WRAPPER_HPP__
#define __TREE_WRAPPER_HPP__

#include <memory>
#include <vector>
class Tree;

class TreeWrapper {
    std::shared_ptr<Tree> m_tree;
public:
    TreeWrapper();

    void addNodeWrapper(int id);
    void removeNodeWrapper(size_t id, size_t& removed);
    std::vector<std::vector<size_t>> getIDsWrapper() const;
    std::vector<size_t> getIDsByCreationTimeWrapper() const;
    std::vector<size_t> getIDsByDepthWrapper(size_t depth) const;
    bool validateCousin(size_t id1, size_t id2, size_t& count) const;
    size_t getCountWrapper(size_t id) const;
};

#endif
