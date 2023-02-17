#pragma once
#include<iostream>
#include<string>
#include "GameObject.h"
#include<vector>
#include"Vector3.h"
#include<cmath>
#include<set>



using namespace std;
namespace NCL::CSC8503 {

	class EnemyAI :public GameObject {
	public:

		void UpdateDistanceFromCentre() {
			distanceFromCentre = (Vector3{0,-20,0} - this->GetTransform().GetPosition()).Length();
		}

		void togglePlayerClose() {
			playerClose = (!playerClose);
		}

		void setAggressionLevel(float newAgressionLevel) {
			agressionLevel = newAgressionLevel;
		}

		float angleToTarget(GameObject* targetPlayer) {
			Vector3 targetPosition = targetPlayer->GetTransform().GetPosition();
			Vector3 enemyPosition = this->GetTransform().GetPosition();
			Vector3 playerToTarget = (targetPosition) - (enemyPosition);
			float targetX = targetPosition.x;float targetY = targetPosition.y;
			float enemyX = enemyPosition.x;float enemyY = enemyPosition.y;
			Vector3 playerOrientation = this->GetTransform().GetOrientation().ToEuler();
			float angleBetween = atan2((targetX*enemyY)-(enemyX*targetY),(enemyX*targetX)+(enemyY*targetY));
			return angleBetween;
		}

	protected:
		const  Vector3 centreSafteyCircle = Vector3{0,-20,0};
		float distanceFromCentre = 0;
		float agressionLevel = 0.5;
		const float innerCircleRadius = 8;
		const float outerCircleRadius = 15;
		bool playerClose = false;
	};

}