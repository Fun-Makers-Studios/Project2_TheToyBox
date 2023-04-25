#include "Particle.h"

Particle::Particle()
{
	
}

Particle::~Particle()
{

}

void Particle::ResetParticle()
{
	timeLeft = -1;
}

void Particle::Initialize(fPoint initialPosition, fPoint initiaVelocity, fPoint acceleration, Color initialColor, Color objectiveColor, float initialScale, float objectiveScale, float lifespan)
{
	this->position = initialPosition;
	this->velocity = initiaVelocity;
	this->acceleration = acceleration;

	this->initialColor = initialColor;
	this->objectiveColor = objectiveColor;

	this->initialScale = initialScale;
	this->objectiveScale = objectiveScale;

	this->lifespan = lifespan;

	timeLeft = lifespan;
}

void Particle::Update(float dt)
{
	timeLeft -= dt;

	float age = lifespan - timeLeft;

	float t = age / lifespan;

	velocity.x += acceleration.x * dt;
	velocity.y += acceleration.y * dt;
	position.x += velocity.x * dt;
	position.y += velocity.y * dt;

	color.r = initialColor.r + (int)(t * (objectiveColor.r - initialColor.r));
	color.g = initialColor.g + (int)(t * (objectiveColor.g - initialColor.g));
	color.b = initialColor.b + (int)(t * (objectiveColor.b - initialColor.b));
	color.a = initialColor.a + (int)(t * (objectiveColor.a - initialColor.a));

	scale = initialScale + (t * (objectiveScale - initialScale));

}

bool Particle::IsBeingUsed()
{
	return (timeLeft > 0);
}
