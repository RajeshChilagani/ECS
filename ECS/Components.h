#pragma once
#include <string>
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