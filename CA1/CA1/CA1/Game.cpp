#include "Game.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <limits> 

Game::Game() :
	window{ sf::VideoMode{ sf::Vector2u{1020U, 720U}, 32U }, "The Fourth Protocol" },
	gameState(GameState::MENU),
	currentPlayer(Player::PLAYER1),
	selectedPieceIndex(-1),
//slectedBoardPiece(nullptr),
	cellSize(100.0f),
	boardOffsetX(250.0f),
	boardOffsetY(100.0f),
	exitGame(false)
{ 
	srand(static_cast<unsigned>(time(0)));
	setupSprites();
	setupBoard();
	setupPieces();
	setupMenu();
}

Game::~Game()
{
}

void Game::run()
{	
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	const float fps{ 60.0f };
	sf::Time timePerFrame = sf::seconds(1.0f / fps); 
	while (window.isOpen())
	{
		processEvents(); 
		timeSinceLastUpdate += clock.restart();
		while (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;
			processEvents(); 
			update(timePerFrame); 
		}
		render(); 
	}
}

void Game::processEvents()
{
	while (const std::optional newEvent = window.pollEvent())
	{
		if (newEvent->is<sf::Event::Closed>())
		{
			exitGame = true;
		}
		if (newEvent->is<sf::Event::KeyPressed>())
		{
			processKeys(newEvent);
		}
		if (newEvent->is<sf::Event::MouseButtonPressed>())
		{
			const sf::Event::MouseButtonPressed* mouseEvent = newEvent->getIf<sf::Event::MouseButtonPressed>();

			if (mouseEvent->button == sf::Mouse::Button::Left)
			{
				sf::Vector2i mousePos = sf::Mouse::getPosition(window);
				processMouseClick(mousePos);
			}
		}
	}
}

void Game::processKeys(const std::optional<sf::Event> t_event)
{
	const sf::Event::KeyPressed *newKeypress = t_event->getIf<sf::Event::KeyPressed>();
	if (sf::Keyboard::Key::Escape == newKeypress->code)
	{
		resetGame();
		gameState = GameState::MENU;
	}
	if (gameState == GameState::GAME_OVER && sf::Keyboard::Key::R == newKeypress->code)
	{
		resetGame();
		return;
	}
}

void Game::processMouseClick(sf::Vector2i mousePos)
{
	
	//handle menu clicks
	if (gameState == GameState::MENU)
	{
		// Check if Start button clicked
		if (startButton.getGlobalBounds().contains(sf::Vector2f(mousePos)))
		{
			gameState = GameState::PLACING;
			std::cout << "Game Started!\n";
		}
		// Check if Exit button clicked
		else if (exitButton.getGlobalBounds().contains(sf::Vector2f(mousePos)))
		{
			exitGame = true;
		}
		return;
	}
	if (gameState == GameState::GAME_OVER || currentPlayer == Player::PLAYER2)
	{
		return;
	}

	int col = static_cast<int>((mousePos.x - boardOffsetX) / cellSize);
	int row = static_cast<int>((mousePos.y - boardOffsetY) / cellSize);

	if (row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE)
	{
		if (gameState == GameState::PLACING)
		{
			placePiece(row, col);
		}
		else if (gameState == GameState::MOVING)
		{
			// First click: select a piece
			if (!m_selectedPiece.isSelected)
			{
				if (board[row][col].owner == currentPlayer &&
					board[row][col].type != PieceType::NONE)
				{
					m_selectedPiece.row = row;
					m_selectedPiece.col = col;
					m_selectedPiece.isSelected = true;

					std::cout << "Selected piece at (" << row << "," << col << ")\n";
				}
			}
			// Second click: move the selected piece
			else
			{
				movePiece(row, col);
			}
		}
	}
	else
	{
		// Clicking outside board during PLACING phase
		if (gameState == GameState::PLACING)
		{
			float pieceX = 50.0f;
			float pieceY = 100.0f;

			for (int i = 0; i < player1Pieces.size(); i++)
			{
				if (!player1Pieces[i].isPlaced)
				{
					if (mousePos.x >= pieceX && mousePos.x <= pieceX + 80 &&
						mousePos.y >= pieceY && mousePos.y <= pieceY + 80)
					{
						selectPiece(i);
						return;
					}
					pieceY += 100.0f;
				}
			}
		}
	}
}

void Game::checkKeyboardState()
{
	//if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
	//{
	//	gameState = GameState::MENU;
	//}
}

