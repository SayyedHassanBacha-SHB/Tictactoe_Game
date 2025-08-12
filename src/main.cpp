#include <SFML/Graphics.hpp> // Includes the SFML Graphics library for rendering.
#include <SFML/Audio.hpp>    // Includes the SFML Audio library for sound handling.
#include <iostream>          // Includes the standard input/output stream library.
#include <string>            // Includes the string library for handling text.
#include <vector>            // Includes the vector library for dynamic arrays.
#include <filesystem>

// Constants for window dimensions and cell size
const int WINDOW_WIDTH = 600;
const int WINDOW_HEIGHT = 700; // Increased height for displaying player info
const int CELL_SIZE = WINDOW_WIDTH / 3;
const sf::Color LIGHT_BLUE(230,230, 250); // Light blue color

// Enumerations for players and game modes
enum Player { NO_PLAYER, PLAYER1, PLAYER2 };
enum GameMode { NONE, SINGLE_PLAYER, TWO_PLAYERS };

// Structure to hold the game state
struct GameState {
    Player board[3][3];      // 3x3 board for Tic-Tac-Toe
    Player currentPlayer;    // Tracks whose turn it is
    int player1Wins;         // Number of wins for Player 1
    int player2Wins;         // Number of wins for Player 2
    std::string player1Name; // Name of Player 1
    std::string player2Name; // Name of Player 2
};

// Resets the game board to its initial state
void resetBoard(GameState &gameState) {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            gameState.board[i][j] = NO_PLAYER;
        }
    }
    gameState.currentPlayer = PLAYER1; // Player 1 starts the game
}

// Checks if a player has won the game
bool checkWin(GameState &gameState, Player player) {
    // Check rows and columns for a win
    for (int i = 0; i < 3; ++i) {
        if (gameState.board[i][0] == player && gameState.board[i][1] == player && gameState.board[i][2] == player)
            return true;
        if (gameState.board[0][i] == player && gameState.board[1][i] == player && gameState.board[2][i] == player)
            return true;
    }
    // Check diagonals for a win
    if (gameState.board[0][0] == player && gameState.board[1][1] == player && gameState.board[2][2] == player)
        return true;
    if (gameState.board[0][2] == player && gameState.board[1][1] == player && gameState.board[2][0] == player)
        return true;
    return false; // No win found
}

// Checks if the game has ended in a draw
bool checkDraw(GameState &gameState) {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (gameState.board[i][j] == NO_PLAYER)
                return false; // If any cell is empty, it's not a draw
        }
    }
    return true; // No empty cells and no win, so it's a draw
}

// Draws the Tic-Tac-Toe board with the player markers
void drawBoard(sf::RenderWindow &window, GameState &gameState, sf::Sprite &player1Sprite, sf::Sprite &player2Sprite) {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (gameState.board[i][j] == PLAYER1) {
                // Position and draw Player 1's marker (X)
                player1Sprite.setPosition(j * CELL_SIZE + CELL_SIZE / 2 - player1Sprite.getGlobalBounds().width / 2,
                                          i * CELL_SIZE + CELL_SIZE / 2 - player1Sprite.getGlobalBounds().height / 2 + 50);
                window.draw(player1Sprite);
            } else if (gameState.board[i][j] == PLAYER2) {
                // Position and draw Player 2's marker (O)
                player2Sprite.setPosition(j * CELL_SIZE + CELL_SIZE / 2 - player2Sprite.getGlobalBounds().width / 2,
                                          i * CELL_SIZE + CELL_SIZE / 2 - player2Sprite.getGlobalBounds().height / 2 + 50);
                window.draw(player2Sprite);
            }
        }
    }
}

