# C++ Todo Backend - No Web Framework

This project connects the supplied Todo frontend to MySQL through a C++ HTTP server.

## Stack

- C++17
- OOP
- STL
- Windows Winsock
- MySQL Connector/C++
- MySQL
- HTML/CSS/JavaScript

No Drogon, Crow, oat++, Boost.Beast, or other C++ web framework is used.

## 1. Database

Open MySQL Workbench or the MySQL command line and run:

    source backend/sql/database.sql;

Or paste the SQL manually.

## 2. Configure MySQL password

Open:

    backend/src/main.cpp

Change:

    const std::string DB_PASSWORD = "YOUR_MYSQL_PASSWORD";

Also change DB_USER if your MySQL user is not root.

## 3. Install MySQL Connector/C++

Install the MySQL Connector/C++ 8.x Windows package.

You need a directory similar to:

    C:/mysql-connector-c++/
        include/
        lib64/

The exact directory depends on the installed version.

## 4. Build

From the backend directory:

    cmake -S . -B build -DMYSQL_CONNECTOR_CPP_ROOT="C:/path/to/mysql-connector-c++"
    cmake --build build --config Release

If CMake cannot find mysqlcppconn8, check the Connector/C++ lib64 directory and library name for your installed version.

## 5. Runtime DLL

Windows also needs the MySQL Connector/C++ DLL available to the executable.

Copy the Connector/C++ runtime DLLs required by your installed version next to:

    build/Release/todo_backend.exe

or add the Connector/C++ bin directory to your Windows PATH.

## 6. Start backend

Run:

    build/Release/todo_backend.exe

Expected output:

    C++ Todo API running at http://127.0.0.1:8080

## 7. Start frontend

Open frontend/index.html using your IDE/browser.

The JavaScript sends requests to:

    http://localhost:8080/api/todos

## API

GET    /api/todos
GET    /api/todos/{id}
POST   /api/todos
PUT    /api/todos/{id}
DELETE /api/todos/{id}

## Example POST

    curl -X POST http://localhost:8080/api/todos ^
      -H "Content-Type: application/json" ^
      -d "{\"text\":\"Learn REST API\",\"isChecked\":false}"

## Example GET

    curl http://localhost:8080/api/todos

## Example PUT

    curl -X PUT http://localhost:8080/api/todos/1 ^
      -H "Content-Type: application/json" ^
      -d "{\"text\":\"Learn advanced C++\",\"isChecked\":true}"

## Example DELETE

    curl -X DELETE http://localhost:8080/api/todos/1

## Architecture

Frontend
  -> fetch()
  -> HTTP
  -> Windows Winsock C++ server
  -> HttpServer
  -> TodoService
  -> TodoRepository
  -> MySQL Connector/C++
  -> MySQL

## Important limitation

The HTTP and JSON implementation is intentionally small and educational. It is suitable for this local Todo project, not for production internet-facing use. A production server should use a mature HTTP server and JSON parser/framework.
