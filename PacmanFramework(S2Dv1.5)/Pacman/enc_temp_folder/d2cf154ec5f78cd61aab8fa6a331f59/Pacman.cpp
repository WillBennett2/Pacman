#include "Pacman.h"
using namespace std;
#include <iostream>
#include <sstream>

Pacman::Pacman(int argc, char* argv[]) : Game(argc, argv), _cPacmanSpeed(0.1f),_cPacmanFrameTime(250), _cMunchieFrameTime(500),_cCherryFrameTime(500)
{
	//local variable
	for (int i = 0; i <MUNCHIECOUNT; i++)
	{
		_munchies[i] = new Enemy();
		_munchies[i]->frameCount = rand() % 1;
		_munchies[i]->currentFrameTime = 0;
		_munchies[i]->frameTime = rand() % 500 + 50;
	}

	for (int i = 0; i < GHOSTCOUNT; i++)
	{
		_ghosts[i] = new MovingEnemy();
		_ghosts[i]->direction = 0;
		_ghosts[i]->speed = 0.2f;
	}
	
	//Initialise member variables
	_pacman = new Player();
	_pacman->dead = false;
	_pacman->moving = true;
	_pacman->previousDirection = -1;

	_cherry = new Enemy();

	_menu = new Menu();

	_walls = new Walls();
	for (int i = 0; i <WALLCOUNT; i++)
	{
		_wallCoord[i] = new WallCoord();
	}

	_cherry->frame = 0;
	_cherry->currentFrameTime = 0;

	_menu ->paused = false;
	_menu-> pKeyDown = false;
	_menu->started = false;
	_menu->randomised = false;

	//Initialise important Game aspects
	Graphics::Initialise(argc, argv, this, 1024, 768, false, 25, 25, "Pacman", 60);
	Input::Initialise();

	// Start the Game Loop - This calls Update and Draw in game loop
	Graphics::StartGameLoop();
}

Pacman::~Pacman() //clearing memory when the program ends
{
	delete _pacman->texture;
	delete _pacman->sourceRect;
	delete _pacman->position;
	delete _pacman;
	
	delete _ghosts[0]->texture;
	for (int nCount = 0; nCount < GHOSTCOUNT; nCount++)
	{
		delete _ghosts[nCount]->sourceRect;
		delete _ghosts[nCount]->position;
		delete _ghosts[nCount];
	}
	delete[] _ghosts;

	delete _munchies[0]->texture;
	for (int nCount = 0; nCount < MUNCHIECOUNT; nCount++)
	{
		delete _munchies[nCount]->position;
		delete _munchies[nCount]->sourceRect;
		delete _munchies[nCount];
	}
	delete[] _munchies;

	delete _cherry->texture;
	delete _cherry->invertedTexture;
	delete _cherry->sourceRect;
	delete _cherry;

	delete _menu;

	delete _walls->texture;
	delete _walls;
}

