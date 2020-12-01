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

#define MUNCHIECOUNT 50
#define GHOSTCOUNT 4
#define WALLCOUNT 45

// Reduces the amount of typing by including all classes in S2D namespace
using namespace S2D;

// Structure Definition
struct Player
{
	// Data to represent Pacman
	Vector2* position;
	Rect* sourceRect;
	Texture2D* texture;
	bool dead;

	// Pacman animation
	int direction;
	float speedMulitplier;
	char collision;

	int frame;
	int currentFrameTime;
};

struct Enemy
{

	//Munchie animation
	int frame;
	int currentFrameTime;
	int frameTime;

	// Data to represent Enemy
	Vector2* position;
	int frameCount;
	Rect* sourceRect;
	Texture2D* texture;
	Texture2D* invertedTexture;
};

struct MovingEnemy
{
	Vector2* position;
	Texture2D* texture;
	Rect* sourceRect;
	int direction;
	float speed;
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

	bool randomised;
};

struct WallCoord
{
	Vector2* position;
	int width;
	int height;

};
struct Walls
{
	Texture2D* texture;
	Rect* rectangle;
};
// Declares the Pacman class which inherits from the Game class.
// This allows us to overload the Game class methods to help us
// load content, draw and update our game.
//Class Definition
class Pacman : public Game
{
private:
	Player* _pacman;
	MovingEnemy* _ghosts[GHOSTCOUNT];
	Enemy* _munchies[MUNCHIECOUNT];
	Enemy* _cherry;

	Menu* _menu;

	Walls* _walls;
	WallCoord* _wallCoord[WALLCOUNT];
	int wallArray[45][4];

	//cosntant variable data for game
	const float _cPacmanSpeed;

	//constansts for animation
	const int _cPacmanFrameTime; //keeps the animation speed constant

	//Edibles animation
	const int _cMunchieFrameTime;
	const int _cCherryFrameTime;
	
	//Position for String
	Vector2* _stringPosition;

	//input method
	void Input(int elapsedTime, Input::KeyboardState* state,Input::MouseState* mouseState);

	//check methods
	void CheckPaused(Input::KeyboardState* state, Input::Keys pauseKey);
	void CheckViewportCollision();
	void CheckStart(Input::KeyboardState* state, Input::Keys startKey);
	bool CollisionCheck(int x1, int y1, int width1, int height1, int x2, int y2, int width2, int height2);
	void CheckGhostCollision();
	bool CheckWallCollision(int x1, int y1, int width1, int height1);

	void LoadWallCoord();
	//Update methods
	void UpdatePacman(int elapsedTime);
	void UpdateGhost(MovingEnemy*, int elapsedTime);
	void UpdateMunchie(Enemy*, int elapsedTime);
	void UpdateCherry(int elapsedTime);

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