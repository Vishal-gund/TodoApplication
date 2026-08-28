#pragma once
#include "TodoRepository.h"
#include <vector>
#include <optional>

class TodoService {
public:
    explicit TodoService(TodoRepository& repository);

    Todo create(const std::string& text, bool isChecked);
    std::vector<Todo> getAll();
    std::optional<Todo> getById(int id);
    std::optional<Todo> update(int id, const std::string& text, bool isChecked);
    bool remove(int id);

private:
    TodoRepository& repository_;
    static void validateText(const std::string& text);
};
