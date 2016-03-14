#ifndef _PARTICLE_H
#define _PARTICLE_H

#include <fstream>
#include "collision.h"

class particle
{
public:
	// (posX, posY) - pozycja środka; (rad_e, rad_f) - długości półosi; angle - kąt obrotu
	particle(double posX, double posY, double rad_e, double rad_f, double angle);
	particle(double posX, double posY, double rad_e, double rad_f, double angle, double velX, double velY);
	~particle();

	void DrawParticle(std::fstream& file);
	void Move(double step);

	double X;
	double Y;
	double E;
	double F;
	double ANG;
	double V[2];

private:
	static const int N = 100;
};

bool CheckCollision(particle* iParticle, particle** vParticle, int I);

#endif