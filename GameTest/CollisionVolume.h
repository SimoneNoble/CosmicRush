#pragma once

enum class VolumeType {
	AABB	= 1,
	Sphere	= 2
};

class CollisionVolume {
public:
	CollisionVolume() : type(VolumeType::AABB) {} // default to AABB
	virtual ~CollisionVolume() {}

	VolumeType GetType() const { return type; }

protected:
	VolumeType type;
};