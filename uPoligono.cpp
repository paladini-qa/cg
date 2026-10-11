//---------------------------------------------------------------------------

#pragma hdrstop

#include "uPoligono.h"
#include "funcoes.h"
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <cmath>
#include <cstdio>
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
	int x = x1;
	int y = y1;
	int sx = SIGN(x2 - x1);
	int sy = SIGN(y2 - y1);
	float DeltaX = (float)MABS(x2 - x1);
	float DeltaY = (float)MABS(y2 - y1);
	bool troca = false;

	if (DeltaY > DeltaX) {
		float tmp = DeltaX;
		DeltaX = DeltaY;
		DeltaY = tmp;
		troca = true;
	}

	if (DeltaX == 0) {
		canvas->Pixels[x][y] = canvas->Pen->Color;
		return;
	}

	float e = DeltaY / DeltaX - 0.5f;
	for (int i = 1; i <= DeltaX; i++) {
		canvas->Pixels[x][y] = canvas->Pen->Color;
		if (e > 0) {
			if (troca) x += sx; else y += sy;
			e--;
		}
		if (troca) y += sy; else x += sx;
		e += DeltaY / DeltaX;
	}
	// pixel final
	canvas->Pixels[x2][y2] = canvas->Pen->Color;
}

// --- Auxiliares para matrizes homogeneas 3x3 (vetor linha [x y 1] * M) ---
static void mult3(const double a[3][3], const double b[3][3], double r[3][3]) {
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++) {
			r[i][j] = 0;
			for (int k = 0; k < 3; k++)
				r[i][j] += a[i][k] * b[k][j];
		}
}

static void aplicaMatriz2D(Ponto &p, const double m[3][3]) {
	double nx = p.x * m[0][0] + p.y * m[1][0] + m[2][0];
	double ny = p.x * m[0][1] + p.y * m[1][1] + m[2][1];
	p.x = nx;
	p.y = ny;
}

// Translacao (matricial)
//           | 1  0  0 |
// [x y 1] * | 0  1  0 |
//           | dx dy 1 |
void Poligono::translada(double dx, double dy) {
	double T[3][3] = {
		{  1,  0, 0 },
		{  0,  1, 0 },
		{ dx, dy, 1 } };

	for (int i = 0; i < (int)pontos.size(); i++)
		aplicaMatriz2D(pontos[i], T);
}

// Rotacao simples (matricial): em torno da origem, sem ponto central
//           |  cos  sen  0 |
// [x y 1] * | -sen  cos  0 |
//           |   0    0   1 |
void Poligono::rotaciona(double angulo) {
	const double PI = 3.14159265358979323846;
	double rad = angulo * PI / 180.0;
	double c = cos(rad);
	double s = sin(rad);
	double R[3][3] = {
		{  c,  s, 0 },
		{ -s,  c, 0 },
		{  0,  0, 1 } };

	for (int i = 0; i < (int)pontos.size(); i++)
		aplicaMatriz2D(pontos[i], R);
}

// Rotacao homogenea (matricial): em torno do ponto central
// M = T(-px,-py) * R * T(px,py)
void Poligono::rotacionaHomogenea(double angulo, const PontoCentral &centro) {
	const double PI = 3.14159265358979323846;
	double rad = angulo * PI / 180.0;
	double c = cos(rad);
	double s = sin(rad);
	double px = centro.x;
	double py = centro.y;

	double T1[3][3] = {
		{   1,   0, 0 },
		{   0,   1, 0 },
		{ -px, -py, 1 } };
	double R[3][3] = {
		{  c,  s, 0 },
		{ -s,  c, 0 },
		{  0,  0, 1 } };
	double T2[3][3] = {
		{  1,  0, 0 },
		{  0,  1, 0 },
		{ px, py, 1 } };

	double tmp[3][3], M[3][3];
	mult3(T1, R, tmp);
	mult3(tmp, T2, M);

	for (int i = 0; i < (int)pontos.size(); i++)
		aplicaMatriz2D(pontos[i], M);
}

