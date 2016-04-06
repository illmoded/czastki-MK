#ifndef _PARTICLE_H
#define _PARTICLE_H

#include <vector>
#include <stdio.h>
#include <math.h>

class particle
{
public:
	// (posX, posY) - pozycja środka; rad - promień; (velX, velY) - współrzędne prędkości
	particle(double posX, double posY, double rad);
	particle(double posX, double posY, double rad, double velX, double velY);
	~particle();

	void Flush();
	void Move(double step);
	void ScalePosition(double s);
	void Translate(double xVec, double yVec);
	double GetMass();

	double X;
	double Y;
	double R;
	double V[2];

private:
	static const double M = 1.0;
};

bool CheckCollision(particle* iParticle, particle** vParticle, int I);
std::vector<std::pair<int, int> > Collision(particle** vParticle, int nParticle);
void EvaluateVelocities(particle* first, particle* second);

#endif