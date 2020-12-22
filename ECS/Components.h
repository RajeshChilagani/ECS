#pragma once
#include <string>
struct Vector
{
	float X, Y, Z;
};
struct TagComponent
{
	std::string Name;
	TagComponent() = default;
	TagComponent(const char* name)
		:Name(name)
	{}
};

struct TransformComponent
{
	float X, Y;
	TransformComponent() = default;
	TransformComponent(float O)
		:X(O)
		, Y(O)
	{}
};

struct ColliderComponent
{
	Vector Min, Max;
	ColliderComponent() = default;
	ColliderComponent(const Vector& i_Min, const Vector& i_Max)
		:Min(i_Min)
		, Max(i_Max)
	{}
};