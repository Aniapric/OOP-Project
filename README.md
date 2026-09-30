# Galmondo – Restaurant Management System

A C++17 console application for managing daily restaurant operations, developed for an Object-Oriented Programming (OOP) university course.

---

## Features

- **Staff Management:** Manage employees (Chefs, Waiters, Administrators), calculate role-specific salaries, and distribute bonuses.
- **Menu Management:** Add and manage dishes and beverages, filter items by category, and sort by price.
- **Table & Seating Management:** Track tables, check available seats, and prevent overbooking.
- **Order Processing:** Create, update, and close live orders with automatic totals.
- **Customer Feedback:** Collect star ratings and written reviews, calculate average ratings, and print summary reports.
- **System Logger:** Record actions and caught errors throughout the app.
- **Fast-Service Combos:** Generate standard or vegan meal combinations using an abstract factory.

---

## OOP & C++ Concepts Used

- **Inheritance & Polymorphism:** 
  - Base interface `IObject` with virtual `toString()` and unique IDs.
  - Class hierarchy: `Persoana` -> `Angajat` -> `Bucatar` / `Ospatar` (`final`), and `Administrator`.
- **Composition:** `Persoana` contains a `DataNastere` object.
- **Design Patterns:**
  - **Singleton:** `Logger` class (Meyers' implementation) for centralized event logging.
  - **Abstract Factory:** `IFabricaMeniu` to create families of related food items (Standard vs. Vegan).
- **Templates:**
  - `RegistruFeedback<T, int LimitaMemorie>` with memory bounds.
  - Template functions with full specialization (`genereazaRaport<int>` for average ratings and `genereazaRaport<string>` for review lists).
- **STL Containers & Algorithms:**
  - `std::vector` (logs, feedback, menu)
  - `std::list` (active orders)
  - `std::map` (employee catalog by ID)
  - Algorithms with lambdas: `std::find_if`, `std::sort`, `std::remove_if`, `std::count_if`.
- **Exception Handling:**
  - Custom exceptions inheriting from `std::exception`: `CapacitateMasaException`, `ProdusInexistentException`, `DateInvalideException`, `PersoanaNegasitaException`.
  - Multi-level `try / catch` blocks to prevent unexpected crashes.

---

## How to Compile and Run

### Prerequisites
A C++ compiler supporting C++17 (`g++` or `clang++`).

### Option 1: Using Makefile
```bash
make
make run
```

### Option 2: Using g++ directly
```bash
# macOS / Linux
g++ -std=c++17 restaurant.cpp -o restaurant
./restaurant

# Windows
g++ -std=c++17 restaurant.cpp -o restaurant.exe
.\restaurant.exe
```

---

## Author

- **Author:** Ania Pricop
- **Course:** Object-Oriented Programming (OOP)
