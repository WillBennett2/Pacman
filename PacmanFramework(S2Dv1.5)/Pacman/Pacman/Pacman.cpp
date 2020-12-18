#include "Pacman.h"
using namespace std;
#include <sstream>

//constructor
Pacman::Pacman(int argc, char* argv[]) : Game(argc, argv), _cPacmanSpeed(0.1f),_cPacmanFrameTime(150), _cMunchieFrameTime(500), _cGhostFrameTime(150)
{
	//CHARACTERS
	//Initialise member variables
	_pacman = new Player();
	_pacman->dead = false;
	_pacman->moving = true;
	_pacman->previousDirection = -1;
	_pacman->score = 0;
	_pacman->lives = 3;
	_pacman->munchiesCollected = 0;
	_pacman->previousLives = 3;

	//initalising fruits
	_cherry = new Enemy();
	_cherry->frameCount = 0;
	_cherry->currentFrameTime = 0;

	_grapefruit = new Enemy();
	_grapefruit->frameCount = 0;
	_grapefruit->currentFrameTime = 0;

	_apple = new Enemy();
	_apple->frameCount = 0;
	_apple->currentFrameTime = 0;

	_grape = new Enemy();
	_grape->frameCount = 0;
	_grape->currentFrameTime = 0;



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
		_ghosts[i]->previousDirection = -1;
		_ghosts[i]->moving = true;
		_ghosts[i]->speed = 0.1f;
		_ghosts[i]->count = 0;

		_ghosts[i]->frame = 0;
		_ghosts[i]->currentFrameTime = 0;
	}

	//menu
	_menu = new Menu();
	_menu ->paused = false;
	_menu-> pKeyDown = false;
	_menu->m1Down = false;
	_menu->started = false;
	_menu->randomised = false;

	//sounds
	_munch = new SoundEffect();
	_death = new SoundEffect();
	_levelUp = new SoundEffect();
	_gameOver = new SoundEffect();
	_bonk = new SoundEffect();
	_nextLevel = new SoundEffect();

	//walls
	for (int i = 0; i < WALLCOUNT; i++)
		_walls[i] = new Walls();

	//Initialise important Game aspects
	Audio::Initialise();
	if (!_munch->IsLoaded())
	{
		cout << "_munch member sound effect has not loaded" << endl;
	}
	Graphics::Initialise(argc, argv, this, 1024, 768, false, 25, 25, "Pacman", 60);
	Input::Initialise();

	// Start the Game Loop - This calls Update and Draw in game loop
	Graphics::StartGameLoop();


}

Pacman::~Pacman() //clearing memory when the program ends
{
	//cherries
	delete _cherry->texture;
	delete _cherry->sourceRect;
	delete _cherry;
	//grapefruit
	delete _grapefruit->texture;
	delete _grapefruit->sourceRect;
	delete _grapefruit;
	//apple
	delete _apple->texture;
	delete _apple->sourceRect;
	delete _apple;
	//grape
	delete _grape->texture;
	delete _grape->sourceRect;
	delete _grape;

	//ghosts
	delete _ghosts[0]->texture;
	for (int nCount = 0; nCount < GHOSTCOUNT; nCount++)
	{
		delete _ghosts[nCount]->sourceRect;
		delete _ghosts[nCount]->position;
		delete _ghosts[nCount];
	}
	delete[] _ghosts;

	//munchies
	delete _munchies[0]->texture;
	for (int nCount = 0; nCount < MUNCHIECOUNT; nCount++)
	{
		delete _munchies[nCount]->position;
		delete _munchies[nCount]->sourceRect;
		delete _munchies[nCount];
	}
	delete[] _munchies;

	//menu
	delete _menu;

	//pacman
	delete _pacman->texture;
	delete _pacman->sourceRect;
	delete _pacman->position;
	delete _pacman;

	//sound 
	delete _munch;
	delete	_death;
	delete _levelUp;
	delete _gameOver;

	//walls
	delete _walls[0]->texture;
	delete _walls[0]->rectangle;
	for (int nCount = 0; nCount < WALLCOUNT; nCount++)
	{
		delete _walls[nCount];
	}
	delete[] _walls;
}

