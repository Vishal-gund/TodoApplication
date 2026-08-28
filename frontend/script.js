const API_URL = "http://localhost:8080/api/todos";

let todoItemsContainerEl = document.getElementById("todoItemsContainer");
let addTodoBtn = document.getElementById("addTodoBtn");
let saveTodoEl = document.getElementById("saveTodo");
let userInputEl = document.getElementById("userInput");
let messageEl = document.getElementById("message");

let todoList = [];

function showMessage(text, isError = false) {
    messageEl.textContent = text;
    messageEl.style.color = isError ? "#d11a2a" : "#198754";
}

function escapeHtml(text) {
    const div = document.createElement("div");
    div.textContent = text;
    return div.innerHTML;
}

async function loadTodos() {
    try {
        const response = await fetch(API_URL);
        const data = await response.json();
        if (!response.ok) throw new Error(data.error || "Unable to load todos");
        todoList = data;
        renderTodos();
        showMessage("Todos loaded from MySQL");
    } catch (error) {
        showMessage(error.message, true);
    }
}

function renderTodos() {
    todoItemsContainerEl.innerHTML = "";
    for (const todo of todoList) createAndAppendTodo(todo);
}

async function onChangeTodoStatus(todoId) {
    const todo = todoList.find(t => t.id === todoId);
    if (!todo) return;

    const oldValue = todo.isChecked;
    todo.isChecked = !todo.isChecked;

    try {
        const response = await fetch(`${API_URL}/${todoId}`, {
            method: "PUT",
            headers: {"Content-Type": "application/json"},
            body: JSON.stringify({text: todo.text, isChecked: todo.isChecked})
        });
        const data = await response.json();
        if (!response.ok) throw new Error(data.error || "Update failed");
        todoList = todoList.map(t => t.id === todoId ? data : t);
        renderTodos();
    } catch (error) {
        todo.isChecked = oldValue;
        renderTodos();
        showMessage(error.message, true);
    }
}

async function onDeleteTodo(todoId) {
    try {
        const response = await fetch(`${API_URL}/${todoId}`, {method: "DELETE"});
        const data = await response.json();
        if (!response.ok) throw new Error(data.error || "Delete failed");
        todoList = todoList.filter(t => t.id !== todoId);
        renderTodos();
        showMessage("Todo deleted");
    } catch (error) {
        showMessage(error.message, true);
    }
}

function createAndAppendTodo(todo) {
    const todoContainer = document.createElement("div");
    todoContainer.id = "todo" + todo.id;
    todoContainer.classList.add("todo-item-container");

    const checkBox = document.createElement("input");
    checkBox.type = "checkbox";
    checkBox.classList.add("CheckBox");
    checkBox.checked = todo.isChecked;
    checkBox.onclick = () => onChangeTodoStatus(todo.id);
    todoContainer.appendChild(checkBox);

    const labelContainer = document.createElement("div");
    labelContainer.classList.add("label-conatner");

    const label = document.createElement("label");
    label.textContent = todo.text;
    label.style.fontSize = "20px";
    if (todo.isChecked) label.classList.add("checked");
    labelContainer.appendChild(label);

    const deleteIcon = document.createElement("i");
    deleteIcon.classList.add("far", "fa-trash-alt", "delete-icon", "delete-button");
    deleteIcon.onclick = () => onDeleteTodo(todo.id);
    labelContainer.appendChild(deleteIcon);

    todoContainer.appendChild(labelContainer);
    todoItemsContainerEl.appendChild(todoContainer);
}

async function onAddTodo() {
    const text = userInputEl.value.trim();
    if (text === "") {
        alert("Enter valid task");
        return;
    }

    try {
        const response = await fetch(API_URL, {
            method: "POST",
            headers: {"Content-Type": "application/json"},
            body: JSON.stringify({text: text, isChecked: false})
        });
        const data = await response.json();
        if (!response.ok) throw new Error(data.error || "Create failed");

        todoList.push(data);
        createAndAppendTodo(data);
        userInputEl.value = "";
        showMessage("Todo saved to MySQL");
    } catch (error) {
        showMessage(error.message, true);
    }
}

addTodoBtn.onclick = onAddTodo;
saveTodoEl.onclick = loadTodos;
userInputEl.addEventListener("keydown", e => {
    if (e.key === "Enter") onAddTodo();
});

loadTodos();
