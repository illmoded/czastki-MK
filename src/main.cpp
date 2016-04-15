#include "particle.h"
#include "random.h"
#include "gnuplot-iostream.h"

const int nParticle	= 100;
const int nStep = 10000;
const double xBox[2] = {-0.5, 0.5};
const double yBox[2] = {-0.5, 0.5};
const double dt = 0.02;
const double s = 1.;
const double r = 0.01;
const double offset = 0.0;
const double T = .1;

void MakeHistogram(particle** vParticle)
{
	int M = 15;
	double a = 0.;
	double b = 0.3;
	double v;
	int H[M] = {};

	std::fstream data("data", std::fstream::out);

	for (int iParticle = 0; iParticle < nParticle; ++iParticle) {
		double vx = vParticle[iParticle]->VX;
		double vy = vParticle[iParticle]->VY;

		v = sqrt(vx*vx+vy*vy);
		for (int i = 0; i < M; ++i) {
			if (v > a+(b-a)/M*i && v < a+(b-a)/M*(i+1)) H[i]++;
		}
	}

	for (int i = 0; i < M; ++i) data << a+(b-a)/M*((double)i+0.5) << "\t" << H[i] << std::endl;
}

bool OutOfBox(particle* iParticle)
{
	if(	iParticle->X < xBox[0] || iParticle->X > xBox[1] ||
		iParticle->Y < yBox[0] || iParticle->Y > yBox[1]) return true;
	return false;
}

bool OutOfBox2(particle* iParticle)
{
	double rr = iParticle->R;
	if(	iParticle->X < xBox[0]+rr || iParticle->X > xBox[1]-rr ||
		iParticle->Y < yBox[0]+rr || iParticle->Y > yBox[1]-rr) return true;
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
	double rr = iParticle->R;
	if(iParticle->X < xBox[0]+rr || iParticle->X > xBox[1]-rr) iParticle->VX *= -1.;
	if(iParticle->Y < yBox[0]+rr || iParticle->Y > yBox[1]-rr) iParticle->VY *= -1.;
}

void Draw(particle** vParticle) // rysowanie GNUplot (debug)
{
	Gnuplot gp;
	std::fstream data("data", std::fstream::out);

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
			vParticle[iParticle]->R)
		);
		data << vParticle[iParticle]->X << "\t" << vParticle[iParticle]->Y << "\t" << vParticle[iParticle]->R << std::endl;
	}

	gp << "set terminal wxt size 600,600\nset key off\n";
	gp << "plot '-' with circles, '-' with line\n";
	gp.send1d(vCircle);
	gp.send1d(boxPoints);
}

void Animate(particle** vParticle) // liczenie + animowanie
{
	Gnuplot gp;
	gp << "set terminal wxt size 600,600\nset key off\n";

	std::vector<std::pair<int, int> > vPairs;
	std::vector<std::pair<double, double> > boxPoints;
	std::vector<boost::tuple<double, double, double> > vCircle;

	boxPoints.push_back(std::make_pair(xBox[0], yBox[0]));
	boxPoints.push_back(std::make_pair(xBox[0], yBox[1]));
	boxPoints.push_back(std::make_pair(xBox[1], yBox[1]));
	boxPoints.push_back(std::make_pair(xBox[1], yBox[0]));
	boxPoints.push_back(std::make_pair(xBox[0], yBox[0]));

	for (int iStep = 0; iStep < nStep; ++iStep) {
		vPairs = Collision(vParticle, nParticle);
		if (vPairs.size() != 0) {
			for (std::vector<std::pair<int, int> >::iterator vec = vPairs.begin(); vec != vPairs.end(); ++vec) {
				int a = vec->first;
				int b = vec->second;

				EvaluateVelocities(vParticle[a], vParticle[b]);
			}
		}

		for (int iParticle = 0; iParticle < nParticle; ++iParticle) {
			vParticle[iParticle]->Move(dt);
			// if (OutOfBox(vParticle[iParticle])) ReturnToBox(vParticle[iParticle]);
			if (OutOfBox2(vParticle[iParticle])) BounceOffBox(vParticle[iParticle]);

			vCircle.push_back(boost::make_tuple(
				vParticle[iParticle]->X, 
				vParticle[iParticle]->Y, 
				vParticle[iParticle]->R)
			);
		}

		gp << "plot '-' with circles, '-' with line\n";
		gp.send1d(vCircle);
		gp.send1d(boxPoints);
		gp.flush();

		vPairs.clear();
		vCircle.clear();
	}
}

