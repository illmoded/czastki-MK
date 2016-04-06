#include "particle.h"

// okręgi
particle::particle(double posX, double posY, double rad)
{
	X = posX;
	Y = posY;
	R = rad;
}

particle::particle(double posX, double posY, double rad, double velX, double velY)
{
	X = posX;
	Y = posY;
	R = rad;
	V[0] = velX;
	V[1] = velY;
}

void particle::Flush()
{
	printf("X:\tY:\tR:\tVx:\tVy:\n");
	printf("%f\t%f\t%f\t%f\t%f\n", X, Y, R, V[0], V[1]);
}

void particle::Move(double step)
{
	X += step*V[0];
	Y += step*V[1];
}

void particle::ScalePosition(double s)
{
	X *= s;
	Y *= s;
	R *= s;
}

void particle::Translate(double xVec, double yVec)
{
	X += xVec;
	Y += yVec;
}

double particle::GetMass()
{
	return M;
}

bool CheckCollision(particle* iParticle, particle** vParticle, int I)
{
	double x1 = iParticle->X;
	double y1 = iParticle->Y;
	double r1 = iParticle->R;

	for (int i = 0; i < I; ++i) {
		double x2 = vParticle[i]->X;
		double y2 = vParticle[i]->Y;
		double r2 = vParticle[i]->R;

		double d = sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));

		if (d < r1+r2) return true;
	}

	return false;
}

std::vector<std::pair<int, int> > Collision(particle** vParticle, int nParticle)
{
	std::vector<std::pair<int, int> > vCol;

	for (int i = 0; i < nParticle; ++i) {
		double x1 = vParticle[i]->X;
		double y1 = vParticle[i]->Y;
		double r1 = vParticle[i]->R;

		for (int j = i; j < nParticle; ++j) {
			double x2 = vParticle[j]->X;
			double y2 = vParticle[j]->Y;
			double r2 = vParticle[j]->R;

			double d = sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));
			if (d <= r1+r2 && i != j) {
				vCol.push_back(std::make_pair(i, j));
			}			
		}
	}

	return vCol;
}

void EvaluateVelocities(particle* first, particle* second)
{
	double x1 = first->X;
	double y1 = first->Y;
	double vx1 = first->V[0];
	double vy1 = first->V[1];
	double v1 = sqrt(vx1*vx1+vy1*vy1);
	double costh1 = vx1/v1;
	double sinth1 = vy1/v1;

	double x2 = second->X;
	double y2 = second->Y;
	double vx2 = second->V[0];
	double vy2 = second->V[1];
	double v2 = sqrt(vx2*vx2+vy2*vy2);
	double costh2 = vx2/v2;
	double sinth2 = vy2/v2;

	double k = 1;
	double m = first->GetMass()/second->GetMass();
	double d = sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
	double sinfi = (x2-x1)/d;
	double cosfi = (y1-y2)/d;

	double wm1 = v1*(costh1*cosfi+sinth1*sinfi);
	double wn1 = v1*(m-k)/(m+1)*(sinth1*cosfi-costh1*sinfi)+v2*(k+1)/(m+1)*(sinth2*cosfi-costh2*sinfi);
	double wm2 = v2*(costh2*cosfi+sinth2*sinfi);
	double wn2 = v1*m*(k+1)/(m+1)*(sinth1*cosfi-costh1*sinfi)+v2*(1-k*m)/(m+1)*(sinth2*cosfi-costh2*sinfi);

	first->V[0] = wm1*cosfi-wn1*sinfi;
	first->V[1] = wm1*sinfi+wn1*cosfi;;
	second->V[0] = wm2*cosfi-wn2*sinfi;
	second->V[1] = wm2*sinfi+wn2*cosfi;
}