void Game::update(sf::Time t_deltaTime)
{
	checkKeyboardState();

	if (exitGame)
	{
		window.close();
	}

	//update menu effects
	if (gameState == GameState::MENU)
	{
		sf::Vector2i mousePos = sf::Mouse::getPosition(window);
		updateMenu(mousePos);
		return;
	}

	// Check if all pieces placed
	if (gameState == GameState::PLACING)
	{
		bool allPlaced = true;

		for (const auto& piece : player1Pieces) 
		{
			if (!piece.isPlaced)
			{
				allPlaced = false;
			}
		}
		for (const auto& piece : player2Pieces)
		{
			if (!piece.isPlaced)
			{
				allPlaced = false;
			}
		}

		if (allPlaced)
		{
			gameState = GameState::MOVING;
		}
	}

	// AI turn
	if (currentPlayer == Player::PLAYER2 && gameState != GameState::GAME_OVER)
	{
		static sf::Clock aiDelay;

		if (aiDelay.getElapsedTime().asSeconds() > 0.5f)
		{
			aiTurn();
			aiDelay.restart();
		}
	}
}

void Game::render()
{
	window.clear(sf::Color::Black);

	if (gameState == GameState::MENU)
	{
		drawMenu();
	}
	else
	{
		drawBoard();
		drawPieces();
		drawUI();
	}
	
	window.display();
}

///////////////////////////////////////////////////////////////////////
// setup functions
///////////////////////////////////////////////////////////////////////

void Game::setupSprites()
{
	if (!donkeyTexture.loadFromFile("ASSETS\\IMAGES\\Donkey.png"))
	{

		std::cout << "problem loading donkey" << std::endl;
	}

	donkeySprite.setTextureRect(sf::IntRect({ 0,0 }, { 128,128 }));

	if (!snakeTexture.loadFromFile("ASSETS\\IMAGES\\Snake.png"))
	{

		std::cout << "problem loading snake" << std::endl;
	}

	snakeSprite.setTextureRect(sf::IntRect({ 0,0 }, { 128,128 }));

	if (!frogTexture.loadFromFile("ASSETS\\IMAGES\\Frog.png"))
	{

		std::cout << "problem loading frog" << std::endl;
	}

	frogSprite.setTextureRect(sf::IntRect({ 0,0 }, { 128,128 }));

	//sprite.setTexture(texture,true);
	//sprite.setPosition(sf::Vector2f{ 150.0f, 50.0f });


	if (!font.openFromFile("ASSETS/FONTS/Jersey20-Regular.ttf"))
	{
		std::cout << "Problem loading font\n";
	}
}

void Game::setupBoard()
{

	for (int row = 0; row < BOARD_SIZE; row++)
	{
		for (int col = 0; col < BOARD_SIZE; col++)
		{
			board[row][col] = Piece();
		}
	}

}

void Game::setupPieces()
{

	// Player 1 pieces
	player1Pieces.push_back(Piece(PieceType::FROG, Player::PLAYER1));
	player1Pieces.push_back(Piece(PieceType::SNAKE, Player::PLAYER1));
	player1Pieces.push_back(Piece(PieceType::DONKEY, Player::PLAYER1));
	player1Pieces.push_back(Piece(PieceType::DONKEY, Player::PLAYER1));
	player1Pieces.push_back(Piece(PieceType::DONKEY, Player::PLAYER1));

	// Player 2 pieces
	player2Pieces.push_back(Piece(PieceType::FROG, Player::PLAYER2));
	player2Pieces.push_back(Piece(PieceType::SNAKE, Player::PLAYER2));
	player2Pieces.push_back(Piece(PieceType::DONKEY, Player::PLAYER2));
	player2Pieces.push_back(Piece(PieceType::DONKEY, Player::PLAYER2));
	player2Pieces.push_back(Piece(PieceType::DONKEY, Player::PLAYER2));
}

///////////////////////////////////////////////////////////////////////
// core game logic functions
///////////////////////////////////////////////////////////////////////

void Game::selectPiece(int index)
{

	selectedPieceIndex = index;

}

void Game::placePiece(int row, int col)
{

	if (currentPlayer == Player::PLAYER1 && selectedPieceIndex >= 0)
	{
		if (isValidPlacement(row, col))
		{
			Piece& piece = player1Pieces[selectedPieceIndex];
			piece.row = row;
			piece.col = col;
			piece.isPlaced = true;
			board[row][col] = piece;

			selectedPieceIndex = -1;

			if (checkWin(currentPlayer))
			{
				gameState = GameState::GAME_OVER;
				return;
			}

			switchTurn();
		}
	}
}

