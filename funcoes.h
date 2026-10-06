//---------------------------------------------------------------------------

#ifndef funcoesH
#define funcoesH

#include "uPoligono.h"
#include "uDisplay.h"
#include "uJanela.h"
#include "uCircunferencia.h"
#include <stack>
#include <Vcl.Graphics.hpp>
#include <Vcl.StdCtrls.hpp>
//---------------------------------------------------------------------------

void aplicarTransladar(Display &display, int idx, double dx, double dy);

void aplicarRotacionar(Display &display, int idx, double angulo);

void aplicarRotacaoHomogenea(Display &display, int idx, double angulo, double px, double py);

void aplicarRefletir(Display &display, int idx, int eixo);

void aplicarCircunferencia(Display &display, double xc, double yc, double r,
                            TCanvas *canvas, const Janela &vp, const Janela &mundo,
                            int tipoReta, bool clipAtivo, TListBox *lbPoligonos);

void aplicarClipping(Display &display, bool &clipAtivo,
                     double cxMin, double cxMax, double cyMin, double cyMax,
                     TCanvas *canvas, const Janela &vp, const Janela &mundo,
                     int tipoReta, TListBox *lbPoligonos);

void recriaEixos(Display &display, const Janela &mundo);

void aplicarZoomIn(Display &display, Janela &mundo, std::stack<Janela> &historico,
                   double xMin, double yMin, double xMax, double yMax,
                   TCanvas *canvas, const Janela &vp, int tipoReta, bool clipAtivo);

void aplicarZoomOut(Display &display, Janela &mundo, std::stack<Janela> &historico,
                    TEdit *edXmin, TEdit *edXmax, TEdit *edYmin, TEdit *edYmax,
                    TCanvas *canvas, const Janela &vp, int tipoReta, bool clipAtivo);

// tipo: 8 = Hermite, 9 = Bezier
// Retorna false se o poligono selecionado tiver menos de 4 pontos
bool aplicarCurva(Display &display, int idx, int tipo, int &contId);

// Curvas por Forward Differences (somente somas no laco de desenho)
void DesenhaCurvaFwdDiff(Poligono &curva, int n,
                         double x, double Dx, double D2x, double D3x,
                         double y, double Dy, double D2y, double D3y);

// Curva de Hermite; n = numero de passos (delta t = 1/n)
// Retorna false se o poligono selecionado tiver menos de 4 pontos
bool ConfiguraE_DesenhaCurvaFwdDiff(Display &display, int idx,
                                    int n, int &contId);

// ---------------------------------------------------------------------------
// Cohen-Sutherland Line Clipping
// Codigos de regiao. Bit layout: [TOP | BOTTOM | RIGHT | LEFT]
// ---------------------------------------------------------------------------
const int CS_INSIDE  = 0; // 0000
const int CS_LEFT    = 1; // 0001
const int CS_RIGHT   = 2; // 0010
const int CS_BOTTOM  = 4; // 0100  (y < yMin no sistema mundo)
const int CS_TOP     = 8; // 1000  (y > yMax no sistema mundo)

// Retorna o codigo de regiao de um ponto (xp, yp) em relacao a janela clip
int cohenSutherlandCodigo(double xp, double yp, const Janela &clip);

// Retorna true  -> segmento visivel (total ou parcialmente); x1,y1,x2,y2 atualizados.
// Retorna false -> segmento totalmente fora.
bool cohenSutherlandClip(double &x1, double &y1,
                         double &x2, double &y2,
                         const Janela &clip);

#endif