void Pacman::LoadContent()
{
	// Load Pacman
	_pacman->texture = new Texture2D();
	_pacman->texture->Load("Textures/Pacman.png", false);
	_pacman->position = new Vector2(350.0f, 350.0f);
	_pacman->sourceRect = new Rect(0.5f, 0.5f, 35, 36);

	// Load ghost
	for (int i = 0; i< GHOSTCOUNT; i++)
	{
		Texture2D* GhostTex = new Texture2D();
		switch (i)
		{
		case(0):
			GhostTex->Load("Textures/Red.png", false);
			break;
		case(1):
			GhostTex->Load("Textures/Cyan.png", false);
			break;
		case(2):
			GhostTex->Load("Textures/Pink.png", false);
			break;
		case(3):
			GhostTex->Load("Textures/Orange.png", false);
			break;
		}
		_ghosts[i]->texture = GhostTex;
		_ghosts[i]->position = new Vector2((300), (rand() % Graphics::GetViewportHeight()));//rand() % Graphics::GetViewportWidth()
		_ghosts[i]->sourceRect = new Rect(0.0f, 0.7f, 20, 20);


	}

	// Load Munchie
	Texture2D* munchieTex = new Texture2D();
	munchieTex->Load("Textures/Munchie.tga", false);
	Texture2D* invertedMunchieTex = new Texture2D();
	invertedMunchieTex->Load("Textures/MunchieInverted.tga", true);
	for (int i = 0; i < MUNCHIECOUNT; i++)
	{
		_munchies[i]->position = new Vector2((rand() % Graphics::GetViewportWidth()), (rand() % Graphics::GetViewportHeight()));
		_munchies[i]->texture = munchieTex;
		_munchies[i]->invertedTexture = invertedMunchieTex;
		_munchies[i]->sourceRect = new Rect(_munchies[i]->position->X, _munchies[i]->position->Y, 12, 12);
	}

	//load Cherry
	_cherry->texture = new Texture2D();
	_cherry->texture->Load("Textures/Cherry.png", true);
	_cherry->invertedTexture = new Texture2D();
	_cherry->invertedTexture->Load("Textures/CherryInverted.png", true);

	_cherry->position = new Vector2((rand() % Graphics::GetViewportWidth()), (rand() % Graphics::GetViewportHeight()));
	_cherry->sourceRect = new Rect(_cherry->position->X, _cherry->position->Y, 28, 28);

	// Set string position
	_stringPosition = new Vector2(10.0f, 25.0f);

	// Set walls image 
	_walls->texture = new Texture2D();
	_walls->texture->Load("Textures/Map.png", true);
	_walls->rectangle = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth(), Graphics::GetViewportHeight());

	// Set wall positions
	LoadWallCoord();
	for (int i = 0; i < 11; i++)
	{
		_wallCoord[i]->X = wallArray[i][0];
		_wallCoord[i]->Y = wallArray[i][1];
		_wallCoord[i]->width = wallArray[i][2];
		_wallCoord[i]->height = wallArray[i][3];
	}


	// Set Menu Paramters
	_menu->background = new Texture2D();
	_menu->background->Load("Textures/Transparency.png", false);
	_menu->rectangle = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth(), Graphics::GetViewportHeight());
	_menu->stringPosition = new Vector2(Graphics::GetViewportWidth() / 2.0f, Graphics::GetViewportHeight() / 2.0f);

}
void Pacman::LoadWallCoord()
{
	int windowWidth = Graphics::GetViewportWidth();
	//left x y width height
	// outer rim
	wallArray[0][0] = 0, wallArray[0][1] = 0, wallArray[0][2] = windowWidth, wallArray[0][3] = 21;
	wallArray[1][0] = 0, wallArray[1][1] = 21, wallArray[1][2] = 29, wallArray[1][3] = 251;
	wallArray[2][0] = windowWidth - 32, wallArray[2][1] = 21, wallArray[2][2] = 29, wallArray[2][3] = 251;
	wallArray[3][0] = 0, wallArray[3][1] = 272, wallArray[3][2] = 191, wallArray[3][3] = 78;
	wallArray[4][0] = 829, wallArray[4][1] = 272, wallArray[4][2] = 191, wallArray[4][3] = 78;

	wallArray[5][0] = 0, wallArray[5][1] = 408, wallArray[5][2] = 191, wallArray[5][3] = 78;
	wallArray[6][0] = 829, wallArray[6][1] = 408, wallArray[6][2] = 191, wallArray[6][3] = 78;
	wallArray[7][0] = 0, wallArray[7][1] = 486, wallArray[7][2] = 29, wallArray[7][3] = 258;
	wallArray[8][0] = 0, wallArray[8][1] = 615, wallArray[8][2] = 29, wallArray[8][3] = 258;
	wallArray[9][0] = 21, wallArray[9][1] = 615, wallArray[9][2] = 80, wallArray[9][3] = 39;
	wallArray[10][0] = 923, wallArray[10][1] = 615, wallArray[10][2] = 80, wallArray[10][3] = 39;
	wallArray[11][0] = 0, wallArray[11][1] = 742, wallArray[11][2] = windowWidth, wallArray[11][3] = 21;


	// top inner half

	//cage for ghosts

	//bottom inner half

}