void Game::movePiece(int newRow, int newCol)
{
	if (!m_selectedPiece.isSelected)
	{
		return;
	}

	int oldRow = m_selectedPiece.row;
	int oldCol = m_selectedPiece.col;

	// Get reference to the piece we're moving
	Piece& pieceToMove = board[oldRow][oldCol];

	// Validate the move
	if (isValidMove(pieceToMove, newRow, newCol))
	{
		std::cout << "Moving piece from (" << oldRow << "," << oldCol
			<< ") to (" << newRow << "," << newCol << ")\n";

		// Find this piece in the player's piece vector and update it
		std::vector<Piece>& pieces = (currentPlayer == Player::PLAYER1) ?
			player1Pieces : player2Pieces;

		for (auto& piece : pieces)
		{
			if (piece.row == oldRow && piece.col == oldCol &&
				piece.owner == currentPlayer)
			{
				// Update the piece in the vector
				piece.row = newRow;
				piece.col = newCol;
				break;
			}
		}

		// Update the board
		board[newRow][newCol] = pieceToMove;
		board[newRow][newCol].row = newRow;
		board[newRow][newCol].col = newCol;

		// Clear old position
		board[oldRow][oldCol] = Piece();

		// Deselect
		m_selectedPiece.isSelected = false;

		// Check win
		if (checkWin(currentPlayer))
		{
			gameState = GameState::GAME_OVER;
			std::cout << "Game Over! " <<
				(currentPlayer == Player::PLAYER1 ? "Player 1" : "AI") << " wins!\n";
			return;
		}

		switchTurn();
	}
	else
	{
		std::cout << "Invalid move!\n";
		// Deselect if invalid move
		m_selectedPiece.isSelected = false;
	}
}

bool Game::isValidPlacement(int row, int col)
{
	return board[row][col].type == PieceType::NONE;
}

bool Game::isValidMove(Piece& piece, int newRow, int newCol)
{
	if (board[newRow][newCol].type != PieceType::NONE)
	{
		return false;
	}

	int rowDiff = abs(newRow - piece.row);
	int colDiff = abs(newCol - piece.col);

	switch (piece.type)
	{
	case PieceType::DONKEY:

		return (rowDiff == 1 && colDiff == 0) || (rowDiff == 0 && colDiff == 1);

	case PieceType::SNAKE:

		return rowDiff <= 1 && colDiff <= 1 && (rowDiff != 0 || colDiff != 0);

	case PieceType::FROG:

		if (rowDiff <= 1 && colDiff <= 1 && (rowDiff != 0 || colDiff != 0))
		{
			return true;
		}

		if (rowDiff == 0 || colDiff == 0 || rowDiff == colDiff)
		{
			int rowStep = (newRow > piece.row) ? 1 : (newRow < piece.row ? -1 : 0);
			int colStep = (newCol > piece.col) ? 1 : (newCol < piece.col ? -1 : 0);

			int checkRow = piece.row + rowStep;
			int checkCol = piece.col + colStep;
			bool jumpedPiece = false;

			while (checkRow != newRow || checkCol != newCol)
			{
				if (board[checkRow][checkCol].type != PieceType::NONE)
				{
					jumpedPiece = true;
				}
				checkRow += rowStep;
				checkCol += colStep;
			}

			return jumpedPiece;
		}
		return false;

	default:
		return false;
	}
}

bool Game::checkWin(Player player)
{
	// Horizontal
	for (int row = 0; row < BOARD_SIZE; row++)
	{
		for (int col = 0; col <= BOARD_SIZE - 4; col++)
		{
			if (board[row][col].owner == player && board[row][col + 1].owner == player && board[row][col + 2].owner == player && board[row][col + 3].owner == player)
			{
				return true;
			}
		}
	}

	// Vertical
	for (int col = 0; col < BOARD_SIZE; col++)
	{
		for (int row = 0; row <= BOARD_SIZE - 4; row++) 
		{
			if (board[row][col].owner == player && board[row + 1][col].owner == player && board[row + 2][col].owner == player && board[row + 3][col].owner == player)
			{
				return true;
			}
		}
	}

	// Diagonal (top-left to bottom-right)
	for (int row = 0; row <= BOARD_SIZE - 4; row++)
	{
		for (int col = 0; col <= BOARD_SIZE - 4; col++)
		{
			if (board[row][col].owner == player && board[row + 1][col + 1].owner == player && board[row + 2][col + 2].owner == player && board[row + 3][col + 3].owner == player)
			{
				return true;
			}
		}
	}

	// Diagonal (top-right to bottom-left)
	for (int row = 0; row <= BOARD_SIZE - 4; row++)
	{
		for (int col = 3; col < BOARD_SIZE; col++)
		{
			if (board[row][col].owner == player && board[row + 1][col - 1].owner == player && board[row + 2][col - 2].owner == player && board[row + 3][col - 3].owner == player)
			{
				return true;
			}
			
		}
	}

	return false;
}

