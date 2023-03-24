#pragma once
#include<iostream>
#include<string>
#include "GameObject.h"
#include<vector>
#include"Vector3.h"
#include<cmath>
#include<set>
#include <map>
#include <array>
#include"PhysicsObject.h"


using namespace std;
namespace NCL::CSC8503 {
	class playerState :public GameObject {
	public:

		playerState(GameObject* theEnemy, GameObject* thePlayer);

		~playerState();

		struct PositionData
		{
			Vector3 playerPosition;
			Vector3 relativePosition;
			float distanceFromEnemy;
			float distanceFromCentre;

		};

		struct ArrayCompare {
			bool operator()(const std::array<int, 2>& a, const std::array<int, 2>& b) const {
				if (a[0] < b[0]) {
					return true;
				}
				else if (a[0] == b[0] && a[1] < b[1]) {
					return true;
				}
				else {
					return false;
				}
			}
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


	
		vector <totalState>& getResponseValue(std::array<int,2> arrayKey) {
			return totalResponse[arrayKey];
		}

		/*vector <totalState>& getCurrentResponse() {
			return totalResponse[response];
		}*/

		void AddToTotalResponse(std::array<int, 2> arrayToVector) {//augment the vector you wish to add element too
			if (getResponseValue(arrayToVector).size() > 20) {
				getResponseValue(arrayToVector).pop_back();//checks if the vector is above size 20 if it is remove an element
			}
			getResponseValue(arrayToVector).push_back(response);
		}

		void updateResponse();

		void updatePlayerForwardState();

		void updatePlayerForwardStateJumping();

		void updatePlayerSideState();

		void updatePlayerSideStateJumping();

		void UpdateCurrentPosition();

		void setInitialPosition();

		void setIsPlayerJumping();

		bool getIsPlayerJumping() {
			return playerIsJumping;
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

		PositionData* GetTemporaryPosition() {
			return temporaryPosition;
		}

		void SetTemporaryPosition(PositionData newTemporaryPosition) {
			temporaryPosition = &newTemporaryPosition;
		}

		totalState* GetTemporaryState() {
			return temporaryState;
		}

		void SetTemporaryState() {
			temporaryState = currentState;
		}

		void updateState();

		totalState* GetCurrentState() {
			return currentState;
		}

		void UpdateCurrentState() {
			if (!playerForwardState || !playerSideState ) { return; }
			currentState->forwardResponse = playerForwardState;
			currentState->sidewardsResponse = playerSideState;
		}

		bool getUpdateFlag() {
			return updateFlag;
		}

		void toggleUpdateFlag() {
			updateFlag = !updateFlag;
		}

		PositionData getCurrentPosition() {
			return currentPosition;
		}

		//speed stuff

		float getPlayerSpeed() {
			return playerSpeed;
		}

		void setPlayerSpeed(float time) {
			Vector3 newPosition = playerTrack->GetTransform().GetPosition();
			float distance = (oldPosition - newPosition).Length();
			playerSpeed = distance / time;
			oldPosition = newPosition;
		}

		//speed stuff

	protected:

		float playerSpeed;
		bool updateFlag = true;
		bool playerIsJumping = false;
		currentStateForward playerForwardState;
		currentStateSideward playerSideState;
		totalState* currentState;
		GameObject* playerTrack;
		GameObject* enemy;
		PositionData currentPosition;// compare these 2 to set the new state 
		PositionData PreviousPosition;
		PositionData *temporaryPosition = nullptr;
		totalState* temporaryState = nullptr;
		//vector <totalState> totalResponse[24]= { still_stillSide,still_runRight,still_runLeft,still_jumpRight,still_jumpLeft,forwards_stillSide,forwards_runRight,forwards_runLeft,forwards_jumpRight,forwards_jumpLeft,backwards_stillSide,backwards_runRight,backwards_runLeft,backwards_jumpRight,backwards_jumpLeft,jumping_stillSide,jumping_jumpRight,jumping_jumpLeft,jumpingforward_stillSide,jumpingforward_jumpRight,jumpingforward_jumpLeft,jumpingBackwards_stillSide,jumpingBackwards_jumpRight,jumpingBackwards_jumpLeft };
		std::map< std::array<int,2>,std::vector<totalState>> totalResponse;
		totalState response{};
		Vector3 oldPosition{ 0,0,0 };

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