void Evaluate(particle** vParticle) // liczenie + histogram prędkości
{
	std::vector<std::pair<int, int> > vPairs;

	for (int iStep = 0; iStep < nStep; ++iStep) {
		vPairs = Collision(vParticle, nParticle);
		if (vPairs.size() != 0) {
			for (std::vector<std::pair<int, int> >::iterator vec = vPairs.begin(); vec != vPairs.end(); ++vec) {
				int a = vec->first;
				int b = vec->second;

				EvaluateVelocities(vParticle[a], vParticle[b]);
			}
		}

		for (int iParticle = 0; iParticle < nParticle; ++iParticle) {
			vParticle[iParticle]->Move(dt);
			// if (OutOfBox(vParticle[iParticle])) ReturnToBox(vParticle[iParticle]);
			if (OutOfBox2(vParticle[iParticle])) BounceOffBox(vParticle[iParticle]);
		}

		std::cout << "\rPoliczono: " << iStep+1 << "/" << nStep << std::flush;
	}

	std::cout << std::endl;
	MakeHistogram(vParticle);
}

void GenerateRandom(particle** vParticle)
{
	rnd* R = new rnd(1);
	int err = 0;

	for (int iParticle = 0; iParticle < nParticle; ++iParticle) { // tworzenie obiektów: losowe
		std::cout << "\rUtworzono: " << iParticle+1 << "/" << nParticle << std::flush;
		vParticle[iParticle] = new particle (
											R->jedn(xBox[0]+r, xBox[1]-r), 
											R->jedn(yBox[0]+r, yBox[1]-r), 
											r, 
											R->gauss(0, T), 
											R->gauss(0, T)
										);
		while (CheckCollision(vParticle[iParticle], vParticle, iParticle)) {
			vParticle[iParticle] = new particle (
												R->jedn(xBox[0]+r, xBox[1]-r), 
												R->jedn(yBox[0]+r, yBox[1]-r), 
												r, 
												R->gauss(0, T), 
												R->gauss(0, T)
											);
			if (++err == 10000) {
				std::cout << std::endl;
				throw std::exception();
			}
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
			vParticle[i*(int)sqrt(nParticle)+j] = new particle(	2*i+offset, 
																2*j+offset, 
																1-2*offset, 
																R->gauss(0, T), 
																R->gauss(0, T)
															);
		}
	}

	for (int iParticle = 0; iParticle < nParticle; ++iParticle) {
		vParticle[iParticle]->Translate(-sqrt(nParticle)+1-offset, -sqrt(nParticle)+1-offset);
	}

	for (int iParticle = 0; iParticle < nParticle; ++iParticle) {
		vParticle[iParticle]->ScalePosition(0.5/sqrt(nParticle)/s);
	}
}

void GenerateHexagonal(particle** vParticle)
{
	rnd* R = new rnd(time(NULL));

	for (int i = 0; i < sqrt(nParticle); ++i) { // tworzenie obiektów: sieć kwadratowa
		for (int j = 0; j < sqrt(nParticle); ++j) {
			if (j%2) vParticle[i*(int)sqrt(nParticle)+j] = new particle(2*i+offset, 
																		2*j+offset, 
																		1-2*offset, 
																		R->gauss(0, T), 
																		R->gauss(0, T)
																	);
			else vParticle[i*(int)sqrt(nParticle)+j] = new particle(2*i+1+offset, 
																	2*j+offset, 
																	1-2*offset, 
																	R->gauss(0, T), 
																	R->gauss(0, T)
																);
		}
	}

	for (int iParticle = 0; iParticle < nParticle; ++iParticle) {
		vParticle[iParticle]->Translate(-sqrt(nParticle)+1-offset, -sqrt(nParticle)+1-offset);
	}

	for (int iParticle = 0; iParticle < nParticle; ++iParticle) {
		vParticle[iParticle]->ScalePosition(0.5/sqrt(nParticle)/s);
	}
}

int main(int argc, char const *argv[]) // main
{
	particle* vParticle[nParticle];
	GenerateHexagonal(vParticle);
	
	Draw(vParticle);
	// Animate(vParticle);
	// Evaluate(vParticle);

	return 0;
}