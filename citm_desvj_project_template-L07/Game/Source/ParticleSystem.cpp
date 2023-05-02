#include "ParticleSystem.h"

#include "App.h"

#include "Render.h"
#include "Scale.h"

ParticleSystem::ParticleSystem()
{
}

ParticleSystem::~ParticleSystem()
{
}

void ParticleSystem::SetParticleSystem(dPoint position, dPoint velocity, dPoint acceleration,
	char initialR, char initialG, char initialB, char initialAlpha,
	float spawnRate, float lifespan, bool isConstant)
{
	/*this->position = position;
	this->velocity = velocity;
	this->acceleration = acceleration;
	this->initialR = initialR;
	this->initialG = initialG;
	this->initialB = initialB;
	this->initialAlpha = initialAlpha;*/
	this->spawnRate = spawnRate;
	this->PSLifespan = lifespan;
	this->isConstant = isConstant;

	age = 0;
}

void ParticleSystem::SetObjective(int x, int y)
{
	PSLifespan = PSLifespan - age;
	age = 0;

	initialPosition.x = position.x;
	initialPosition.y = position.y;

	objectivePosition.x = x;
	objectivePosition.y = y;
}

void ParticleSystem::Start()
{

}

bool ParticleSystem::Update(float dt)
{
	timeFromLastSpawn += dt;

	age += dt;

	for (ListItem<Particle*>* item = particles.start; item != nullptr; item = item->next) {
		if (item->data != nullptr) {
			if (item->data->IsBeingUsed()) {
				item->data->Update(dt);
			}
			else {
				if (spawnRate <= timeFromLastSpawn && (age < PSLifespan || isConstant)) {
					SpawnParticle(item->data);
					timeFromLastSpawn -= spawnRate;
				}
			}
		}
	}

	return (age < (PSLifespan + particleLifespan) || isConstant);
}

void ParticleSystem::PostUpdate()
{
	ScaleType scaleType = app->scaleObj->GetCurrentScale();

	for (ListItem<Particle*>* item = particles.start; item != nullptr; item = item->next) {
		if (item->data != nullptr) {
			if (item->data->IsBeingUsed()) {
				app->render->DrawParticleAlpha(texture, item->data->GetPosition().x, item->data->GetPosition().y,
					item->data->GetColor().r, item->data->GetColor().g, item->data->GetColor().b, item->data->GetColor().a, item->data->GetScale(), scaleType);
			}
		}
	}
}

void ParticleSystem::CleanParticles()
{
	particles.Clear();
}

void ParticleSystem::AssignParticle(Particle* particle)
{
	particle->ResetParticle();
	particles.Add(particle);
}

void ParticleSystem::SpawnParticle(Particle* p)
{
	float t = age / PSLifespan;

	// lerp
	position.x = initialPosition.x + (double)(t * (float)(objectivePosition.x - initialPosition.x));
	position.y = initialPosition.y + (double)(t * (float)(objectivePosition.y - initialPosition.y));

	if (randomSpawnPositionRangeMin.x - randomSpawnPositionRangeMax.x != 0) {
		if (randomSpawnPositionRangeMax.x > randomSpawnPositionRangeMin.x) {
			position.x += (rand() % abs((int)(randomSpawnPositionRangeMax.x - randomSpawnPositionRangeMin.x)) + randomSpawnPositionRangeMin.x);
		}
		else {
			position.x += (rand() % abs((int)(randomSpawnPositionRangeMin.x - randomSpawnPositionRangeMax.x)) + randomSpawnPositionRangeMax.x);
		}
	}

	if (randomSpawnPositionRangeMin.y - randomSpawnPositionRangeMax.y != 0) {
		if (randomSpawnPositionRangeMax.y > randomSpawnPositionRangeMin.y) {
			position.y += (rand() % abs((int)(randomSpawnPositionRangeMax.y - randomSpawnPositionRangeMin.y)) + randomSpawnPositionRangeMin.y);
		}
		else {
			position.y += (rand() % abs((int)(randomSpawnPositionRangeMin.y - randomSpawnPositionRangeMax.y)) + randomSpawnPositionRangeMax.y);
		}
	}

	fPoint velocity = shootingVelocity;

	if (randomShootingVelocityRangeMin.x - randomShootingVelocityRangeMax.x != 0) {
		if (randomShootingVelocityRangeMax.x > randomShootingVelocityRangeMin.x) {
			velocity.x += (rand() % abs((int)(randomShootingVelocityRangeMax.x - randomShootingVelocityRangeMin.x)) + randomShootingVelocityRangeMin.x);
		}
		else {
			velocity.x += (rand() % abs((int)(randomShootingVelocityRangeMin.x - randomShootingVelocityRangeMax.x)) + randomShootingVelocityRangeMax.x);
		}
	}

	if (randomShootingVelocityRangeMin.y - randomShootingVelocityRangeMax.y != 0) {
		if (randomShootingVelocityRangeMax.y > randomShootingVelocityRangeMin.y) {
			velocity.y += (rand() % abs((int)(randomShootingVelocityRangeMax.y - randomShootingVelocityRangeMin.y)) + randomShootingVelocityRangeMin.y);
		}
		else {
			velocity.y += (rand() % abs((int)(randomShootingVelocityRangeMin.y - randomShootingVelocityRangeMax.y)) + randomShootingVelocityRangeMax.y);
		}
	}

	fPoint spawnPosition;
	spawnPosition.x = position.x;
	spawnPosition.y = position.y;

	p->Initialize(spawnPosition, velocity, shootingAcceleration, initialColor, objectiveColor, initialScale, objectiveScale, particleLifespan);
}