void Game::switchTurn()
{

	currentPlayer = (currentPlayer == Player::PLAYER1) ? Player::PLAYER2 : Player::PLAYER1;
}

void Game::resetGame()
{
	// Clear board
	for (int row = 0; row < BOARD_SIZE; row++)
	{
		for (int col = 0; col < BOARD_SIZE; col++)
		{
			board[row][col] = Piece();
		}
	}

	// Reset player pieces
	player1Pieces.clear();
	player2Pieces.clear();
	setupPieces();  // recreate all pieces

	// Reset state
	currentPlayer = Player::PLAYER1;
	gameState = GameState::PLACING;

	// Clear selections
	selectedPieceIndex = -1;
	m_selectedPiece = SelectedPiece();

	std::cout << "Game Reset!\n";
}

///////////////////////////////////////////////////////////////////////
// AI functions
///////////////////////////////////////////////////////////////////////

void Game::aiTurn()
{
	if (gameState == GameState::PLACING)
	{
		aiPlacePiece();
	}
	else if (gameState == GameState::MOVING) 
	{
		aiMovePiece();
	}
}

void Game::aiPlacePiece()
{
	// Find the best unplaced piece
	Move bestPlacement;
	int bestScore = std::numeric_limits<int>::min();

	for (int i = 0; i < player2Pieces.size(); i++)
	{
		if (!player2Pieces[i].isPlaced)
		{
			// Try placing this piece on every empty cell
			for (int row = 0; row < BOARD_SIZE; row++)
			{
				for (int col = 0; col < BOARD_SIZE; col++)
				{
					if (isValidPlacement(row, col))
					{
						// Temporarily place the piece
						player2Pieces[i].row = row;
						player2Pieces[i].col = col;
						player2Pieces[i].isPlaced = true;
						board[row][col] = player2Pieces[i];

						// Evaluate using minimax (or just evaluateBoard for speed)
						int score = evaluateBoard();
						// OR for deeper search: minimax(2, false, MIN, MAX);

						// Undo the placement
						board[row][col] = Piece();
						player2Pieces[i].isPlaced = false;
						player2Pieces[i].row = -1;
						player2Pieces[i].col = -1;

						// Track best placement
						if (score > bestScore)
						{
							bestScore = score;
							bestPlacement = Move(i, -1, -1, row, col);
						}
					}
				}
			}

			// Place the best piece found
			if (bestPlacement.pieceIndex >= 0)
			{
				player2Pieces[bestPlacement.pieceIndex].row = bestPlacement.toRow;

				player2Pieces[bestPlacement.pieceIndex].col = bestPlacement.toCol;

				player2Pieces[bestPlacement.pieceIndex].isPlaced = true;

				board[bestPlacement.toRow][bestPlacement.toCol] = player2Pieces[bestPlacement.pieceIndex];

				if (checkWin(Player::PLAYER2))
				{
					gameState = GameState::GAME_OVER;
					return;
				}

				switchTurn();
				return;
			}
		}
	}
}

void Game::aiMovePiece()
{
	// Use minimax to find best move
	Move bestMove = findBestMove();

	if (bestMove.pieceIndex >= 0)
	{
		makeMove(bestMove, Player::PLAYER2);

		if (checkWin(Player::PLAYER2))
		{
			gameState = GameState::GAME_OVER;
			return;
		}

		switchTurn();
	}
	else {
		// No valid moves, just pass
		switchTurn();
	}
}

///////////////////////////////////////////////////////////////////////
// helper functions
///////////////////////////////////////////////////////////////////////

int Game::getBoardIndex(int row, int col)
{
	return row * BOARD_SIZE + col;
}

