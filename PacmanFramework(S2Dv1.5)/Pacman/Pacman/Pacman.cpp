#include "Pacman.h"

#include <sstream>

Pacman::Pacman(int argc, char* argv[]) : Game(argc, argv), _cPacmanSpeed(0.1f),_cPacmanFrameTime(250), _cMunchieFrameTime(500)
{
	_munchieFrameCount = 0;
	_pacmanCurrentFrameTime = 0;
	_pacmanFrame = 0;
	_munchieCurrentFrameTime = 0;

	_paused = false;
	_pKeyDown = false;
	_start = true;

	//Initialise important Game aspects
	Graphics::Initialise(argc, argv, this, 1024, 768, false, 25, 25, "Pacman", 60);
	Input::Initialise();

	// Start the Game Loop - This calls Update and Draw in game loop
	Graphics::StartGameLoop();
}

Pacman::~Pacman() //clearing memory when the program ends
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
	_menuBackground->Load("Textures/Transparency.png", false);
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

	if (!_start) //cant take inputs in if game hasnt started
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
			if (keyboardState->IsKeyDown(Input::Keys::D) || keyboardState->IsKeyDown(Input::Keys::RIGHT)) { // Checks if D/right key is pressed
				//_pacmanPosition->X += _cPacmanSpeed * elapsedTime; //Moves Pacman across X axis
				_pacmanMoving = 1;//variable used for switch statements to keep pacman once a direction has been picked
			}

			else if (keyboardState->IsKeyDown(Input::Keys::A) || keyboardState->IsKeyDown(Input::Keys::LEFT)) {// Checks if A/left key is pressed
				//_pacmanPosition->X -= _cPacmanSpeed * elapsedTime;
				_pacmanMoving = 2;
			}
			else if (keyboardState->IsKeyDown(Input::Keys::W) || keyboardState->IsKeyDown(Input::Keys::UP)) {//checks if W/up key is pressed
				//_pacmanPosition->Y -= _cPacmanSpeed * elapsedTime;
				_pacmanMoving = 3;
			}

			else if (keyboardState->IsKeyDown(Input::Keys::S) || keyboardState->IsKeyDown(Input::Keys::DOWN)) { //checks if S/down key is pressed
				//_pacmanPosition->Y += _cPacmanSpeed * elapsedTime;
				_pacmanMoving = 4;
			}

			switch (_pacmanMoving)
			{
			case 1:
				_pacmanPosition->X += _cPacmanSpeed * elapsedTime; //Moves Pacman across X axis(right)
				_pacmanDirection = 0; //rotating pacman right 
				break;
			case 2:
				_pacmanPosition->X -= _cPacmanSpeed * elapsedTime;//Moves Pacman across X axis(left)
				_pacmanDirection = 2;//rotating pacman left 
				break;
			case 3:
				_pacmanPosition->Y -= _cPacmanSpeed * elapsedTime;//Moves Pacman across Y axis(up)
				_pacmanDirection = 3;//rotating pacman up 
				break;
			case 4:
				_pacmanPosition->Y += _cPacmanSpeed * elapsedTime;//Moves Pacman across Y axis(down)
				_pacmanDirection = 1;//rotating pacman down 
				break;

			default:
				break;
			}
		}
	}

	//animation related to direction of pacman's movement
	_pacmanCurrentFrameTime += elapsedTime;
	if (_pacmanCurrentFrameTime > _cPacmanFrameTime)
	{
		_pacmanFrame++;
		if (_pacmanFrame >= 2) //2 represents the column in the spritesheet
			_pacmanFrame = 0;//resets
		_pacmanCurrentFrameTime = 0;
	}
	_pacmanSourceRect->X = _pacmanSourceRect->Width * _pacmanFrame; //opens and closes mouth.
	_pacmanSourceRect->Y = _pacmanSourceRect->Height * _pacmanDirection; //rotates pacman.

	_munchieCurrentFrameTime += elapsedTime;
	if (_munchieCurrentFrameTime > _cMunchieFrameTime)
	{
		_munchieFrameCount++;
		if (_munchieFrameCount >= 2)
			_munchieFrameCount = 0;
		_munchieCurrentFrameTime = 0;
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
	stream << "Pacman X: " << _pacmanPosition->X << " Y: " << _pacmanPosition->Y ;

	SpriteBatch::BeginDraw(); // Starts Drawing
	SpriteBatch::Draw(_pacmanTexture, _pacmanPosition, _pacmanSourceRect); // Draws Pacman

	if (_munchieFrameCount == 0)
	{
		// Draws Red Munchie
		SpriteBatch::Draw(_munchieInvertedTexture, _munchieRect, nullptr, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);

		
	}
	else
	{
		// Draw Blue Munchie
		SpriteBatch::Draw(_munchieBlueTexture, _munchieRect, nullptr, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);
		
		_munchieFrameCount++;

		if (_munchieFrameCount >= 60)
			_munchieFrameCount = 0;

	}
	
	// Draws String
	SpriteBatch::DrawString(stream.str().c_str(), _stringPosition, Color::Green);
	if (_start)
	{
		std::stringstream menuStream;
		menuStream << "Start screen!";
		SpriteBatch::Draw(_menuBackground, _menuRectangle, nullptr);
		SpriteBatch::DrawString(menuStream.str().c_str(), _menuStringPosition, Color::Green);

		_munchieFrameCount++;
	}

	if (_paused)
	{
		std::stringstream menuStream;
		menuStream << "PAUSED!";
		SpriteBatch::Draw(_menuBackground, _menuRectangle, nullptr);
		SpriteBatch::DrawString(menuStream.str().c_str(), _menuStringPosition, Color::Red);

		_munchieFrameCount++;
	}
	SpriteBatch::EndDraw(); // Ends Drawing
}