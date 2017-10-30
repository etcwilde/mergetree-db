// vim:set et sw=2 ts=4 tw=72:
// Evan Wilde (C) 2017
// C++ Wrapper for sqlite databases

#ifndef SQLITE3_HPP
#define SQLITE3_HPP

#include <string>
#include <stdexcept>

#include <sqlite3.h>

class SqliteDB{
public:
  SqliteDB(std::string const & fname);
  ~SqliteDB();

  std::vector<std::vector<std::string>> query(std::string const & query);

private:
  sqlite3 * mDb {nullptr};
};

SqliteDB::SqliteDB(std::string const & fname) {
  int rc = sqlite3_open_v2(fname.c_str(), &mDb, SQLITE_OPEN_READWRITE, nullptr);
  if (rc) {
    auto msg {std::string("Database Error: ") + sqlite3_errmsg(mDb)};
    sqlite3_close(mDb);
    throw std::runtime_error(msg);
  }
}

SqliteDB::~SqliteDB() {
  if (mDb != nullptr) sqlite3_close(mDb);
}

std::vector<std::vector<std::string>> SqliteDB::query(std::string const & query) {
    char* errMsg = nullptr;
    std::vector<std::vector<std::string>> table;
    auto callback = [](void *table_ptr, int argc, char ** argv, char**) {
      std::vector<std::vector<std::string>> & table = *static_cast<std::vector<std::vector<std::string>> *>(table_ptr);
      std::vector<std::string> row(argc);
      for (int i = 0; i < argc; ++i) { row.at(i) = argv[i] ? argv[i] : "NULL"; }
      table.push_back(row);
      return 0;
    };

    int rc = sqlite3_exec(mDb, query.c_str(), callback, &table, &errMsg);
    if (rc != SQLITE_OK) {
      std::string msg(errMsg);
      sqlite3_free(errMsg);
      throw std::runtime_error("Database Error: " + msg);
    }
    return table;
  }
#endif
