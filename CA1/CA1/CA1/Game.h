#ifndef GAME_HPP
#define GAME_HPP
#pragma warning( push )
#pragma warning( disable : 4275 )

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>

// Piece types
enum class PieceType {
    NONE,
    DONKEY,
    SNAKE,
    FROG
};

struct SelectedPiece {
    int row = -1;
    int col = -1;
    bool isSelected = false;
};

// Players
enum class Player {
    NONE,
    PLAYER1,  // Human
    PLAYER2   // AI
};

// Struct to represent a piece
struct Piece
{
    PieceType type;
    Player owner;
    int row;
    int col;
    bool isPlaced;

    Piece() : type(PieceType::NONE), owner(Player::NONE), row(-1), col(-1), isPlaced(false) {}

    Piece(PieceType t, Player p) : type(t), owner(p), row(-1), col(-1), isPlaced(false) {}
};

// States
enum class GameState
{
    MENU,
    DIFFICULTY_SELECT,
    PLACING,
    MOVING,
    GAME_OVER
};




// Structure to store move
struct Move
{
    int pieceIndex;
    int fromRow;
    int fromCol;
    int toRow;
    int toCol;

    Move() : pieceIndex(-1), fromRow(-1), fromCol(-1), toRow(-1), toCol(-1) {}

    Move(int idx, int fRow, int fCol, int tRow, int tCol): pieceIndex(idx), fromRow(fRow), fromCol(fCol), toRow(tRow), toCol(tCol) {}
};

class Game {
public:
    Game();
    ~Game();
    void run();

private:
    // Difficulty settings
    enum class Difficulty {
        EASY,
        MEDIUM,
        HARD
    };

    Difficulty currentDifficulty = Difficulty::MEDIUM;

    // Main game loop funcs
    void processEvents();
    void processKeys(const std::optional<sf::Event> t_event);
    void processMouseClick(sf::Vector2i mousePos);
    void checkKeyboardState();
    void update(sf::Time t_deltaTime);
    void render();

    // Setup functions
    void setupSprites();
    void setupBoard();
    void setupPieces();
    void setupMenu();

    // Menu functions 
    void updateMenu(sf::Vector2i mousePos);
    void drawMenu();

	// Difficulty screen functions
    void setupDifficultyScreen();
    void updateDifficultyScreen(sf::Vector2i mousePos);
    void drawDifficultyScreen();
    void setDifficulty(Difficulty difficulty);

    // Core game logic funcs
    void selectPiece(int index);
    void placePiece(int row, int col);
    void movePiece(int row, int col);
    bool isValidPlacement(int row, int col);
    bool isValidMove(Piece& piece, int newRow, int newCol);
    bool checkWin(Player player);
    void switchTurn();
    void resetGame();

    // AI funcs
    void aiTurn();
    void aiPlacePiece();
    void aiMovePiece();

    // P2 Minimax funcs
    int minimax(int depth, bool isMaximizing, int alpha, int beta);
    int evaluateBoard();
    std::vector<Move> getPossibleMoves(Player player);
    void makeMove(const Move& move, Player player);
    void undoMove(const Move& move, Player player);
    Move findBestMove();

	// P3 Advanced minmax and evaluation funcs
    // Evaluation helper functions
    int countDiagonalThreats(Player player, int length);
    int evaluateCenterControl(Player player);
    int evaluateMobility(Player player);
    int evaluatePiecePositions(Player player);

    // Helper funcs
    int getBoardIndex(int row, int col);
    sf::Vector2f getBoardPosition(int row, int col);
    void drawBoard();
    void drawPieces();
    void drawUI();
    int countLineThreats(Player player, int length);

  
    sf::RenderWindow window;
    sf::Font font;
    sf::Text text{ font };

    // menu screen ui elements
    sf::RectangleShape startButton;
    sf::RectangleShape exitButton;
    sf::Text startButtonText{ font };
    sf::Text exitButtonText{ font };
    sf::Text titleText{ font };
	sf::Text restartText{ font };

    // Difficulty screen UI elements
    sf::RectangleShape easyButton;
    sf::RectangleShape mediumButton;
    sf::RectangleShape hardButton;
    sf::RectangleShape backButton;
    sf::Text easyButtonText{ font };
    sf::Text mediumButtonText{ font };
    sf::Text hardButtonText{ font };
    sf::Text backButtonText{ font };
    sf::Text difficultyTitleText{ font };

    // Textures
    sf::Texture donkeyTexture;
    sf::Texture snakeTexture;
    sf::Texture frogTexture;

    sf::Sprite donkeySprite{ donkeyTexture };
    sf::Sprite snakeSprite{ snakeTexture };
    sf::Sprite frogSprite{ frogTexture };

    // Game board
    static const int BOARD_SIZE = 5;
    Piece board[BOARD_SIZE][BOARD_SIZE];

    // Player pieces
    std::vector<Piece> player1Pieces;
    std::vector<Piece> player2Pieces;

    // Game state
    GameState gameState;
    Player currentPlayer;
    int selectedPieceIndex;
    SelectedPiece m_selectedPiece;

    
    float cellSize;
    float boardOffsetX;
    float boardOffsetY;

    // Minimax settings
    int MAX_DEPTH;  // Search depth for minimax

    bool exitGame;
};

#pragma warning( pop )
#endif