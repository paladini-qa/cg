//---------------------------------------------------------------------------

#pragma hdrstop

#include "uPoligono.h"
#include "funcoes.h"
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <cmath>
//---------------------------------------------------------------------------
#pragma package(smart_init)

Poligono::Poligono(){
	tipo = 'N';
	id = -1;
}

void Poligono::mostra(TListBox *listBox){
	listBox->Items->Clear();
	for (int i = 0; i < (int)pontos.size(); i++) {
		listBox->Items->Add(pontos[i].toString());
	}
}

// --- Macros auxiliares (conforme slides) ---
#define SIGN(x)  ((x) < 0 ? (-1) : (1))
#define MABS(x)  ((x) < 0 ? (-(x)) : (x))
#define FLOOR(x) ((x) < 0 ? ( (x) - (int)(x) != 0 ? ((int)(x) - 1) : ((int)(x))) : (int)(x))

// --- Algoritmo DDA (conforme slide Aula 2) ---
static void desenhaDDA(TCanvas *canvas, int x1, int y1, int x2, int y2) {
	int length;
	if (MABS(x2 - x1) >= MABS(y2 - y1))
		length = MABS(x2 - x1);
	else
		length = MABS(y2 - y1);

	if (length == 0) {
		canvas->Pixels[x1][y1] = canvas->Pen->Color;
		return;
	}

	float deltax = (float)(x2 - x1) / (float)length;
	float deltay = (float)(y2 - y1) / (float)length;
	float x = x1 + 0.5f * SIGN(deltax);
	float y = y1 + 0.5f * SIGN(deltay);

	for (int i = 0; i < length; i++) {
		canvas->Pixels[FLOOR(x)][FLOOR(y)] = canvas->Pen->Color;
		x += deltax;
		y += deltay;
	}
}

// --- Algoritmo de Bresenham (conforme slide Aula 2) ---
static void desenhaBresenham(TCanvas *canvas, int x1, int y1, int x2, int y2) {
	int deltax  = MABS(x2 - x1);
	int deltay  = MABS(y2 - y1);
	int signalx = SIGN(x2 - x1);
	int signaly = SIGN(y2 - y1);
	int x = x1;
	int y = y1;

	if (signalx < 0) x -= 1;
	if (signaly < 0) y -= 1;

	bool interchange = false;
	if (deltay > deltax) {
		int tmp = deltax;
		deltax  = deltay;
		deltay  = tmp;
		interchange = true;
	}

	int erro = 2 * deltay - deltax;

	for (int i = 0; i < deltax; i++) {
		canvas->Pixels[x][y] = canvas->Pen->Color;
		while (erro >= 0) {
			if (interchange)
				x = x + signalx;
			else
				y = y + signaly;
			erro = erro - 2 * deltax;
		}
		if (interchange)
			y = y + signaly;
		else
			x = x + signalx;
		erro = erro + 2 * deltay;
	}
}

void Poligono::translada(double dx, double dy) {
	for (int i = 0; i < (int)pontos.size(); i++) {
		pontos[i].x += dx;
		pontos[i].y += dy;
	}
}

void Poligono::rotaciona(double angulo) {
	if (pontos.empty()) return;

	const double PI = 3.14159265358979323846;
	double rad  = angulo * PI / 180.0;
	double cosA = cos(rad);
	double sinA = sin(rad);

	// 1) Calcula o centroide do polígono
	double cx = 0.0, cy = 0.0;
	int n = (int)pontos.size();
	for (int i = 0; i < n; i++) {
		cx += pontos[i].x;
		cy += pontos[i].y;
	}
	cx /= n;
	cy /= n;

	for (int i = 0; i < n; i++) {
		double dx = pontos[i].x - cx;
		double dy = pontos[i].y - cy;
		pontos[i].x = dx * cosA - dy * sinA + cx;
		pontos[i].y = dx * sinA + dy * cosA + cy;
	}
}

// Rotação homogênea em torno do ponto pivô (px, py)
// Equivale a: T(-px,-py) · R(?) · T(px,py)
// x' = (x-px)*cos(?) - (y-py)*sin(?) + px
// y' = (x-px)*sin(?) + (y-py)*cos(?) + py
void Poligono::rotacionaHomogenea(double angulo, double px, double py) {
	const double PI = 3.14159265358979323846;
	double rad = angulo * PI / 180.0;
	double cosA = cos(rad);
	double sinA = sin(rad);
	for (int i = 0; i < (int)pontos.size(); i++) {
		double dx = pontos[i].x - px;
		double dy = pontos[i].y - py;
		pontos[i].x = dx * cosA - dy * sinA + px;
		pontos[i].y = dx * sinA + dy * cosA + py;
	}
}

