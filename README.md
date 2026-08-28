# Todo Application – Task Management System

A full-stack **Todo Application** designed to help users efficiently create, manage, update, and delete tasks.

The project uses a **C++ backend** with Object-Oriented Programming, STL, and algorithms, along with **MySQL** for database management and a simple **HTML, CSS, and JavaScript** frontend.

## 🚀 Features

- Create new tasks
- View all tasks
- Update existing tasks
- Delete tasks
- Mark tasks as completed
- Store tasks persistently in MySQL
- Backend developed using C++
- Object-Oriented Programming implementation
- Efficient data handling using STL
- REST-style communication between frontend and backend

## 🛠️ Technologies Used

### Frontend
- HTML5
- CSS3
- JavaScript

### Backend
- C++
- Object-Oriented Programming (OOP)
- STL (Standard Template Library)
- Data Structures & Algorithms

### Database
- MySQL
- MySQL Connector/C++

## 🏗️ Project Architecture

```text
Todo Application
│
├── Frontend
│   ├── HTML
│   ├── CSS
│   └── JavaScript
│
├── Backend
│   └── C++
│       ├── OOP
│       ├── STL
│       └── Algorithms
│
└── Database
    └── MySQL
```

## 📂 Project Structure

```text
Todo-Application/
│
├── frontend/
│   ├── index.html
│   ├── style.css
│   └── script.js
│
├── backend/
│   ├── main.cpp
│   ├── Todo.cpp
│   ├── Todo.h
│   └── ...
│
├── database/
│   └── database.sql
│
└── README.md
```

> The exact file structure may vary depending on the implementation.

## ⚙️ How It Works

1. The user interacts with the Todo application through the frontend.
2. JavaScript sends requests to the C++ backend.
3. The C++ backend processes the request using OOP, STL, and algorithms.
4. The backend communicates with MySQL to store or retrieve task data.
5. The result is returned to the frontend and displayed to the user.

## 🗄️ Database

The application uses **MySQL** to store task information.

Example task data:

```text
Task ID
Task Title
Task Description
Task Status
Created Date
```

## 💡 OOP Concepts Used

The C++ backend demonstrates important Object-Oriented Programming concepts such as:

- Classes and Objects
- Encapsulation
- Inheritance
- Polymorphism
- Abstraction
- Constructors
- Member Functions

## 📚 STL & Algorithms

The project uses C++ STL and algorithmic concepts for efficient data management.

Examples include:

- `vector`
- `string`
- `map`
- Iterators
- Searching
- Sorting
- Data manipulation

## 🔧 Requirements

Before running the project, make sure you have:

- C++ Compiler
- MySQL Server
- MySQL Connector/C++
- Web Browser
- C++ development environment such as Visual Studio

## ▶️ Setup

### 1. Clone the Repository

```bash
git clone https://github.com/your-username/Todo-Application.git
cd Todo-Application
```

### 2. Configure MySQL

Create the required database and tables using the SQL file:

```text
database/database.sql
```

Update the database connection details in the C++ backend according to your MySQL configuration.

### 3. Build the C++ Backend

Compile the backend using your configured C++ compiler and MySQL Connector/C++.

### 4. Start the Backend

Run the generated backend executable.

### 5. Run the Frontend

Open:

```text
frontend/index.html
```

in your browser.

## 🎯 Learning Objectives

This project was developed to gain practical experience with:

- Full-stack application development
- C++ backend development
- Object-Oriented Programming
- STL and Data Structures
- Algorithms
- MySQL database integration
- Frontend and backend communication
- CRUD operations
- API-based application architecture

## 🔮 Future Improvements

Possible future improvements include:

- User authentication and registration
- Task priorities
- Task categories
- Due dates and reminders
- Search and filtering
- Pagination
- Responsive UI improvements
- JWT-based authentication
- Deployment to a cloud server

## 👨‍💻 Author

**Vishal Patil**

Built as a practical project to demonstrate **C++, OOP, STL, Algorithms, MySQL, HTML, CSS, and JavaScript** skills.
