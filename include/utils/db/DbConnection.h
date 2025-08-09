#pragma once
#include "DbException.h"
#include <cppconn/connection.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <memory>
#include <muduo/base/Logging.h>
#include <mutex>
#include <mysql/mysql.h>
#include <mysql_driver.h>
#include <string>

namespace ww_http {
namespace db

{

class DbConnection {

public:
  DbConnection(const std::string &host, const std::string &user,
               const std::string &password, const std::string &database);
  ~DbConnection();

  DbConnection(const DbConnection &) = delete;
  DbConnection &operator=(const DbConnection &) = delete;

  bool isValid();
  void reconnect();
  void cleanup();

  template <typename... Args>
  sql::ResultSet *executeQuery(const std::string &sql, Args &&... args) {
    std::lock_guard<std::mutex> lock(mutex_);
    try {
      std::unique_ptr<sql::PreparedStatement> stmt(
          conn_->prepareStatement(sql));
      bindParams(stmt.get(), 1, std::forward<Args>(args)...);
      return stmt->executeQuery();
    } catch (const sql::SQLException &e) {
      LOG_ERROR << "Query failed: " << e.what() << ", SQL: " << sql;
      throw DbException(e.what());
    }
  }

  template <typename... Args>
  int executeUpdate(const std::string &sql, Args &&... args) {
    std::lock_guard<std::mutex> lock(mutex_);
    try {
      // 直接创建新的预处理语句，不使用缓存
      std::unique_ptr<sql::PreparedStatement> stmt(
          conn_->prepareStatement(sql));
      bindParams(stmt.get(), 1, std::forward<Args>(args)...);
      return stmt->executeUpdate();
    } catch (const sql::SQLException &e) {
      LOG_ERROR << "Update failed: " << e.what() << ", SQL: " << sql;
      throw DbException(e.what());
    }
  }

  bool ping(); // 添加检测连接是否有效的方法

private:
  //辅助函数，递归终止条件
  void bindParems(sql::PreparedStatement *, int) {}

  template <typename T, typename... Args>
  void bindParems(sql::PreparedStatement *stmt, int index, T &&value,
                  Args &&... args) {
    stmt->setString(index, std::to_string(std::forward<T>(value)));
    bindParems(stmt, index + 1, std::forward<Args>(args)...);
  }

  // 特化 string 类型的参数绑定
  template <typename... Args>
  void bindParams(sql::PreparedStatement *stmt, int index,
                  const std::string &value, Args &&... args) {
    stmt->setString(index, value);
    bindParams(stmt, index + 1, std::forward<Args>(args)...);
  }

private:
  std::shared_ptr<sql::Connection> conn_;
  std::string host_;
  std::string user_;
  std::string password_;
  std::string database_;
  std::mutex mutex_;
};

} // namespace db

} // namespace ww_http
