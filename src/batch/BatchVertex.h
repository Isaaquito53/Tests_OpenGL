#pragma once

struct Vec2
{
	float x, y;
};

struct Vec3
{
	float x, y, z;
};

struct Vec4
{
	float r, g, b, a;
};

/*
struct BatchVertex
{
	Vec3 Position;
	Vec4 Color;
};
*/

struct BatchVertex
{
	Vec3 Position;
	Vec4 Color;
	Vec2 Texture;
	Vec3 Normal;
};