sf::Vector2f Game::getBoardPosition(int row, int col)
{
	return sf::Vector2f(boardOffsetX + col * cellSize, boardOffsetY + row * cellSize);
}

void Game::drawBoard()
{
	for (int row = 0; row < BOARD_SIZE; row++)
	{
		for (int col = 0; col < BOARD_SIZE; col++)
		{
			sf::RectangleShape cell(sf::Vector2f(cellSize - 2, cellSize - 2));

			cell.setPosition(sf::Vector2f(boardOffsetX + col * cellSize, boardOffsetY + row * cellSize));
			cell.setFillColor(sf::Color(60, 60, 60));
			cell.setOutlineColor(sf::Color::White);
			cell.setOutlineThickness(1);

			window.draw(cell);
		}
	}
}

void Game::drawPieces()
{
	// Draw pieces on board
	for (int row = 0; row < BOARD_SIZE; row++)
	{
		for (int col = 0; col < BOARD_SIZE; col++)
		{
			if (board[row][col].type != PieceType::NONE)
			{
				sf::Sprite* currentSprite = nullptr;

				// Select the appropriate sprite based on piece type
				switch (board[row][col].type)
				{
				case PieceType::DONKEY:
					currentSprite = &donkeySprite;
					break;
				case PieceType::SNAKE:
					currentSprite = &snakeSprite;
					break;
				case PieceType::FROG:
					currentSprite = &frogSprite;
					break;
				}

				if (currentSprite != nullptr)
				{
					// Set position
					currentSprite->setPosition(sf::Vector2f(
						boardOffsetX + col * cellSize + 10,
						boardOffsetY + row * cellSize + 10));

					// Scale to fit cell
					sf::FloatRect bounds = currentSprite->getLocalBounds();
					float scale = (cellSize - 20) / std::max(bounds.size.x, bounds.size.y);
					currentSprite->setScale(sf::Vector2f(scale, scale));

					// Color based on owner (Blue for Player 1, Red for AI)
					if (board[row][col].owner == Player::PLAYER1)
					{
						currentSprite->setColor(sf::Color::Green);
					}
					else
					{
						currentSprite->setColor(sf::Color::Red);
					}

					window.draw(*currentSprite);

					// ? NEW: Highlight selected piece
					if (m_selectedPiece.isSelected &&
						m_selectedPiece.row == row &&
						m_selectedPiece.col == col)
					{
						sf::RectangleShape highlight(sf::Vector2f(cellSize - 2, cellSize - 2));
						highlight.setPosition(sf::Vector2f(
							boardOffsetX + col * cellSize,
							boardOffsetY + row * cellSize));
						highlight.setFillColor(sf::Color::Transparent);
						highlight.setOutlineColor(sf::Color::Yellow);
						highlight.setOutlineThickness(3);
						window.draw(highlight);
					}
				}
			}
		}
	}

	// Draw unplaced pieces on the left side (PLACING phase only)
	if (gameState == GameState::PLACING)
	{
		float pieceX = 50.0f;
		float pieceY = 100.0f;

		for (int i = 0; i < player1Pieces.size(); i++)
		{
			if (!player1Pieces[i].isPlaced)
			{
				sf::Sprite* currentSprite = nullptr;

				switch (player1Pieces[i].type)
				{
				case PieceType::DONKEY:
					currentSprite = &donkeySprite;
					break;
				case PieceType::SNAKE:
					currentSprite = &snakeSprite;
					break;
				case PieceType::FROG:
					currentSprite = &frogSprite;
					break;
				}

				if (currentSprite != nullptr)
				{
					currentSprite->setPosition(sf::Vector2f(pieceX, pieceY));
					currentSprite->setScale(sf::Vector2f(0.5f, 0.5f));
					currentSprite->setColor(sf::Color::Green);

					// Highlight selected piece
					if (selectedPieceIndex == i)
					{
						sf::RectangleShape highlight(sf::Vector2f(80, 80));
						highlight.setPosition(sf::Vector2f(pieceX - 5, pieceY - 5));
						highlight.setFillColor(sf::Color::Transparent);
						highlight.setOutlineColor(sf::Color::Yellow);
						highlight.setOutlineThickness(3);
						window.draw(highlight);
					}

					window.draw(*currentSprite);
					pieceY += 100.0f;
				}
			}
		}
	}
}