// Reflexao (matricial): vetor linha [x y 1] * M
// 0 = Eixo X: diag(1,-1,1)   1 = Eixo Y: diag(-1,1,1)
// 2 = Origem: diag(-1,-1,1)  3 = Reta y=x: troca x e y
void Poligono::reflete(int eixo) {
	double M[3][3] = {
		{ 1, 0, 0 },
		{ 0, 1, 0 },
		{ 0, 0, 1 } };

	switch (eixo) {
		case 0: // Eixo X  (y -> -y)
			M[1][1] = -1;
			break;
		case 1: // Eixo Y  (x -> -x)
			M[0][0] = -1;
			break;
		case 2: // Origem  (x -> -x, y -> -y)
			M[0][0] = -1;
			M[1][1] = -1;
			break;
		case 3: // Reta y=x  (troca x e y)
			M[0][0] = 0; M[0][1] = 1;
			M[1][0] = 1; M[1][1] = 0;
			break;
	}

	for (int i = 0; i < (int)pontos.size(); i++)
		aplicaMatriz2D(pontos[i], M);
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

// ---------------------------------------------------------------------------
// Transformacoes 3D (delegam para as matrizes homogeneas da classe Ponto)
// ---------------------------------------------------------------------------
void Poligono::translada3D(double dx, double dy, double dz) {
	for (int i = 0; i < (int)pontos.size(); i++)
		pontos[i].Translacao(dx, dy, dz);
}

// Aplica uma transformacao em torno do centroide: T(-c) * Transf * T(c)
#define TRANSFORMA_NO_CENTROIDE(CHAMADA) \
	if (pontos.empty()) return; \
	double cx = 0, cy = 0, cz = 0; \
	int n = (int)pontos.size(); \
	for (int i = 0; i < n; i++) { cx += pontos[i].x; cy += pontos[i].y; cz += pontos[i].z; } \
	cx /= n; cy /= n; cz /= n; \
	for (int i = 0; i < n; i++) { \
		pontos[i].Translacao(-cx, -cy, -cz); \
		pontos[i].CHAMADA; \
		pontos[i].Translacao(cx, cy, cz); \
	}

void Poligono::escalona(double sx, double sy, double sz) {
	TRANSFORMA_NO_CENTROIDE(Escalonamento(sx, sy, sz))
}

void Poligono::rotacionaX(double angulo) {
	TRANSFORMA_NO_CENTROIDE(RotacaoX(angulo))
}

void Poligono::rotacionaY(double angulo) {
	TRANSFORMA_NO_CENTROIDE(RotacaoY(angulo))
}

void Poligono::rotacionaZ(double angulo) {
	TRANSFORMA_NO_CENTROIDE(RotacaoZ(angulo))
}
// ---------------------------------------------------------------------------
// Importa os pontos de um arquivo texto usando fscanf.
// Formato: primeira linha = quantidade N de pontos; depois N linhas "x y z".
// Tenta o caminho informado e algumas pastas acima (exe em Win64x\Debug etc.)
// ---------------------------------------------------------------------------
bool Poligono::carregaArquivo(const char *caminho) {
	const char *prefixos[] = { "", "..\\", "..\\..\\", "..\\..\\..\\" };
	FILE *f = NULL;
	char nome[512];
	for (int i = 0; i < 4 && f == NULL; i++) {
		sprintf(nome, "%s%s", prefixos[i], caminho);
		f = fopen(nome, "r");
	}
	if (f == NULL) return false;

	int qtd = 0;
	if (fscanf(f, "%d", &qtd) != 1 || qtd <= 0) {
		fclose(f);
		return false;
	}

	std::vector<Ponto> lidos;
	for (int i = 0; i < qtd; i++) {
		double x, y, z;
		if (fscanf(f, "%lf %lf %lf", &x, &y, &z) != 3) {
			fclose(f);
			return false;
		}
		lidos.push_back(Ponto(x, y, z));
	}
	fclose(f);

	pontos = lidos;
	return true;
}