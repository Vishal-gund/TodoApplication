#pragma once
#include <memory>
#include <string>
#include <mysql_driver.h>
#include <mysql_connection.h>

class Database {
public:
    Database(const std::string& host,
             const std::string& user,
             const std::string& password,
             const std::string& database);

    std::unique_ptr<sql::Connection> connect() const;

private:
    std::string host_;
    std::string user_;
    std::string password_;
    std::string database_;
};