void Game::drawUI()
{
	text.setFont(font);
	text.setCharacterSize(24);
	text.setFillColor(sf::Color::White);

	if (gameState == GameState::GAME_OVER) 
	{
		text.setString("Game Over!");

		text.setPosition(sf::Vector2f(400, 30));
	}
	else
	{
		std::string state = (gameState == GameState::PLACING) ? "Placing" : "Moving";

		std::string player = (currentPlayer == Player::PLAYER1) ? "Player 1" : "AI";

		text.setString(state + " Phase - " + player + "'s Turn");
		text.setPosition(sf::Vector2f(350, 30));
	}

	window.draw(text);
}

///////////////////////////////////////////////////////////////////////
// minimax functions
///////////////////////////////////////////////////////////////////////

int Game::minimax(int depth, bool isMaximizing, int alpha, int beta)
{
	//reached max depth or game over
	if (depth == 0 || checkWin(Player::PLAYER1) || checkWin(Player::PLAYER2))
	{
		return evaluateBoard();
	}

	if (isMaximizing)
	{
		// AI's turn (maximize score)
		int maxScore = std::numeric_limits<int>::min();
		std::vector<Move> moves = getPossibleMoves(Player::PLAYER2);

		for (const Move& move : moves)
		{
			makeMove(move, Player::PLAYER2);
			int score = minimax(depth - 1, false, alpha, beta);
			undoMove(move, Player::PLAYER2);

			maxScore = std::max(maxScore, score);
			alpha = std::max(alpha, score);

			if (beta <= alpha)
			{
				break;  // Beta cutoff
			}
		}

		return maxScore;
	}
	else
	{
		// Human's turn (minimize score)
		int minScore = std::numeric_limits<int>::max();
		std::vector<Move> moves = getPossibleMoves(Player::PLAYER1);

		for (const Move& move : moves)
		{
			makeMove(move, Player::PLAYER1);
			int score = minimax(depth - 1, true, alpha, beta);
			undoMove(move, Player::PLAYER1);

			minScore = std::min(minScore, score);
			beta = std::min(beta, score);

			if (beta <= alpha)
			{
				break;  // Alpha cutoff
			}
		}

		return minScore;
	}
}

int Game::evaluateBoard()
{
	// Simple evaluation function
	int score = 0;

	// Check if someone won
	if (checkWin(Player::PLAYER2))
	{
		
		return 1000;  // AI wins
	}
	if (checkWin(Player::PLAYER1))
	{
		
		return -1000;  // Human wins
	}

	// Count threats (3 in a row, 2 in a row)
	score += countLineThreats(Player::PLAYER2, 3) * 50;  // AI 3 in a row
	score += countLineThreats(Player::PLAYER2, 2) * 10;  // AI 2 in a row

	score -= countLineThreats(Player::PLAYER1, 3) * 50;  // Human 3 in a row
	score -= countLineThreats(Player::PLAYER1, 2) * 10;  // Human 2 in a row

	// 3. DIAGONAL THREATS 
	score += countDiagonalThreats(Player::PLAYER2, 3) * 100;
	score += countDiagonalThreats(Player::PLAYER2, 2) * 20;
	score -= countDiagonalThreats(Player::PLAYER1, 3) * 100;
	score -= countDiagonalThreats(Player::PLAYER1, 2) * 20;

	// 4. CENTER CONTROL 
	score += evaluateCenterControl(Player::PLAYER2) * 15;
	score -= evaluateCenterControl(Player::PLAYER1) * 15;

	// 5. MOBILITY (more possible moves = better position)
	score += evaluateMobility(Player::PLAYER2) * 3;
	score -= evaluateMobility(Player::PLAYER1) * 3;

	// 6. PIECE VALUE (Frogs and Snakes are more valuable due to movement)
	score += evaluatePiecePositions(Player::PLAYER2) * 5;
	score -= evaluatePiecePositions(Player::PLAYER1) * 5;


	return score;
}

int Game::countLineThreats(Player player, int length)
{
	int count = 0;

	// Horizontal lines
	for (int row = 0; row < BOARD_SIZE; row++)
	{
		for (int col = 0; col <= BOARD_SIZE - length; col++)
		{
			int playerCount = 0;
			int emptyCount = 0;

			for (int i = 0; i < length; i++)
			{
				if (board[row][col + i].owner == player)
				{
					playerCount++;
				}
				else if (board[row][col + i].type == PieceType::NONE)
				{
					emptyCount++;
				}
			}

			if (playerCount == length - 1 && emptyCount == 1)
			{
				count++;
			}
		}
	}

	// Vertical lines
	for (int col = 0; col < BOARD_SIZE; col++)
	{
		for (int row = 0; row <= BOARD_SIZE - length; row++)
		{
			int playerCount = 0;
			int emptyCount = 0;

			for (int i = 0; i < length; i++)
			{
				if (board[row + i][col].owner == player)
				{
					playerCount++;
				}
				else if (board[row + i][col].type == PieceType::NONE)
				{
					emptyCount++;
				}
			}

			if (playerCount == length - 1 && emptyCount == 1)
			{
				count++;
			}
		}
	}

	return count;
}

