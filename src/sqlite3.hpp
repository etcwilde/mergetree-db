// vim:set et sw=2 ts=4 tw=72:
// Evan Wilde (C) 2017
// C++ Wrapper for sqlite databases

#ifndef SQLITE3_HPP
#define SQLITE3_HPP

#include <string>
#include <stdexcept>
#include <vector>

#include <sqlite3.h>

class SqliteDB{
public:
  SqliteDB(std::string const & fname);
  ~SqliteDB();

  std::vector<std::vector<std::string>> query(std::string const & query);

private:
  sqlite3 * mDb {nullptr};
};
#endif
