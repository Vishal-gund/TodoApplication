#include "Database.h"

Database::Database(const std::string& host,
                   const std::string& user,
                   const std::string& password,
                   const std::string& database)
    : host_(host), user_(user), password_(password), database_(database) {}

std::unique_ptr<sql::Connection> Database::connect() const {
    sql::mysql::MySQL_Driver* driver = sql::mysql::get_mysql_driver_instance();
    std::unique_ptr<sql::Connection> connection(
        driver->connect(host_, user_, password_)
    );
    connection->setSchema(database_);
    return connection;
}
