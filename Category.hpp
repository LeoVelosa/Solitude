#pragma once
namespace Category
{
	enum Type
	{
		None				= 0,
		Scene				= 1 << 0,
		PlayerKnight      	= 1 << 1,
		AlliedKnight		= 1 << 2,
		EnemyKnight         = 1 << 3,
		Pickup				= 1 << 4,
		AlliedProjectile    = 1 << 5,
		EnemyProjectile     = 1 << 6,
		AlliedSword			= 1 << 7,
		EnemySword			= 1 << 8,
		Door				= 1 << 9,
		SoundEffect			= 1 << 10,

		Knight = PlayerKnight | AlliedKnight | EnemyKnight,
		Projectile = AlliedProjectile | EnemyProjectile,
		Sword = AlliedSword | EnemySword
	};
}