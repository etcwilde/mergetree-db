#include "graph.hpp"

std::set<std::string> computeInMaster(std::string const & HEAD, SqliteDB & db) {

  std::set<std::string> master_commits;
  auto main_parents {db.query("SELECT cid, parent FROM parents WHERE idx = 0;")};

  size_t i {0};
  try {
    for (; HEAD != main_parents.at(i).at(0); ++i) { }
  } catch (std::out_of_range const &) {
    throw std::runtime_error("Commit not found");
  }

  std::string current, next;
  for (;i < main_parents.size();) {
    current = main_parents.at(i).at(0);
    next = main_parents.at(i).at(1);
    master_commits.insert(current);
    while (i < main_parents.size() && main_parents.at(i).at(0) != next)  { ++i; }
  }
  return master_commits;
}

/*
 * DiGraph Implementations
 */

void DiGraph::add(std::string const & from, std::string const & to) {
  if (_edges.count(from) == 0) _edges.insert({from, {{to}, {to}}});
  else if (std::get<1>(_edges.at(from)).count(to) == 0) {
    std::get<0>(_edges.at(from)).push_back(to);
    std::get<1>(_edges.at(from)).insert(to);
  }
}

void DiGraph::add(std::string const & from) {
  if (_edges.count(from) == 0) _edges.insert({from, {{}, {}}});
}

std::vector<std::string> const & DiGraph::getParents(std::string const & node) const {
  return std::get<0>(_edges.at(node));
}

bool DiGraph::contains(std::string const & node) const {
  return _edges.count(node) != 0;
}

template <size_t N>
std::string const & DiGraph::getParent(std::string const & node) const {
  return std::get<0>(_edges.at(node)).at(N);
}

std::string const & DiGraph::getFirstParent(std::string const & node) const {
  return getParent<0>(node);
}
