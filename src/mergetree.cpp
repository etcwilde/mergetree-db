// vim:set et sw=2 ts=4 tw=72:
// Evan Wilde (C) 2017
//
// Usage: ./mergetree <database file> <master cid>

#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <unordered_map>
#include <set>

#include "sqlite3.hpp"
#include "graph.hpp"
#include "tree.hpp"

DiGraph buildDAG(SqliteDB & db) {
  DiGraph dag;
  auto parents = db.query("SELECT cid, parent, idx FROM parents;");
  std::sort(parents.begin(), parents.end(), [](auto &left, auto & right) {
            return left.at(2) < right.at(2); });
  for (auto row : parents) { dag.add(row.at(0), row.at(1)); dag.add(row.at(1)); }
  return dag;
}

DiGraph Phase1(DiGraph const & dag, std::string const & HEAD, std::unordered_map<std::string, unsigned int> & depthMap) {
  DiGraph tree;
  std::queue<std::string> Q;
  int openBranches {0};
  std::set<std::string> visited;

  tree.add(HEAD);
  Q.push(HEAD);
  visited.insert(HEAD);
  depthMap.insert({HEAD, 0});

  do {
    std::string cur;
    std::vector<std::string> parents;
    cur = Q.front(); Q.pop();

    parents = dag.getParents(cur);
    openBranches = parents.empty() ? openBranches : openBranches + parents.size() - 1;
    for (size_t index {0}; index < parents.size(); ++index) {
      auto parent = parents.at(index);
      if (depthMap.count(parent) == 0) {
        depthMap.insert({parent, depthMap.at(cur) + index});
      } else if (depthMap.at(cur) > depthMap.at(parent)) {
        openBranches -= 1;
      } else if (depthMap.at(cur) < depthMap.at(parent)) {
        openBranches -= 1;
        depthMap.at(parent) = depthMap.at(cur);
      }
      tree.add(parent, cur);
      if (visited.count(parent) == 0) {
        Q.push(parent);
        visited.insert(parent);
      }
    }
  } while (Q.size() && openBranches != 0);
  return tree;
}

// dag: original DAG
// idag: inverted DAG
// HEAD: Root of the tree
// Depths: Mapping between node and depth
Tree Phase2(DiGraph const & dag, DiGraph const & idag, std::string const & HEAD, std::unordered_map<std::string, unsigned int> depths) {

  auto getRealParent = [&depths, &idag](std::string const & p) -> std::string{
      auto comparisonFunc = [&depths](std::string const & parent, std::string const & child) { return depths.at(parent) > depths.at(child); };
      auto predFunc = [&depths, &p](std::string const & child) { return depths.at(p) >= depths.at(child); };
      auto const & childrenList = idag.getParents(p); // Get parents

      std::vector<std::string> filteredChildrenList;
      filteredChildrenList.reserve(childrenList.size()); // filter children
      std::copy_if(childrenList.begin(), childrenList.end(), std::back_inserter(filteredChildrenList), predFunc);

      if (filteredChildrenList.empty()) return "";
      else return *std::max_element(filteredChildrenList.begin(), filteredChildrenList.end(), comparisonFunc);
  };

  Tree tree(HEAD);
  std::queue<std::string> Q;
  tree.add(HEAD);
  auto parentList = std::vector<std::string>(dag.getParents(HEAD));
  parentList.erase(parentList.begin());
  for (auto parent : parentList) {
    auto realParent = getRealParent(parent);
    if (realParent == HEAD) {
      tree.add(HEAD, parent);
      tree.add(parent);
      Q.push(parent);
    }
  }
  while (Q.size()) {
    auto cur = Q.front(); Q.pop();
    parentList = dag.getParents(cur);
    for (auto parent : parentList) {
      auto realParent = getRealParent(parent);
      if (realParent == cur) {
        tree.add(cur, parent);
        tree.add(parent);
        Q.push(parent);
      }
    }
  }
  return tree;
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

  std::vector<Tree> trees;
  std::set<std::string> masterMerges { computeInMaster(args.at(2), db)};

  std::cout << masterMerges.size() << " trees to compute" << std::endl;
  trees.reserve(masterMerges.size());

  {
    auto DAG = buildDAG(db);
    std::cout << "DAG nodes: " << DAG.nodes() << std::endl;

    for (auto m : masterMerges) {
      std::unordered_map<std::string, unsigned int> depths;
      auto invertedDAG = Phase1(DAG, m, depths);
      trees.push_back(Phase2(DAG, invertedDAG, m, depths));
      std::cout << "Trees Computed: " << trees.size() << '/' << masterMerges.size() << '\r' << std::flush;
    }
    std::cout << std::endl;
  }

  std::cout << "Trees computed: " << trees.size() << std::endl;

}
