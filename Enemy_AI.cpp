#include "Enemy_AI.h"

namespace NCL::CSC8503 {

	EnemyAI::EnemyAI(GameObject* thePlayerPassthrough) {
		playerTag = new playerState(this, thePlayerPassthrough);
		preResponseCapture(thePlayerPassthrough);
		blankResponse = new playerState::totalState();
		blankResponse->forwardResponse = playerState::currentStateForward::non_forward;
		blankResponse->sidewardsResponse = playerState::currentStateSideward::non_side;
	}


	EnemyAI::~EnemyAI() {
		playerTag;
	}

	void EnemyAI::preResponseCapture(GameObject* playerCap) { //Set the initial position
		prePlayerPositionCapture = playerCap->GetTransform().GetPosition();
	}

	void EnemyAI::UpdateDistanceFromCentre() {
		distanceFromCentre = (Vector3{ 0,-20,0 } - this->GetTransform().GetPosition()).Length();
	}

	void EnemyAI::updatePlayerClose() {
		float distanceBetween = playerTag->getCurrentPosition().distanceFromEnemy;
		if (distanceBetween < 10) {
			setPlayerClose(true);
		}
		else
		{
			setPlayerClose(false);
		}
	}

	void EnemyAI::updateInAgressionRadius() {
		if (distanceFromCentre < agressionRadius) {
			insideAgressionRadius = true;
		}
		insideAgressionRadius = false;
	}
}