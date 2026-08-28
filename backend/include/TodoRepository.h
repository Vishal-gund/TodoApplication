#pragma once
#include "Todo.h"
#include "Database.h"
#include <vector>
#include <optional>

class TodoRepository {
public:
    explicit TodoRepository(const Database& db);

    Todo create(const Todo& todo);
    std::vector<Todo> getAll();
    std::optional<Todo> getById(int id);
    std::optional<Todo> update(int id, const Todo& todo);
    bool remove(int id);

private:
    const Database& db_;
};
