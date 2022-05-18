#include "DataTables.hpp"
#include "Knight.hpp"
#include "Projectile.hpp"
#include "Pickup.hpp"
#include "Sword.hpp"
#include "Door.hpp"


// For std::bind() placeholders _1, _2, ...
using namespace std::placeholders;

std::vector<KnightData> initializeKnightData()
{
	std::vector<KnightData> data(Knight::TypeCount);

	data[Knight::Green].hitpoints = 100;
	data[Knight::Green].speed = 200.f;
	data[Knight::Green].fireInterval = sf::seconds(1);
	data[Knight::Green].swingInterval = sf::seconds(2);
	data[Knight::Green].texture = Textures::GreenForward;

	data[Knight::AxeKnight].hitpoints = 100;
	data[Knight::AxeKnight].speed = 80.f;
	data[Knight::AxeKnight].texture = Textures::AxeKnight;
	data[Knight::AxeKnight].swingInterval = sf::seconds(1);

	data[Knight::Boss].hitpoints = 1000;
	data[Knight::Boss].speed = 100.f;
	data[Knight::Boss].texture = Textures::Boss;
	data[Knight::Boss].swingInterval = sf::seconds(2);
	data[Knight::Boss].fireInterval = sf::seconds(1);
	
	data[Knight::GunKnight].hitpoints = 40;
	data[Knight::GunKnight].speed = 50.f;
	data[Knight::GunKnight].texture = Textures::GunKnight;
	data[Knight::GunKnight].directions.push_back(Direction(  180.f,  50.f));
	data[Knight::GunKnight].directions.push_back(Direction(   0.f,  50.f));
	data[Knight::GunKnight].directions.push_back(Direction(  180.f,  50.f));
	data[Knight::GunKnight].directions.push_back(Direction(  0.f,  50.f));
	data[Knight::GunKnight].fireInterval = sf::seconds(2);

	return data;
}

std::vector<ProjectileData> initializeProjectileData()
{
	std::vector<ProjectileData> data(Projectile::TypeCount);

	data[Projectile::AlliedBullet].damage = 10;
	data[Projectile::AlliedBullet].speed = 300.f;
	data[Projectile::AlliedBullet].texture = Textures::Bullet;

	data[Projectile::EnemyBullet].damage = 10;
	data[Projectile::EnemyBullet].speed = 300.f;
	data[Projectile::EnemyBullet].texture = Textures::Bullet;
	
	return data;
}

std::vector<SwordData> initializeSwordData()
{
	std::vector<SwordData> data(Sword::TypeCount);

	data[Sword::AlliedBasicSword].damage = 10;
	data[Sword::AlliedBasicSword].speed = 100.f;
	data[Sword::AlliedBasicSword].texture = Textures::Sword;

	data[Sword::EnemyBasicSword].damage = 10;
	data[Sword::EnemyBasicSword].speed = 300.f;
	data[Sword::EnemyBasicSword].texture = Textures::Sword;

	data[Sword::EnemyBossSword].damage = 10;
	data[Sword::EnemyBossSword].speed = 300.f;
	data[Sword::EnemyBossSword].texture = Textures::BossSword;
	
	return data;
}

std::vector<PickupData> initializePickupData()
{
	std::vector<PickupData> data(Pickup::TypeCount);
	
	data[Pickup::HealthRefill].texture = Textures::HealthRefill;
	data[Pickup::HealthRefill].action = [] (Knight& a) { a.repair(25); };
	
	data[Pickup::Speed].texture = Textures::Speed;
	data[Pickup::Speed].action = std::bind(&Knight::increaseSpeed, _1);
	
	//data[Pickup::FireRate].texture = Textures::FireRate;
	//data[Pickup::FireRate].action = std::bind(&Knight::increaseFireRate, _1);

	return data;
}

std::vector<DoorData> initializeDoorData()
{
	std::vector<DoorData> data(Door::TypeCount);
	
	data[Door::ExitDoor].texture = Textures::Door;
	data[Door::ExitDoor].hitpoints = 1;
	data[Door::ExitDoor].action = [] (Knight& a) { a.repair(25); };

	return data;
}
