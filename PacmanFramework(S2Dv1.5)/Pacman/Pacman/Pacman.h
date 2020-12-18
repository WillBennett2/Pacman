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
#include <iostream>

#define MUNCHIECOUNT 150
#define GHOSTCOUNT 4
#define WALLCOUNT 47

// Reduces the amount of typing by including all classes in S2D namespace
using namespace S2D;

// Structure Definition
struct Player
{
	// Data to enhance game
	int lives;
	int score;
	int munchiesCollected;
	int previousLives;
	// Data to represent Pacman
	Vector2* position;
	Vector2* menuPosition;
	Rect* sourceRect;
	Texture2D* texture;
	bool dead;
	bool moving;


	// Pacman animation
	int direction;
	int previousDirection;

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
	//enhancing data
	int count;

	//data to represnt ghost
	Vector2* position;
	Vector2* menuPosition;
	Rect* sourceRect;
	Texture2D* texture;
	bool moving;
	float speed;

	//ghost animation
	int direction;
	int previousDirection;

	int frame;
	int currentFrameTime;

};

struct Menu
{

	//data for menu
	Texture2D* background;
	Rect* rectangle;
	Vector2* stringPosition;
	bool paused;
	bool pKeyDown;
	bool m1Down;

	bool started;


	bool randomised;
};

struct Walls
{
	Texture2D* texture;
	Rect* rectangle;

	int X;
	int Y;
	int width;
	int height;
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
	int munchieArray[MUNCHIECOUNT][2];

	Enemy* _cherry;
	Enemy* _grapefruit;
	Enemy* _apple;
	Enemy* _grape;

	Menu* _menu;

	//sound realted pointers
	SoundEffect* _munch;
	SoundEffect* _death;
	SoundEffect* _levelUp;
	SoundEffect* _gameOver;
	SoundEffect* _bonk;
	SoundEffect* _nextLevel;

	Walls* _walls[WALLCOUNT];
	int wallArray[WALLCOUNT][4];

	//cosntant variable data for game
	const float _cPacmanSpeed;

	//constansts for animation
	const int _cPacmanFrameTime; //keeps the animation speed constant
	const int _cGhostFrameTime;
	//Edibles animation
	const int _cMunchieFrameTime;
	
	//Position for String
	Vector2* _stringPosition;

	string _currentFruit;

	//input method
	void Input(int elapsedTime, Input::KeyboardState* state,Input::MouseState* mouseState);

	//check methods

	bool CollisionCheck(int x1, int y1, int width1, int height1, int x2, int y2, int width2, int height2);
	void CheckDeath();
	void CheckEnemyViewportCollision(MovingEnemy* ghost);
	void CheckFruitCollision(int elapsedTime);
	bool CheckGhostCollision();
	void CheckPacmanViewportCollision(Player* pacman);
	void CheckPaused(Input::KeyboardState* state, Input::Keys pauseKey);
	void CheckStart(Input::KeyboardState* state, Input::Keys startKey);
	bool CheckWallCollision(int x1, int y1, int width1, int height1);

	void LoadMunchieCoord();
	void LoadWallCoord();

	void ResetGame();

	//Update methods
	void UpdateFruit(Enemy*, int elapsedTime);
	void UpdateGhost(MovingEnemy*, int elapsedTime);
	void UpdateMunchie(Enemy*, int elapsedTime);
	void UpdatePacman(int elapsedTime);

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