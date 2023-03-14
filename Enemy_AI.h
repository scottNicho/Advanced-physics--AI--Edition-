#pragma once
#include<iostream>
#include<string>
#include "GameObject.h"
#include<vector>
#include"Vector3.h"
#include"Vector2.h"
#include<cmath>
#include<set>
#include"PhysicsObject.h"
#include <chrono>
#include <thread>
#include<windows.h>
#include <cstdlib>
#include"playerState.h"

using namespace std;

namespace NCL::CSC8503 {

	class EnemyAI :public GameObject {
	public:


		EnemyAI(GameObject* thePlayerPassthrough);


		~EnemyAI();

		void preResponseCapture(); // sets the initial player position

		playerState* getPlayerTag() {
			return playerTag;
		}

		//void postResponse

		

		void UpdateDistanceFromCentre(); //updates the enemy's distance from the centre 

		void setPlayerClose(bool isClose) {
			playerClose = isClose;
		}

		void updatePlayerClose(); // sets the player to be close if within a certain range 

		void setAggressionLevel(float newAgressionLevel) {
			agressionLevel = newAgressionLevel;
		}

		bool getFacingPlayer() {
			return facingPlayer;
		}

		void toggleFacingPlayer() {
			facingPlayer = !facingPlayer;
		}

		void setFacingPlayer(bool facing) {
			facingPlayer = facing;
		}

		bool getInsideAgressionRadius() {
			return insideAgressionRadius;
		}

		bool getFaintMoveBackSwitch() {
			return faintMoveBackSwitch;
		}

		void SetFaintMoveBackSwitch(bool onOff) {
			faintMoveBackSwitch = onOff;
		}

		void updateInAgressionRadius(); // sets the insideAgressionRadius bool to be true if within a certain range

		float getAngleObjectTarget(GameObject* target); //Returns the angle between the EnemyAI and the targeted gameObject

		float getAngleToPosition(Vector3 position);//Returns the angle between the enemy AI and A given position

		bool facePosition(Vector3 anchorPosition); //The EnemyAI will turn to face the given position in the fastest way 

		void faceTarget();//The EnemyAI will turn to face the given gameObject in the fastest way 

		int relativeLeftOrRight(GameObject* targetPlayer);//Returns 2 possible int values telling if it is faster to turn right or left to face the given gameObject


		int relativeLeftOrRightPosition(Vector3 targetPosition);//Returns 2 possible int values telling if it is faster to turn right or left to face the given position

		//Enemy Ai movement

		bool getEnemyDash() {
			return enemyDash;
		}

		void toggleEnemyDash() {
			enemyDash = !enemyDash;
		}


		void enemyMoveForward() {
			Vector3 enemyDirectionVector = this->GetTransform().GetOrientation() * Vector3 { 0, 0, -1 };
			this->GetPhysicsObject()->AddForce(enemyDirectionVector * 2000.0f);
		}

		void enemyMoveBackwards(GameObject* enemyAI) {
			Vector3 enemyDirectionVector = enemyAI->GetTransform().GetOrientation() * Vector3 { 0, 0, -1 };
			enemyAI->GetPhysicsObject()->AddForce(enemyDirectionVector * -10.0f);
		}

		void enemyMoveRight() {
			Vector3 enemyDirectionVector = this->GetTransform().GetOrientation() * Vector3 { 1, 0, 0 };
			this->GetPhysicsObject()->AddForce(enemyDirectionVector * 100.0f);
		}

		void enemyMoveLeft() {
			Vector3 enemyDirectionVector = this->GetTransform().GetOrientation() * Vector3 { -1, 0, 0 };
			this->GetPhysicsObject()->AddForce(enemyDirectionVector * 100.0f);
		}

		void enemyJump() {
			Vector3 enemyDirectionVector = this->GetTransform().GetOrientation() * Vector3 { 0, 1, 0 };
			this->GetPhysicsObject()->AddForce(enemyDirectionVector * 1100.0f);
		}

		void enemyCharge() {
			if (!(this->getEnemyDash())) {
				return;
			}
			Vector3 enemyDirectionVector = this->GetTransform().GetOrientation() * Vector3 { 0, 0, -1 };
			this->GetPhysicsObject()->AddForce(enemyDirectionVector * 5000.0f);
			this->GetPhysicsObject()->AddForce(dashPredictionVector * 1000.0f);
			this->toggleEnemyDash();
		}

		void toggleEnemyCanFaint() {
			enemyCanFaint = !enemyCanFaint;
		}

