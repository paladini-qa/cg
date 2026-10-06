//---------------------------------------------------------------------------

#pragma hdrstop

#include "uDisplay.h"
#include "uPonto.h"
#include <Vcl.StdCtrls.hpp>
//---------------------------------------------------------------------------
#pragma package(smart_init)

void Display::desenha(TCanvas *canvas, Janela vp, Janela mundo, int tipoReta, bool useClipping){

   // 1) Limpa completamente a área de desenho
   canvas->Brush->Color = clWhite;
   canvas->FillRect(Rect((int)vp.xMin, (int)vp.yMin, (int)vp.xMax, (int)vp.yMax));

   canvas->Pen->Width = 1;
   canvas->Pen->Style = psSolid;

   // 2) Quando clipping ativo: desenha o retângulo da janelaCorte em vermelho ANTES
   //    dos polígonos, para que ele fique na camada de fundo da cena
   if (useClipping) {
	  canvas->Pen->Color = clRed;
	  canvas->Pen->Width = 2;
	  Ponto tl(janelaCorte.xMin, janelaCorte.yMax);
	  Ponto tr(janelaCorte.xMax, janelaCorte.yMax);
	  Ponto br(janelaCorte.xMax, janelaCorte.yMin);
	  Ponto bl(janelaCorte.xMin, janelaCorte.yMin);
	  canvas->MoveTo(tl.xW2Vp(vp, mundo), tl.yW2Vp(vp, mundo));
	  canvas->LineTo(tr.xW2Vp(vp, mundo), tr.yW2Vp(vp, mundo));
	  canvas->LineTo(br.xW2Vp(vp, mundo), br.yW2Vp(vp, mundo));
	  canvas->LineTo(bl.xW2Vp(vp, mundo), bl.yW2Vp(vp, mundo));
	  canvas->LineTo(tl.xW2Vp(vp, mundo), tl.yW2Vp(vp, mundo));
	  canvas->Pen->Width = 1;
   }

   // 3) Desenha polígonos em preto.
   //    Quando useClipping=true, Poligono::desenha() aplica Cohen-Sutherland
   //    internamente e só rasteriza os segmentos dentro da janelaCorte;
   //    as partes fora NÃO são desenhadas (nenhum "fantasma" cinza).
   canvas->Pen->Color = clBlack;
   for (int i = 0; i < (int)poligonos.size(); i++) {
	  poligonos[i].desenha(canvas, vp, mundo, tipoReta, useClipping, janelaCorte);
   }

   // Desenha circunferências
   for (int i = 0; i < (int)circunferencias.size(); i++) {
	  circunferencias[i].desenha(canvas, vp, mundo);
   }
}


void Display::mostra(TListBox *listBox){
   listBox->Items->Clear();
   for (int i = 0; i < (int)poligonos.size(); i++) {
	  listBox->Items->Add(
		 IntToStr(poligonos[i].id) + "-" +
		 poligonos[i].tipo + "-" +
		 IntToStr((int)poligonos[i].pontos.size()));
   }
   for (int i = 0; i < (int)circunferencias.size(); i++) {
	  listBox->Items->Add(circunferencias[i].toString());
   }
}