void Pacman::Update(int elapsedTime)
{
	// Gets the current state of the keyboard
	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();
	// Gets the current state of the mouse
	Input::MouseState* mouseState = Input::Mouse::GetState();

	if (!_menu->started) //cant take inputs in if game hasnt started
	{
		CheckStart(keyboardState, Input::Keys::SPACE);
	}
	else
	{
		CheckPaused(keyboardState, Input::Keys::P);

		if (!_menu->paused)
		{
			Input(elapsedTime, keyboardState, mouseState);
			if (_menu->randomised)
				_menu->randomised = false;

			if (_pacman->moving == true)
			{
				_pacman->previousDirection = _pacman->direction;

				if (CheckWallCollision(_pacman->position->X, _pacman->position->Y, _pacman->sourceRect->Width, _pacman->sourceRect->Height))
					_pacman->moving = false;
			}
			else if (!CheckWallCollision(_pacman->position->X, _pacman->position->Y, _pacman->sourceRect->Width, _pacman->sourceRect->Height))
				_pacman->moving = true;

			UpdatePacman(elapsedTime);
			for (int i = 0; i < GHOSTCOUNT; i++) 
			{
				if (!CheckWallCollision(_ghosts[i]->position->X, _ghosts[i]->position->Y, _ghosts[i]->sourceRect->Width, _ghosts[i]->sourceRect->Height) )
				{
					//UpdateGhost(_ghosts[i], elapsedTime);
				}
					
				CheckGhostCollision();

			}

			for (int i = 0; i < MUNCHIECOUNT; i++)
			{
				if (CollisionCheck(_pacman->position->X, _pacman->position->Y, _pacman->sourceRect->Width,  _pacman->sourceRect->Height,
					_munchies[i]->position->X, _munchies[i]->position->Y, _munchies[i]->sourceRect->Width, _munchies[i]->sourceRect->Height)) {

					_pacman->collision = 'Y';
					_munchies[i]->position->X = -100;
					_munchies[i]->position->Y = -100;
				}
				else
					_pacman->collision = 'N';

				UpdateMunchie(_munchies[i], elapsedTime);

			}

			if (CollisionCheck(_pacman->position->X, _pacman->position->Y, _pacman->sourceRect->Width, _pacman->sourceRect->Height, 
				_cherry->position->X, _cherry->position->Y, _cherry->sourceRect->Width, _cherry->sourceRect->Height))
			{
				_pacman->collision = 'H';
				_cherry->position->X = -100;
				_cherry->position->Y = -100;
			}
			else
				_pacman->collision = 'N';

			UpdateCherry(elapsedTime);
			CheckViewportCollision();
			//check if dead
		}
	}

}

void Pacman::CheckStart(Input::KeyboardState* state, Input::Keys startKey)
{
	if (state->IsKeyDown(Input::Keys::SPACE))
	{
		_menu ->started = true;
	}
}

