#pragma once
#include "Vector2.h"
#include "NavigationPath.h"

class NavigationMap
{
public:
	NavigationMap() {}
	~NavigationMap() {}

	virtual bool FindPath(const Vector2& from, const Vector2& to, NavigationPath& outPath) = 0;
};