// Draws the grid lines for the Tic-Tac-Toe board
void drawGrid(sf::RenderWindow &window) {
    for (int i = 1; i < 3; ++i) {
        // Horizontal lines
        sf::RectangleShape line1(sf::Vector2f(WINDOW_WIDTH, 2));
        line1.setPosition(0, i * CELL_SIZE + 50);
        line1.setFillColor(sf::Color::Black);
        window.draw(line1);

        // Vertical lines
        sf::RectangleShape line2(sf::Vector2f(2, CELL_SIZE * 3));
        line2.setPosition(i * CELL_SIZE, 50);
        line2.setFillColor(sf::Color::Black);
        window.draw(line2);
    }
}

// Utility function to draw text at a specific position
void drawText(sf::RenderWindow &window, sf::Font &font, const std::string &text, int x, int y, int size, sf::Color color) {
    sf::Text sfText;
    sfText.setFont(font);
    sfText.setString(text);
    sfText.setCharacterSize(size);
    sfText.setFillColor(color);
    sfText.setPosition(x, y);
    window.draw(sfText);
}

// Utility function to draw centered text at a specific y-position
void drawCenteredText(sf::RenderWindow &window, sf::Font &font, const std::string &text, int y, int size, sf::Color color) {
    sf::Text sfText;
    sfText.setFont(font);
    sfText.setString(text);
    sfText.setCharacterSize(size);
    sfText.setFillColor(color);
    sf::FloatRect textRect = sfText.getLocalBounds();
    sfText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    sfText.setPosition(sf::Vector2f(WINDOW_WIDTH / 2.0f, y));
    window.draw(sfText);
}

// Captures text input from the player
void getInput(sf::RenderWindow &window, sf::Font &font, std::string &input, const std::string &prompt) {
    sf::Text promptText(prompt, font, 30);
    promptText.setFillColor(sf::Color::Black);
    promptText.setPosition(WINDOW_WIDTH / 2 - promptText.getGlobalBounds().width / 2, WINDOW_HEIGHT / 2 - 50);

    sf::Text inputText("", font, 30);
    inputText.setFillColor(sf::Color::Black);
    inputText.setPosition(WINDOW_WIDTH / 2 - inputText.getGlobalBounds().width / 2, WINDOW_HEIGHT / 2);

    window.clear(LIGHT_BLUE);
    window.draw(promptText);
    window.display();

    sf::Event event;
    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode == '\b' && !input.empty()) {
                    input.pop_back(); // Handle backspace to delete characters
                } else if (event.text.unicode == '\r') {
                    return; // Handle Enter to finalize input
                } else if (event.text.unicode < 128) {
                    input += static_cast<char>(event.text.unicode); // Append typed characters to input
                }
            }

            // Update and display the input text
            inputText.setString(input);
            inputText.setPosition(WINDOW_WIDTH / 2 - inputText.getGlobalBounds().width / 2, WINDOW_HEIGHT / 2);
            window.clear(LIGHT_BLUE);
            window.draw(promptText);
            window.draw(inputText);
            window.display();
        }
    }
}

// Displays the main menu and handles menu selection
GameMode showMainMenu(sf::RenderWindow &window, sf::Font &font, sf::Sound &clickSound) {
    while (window.isOpen()) {
        window.clear(LIGHT_BLUE);
        drawCenteredText(window, font, "Single Player", WINDOW_HEIGHT / 2 - 100, 40, sf::Color::Black);
        drawCenteredText(window, font, "Two Players", WINDOW_HEIGHT / 2, 40, sf::Color::Black);
        drawCenteredText(window, font, "Exit", WINDOW_HEIGHT / 2 + 100, 40, sf::Color::Black);
        window.display();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close(); // Handle window close event
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                clickSound.play(); // Play sound when an option is clicked
                if (event.mouseButton.y >= WINDOW_HEIGHT / 2 - 120 && event.mouseButton.y <= WINDOW_HEIGHT / 2 - 80) {
                    return SINGLE_PLAYER; // Start single player mode
                } else if (event.mouseButton.y >= WINDOW_HEIGHT / 2 - 20 && event.mouseButton.y <= WINDOW_HEIGHT / 2 + 20) {
                    return TWO_PLAYERS; // Start two players mode
                } else if (event.mouseButton.y >= WINDOW_HEIGHT / 2 + 80 && event.mouseButton.y <= WINDOW_HEIGHT / 2 + 120) {
                    window.close(); // Exit the game
                }
            }
        }
    }
    return NONE; // Default return value if the window is closed
}

