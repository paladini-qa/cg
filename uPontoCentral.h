//---------------------------------------------------------------------------

#ifndef uPontoCentralH
#define uPontoCentralH
//---------------------------------------------------------------------------

// Ponto central (pivo) usado pela rotacao homogenea.
class PontoCentral {
	public:
		double x;
		double y;

		PontoCentral();
		PontoCentral(double nx, double ny);
};

#endif
