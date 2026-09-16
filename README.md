# C++ Inventory Management System

A console-based inventory management application developed in modern C++.

## Features
- Add, view, search, update and delete products
- Automatic product ID generation
- Persistent storage using file handling
- Inventory valuation
- Low-stock reporting
- Input validation
- Object-Oriented Programming design

## Concepts Demonstrated
- Classes and objects
- Encapsulation
- STL `vector`
- File I/O
- Algorithms (`find`, `remove_if`, `transform`)
- Exception handling
- Input validation
- Modular program design

## Requirements
- C++17 or later
- GCC, MinGW, Clang, or Visual Studio

## Compile and Run

### GCC / MinGW
```bash
g++ -std=c++17 -O2 -Wall -Wextra main.cpp -o inventory
./inventory
```

On Windows:
```bash
inventory.exe
```

The program creates `products.txt` in the same directory to persist inventory records.

## Sample Workflow
1. Add products such as Laptop, Keyboard and Mouse.
2. View all products.
3. Search for a product by name or category.
4. Update quantity/price when stock changes.
5. Use Inventory Report to see total units, inventory value and low-stock items.

## Project Structure
```text
Cpp_Inventory_Management_System/
├── main.cpp
├── README.md
└── .gitignore
```

## LinkedIn Description
Developed a C++ Inventory Management System using OOP and file handling. Implemented CRUD operations, product search, input validation, persistent data storage, inventory valuation, and low-stock reporting. Demonstrates practical use of STL, classes, file I/O, algorithms, and problem-solving in C++.
