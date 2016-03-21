#include "particle.h"
#include "random.h"
#include "gnuplot-iostream.h"

const int nParticle	= 1000;
const int nStep = 1000;
const int xBox[2] = {-50, 50};
const int yBox[2] = {-50, 50};
const double scale = 1;
const double dt = 0.1;

void Draw(particle** vParticle) // rysowanie GNUplot
{
	Gnuplot gp;
	std::vector<std::pair<double, double> > vPoints; 
	std::vector<std::pair<double, double> > tempPoints;

	for (int iParticle = 0; iParticle < nParticle; ++iParticle) {
		tempPoints = vParticle[iParticle]->DrawParticle();
		vPoints.insert(vPoints.end(), tempPoints.begin(), tempPoints.end());
	}

	gp << "set terminal wxt size 600,600\nset key off\n";
	gp << "plot '-' with dots\n";
	gp.send1d(vPoints);
}

void GenerateRandom(particle** vParticle)
{
	rnd* R = new rnd(time(NULL));
	int err = 0;

	for (int iParticle = 0; iParticle < nParticle; ++iParticle) { // tworzenie obiektów: losowe
		std::cout << "\r" << iParticle+1 << "/" << nParticle;
		vParticle[iParticle] = new particle(R->jedn(xBox[0], xBox[1]), R->jedn(yBox[0], yBox[1]), 1);
		while (CheckCollision(vParticle[iParticle], vParticle, iParticle)) {
			vParticle[iParticle] = new particle(R->jedn(xBox[0], xBox[1]), R->jedn(yBox[0], yBox[1]), 1);
			if (++err == 10000) throw std::exception();
		}
		err = 0;
	}	
	std::cout << std::endl;
}

void GenerateLattice(particle** vParticle)
{
	double r = 0.5;
	for (int i = 0; i < sqrt(nParticle); ++i) { // tworzenie obiektów: sieć kwadratowa
		for (int j = 0; j < sqrt(nParticle); ++j) {
			vParticle[i*(int)sqrt(nParticle)+j] = new particle(2*r*i, 2*r*j, r);
		}
	}

	if((int)sqrt(nParticle)%2)
		for (int iParticle = 0; iParticle < nParticle; ++iParticle) {
			vParticle[iParticle]->Translate(-r*sqrt(nParticle), -r*sqrt(nParticle));
		}
	else
		for (int iParticle = 0; iParticle < nParticle; ++iParticle) {
			vParticle[iParticle]->Translate(-r*sqrt(nParticle)+r, -r*sqrt(nParticle)+r);
		}

	for (int iParticle = 0; iParticle < nParticle; ++iParticle)
	{
		vParticle[iParticle]->ScaleAll(1./sqrt(nParticle));
	}
}

int main(int argc, char const *argv[]) // main
{
	std::fstream data("data", std::fstream::out);

	particle* vParticle[nParticle];
	GenerateRandom(vParticle);

	// for (int iParticle = 0; iParticle < nParticle; ++iParticle) { // zapis obiektów do pliku
	// 	vParticle[iParticle]->DrawParticle(data);
	// }
	
	Draw(vParticle); // rysowanie w GNUplot
	
	return 0;
}