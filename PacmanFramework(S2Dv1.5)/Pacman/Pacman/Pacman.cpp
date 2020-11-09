#include "Pacman.h"
using namespace std;
#include <iostream>
#include <sstream>

Pacman::Pacman(int argc, char* argv[]) : Game(argc, argv), _cPacmanSpeed(0.1f),_cPacmanFrameTime(250), _cMunchieFrameTime(500)
{
	//Initialise member variables
	_pacman = new Player();
	_munchie = new Munchie();
	_menu = new Menu();

	_munchie->frameCount = 0;
	_pacman->currentFrameTime = 0;
	_pacman->frame = 0;

	_munchie->currentFrameTime = 0;

	_menu ->paused = false;
	_menu-> pKeyDown = false;
	_menu->started = false;

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

	delete _munchie->blueTexture;
	delete _munchie->invertedTexture;
	delete _munchie->rect;
	delete _munchie;

	delete _menu;
}

void Pacman::LoadContent()
{
	// Load Pacman
	_pacman->texture = new Texture2D();
	_pacman->texture->Load("Textures/Pacman.tga", false);
	_pacman->position = new Vector2(350.0f, 350.0f);
	_pacman->sourceRect = new Rect(0.0f, 0.0f, 32, 32);
	
	// Load Munchie
	_munchie->blueTexture = new Texture2D();
	_munchie->blueTexture->Load("Textures/Munchie.tga", true);
	_munchie->invertedTexture = new Texture2D();
	_munchie->invertedTexture->Load("Textures/MunchieInverted.tga", true);
	_munchie->rect = new Rect(100.0f, 450.0f, 12, 12);

	// Set string position
	_menu->_stringPosition = new Vector2(10.0f, 25.0f);

	// Set Menu Paramters
	_menu->background = new Texture2D();
	_menu->background->Load("Textures/Transparency.png", false);
	_menu->rectangle = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth(), Graphics::GetViewportHeight());
	_menu->stringPosition = new Vector2(Graphics::GetViewportWidth() / 2.0f, Graphics::GetViewportHeight() / 2.0f);

}

void Pacman::Update(int elapsedTime)
{
	// Gets the current state of the keyboard
	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();
	

	if (!_menu->started) //cant take inputs in if game hasnt started
	{
		CheckStart(keyboardState, Input::Keys::SPACE);
	}
	else
	{
		CheckPaused(keyboardState, Input::Keys::P);

		if (!_menu->paused)
		{
			Input(elapsedTime, keyboardState);
			UpdatePacman(elapsedTime);
			UpdateMunchie(elapsedTime);
			CheckViewportCollision();
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

void Pacman::Input(int elapsedTime, Input::KeyboardState*state)
{
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

	switch (_pacman->direction)
	{
	case 4:
		_pacman->position->X += _cPacmanSpeed * elapsedTime; //Moves Pacman across X axis(right)
		_pacman->direction = 4;//rotating pacman right 
		break;
	case 2:
		_pacman->position->X -= _cPacmanSpeed * elapsedTime;//Moves Pacman across X axis(left)
		_pacman->direction = 2;//rotating pacman left 
		break;
	case 3:
		_pacman->position->Y -= _cPacmanSpeed * elapsedTime;//Moves Pacman across Y axis(up)
		_pacman->direction = 3;//rotating pacman up 
		break;
	case 1:
		_pacman->position->Y += _cPacmanSpeed * elapsedTime;//Moves Pacman across Y axis(down)
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
void Pacman::UpdateMunchie(int elapsedTime)
{
	_munchie->currentFrameTime += elapsedTime;
	if (_munchie->currentFrameTime > _cMunchieFrameTime)
	{
		_munchie->frameCount++;
		if (_munchie->frameCount >= 2)
			_munchie->frameCount = 0;
		_munchie->currentFrameTime = 0;
	}
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

void Pacman::Draw(int elapsedTime)
{
	// Allows us to easily create a string
	std::stringstream stream;
	stream << "Pacman X: " << _pacman->position->X << " Y: " << _pacman->position->Y ;

	SpriteBatch::BeginDraw(); // Starts Drawing
	SpriteBatch::Draw(_pacman->texture, _pacman->position, _pacman->sourceRect); // Draws Pacman

	if (_munchie->frameCount == 0)
	{
		// Draws Red Munchie
		SpriteBatch::Draw(_munchie->invertedTexture, _munchie->rect, nullptr, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);
	}
	else
	{
		// Draw Blue Munchie
		SpriteBatch::Draw(_munchie->blueTexture, _munchie->rect, nullptr, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);
		
		_munchie->frameCount++;
		if (_munchie->frameCount >= 60)
			_munchie->frameCount = 0;

	}
	
	// Draws String
	SpriteBatch::DrawString(stream.str().c_str(), _menu->_stringPosition, Color::Green);
	if (!_menu->started)
	{
		std::stringstream menuStream;
		menuStream << "Start screen!";
		SpriteBatch::Draw(_menu->background, _menu->rectangle, nullptr);
		SpriteBatch::DrawString(menuStream.str().c_str(), _menu->stringPosition, Color::Green);

		_munchie->frameCount++;
	}

	if (_menu->paused)
	{
		std::stringstream menuStream;
		menuStream << "PAUSED!";
		SpriteBatch::Draw(_menu->background, _menu->rectangle, nullptr);
		SpriteBatch::DrawString(menuStream.str().c_str(), _menu->stringPosition, Color::Red);

		_munchie->frameCount++;
	}
	SpriteBatch::EndDraw(); // Ends Drawing
}