#include "tree.hpp"
#include "tree_wrapper.hpp"

TreeWrapper::TreeWrapper() {
    m_tree = std::make_shared<Tree>();
}

void TreeWrapper::addNodeWrapper(int id) {
    m_tree->addNodeByParentID(id);
}
void TreeWrapper::removeNodeWrapper(size_t id, size_t& removed) {
    m_tree->removeNodeByID(id, removed);
}
std::vector<std::vector<size_t>> TreeWrapper::getIDsWrapper() const {
    return m_tree->getAllIDs();
}

std::vector<size_t> TreeWrapper::getIDsByCreationTimeWrapper() const {
    return m_tree->getIDsByCreationTime();
}

std::vector<size_t> TreeWrapper::getIDsByDepthWrapper(size_t depth) const {
    return m_tree->getIDsWithDepth(depth);
}

bool TreeWrapper::validateCousin(size_t id1, size_t id2, size_t& count) const {
    return m_tree->areCousins(id1, id2, count);
}

size_t TreeWrapper::getCountWrapper(size_t id) const {
    return m_tree->getCount(id);
}
