#ifndef PATHFINDING_VARIABLES_H
#define PATHFINDING_VARIABLES_H

#pragma once
#include "Vector2.h"
#include "NavigationGrid.h"
#include "Enemy.h"

class Pathfinder {
public:

	Vector2 currentNode;
	int nodeCount = 1;
	bool completedPath = false;
	bool opponentKnocked = false;

	Vector2 outputVec = {};

	/*std::vector<Vector2> testNodes;

	Vector2 outputVec = {};
	Vector2 getCurrentNode() {
		return currentNode;
	}

	Vector2 getOutputVec() {
		return outputVec;
	}

	std::vector<Vector2> GetTestNodes() const { 
		return testNodes; 
	}

	int getNodeCount() {
		return nodeCount;
	}*/

	void PathfindingFunction(Enemy* enemy, float dt, float speed);

	Vector2 getOutputVec() {
		return outputVec;
	}

};


#endif // PATHFINDING_VARIABLES_H