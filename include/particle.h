#ifndef _PARTICLE_H
#define _PARTICLE_H

#include <fstream>
#include <vector>
#include "collision.h"

class particle
{
public:
	// (posX, posY) - pozycja środka; (rad_e, rad_f) - długości półosi / rad - promień; angle - kąt obrotu
	particle(double posX, double posY, double rad);
	particle(double posX, double posY, double rad, double velX, double velY);
	// particle(double posX, double posY, double rad_e, double rad_f, double angle);
	// particle(double posX, double posY, double rad_e, double rad_f, double angle, double velX, double velY);
	~particle();

	void DrawParticle(std::fstream& file);
	std::vector<std::pair<double, double> > DrawParticle();
	void Move(double step);
	void ScaleAll(double s);
	void Translate(double xVec, double yVec);

	double X;
	double Y;
	double E;
	double F;
	double ANG;
	double V[2];

private:
	static const int N = 100;
	static const double M = 1.0;
};

bool CheckCollision(particle* iParticle, particle** vParticle, int I);

#endif