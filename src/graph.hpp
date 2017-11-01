// vim:set et sw=2 ts=4 tw=72:
// Evan Wilde (C) 2017

#ifndef MERGE_TREE_HPP
#define MERGE_TREE_HPP

#include <iterator>
#include <unordered_map>
#include <set>
#include <vector>

#include "sqlite3.hpp"


/**
 * Find the nodes that are on on the same branch as the HEAD node
 *
 * HEAD: the hash of the most recent commit on the master branch
 * db: A reference to the database storing all commits
 *
 * returns: set of hashes that are along the master branch.
 */
std::set<std::string> computeInMaster(std::string const & HEAD, SqliteDB & db);

class DiGraph {
public:
  DiGraph () {}

  /**
   * Add edge between from and to nodes
   *
   * from: hash of the starting commit (child)
   * to:   hash of the ending commit (parent)
   */
  void add(std::string const & from, std::string const & to);

  /**
   * Add node to graph
   *
   * node: empty node with no parents ☹
   */
  void add(std::string const & node);

  /**
   * Gets the parents (to nodes) of a given node (hash)
   *
   * node: Hash of originating commit
   *
   * returns: Vector of parent node hashes (ordered)
   */
  std::vector<std::string> const & getParents(std::string const & node) const;

  bool contains(std::string const & node) const;

  /**
   * Get the N'th parent
   *
   * Template param: N, the position of the parent in the list of
   * parents
   *
   * node: Hash of the originating node
   *
   * returns: The hash of the N'th parent
   */
  template <size_t N=0>
  std::string const & getParent(std::string const & node) const;

  /**
   * Get first parent, this is the parent that is on the same branch as
   * the node.
   *
   * Same as getParent<0>(node)
   *
   * node: hash of the originating node
   *
   * returns: hash of the first parent.
   */
  std::string const & getFirstParent(std::string const & node) const;


  inline size_t nodes() const { return _edges.size(); }
private:
  std::unordered_map<std::string, std::pair<std::vector<std::string>, std::set<std::string>>> _edges;
};
#endif
