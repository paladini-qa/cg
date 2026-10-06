//---------------------------------------------------------------------------

#ifndef uPoligonoH
#define uPoligonoH
#include <vector>
#include "uPonto.h"
#include <Vcl.Controls.hpp>
#include <Vcl.ExtCtrls.hpp>
//---------------------------------------------------------------------------

class Poligono{
	public:
       Poligono();
	   std::vector <Ponto> pontos;
	   int id;
	   char tipo;

	   void mostra(TListBox *listBox);
	   void desenha(TCanvas *canvas, Janela vp, Janela mundo, int tipoReta = 0, bool useClipping = false, Janela janelaCorte = Janela());
	   Poligono clipa(const Janela &clip);  // retorna cópia com segmentos recortados
	   void translada(double dx, double dy);
	   void rotaciona(double angulo);
	   void rotacionaHomogenea(double angulo, double px, double py);
	   void reflete(int eixo); // 0=EixoX, 1=EixoY, 2=Origem, 3=y=x



};
#endif
