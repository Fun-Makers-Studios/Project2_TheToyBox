#pragma once
#include "Point.h"
#include "Color.h"


class Particle {
	// methods
public:
	Particle();
	~Particle();

	void Initialize(fPoint initialPosition, fPoint initiaVelocity, fPoint acceleration, Color initialColor, Color objectiveColor, float initialScale, float objectiveScale, float lifespan);

	void Update(float dt);

	void ResetParticle();

	bool IsBeingUsed();

	// get the next free particle in the pool
	Particle* GetNext() const {
		return next;
	}

	void SetNext(Particle* next) {
		this->next = next;
	}

	fPoint GetPosition() const {
		return position;
	}

	Color GetColor() const {
		return color;
	}

	float GetScale() const {
		return scale;
	}

protected:
private:

	// variables
public:
	

protected:
private:

	float timeLeft;

	fPoint position;
	fPoint velocity;
	fPoint acceleration;

	Color initialColor;
	Color color;
	Color objectiveColor;

	float initialScale;
	float scale;
	float objectiveScale;

	float lifespan;

	Particle* next;
};