// Gets player details (names and marker selection)
void getPlayerDetails(GameState &gameState, sf::RenderWindow &window, sf::Font &font, bool singlePlayer) {
    getInput(window, font, gameState.player1Name, "Enter name of Player 1: ");
    std::string marker;
    getInput(window, font, marker, "Choose your marker (X/O): ");
    if (marker == "X" || marker == "x") {
        gameState.currentPlayer = PLAYER1; // Player 1 chooses X
    } else {
        gameState.currentPlayer = PLAYER2; // Player 1 chooses O
    }

    if (singlePlayer) {
        gameState.player2Name = "Computer"; // Default name for the AI opponent
    } else {
        getInput(window, font, gameState.player2Name, "Enter name of Player 2: ");
    }
}

// Simple AI move selection for single-player mode
void makeAIMove(GameState &gameState) {
    std::vector<std::pair<int, int>> availableMoves;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (gameState.board[i][j] == NO_PLAYER) {
                availableMoves.push_back(std::make_pair(i, j)); // Collect available moves
            }
        }
    }

    if (!availableMoves.empty()) {
        int moveIndex = rand() % availableMoves.size(); // Randomly select a move
        gameState.board[availableMoves[moveIndex].first][availableMoves[moveIndex].second] = PLAYER2;
    }
}

