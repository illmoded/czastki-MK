#include "particle.h"

particle::particle(double posX, double posY, double rad_e, double rad_f, double angle)
{
	X = posX;
	Y = posY;
	E = rad_e;
	F = rad_f;
	ANG = angle;
}

particle::particle(double posX, double posY, double rad_e, double rad_f, double angle, double velX, double velY)
{
	X = posX;
	Y = posY;
	E = rad_e;
	F = rad_f;
	ANG = angle;
	V[1] = velX;
	V[2] = velY;
}

void particle::DrawParticle(std::fstream& file)
{
	for (int i = 0; i < N; ++i) {
		double alpha = static_cast<double>(i)/static_cast<double>(N) * 2*M_PI;

		double x = X + E*cos(alpha);
		double y = Y + F*sin(alpha);

		double xo = (x-X)*cos(ANG)-(y-Y)*sin(ANG)+X;
		double yo = (x-X)*sin(ANG)+(y-Y)*cos(ANG)+Y;

		file << xo << "\t" << yo << std::endl;
	}
}

bool CheckCollision(particle* iParticle, particle** vParticle, int I)
{
	double x1 = iParticle->X;
	double y1 = iParticle->Y;
	double wx1 = iParticle->E*cos(iParticle->ANG);
	double wy1 = iParticle->E*sin(iParticle->ANG);
	double hw1 = iParticle->F/iParticle->E;
	EllipseCollisionTest* ECT = new EllipseCollisionTest(100);

	for (int i = 0; i < I; ++i) {
		double x2 = vParticle[i]->X;
		double y2 = vParticle[i]->Y;
		double wx2 = vParticle[i]->E*cos(vParticle[i]->ANG);
		double wy2 = vParticle[i]->E*sin(vParticle[i]->ANG);
		double hw2 = vParticle[i]->F/vParticle[i]->E;

		if (iParticle->E == iParticle->F && vParticle[i]->E == vParticle[i]->F)
		{
			double x2 = vParticle[i]->X;
			double y2 = vParticle[i]->Y;
			double r1 = iParticle->E;
			double r2 = vParticle[i]->E;

			double d = sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));

			if (d < r1+r2) return true;
		} else if (ECT->collide(x1, y1, wx1, wy1, hw1, x2, y2, wx2, wy2, hw2)) return true;
	}

	return false;
}