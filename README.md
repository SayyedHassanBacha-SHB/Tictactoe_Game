# 🎮 TicTacToe Game

A visually appealing **Tic-Tac-Toe** game built in **C++** using the [SFML](https://www.sfml-dev.org/) multimedia library.  
Features smooth graphics, engaging sound effects, and a clean CMake build system — perfect for learning and showcasing SFML game development.

----------------------------------------------------------------

## ✨ Features
- 🖥 **Graphical Interface** — Built with SFML for a modern look
- 🔊 **Sound Effects** — Feedback for moves and game events
- 🎮 **Single & Two Player Support** *(if implemented)*
- ⚡ **Lightweight & Fast** — Minimal dependencies
- 📦 **Cross-platform Build** — Works with CMake on Windows/Linux/Mac

----------------------------------------------------------------

## 📂 Folder Structure
Tictactoe_Game/  
│                                 
├── assets/ # Images, fonts, sounds             
├── src/ # Source code (.cpp)               
├── CMakeLists.txt # Build configuration             
├── .gitignore # Git ignored files                    
└── README.md # Documentation                       

----------------------------------------------------------------

## 🛠 Installation & Build Instructions (Windows + MinGW + SFML)

### 1️⃣ Prerequisites
Make sure you have:
- [MinGW-w64](https://www.mingw-w64.org/) installed & added to PATH  
- [CMake](https://cmake.org/download/) installed  
- [SFML 2.6.1](https://www.sfml-dev.org/download.php) downloaded & extracted  

-----------------------------

### 2️⃣ Build the Project
```bash
# Clone repository
git clone https://github.com/SayyedHassanBacha-SHB/Tictactoe_Game.git
cd Tictactoe_Game

# Configure with CMake (change SFML path if needed)
cmake -G "MinGW Makefiles" -B build -DSFML_DIR="C:/SFML-2.6.1/lib/cmake/SFML"

# Compile the game
cmake --build build --config Release
```
--------------------------------

### 3️⃣ Run the Game
      ./build/TicTacToe.exe

----------------------------------------------------------------

### 🚀 Future Improvements
-🤖 AI opponent using Minimax
-📊 Score tracking
-🎨 Custom themes/skins
-🌀 Animations for moves & win effects

----------------------------------------------------------------

### 📜 License
This project is licensed under the MIT License — you are free to use, modify, and distribute it.

----------------------------------------------------------------

# 💡 Tip: If you want to contribute or improve this game, fork the repo, make changes, and submit a pull request!

