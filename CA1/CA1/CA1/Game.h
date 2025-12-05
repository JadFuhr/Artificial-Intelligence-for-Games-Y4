/**
 * @file Game.h
 * @brief Main game class header for The Fourth Protocol
 * @author Jad Fuhr 
 * @date 05/12/2025
 */

#ifndef GAME_HPP
#define GAME_HPP
#pragma warning( push )
#pragma warning( disable : 4275 )

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>

 /**
  * @enum PieceType
  * @brief Defines the types of game pieces available
  */
enum class PieceType {
    NONE,    ///< No piece
    DONKEY,  ///< Donkey piece - moves orthogonally one square
    SNAKE,   ///< Snake piece - moves in any direction one square
    FROG     ///< Frog piece - can jump over other pieces
};

/**
 * @struct SelectedPiece
 * @brief Stores information about currently selected piece
 */
struct SelectedPiece {
    int row = -1;        ///< Row position of selected piece
    int col = -1;        ///< Column position of selected piece
    bool isSelected = false;  ///< Whether a piece is currently selected
};

/**
 * @enum Player
 * @brief Defines the players in the game
 */
enum class Player {
    NONE,     ///< No player
    PLAYER1,  ///< Human player
    PLAYER2   ///< AI player
};

/**
 * @struct Piece
 * @brief Represents a game piece with its properties
 */
struct Piece
{
    PieceType type;  ///< Type of the piece
    Player owner;    ///< Owner of the piece
    int row;         ///< Current row position
    int col;         ///< Current column position
    bool isPlaced;   ///< Whether piece has been placed on board

    /**
     * @brief Default constructor
     */
    Piece() : type(PieceType::NONE), owner(Player::NONE), row(-1), col(-1), isPlaced(false) {}

    /**
     * @brief Parameterized constructor
     * @param t Piece type
     * @param p Player owner
     */
    Piece(PieceType t, Player p) : type(t), owner(p), row(-1), col(-1), isPlaced(false) {}
};

/**
 * @enum GameState
 * @brief Defines the possible states of the game
 */
enum class GameState
{
    MENU,              ///< Main menu screen
    DIFFICULTY_SELECT, ///< Difficulty selection screen
    PLACING,           ///< Piece placement phase
    MOVING,            ///< Piece movement phase
    GAME_OVER          ///< Game over state
};

/**
 * @struct Move
 * @brief Represents a move in the game
 */
struct Move
{
    int pieceIndex;  ///< Index of piece being moved
    int fromRow;     ///< Starting row position
    int fromCol;     ///< Starting column position
    int toRow;       ///< Destination row position
    int toCol;       ///< Destination column position

    /**
     * @brief Default constructor
     */
    Move() : pieceIndex(-1), fromRow(-1), fromCol(-1), toRow(-1), toCol(-1) {}

    /**
     * @brief Parameterized constructor
     * @param idx Piece index
     * @param fRow From row
     * @param fCol From column
     * @param tRow To row
     * @param tCol To column
     */
    Move(int idx, int fRow, int fCol, int tRow, int tCol) : pieceIndex(idx), fromRow(fRow), fromCol(fCol), toRow(tRow), toCol(tCol) {}
};

/**
 * @class Game
 * @brief Main game class that handles all game logic and rendering
 *
 * This class manages the game loop, handles player input, AI decisions,
 * and all rendering operations for The Fourth Protocol game.
 */
class Game {
public:
    /**
     * @brief Constructor - initializes game window and components
     */
    Game();

    /**
     * @brief Destructor
     */
    ~Game();

    /**
     * @brief Main game loop
     */
    void run();

private:
    /**
     * @enum Difficulty
     * @brief AI difficulty levels
     */
    enum class Difficulty {
        EASY,    ///< Easy difficulty (depth 1)
        MEDIUM,  ///< Medium difficulty (depth 3)
        HARD     ///< Hard difficulty (depth 5)
    };

    Difficulty currentDifficulty = Difficulty::MEDIUM;  ///< Current AI difficulty

    // Main game loop functions
    /**
     * @brief Processes all events from the event queue
     */
    void processEvents();

    /**
     * @brief Processes keyboard input
     * @param t_event The keyboard event to process
     */
    void processKeys(const std::optional<sf::Event> t_event);

