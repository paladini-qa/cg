//---------------------------------------------------------------------------

#ifndef uPontoH
#define uPontoH
#include "uJanela.h"
//---------------------------------------------------------------------------

class Ponto {
	public:
		double x;
		double y;
		double z; // profundidade (0 para pontos 2D)

		Ponto();
		Ponto(double nx, double ny);
		Ponto(double nx, double ny, double nz);
		int xW2Vp(Janela vp, Janela mundo);
		int yW2Vp(Janela vp, Janela mundo);
        AnsiString toString();

		// Transformacoes 3D: vetor [x y z 1] * matriz 4x4 (atualiza x, y, z)
		void aplicaMatriz(const double m[4][4]);
		void Translacao(double tx, double ty, double tz);
		void Escalonamento(double sx, double sy, double sz);
		void RotacaoX(double anguloGraus); // plano YZ
		void RotacaoY(double anguloGraus); // plano XZ
		void RotacaoZ(double anguloGraus); // plano XY
};

#endif
