#include "particle.h"
#include "random.h"
#include "gnuplot-iostream.h"

const int nParticle	= 100;
const int nStep = 100;
const double xBox[2] = {-0.5, 0.5};
const double yBox[2] = {-0.5, 0.5};
const double dt = 0.01;
const double s = 1.;
const double r = 0.025;
const double offset = 0.1;

bool OutOfBox(particle* iParticle)
{
	if(	iParticle->X < xBox[0] || iParticle->X > xBox[1] ||
		iParticle->Y < yBox[0] || iParticle->Y > yBox[1]) return true;
	return false;
}

bool OutOfBox2(particle* iParticle)
{
	if(	iParticle->X < xBox[0]+r || iParticle->X > xBox[1]-r ||
		iParticle->Y < yBox[0]+r || iParticle->Y > yBox[1]-r) return true;
	return false;
}

void ReturnToBox(particle* iParticle)
{
	if(iParticle->X < xBox[0]) iParticle->X += xBox[1]-xBox[0];
	if(iParticle->X > xBox[1]) iParticle->X -= xBox[1]-xBox[0];
	if(iParticle->Y < yBox[0]) iParticle->Y += yBox[1]-yBox[0];
	if(iParticle->Y > yBox[1]) iParticle->Y -= yBox[1]-yBox[0];
}

void BounceOffBox(particle* iParticle)
{
	double r = iParticle->E;
	if(iParticle->X < xBox[0]+r || iParticle->X > xBox[1]-r) iParticle->V[0] *= -1.;
	if(iParticle->Y < yBox[0]+r || iParticle->Y > yBox[1]-r) iParticle->V[1] *= -1.;
}

void Draw(particle** vParticle) // rysowanie GNUplot (debug)
{
	Gnuplot gp;
	std::vector<std::pair<double, double> > boxPoints;
	std::vector<boost::tuple<double, double, double> > vCircle;

	boxPoints.push_back(std::make_pair(xBox[0], yBox[0]));
	boxPoints.push_back(std::make_pair(xBox[0], yBox[1]));
	boxPoints.push_back(std::make_pair(xBox[1], yBox[1]));
	boxPoints.push_back(std::make_pair(xBox[1], yBox[0]));
	boxPoints.push_back(std::make_pair(xBox[0], yBox[0]));

	for (int iParticle = 0; iParticle < nParticle; ++iParticle) {
		vCircle.push_back(boost::make_tuple(
			vParticle[iParticle]->X, 
			vParticle[iParticle]->Y, 
			vParticle[iParticle]->E)
		);
	}

	gp << "set terminal wxt size 600,600\nset key off\n";
	gp << "plot '-' with circles, '-' with line\n";
	gp.send1d(vCircle);
	gp.send1d(boxPoints);
}

void Animate(particle **vParticle) // liczenie + animowanie
{
	Gnuplot gp;

	gp << "set terminal wxt size 600,600\nset key off\n";

	std::vector<std::pair<double, double> > boxPoints;
	std::vector<boost::tuple<double, double, double> > vCircle;

	boxPoints.push_back(std::make_pair(xBox[0], yBox[0]));
	boxPoints.push_back(std::make_pair(xBox[0], yBox[1]));
	boxPoints.push_back(std::make_pair(xBox[1], yBox[1]));
	boxPoints.push_back(std::make_pair(xBox[1], yBox[0]));
	boxPoints.push_back(std::make_pair(xBox[0], yBox[0]));


	for (int iStep = 0; iStep < nStep; ++iStep) {
		for (int iParticle = 0; iParticle < nParticle; ++iParticle) {
			vParticle[iParticle]->Move(dt);
			// if (OutOfBox(vParticle[iParticle])) ReturnToBox(vParticle[iParticle]);
			if (OutOfBox2(vParticle[iParticle])) BounceOffBox(vParticle[iParticle]);

			vCircle.push_back(boost::make_tuple(
				vParticle[iParticle]->X, 
				vParticle[iParticle]->Y, 
				vParticle[iParticle]->E)
			);
		}

		gp << "plot '-' with circles, '-' with line\n";
		gp.send1d(vCircle);
		gp.send1d(boxPoints);
		gp.flush();

		vCircle.clear();
	}
}

void GenerateRandom(particle** vParticle)
{
	rnd* R = new rnd(time(NULL));
	int err = 0;

	for (int iParticle = 0; iParticle < nParticle; ++iParticle) { // tworzenie obiektów: losowe
		std::cout << "\r" << iParticle+1 << "/" << nParticle;
		vParticle[iParticle] = new particle (
											R->jedn(xBox[0]+r, xBox[1]-r), 
											R->jedn(yBox[0]+r, yBox[1]-r), 
											r, 
											R->jedn(-1, 1), 
											R->jedn(-1, 1)
										);
		while (CheckCollision(vParticle[iParticle], vParticle, iParticle)) {
			vParticle[iParticle] = new particle (
												R->jedn(xBox[0]+r, xBox[1]-r), 
												R->jedn(yBox[0]+r, yBox[1]-r), 
												r, 
												R->jedn(-1, 1), 
												R->jedn(-1, 1)
											);
			if (++err == 10000) throw std::exception();
		}
		err = 0;
	}	
	std::cout << std::endl;

	for (int iParticle = 0; iParticle < nParticle; ++iParticle) {
		vParticle[iParticle]->ScalePosition(0.5/xBox[1]/s);
	}
}

void GenerateLattice(particle** vParticle)
{
	rnd* R = new rnd(time(NULL));

	for (int i = 0; i < sqrt(nParticle); ++i) { // tworzenie obiektów: sieć kwadratowa
		for (int j = 0; j < sqrt(nParticle); ++j) {
			vParticle[i*(int)sqrt(nParticle)+j] = new particle(2*i+offset, 2*j+offset, 1-2*offset, R->jedn(-1, 1), R->jedn(-1, 1));
		}
	}

	for (int iParticle = 0; iParticle < nParticle; ++iParticle) {
		vParticle[iParticle]->Translate(-sqrt(nParticle)+1, -sqrt(nParticle)+1);
	}

	for (int iParticle = 0; iParticle < nParticle; ++iParticle) {
		vParticle[iParticle]->ScalePosition(0.5/sqrt(nParticle)/s);
	}
}

int main(int argc, char const *argv[]) // main
{
	particle* vParticle[nParticle];
	GenerateRandom(vParticle);
	// Draw(vParticle);
	Animate(vParticle);
	
	return 0;
}
