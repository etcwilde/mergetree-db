#include "sqlite3.hpp"

SqliteDB::SqliteDB(std::string const & fname) {
  int rc = sqlite3_open_v2(fname.c_str(), &mDb, SQLITE_OPEN_READWRITE, nullptr);
  if (rc) {
    auto msg {std::string("Database Error: ") + sqlite3_errmsg(mDb)};
    sqlite3_close(mDb);
    throw std::runtime_error(msg);
  }
}

SqliteDB::~SqliteDB() {

  for (auto stmt : _stmts) { sqlite3_finalize(stmt); }
  if (mDb != nullptr) sqlite3_close(mDb);
}

size_t SqliteDB::prepare(std::string const & query) {
  sqlite3_stmt * stmt;
  int rc = sqlite3_prepare_v2(mDb, query.c_str(), query.size(), &stmt, nullptr);
  if (rc != SQLITE_OK) throw std::runtime_error("Failed to prepare statement");

  size_t index = _stmts.size();
  _stmts.push_back(stmt);
  return index;
}

void SqliteDB::bind(size_t stmt_handle, int p, std::string const & text) {
  sqlite3_bind_text(_stmts.at(stmt_handle), p + 1, text.c_str(), text.size(), nullptr);
}

void SqliteDB::bind(size_t stmt_handle, int p, int32_t val) {
  sqlite3_bind_int(_stmts.at(stmt_handle), p + 1, val);
}

void SqliteDB::bind(size_t stmt_handle, int p, int64_t val) {
  sqlite3_bind_int64(_stmts.at(stmt_handle), p + 1, val);
}

void SqliteDB::bind(size_t stmt_handle, int p, double val) {
  sqlite3_bind_double(_stmts.at(stmt_handle), p + 1, val);
}

std::vector<std::vector<std::string>> SqliteDB::query(size_t stmt_handle) {
  std::vector<std::vector<std::string>> table;
  while (sqlite3_step(_stmts.at(stmt_handle)) == SQLITE_ROW) {
    int columns = sqlite3_column_count(_stmts.at(stmt_handle));
    std::vector<std::string> row(columns);
    for (size_t c {0}; c < columns; ++c) {
      row.at(c) = std::string(reinterpret_cast<char const *>(sqlite3_column_text(_stmts.at(stmt_handle), c)));
    }
    table.push_back(row);
  }

  sqlite3_reset(_stmts.at(stmt_handle));
  sqlite3_clear_bindings(_stmts.at(stmt_handle));
  return table;
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
