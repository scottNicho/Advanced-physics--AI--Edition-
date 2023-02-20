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
			currentPlayerAction = still;
			playerTrack = nullptr;
			enemy = theEnemy;

		};

		
		enum currentState
		{
			still,
			run_right,
			run_left,
			at_enemy,
			away,
			jumping,
			jump_right,
			jump_left,
			jump_away,
			non
		};

		struct PositionData
		{
			Vector3 playerPosition = { 0,0,0 };
			Vector3 relativePosition = {0,0,0};
			float distanceFrom = 0.0f;
			currentState playerState = non;

		};


		void setPlayerTrack(GameObject* thePlayer) {
			playerTrack = thePlayer;
		}

		GameObject* getPlayerTrack() {
			return playerTrack;
		}

		currentState getCurrentPlayerAction() {
			return currentPlayerAction;
		}

		void setCurrentPlayerAction(currentState newState) {
			currentPlayerAction = newState;
		}

		void updateState() {

		}

	protected:
		currentState currentPlayerAction;
		GameObject* playerTrack;
		GameObject* enemy;

	};
};