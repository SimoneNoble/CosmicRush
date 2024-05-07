#include "stdafx.h"

//#pragma once
#include "Pathfinding.h"
#include "Vector2.h"
#include "NavigationGrid.h"
#include "Enemy.h"
#include "app\app.h"

void Pathfinder::PathfindingFunction(Enemy* enemy, float dt, float speed) {
	if (enemy == nullptr)
	{
		std::cout << "ERROR" << std::endl;
		return;
	}
	std::vector<Vector2> testNodes;
	NavigationGrid grid("RaceTrack.txt");

	NavigationPath outPath;

	Vector2 startPos(250, 50);
	Vector2 endPos(690, 690);

	bool found = grid.FindPath(startPos, endPos, outPath);

	Vector2 pos;
	while (outPath.PopWaypoint(pos)) {
		testNodes.push_back(pos);
	}

	currentNode = testNodes[nodeCount];

	for (int i = 1; i < testNodes.size(); ++i) {
		Vector2 a = testNodes[i - 1];
		Vector2 b = testNodes[i];

		//int speed = choice * 300;

		if ((enemy->Vector2GetPosition() - endPos).Length() < 2.5f) {
			completedPath = true;
			outputVec = 0;
		}

		if (((enemy->Vector2GetPosition() - currentNode).Length() < 2.5f) && completedPath == false) {
			if ((nodeCount + 1) <= testNodes.size()) {
				currentNode = testNodes[nodeCount + 1];
				nodeCount++;
				// slow down enemy here to seem more realistic
			}
		}

		outputVec = b - a;

		if (i == nodeCount) {

			Vector2 currentPos = enemy->Vector2GetPosition();

			Vector2 movement = outputVec * speed ;

			Vector2 newPos = currentPos + movement;
			enemy->SetPosition(newPos.x, newPos.y);

			enemy->enemyCollisionVolume.SetCenter(newPos);


			if (outputVec == Vector2(0, 10)) {
				enemy->SetAngle(1.6f);
			}
			else if (outputVec == Vector2(0, -10)) {
				enemy->SetAngle(4.75f);
			}
			else if (outputVec == Vector2(10, 0)) {
				enemy->SetAngle(0.0f);
			}
			else if (outputVec == Vector2(-10, 0)) {
				enemy->SetAngle(3.15f);
			}

		}

		App::DrawLine(a.x, a.y, b.x, b.y, 1, 1, 1);

	}

}