std::vector<Move> Game::getPossibleMoves(Player player)
{
	std::vector<Move> moves;
	std::vector<Piece>& pieces = (player == Player::PLAYER2) ? player2Pieces : player1Pieces;

	for (int i = 0; i < pieces.size(); i++)
	{
		if (pieces[i].isPlaced)
		{
			// Try all possible destination squares
			for (int row = 0; row < BOARD_SIZE; row++)
			{
				for (int col = 0; col < BOARD_SIZE; col++)
				{
					if (isValidMove(pieces[i], row, col))
					{
						Move move(i, pieces[i].row, pieces[i].col, row, col);
						moves.push_back(move);
					}
				}
			}
		}
	}

	return moves;
}

void Game::makeMove(const Move& move, Player player)
{
	std::vector<Piece>& pieces = (player == Player::PLAYER2) ? player2Pieces : player1Pieces;

	// Clear old position
	board[move.fromRow][move.fromCol] = Piece();

	// Update piece
	pieces[move.pieceIndex].row = move.toRow;
	pieces[move.pieceIndex].col = move.toCol;

	// Place at new position
	board[move.toRow][move.toCol] = pieces[move.pieceIndex];
}

void Game::undoMove(const Move& move, Player player)
{
	std::vector<Piece>& pieces = (player == Player::PLAYER2) ? player2Pieces : player1Pieces;

	// Clear new position
	board[move.toRow][move.toCol] = Piece();

	// Restore piece to old position
	pieces[move.pieceIndex].row = move.fromRow;
	pieces[move.pieceIndex].col = move.fromCol;

	// Place back at old position
	board[move.fromRow][move.fromCol] = pieces[move.pieceIndex];
}

Move Game::findBestMove()
{
	Move bestMove;
	int bestScore = std::numeric_limits<int>::min();

	std::vector<Move> moves = getPossibleMoves(Player::PLAYER2);

	for (const Move& move : moves)
	{
		makeMove(move, Player::PLAYER2);
		int score = minimax(MAX_DEPTH - 1, false,
			std::numeric_limits<int>::min(),
			std::numeric_limits<int>::max());
		undoMove(move, Player::PLAYER2);

		if (score > bestScore)
		{
			bestScore = score;
			bestMove = move;
		}
	}

	return bestMove;
}

///////////////////////////////////////////////////////////////////////
// advanced evaluation functions
///////////////////////////////////////////////////////////////////////

int Game::countDiagonalThreats(Player player, int length)
{
	int count = 0;

	// Diagonal (top-left to bottom-right) ?
	for (int row = 0; row <= BOARD_SIZE - length; row++)
	{
		for (int col = 0; col <= BOARD_SIZE - length; col++)
		{
			int playerCount = 0;
			int emptyCount = 0;

			for (int i = 0; i < length; i++)
			{
				if (board[row + i][col + i].owner == player)
				{
					playerCount++;
				}
				else if (board[row + i][col + i].type == PieceType::NONE)
				{
					emptyCount++;
				}
			}

			if (playerCount == length - 1 && emptyCount == 1)
			{
				count++;
			}
		}
	}

	// Diagonal (top-right to bottom-left) ?
	for (int row = 0; row <= BOARD_SIZE - length; row++)
	{
		for (int col = length - 1; col < BOARD_SIZE; col++)
		{
			int playerCount = 0;
			int emptyCount = 0;

			for (int i = 0; i < length; i++)
			{
				if (board[row + i][col - i].owner == player)
				{
					playerCount++;
				}
				else if (board[row + i][col - i].type == PieceType::NONE)
				{
					emptyCount++;
				}
			}

			if (playerCount == length - 1 && emptyCount == 1)
			{
				count++;
			}
		}
	}

	return count;
}