void Pacman::LoadContent()
{
	// Set string position
	_stringPosition = new Vector2(41.0f, 15.0f);

	//load Cherry
	_cherry->texture = new Texture2D();
	_cherry->texture->Load("Textures/Cherry.png", true);

	_cherry->position = new Vector2(500.0f, 450.0f);
	_cherry->sourceRect = new Rect(0.0f, 0.5f, 25, 30);

	//Load Grapefruit
	_grapefruit->texture = new Texture2D();
	_grapefruit->texture->Load("Textures/Grapefruit.png", true);

	_grapefruit->position = new Vector2(500.0f, 450.0f);
	_grapefruit->sourceRect = new Rect(0.0f, 0.5f, 25, 30);

	//Load Apple
	_apple->texture = new Texture2D();
	_apple->texture->Load("Textures/Apple.png", true);

	_apple->position = new Vector2(500.0f, 450.0f);
	_apple->sourceRect = new Rect(0.0f, 0.5f, 25, 30);

	//LoadGrape
	_grape->texture = new Texture2D();
	_grape->texture->Load("Textures/Grape.png", true);

	_grape->position = new Vector2(500.0f, 450.0f);
	_grape->sourceRect = new Rect(0.0f, 0.5f, 25, 30);


	// Load ghost
	for (int i = 0; i < GHOSTCOUNT; i++)
	{
		Texture2D* GhostTex = new Texture2D();
		switch (i)
		{
		case(0):
			GhostTex->Load("Textures/Red.png", false);
			_ghosts[i]->position = new Vector2((380), (134));
			_ghosts[i]->menuPosition = new Vector2(450.0f, 580.0f);
			break;
		case(1):
			GhostTex->Load("Textures/Cyan.png", false);
			_ghosts[i]->position = new Vector2((475), (360));
			break;
		case(2):
			GhostTex->Load("Textures/Pink.png", false);
			_ghosts[i]->position = new Vector2((520), (360));
			break;
		case(3):
			GhostTex->Load("Textures/Orange.png", false);
			_ghosts[i]->position = new Vector2((560), (360));
			break;
		}
		_ghosts[i]->texture = GhostTex;
		//_ghosts[i]->position = new Vector2((300), (rand() % Graphics::GetViewportHeight()));//rand() % Graphics::GetViewportWidth()
		_ghosts[i]->sourceRect = new Rect(0.0f, 0.5f, 45, 44);
	}

	// Set Menu Paramters
	_menu->background = new Texture2D();
	_menu->background->Load("Textures/Transparency.png", false);
	_menu->rectangle = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth(), Graphics::GetViewportHeight());
	_menu->stringPosition = new Vector2(Graphics::GetViewportWidth() / 2.0f, Graphics::GetViewportHeight() / 2.0f);

	// Load Munchie
	Texture2D* munchieTex = new Texture2D();
	munchieTex->Load("Textures/Munchie.tga", false);
	Texture2D* invertedMunchieTex = new Texture2D();
	invertedMunchieTex->Load("Textures/MunchieInverted.tga", true);
	LoadMunchieCoord();
	for (int i = 0; i < MUNCHIECOUNT; i++)
	{
		_munchies[i]->position = new Vector2(munchieArray[i][0],munchieArray[i][1]);
		_munchies[i]->texture = munchieTex;
		_munchies[i]->invertedTexture = invertedMunchieTex;
		_munchies[i]->sourceRect = new Rect(_munchies[i]->position->X, _munchies[i]->position->Y, 12, 12);
	}

	// Load Pacman
	_pacman->texture = new Texture2D();
	_pacman->texture->Load("Textures/Pacman.png", false);
	_pacman->position = new Vector2(500.0f, 580.0f);
	_pacman->sourceRect = new Rect(0.0f, 0.5f, 33, 32);
	_pacman->menuPosition = new Vector2(520.0f, 580.0f);

	// Load Sounds
	_munch->Load("Sounds/munch.wav");
	_death -> Load("Sounds/pacmanDeath.wav");
	_levelUp -> Load("Sounds/eatFruit.wav");
	_gameOver -> Load("Sounds/gameOver.wav");
	_bonk->Load("Sounds/bonk.wav");
	_nextLevel->Load("Sounds/nextLevel.wav");

	// Load Walls
	//Set walls image 
	_walls[0]->texture = new Texture2D();
	_walls[0]->texture->Load("Textures/Map.png", true);
	_walls[0]->rectangle = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth(), Graphics::GetViewportHeight());

	//Giving the walls their positions
	LoadWallCoord();
	for (int i = 0; i < WALLCOUNT; i++)
	{
		_walls[i]->X = wallArray[i][0];
		_walls[i]->Y = wallArray[i][1];
		_walls[i]->width = wallArray[i][2];
		_walls[i]->height = wallArray[i][3];
	}



}

