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
	class playerState :public GameObject {
	public:

		playerState(GameObject* theEnemy) {
			playerForwardState = still;
			playerSideState = still_side;
			playerTrack = nullptr;
			enemy = theEnemy;

		};

		struct PositionData
		{
			Vector3 playerPosition;
			Vector3 relativePosition;
			float distanceFromEnemy;
			float distanceFromCentre;

		};
		
		enum currentStateForward
		{
			still,
			forwards,
			backwards,
			jumping,
			jumping_forwards,
			jumping_backwards,
			non_forward
		};

		enum currentStateSideward
		{
			still_side,
			run_right,
			run_left,
			jumping_right,
			jumping_left,
			non_side

		};


		void setCurrentPosition() {
			PreviousPosition = currentPosition;
			Vector3 playerPos = this->GetTransform().GetPosition();
			Vector3 playerEnemyPos = playerPos - (enemy->GetTransform().GetPosition());
			float PlayerDistEnemy = playerEnemyPos.Length();
			float PlayerDistCentre = (playerPos - Vector3{ 0,-20,0 }).Length();
			currentPosition = PositionData{ playerPos,playerEnemyPos,PlayerDistEnemy,PlayerDistCentre };
		}

		void setIsPlayerJumping() {
			if (currentPosition.playerPosition.y > PreviousPosition.playerPosition.y && currentPosition.playerPosition.y > 1) {
				playerIsJumping = true;
				return;
			}
			playerIsJumping = false;
		}

		bool getPlayerIsJumping() {
			return playerIsJumping;
		}

		void updatePlayerForwardState() {
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

		void updatePlayerForwardStateJumping() {
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

		void updatePlayerSideState() {
			float AnglePreviousCurrent = Vector3::Dot(PreviousPosition.relativePosition, currentPosition.relativePosition);//the angle between the players 2 positions relative to teh player
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

		void updatePlayerSideStateJumping() {
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

		void setPlayerTrack(GameObject* thePlayer) {
			playerTrack = thePlayer;
		}

		GameObject* getPlayerTrack() {
			return playerTrack;
		}

		currentStateForward getPlayerForwardState() {
			return playerForwardState;
		}

		void setPlayerForwardState(currentStateForward newState) {
			playerForwardState = newState;
		}

		currentStateSideward getPlayerSideState() {
			return playerSideState;
		}

		void setPlayerStateSideward( currentStateSideward newState) {
			playerSideState = newState;
		}

		void updateState() {

		}

	protected:

		bool playerIsJumping = false;
		currentStateForward playerForwardState;
		currentStateSideward playerSideState;
		GameObject* playerTrack;
		GameObject* enemy;
		PositionData currentPosition;// compare these 2 to set the new state 
		PositionData PreviousPosition;

	};
};
