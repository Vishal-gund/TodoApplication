#pragma once
#include "TodoService.h"
#include <string>

class HttpServer {
public:
    explicit HttpServer(TodoService& service);
    void run(const std::string& host, int port);

private:
    TodoService& service_;

    struct Request {
        std::string method;
        std::string path;
        std::string body;
    };

    Request parseRequest(const std::string& raw);
    std::string route(const Request& request);
    std::string makeResponse(int status, const std::string& body) const;

    static std::string jsonEscape(const std::string& value);
    static std::string todoToJson(const Todo& todo);
    static std::string todosToJson(const std::vector<Todo>& todos);
    static std::string getJsonString(const std::string& body, const std::string& key);
    static bool getJsonBool(const std::string& body, const std::string& key, bool defaultValue);
    static int idFromPath(const std::string& path);
};
