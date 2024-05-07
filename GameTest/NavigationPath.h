#pragma once
#include "Vector2.h"
#include <vector>

class NavigationPath {
public:
	NavigationPath() {}
	~NavigationPath() {}

	void	Clear() {
		waypoints.clear();
	}
	void	PushWaypoint(const Vector2& wp) {
		waypoints.emplace_back(wp);
	}
	bool	PopWaypoint(Vector2& waypoint) {
		if (waypoints.empty()) {
			return false;
		}
		waypoint = waypoints.back();
		waypoints.pop_back();
		return true;
	}

protected:
	std::vector <Vector2> waypoints;
};

