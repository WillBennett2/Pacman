#pragma once

// If Windows and not in Debug, this will run without a console window
// You can use this to output information when debugging using cout or cerr
#ifdef WIN32 
	#ifndef _DEBUG
		#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
	#endif
#endif

// Just need to include main header file
#include "S2D/S2D.h"

// Reduces the amount of typing by including all classes in S2D namespace
using namespace S2D;

// Structure Definition
struct Player
{
	// Data to represent Pacman
	Vector2* position;
	Rect* sourceRect;
	Texture2D* texture;

	// Pacman animation
	int direction;
	int frame;
	int currentFrameTime;

};

struct Munchie
{

	//Munchie animation
	int frame;
	int currentFrameTime;

	// Data to represent Munchie
	int frameCount;
	Rect* rect;
	Texture2D* blueTexture;
	Texture2D* invertedTexture;
};

struct Menu
{

	//data for menu
	Texture2D* background;
	Rect* rectangle;
	Vector2* stringPosition;
	bool paused;
	bool pKeyDown;

	bool started;

	// Position for String
	Vector2* _stringPosition;
};
// Declares the Pacman class which inherits from the Game class.
// This allows us to overload the Game class methods to help us
// load content, draw and update our game.
//Class Definition
class Pacman : public Game
{
private:
	Player* _pacman;

	Munchie* _munchie;

	Menu* _menu;

	//cosntant variable data for game
	const float _cPacmanSpeed;

	//constansts for animation
	const int _cPacmanFrameTime; //keeps the animation speed constant

	//Munchie animation
	const int _cMunchieFrameTime;

	// Position for String
	Vector2* _stringPosition;

	//input method
	void Input(int elapsedTime, Input::KeyboardState* state);

	//check methods
	void CheckPaused(Input::KeyboardState* state, Input::Keys pauseKey);
	void CheckViewportCollision();
	void CheckStart(Input::KeyboardState* state, Input::Keys startKey);

	//Update methods
	void UpdatePacman(int elapsedTime);
	void UpdateMunchie(int elapsedTime);

public:
	/// <summary> Constructs the Pacman class. </summary>
	Pacman(int argc, char* argv[]);

	/// <summary> Destroys any data associated with Pacman class. </summary>
	virtual ~Pacman();

	/// <summary> All content should be loaded in this method. </summary>
	void virtual LoadContent();

	/// <summary> Called every frame - update game logic here. </summary>
	void virtual Update(int elapsedTime);

	/// <summary> Called every frame - draw game here. </summary>
	void virtual Draw(int elapsedTime);
};