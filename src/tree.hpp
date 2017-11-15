// vim:set et sw=2 ts=4 tw=72:
// Evan Wilde (C) 2017

#ifndef TREE_HPP
#define TREE_HPP

#include <map>
#include <set>
#include <vector>

class Tree {
public:

  Tree(std::string const & root) : _root(root) {}

  void add(std::string const & from, std::string const & to);
  void add(std::string const & node);

  std::vector<std::string> const & getChildren(std::string const & node) const;
  std::string const & getParent(std::string const & node) const;

  inline std::string const & getRoot() const { return _root; }

  inline size_t nodes() const { return _children.size(); }

  inline bool contains(std::string const & node) const { return _children.count(node) == 0; }

private:

  std::string _root;

  std::map<std::string, std::pair<std::vector<std::string>, std::set<std::string>>> _children;
  std::map<std::string, std::string> _parent;
};


#endif /* ifndef TREE_HPP

 */