void Pacman::Input(int elapsedTime, Input::KeyboardState*state, Input::MouseState*mouseState)
{
	float pacmanSpeed = _cPacmanSpeed * elapsedTime* _pacman->speedMulitplier;
	if (state->IsKeyDown(Input::Keys::LEFTSHIFT))
		pacmanSpeed = 2.0f; //apply multiplier

	else
		pacmanSpeed = 1.0f; //reset multiplier

	if (state->IsKeyDown(Input::Keys::R) && !_menu->randomised)
	{
		_cherry->position->X = rand() % Graphics::GetViewportWidth();
		_cherry->position->Y = rand() % Graphics::GetViewportHeight();
		_menu->randomised = true;
	}

	/*if (mouseState->LeftButton == Input::ButtonState::PRESSED) 
	{
		_cherry->position->X = mouseState->X;
		_cherry->position->Y = mouseState->Y;
	}*/

	if (state->IsKeyDown(Input::Keys::D) || state->IsKeyDown(Input::Keys::RIGHT)) { // Checks if D/right key is pressed
				//_pacmanPosition->X += _cPacmanSpeed * elapsedTime; //Moves Pacman across X axis
		_pacman->direction = 4;//variable used for switch statements to keep pacman once a direction has been picked
	}

	else if (state->IsKeyDown(Input::Keys::A) || state->IsKeyDown(Input::Keys::LEFT)) {// Checks if A/left key is pressed
		//_pacmanPosition->X -= _cPacmanSpeed * elapsedTime;
		_pacman->direction = 2;
	}
	else if (state->IsKeyDown(Input::Keys::W) || state->IsKeyDown(Input::Keys::UP)) {//checks if W/up key is pressed
		//_pacmanPosition->Y -= _cPacmanSpeed * elapsedTime;
		_pacman->direction = 3;
	}

	else if (state->IsKeyDown(Input::Keys::S) || state->IsKeyDown(Input::Keys::DOWN)) { //checks if S/down key is pressed
		//_pacmanPosition->Y += _cPacmanSpeed * elapsedTime;
		_pacman->direction = 1;
	}
	
	//if (_pacman->previousDirection != _pacman->direction)
		//_pacman->moving = true;

	if (_pacman->moving == true || _pacman->previousDirection != _pacman->direction)
		//_pacman->moving = true;
		switch (_pacman->direction)
		{
		case 4:
			_pacman->position->X += pacmanSpeed; //Moves Pacman across X axis(right)
			_pacman->direction = 4;//rotating pacman right 
			break;
		case 2:
			_pacman->position->X -= pacmanSpeed;//Moves Pacman across X axis(left)
			_pacman->direction = 2;//rotating pacman left 
			break;
		case 3:
			_pacman->position->Y -= pacmanSpeed;//Moves Pacman across Y axis(up)
			_pacman->direction = 3;//rotating pacman up 
			break;
		case 1:
			_pacman->position->Y += pacmanSpeed;//Moves Pacman across Y axis(down)
			_pacman->direction = 1;//rotating pacman down 
			break;

		default:
			break;
		}
	
	
}

void Pacman::CheckPaused(Input::KeyboardState*state , Input::Keys pauseKey)
{

	if (state->IsKeyDown(Input::Keys::P) && !_menu->pKeyDown)
	{
		_menu->pKeyDown = true;
		_menu->paused = !_menu->paused;
	}
	if (state->IsKeyDown(Input::Keys::P))
		_menu->pKeyDown = false;
}

void Pacman::UpdatePacman(int elapsedTime)
{
	//animation related to direction of pacman's movement
	_pacman->currentFrameTime += elapsedTime;
	if (_pacman->currentFrameTime > _cPacmanFrameTime)
	{
		_pacman->frame++;
		if (_pacman->frame >= 2) //2 represents the column in the spritesheet
			_pacman->frame = 0;//resets
		_pacman->currentFrameTime = 0;
	}
	_pacman->sourceRect->X = _pacman->sourceRect->Width * _pacman->frame; //opens and closes mouth.
	_pacman->sourceRect->Y = _pacman->sourceRect->Height * _pacman->direction; //rotates pacman.
}
void Pacman::UpdateGhost(MovingEnemy*ghost, int elapsedTime)
{
	if (ghost->direction == 0)//moves right
	{
		ghost->position->X += ghost->speed * elapsedTime;
		ghost->sourceRect = new Rect(0.0f, 0.7f, 20, 23);
	}
	else if (ghost->direction == 1)//moves left
	{
		ghost->position->X -= ghost->speed * elapsedTime;
		ghost->sourceRect = new Rect(0.0f, 66.7f, 20, 23);
	}
	if (ghost->position->X + ghost->sourceRect->Width >= Graphics::GetViewportWidth())//hits right edge
		ghost->direction = 1;//change direction
	else if (ghost->position->X <= 0)//hits left edge
		ghost->direction = 0;//change direction
}
void Pacman::UpdateMunchie(Enemy*_munchie, int elapsedTime)
{

	_munchie->currentFrameTime += elapsedTime;
	if (_munchie->currentFrameTime > _cMunchieFrameTime)
	{
		_munchie->frameCount++;
		if (_munchie->frameCount >= 2)
			_munchie->frameCount = 0;
		_munchie->currentFrameTime = 0;
	}
	_munchie->sourceRect->X = _munchie->position->X;
	_munchie->sourceRect->Y = _munchie->position->Y;


}
void Pacman::UpdateCherry(int elapsedTime)
{
	_cherry->currentFrameTime += elapsedTime;
	if (_cherry->currentFrameTime > _cMunchieFrameTime)
	{
		_cherry->frameCount++;
		if (_cherry->frameCount >= 2)
			_cherry->frameCount = 0;
		_cherry->currentFrameTime = 0;
	}
	_cherry->sourceRect->X = _cherry->position->X;
	_cherry->sourceRect->Y = _cherry->position->Y;
}

