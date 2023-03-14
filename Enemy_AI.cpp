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
	}

	void EnemyAI::preResponseCapture() { //Set the initial position
		prePlayerPositionCapture = playerTag->GetTransform().GetPosition();
	}

	void EnemyAI::UpdateDistanceFromCentre() {
		distanceFromCentre = (Vector3{ 0,-20,0 } - this->GetTransform().GetPosition()).Length();
	}

	void EnemyAI::updatePlayerClose() {
		float distanceBetween = playerTag->getCurrentPosition().distanceFromEnemy;
		if (distanceBetween < 12) {
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
}
