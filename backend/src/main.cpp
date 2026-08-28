#include "Database.h"
#include "TodoRepository.h"
#include "TodoService.h"
#include "HttpServer.h"

#include <iostream>
#include <string>
#include <exception>

int main() {
    try {
        const std::string DB_HOST = "tcp://127.0.0.1:3306";
        const std::string DB_USER = "root";
        const std::string DB_PASSWORD = "root";
        const std::string DB_NAME = "TodoApplication";

        Database database(
            DB_HOST,
            DB_USER,
            DB_PASSWORD,
            DB_NAME
        );

        TodoRepository repository(database);
        TodoService service(repository);
        HttpServer server(service);

        server.run("127.0.0.1", 8080);

    } catch (const std::exception& e) {
        std::cerr << "Fatal error: "
                  << e.what()
                  << std::endl;

        return 1;
    }

    return 0;
}