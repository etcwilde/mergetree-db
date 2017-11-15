#include "tree.hpp"

void Tree::add(std::string const & from, std::string const & to) {
  if( _children.count(from) == 0) _children.insert({from, {{to}, {to}}});
  else if (std::get<1>(_children.at(from)).count(to) == 0) {
    std::get<0>(_children.at(from)).push_back(to);
    std::get<1>(_children.at(from)).insert(to);
  }
  _parent.insert({to, from});
}

void Tree::add(std::string const & from) {
  if (_children.count(from) == 0) _children.insert({from, {{}, {}}});
}

std::vector<std::string> const & Tree::getChildren(std::string const & node) const {
  return std::get<0>(_children.at(node));
}

std::string const & Tree::getParent(std::string const & node) const {
  return _parent.at(node);
}