		void enemyFaint() {
			if (enemyCanFaint) {
				this->toggleResponseCapture();
				this->preResponseCapture();
				faintDirectionVector = this->GetTransform().GetOrientation() * Vector3 { 0, 0, -1 };
				this->GetPhysicsObject()->AddForce(faintDirectionVector * 5000.0f);
				faintStartPosition = this->GetTransform().GetPosition();
				toggleEnemyCanFaint();
				SetFaintMoveBackSwitch(true);
				return;
			}
			if ((((this->GetTransform().GetPosition()) - (faintStartPosition)).Length() > 3)&& getFaintMoveBackSwitch()) {
				this->GetPhysicsObject()->AddForce(faintDirectionVector * -4900.0f);
				//toggleEnemyCanFaint();
				SetFaintMoveBackSwitch(false);
				return;
			}
		}


		void enemySideDodge() {
			int ranX = rand() % 10;
			if (ranX >= 5) {
				enemyMoveRight();
				return;
			}
			enemyMoveLeft();
		}

		void moveToCentre() {
			int ranY = rand()%20;
			Vector3 directionToCentre = (centreSafteyCircle - this->GetTransform().GetPosition()).Normalised();
			float distanceToCentre = directionToCentre.Length();
			this->GetPhysicsObject()->AddForce(directionToCentre * (10 * distanceToCentre));
			if (ranY >= 15) {
				enemyJump();
			}
		}

		void updateEnemyAction() {
			updatePlayerClose();
			getPlayState();
			//moveToTarget();
			if (playerClose) {
				//enemyCharge();
				enemyFaint();
			}
		}

		void moveToTarget() {
			GameObject* playerCharacter = playerTag->getPlayerTrack();
			Vector3 playerTargetPosition = playerCharacter->GetTransform().GetPosition();
			float angle = getAngleObjectTarget(playerCharacter);
			if (angle <= 0.2) {
				setFacingPlayer(true);
			}
			else
			{
				setFacingPlayer(false);
			}

			if (getFacingPlayer()) {
				enemyMoveForward();
				return;
			}
			else
			{
				faceTarget();
			}
		}
		//Enemy Ai movement 


		void getPlayState(){
			playerTag->UpdateCurrentPosition();
			playerTag->updateState();
			cout <<"player Forward state " << playerTag->getPlayerForwardState() << endl;
			cout << "player side state " << playerTag->getPlayerSideState() << endl;
		}

		//State anlysis

		float percentageFind(vector<playerState::totalState> gottenState, int indexNum) {
			if (indexNum < 0 || indexNum >= gottenState.size()) {
				return 0.0f;
			}

			int count = 0;
			for (int i = 0; i < gottenState.size(); i++) {
				if (gottenState[indexNum].forwardResponse == gottenState[i].forwardResponse && gottenState[indexNum].sidewardsResponse == gottenState[i].sidewardsResponse) {
					count += 1;
				}
			}

			return static_cast<float>(count) / gottenState.size();
		}

		playerState::totalState mostLikelyResponseFinder(vector<playerState::totalState> currentStateData) {
			playerState::totalState mostLikelyresponse;
			float highestPercentage = 0.0f;
			if (currentStateData.size() == 0) {
				return *blankResponse; 
			}
			for (int i = 0; i < currentStateData.size(); i++) {
				float currentPercentage = percentageFind(currentStateData, i);
				if (currentPercentage > highestPercentage) {
					highestPercentage = currentPercentage;
					mostLikelyresponse = currentStateData[i];
				}
			}
			if (highestPercentage < 0.35) {
				return *blankResponse;
			}
			else
			{
				return mostLikelyresponse;
			}
		}

		//state analysis 
		//state response 
		void toggleResponseCapture() {
			responseCapture = !responseCapture;
		}

		bool getResponseCapture() {
			return responseCapture;
		}

		void setDashPredictionVector(Vector3 newDashVector) {
			dashPredictionVector = newDashVector;
		}
		//state response

	protected:
		const  Vector3 centreSafteyCircle = Vector3{0,-20,0};
		Vector3 faintStartPosition = {};
		Vector3 faintDirectionVector = {};
		Vector3 prePlayerPositionCapture = {};
		Vector3 dashPredictionVector = {};

		float distanceFromCentre = 0;
		float agressionRadius = 10;
		float agressionLevel = 0.5;
		const float innerCircleRadius = 8;
		const float outerCircleRadius = 15;

		bool responseCapture = false;
		bool enemyCanFaint = true;
		bool playerClose = false;
		bool facingPlayer = false;
		bool enemyDash = true;
		bool insideAgressionRadius = false;
		bool faintMoveBackSwitch = true; // in the Ai faint this switch stops the counter force being added repeatedly

		playerState::totalState *blankResponse; 
		playerState *playerTag;
	};

}
