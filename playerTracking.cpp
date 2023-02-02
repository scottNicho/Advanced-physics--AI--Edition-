#include"playerTracking.h"


namespace NCL::CSC8503 {

	playerTracking::playerTracking() {
		playerYawOrientation = 0.0f;
		playerPitchOrientation = 0.0f;
		playerID = 1;
		teamID = 0;
		IndividualplayerScore = 0;
		type = pistol;
		playerProjectile = new Projectile();
		paintColor = { 1,1,0,1 };
		bulletsUsed = {};

	}


	void playerTracking::clearBulletsUsed() {
		int numOfUsedBullets = bulletsUsed.size();
		for (int i = 0; i < numOfUsedBullets; i++) {
			delete (bulletsUsed[i]);
		}
	}

}