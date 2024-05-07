//------------------------------------------------------------------------
// GameTest.cpp
//------------------------------------------------------------------------
#include "stdafx.h"
//------------------------------------------------------------------------
#include <windows.h>
#include <math.h>
//------------------------------------------------------------------------
#include "app\app.h"
//------------------------------------------------------------------------
#include "Player.h"
#include "Enemy.h"
#include "TestObject.h"
#include "CollisionDetection.h"
#include <iostream>
#include "PhysicsManager.h"
#include "NavigationGrid.h"
#include "Pathfinding.h"
#include "Wall.h"

enum
{
	//ANIM_FORWARDS,
	//ANIM_BACKWARDS,
	ANIM_DEFAULT,
	ANIM_LEFT,
	ANIM_RIGHT,
	BLOW_UP,
};
//------------------------------------------------------------------------

enum GameState
{
	START_SCREEN,
	COUNTDOWN,
	PLAYING,
	END_SCREEN
};

GameState gameState = START_SCREEN;

enum Difficulty
{
	EASY,
	MEDIUM,
	HARD
};

Difficulty selectedDifficulty = EASY;

Player* player;
Enemy* enemy;
std::vector<Wall*>raceTrackWalls; 
PhysicsManager physicsManager;
Pathfinder* pathfinder = new Pathfinder();
Wall* hypercore;
Wall* asteroid;

bool areShipsColliding = false;
bool wallCollision = false;
static bool debugMode = false;
bool isPaused = false;
float speed;
float countdownTimer = 0.0f;
int countdownValue = 3;
bool countdownStarted = false;
float raceTimer = 0.0f;
bool playerWon = false;
bool enemyWon = false;

void AddRaceTrackWallsH(std::vector<Wall*>& walls, int numWalls, float startX, float startY, float spacing, float rotationDegrees);
void AddRaceTrackWallsV(std::vector<Wall*>& walls, int numWalls, float startX, float startY, float spacing, float rotationDegrees);

void Init()
{
	player = new Player(".\\TestData\\Ship2.png", 7, 4);
	player->SetPosition(300, 50);
	player->SetAngle(1.6f); // = - 90 degrees from east
	float speed = 1.0f / 15.0f;
	player->CreateAnimation(ANIM_RIGHT, speed, { 2 });
	player->CreateAnimation(ANIM_LEFT, speed, { 5 });
	player->CreateAnimation(BLOW_UP, speed, { 22,23,24,25,26,27,28 });
	player->CreateAnimation(ANIM_DEFAULT, speed, { 1 });
	player->SetScale(1.0f);

	enemy = new Enemy(".\\TestData\\EnemyShip.png", 7, 4);
	enemy->SetPosition(250, 50);
	enemy->SetAngle(1.6f); // = - 90 degrees from east
	enemy->SetScale(1.0f);
	enemy->CreateAnimation(BLOW_UP, speed, { 21,22,23,24,25,26,27,26,25,24,23,22 });
	enemy->CreateAnimation(ANIM_DEFAULT, speed, { 0 });
	enemy->enemyCollisionVolume.SetCenter(Vector2(250, 50));

	hypercore = new Wall(".\\TestData\\Core2.png", 1, 1);
	hypercore->SetPosition(690, 700);  
	hypercore->SetScale(1.0f);
	hypercore->SetAngle(0.0f);
	hypercore->wallCollisionVolumeB.SetCenter(Vector2(690, 700));  
	//hypercore->CreateAnimation(BLOW_UP, speed, { 5,6,7 });
	//hypercore->CreateAnimation(ANIM_DEFAULT, speed, { 4 });

	asteroid = new Wall(".\\TestData\\Asteroid.png", 1, 1);
	asteroid->SetPosition(500, 200);
	asteroid->SetScale(1.0f);
	asteroid->SetAngle(0.0f);

	player->blowUpTimer = 0.0f;
	enemy->blowUpTimer = 0.0f;

	AddRaceTrackWallsH(raceTrackWalls, 20, 25.0f, 5.0f, 50.0f, 0);
	AddRaceTrackWallsH(raceTrackWalls, 20, 25.0f, 758.0f, 50.0f, 0);
	AddRaceTrackWallsV(raceTrackWalls, 15, 1.0f, 35.0f, 50.0f, 1.6f);
	AddRaceTrackWallsV(raceTrackWalls, 15, 1000.0f, 35.0f, 50.0f, 1.6f);
	
	AddRaceTrackWallsV(raceTrackWalls, 5, 215.0f, 40.0f, 50.0f, 1.6f);
	AddRaceTrackWallsH(raceTrackWalls, 5, 140.0f, 325.0f, 50.0f, 0);
	AddRaceTrackWallsV(raceTrackWalls, 6, 365.0f, 40.0f, 50.0f, 1.6f);
	AddRaceTrackWallsV(raceTrackWalls, 11, 105.0f, 115.0f, 50.0f, 1.6f);
	AddRaceTrackWallsV(raceTrackWalls, 13, 625.0f, 150.0f, 50.0f, 1.6f);
	AddRaceTrackWallsH(raceTrackWalls, 8, 140.0f, 645.0f, 50.0f, 0);
	AddRaceTrackWallsH(raceTrackWalls, 5, 200.0f, 580.0f, 50.0f, 0);
	AddRaceTrackWallsV(raceTrackWalls, 3, 525.0f, 525.0f, 50.0f, 1.6f);
	AddRaceTrackWallsV(raceTrackWalls, 3, 175.0f, 450.0f, 50.0f, 1.6f);
	AddRaceTrackWallsH(raceTrackWalls, 6, 250.0f, 490.0f, 50.0f, 0);
	AddRaceTrackWallsH(raceTrackWalls, 9, 200.0f, 420.0f, 50.0f, 0);
	AddRaceTrackWallsH(raceTrackWalls, 5, 650.0f, 130.0f, 50.0f, 0);
	AddRaceTrackWallsV(raceTrackWalls, 10, 875.0f, 150.0f, 50.0f, 1.6f);
	AddRaceTrackWallsV(raceTrackWalls, 11, 750.0f, 290.0f, 50.0f, 1.6f);

}

