#include"playerState.h"







namespace NCL::CSC8503 {
	playerState::playerState(GameObject* theEnemy, GameObject* thePlayer) {
		playerForwardState = still;
		playerSideState = still_side;
		playerTrack = thePlayer;
		enemy = theEnemy;
		setInitialPosition();
		totalResponse = {
			{"still_stillSide", still_stillSide},
            {"still_runRight", still_runRight},
            {"still_runLeft", still_runLeft},
            {"still_jumpRight", still_jumpRight},
            {"still_jumpLeft", still_jumpLeft},
            {"forwards_stillSide", forwards_stillSide},
            {"forwards_runRight", forwards_runRight},
            {"forwards_runLeft", forwards_runLeft},
            {"forwards_jumpRight", forwards_jumpRight},
            {"forwards_jumpLeft", forwards_jumpLeft},
            {"backwards_stillSide", backwards_stillSide},
            {"backwards_runRight", backwards_runRight},
            {"backwards_runLeft", backwards_runLeft},
            {"backwards_jumpRight", backwards_jumpRight},
            {"backwards_jumpLeft", backwards_jumpLeft},
            {"jumping_stillSide", jumping_stillSide},
            {"jumping_jumpRight", jumping_jumpRight},
            {"jumping_jumpLeft", jumping_jumpLeft},
            {"jumpingforward_stillSide", jumpingforward_stillSide},
            {"jumpingforward_jumpRight", jumpingforward_jumpRight},
            {"jumpingforward_jumpLeft", jumpingforward_jumpLeft},
            {"jumpingBackwards_stillSide", jumpingBackwards_stillSide},
            {"jumpingBackwards_jumpRight", jumpingBackwards_jumpRight},
            {"jumpingBackwards_jumpLeft", jumpingBackwards_jumpLeft},
		};
	}



	void playerState::updateResponse() {
		response.forwardResponse = playerForwardState;
		response.sidewardsResponse = playerSideState;
	}

	void playerState::UpdateCurrentPosition() {
		PreviousPosition = currentPosition;
		Vector3 playerPos = getPlayerTrack()->GetTransform().GetPosition();
		Vector3 playerEnemyPos = playerPos - (enemy->GetTransform().GetPosition());
		float PlayerDistEnemy = playerEnemyPos.Length();
		float PlayerDistCentre = (playerPos - Vector3{ 0,-20,0 }).Length();
		currentPosition = { playerPos,playerEnemyPos,PlayerDistEnemy,PlayerDistCentre };
	}

	void playerState::setInitialPosition() {
		Vector3 playerPos = getPlayerTrack()->GetTransform().GetPosition();
		Vector3 playerEnemyPos = playerPos - (enemy->GetTransform().GetPosition());
		float PlayerDistEnemy = playerEnemyPos.Length();
		float PlayerDistCentre = (playerPos - Vector3{ 0,-20,0 }).Length();
		currentPosition = { playerPos,playerEnemyPos,PlayerDistEnemy,PlayerDistCentre };
	}

	void playerState::setIsPlayerJumping() {
		if (currentPosition.playerPosition.y > PreviousPosition.playerPosition.y && currentPosition.playerPosition.y > 1) {
			playerIsJumping = true;
			return;
		}
		playerIsJumping = false;
	}


	void playerState::updatePlayerForwardState() {
		float changeInDistance = (currentPosition.distanceFromEnemy) - (PreviousPosition.distanceFromEnemy);
		if (abs(changeInDistance) < 3) {
			playerForwardState = still;
			return;
		}
		if (changeInDistance >= 3) {
			playerForwardState = backwards;
			return;
		}
		else
		{
			playerForwardState = forwards;
			return;
		}

	}

	void playerState::updatePlayerForwardStateJumping() {
		float changeInDistance = (currentPosition.distanceFromEnemy) - (PreviousPosition.distanceFromEnemy);
		if (abs(changeInDistance) < 3) {
			playerForwardState = jumping;
			return;
		}
		if (changeInDistance >= 3) {
			playerForwardState = jumping_backwards;
			return;
		}
		else
		{
			playerForwardState = jumping_forwards;
			return;
		}

	}

	void playerState::updatePlayerSideState() {
		float AnglePreviousCurrent = Vector3::Dot(PreviousPosition.relativePosition, currentPosition.relativePosition);//the angle between the players 2 positions relative to the player
		if (AnglePreviousCurrent > 5) {
			playerSideState = run_right;
			return;
		}
		if (AnglePreviousCurrent < 5) {
			playerSideState = run_left;
			return;
		}
		playerSideState = still_side;
	}

	void playerState::updatePlayerSideStateJumping() {
		float AnglePreviousCurrent = Vector3::Dot(PreviousPosition.relativePosition, currentPosition.relativePosition);//the angle between the players 2 positions relative to teh player
		if (AnglePreviousCurrent > 5) {
			playerSideState = jumping_right;
			return;
		}
		if (AnglePreviousCurrent < 5) {
			playerSideState = jumping_left;
			return;
		}
		playerSideState = still_side;
	}

	void playerState::updateState() {
		if (!updateFlag) {
			return;
		}
		setIsPlayerJumping();
		if (!getIsPlayerJumping()) {
			updatePlayerForwardState();
			updatePlayerSideState();
			updateResponse();
			return;
		}
		else
		{
			updatePlayerForwardStateJumping();
			updatePlayerSideStateJumping();
			updateResponse();
		}

	}

}