// Main function
int main() {
    std::srand(static_cast<unsigned>(std::time(nullptr))); // seed AI random
    std::cout << "Working directory: " << std::filesystem::current_path() << "\n";
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Tic-Tac-Toe"); // Create the game window
    sf::Font font;
    if (!font.loadFromFile("assets/fonts/arial.ttf")) {
        std::cerr << "Failed to load font" << std::endl;
        return -1; // Exit if the font fails to load
    }

    // Load sound files
    sf::SoundBuffer clickBuffer, moveBuffer, winBuffer, drawBuffer;
    if (!clickBuffer.loadFromFile("assets/sounds/click.wav") ||
        !moveBuffer.loadFromFile("assets/sounds/move.wav") ||
        !winBuffer.loadFromFile("assets/sounds/win.wav") ||
        !drawBuffer.loadFromFile("assets/sounds/draw.wav")) {
        std::cerr << "Failed to load sound files" << std::endl;
        return -1; // Exit if the sounds fail to load
    }

    // Create sound objects from the sound buffers
    sf::Sound clickSound(clickBuffer);
    sf::Sound moveSound(moveBuffer);
    sf::Sound winSound(winBuffer);
    sf::Sound drawSound(drawBuffer);

    // Show the main menu and get the selected game mode
    GameMode gameMode = showMainMenu(window, font, clickSound);
    if (gameMode == NONE) return 0; // Exit if no mode is selected

    // Initialize game state
    GameState gameState;
    gameState.player1Wins = 0;
    gameState.player2Wins = 0;

    // Get player details based on the selected game mode
    getPlayerDetails(gameState, window, font, gameMode == SINGLE_PLAYER);
    resetBoard(gameState); // Reset the game board

    // Load player marker images (X and O)
    sf::Texture xTexture, oTexture;
    if (!xTexture.loadFromFile("assets/images/x.png") || !oTexture.loadFromFile("assets/images/o.png")) {
        std::cerr << "Failed to load images" << std::endl;
        return -1; // Exit if the images fail to load
    }
    sf::Sprite player1Sprite(xTexture);
    sf::Sprite player2Sprite(oTexture);

    // Main game loop
    bool gameRunning = true;
    bool askReplay = false;
    std::string message;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close(); // Close the window if the close event is triggered

            if (gameRunning && event.type == sf::Event::MouseButtonPressed) {
                moveSound.play(); // Play move sound when clicking on the board
                int row = (event.mouseButton.y - 50) / CELL_SIZE;
                int col = event.mouseButton.x / CELL_SIZE;
                if (row >= 0 && row < 3 && col >= 0 && col < 3 && gameState.board[row][col] == NO_PLAYER) {
                    gameState.board[row][col] = gameState.currentPlayer;

                    // Check for a win or draw after the move
                    if (checkWin(gameState, gameState.currentPlayer)) {
                        message = (gameState.currentPlayer == PLAYER1) ? gameState.player1Name + " wins!" : gameState.player2Name + " wins!";
                        winSound.play(); // Play win sound
                        if (gameState.currentPlayer == PLAYER1) {
                            gameState.player1Wins++;
                        } else {
                            gameState.player2Wins++;
                        }
                        gameRunning = false; // End the game
                        askReplay = true; // Ask if the player wants to replay
                    } else if (checkDraw(gameState)) {
                        message = "It's a draw!";
                        drawSound.play(); // Play draw sound
                        gameRunning = false; // End the game
                        askReplay = true; // Ask if the player wants to replay
                    } else {
                        gameState.currentPlayer = (gameState.currentPlayer == PLAYER1) ? PLAYER2 : PLAYER1; // Switch turns
                        if (gameMode == SINGLE_PLAYER && gameState.currentPlayer == PLAYER2) {
                            makeAIMove(gameState); // Make a move for the AI
                            if (checkWin(gameState, PLAYER2)) {
                                message = "Computer wins!";
                                winSound.play(); // Play win sound
                                gameState.player2Wins++;
                                gameRunning = false; // End the game
                                askReplay = true; // Ask if the player wants to replay
                            } else if (checkDraw(gameState)) {
                                message = "It's a draw!";
                                drawSound.play(); // Play draw sound
                                gameRunning = false; // End the game
                                askReplay = true; // Ask if the player wants to replay
                            } else {
                                gameState.currentPlayer = PLAYER1; // Switch back to Player 1
                            }
                        }
                    }
                }
            }

            // Handle replay input
            if (!gameRunning && event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Y) {
                    resetBoard(gameState); // Reset the board for a new game
                    gameRunning = true;
                    askReplay = false;
                } else if (event.key.code == sf::Keyboard::N) {
                    window.close(); // Close the window if the player doesn't want to replay
                }
            }
        }

        // Clear the window and draw the game elements
        window.clear(LIGHT_BLUE);

        if (!askReplay) {
            drawGrid(window); // Draw the board grid
            drawBoard(window, gameState, player1Sprite, player2Sprite); // Draw the X and O markers
        }

        if (askReplay) {
            drawCenteredText(window, font, message, WINDOW_HEIGHT / 2 - 50, 50, sf::Color::Red);
            drawCenteredText(window, font, "Play again? (Y/N)", WINDOW_HEIGHT / 2 + 50, 30, sf::Color::Black);
        } else {
            std::string turnMessage = (gameState.currentPlayer == PLAYER1) ? gameState.player1Name + "'s turn (X)" : gameState.player2Name + "'s turn (O)";
            drawCenteredText(window, font, turnMessage, 20, 30, sf::Color::Black);
        }

        // Draw the win counts at the bottom of the window
        drawCenteredText(window, font, gameState.player1Name + " Wins: " + std::to_string(gameState.player1Wins), WINDOW_HEIGHT - 110, 20, sf::Color::Black);
        drawCenteredText(window, font, gameState.player2Name + " Wins: " + std::to_string(gameState.player2Wins), WINDOW_HEIGHT - 80, 20, sf::Color::Black);

        window.display(); // Display the updated frame
    }

    return 0; // End of the program
}