void AddRaceTrackWallsH(std::vector<Wall*>& walls, int numWalls, float startX, float startY, float spacing, float rotationDegrees) {
	float currentX = startX;
	float currentY = startY;

	for (int i = 0; i < numWalls; i++) {
		Wall* wall = new Wall(".\\TestData\\CloudSprite.png", 1, 1);
		wall->SetPosition(currentX, currentY);
		wall->SetScale(1.0f);
		wall->SetAngle(rotationDegrees);

		wall->wallCollisionVolumeB.SetCenter(Vector2(currentX, currentY));

		walls.push_back(wall);

		currentX += spacing;
	}
}

void AddRaceTrackWallsV(std::vector<Wall*>& walls, int numWalls, float startX, float startY, float spacing, float rotationDegrees) {
	float currentX = startX;
	float currentY = startY;

	for (int i = 0; i < numWalls; i++) {
		Wall* wall = new Wall(".\\TestData\\CloudSprite.png", 1, 1);
		wall->SetPosition(currentX, currentY);
		wall->SetScale(1.0f);
		wall->SetAngle(rotationDegrees); 

		wall->wallCollisionVolumeA.SetCenter(Vector2(currentX, currentY));

		walls.push_back(wall); 

		currentY += spacing; 
	}
}

void DrawDebugLine(float x, float y, float width, float height) {
	float halfWidth = width / 2.0f;
	float halfHeight = height / 2.0f;
	float r = 1.0f, g = 0.0f, b = 0.0f;

	// corners:
	float x1 = x - halfWidth;
	float y1 = y - halfHeight;
	float x2 = x + halfWidth;
	float y2 = y - halfHeight;
	float x3 = x + halfWidth;
	float y3 = y + halfHeight;
	float x4 = x - halfWidth;
	float y4 = y + halfHeight;

	App::DrawLine(x1, y1, x2, y2, r, g, b);
	App::DrawLine(x2, y2, x3, y3, r, g, b);
	App::DrawLine(x3, y3, x4, y4, r, g, b);
	App::DrawLine(x4, y4, x1, y1, r, g, b);
}

