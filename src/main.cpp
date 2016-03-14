#include "particle.h"
#include "random.h"

const int nParticle	= 1000;
const int mod = 4; // wersja klasy: 1 - koła, 2 - elipsy moje, 3 - elipsy z zewn. biblioteką, 4 - 1+3 z dziedziczeniem

int main(int argc, char const *argv[])
{
	std::fstream data("data", std::fstream::out);
	rnd* R = new rnd(time(NULL));
	int err = 0;

	particle* vParticle[nParticle];
	for (int iParticle = 0; iParticle < nParticle; ++iParticle) {
		std::cout << "\r" << iParticle+1 << "/" << nParticle;
		vParticle[iParticle] = new particle(R->jedn(0, 100), R->jedn(0, 100), 1, 1, R->jedn(0, 360));
		while (CheckCollision(vParticle[iParticle], vParticle, iParticle)) {
			vParticle[iParticle] = new particle(R->jedn(0, 100), R->jedn(0, 100), 1, 1, R->jedn(0, 360));
			if (++err == 100000) return 100;
		}
		err = 0;
	}
	std::cout << std::endl;

	for (int iParticle = 0; iParticle < nParticle; ++iParticle) {
		vParticle[iParticle]->DrawParticle(data);
	}

	return 0;
}