void Pacman::LoadWallCoord()
{
	//defining wall coords, widths, and heights of all the wall to create a box collider
	//ARRAY STRUCTURE: [0][0] = x, [0][1] = y, [0][2] = width, [0][3] = height
	int windowWidth = Graphics::GetViewportWidth();

	// outer rim
	wallArray[0][0] = 0, wallArray[0][1] = 0, wallArray[0][2] = windowWidth, wallArray[0][3] = 21;
	wallArray[1][0] = 0, wallArray[1][1] = 21, wallArray[1][2] = 29, wallArray[1][3] = 251;
	wallArray[2][0] = windowWidth - 32, wallArray[2][1] = 21, wallArray[2][2] = 29, wallArray[2][3] = 251;
	wallArray[3][0] = 0, wallArray[3][1] = 272, wallArray[3][2] = 191, wallArray[3][3] = 78;

	wallArray[4][0] = 829, wallArray[4][1] = 272, wallArray[4][2] = 191, wallArray[4][3] = 78;
	wallArray[5][0] = 487, wallArray[5][1] = 21, wallArray[5][2] = 49, wallArray[5][3] = 116;

	wallArray[6][0] = 0, wallArray[6][1] = 408, wallArray[6][2] = 191, wallArray[6][3] = 78;
	wallArray[7][0] = 829, wallArray[7][1] = 408, wallArray[7][2] = 191, wallArray[7][3] = 78;

	wallArray[8][0] = 0, wallArray[8][1] = 486, wallArray[8][2] = 29, wallArray[8][3] = 258;
	wallArray[9][0] = windowWidth - 32, wallArray[9][1] = 486, wallArray[9][2] = 29, wallArray[9][3] = 258;

	wallArray[10][0] = 21, wallArray[10][1] = 615, wallArray[10][2] = 75, wallArray[10][3] = 39;
	wallArray[11][0] = 918, wallArray[11][1] = 615, wallArray[11][2] = 80, wallArray[11][3] = 39;

	wallArray[12][0] = 0, wallArray[12][1] = 743, wallArray[12][2] = windowWidth, wallArray[12][3] = 21;

	// top inner half
	//medium BOXES
	wallArray[13][0] = 81, wallArray[13][1] = 56, wallArray[13][2] = 111, wallArray[13][3] = 81;
	wallArray[14][0] = 830, wallArray[14][1] = 56, wallArray[14][2] = 111, wallArray[14][3] = 81;
	//large box
	wallArray[15][0] = 252, wallArray[15][1] = 56, wallArray[15][2] = 184, wallArray[15][3] = 81;
	wallArray[16][0] = 587, wallArray[16][1] = 56, wallArray[16][2] = 184, wallArray[16][3] = 81;
	//left T boxes
	wallArray[17][0] = 263, wallArray[17][1] = 190, wallArray[17][2] = 51, wallArray[17][3] = 161;
	wallArray[18][0] = 303, wallArray[18][1] = 258, wallArray[18][2] = 133, wallArray[18][3] = 30;
	//right T boxes
	wallArray[19][0] = 709, wallArray[19][1] = 190, wallArray[19][2] = 51, wallArray[19][3] = 161;
	wallArray[20][0] = 587, wallArray[20][1] = 258, wallArray[20][2] = 133, wallArray[20][3] = 30;
	//Middle T boxes
	wallArray[21][0] = 374, wallArray[21][1] = 182, wallArray[21][2] = 274, wallArray[21][3] = 39;
	wallArray[22][0] = 487, wallArray[22][1] = 221, wallArray[22][2] = 49, wallArray[22][3] = 68;
	//small box
	wallArray[23][0] = 80, wallArray[23][1] = 188, wallArray[23][2] = 121, wallArray[23][3] = 47;
	wallArray[24][0] = 820, wallArray[24][1] = 188, wallArray[24][2] = 121, wallArray[24][3] = 47;

	
	//cage for ghosts
	wallArray[25][0] = 374, wallArray[25][1] = 329, wallArray[25][2] = 110, wallArray[25][3] = 20;
	wallArray[26][0] = 547, wallArray[26][1] = 329, wallArray[26][2] = 110, wallArray[26][3] = 20;

	wallArray[27][0] = 374, wallArray[27][1] = 334, wallArray[27][2] = 32, wallArray[27][3] = 99;
	wallArray[28][0] = 618, wallArray[28][1] = 334, wallArray[28][2] = 32, wallArray[28][3] = 99;

	wallArray[29][0] = 385, wallArray[29][1] = 416, wallArray[29][2] = 253, wallArray[29][3] = 24;

	//bottom inner half
	//small box
	wallArray[30][0] = 0, wallArray[30][1] = 0, wallArray[30][2] = 0, wallArray[30][3] = 0;
	//higher middle T box
	wallArray[31][0] = 375, wallArray[31][1] = 479, wallArray[31][2] = 272, wallArray[31][3] = 33;
	wallArray[32][0] = 486, wallArray[32][1] = 509, wallArray[32][2] = 49, wallArray[32][3] = 60;
	//Left + right R boxes
	wallArray[33][0] = 80, wallArray[33][1] = 533, wallArray[33][2] = 120, wallArray[33][3] = 36;
	wallArray[34][0] = 820, wallArray[34][1] = 533, wallArray[34][2] = 120, wallArray[34][3] = 36;
	wallArray[35][0] = 150, wallArray[35][1] = 570, wallArray[35][2] = 51, wallArray[35][3] = 61;
	wallArray[36][0] = 820, wallArray[36][1] = 570, wallArray[36][2] = 51, wallArray[36][3] = 61;
	//long boxes
	wallArray[37][0] = 255, wallArray[37][1] = 561, wallArray[37][2] = 158, wallArray[37][3] = 23;
	wallArray[38][0] = 609, wallArray[38][1] = 561, wallArray[38][2] = 158, wallArray[38][3] = 23;
	//upside down T boxes
	wallArray[39][0] = 253, wallArray[39][1] = 623, wallArray[39][2] = 61, wallArray[39][3] = 65;
	wallArray[40][0] = 709, wallArray[40][1] = 623, wallArray[40][2] = 61, wallArray[40][3] = 65;
	wallArray[41][0] = 115, wallArray[41][1] = 688, wallArray[41][2] = 300, wallArray[41][3] = 22;
	wallArray[42][0] = 608, wallArray[42][1] = 688, wallArray[42][2] = 300, wallArray[42][3] = 22;
	//middle T boxes
	wallArray[43][0] = 375, wallArray[43][1] = 624, wallArray[43][2] = 273, wallArray[43][3] = 29;
	wallArray[44][0] = 487, wallArray[44][1] = 653, wallArray[44][2] = 50, wallArray[44][3] = 53;

	//small box
	wallArray[45][0] = 251, wallArray[45][1] = 410, wallArray[45][2] = 60, wallArray[45][3] = 76;
	wallArray[46][0] = 708, wallArray[46][1] = 410, wallArray[46][2] = 60, wallArray[46][3] = 76;

}
void Pacman::LoadMunchieCoord()
{
	munchieArray[0][0] = 458, munchieArray[0][1] = 132;
	munchieArray[1][0] = 461, munchieArray[1][1] = 100;
	munchieArray[2][0] = 464, munchieArray[2][1] = 65;
	munchieArray[3][0] = 464, munchieArray[3][1] = 33;
	munchieArray[4][0] = 430, munchieArray[4][1] = 33;
	munchieArray[5][0] = 400, munchieArray[5][1] = 33;
	munchieArray[6][0] = 358, munchieArray[6][1] = 33;
	munchieArray[7][0] = 303, munchieArray[7][1] = 34;
	munchieArray[8][0] = 254, munchieArray[8][1] = 36;
	munchieArray[9][0] = 214, munchieArray[9][1] = 31;
	munchieArray[10][0] = 170, munchieArray[10][1] = 32;
	munchieArray[11][0] = 126, munchieArray[11][1] = 32;
	munchieArray[12][0] = 72, munchieArray[12][1] = 32;
	munchieArray[13][0] = 55, munchieArray[13][1] = 65;
	munchieArray[14][0] = 52, munchieArray[14][1] = 103;
	munchieArray[15][0] = 54, munchieArray[15][1] = 138;
	munchieArray[16][0] = 52, munchieArray[16][1] = 178;
	munchieArray[17][0] = 113, munchieArray[17][1] = 159;
	munchieArray[18][0] = 177, munchieArray[18][1] = 159;
	munchieArray[19][0] = 222, munchieArray[19][1] = 131;
	munchieArray[20][0] = 223, munchieArray[20][1] = 92;
	munchieArray[21][0] = 57, munchieArray[21][1] = 242;
	munchieArray[22][0] = 104, munchieArray[22][1] = 247;
	munchieArray[23][0] = 180, munchieArray[23][1] = 248;
	munchieArray[24][0] = 288, munchieArray[24][1] = 160;
	munchieArray[25][0] = 331, munchieArray[25][1] = 159;
	munchieArray[26][0] = 402, munchieArray[26][1] = 160;
	munchieArray[27][0] = 346, munchieArray[27][1] = 201;
	munchieArray[28][0] = 344, munchieArray[28][1] = 238;
	munchieArray[29][0] = 400, munchieArray[29][1] = 238;
	munchieArray[30][0] = 461, munchieArray[30][1] = 239;
	munchieArray[31][0] = 461, munchieArray[31][1] = 275;
	munchieArray[32][0] = 226, munchieArray[32][1] = 285;
	munchieArray[33][0] = 224, munchieArray[33][1] = 326;
	munchieArray[34][0] = 226, munchieArray[34][1] = 375;
	munchieArray[35][0] = 286, munchieArray[35][1] = 377;
	munchieArray[36][0] = 166, munchieArray[36][1] = 376;
	munchieArray[37][0] = 114, munchieArray[37][1] = 374;
	munchieArray[38][0] = 61, munchieArray[38][1] = 374;
	munchieArray[39][0] = 14, munchieArray[39][1] = 373;
	munchieArray[40][0] = 217, munchieArray[40][1] = 410;
	munchieArray[41][0] = 216, munchieArray[41][1] = 456;
	munchieArray[42][0] = 213, munchieArray[42][1] = 504;
	munchieArray[43][0] = 164, munchieArray[43][1] = 505;
	munchieArray[44][0] = 103, munchieArray[44][1] = 503;
	munchieArray[45][0] = 61, munchieArray[45][1] = 505;
	munchieArray[46][0] = 55, munchieArray[46][1] = 543;
	munchieArray[47][0] = 58, munchieArray[47][1] = 588;
	munchieArray[48][0] = 123, munchieArray[48][1] = 590;
	munchieArray[49][0] = 121, munchieArray[49][1] = 639;
	munchieArray[50][0] = 93, munchieArray[50][1] = 671;
	munchieArray[51][0] = 54, munchieArray[51][1] = 675;
	munchieArray[52][0] = 228, munchieArray[52][1] = 657;
	munchieArray[53][0] = 226, munchieArray[53][1] = 608;
	munchieArray[54][0] = 228, munchieArray[54][1] = 562;
	munchieArray[55][0] = 67, munchieArray[55][1] = 720;
	munchieArray[56][0] = 128, munchieArray[56][1] = 721;
	munchieArray[57][0] = 198, munchieArray[57][1] = 722;
	munchieArray[58][0] = 259, munchieArray[58][1] = 723;
	munchieArray[59][0] = 327, munchieArray[59][1] = 723;
	munchieArray[60][0] = 383, munchieArray[60][1] = 722;
	munchieArray[61][0] = 449, munchieArray[61][1] = 724;
	munchieArray[62][0] = 448, munchieArray[62][1] = 670;
	munchieArray[63][0] = 394, munchieArray[63][1] = 667;
	munchieArray[64][0] = 340, munchieArray[64][1] = 666;
	munchieArray[65][0] = 338, munchieArray[65][1] = 604;
	munchieArray[66][0] = 406, munchieArray[66][1] = 606;
	munchieArray[67][0] = 453, munchieArray[67][1] = 605;
	munchieArray[68][0] = 511, munchieArray[68][1] = 602;
	munchieArray[69][0] = 452, munchieArray[69][1] = 571;
	munchieArray[70][0] = 451, munchieArray[70][1] = 533;
	munchieArray[71][0] = 380, munchieArray[71][1] = 533;
	munchieArray[72][0] = 332, munchieArray[72][1] = 536;
	munchieArray[73][0] = 337, munchieArray[73][1] = 490;
	munchieArray[74][0] = 507, munchieArray[74][1] = 725;
	munchieArray[75][0] = 577, munchieArray[75][1] = 725;
	munchieArray[76][0] = 572, munchieArray[76][1] = 664;
	munchieArray[77][0] = 633, munchieArray[77][1] = 668;
	munchieArray[78][0] = 684, munchieArray[78][1] = 669;
	munchieArray[79][0] = 684, munchieArray[79][1] = 631;
	munchieArray[80][0] = 681, munchieArray[80][1] = 594;
	munchieArray[81][0] = 620, munchieArray[81][1] = 600;
	munchieArray[82][0] = 567, munchieArray[82][1] = 599;
	munchieArray[83][0] = 567, munchieArray[83][1] = 568;
	munchieArray[84][0] = 567, munchieArray[84][1] = 532;
	munchieArray[85][0] = 624, munchieArray[85][1] = 536;
	munchieArray[86][0] = 717, munchieArray[86][1] = 533;
	munchieArray[87][0] = 768, munchieArray[87][1] = 534;
	munchieArray[88][0] = 680, munchieArray[88][1] = 498;
	munchieArray[89][0] = 628, munchieArray[89][1] = 726;
	munchieArray[90][0] = 668, munchieArray[90][1] = 726;
	munchieArray[91][0] = 713, munchieArray[91][1] = 725;
	munchieArray[92][0] = 756, munchieArray[92][1] = 727;
	munchieArray[93][0] = 800, munchieArray[93][1] = 727;
	munchieArray[94][0] = 843, munchieArray[94][1] = 728;
	munchieArray[95][0] = 893, munchieArray[95][1] = 730;
	munchieArray[96][0] = 957, munchieArray[96][1] = 730;
	munchieArray[97][0] = 960, munchieArray[97][1] = 698;
	munchieArray[98][0] = 960, munchieArray[98][1] = 665;
	munchieArray[99][0] = 916, munchieArray[99][1] = 664;
	munchieArray[100][0] = 857, munchieArray[100][1] = 664;
	munchieArray[101][0] = 798, munchieArray[101][1] = 661;
	munchieArray[102][0] = 797, munchieArray[102][1] = 624;
	munchieArray[103][0] = 901, munchieArray[103][1] = 633;
	munchieArray[104][0] = 900, munchieArray[104][1] = 593;
	munchieArray[105][0] = 958, munchieArray[105][1] = 594;
	munchieArray[106][0] = 960, munchieArray[106][1] = 544;
	munchieArray[107][0] = 958, munchieArray[107][1] = 498;
	munchieArray[108][0] = 909, munchieArray[108][1] = 498;
	munchieArray[109][0] = 848, munchieArray[109][1] = 496;
	munchieArray[110][0] = 793, munchieArray[110][1] = 496;
	munchieArray[111][0] = 795, munchieArray[111][1] = 448;
	munchieArray[112][0] = 795, munchieArray[112][1] = 387;
	munchieArray[113][0] = 748, munchieArray[113][1] = 383;
	munchieArray[114][0] = 857, munchieArray[114][1] = 383;
	munchieArray[115][0] = 904, munchieArray[115][1] = 383;
	munchieArray[116][0] = 950, munchieArray[116][1] = 384;
	munchieArray[117][0] = 1010, munchieArray[117][1] = 385;
	munchieArray[118][0] = 790, munchieArray[118][1] = 329;
	munchieArray[119][0] = 794, munchieArray[119][1] = 277;
	munchieArray[120][0] = 794, munchieArray[120][1] = 234;
	munchieArray[121][0] = 842, munchieArray[121][1] = 251;
	munchieArray[122][0] = 902, munchieArray[122][1] = 251;
	munchieArray[123][0] = 960, munchieArray[123][1] = 252;
	munchieArray[124][0] = 966, munchieArray[124][1] = 209;
	munchieArray[125][0] = 963, munchieArray[125][1] = 155;
	munchieArray[126][0] = 916, munchieArray[126][1] = 158;
	munchieArray[127][0] = 877, munchieArray[127][1] = 158;
	munchieArray[128][0] = 843, munchieArray[128][1] = 158;
	munchieArray[129][0] = 801, munchieArray[129][1] = 160;
	munchieArray[130][0] = 801, munchieArray[130][1] = 110;
	munchieArray[131][0] = 799, munchieArray[131][1] = 70;
	munchieArray[132][0] = 803, munchieArray[132][1] = 34;
	munchieArray[133][0] = 850, munchieArray[133][1] = 34;
	munchieArray[134][0] = 900, munchieArray[134][1] = 34;
	munchieArray[135][0] = 967, munchieArray[135][1] = 35;
	munchieArray[136][0] = 735, munchieArray[136][1] = 34;
	munchieArray[137][0] = 688, munchieArray[137][1] = 32;
	munchieArray[138][0] = 654, munchieArray[138][1] = 33;
	munchieArray[139][0] = 574, munchieArray[139][1] = 33;
	munchieArray[140][0] = 566, munchieArray[140][1] = 67;
	munchieArray[141][0] = 564, munchieArray[141][1] = 154;
	munchieArray[142][0] = 620, munchieArray[142][1] = 158;
	munchieArray[143][0] = 670, munchieArray[143][1] = 156;
	munchieArray[144][0] = 735, munchieArray[144][1] = 157;
	munchieArray[145][0] = 683, munchieArray[145][1] = 199;
	munchieArray[146][0] = 682, munchieArray[146][1] = 232;
	munchieArray[147][0] = 623, munchieArray[147][1] = 239;
	munchieArray[148][0] = 562, munchieArray[148][1] = 233;
	munchieArray[149][0] = 556, munchieArray[149][1] = 271;
}