// deltaTime (ms) < APP_MAX_FRAME_RATE
void Update(float deltaTime)
{
	if (gameState == START_SCREEN) {
		if (App::GetController().CheckButton(XINPUT_GAMEPAD_A, true)) {
			
			if (selectedDifficulty == 0) {
				speed = 0.1f;
			}
			else if (selectedDifficulty == 1) {
				speed = 0.2f;
			}
			else if (selectedDifficulty == 2) {
				speed = 0.3f;
			}
			
			gameState = COUNTDOWN;
			countdownStarted = true;

		}
	}
	else if (gameState == COUNTDOWN) {
		countdownTimer += deltaTime / 1000.0f; 

		if (countdownTimer >= 1.0f) {
			countdownValue--;

			if (countdownValue > 0) {
				countdownTimer = 0.0f;
			}
			else {
				gameState = PLAYING;
				countdownStarted = false;
				countdownTimer = 0.0f;
				countdownValue = 3;
			}
		}
	}
	else if (gameState == PLAYING) {

		if (App::GetController().CheckButton(XINPUT_GAMEPAD_DPAD_UP, true)) {
			isPaused = !isPaused;
		}

		if (!isPaused) {

			raceTimer += deltaTime / 1000.0f;

			player->Update(deltaTime);
			enemy->Update(deltaTime);
			hypercore->Update(deltaTime);

			areShipsColliding = CollisionDetection::AABBvsAABB(player->playerCollisionVolume, enemy->enemyCollisionVolume);

			player->playerCollisionVolume.SetCenter(Vector2(player->Vector2GetPosition().x, player->Vector2GetPosition().y));

			wallCollision = false;

			for (Wall* wall : raceTrackWalls) {
				if (CollisionDetection::AABBvsAABB(player->playerCollisionVolume, wall->wallCollisionVolumeA) ||
					CollisionDetection::AABBvsAABB(player->playerCollisionVolume, wall->wallCollisionVolumeB)) {
					wallCollision = true;
					player->SetAnimation(BLOW_UP);
					physicsManager.ResolvePlayerWallCollision(player, wall);
					break;
				}
			}

			physicsManager.Update(deltaTime, player, enemy, areShipsColliding);

			const float maxSpeed = 2.0f;
			const float acceleration = 0.06f;
			const float deceleration = 0.01f;

			Vector2 currentVelocity = player->GetVelocity();

			float inputX = App::GetController().GetLeftThumbStickX();
			float inputY = App::GetController().GetLeftThumbStickY();

			if (inputX > 0.5f || inputX < -0.5f || inputY > 0.5f || inputY < -0.5f)
			{
				currentVelocity.x += inputX * acceleration;
				currentVelocity.y += inputY * acceleration;
			}
			else
			{
				float speed = currentVelocity.Length();
				if (speed > deceleration * deltaTime)
				{
					currentVelocity -= currentVelocity.Normalised() * deceleration * deltaTime;
				}
				else
				{
					currentVelocity = Vector2(0,0);
				}
			}

			float playerSpeed = currentVelocity.Length();
			if (currentVelocity.x > 2 && currentVelocity.y > 2)
			{
				currentVelocity.Normalised();

				currentVelocity *= maxSpeed;
			}

			player->SetVelocity(currentVelocity);

			Vector2 currentPosition = player->Vector2GetPosition();
			Vector2 newPosition = currentPosition + currentVelocity;
			player->SetPosition(newPosition.x, newPosition.y);
			player->playerCollisionVolume.SetCenter(newPosition);

			/*if (App::GetController().GetLeftThumbStickX() > 0.5f)
			{
				float x, y;
				player->GetPosition(x, y);
				x += 2.0f;
				player->SetPosition(x, y);
				player->playerCollisionVolume.SetCenter(Vector2(x, y));
			}
			if (App::GetController().GetLeftThumbStickX() < -0.5f)
			{
				float x, y;
				player->GetPosition(x, y);
				x -= 2.0f;
				player->SetPosition(x, y);
				player->playerCollisionVolume.SetCenter(Vector2(x, y));
			}
			if (App::GetController().GetLeftThumbStickY() > 0.5f)
			{
				float x, y;
				player->GetPosition(x, y);
				y += 2.0f;
				player->SetPosition(x, y);
				player->playerCollisionVolume.SetCenter(Vector2(x, y));
			}
			if (App::GetController().GetLeftThumbStickY() < -0.5f)
			{
				float x, y;
				player->GetPosition(x, y);
				y -= 2.0f;
				player->SetPosition(x, y);
				player->playerCollisionVolume.SetCenter(Vector2(x, y));
			}*/
			if (App::GetController().CheckButton(XINPUT_GAMEPAD_DPAD_UP, false))
			{
				//player->SetScale(player->GetScale() + 0.1f);
				//player->SetAnimation(BLOW_UP);
			}
			if (App::GetController().CheckButton(XINPUT_GAMEPAD_DPAD_DOWN, false))
			{
				//player->SetScale(player->GetScale() - 0.1f);
				player->SetAnimation(-1);
				debugMode = !debugMode;
			}
			if (App::GetController().CheckButton(XINPUT_GAMEPAD_DPAD_LEFT, false))
			{
				player->SetAngle(player->GetAngle() + 0.05f);
				//player->SetAnimation(ANIM_LEFT);
			}
			if (App::GetController().CheckButton(XINPUT_GAMEPAD_DPAD_RIGHT, false))
			{
				player->SetAngle(player->GetAngle() - 0.05f);
				//player->SetAnimation(ANIM_RIGHT);
			}
			if (App::GetController().CheckButton(XINPUT_GAMEPAD_A, true))
			{
				player->SetAnimation(-1);
				player->Shoot();
				App::PlaySound(".\\TestData\\Test.wav");
				hypercore->SetAnimation(BLOW_UP); // currently trying to see the hypercore
			}

			// check if player/enemy is in BLOW_UP state and update the timer:
			if (player->IsInAnimation(BLOW_UP)) {
				player->blowUpTimer += deltaTime / 1000.0f;
				if (player->blowUpTimer >= 2.0f) {
					player->SetAnimation(ANIM_DEFAULT);
					player->blowUpTimer = 0.0f;
				}
			}

			if (enemy->IsInAnimation(BLOW_UP)) {
				enemy->blowUpTimer += deltaTime / 1000.0f;
				if (enemy->blowUpTimer >= 2.0f) {
					enemy->SetAnimation(ANIM_DEFAULT);
					speed = speed / 0.66f;
					enemy->blowUpTimer = 0.0f;
				}
			}

			player->UpdateBullets(deltaTime);

			auto& bullets = player->bullets;
			for (auto it = bullets.begin(); it != bullets.end();) {
				if (CollisionDetection::AABBvsAABB(it->collisionVolume, enemy->enemyCollisionVolume)) {
					enemy->SetAnimation(BLOW_UP);
					player->blowUpTimer = 0.0f;
					enemy->blowUpTimer = 0.0f;
					it = bullets.erase(it);
					speed = speed * 0.66f;
				}
				else {
					bool hitWall = false;
					for (Wall* wall : raceTrackWalls) {
						if (CollisionDetection::AABBvsAABB(it->collisionVolume, wall->wallCollisionVolumeA) ||
							CollisionDetection::AABBvsAABB(it->collisionVolume, wall->wallCollisionVolumeB)) {
							hitWall = true;
							break;
						}
					}

					if (hitWall) {
						it = bullets.erase(it);
					}
					else {
						++it;
					}
				}
			}

			if (CollisionDetection::AABBvsAABB(player->playerCollisionVolume, enemy->enemyCollisionVolume)) {
				player->SetAnimation(BLOW_UP);
				enemy->SetAnimation(BLOW_UP);
				player->blowUpTimer = 0.0f;
				enemy->blowUpTimer = 0.0f;
			}

			std::cout << "Test Sprite Volume Center: " << player->playerCollisionVolume.GetCenter().x << ", " << player->playerCollisionVolume.GetCenter().y << std::endl;
			std::cout << "Block Volume Center: " << enemy->enemyCollisionVolume.GetCenter().x << ", " << enemy->enemyCollisionVolume.GetCenter().y << std::endl;

			pathfinder->PathfindingFunction(enemy, deltaTime, speed);

			if (CollisionDetection::AABBvsAABB(player->playerCollisionVolume, hypercore->wallCollisionVolumeB)) {
				gameState = END_SCREEN;
				playerWon = true;
			}

			if (CollisionDetection::AABBvsAABB(enemy->enemyCollisionVolume, hypercore->wallCollisionVolumeB)) {
				gameState = END_SCREEN;
				enemyWon = true;
			}
		}
	}
}

