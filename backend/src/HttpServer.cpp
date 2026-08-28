#include "HttpServer.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <cctype>

#pragma comment(lib, "Ws2_32.lib")

HttpServer::HttpServer(TodoService& service) : service_(service) {}

std::string HttpServer::jsonEscape(const std::string& value) {
    std::string out;
    for (char c : value) {
        switch (c) {
            case '"': out += "\\\""; break;
            case '\\': out += "\\\\"; break;
            case '\n': out += "\\n"; break;
            case '\r': out += "\\r"; break;
            case '\t': out += "\\t"; break;
            default: out += c;
        }
    }
    return out;
}

std::string HttpServer::todoToJson(const Todo& todo) {
    return "{\"id\":" + std::to_string(todo.id) +
           ",\"text\":\"" + jsonEscape(todo.text) +
           "\",\"isChecked\":" + (todo.isChecked ? "true" : "false") + "}";
}

std::string HttpServer::todosToJson(const std::vector<Todo>& todos) {
    std::string out = "[";
    for (size_t i = 0; i < todos.size(); ++i) {
        if (i) out += ",";
        out += todoToJson(todos[i]);
    }
    return out + "]";
}

std::string HttpServer::getJsonString(const std::string& body, const std::string& key) {
    const std::string marker = "\"" + key + "\"";
    size_t p = body.find(marker);
    if (p == std::string::npos) throw std::invalid_argument("Missing field: " + key);
    p = body.find(':', p + marker.size());
    if (p == std::string::npos) throw std::invalid_argument("Invalid JSON");
    p = body.find('"', p + 1);
    if (p == std::string::npos) throw std::invalid_argument("Field must be a string: " + key);

    std::string result;
    bool escaped = false;
    for (++p; p < body.size(); ++p) {
        char c = body[p];
        if (escaped) {
            switch (c) {
                case '"': result += '"'; break;
                case '\\': result += '\\'; break;
                case 'n': result += '\n'; break;
                case 'r': result += '\r'; break;
                case 't': result += '\t'; break;
                default: result += c; break;
            }
            escaped = false;
        } else if (c == '\\') {
            escaped = true;
        } else if (c == '"') {
            return result;
        } else {
            result += c;
        }
    }
    throw std::invalid_argument("Unterminated JSON string");
}

bool HttpServer::getJsonBool(const std::string& body, const std::string& key, bool defaultValue) {
    const std::string marker = "\"" + key + "\"";
    size_t p = body.find(marker);
    if (p == std::string::npos) return defaultValue;
    p = body.find(':', p + marker.size());
    if (p == std::string::npos) throw std::invalid_argument("Invalid JSON");
    p++;
    while (p < body.size() && std::isspace(static_cast<unsigned char>(body[p]))) ++p;
    if (body.compare(p, 4, "true") == 0) return true;
    if (body.compare(p, 5, "false") == 0) return false;
    throw std::invalid_argument("Field must be boolean: " + key);
}

HttpServer::Request HttpServer::parseRequest(const std::string& raw) {
    size_t headerEnd = raw.find("\r\n\r\n");
    if (headerEnd == std::string::npos) throw std::invalid_argument("Malformed HTTP request");

    std::istringstream firstLine(raw.substr(0, raw.find("\r\n")));
    Request req;
    firstLine >> req.method >> req.path;
    req.body = raw.substr(headerEnd + 4);
    return req;
}

int HttpServer::idFromPath(const std::string& path) {
    size_t slash = path.find_last_of('/');
    if (slash == std::string::npos || slash + 1 >= path.size())
        throw std::invalid_argument("Invalid id");
    int id = std::stoi(path.substr(slash + 1));
    return id;
}

