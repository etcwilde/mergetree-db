// vim:set et sw=2 ts=4 tw=72:
// Evan Wilde (C) 2017
//
// Usage: ./mergetree <database file> <master cid>

#include <iostream>
#include <vector>
#include <algorithm>

#include "sqlite3.hpp"
#include "graph.hpp"

DiGraph buildDAG(SqliteDB & db) {
  DiGraph dag;
  auto parents = db.query("SELECT cid, parent, idx FROM parents;");
  std::sort(parents.begin(), parents.end(), [](auto &left, auto & right) {
            return left.at(2) < right.at(2); });
  for (auto row : parents) { dag.add(row.at(0), row.at(1)); dag.add(row.at(1)); }
  return dag;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args(argv, argv + argc);

  if (args.size() < 2) {
    std::cerr << "Usage: " <<  args.at(0) << " <database file> [<HEAD hash>]" << std::endl;
    return EXIT_FAILURE;
  }
  SqliteDB db(args.at(1));

  if (args.size() == 2) {
    // Grab default HEAD
    args.push_back(db.query("SELECT cid FROM commits EXCEPT SELECT parent FROM parents;").at(0).at(0));
  }
  std::cout << "Database: " << args.at(1) << " HEAD: " << args.at(2) << std::endl;

  std::vector<DiGraph> trees;
  std::set<std::string> masterMerges = computeInMaster(args.at(2), db);

  std::cout << masterMerges.size() << " trees to compute" << std::endl;
  trees.reserve(masterMerges.size());

  {
    std::unordered_map<std::string, unsigned int> depths;
    auto DAG = buildDAG(db);
  }
}
