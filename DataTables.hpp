#pragma once

#include "ResourceIdentifiers.hpp"

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Color.hpp>

#include <vector>
#include <functional>


class Knight;

struct Direction
{
	Direction(float angle, float distance)
	: angle(angle)
	, distance(distance)
	{
	}

	float angle;
	float distance;
};

struct KnightData
{
	int								hitpoints;
	float							speed;
	Textures::ID					texture;
	sf::Time						fireInterval;
	sf::Time						swingInterval;
	std::vector<Direction>			directions;
};

struct ProjectileData
{
	int								damage;
	float							speed;
	Textures::ID					texture;
};

struct SwordData
{
	int								damage;
	float							speed;
	Textures::ID					texture;
};

struct PickupData
{
	std::function<void(Knight&)>	action;
	Textures::ID					texture;
};

struct DoorData
{
	std::function<void(Knight&)>	action;
	Textures::ID					texture;
	int								hitpoints;
};

std::vector<KnightData>		initializeKnightData();
std::vector<ProjectileData>	initializeProjectileData();
std::vector<SwordData>		initializeSwordData();
std::vector<PickupData>		initializePickupData();
std::vector<DoorData>		initializeDoorData();

