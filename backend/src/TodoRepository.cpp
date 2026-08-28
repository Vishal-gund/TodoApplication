#include "TodoRepository.h"
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <stdexcept>

TodoRepository::TodoRepository(const Database& db) : db_(db) {}

Todo TodoRepository::create(const Todo& todo) {
    auto conn = db_.connect();
    std::unique_ptr<sql::PreparedStatement> stmt(
        conn->prepareStatement("INSERT INTO todos (text, is_checked) VALUES (?, ?)")
    );
    stmt->setString(1, todo.text);
    stmt->setBoolean(2, todo.isChecked);
    stmt->executeUpdate();

    Todo result = todo;
    std::unique_ptr<sql::Statement> idStmt(conn->createStatement());
    std::unique_ptr<sql::ResultSet> rs(idStmt->executeQuery("SELECT LAST_INSERT_ID() AS id"));
    if (rs->next()) result.id = rs->getInt("id");
    return result;
}

std::vector<Todo> TodoRepository::getAll() {
    auto conn = db_.connect();
    std::unique_ptr<sql::PreparedStatement> stmt(
        conn->prepareStatement("SELECT id, text, is_checked FROM todos ORDER BY id")
    );
    std::unique_ptr<sql::ResultSet> rs(stmt->executeQuery());

    std::vector<Todo> result;
    while (rs->next()) {
        Todo todo;
        todo.id = rs->getInt("id");
        todo.text = rs->getString("text");
        todo.isChecked = rs->getBoolean("is_checked");
        result.push_back(todo);
    }
    return result;
}

std::optional<Todo> TodoRepository::getById(int id) {
    auto conn = db_.connect();
    std::unique_ptr<sql::PreparedStatement> stmt(
        conn->prepareStatement("SELECT id, text, is_checked FROM todos WHERE id = ?")
    );
    stmt->setInt(1, id);
    std::unique_ptr<sql::ResultSet> rs(stmt->executeQuery());

    if (!rs->next()) return std::nullopt;

    Todo todo;
    todo.id = rs->getInt("id");
    todo.text = rs->getString("text");
    todo.isChecked = rs->getBoolean("is_checked");
    return todo;
}

std::optional<Todo> TodoRepository::update(int id, const Todo& todo) {
    auto conn = db_.connect();
    std::unique_ptr<sql::PreparedStatement> stmt(
        conn->prepareStatement("UPDATE todos SET text = ?, is_checked = ? WHERE id = ?")
    );
    stmt->setString(1, todo.text);
    stmt->setBoolean(2, todo.isChecked);
    stmt->setInt(3, id);

    if (stmt->executeUpdate() == 0) return std::nullopt;

    Todo result = todo;
    result.id = id;
    return result;
}

bool TodoRepository::remove(int id) {
    auto conn = db_.connect();
    std::unique_ptr<sql::PreparedStatement> stmt(
        conn->prepareStatement("DELETE FROM todos WHERE id = ?")
    );
    stmt->setInt(1, id);
    return stmt->executeUpdate() > 0;
}