int Game::evaluateCenterControl(Player player)
{
	int centerScore = 0;
	int center = BOARD_SIZE / 2; // Center cell (2 on a 5x5 board)

	// Pieces closer to center are more valuable
	for (int row = 0; row < BOARD_SIZE; row++)
	{
		for (int col = 0; col < BOARD_SIZE; col++)
		{
			if (board[row][col].owner == player)
			{
				// Distance from center (Manhattan distance)
				int distFromCenter = abs(row - center) + abs(col - center);

				// Closer to center = higher score
				centerScore += (4 - distFromCenter); // Max distance is 4 on 5x5
			}
		}
	}

	return centerScore;
}

int Game::evaluateMobility(Player player)
{
	std::vector<Move> moves = getPossibleMoves(player);
	return static_cast<int>(moves.size());
}

int Game::evaluatePiecePositions(Player player)
{
	int score = 0;

	for (int row = 0; row < BOARD_SIZE; row++)
	{
		for (int col = 0; col < BOARD_SIZE; col++)
		{
			if (board[row][col].owner == player)
			{
				// Frogs are most valuable (can jump)
				if (board[row][col].type == PieceType::FROG)
				{
					score += 3;
				}
				// Snakes are moderately valuable (8-way movement)
				else if (board[row][col].type == PieceType::SNAKE)
				{
					score += 2;
				}
				// Donkeys are least valuable (4-way movement only)
				else if (board[row][col].type == PieceType::DONKEY)
				{
					score += 1;
				}
			}
		}
	}

	return score;
}

///////////////////////////////////////////////////////////////////////
// setup menu functions
///////////////////////////////////////////////////////////////////////


void Game::setupMenu()
{
	// Title text
	titleText.setFont(font);
	titleText.setString("THE FOURTH PROTOCOL");
	titleText.setCharacterSize(48);
	titleText.setFillColor(sf::Color::White);
	titleText.setPosition(sf::Vector2f(260, 150));

	// Start button
	startButton.setSize(sf::Vector2f(300, 80));
	startButton.setPosition(sf::Vector2f(360, 320));
	startButton.setFillColor(sf::Color(50, 150, 50));
	startButton.setOutlineColor(sf::Color::White);
	startButton.setOutlineThickness(3);

	startButtonText.setFont(font);
	startButtonText.setString("START GAME");
	startButtonText.setCharacterSize(36);
	startButtonText.setFillColor(sf::Color::White);
	// Center text in button
	sf::FloatRect textBounds = startButtonText.getLocalBounds();
	startButtonText.setPosition(sf::Vector2f(startButton.getPosition().x + (startButton.getSize().x - textBounds.size.x) / 2, startButton.getPosition().y + (startButton.getSize().y - textBounds.size.y) / 2 - 10));

	// Exit button
	exitButton.setSize(sf::Vector2f(300, 80));
	exitButton.setPosition(sf::Vector2f(360, 440));
	exitButton.setFillColor(sf::Color(150, 50, 50));
	exitButton.setOutlineColor(sf::Color::White);
	exitButton.setOutlineThickness(3);

	exitButtonText.setFont(font);
	exitButtonText.setString("EXIT");
	exitButtonText.setCharacterSize(36);
	exitButtonText.setFillColor(sf::Color::White);
	// Center text in button
	textBounds = exitButtonText.getLocalBounds();
	exitButtonText.setPosition(sf::Vector2f(exitButton.getPosition().x + (exitButton.getSize().x - textBounds.size.x) / 2, exitButton.getPosition().y + (exitButton.getSize().y - textBounds.size.y) / 2 - 10));
}

void Game::updateMenu(sf::Vector2i mousePos)
{
	// Hover effect for Start button
	if (startButton.getGlobalBounds().contains(sf::Vector2f(mousePos)))
	{
		startButton.setFillColor(sf::Color(70, 200, 70)); // Brighter green
	}
	else
	{
		startButton.setFillColor(sf::Color(50, 150, 50)); // Normal green
	}

	// Hover effect for Exit button
	if (exitButton.getGlobalBounds().contains(sf::Vector2f(mousePos)))
	{
		exitButton.setFillColor(sf::Color(200, 70, 70)); // Brighter red
	}
	else
	{
		exitButton.setFillColor(sf::Color(150, 50, 50)); // Normal red
	}
}

void Game::drawMenu()
{
	window.draw(titleText);
	window.draw(startButton);
	window.draw(startButtonText);
	window.draw(exitButton);
	window.draw(exitButtonText);
}