void Render()
{
	//Render 'stars':
	const int screenWidth = 1000;
	const int screenHeight = 1000;
	const int numStars = 100;
	const float dotSize = 2.0f;
	const float dotColourR = 1.0f;
	const float dotColourG = 1.0f;
	const float dotColourB = 1.0f;

	srand(static_cast<unsigned int>(time(NULL)));  // seed random number generator

	for (int i = 0; i < numStars; ++i) {
		float x = static_cast<float>(rand() % screenWidth);
		float y = static_cast<float>(rand() % screenHeight);
		App::DrawLine(x, y, x + dotSize, y, dotColourR, dotColourG, dotColourB);
	}

	if (gameState == START_SCREEN) {
		App::Print(460, 460, "Cosmic Rush");
		App::Print(450, 445, "------------");

		App::Print(220, 420, "In a galaxy's final hour, where the last remaining hyper-core is essential for survival, two rival factions");
		App::Print(260, 400, "send their best pilots to the vastness of space to secure their people's hope for tomorrow");
		App::Print(410, 260, "Press SPACE to begin");

		if (App::GetController().CheckButton(XINPUT_GAMEPAD_DPAD_UP, true))
		{
			// Move the cursor up to the previous difficulty option
			if (selectedDifficulty > EASY)
			{
				selectedDifficulty = static_cast<Difficulty>(selectedDifficulty - 1);
			}
		}
		else if (App::GetController().CheckButton(XINPUT_GAMEPAD_DPAD_DOWN, true))
		{
			// Move the cursor down to the next difficulty option
			if (selectedDifficulty < HARD)
			{
				selectedDifficulty = static_cast<Difficulty>(selectedDifficulty + 1);
			}
		}

		App::Print(478, 360, "Beginner");
		App::Print(470, 330, "Intermediate");
		App::Print(471, 300, "Impossible!");

		if (selectedDifficulty == 0) {
			App::Print(425, 360, "-->                                <--");
		}
		else if (selectedDifficulty == 1) {
			App::Print(425, 330, "-->                                <--");
		}
		else if (selectedDifficulty == 2) {
			App::Print(425, 300, "-->                                <--");
		}
	}

	else if (gameState == COUNTDOWN) {
		if (countdownStarted) {
			char countdownText[3];
			sprintf(countdownText, "%d", countdownValue);
			App::Print(500, 400, countdownText);
		}
	}

	else if (gameState == PLAYING) {

		player->Draw();
		player->RenderBullets();
		enemy->Draw();
		hypercore->Draw();
		asteroid->Draw();

		for (Wall* wall : raceTrackWalls)
		{
			wall->Draw();
		}

		/*if (wallCollision) {
			App::Print(100, 170, "Objects are touching");
		}
		else {
			App::Print(100, 170, "Objects are not touching");
		}*/

		if (isPaused) {
			App::Print(400, 450, "Game Paused");
		}

		/*char debugInfo[256];
		sprintf(debugInfo, "Player Volume Center: X: %.2f, Y: %.2f", player->playerCollisionVolume.GetCenter().x, player->playerCollisionVolume.GetCenter().y);
		App::Print(100, 90, debugInfo);

		sprintf(debugInfo, "Core Volume Center: X: %.2f, Y: %.2f", enemy->enemyCollisionVolume.GetCenter().x, enemy->enemyCollisionVolume.GetCenter().y);
		App::Print(100, 110, debugInfo);

		sprintf(debugInfo, "Player Center: X: %.2f, Y: %.2f", player->Vector2GetPosition().x, player->Vector2GetPosition().y);
		App::Print(100, 130, debugInfo);

		sprintf(debugInfo, "Enemy Center: X: %.2f, Y: %.2f", enemy->Vector2GetPosition().x, enemy->Vector2GetPosition().y);
		App::Print(100, 150, debugInfo);*/

		char timerText[64];
		sprintf(timerText, "Time: %.2f seconds", raceTimer);
		App::Print(25, 710, timerText);

		float x, y;
		player->GetPosition(x, y);

		if (debugMode) {
			const float bulletWidth = 5.0f;
			const float bulletHeight = 1.0f;
			for (const auto& bullet : player->bullets) {
				DrawDebugLine(bullet.x, bullet.y, bulletWidth, bulletHeight);
			}

			float playerX, playerY;
			player->GetPosition(playerX, playerY);
			DrawDebugLine(playerX, playerY, player->GetWidth(), player->GetHeight());

			float enemyX, enemyY;
			enemy->GetPosition(enemyX, enemyY);
			DrawDebugLine(enemyX, enemyY, enemy->GetWidth(), enemy->GetHeight());

		}

	}

	else if (gameState == END_SCREEN) {
		if (playerWon) {
			App::Print(430, 440, "You Win!");
			App::Print(410, 420, "You retrieved the hyper-core, well done!");
			char timerText[64];
			sprintf(timerText, "Your time: %.2f seconds", raceTimer);
			App::Print(400, 400, timerText);
		}
		else if (enemyWon) {
			App::Print(430, 440, "You Lose!");
			App::Print(410, 420, "The opponent stole the hyper-core");
			char timerText[64];
			sprintf(timerText, "Time to beat: %.2f seconds", raceTimer);
			App::Print(400, 400, timerText);
		}
	}
}

void Shutdown()
{
	delete player;
	delete enemy;
	delete pathfinder;
	pathfinder = nullptr;

	for (Wall* wall : raceTrackWalls)
	{
		delete wall;
	}
}
