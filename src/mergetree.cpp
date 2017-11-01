// vim:set et sw=2 ts=4 tw=72:
// Evan Wilde (C) 2017
//
// Usage: ./mergetree <database file> <master cid>

#include <iostream>
#include <vector>

#include "sqlite3.hpp"

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

}