    /**
     * @brief Processes mouse click events
     * @param mousePos Position of the mouse click
     */
    void processMouseClick(sf::Vector2i mousePos);

    /**
     * @brief Checks current keyboard state
     */
    void checkKeyboardState();

    /**
     * @brief Updates game state
     * @param t_deltaTime Time elapsed since last update
     */
    void update(sf::Time t_deltaTime);

    /**
     * @brief Renders all game elements to screen
     */
    void render();

    // Setup functions
    /**
     * @brief Loads and sets up all sprite textures
     */
    void setupSprites();

    /**
     * @brief Initializes the game board
     */
    void setupBoard();

    /**
     * @brief Creates initial piece sets for both players
     */
    void setupPieces();

    /**
     * @brief Sets up main menu UI elements
     */
    void setupMenu();

    // Menu functions 
    /**
     * @brief Updates menu hover effects
     * @param mousePos Current mouse position
     */
    void updateMenu(sf::Vector2i mousePos);

    /**
     * @brief Renders the main menu
     */
    void drawMenu();

    // Difficulty screen functions
    /**
     * @brief Sets up difficulty selection screen UI
     */
    void setupDifficultyScreen();

    /**
     * @brief Updates difficulty screen hover effects
     * @param mousePos Current mouse position
     */
    void updateDifficultyScreen(sf::Vector2i mousePos);

    /**
     * @brief Renders difficulty selection screen
     */
    void drawDifficultyScreen();

    /**
     * @brief Sets the AI difficulty level
     * @param difficulty The difficulty level to set
     */
    void setDifficulty(Difficulty difficulty);

    // Core game logic functions
    /**
     * @brief Selects a piece for placement
     * @param index Index of piece in player's piece vector
     */
    void selectPiece(int index);

    /**
     * @brief Places selected piece on the board
     * @param row Row to place piece
     * @param col Column to place piece
     */
    void placePiece(int row, int col);

    /**
     * @brief Moves a piece to new position
     * @param row Destination row
     * @param col Destination column
     */
    void movePiece(int row, int col);

    /**
     * @brief Checks if a placement is valid
     * @param row Row to check
     * @param col Column to check
     * @return true if placement is valid
     */
    bool isValidPlacement(int row, int col);

    /**
     * @brief Checks if a move is valid for a piece
     * @param piece The piece to move
     * @param newRow Destination row
     * @param newCol Destination column
     * @return true if move is valid
     */
    bool isValidMove(Piece& piece, int newRow, int newCol);

    /**
     * @brief Checks if a player has won
     * @param player Player to check for win
     * @return true if player has won
     */
    bool checkWin(Player player);

    /**
     * @brief Switches turn to other player
     */
    void switchTurn();

    /**
     * @brief Resets game to initial state
     */
    void resetGame();

    // AI functions
    /**
     * @brief Executes AI turn (placement or movement)
     */
    void aiTurn();

    /**
     * @brief AI piece placement logic
     */
    void aiPlacePiece();

    /**
     * @brief AI piece movement logic
     */
    void aiMovePiece();

    // Minimax functions
    /**
     * @brief Minimax algorithm with alpha-beta pruning
     * @param depth Current search depth
     * @param isMaximizing Whether current player is maximizing
     * @param alpha Alpha value for pruning
     * @param beta Beta value for pruning
     * @return Evaluation score for current position
     */
    int minimax(int depth, bool isMaximizing, int alpha, int beta);

    /**
     * @brief Evaluates current board state
     * @return Score representing board advantage
     */
    int evaluateBoard();

    /**
     * @brief Gets all possible moves for a player
     * @param player Player to get moves for
     * @return Vector of possible moves
     */
    std::vector<Move> getPossibleMoves(Player player);

    /**
     * @brief Applies a move to the board
     * @param move Move to make
     * @param player Player making the move
     */
    void makeMove(const Move& move, Player player);

    /**
     * @brief Undoes a move on the board
     * @param move Move to undo
     * @param player Player whose move to undo
     */
    void undoMove(const Move& move, Player player);

    /**
     * @brief Finds best move using minimax
     * @return Best move for AI
     */
    Move findBestMove();

