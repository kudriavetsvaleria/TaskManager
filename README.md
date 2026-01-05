# Task Manager

A powerful, sleek, and high-performance Task Manager application built with **C++** and **Qt 6**. 
Designed with a focus on User Experience, it features a unique frameless UI with **physics-based synchronized window movement**.

![Preview](C:/Users/vvkud/.gemini/antigravity/brain/cdca0fcf-43dc-4a78-b5dc-a269d290751d/uploaded_image_1767613362610.png)

## ✨ Features

- **Modern Frameless UI**: A distraction-free, dark-themed interface with custom rounded corners.
- **Atomic Window Synchronization**: 
    - The application consists of three independent windows (Main, List, Canvas) that behave as a single cohesive unit.
    - **Pixel-Perfect Dragging**: Utilizes the **Windows DeferWindowPos API** to move all windows in a single atomic transaction, ensuring zero lag or rubber-banding.
    - **Input Throttling**: Intelligent event processing handles high-poll-rate gaming mice (1000Hz+) for silky smooth movement (~120 FPS lock).
- **Persistent Storage**: All tasks and instructions are efficiently stored in a local **SQLite** database.
- **Detailed Task Canvas**: Click the "More" button to open a dedicated, modeless text editor for task descriptions.
- **Interactive States**: Strike-through completion, hover effects, and animated transitions.

## 🛠️ Technology Stack

- **Core**: C++17
- **Framework**: Qt 6.7 (Widgets)
- **Database**: SQLite (via QtSql)
- **OS Integration**: Win32 API (User32.dll) for window management optimization.
- **Build System**: QMake

## � Installation & Build

### Prerequisites
- Qt 6.7 or higher
- MinGW 64-bit Compiler (recommended)

### Steps
1. Clone the repository:
   ```bash
   git clone https://github.com/kudriavetsvaleria/TaskManager.git
   ```
2. Open `RestartTaskManager.pro` in Qt Creator.
3. Configure the project for **Release** or **Debug**.
4. Build and Run.

## 🎮 Controls

- **Adding Tasks**: Click the `+` button to create a new task.
- **Removing Tasks**: Select a task and click the `-` button.
- **Details (Canvas)**: Click the **...** (three dots) button on a task to open the detailed view.
- **Completion**: Click the **Scratch** icon to toggle task completion.
- **Window Management**: 
    - Drag anywhere on the Main Window or List to move the entire application.
    - Click the Arrow button on the Main Window to collapse/expand the task list.

## 🔧 Inner Workings

The project demonstrates advanced Qt widget manipulation:
*   **Event Filtering**: Custom `QEvent` handling to hijack drag events from child windows.
*   **Window Flags**: Usage of `Qt::Tool` to manage taskbar visibility and parent-child relationships without standard window chrome.
*   **Performance**: Direct Win32 API calls (`BeginDeferWindowPos`) bypass the standard Qt event loop for dragging, providing native performance.

---
*Developed by [Valeria Kudriavets](https://github.com/kudriavetsvaleria)*
