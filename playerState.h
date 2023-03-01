#pragma once
#include<iostream>
#include<string>
#include "GameObject.h"
#include<vector>
#include"Vector3.h"
#include<cmath>
#include<set>
#include <unordered_map>



using namespace std;
namespace NCL::CSC8503 {
	class playerState :public GameObject {
	public:

		playerState(GameObject* theEnemy);

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

		struct totalState
		{
			currentStateForward forwardResponse;
			currentStateSideward sidewardsResponse;
		};


	
		vector <totalState>& getResponseValue(string valueNeeded) {
			return totalResponse[valueNeeded];
		}


		void updateResponse();

		void setCurrentPosition();

		void setIsPlayerJumping();

		bool getIsPlayerJumping() {
			return playerIsJumping;
		}

		void updatePlayerForwardState();

		void updatePlayerForwardStateJumping();

		void updatePlayerSideState();

		void updatePlayerSideStateJumping();

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

		void updateState();

	protected:

		bool playerIsJumping = false;
		currentStateForward playerForwardState;
		currentStateSideward playerSideState;
		GameObject* playerTrack;
		GameObject* enemy;
		PositionData currentPosition;// compare these 2 to set the new state 
		PositionData PreviousPosition;
		//vector <totalState> totalResponse[24]= { still_stillSide,still_runRight,still_runLeft,still_jumpRight,still_jumpLeft,forwards_stillSide,forwards_runRight,forwards_runLeft,forwards_jumpRight,forwards_jumpLeft,backwards_stillSide,backwards_runRight,backwards_runLeft,backwards_jumpRight,backwards_jumpLeft,jumping_stillSide,jumping_jumpRight,jumping_jumpLeft,jumpingforward_stillSide,jumpingforward_jumpRight,jumpingforward_jumpLeft,jumpingBackwards_stillSide,jumpingBackwards_jumpRight,jumpingBackwards_jumpLeft };
		std::unordered_map<std::string, std::vector<totalState>> totalResponse;
		totalState response{};


		vector <totalState> still_stillSide;
		vector <totalState> still_runRight;
		vector <totalState> still_runLeft;
		vector <totalState> still_jumpRight;
		vector <totalState> still_jumpLeft;

		vector <totalState> forwards_stillSide;
		vector <totalState> forwards_runRight;
		vector <totalState> forwards_runLeft;
		vector <totalState> forwards_jumpRight;
		vector <totalState> forwards_jumpLeft;

		vector <totalState> backwards_stillSide;
		vector <totalState> backwards_runRight;
		vector <totalState> backwards_runLeft;
		vector <totalState> backwards_jumpRight;
		vector <totalState> backwards_jumpLeft;

		vector <totalState> jumping_stillSide;
		vector <totalState> jumping_jumpRight;
		vector <totalState> jumping_jumpLeft;

		vector <totalState> jumpingforward_stillSide;
		vector <totalState> jumpingforward_jumpRight;
		vector <totalState> jumpingforward_jumpLeft;

		vector <totalState> jumpingBackwards_stillSide;
		vector <totalState> jumpingBackwards_jumpRight;
		vector <totalState> jumpingBackwards_jumpLeft;
	};
};
