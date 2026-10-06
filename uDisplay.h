//---------------------------------------------------------------------------

#ifndef uDisplayH
#define uDisplayH
#include <vector>
#include "uPoligono.h"
#include "uCircunferencia.h"
//---------------------------------------------------------------------------
class Display{
	public:
		std::vector <Poligono>       poligonos;
		std::vector <Circunferencia> circunferencias;
		Janela janelaCorte;  // janela de recorte (Cohen-Sutherland), separada do mundo

		void desenha(TCanvas *canvas, Janela vp, Janela mundo, int tipoReta = 0, bool useClipping = false);

        void mostra(TListBox *listBox);
};

#endif
