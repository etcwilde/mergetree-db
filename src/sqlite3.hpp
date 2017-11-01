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

  // prepares only a single statement
  size_t prepare(std::string const & query);

  void bind(size_t stmt_handle, int p, std::string const & text);
  void bind(size_t stmt_handle, int p, int32_t value);
  void bind(size_t stmt_handle, int p, int64_t value);
  void bind(size_t stmt_handle, int p, double value);

  std::vector<std::vector<std::string>> query(size_t stmt_handle);
  std::vector<std::vector<std::string>> query(std::string const & query);

private:
  sqlite3 * mDb {nullptr};

  std::vector<sqlite3_stmt*> _stmts;

};
#endif