std::string HttpServer::makeResponse(int status, const std::string& body) const {
    std::string statusText = "OK";
    if (status == 201) statusText = "Created";
    else if (status == 400) statusText = "Bad Request";
    else if (status == 404) statusText = "Not Found";
    else if (status == 405) statusText = "Method Not Allowed";
    else if (status == 500) statusText = "Internal Server Error";

    return "HTTP/1.1 " + std::to_string(status) + " " + statusText + "\r\n"
           "Content-Type: application/json; charset=utf-8\r\n"
           "Access-Control-Allow-Origin: *\r\n"
           "Access-Control-Allow-Methods: GET, POST, PUT, DELETE, OPTIONS\r\n"
           "Access-Control-Allow-Headers: Content-Type\r\n"
           "Connection: close\r\n"
           "Content-Length: " + std::to_string(body.size()) + "\r\n\r\n" + body;
}

std::string HttpServer::route(const Request& request) {
    try {
        if (request.method == "OPTIONS")
            return makeResponse(204, "{}");

        if (request.path == "/api/todos" && request.method == "GET")
            return makeResponse(200, todosToJson(service_.getAll()));

        if (request.path == "/api/todos" && request.method == "POST") {
            std::string text = getJsonString(request.body, "text");
            bool checked = getJsonBool(request.body, "isChecked", false);
            return makeResponse(201, todoToJson(service_.create(text, checked)));
        }

        if (request.path.rfind("/api/todos/", 0) == 0) {
            int id = idFromPath(request.path);

            if (request.method == "GET") {
                auto todo = service_.getById(id);
                if (!todo) return makeResponse(404, "{\"error\":\"Todo not found\"}");
                return makeResponse(200, todoToJson(*todo));
            }

            if (request.method == "PUT") {
                std::string text = getJsonString(request.body, "text");
                bool checked = getJsonBool(request.body, "isChecked", false);
                auto todo = service_.update(id, text, checked);
                if (!todo) return makeResponse(404, "{\"error\":\"Todo not found\"}");
                return makeResponse(200, todoToJson(*todo));
            }

            if (request.method == "DELETE") {
                if (!service_.remove(id))
                    return makeResponse(404, "{\"error\":\"Todo not found\"}");
                return makeResponse(200, "{\"message\":\"Todo deleted\"}");
            }
        }

        return makeResponse(405, "{\"error\":\"Method or endpoint not supported\"}");
    } catch (const std::invalid_argument& e) {
        return makeResponse(400, std::string("{\"error\":\"") + jsonEscape(e.what()) + "\"}");
    } catch (const std::exception& e) {
        return makeResponse(500, std::string("{\"error\":\"") + jsonEscape(e.what()) + "\"}");
    }
}

void HttpServer::run(const std::string& host, int port) {
    WSADATA wsa{};
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        throw std::runtime_error("WSAStartup failed");

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET) {
        WSACleanup();
        throw std::runtime_error("Unable to create socket");
    }

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_port = htons(static_cast<u_short>(port));
    inet_pton(AF_INET, host.c_str(), &address.sin_addr);

    int reuse = 1;
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR,
               reinterpret_cast<const char*>(&reuse), sizeof(reuse));

    if (bind(serverSocket, reinterpret_cast<sockaddr*>(&address), sizeof(address)) == SOCKET_ERROR ||
        listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        closesocket(serverSocket);
        WSACleanup();
        throw std::runtime_error("Unable to bind/listen on port " + std::to_string(port));
    }

    std::cout << "C++ Todo API running at http://" << host << ":" << port << "\n";

    while (true) {
        SOCKET client = accept(serverSocket, nullptr, nullptr);
        if (client == INVALID_SOCKET) continue;

        std::string raw;
        char buffer[8192];
        int received;
        do {
            received = recv(client, buffer, sizeof(buffer), 0);
            if (received > 0) raw.append(buffer, received);
        } while (received == sizeof(buffer));

        try {
            Request request = parseRequest(raw);
            std::string response = route(request);
            send(client, response.c_str(), static_cast<int>(response.size()), 0);
        } catch (...) {
            std::string response = makeResponse(400, "{\"error\":\"Malformed HTTP request\"}");
            send(client, response.c_str(), static_cast<int>(response.size()), 0);
        }

        closesocket(client);
    }
}