void Poligono::reflete(int eixo) {
	for (int i = 0; i < (int)pontos.size(); i++) {
		double x = pontos[i].x;
		double y = pontos[i].y;
		switch (eixo) {
			case 0: // Eixo X  (y ? -y)
				pontos[i].y = -y;
				break;
			case 1: // Eixo Y  (x ? -x)
				pontos[i].x = -x;
				break;
			case 2: // Origem  (x ? -x, y ? -y)
				pontos[i].x = -x;
				pontos[i].y = -y;
				break;
			case 3: // Reta y=x  (troca x e y)
				pontos[i].x = y;
				pontos[i].y = x;
				break;
		}
	}
}

void Poligono::desenha(TCanvas *canvas, Janela vp, Janela mundo, int tipoReta, bool useClipping, Janela janelaCorte) {
	if (!useClipping) {
		// Comportamento original: MoveTo no primeiro ponto, LineTo nos demais
		for (int i = 0; i < (int)pontos.size(); i++) {
			int x = pontos[i].xW2Vp(vp, mundo);
			int y = pontos[i].yW2Vp(vp, mundo);

			if (i == 0) {
				canvas->MoveTo(x, y);
			} else {
				int xAnt = pontos[i-1].xW2Vp(vp, mundo);
				int yAnt = pontos[i-1].yW2Vp(vp, mundo);

				if (tipoReta == 1) {
					desenhaDDA(canvas, xAnt, yAnt, x, y);
				} else if (tipoReta == 2) {
					desenhaBresenham(canvas, xAnt, yAnt, x, y);
				} else {
					canvas->LineTo(x, y);
				}
			}
		}
	} else {
		// Modo Cohen-Sutherland: recorta cada segmento contra janelaCorte
		// Segmentos fora: omitidos. Segmentos dentro: desenhados em preto.
		TColor corOriginal = canvas->Pen->Color;

		for (int i = 1; i < (int)pontos.size(); i++) {
			double wx1 = pontos[i-1].x, wy1 = pontos[i-1].y;
			double wx2 = pontos[i].x,   wy2 = pontos[i].y;

			// Tenta recortar contra janelaCorte
			if (!cohenSutherlandClip(wx1, wy1, wx2, wy2, janelaCorte))
				continue; // segmento totalmente fora — pula

			// Converte pontos recortados para viewport (usando mundo para mapeamento)
			Ponto p1(wx1, wy1), p2(wx2, wy2);
			int xAnt = p1.xW2Vp(vp, mundo);
			int yAnt = p1.yW2Vp(vp, mundo);
			int x    = p2.xW2Vp(vp, mundo);
			int y    = p2.yW2Vp(vp, mundo);

			canvas->Pen->Color = corOriginal;

			if (tipoReta == 1) {
				desenhaDDA(canvas, xAnt, yAnt, x, y);
			} else if (tipoReta == 2) {
				desenhaBresenham(canvas, xAnt, yAnt, x, y);
			} else {
				canvas->MoveTo(xAnt, yAnt);
				canvas->LineTo(x, y);
			}
		}

		canvas->Pen->Color = corOriginal;
	}
}

// ---------------------------------------------------------------------------
// Retorna uma cópia do polígono com os segmentos recortados contra 'clip'.
// Cada segmento que sobrevive ao clipping é representado como par de pontos
// no vetor de resultado (polyline aberta de pares consecutivos).
// O resultado é usado apenas para exibição / debug.
// ---------------------------------------------------------------------------
Poligono Poligono::clipa(const Janela &clip) {
	Poligono resultado;
	resultado.id   = id;
	resultado.tipo = tipo;

	for (int i = 1; i < (int)pontos.size(); i++) {
		double x1 = pontos[i-1].x, y1 = pontos[i-1].y;
		double x2 = pontos[i].x,   y2 = pontos[i].y;

		if (cohenSutherlandClip(x1, y1, x2, y2, clip)) {
			// Adiciona par de pontos recortado
			resultado.pontos.push_back(Ponto(x1, y1));
			resultado.pontos.push_back(Ponto(x2, y2));
		}
	}
	return resultado;
}
