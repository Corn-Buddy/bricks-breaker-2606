#include "stdafx.h"
#include "Game.h"

Game::Game()
{
	Reset();
}

void Game::Reset()
{
	Console::SetWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	Console::CursorVisible(false);
	paddle.width = 12;
	paddle.height = 2;
	paddle.x_position = 32;
	paddle.y_position = 30;

	ball.visage = 'O';
	ball.color = ConsoleColor::Cyan;
	ResetBall();

	gameOver = false;												//resets the game over state
	playerWon = false;												//resets the win state

	// TODO #2 - Add this brick and 4 more bricks to the vector
	bricks.clear();													//clears old bricks before adding new ones

	int brickWidth = 10;
	int brickHeight = 2;
	int brickY = 5;													//same brick dimensions on the same row
	int spacing = 4;												//space between each brick

	for (int i = 0; i < 5; i++){									//create 5 bricks
	
		Box newBrick;												//create one brick object

		newBrick.width = brickWidth;
		newBrick.height = brickHeight;
		newBrick.x_position = i * (brickWidth + spacing);			//setup brick objects and spacing
		newBrick.y_position = brickY;								//sets the brick row position
		newBrick.doubleThick = true;								//makes the brick take multiple hits
		newBrick.color = ConsoleColor::DarkGreen;					//sets the brick color

		bricks.push_back(newBrick);									//adds the brick to the vector
	}
}

void Game::ResetBall()
{
	ball.x_position = paddle.x_position + paddle.width / 2;
	ball.y_position = paddle.y_position - 1;
	ball.x_velocity = rand() % 2 ? 1 : -1;
	ball.y_velocity = -1;
	ball.moving = false;
}

bool Game::Update()
{
	if (GetAsyncKeyState(VK_ESCAPE) & 0x1)
		return false;

	if (GetAsyncKeyState(VK_RIGHT) && paddle.x_position < WINDOW_WIDTH - paddle.width)
		paddle.x_position += 2;

	if (GetAsyncKeyState(VK_LEFT) && paddle.x_position > 0)
		paddle.x_position -= 2;

	if (GetAsyncKeyState(VK_SPACE) & 0x1)
		ball.moving = !ball.moving;

	if (GetAsyncKeyState('R') & 0x1)
		Reset();

	ball.Update();
	CheckCollision();
	return true;
}

//  All rendering, including text, should occur in the Render function
void Game::Render() const
{
	Console::Lock(true);
	Console::Clear();
	
	paddle.Draw();
	ball.Draw();

	// TODO #3 - Update render to render all bricks
	for (int i = 0; i < bricks.size(); i++){						//loops through every brick in the vector
	
		bricks[i].Draw();											//draws the current brick
	}


	if (gameOver) {													//checks if the game has ended

		Console::SetCursorPosition(24, 15);							//moves the message near the middle of the screen

		if (playerWon) {											//checks if the player won

			std::cout << "You win! Press 'R' to play again.";		//prints the win message
		}
		else {														//runs if the player lost

			std::cout << "You lose. Press 'R' to play again.";		//prints the lose message
		}
	}

	Console::Lock(false);
}

void Game::CheckCollision()
{
	// TODO #4 - Update collision to check all bricks
	for (int i = 0; i < bricks.size(); i++){																	//loops through each brick in the vector
	
		if (bricks[i].Contains(ball.x_position + ball.x_velocity, ball.y_position + ball.y_velocity)){			//checks if the ball will hit this brick
		
			bricks[i].color = ConsoleColor(bricks[i].color - 1);												//changes the brick color after getting hit
			ball.y_velocity *= -1;																				//bounces the ball back after hitting the brick

			// TODO #5 - If the ball hits the same brick 3 times (color == black), remove it from the vector
			if (bricks[i].color == ConsoleColor::Black){														//checks if the brick has reached black
			
				bricks.erase(bricks.begin() + i);																//removes the brick from the vector
			}

			break;																								//stops checking after one brick is hit
		}
	}

	// TODO #6 - If no bricks remain, pause ball and display (render) victory text with R to reset
	if (bricks.size() == 0) {																					//checks if all bricks have been removed

		gameOver = true;																					
		playerWon = true;																						//marks the correct game state
		ball.x_velocity = 0;																					
		ball.y_velocity = 0;																					//stops the ball from moving 
	}

	if (paddle.Contains(ball.x_position + ball.x_velocity, ball.y_velocity + ball.y_position))
	{
		ball.y_velocity *= -1;
	}

	// TODO #7 - If ball touches bottom of window, pause ball and display (render) defeat text with R to reset
	if (ball.y_position + ball.y_velocity >= WINDOW_HEIGHT - 1) {												//checks if the ball will touch the bottom of the window

		gameOver = true;																						
		playerWon = false;																						//marks the correct game state
		ball.x_velocity = 0;																				
		ball.y_velocity = 0;																					//stops the ball from moving vertically
	}
}