    // Advanced evaluation functions
    /**
     * @brief Counts diagonal threats of specified length
     * @param player Player to count threats for
     * @param length Length of threat to count
     * @return Number of diagonal threats
     */
    int countDiagonalThreats(Player player, int length);

    /**
     * @brief Evaluates player's control of center squares
     * @param player Player to evaluate
     * @return Center control score
     */
    int evaluateCenterControl(Player player);

    /**
     * @brief Evaluates player's mobility (number of possible moves)
     * @param player Player to evaluate
     * @return Mobility score
     */
    int evaluateMobility(Player player);

    /**
     * @brief Evaluates piece positioning and value
     * @param player Player to evaluate
     * @return Piece position score
     */
    int evaluatePiecePositions(Player player);

    // Helper functions
    /**
     * @brief Converts row/col to linear board index
     * @param row Board row
     * @param col Board column
     * @return Linear index
     */
    int getBoardIndex(int row, int col);

    /**
     * @brief Gets screen position for board cell
     * @param row Board row
     * @param col Board column
     * @return Screen position vector
     */
    sf::Vector2f getBoardPosition(int row, int col);

    /**
     * @brief Renders the game board
     */
    void drawBoard();

    /**
     * @brief Renders all pieces
     */
    void drawPieces();

    /**
     * @brief Renders UI elements (text, etc)
     */
    void drawUI();

    /**
     * @brief Counts line threats (horizontal/vertical)
     * @param player Player to count threats for
     * @param length Length of threat
     * @return Number of line threats
     */
    int countLineThreats(Player player, int length);

    // SFML objects
    sf::RenderWindow window;  ///< Main game window
    sf::Font font;            ///< Game font
    sf::Text text{ font };    ///< General text object

    // Menu UI elements
    sf::RectangleShape startButton;  ///< Start button shape
    sf::RectangleShape exitButton;   ///< Exit button shape
    sf::Text startButtonText{ font }; ///< Start button text
    sf::Text exitButtonText{ font };  ///< Exit button text
    sf::Text titleText{ font };       ///< Title text
    sf::Text restartText{ font };     ///< Restart instruction text

    // Difficulty screen UI elements
    sf::RectangleShape easyButton;    ///< Easy difficulty button
    sf::RectangleShape mediumButton;  ///< Medium difficulty button
    sf::RectangleShape hardButton;    ///< Hard difficulty button
    sf::RectangleShape backButton;    ///< Back button
    sf::Text easyButtonText{ font };   ///< Easy button text
    sf::Text mediumButtonText{ font }; ///< Medium button text
    sf::Text hardButtonText{ font };   ///< Hard button text
    sf::Text backButtonText{ font };   ///< Back button text
    sf::Text difficultyTitleText{ font }; ///< Difficulty screen title

    // Textures and sprites
    sf::Texture donkeyTexture;  ///< Donkey piece texture
    sf::Texture snakeTexture;   ///< Snake piece texture
    sf::Texture frogTexture;    ///< Frog piece texture

    sf::Sprite donkeySprite{ donkeyTexture };  ///< Donkey sprite
    sf::Sprite snakeSprite{ snakeTexture };    ///< Snake sprite
    sf::Sprite frogSprite{ frogTexture };      ///< Frog sprite

    // Game board
    static const int BOARD_SIZE = 5;  ///< Size of game board (5x5)
    Piece board[BOARD_SIZE][BOARD_SIZE];  ///< Game board array

    // Player pieces
    std::vector<Piece> player1Pieces;  ///< Player 1's pieces
    std::vector<Piece> player2Pieces;  ///< AI's pieces

    // Game state
    GameState gameState;           ///< Current game state
    Player currentPlayer;          ///< Current active player
    int selectedPieceIndex;        ///< Index of selected piece
    SelectedPiece m_selectedPiece; ///< Currently selected board piece

    // Board layout
    float cellSize;      ///< Size of board cells in pixels
    float boardOffsetX;  ///< X offset of board from window edge
    float boardOffsetY;  ///< Y offset of board from window edge

    // AI settings
    int MAX_DEPTH;  ///< Search depth for minimax algorithm

    bool exitGame;  ///< Flag to exit game loop
};

#pragma warning( pop )
#endif