void Pacman::CheckViewportCollision()
{

	if (_pacman->position->X > Graphics::GetViewportWidth() + (_pacman->sourceRect->Width) / 2) //checks if pacman is slightly off the screen (right)
	{
		//_pacmanPosition-> X = 1024 -_pacmanSourceRect->Width; stops pacman at the edge of screen
		_pacman->position->X = 0; // teleports pacman to other edge 'loop'
	}
	else if (_pacman->position->X < 0 - (_pacman->sourceRect->Width) / 2)//checks if pacman is slightly off the screen (left)
	{
		_pacman->position->X = Graphics::GetViewportWidth(); // teleports pacman to other edge 'loop'
	}

	if (_pacman->position->Y > Graphics::GetViewportHeight() + (_pacman->sourceRect->Height) / 2)//checks if pacman is slightly off the screen (down)
	{
		_pacman->position->Y = 0; // teleports pacman to other edge 'loop'
	}
	else if (_pacman->position->Y < 0 - (_pacman->sourceRect->Height) / 2)//checks if pacman is slightly off the screen (up)
	{
		_pacman->position->Y = Graphics::GetViewportHeight(); // teleports pacman to other edge 'loop'
	}
}
bool Pacman::CollisionCheck(int x1, int y1, int width1, int height1, int x2, int y2, int width2, int height2)
{
	int _pacmanLeft = x1;
	int _pacmanTop = y1;
	int _pacmanRight = x1 + width1;
	int _pacmanBottom = y1 + height1;
	//this can be munchie or an enemy
	int _targetLeft = x2;
	int _targetTop = y2;
	int _targetRight = x2 + width2;
	int _targetBottom = y2 + height2;

	if (_pacmanBottom < _targetTop)
	{
		return false;
	}
	if (_pacmanTop > _targetBottom)
	{
		return false;
	}
	if (_pacmanRight < _targetLeft)
	{
		return false;
	}
	if (_pacmanLeft > _targetRight)
	{
		return false;
	}
	return true;

}
void Pacman::CheckGhostCollision()
{
	int top1 = _pacman->position->Y;
	int top2 = 0;
	int bottom1 = _pacman->position->Y + _pacman->sourceRect->Height;
	int bottom2 = 0;
	int left1 = _pacman->position->X;
	int left2 = 0;
	int right1 = _pacman->position->X + _pacman->sourceRect->Width;
	int right2 = 0;

	for (int i = 0; i < GHOSTCOUNT; i++)
	{
		top2 = _ghosts[i]->position->Y;
		bottom2 = _ghosts[i]->position->Y + _ghosts[i]->sourceRect->Height;
		left2 = _ghosts[i]->position->X;
		right2 = _ghosts[i]->position->X + _ghosts[i]->sourceRect->Width;

		if ((bottom1 > top2) && (top1 < bottom2) && (right1 > left2) && (left1 < right2))
		{
			_pacman->dead = true;
			i = GHOSTCOUNT;
		}
	}
}
bool Pacman::CheckWallCollision(int x1, int y1, int width1, int height1)
{
	bool collision = false;
	int top1 =y1;
	int top2 = 0;
	int bottom1 =y1+height1;
	int bottom2 = 0;
	int left1 = x1;
	int left2 = 0;
	int right1 = x1 + width1;
	int right2 = 0;

	for (int i = 0; i < WALLCOUNT; i++)
	{
		top2 = _wallCoord[i]->Y;
		bottom2 = _wallCoord[i]->Y + _wallCoord[i]->height;
		left2 = _wallCoord[i]->X;
		right2 = _wallCoord[i]->X + _wallCoord[i]->width;

		if ((bottom1 > top2) && (top1 < bottom2) && (right1 > left2) && (left1 < right2))
		{
			i = WALLCOUNT;
			return true;
		}
	}
	return collision;
}

