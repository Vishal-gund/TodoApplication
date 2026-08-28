#include "TodoService.h"
#include <stdexcept>
#include <cctype>

TodoService::TodoService(TodoRepository& repository) : repository_(repository) {}

void TodoService::validateText(const std::string& text) {
    if (text.empty()) throw std::invalid_argument("Todo text cannot be empty");
    if (text.size() > 255) throw std::invalid_argument("Todo text cannot exceed 255 characters");
}

Todo TodoService::create(const std::string& text, bool isChecked) {
    validateText(text);
    return repository_.create(Todo{0, text, isChecked});
}

std::vector<Todo> TodoService::getAll() {
    return repository_.getAll();
}

std::optional<Todo> TodoService::getById(int id) {
    if (id <= 0) throw std::invalid_argument("Invalid todo id");
    return repository_.getById(id);
}

std::optional<Todo> TodoService::update(int id, const std::string& text, bool isChecked) {
    if (id <= 0) throw std::invalid_argument("Invalid todo id");
    validateText(text);
    return repository_.update(id, Todo{0, text, isChecked});
}

bool TodoService::remove(int id) {
    if (id <= 0) throw std::invalid_argument("Invalid todo id");
    return repository_.remove(id);
}
