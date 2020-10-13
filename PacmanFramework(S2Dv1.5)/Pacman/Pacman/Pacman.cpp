#include "Pacman.h"

#include <sstream>

Pacman::Pacman(int argc, char* argv[]) : Game(argc, argv), _cPacmanSpeed(0.1f)
{
	_frameCount = 0;
	_paused = false;
	_pKeyDown = false;

	_start = true;

	//Initialise important Game aspects
	Graphics::Initialise(argc, argv, this, 1024, 768, false, 25, 25, "Pacman", 60);
	Input::Initialise();

	// Start the Game Loop - This calls Update and Draw in game loop
	Graphics::StartGameLoop();
}

Pacman::~Pacman()
{
	delete _pacmanTexture;
	delete _pacmanSourceRect;
	delete _munchieBlueTexture;
	delete _munchieInvertedTexture;
	delete _munchieRect;
}

void Pacman::LoadContent()
{
	// Load Pacman
	_pacmanTexture = new Texture2D();
	_pacmanTexture->Load("Textures/Pacman.tga", false);
	_pacmanPosition = new Vector2(350.0f, 350.0f);
	_pacmanSourceRect = new Rect(0.0f, 0.0f, 32, 32);

	// Load Munchie
	_munchieBlueTexture = new Texture2D();
	_munchieBlueTexture->Load("Textures/Munchie.tga", true);
	_munchieInvertedTexture = new Texture2D();
	_munchieInvertedTexture->Load("Textures/MunchieInverted.tga", true);
	_munchieRect = new Rect(100.0f, 450.0f, 12, 12);

	// Set string position
	_stringPosition = new Vector2(10.0f, 25.0f);

	// Set Menu Paramters
	_menuBackground = new Texture2D();
	_menuBackground->Load("Textures/Trandparency.png", false);
	_menuRectangle = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth(), Graphics::GetViewportHeight());
	_menuStringPosition = new Vector2(Graphics::GetViewportWidth() / 2.0f, Graphics::GetViewportHeight() / 2.0f);

}

void Pacman::Update(int elapsedTime)
{
	// Gets the current state of the keyboard
	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();

	if (keyboardState->IsKeyDown(Input::Keys::SPACE))
	{
		_start = false;
	}

	if (!_start)
	{

		if (keyboardState->IsKeyDown(Input::Keys::P) && !_pKeyDown)
		{
			_pKeyDown = true;
			_paused = !_paused;
		}
		if (keyboardState->IsKeyDown(Input::Keys::P))
			_pKeyDown = false;

		if (!_paused)
		{
			if (keyboardState->IsKeyDown(Input::Keys::D) || keyboardState->IsKeyDown(Input::Keys::RIGHT)) // Checks if D/right key is pressed
				_pacmanPosition->X += _cPacmanSpeed * elapsedTime; //Moves Pacman across X axis

			else if (keyboardState->IsKeyDown(Input::Keys::A) || keyboardState->IsKeyDown(Input::Keys::LEFT))// Checks if A/left key is pressed
				_pacmanPosition->X -= _cPacmanSpeed * elapsedTime;

			else if (keyboardState->IsKeyDown(Input::Keys::W) || keyboardState->IsKeyDown(Input::Keys::UP))//checks if W/up key is pressed
				_pacmanPosition->Y -= _cPacmanSpeed * elapsedTime;

			else if (keyboardState->IsKeyDown(Input::Keys::S) || keyboardState->IsKeyDown(Input::Keys::DOWN))//checks if S/down key is pressed
				_pacmanPosition->Y += _cPacmanSpeed * elapsedTime;
		}
	}

	if (_pacmanPosition->X > Graphics::GetViewportWidth() + (_pacmanSourceRect->Width) / 2) //checks if pacman is slightly off the screen (right)
	{
		//_pacmanPosition-> X = 1024 -_pacmanSourceRect->Width; stops pacman at the edge of screen
		_pacmanPosition->X = 0; // teleports pacman to other edge 'loop'
	}
	else if (_pacmanPosition->X < 0 - (_pacmanSourceRect->Width) / 2)//checks if pacman is slightly off the screen (left)
	{
		_pacmanPosition->X = Graphics::GetViewportWidth(); // teleports pacman to other edge 'loop'
	}

	if (_pacmanPosition->Y > Graphics::GetViewportHeight() + (_pacmanSourceRect->Height) / 2)//checks if pacman is slightly off the screen (down)
	{
		_pacmanPosition->Y = 0; // teleports pacman to other edge 'loop'
	}
	else if (_pacmanPosition->Y < 0 - (_pacmanSourceRect->Height) / 2)//checks if pacman is slightly off the screen (up)
	{
		_pacmanPosition->Y = Graphics::GetViewportHeight(); // teleports pacman to other edge 'loop'
	}
}

void Pacman::Draw(int elapsedTime)
{
	// Allows us to easily create a string
	std::stringstream stream;
	stream << "Pacman X: " << _pacmanPosition->X << " Y: " << _pacmanPosition->Y;

	SpriteBatch::BeginDraw(); // Starts Drawing
	SpriteBatch::Draw(_pacmanTexture, _pacmanPosition, _pacmanSourceRect); // Draws Pacman

	if (_frameCount < 30)
	{
		// Draws Red Munchie
		SpriteBatch::Draw(_munchieInvertedTexture, _munchieRect, nullptr, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);

		_frameCount++;
	}
	else
	{
		// Draw Blue Munchie
		SpriteBatch::Draw(_munchieBlueTexture, _munchieRect, nullptr, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);
		
		_frameCount++;

		if (_frameCount >= 60)
			_frameCount = 0;

	}
	
	// Draws String
	SpriteBatch::DrawString(stream.str().c_str(), _stringPosition, Color::Green);
	if (_start)
	{
		std::stringstream menuStream;
		menuStream << "Start screen!";
		SpriteBatch::Draw(_menuBackground, _menuRectangle, nullptr);
		SpriteBatch::DrawString(menuStream.str().c_str(), _menuStringPosition, Color::Black);

		_frameCount++;
	
	}
	if (_paused)
	{
		std::stringstream menuStream;
		menuStream << "PAUSED!";
		SpriteBatch::Draw(_menuBackground, _menuRectangle, nullptr);
		SpriteBatch::DrawString(menuStream.str().c_str(), _menuStringPosition, Color::Red);

		_frameCount++;
	}
	SpriteBatch::EndDraw(); // Ends Drawing
}