//---------------------------------------------------------------------------

#ifndef uJanelaH
#define uJanelaH
//---------------------------------------------------------------------------

class Janela{
  public:
	double xMin, yMin, xMax, yMax;

	Janela(double nxMin, double nyMin, double nxMax, double nyMax);
	Janela() : xMin(-250), yMin(-250), xMax(250), yMax(250) {}
};
#endif
