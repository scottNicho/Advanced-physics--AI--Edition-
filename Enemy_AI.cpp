#include "Enemy_AI.h"

namespace NCL::CSC8503 {

	EnemyAI::EnemyAI(GameObject* thePlayerPassthrough) {
		playerTag = new playerState(this, thePlayerPassthrough);
		preResponseCapture();
		blankResponse = new playerState::totalState();
		blankResponse->forwardResponse = playerState::currentStateForward::non_forward;
		blankResponse->sidewardsResponse = playerState::currentStateSideward::non_side;
	}


	EnemyAI::~EnemyAI() {
		playerTag;
		blankResponse;
	}

	void EnemyAI::preResponseCapture() { //Set the initial position
		prePlayerPositionCapture = playerTag->GetTransform().GetPosition();
	}

	void EnemyAI::UpdateDistanceFromCentre() {
		distanceFromCentre = (Vector3{ 0,-20,0 } - this->GetTransform().GetPosition()).Length();
	}

	void EnemyAI::updatePlayerClose() {
		float distanceBetween = playerTag->getCurrentPosition().distanceFromEnemy;
		if (distanceBetween < 22) {
			setPlayerClose(true);
			steady(); 
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

	float EnemyAI::getAngleObjectTarget(GameObject* target) {
		Vector3 targetObjectPosition = target->GetTransform().GetPosition();
		Vector3 objectfacing = (this->GetTransform().GetOrientation() * Vector3 { 0, 0, -1 }).Normalised();
		Vector3 objectToTarget = ((targetObjectPosition)-(this->GetTransform().GetPosition())).Normalised();
		float dotProduct = Vector3::Dot(objectfacing, objectToTarget);
		float angleBetween = acos(dotProduct / ((objectfacing.Length()) * (objectToTarget.Length())));
		return angleBetween;
	}

	float EnemyAI::getAngleToPosition(Vector3 position) {
		Vector3 objectfacing = (this->GetTransform().GetOrientation() * Vector3 { 0, 0, -1 }).Normalised();
		Vector3 objectToTarget = ((position)-(this->GetTransform().GetPosition())).Normalised();
		float dotProduct = Vector3::Dot(objectfacing, objectToTarget);
		float angleBetween = acos(dotProduct / ((objectfacing.Length()) * (objectToTarget.Length())));
		return angleBetween;
	}

	bool EnemyAI::facePosition(Vector3 anchorPosition) {
		if (getAngleToPosition(anchorPosition) <= 0.2) {
			return true;
		}
		if (relativeLeftOrRightPosition(anchorPosition) == 1) {
			this->GetPhysicsObject()->AddTorque({ 0,1,0 });
		}
		else
		{
			this->GetPhysicsObject()->AddTorque({ 0,-1,0 });
		}
	}

	void EnemyAI::faceTarget() {
		GameObject* playerTarget = playerTag->getPlayerTrack();
		Vector3 playerTargetPosition = playerTarget->GetTransform().GetPosition();
		float angle = getAngleObjectTarget(playerTarget);
		if (angle <= 0.2) {
			this->GetPhysicsObject()->ClearForces();
			this->GetPhysicsObject()->SetAngularVelocity({ 0,0,0 });
			setFacingPlayer(true);
			return;
		}
		if (relativeLeftOrRight(playerTarget) == 1) {
			this->GetPhysicsObject()->AddTorque({ 0,1,0 });
		}
		else
		{
			this->GetPhysicsObject()->AddTorque({ 0,-1,0 });
		}

	}

	int EnemyAI::relativeLeftOrRight(GameObject* targetPlayer) {
		Vector3 EnemyPosition = this->GetTransform().GetPosition();
		Vector3 TargetPosition = targetPlayer->GetTransform().GetPosition();
		//Vector2 EnemyRelativePosition = {  EnemyPosition.x, EnemyPosition.z}; //consider the enemy position as the origin
		Vector3 EnemyRelativeDirectionPosition = ((this->GetTransform().GetOrientation()) * Vector3 { 0, 0, -1 });
		Vector2 EnemyRelativePosition = { EnemyRelativeDirectionPosition.x, EnemyRelativeDirectionPosition.z };
		float EnemyPositionGradient = EnemyRelativePosition.y / EnemyRelativePosition.x; // gradient of the cartiesian line going through the normalised direction vector and the origin
		Vector2 TargetRelativePosition = { TargetPosition.x - EnemyPosition.x,TargetPosition.z - EnemyPosition.z }; // the target augmented in to space relative to the enemy
		float interceptXPoint = (TargetRelativePosition.y * EnemyRelativePosition.x) / EnemyRelativePosition.y; // solve for the x value when a horisontal line from target intercepts
		Vector2 iterceptionPoint = { interceptXPoint,TargetRelativePosition.y };
		Vector2 targetToInterceptPoint = iterceptionPoint - TargetRelativePosition; // get the vector from target to intercep point to get the direction

		if (targetToInterceptPoint.x > 0 && (EnemyRelativePosition.y >= 0)) {
			return -1;
		}
		if (targetToInterceptPoint.x > 0 && (EnemyRelativePosition.y < 0)) {
			return 1;
		}
		if (targetToInterceptPoint.x < 0 && (EnemyRelativePosition.y > 0)) {
			return 1;
		}

		if (targetToInterceptPoint.x < 0 && (EnemyRelativePosition.y < 0)) {
			return -1;
		}
		/*Vector3 EnemyDirectionVector = ((this->GetTransform().GetOrientation())*Vector3{0,0,-1}).Normalised();
		return EnemyDirectionVector;*/
		return 1;
	}

	int EnemyAI::relativeLeftOrRightPosition(Vector3 targetPosition) {
		Vector3 EnemyPosition = this->GetTransform().GetPosition();
		//Vector2 EnemyRelativePosition = {  EnemyPosition.x, EnemyPosition.z}; //consider the enemy position as the origin
		Vector3 EnemyRelativeDirectionPosition = ((this->GetTransform().GetOrientation()) * Vector3 { 0, 0, -1 });
		Vector2 EnemyRelativePosition = { EnemyRelativeDirectionPosition.x, EnemyRelativeDirectionPosition.z };
		float EnemyPositionGradient = EnemyRelativePosition.y / EnemyRelativePosition.x; // gradient of the cartiesian line going through the normalised direction vector and the origin
		Vector2 TargetRelativePosition = { targetPosition.x - EnemyPosition.x,targetPosition.z - EnemyPosition.z }; // the target augmented in to space relative to the enemy
		float interceptXPoint = (TargetRelativePosition.y * EnemyRelativePosition.x) / EnemyRelativePosition.y; // solve for the x value when a horisontal line from target intercepts
		Vector2 iterceptionPoint = { interceptXPoint,TargetRelativePosition.y };
		Vector2 targetToInterceptPoint = iterceptionPoint - TargetRelativePosition; // get the vector from target to intercep point to get the direction

		if (targetToInterceptPoint.x > 0 && (EnemyRelativePosition.y >= 0)) {
			return -1;
		}
		if (targetToInterceptPoint.x > 0 && (EnemyRelativePosition.y < 0)) {
			return 1;
		}
		if (targetToInterceptPoint.x < 0 && (EnemyRelativePosition.y > 0)) {
			return 1;
		}

		if (targetToInterceptPoint.x < 0 && (EnemyRelativePosition.y < 0)) {
			return -1;
		}
		/*Vector3 EnemyDirectionVector = ((this->GetTransform().GetOrientation())*Vector3{0,0,-1}).Normalised();
		return EnemyDirectionVector;*/
		return 1;
	}

	void EnemyAI::EnemyCloseOnPlayer() {
		Vector3 enemyDirectionVector = this->GetTransform().GetOrientation() * Vector3 { 0, 0, -1 };
		float distance = playerTag->getCurrentPosition().distanceFromEnemy;
		this->GetPhysicsObject()->AddForce(enemyDirectionVector * 10 * (distance / 3));
	}

	void EnemyAI::enemyMoveForward() {
		Vector3 enemyDirectionVector = this->GetTransform().GetOrientation() * Vector3 { 0, 0, -1 };
		this->GetPhysicsObject()->AddForce(enemyDirectionVector * 2000.0f);
	}

	void EnemyAI::enemyMoveBackwards(GameObject* enemyAI) {
		Vector3 enemyDirectionVector = enemyAI->GetTransform().GetOrientation() * Vector3 { 0, 0, -1 };
		enemyAI->GetPhysicsObject()->AddForce(enemyDirectionVector * -10.0f);
	}

	void EnemyAI::enemyMoveRight() {
		Vector3 enemyDirectionVector = this->GetTransform().GetOrientation() * Vector3 { 1, 0, 0 };
		this->GetPhysicsObject()->AddForce(enemyDirectionVector * 100.0f);
	}

	void EnemyAI::enemyMoveLeft() {
		Vector3 enemyDirectionVector = this->GetTransform().GetOrientation() * Vector3 { -1, 0, 0 };
		this->GetPhysicsObject()->AddForce(enemyDirectionVector * 100.0f);
	}

	void EnemyAI::enemyJump() {
		Vector3 enemyDirectionVector = this->GetTransform().GetOrientation() * Vector3 { 0, 1, 0 };
		this->GetPhysicsObject()->AddForce(enemyDirectionVector * 1100.0f);
	}

	void EnemyAI::enemyCharge() {
		if (!(this->GetEnemyDash())) {
			return;
		}
		Vector3 enemyDirectionVector = this->GetTransform().GetOrientation() * Vector3 { 0, 0, -1 };
		this->GetPhysicsObject()->AddForce(enemyDirectionVector * 1000.0f);
		this->GetPhysicsObject()->AddForce(dashPredictionVector );
		this->toggleEnemyDash();
	}

	void EnemyAI::enemyFaint() {
		std::cout << "enemyfaint " << std::endl;
		if (enemyCanFaint) {
			this->SetResponseCapture(true);
			this->preResponseCapture();
			faintDirectionVector = this->GetTransform().GetOrientation() * Vector3 { 0, 0, -1 };
			this->GetPhysicsObject()->AddForce(faintDirectionVector * 5000.0f);
			faintStartPosition = this->GetTransform().GetPosition();
			toggleEnemyCanFaint();
			SetFaintMoveBackSwitch(true);
			return;
		}
		if ((((this->GetTransform().GetPosition()) - (faintStartPosition)).Length() > 10) && getFaintMoveBackSwitch()) {
			this->GetPhysicsObject()->AddForce(faintDirectionVector * -4800.0f);
			//toggleEnemyCanFaint();
			SetFaintMoveBackSwitch(false);
			return;
		}
	}

	void EnemyAI::enemySideDodge() {
		int ranX = rand() % 10;
		if (ranX >= 5) {
			enemyMoveRight();
			return;
		}
		enemyMoveLeft();
	}

	void EnemyAI::moveToCentre() {
		int ranY = rand() % 20;
		Vector3 directionToCentre = (centreSafteyCircle - this->GetTransform().GetPosition()).Normalised();
		float distanceToCentre = directionToCentre.Length();
		this->GetPhysicsObject()->AddForce(directionToCentre * (10 * distanceToCentre));
		if (ranY >= 15) {
			enemyJump();
		}
	}

	void EnemyAI::moveToTarget() {
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
			faceTarget();
			EnemyCloseOnPlayer();
			return;
		}
		else
		{
			//steady();
			faceTarget();
		}
	}

	void EnemyAI::steady() {
		Vector3 reducedVelocity = (this->GetPhysicsObject()->GetLinearVelocity()) * 0.7;
		this->GetPhysicsObject()->SetLinearVelocity(reducedVelocity);
	}

	void EnemyAI::UpdateEnemyLive() {
		updatePlayerClose();
		moveToTarget();
	}

	void EnemyAI::getPlayState(float time) {
		playerTag->setPlayerSpeed(time);
		playerTag->UpdateCurrentPosition();
		playerTag->updateState();
		/*cout << "player Forward state " << playerTag->getPlayerForwardState() << endl;
		cout << "player side state " << playerTag->getPlayerSideState() << endl;*/
	}

	float EnemyAI::percentageFind(vector<playerState::totalState> gottenState, int indexNum) {
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

	playerState::totalState EnemyAI::mostLikelyResponseFinder() {
		vector<playerState::totalState> currentStateData = playerTag->getResponseValue({ (playerTag->GetCurrentState()->forwardResponse),(playerTag->GetCurrentState()->sidewardsResponse) });
		playerState::totalState mostLikelyresponse;
		//std::cout << "total response size " << playerTag->GetTotalResponse().size() << std::endl;
		float highestPercentage = 0.0f;
		if (currentStateData.size() == 0 || currentStateData.size()< 5) {
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

	void EnemyAI::SetDashPredictionVector(playerState::totalState responsePrimer) {

		switch (responsePrimer.forwardResponse) {
		case 0:
			ReadyToChargeStill(responsePrimer.sidewardsResponse);
			break;
		case 1:
			ReadyToChargeForward(responsePrimer.sidewardsResponse);
			break;
		case 2:
			ReadyToChargeBackwards(responsePrimer.sidewardsResponse);
			break;
		case 3:
			ReadyToChargeJumpingInPlace(responsePrimer.sidewardsResponse);
			break;
		case 4:
			ReadyToChargeJumpingForward(responsePrimer.sidewardsResponse);
			break;
		case 5:
			ReadyToChargeJumpingBackwards(responsePrimer.sidewardsResponse);
			break;
		}
	}

	void EnemyAI::ReadyToChargeStill(int sideValue) {
		switch (sideValue) {
		case 0:
			dashPredictionVector.x = 0;
			dashPredictionVector.y = 0;
			dashPredictionVector.z = 0;
			dashPredictionVector *= (playerTag->getPlayerSpeed());
			break;

		case 1:
			dashPredictionVector.x = -2;
			dashPredictionVector.y = 0;
			dashPredictionVector.z = 0;
			dashPredictionVector *= (playerTag->getPlayerSpeed());
			break;
		case 2:
			dashPredictionVector.x = -2;
			dashPredictionVector.y = 0;
			dashPredictionVector.z = 0;
			dashPredictionVector *= (playerTag->getPlayerSpeed());
			break;
		case 3:
			dashPredictionVector.x = -2;
			dashPredictionVector.y = 10;
			dashPredictionVector.z = 0;
			dashPredictionVector *= (playerTag->getPlayerSpeed());
			break;
		case 4:
			dashPredictionVector.x = 2;
			dashPredictionVector.y = 10;
			dashPredictionVector.z = 0;
			dashPredictionVector *= (playerTag->getPlayerSpeed());
			break;
		}


	}

	void EnemyAI::ReadyToChargeForward(int sideValue) {
		switch (sideValue) {
		case 0:
			dashPredictionVector.x = 0;
			dashPredictionVector.y = 0;
			dashPredictionVector.z = 0;
			dashPredictionVector *= (playerTag->getPlayerSpeed());
			break;
		case 1:
			dashPredictionVector.x = -1;
			dashPredictionVector.y = 0;
			dashPredictionVector.z = 0;
			dashPredictionVector *= (playerTag->getPlayerSpeed());
			break;
		case 2:
			dashPredictionVector.x = 1;
			dashPredictionVector.y = 0;
			dashPredictionVector.z = 0;
			dashPredictionVector *= (playerTag->getPlayerSpeed());
			break;
		case 3:
			dashPredictionVector.x = -1;
			dashPredictionVector.y = 10;
			dashPredictionVector.z = 0;
			dashPredictionVector *= (playerTag->getPlayerSpeed());
			break;
		case 4:
			dashPredictionVector.x = 1;
			dashPredictionVector.y = 10;
			dashPredictionVector.z = 0;
			dashPredictionVector *= (playerTag->getPlayerSpeed());
			break;
		}
	}

	void EnemyAI::ReadyToChargeBackwards(int sideValue) {
		switch (sideValue) {
		case 0:
			dashPredictionVector.x = 0;
			dashPredictionVector.y = 0;
			dashPredictionVector.z = 0;
			dashPredictionVector *= (playerTag->getPlayerSpeed());
			break;
		case 1:
			dashPredictionVector.x = -3;
			dashPredictionVector.y = 0;
			dashPredictionVector.z = 0;
			dashPredictionVector *= (playerTag->getPlayerSpeed());
			break;
		case 2:
			dashPredictionVector.x = 3;
			dashPredictionVector.y = 0;
			dashPredictionVector.z = 0;
			dashPredictionVector *= (playerTag->getPlayerSpeed());
			break;
		case 3:
			dashPredictionVector.x = -3;
			dashPredictionVector.y = 5;
			dashPredictionVector.z = 0;
			dashPredictionVector *= (playerTag->getPlayerSpeed());
			break;
		case 4:
			dashPredictionVector.x = 3;
			dashPredictionVector.y = 5;
			dashPredictionVector.z = 0;
			dashPredictionVector *= (playerTag->getPlayerSpeed());
			break;
		}
	}

	void EnemyAI::ReadyToChargeJumpingInPlace(int sideValue) {
		switch (sideValue) {
		case 0:
			dashPredictionVector.x = 0;
			dashPredictionVector.y = 0;
			dashPredictionVector.z = 0;
			dashPredictionVector *= (playerTag->getPlayerSpeed());
			break;
		case 1:
			dashPredictionVector.x = -3;
			dashPredictionVector.y = 0;
			dashPredictionVector.z = 0;
			dashPredictionVector *= (playerTag->getPlayerSpeed());
			break;
		case 2:
			dashPredictionVector.x = 3;
			dashPredictionVector.y = 0;
			dashPredictionVector.z = 0;
			dashPredictionVector *= (playerTag->getPlayerSpeed());
			break;
		}
	}

	void EnemyAI::ReadyToChargeJumpingForward(int sideValue) {
		switch (sideValue) {
		case 0:
			dashPredictionVector.x = 0;
			dashPredictionVector.y = 5;
			dashPredictionVector.z = 0;
			dashPredictionVector *= (playerTag->getPlayerSpeed());
			break;
		case 1:
			dashPredictionVector.x = -1;
			dashPredictionVector.y = 5;
			dashPredictionVector.z = 0;
			dashPredictionVector *= (playerTag->getPlayerSpeed());
			break;
		case 2:
			dashPredictionVector.x = 1;
			dashPredictionVector.y = 5;
			dashPredictionVector.z = 0;
			dashPredictionVector *= (playerTag->getPlayerSpeed());
			break;
		}
	}

	void EnemyAI::ReadyToChargeJumpingBackwards(int sideValue) {
		switch (sideValue) {
		case 0:
			dashPredictionVector.x = 0;
			dashPredictionVector.y = 0;
			dashPredictionVector.z = 0;
			dashPredictionVector *= (playerTag->getPlayerSpeed());
			break;
		case 1:
			dashPredictionVector.x = -3;
			dashPredictionVector.y = 0;
			dashPredictionVector.z = 0;
			dashPredictionVector *= (playerTag->getPlayerSpeed());
			break;
		case 2:
			dashPredictionVector.x = 3;
			dashPredictionVector.y = 0;
			dashPredictionVector.z = 0;
			dashPredictionVector *= (playerTag->getPlayerSpeed());
			break;
		}
	}
}
