#include"playerState.h"







namespace NCL::CSC8503 {
	playerState::playerState(GameObject* theEnemy, GameObject* thePlayer) {
		playerForwardState = still;
		playerSideState = still_side;
		playerTrack = thePlayer;
		enemy = theEnemy;
		setInitialPosition();
		oldPosition = thePlayer->GetTransform().GetPosition();
		std::map<std::array<int, 2>, totalState, ArrayCompare> totalResponse{
			{{0, 0}, {still, still_side}},
            {{0, 1}, {still, run_right}},
            {{0, 2}, {still, run_left}},
            {{0, 3}, {still, jumping_right}},
            {{0, 4}, {still, jumping_left}},
            {{1, 0}, {forwards, still_side}},
            {{1, 1}, {forwards, run_right}},
            {{1, 2}, {forwards, run_left}},
            {{1, 3}, {forwards, jumping_right}},
            {{1, 4}, {forwards, jumping_left}},
            {{2, 0}, {backwards, still_side}},
            {{2, 1}, {backwards, run_right}},
            {{2, 2}, {backwards, run_left}},
            {{2, 3}, {backwards, jumping_right}},
            {{2, 4}, {backwards, jumping_left}},
            {{3, 0}, {jumping, still_side}},
            {{3, 3}, {jumping, jumping_right}},
            {{3, 4}, {jumping, jumping_left}},
            {{4, 0}, {jumping_forwards, still_side}},
            {{4, 3}, {jumping_forwards, jumping_right}},
            {{4, 4}, {jumping_forwards, jumping_left}},
            {{5, 0}, {jumping_backwards, still_side}},
            {{5, 3}, {jumping_backwards, jumping_right}},
            {{5, 4}, {jumping_backwards, jumping_left}}
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
