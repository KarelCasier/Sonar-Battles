#pragma once
// Entity/scene node category, used to dispatch commands
namespace Category
{
	enum Type
	{
		None				= 0,
		Scene				= 1 << 0,
		PlayerMech			= 1 << 1,
		EnemyMech			= 1 << 2,
		PlayerProjectile	= 1 << 3,
		EnemyProjectile		= 1 << 4,
		Unknown				= 1 << 10,
	};
}