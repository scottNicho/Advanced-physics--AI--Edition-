#include"Projectile.h"

namespace NCL::CSC8503 {
	
	Projectile::Projectile(){
		setGunType(pistol);
		aimingPitch = 0.0f;
		aimingYaw = 0.0f;
		physicsProjectile = nullptr;
		bulletDirectionVector = { 0,0,0 };
		}


	Projectile::Projectile(gun GunToUse) {
		setGunType(GunToUse);
		aimingPitch = 0.0f;
		aimingYaw = 0.0f;
		physicsProjectile = nullptr;
		bulletDirectionVector = { 0,0,0 };
	}
	


	void Projectile::setGunType(gun wepType) {
		setExplosionRadius(wepType.radius);
		setProjectilePropultionForce(wepType.projectileForce);
		setWieght(wepType.weight);
		//SetBoundingVolume(VolumeType::Sphere);
		if (wepType.affectedByGravity) {
			setAffectedByGravityTrue();
		}
		else
		{
			setAffectedByGravityFalse();
		}
	}
	

}