void Pacman::Update(int elapsedTime)
{
	// Gets the current state of the keyboard
	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();
	// Gets the current state of the mouse
	Input::MouseState* mouseState = Input::Mouse::GetState();

	if (!_menu->started) //cant take inputs in if game hasnt started
	{
		//CheckStart(keyboardState, Input::Keys::SPACE);
		Input(elapsedTime, keyboardState, mouseState);
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
				{
					_pacman->moving = false;
					Audio::Play(_bonk);
				}
			}


			else if (!CheckWallCollision(_pacman->position->X, _pacman->position->Y, _pacman->sourceRect->Width, _pacman->sourceRect->Height))
				_pacman->moving = true;

			UpdatePacman(elapsedTime);

			for (int i = 0; i < GHOSTCOUNT; i++) 
			{
				if (_ghosts[i]->moving == true)
				{
					_ghosts[i]->previousDirection = _ghosts[i]->direction;
					if (CheckWallCollision(_ghosts[i]->position->X, (_ghosts[i]->position->Y)+10, _ghosts[i]->sourceRect->Width, (_ghosts[i]->sourceRect->Height) - 10))
					{
						_ghosts[i]->moving = false;
						//UpdateGhost(_ghosts[i], elapsedTime);
					}
				}
				else if (!CheckWallCollision(_ghosts[i]->position->X, _ghosts[i]->position->Y, _ghosts[i]->sourceRect->Width, _ghosts[i]->sourceRect->Height))
				{
					_ghosts[i]->moving = true;
					//UpdateGhost(_ghosts[i], elapsedTime);
				}
				
				UpdateGhost(_ghosts[i], elapsedTime);
				CheckEnemyViewportCollision(_ghosts[i]);
				if(CheckGhostCollision())
				{
					_pacman->lives--;
					if (_pacman->lives <= 0)
						_pacman->lives = 0;
					Audio::Play(_death);
					break;
				}

			}

			for (int i = 0; i < MUNCHIECOUNT; i++)
			{
				if (CollisionCheck(_pacman->position->X, _pacman->position->Y, _pacman->sourceRect->Width, _pacman->sourceRect->Height,
					_munchies[i]->position->X, _munchies[i]->position->Y, _munchies[i]->sourceRect->Width, _munchies[i]->sourceRect->Height))
				{
					Audio::Play(_munch);
					_pacman->score += 10;
					//_pacman->collision = 'Y';
					_munchies[i]->position->X = -100;
					_munchies[i]->position->Y = -100;
					_pacman->munchiesCollected++;
					if (_pacman->score % 2400 == 0)
					{
						_pacman->lives++;
						Audio::Play(_levelUp);
					}
				}

				UpdateMunchie(_munchies[i], elapsedTime);

			}

			CheckFruitCollision(elapsedTime);



			if (_pacman->munchiesCollected == MUNCHIECOUNT)
			{
					Audio::Play(_nextLevel);
					ResetGame();
			}

			CheckDeath();
			CheckPacmanViewportCollision(_pacman);

			if (_pacman->dead == true)
			{
				ResetGame();
			}
		}
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
void Pacman::CheckDeath()
{
	if (_pacman->lives < _pacman->previousLives)
	{
		_pacman->position->X = 500.0f;
		_pacman->position->Y = 580.0f;
	}
	if (_pacman->lives <= 0)
	{
		_pacman->dead = true;
		Audio::Play(_gameOver);
	}
	_pacman->previousLives = _pacman->lives;
	return;
}
void Pacman::CheckEnemyViewportCollision(MovingEnemy* ghost)
{
	if (ghost->position->X > Graphics::GetViewportWidth() + (ghost->sourceRect->Width) / 2) //checks if pacman is slightly off the screen (right)
	{
		//_pacmanPosition-> X = 1024 -_pacmanSourceRect->Width; stops pacman at the edge of screen
		ghost->position->X = 0; // teleports pacman to other edge 'loop'
	}
	else if (ghost->position->X < 0 - (ghost->sourceRect->Width) / 2)//checks if pacman is slightly off the screen (left)
	{
		ghost->position->X = Graphics::GetViewportWidth(); // teleports pacman to other edge 'loop'
	}

	if (ghost->position->Y > Graphics::GetViewportHeight() + (ghost->sourceRect->Height) / 2)//checks if pacman is slightly off the screen (down)
	{
		ghost->position->Y = 0; // teleports pacman to other edge 'loop'
	}
	else if (ghost->position->Y < 0 - (ghost->sourceRect->Height) / 2)//checks if pacman is slightly off the screen (up)
	{
		ghost->position->Y = Graphics::GetViewportHeight(); // teleports pacman to other edge 'loop'
	}
}
void Pacman::CheckFruitCollision(int elapsedTime)
{
	//works out current fruit based on how many lives pacman has (points change based on how well you are doing)
	switch (_pacman->lives)
	{
	case(3):
		if (CollisionCheck(_pacman->position->X, _pacman->position->Y, _pacman->sourceRect->Width, _pacman->sourceRect->Height,
			_cherry->position->X, _cherry->position->Y, _cherry->sourceRect->Width, _cherry->sourceRect->Height))
		{
			_pacman->score += 300;
			_cherry->position->X = -100;
			_cherry->position->Y = -100;

			if (_pacman->score % 2400 == 0)
			{
				_pacman->lives++;
				Audio::Play(_levelUp);
			}
			Audio::Play(_munch);
		}
		UpdateFruit(_cherry, elapsedTime);
		break;

	case(2):
		if (CollisionCheck(_pacman->position->X, _pacman->position->Y, _pacman->sourceRect->Width, _pacman->sourceRect->Height,
			_grapefruit->position->X, _grapefruit->position->Y, _grapefruit->sourceRect->Width, _grapefruit->sourceRect->Height))
		{
			_pacman->score += 350;
			_grapefruit->position->X = -100;
			_grapefruit->position->Y = -100;

			if (_pacman->score % 2400 == 0)
			{
				_pacman->lives++;
				Audio::Play(_levelUp);
			}
				
			Audio::Play(_munch);
		}
		UpdateFruit(_grapefruit, elapsedTime);
		break;

	case(1):
		if (CollisionCheck(_pacman->position->X, _pacman->position->Y, _pacman->sourceRect->Width, _pacman->sourceRect->Height,
			_apple->position->X, _apple->position->Y, _apple->sourceRect->Width, _apple->sourceRect->Height))
		{
			_pacman->score += 400;
			_apple->position->X = -100;
			_apple->position->Y = -100;

			if (_pacman->score % 2400 == 0)
			{
				_pacman->lives++;
				Audio::Play(_levelUp);
			}
			Audio::Play(_munch);
		}
		UpdateFruit(_apple, elapsedTime);
		break;

	default:
		if (CollisionCheck(_pacman->position->X, _pacman->position->Y, _pacman->sourceRect->Width, _pacman->sourceRect->Height,
			_grape->position->X, _grape->position->Y, _grape->sourceRect->Width, _grape->sourceRect->Height))
		{
			_pacman->score += 200;
			_grape->position->X = -100;
			_grape->position->Y = -100;

			if (_pacman->score % 2400 == 0)
			{
				_pacman->lives++;
				Audio::Play(_levelUp);
			}
			Audio::Play(_munch);
		}
		UpdateFruit(_grape, elapsedTime);
		break;
	}
}
bool Pacman::CheckGhostCollision()
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
		bottom2 = _ghosts[i]->position->Y + _ghosts[i]->sourceRect->Height ;
		left2 = _ghosts[i]->position->X;
		right2 = _ghosts[i]->position->X + _ghosts[i]->sourceRect->Width;

		if ((bottom1 > top2) && (top1 < bottom2) && (right1 > left2) && (left1 < right2))
		{
			i = GHOSTCOUNT;
			return true;
		}
	}
	return false;
}
void Pacman::CheckPacmanViewportCollision(Player* pacman)
{
	if (pacman->position->X > Graphics::GetViewportWidth() + (pacman->sourceRect->Width) / 2) //checks if pacman is slightly off the screen (right)
	{
		//_pacmanPosition-> X = 1024 -_pacmanSourceRect->Width; stops pacman at the edge of screen
		pacman->position->X = 0; // teleports pacman to other edge 'loop'
	}
	else if (pacman->position->X < 0 - (pacman->sourceRect->Width) / 2)//checks if pacman is slightly off the screen (left)
	{
		pacman->position->X = Graphics::GetViewportWidth(); // teleports pacman to other edge 'loop'
	}

	if (pacman->position->Y > Graphics::GetViewportHeight() + (pacman->sourceRect->Height) / 2)//checks if pacman is slightly off the screen (down)
	{
		pacman->position->Y = 0; // teleports pacman to other edge 'loop'
	}
	else if (pacman->position->Y < 0 - (pacman->sourceRect->Height) / 2)//checks if pacman is slightly off the screen (up)
	{
		pacman->position->Y = Graphics::GetViewportHeight(); // teleports pacman to other edge 'loop'
	}

}
void Pacman::CheckPaused(Input::KeyboardState* state, Input::Keys pauseKey)
{

	if (state->IsKeyDown(Input::Keys::P) && !_menu->pKeyDown)
	{
		_menu->pKeyDown = true;
		_menu->paused = !_menu->paused;
	}
	if (state->IsKeyUp(Input::Keys::P))
		_menu->pKeyDown = false;
}
void Pacman::CheckStart(Input::KeyboardState* state, Input::Keys startKey)
{
	if (state->IsKeyDown(Input::Keys::SPACE))
	{
		_menu->started = true;
	}
}
bool Pacman::CheckWallCollision(int x1, int y1, int width1, int height1)
{
	bool collision = false;
	int top1 = y1;
	int top2 = 0;
	int bottom1 = y1 + height1;
	int bottom2 = 0;
	int left1 = x1;
	int left2 = 0;
	int right1 = x1 + width1;
	int right2 = 0;

	for (int i = 0; i < WALLCOUNT; i++)
	{
		top2 = _walls[i]->Y;
		bottom2 = _walls[i]->Y + _walls[i]->height;
		left2 = _walls[i]->X;
		right2 = _walls[i]->X + _walls[i]->width;

		if ((bottom1 > top2) && (top1 < bottom2) && (right1 > left2) && (left1 < right2))
		{
			i = WALLCOUNT;
			return true;
		}
	}
	return collision;
}


