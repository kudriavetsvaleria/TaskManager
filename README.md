<div align="right">

[English](./README.md) | [Українська](./README.uk.md)

</div>

# TaskManager

A lightweight **Windows** desktop app for quick recording of daily tasks and plans.  
Built with **C++ (C++17)** and **Qt 6 (Widgets)**. Includes a ready-to-use **installer** (no Qt required for end users).

![Preview](./assets/preview.png)

---

## ✨ Features

- Create and remove tasks
- Task details ("Canvas") for extended notes
- Local persistent storage (**SQLite** via QtSql)
- Frameless dark UI with custom styling
- Synchronized multi-window behavior (Main / List / Canvas)

---

## ⬇️ Download (Windows)

Go to **Releases** and download the latest installer:

- `TaskManager_Setup.exe`

> After installation, TaskManager will appear in **Start Menu** and can create a **Desktop shortcut**.

---

## ▶️ How to use

- **Create task:** `+`
- **Remove task:** `-`
- **Details:** `...`
- **Complete task:** Scratch icon
- **Collapse/Expand list:** Arrow button

---

## 🛠️ Technology Stack

- C++17
- Qt 6 (Widgets, QtSql)
- SQLite
- QMake
- Inno Setup (installer)

---

## 🧩 Build from source (optional)

### Requirements
- Qt 6.x (MinGW 64-bit)
- Qt Creator
- Windows x64

### Steps
1. Clone the repo:
   ```bash
   git clone https://github.com/kudriavetsvaleria/TaskManager.git
