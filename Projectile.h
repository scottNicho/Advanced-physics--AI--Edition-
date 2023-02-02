#pragma once



#pragma once
#include "Transform.h"
#include "CollisionVolume.h"
#include"GameObject.h"




namespace NCL::CSC8503 {




	struct gun {
		float radius;
		float projectileForce;
		float weight;// keep a >0 weight on instances. Otherwise applied forces will have no effect
		bool affectedByGravity;
	};

	static gun pistol{
		0.2f,
		1000,
		10.0f,
		false,
	};

	static gun rocket{
		20,
		20,
		5,
		true,
	};


	class NetworkObject;
	class RenderObject;
	class PhysicsObject;
	class Projectile:public GameObject {
	public:

	/*	Projectile() {
			setGunType(pistol);
			aimingPitch = 0.0f;
			aimingYaw = 0.0f;
			physicsProjectile = nullptr;
		}*/
		
		Projectile();
		Projectile(gun GunToUse);
		~Projectile() {
			physicsProjectile;
		}

		void setGunType(gun wepType);


		void setExplosionRadius(float newRadius) {
			explosionRadius = newRadius;
		}

		float getExplosionRadius() {
			return explosionRadius;
		}

		void setProjectilePropultionForce(float newForce) {
			projectilePropultionForce = newForce;
		}
		float getPojectilePropultionForce() {
			return projectilePropultionForce;
		}

		void setWieght(float newweight) {
			weight = newweight;
		}

		float getWeight() {
			return weight;
		}

		void setAffectedByGravityTrue() {
			AffectedGravity = true;
		}

		void setAffectedByGravityFalse() {
			AffectedGravity = false;
		}

		void setBulletDirectionVector(Vector3 aimedDirection) {
			bulletDirectionVector = aimedDirection.Normalised();
		}

		Vector3 getBulletDirectionVector() {
			return bulletDirectionVector;
		}

		bool ProjectileAffectedByGravity() {
			return AffectedGravity;
		}

	protected:
		float explosionRadius;
		float projectilePropultionForce;
		float weight;
		float aimingYaw;
		float aimingPitch;
		//static int TeamID;
		//static int personalID;
		bool AffectedGravity;
		PhysicsObject* physicsProjectile;
		Vector3 bulletDirectionVector;


	};

}