void Pacman::Input(int elapsedTime, Input::KeyboardState* state, Input::MouseState* mouseState)
{
	POINT curser;
	GetCursorPos(&curser);

	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();

	if (!_menu->started)
	{
		int top1 = curser.y;
		int top2 = 0;
		int bottom1 = curser.y + 10.0f;
		int bottom2 = 0;
		int left1 = curser.x;
		int left2 = 0;
		int right1 = curser.x + 10.0f;
		int right2 = 0;

		top2 = (Graphics::GetViewportHeight() / 2);
		bottom2 = (Graphics::GetViewportHeight() / 2 + 100);
		left2 = ((Graphics::GetViewportWidth() / 2 )- 120);
		right2 = ((Graphics::GetViewportWidth() / 2) - 120 + 100);
		if ((bottom1 > top2) && (top1 < bottom2) && (right1 > left2) && (left1 < right2))
		{
			if ((mouseState->LeftButton == Input::ButtonState::PRESSED) && !_menu->m1Down)
				_menu->started = true;
		}

		bottom2 =(( Graphics::GetViewportHeight() / 2) + 100);
		left2 =(( Graphics::GetViewportWidth() / 2 )+ 20);
		right2 = ((Graphics::GetViewportWidth() / 2 ) + 120);
		if ((bottom1 > top2) && (top1 < bottom2) && (right1 > left2) && (left1 < right2))
		{
			if ((mouseState->LeftButton == Input::ButtonState::PRESSED) && !_menu->m1Down)
			{
				//this is possibly a bad way to exit the program(not sure if deconstructor is called) but i dont know of a good way
				exit(0);
			}
		}
	}

	if (mouseState->LeftButton == Input::ButtonState::RELEASED)
		_menu->m1Down = false;

	if (keyboardState->IsKeyDown(Input::Keys::T))
		cout <<  endl;

	float pacmanSpeed = _cPacmanSpeed * elapsedTime * _pacman->speedMulitplier;
	if (state->IsKeyDown(Input::Keys::LEFTSHIFT))
		pacmanSpeed = 2.0f; //apply multiplier

	else
		pacmanSpeed = 1.0f; //reset multiplier


	if (state->IsKeyDown(Input::Keys::D) || state->IsKeyDown(Input::Keys::RIGHT))// Checks if D/right key is pressed
		_pacman->direction = 4;  //variable used for switch statements to keep pacman once a direction has been picked

	else if (state->IsKeyDown(Input::Keys::A) || state->IsKeyDown(Input::Keys::LEFT))// Checks if A/left key is pressed
		_pacman->direction = 2;

	else if (state->IsKeyDown(Input::Keys::W) || state->IsKeyDown(Input::Keys::UP))//checks if W/up key is pressed
		_pacman->direction = 3;

	else if (state->IsKeyDown(Input::Keys::S) || state->IsKeyDown(Input::Keys::DOWN)) //checks if S/down key is pressed
		_pacman->direction = 1;


	if (_pacman->moving == true || _pacman->previousDirection != _pacman->direction)
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

void Pacman::ResetGame()
{
	if (_pacman->dead == true)
	{
		_menu->started = false;
		_pacman->dead = false;
		_pacman->moving = true;
		_pacman->previousDirection = -1;
		_pacman->score = 0;
		_pacman->lives = 3;
		_pacman->munchiesCollected = 0;
		_pacman->previousLives = 3;
	}
	for (int i = 0; i < MUNCHIECOUNT; i++)
	{
		_munchies[i]->position->X = munchieArray[i][0];
		_munchies[i]->position->Y = munchieArray[i][1];
	}

	_pacman->position -> X = 500.0f;
	_pacman->position -> Y = 580.0f;
	_pacman->munchiesCollected = 0;
	_pacman->lives = 3;

	_cherry->position->X = 500.0f;
	_cherry->position->Y = 450.0f;


	_ghosts[0]->position->X = 430;
	_ghosts[0]->position->Y = 360;

	_ghosts[1]->position->X = 475;
	_ghosts[1]->position->Y = 360;


	_ghosts[2]->position->X = 520;
	_ghosts[2]->position->Y = 360;


	_ghosts[3]->position->X = 560;
	_ghosts[3]->position->Y = 360;

}

void Pacman::UpdateFruit(Enemy* fruit, int elapsedTime)
{
	fruit->currentFrameTime += elapsedTime;
	if (fruit->currentFrameTime > _cMunchieFrameTime)
	{
		fruit->frame++;
		if (fruit->frame >= 2)
			fruit->frame = 0;
		fruit->currentFrameTime = 0;
	}
	fruit->sourceRect->X = fruit->sourceRect->Width * fruit->frame;
	fruit->sourceRect->Y = fruit->sourceRect->Height;
}
void Pacman::UpdateGhost(MovingEnemy*ghost, int elapsedTime)
{
	//ghost->count = 0;
	if (ghost->moving == false)
	{
		switch (ghost->count)
		{
		case(0):
			ghost->direction = 3;
			break;
		case(1):
			ghost->direction = 0;
		default:
			break;
		}
		ghost->count++;
		if (ghost->count > 1)
			ghost->count = 0;
		ghost->moving = true;
	}

	if (ghost->moving == true || ghost->previousDirection != ghost->direction)
		if (ghost->direction == 0)//moves right
		{
			ghost->position->X += ghost->speed * elapsedTime;
			//ghost->sourceRect = new Rect(0.0f, 0.7f, 20, 23);
		}
		else if (ghost->direction == 3)//moves left
		{
			ghost->position->X -= ghost->speed * elapsedTime;
			//ghost->sourceRect = new Rect(0.0f, 66.7f, 20, 23);
		}
		//else if (ghost->direction == 3)//moves down
		//{
		//	ghost->position->X -= ghost->speed * elapsedTime;
		//	//ghost->sourceRect = new Rect(0.0f, 22.7f, 20, 23);
		//}
		//else if (ghost->direction == 3)//moves up
		//{
		//	ghost->position->X -= ghost->speed * elapsedTime;
		//	//ghost->sourceRect = new Rect(0.0f, 44.7f, 20, 23);
		//}

	//animation related to direction of ghosts's movement
	ghost->currentFrameTime += elapsedTime;
	if (ghost->currentFrameTime > _cGhostFrameTime)
	{
		ghost->frame++;
		if (ghost->frame >= 2) //2 represents the column in the spritesheet
			ghost->frame = 0;//resets
		ghost->currentFrameTime = 0;
	}
	ghost->sourceRect->X = ghost->sourceRect->Width * ghost->frame; //moves 'legs of ghost'.
	ghost->sourceRect->Y = ghost->sourceRect->Height * ghost->direction; //rotates ghost's eyes.

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

void Pacman::Draw(int elapsedTime)
{
	// Allows us to easily create a string
	std::stringstream stream;
	
	//TESTING 
	//stream << "Pacman X: " << _pacman->position->X << " Y: " << _pacman->position->Y << " collision: "<< _pacman->collision;
	//stream << " Mouse X: " << curser.x << " Mouse Y : " << curser.y-31;

	SpriteBatch::BeginDraw(); // Starts Drawing

	if (_menu->started) 
	{
		SpriteBatch::Draw(_walls[0]->texture, _walls[0]->rectangle, nullptr);
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
		switch (_pacman->lives)
		{
		case(3):
			SpriteBatch::Draw(_cherry->texture, _cherry->position, _cherry->sourceRect);
			break;
		case(2):
			SpriteBatch::Draw(_grapefruit->texture, _grapefruit->position, _grapefruit->sourceRect);
			break;
		case(1):
			SpriteBatch::Draw(_apple->texture, _apple->position, _apple->sourceRect);
			break;

		default:
			SpriteBatch::Draw(_grape->texture, _grape->position, _grape->sourceRect);
			break;

		}
	}
	
	
	// Draws String
	_menu->stringPosition = new Vector2(Graphics::GetViewportWidth() / 2.0f, (Graphics::GetViewportHeight() / 2.0f));
	if (!_menu->started)
	{
		//creates menu detail
		SpriteBatch::Draw(_walls[0]->texture, _walls[0]->rectangle, nullptr);
		SpriteBatch::Draw(_pacman->texture, _pacman->menuPosition, _pacman->sourceRect); // Draws Pacman
		SpriteBatch::Draw(_ghosts[0]->texture, _ghosts[0]->menuPosition, _ghosts[0]->sourceRect);
		

		std::stringstream menuStream;
		menuStream << "PACMAN";
		SpriteBatch::Draw(_menu->background, _menu->rectangle, nullptr);
		_menu->stringPosition = new Vector2((Graphics::GetViewportWidth() / 2.0f)-40, (Graphics::GetViewportHeight() / 2.0f)-80);
		SpriteBatch::DrawString(menuStream.str().c_str(), _menu->stringPosition, Color::Yellow);

		//Color::White;
		std::stringstream beginStream;
		beginStream << "Begin!";
		SpriteBatch::DrawRectangle(Graphics::GetViewportWidth() / 2-120, Graphics::GetViewportHeight() / 2, 100, 100, Color::Green);
		_menu->stringPosition = new Vector2(((Graphics::GetViewportWidth() / 2)-100), (Graphics::GetViewportHeight() / 2) + 50);
		SpriteBatch::DrawString(beginStream.str().c_str(), _menu->stringPosition, Color::White);

		std::stringstream exitStream;
		exitStream << "Exit!";
		SpriteBatch::DrawRectangle(Graphics::GetViewportWidth()/2+20, Graphics::GetViewportHeight()/2,100,100,Color::Red);
		_menu->stringPosition = new Vector2(((Graphics::GetViewportWidth() / 2) + 50), (Graphics::GetViewportHeight() / 2) + 50);
		SpriteBatch::DrawString(exitStream.str().c_str(), _menu->stringPosition, Color::White);

		for (int i = 0; i < MUNCHIECOUNT; i++)
		{
			_munchies[i]->frameCount++;
		}

	}	
	else
	{
		_menu->stringPosition = new Vector2(Graphics::GetViewportWidth() / 2.0f, Graphics::GetViewportHeight() / 2.0f);
		stream << "SCORE: " << _pacman->score << " LIVES: " << _pacman->lives;
		SpriteBatch::DrawString(stream.str().c_str(), _stringPosition, Color::White);
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