void Pacman::Draw(int elapsedTime)
{
	// Allows us to easily create a string
	std::stringstream stream;
	stream << "Pacman X: " << _pacman->position->X << " Y: " << _pacman->position->Y << " collision: "<< _pacman->collision;
	POINT p;
	GetCursorPos(&p);
	Input::MouseState* mouseState = Input::Mouse::GetState();
	if (mouseState->LeftButton == Input::ButtonState::PRESSED)
	{
		system("cls");
		cout << "Mouse X: " << p.x << " Mouse Y : " << p.y - 31;
	}
	stream << " Mouse X: " << p.x << " Mouse Y : " << p.y-31;
	SpriteBatch::BeginDraw(); // Starts Drawing

	if (!_pacman->dead)
		SpriteBatch::Draw(_pacman->texture, _pacman->position, _pacman->sourceRect); // Draws Pacman

	for (int i = 0; i < GHOSTCOUNT; i++)
	{
		SpriteBatch::Draw(_ghosts[i]->texture, _ghosts[i]->position, _ghosts[i]->sourceRect);
	}

	for (int i = 0; i < MUNCHIECOUNT; i++)
	{
		if (_munchies[i]->frameCount == 0)
		{
			// Draws Red Munchie
			SpriteBatch::Draw(_munchies[i]->invertedTexture, _munchies[i]->sourceRect, nullptr, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);
		}
		else
		{
			// Draw Blue Munchie
			SpriteBatch::Draw(_munchies[i]->texture, _munchies[i]->sourceRect, nullptr, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);

			_munchies[i]->frameCount++;
			if (_munchies[i]->frameCount >= 60)
				_munchies[i]->frameCount = 0;

		}
	}

	if (_cherry->frameCount == 0) 
	{
		SpriteBatch::Draw(_cherry->invertedTexture, _cherry->sourceRect, nullptr, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);
	}
	else
	{
		// Draw Blue Munchie
		SpriteBatch::Draw(_cherry->texture, _cherry->sourceRect, nullptr, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);

		_cherry->frameCount++;
		if (_cherry->frameCount >= 60)
			_cherry->frameCount = 0;

	}

	if (_menu->started) 
	{
		SpriteBatch::Draw(_walls->texture, _walls->rectangle, nullptr);
	}
	
	
	// Draws String
	SpriteBatch::DrawString(stream.str().c_str(), _stringPosition, Color::Green);
	if (!_menu->started)
	{
		std::stringstream menuStream;
		menuStream << "Press space to start!";
		SpriteBatch::Draw(_menu->background, _menu->rectangle, nullptr);
		SpriteBatch::DrawString(menuStream.str().c_str(), _menu->stringPosition, Color::Green);
		for (int i = 0; i < MUNCHIECOUNT; i++)
		{
			_munchies[i]->frameCount++;
		}
	}	

	if (_menu->paused)
	{
		std::stringstream menuStream;
		menuStream << "PAUSED!";
		SpriteBatch::Draw(_menu->background, _menu->rectangle, nullptr);
		SpriteBatch::DrawString(menuStream.str().c_str(), _menu->stringPosition, Color::Red);
		for (int i = 0; i < MUNCHIECOUNT; i++)
		{
			_munchies[i]->frameCount++;
		}
		_cherry->frameCount++;
	}
	SpriteBatch::EndDraw(); // Ends Drawing
}