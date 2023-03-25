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

		bool GetEnemyDash() {
			return enemyDash;
		}

		void toggleEnemyDash() {
			enemyDash = !enemyDash;
		}


		void EnemyCloseOnPlayer();

		void enemyMoveForward();

		void enemyMoveBackwards(GameObject* enemyAI);

		void enemyMoveRight();

		void enemyMoveLeft();

		void enemyJump();

		void enemyCharge();

		bool GetEnemyCanFaint() {
			return enemyCanFaint;
		}

		void toggleEnemyCanFaint() {
			enemyCanFaint = !enemyCanFaint;
		}

		void enemyFaint();


		void enemySideDodge();

		void moveToCentre();


		void moveToTarget();

		void steady();

		//Enemy Ai movement 
		//Enemy AI control
		void updateEnemyAction(float time) {
			getPlayState(time);
			if (playerClose && playerTag->GetCurrentState()) {
				playerState::totalState responseState = mostLikelyResponseFinder();
				if (responseState.forwardResponse != playerState::currentStateForward::non_forward) {
					SetDashPredictionVector(responseState);
					enemyCharge();
				}
				else if (playerClose) {
					//enemyCharge();
					enemyFaint();
				}
		
			}
		    else if (playerClose) {
				//enemyCharge();
				enemyFaint();
			}
		}
		//Enemy AI control
		void UpdateEnemyLive();


		//Player Info
		void getPlayState(float time);
		//Player info
		 
		
		//State anlysis

		float percentageFind(vector<playerState::totalState> gottenState, int indexNum);

		playerState::totalState mostLikelyResponseFinder();

		void SetDashPredictionVector(playerState::totalState responsePrimer);

		void ReadyToChargeStill(int sideValue);

		void ReadyToChargeForward(int sideValue);
		
		void ReadyToChargeBackwards(int sideValue);

		void ReadyToChargeJumpingInPlace(int sideValue);

		void ReadyToChargeJumpingForward(int sideValue);

		void ReadyToChargeJumpingBackwards(int sideValue);


		//state analysis 
		//state response 
		void SetResponseCapture(bool capture) {
			responseCapture = capture;
		}

		bool getResponseCapture() {
			return responseCapture;
		}

		/*void setDashPredictionVector(Vector3 newDashVector) {
			dashPredictionVector = newDashVector;
		}*/
		//state response
		//EnemyAI master control

		//EnemyAI master control
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
