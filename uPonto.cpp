//---------------------------------------------------------------------------

#pragma hdrstop

#include "uPonto.h"
#include <cmath>
//---------------------------------------------------------------------------
#pragma package(smart_init)

Ponto::Ponto(){
	x = y = z = 0;
};

Ponto::Ponto(double nx, double ny){
	x = nx;
	y = ny;
	z = 0;
};

Ponto::Ponto(double nx, double ny, double nz){
	x = nx;
	y = ny;
	z = nz;
};

// Multiplica o vetor linha [x y z 1] pela matriz 4x4 e atualiza x, y, z
void Ponto::aplicaMatriz(const double m[4][4]){
	double v[4] = { x, y, z, 1.0 };
	double r[4] = { 0, 0, 0, 0 };
	for (int c = 0; c < 4; c++)
		for (int l = 0; l < 4; l++)
			r[c] += v[l] * m[l][c];
	x = r[0];
	y = r[1];
	z = r[2];
};

static double grausParaRad(double g){
	return g * 3.14159265358979323846 / 180.0;
}

void Ponto::Translacao(double tx, double ty, double tz){
	double m[4][4] = {
		{ 1,  0,  0,  0 },
		{ 0,  1,  0,  0 },
		{ 0,  0,  1,  0 },
		{ tx, ty, tz, 1 } };
	aplicaMatriz(m);
};

void Ponto::Escalonamento(double sx, double sy, double sz){
	double m[4][4] = {
		{ sx, 0,  0,  0 },
		{ 0,  sy, 0,  0 },
		{ 0,  0,  sz, 0 },
		{ 0,  0,  0,  1 } };
	aplicaMatriz(m);
};

void Ponto::RotacaoX(double anguloGraus){
	double c = cos(grausParaRad(anguloGraus));
	double s = sin(grausParaRad(anguloGraus));
	double m[4][4] = {
		{ 1,  0,  0, 0 },
		{ 0,  c,  s, 0 },
		{ 0, -s,  c, 0 },
		{ 0,  0,  0, 1 } };
	aplicaMatriz(m);
};

void Ponto::RotacaoY(double anguloGraus){
	double c = cos(grausParaRad(anguloGraus));
	double s = sin(grausParaRad(anguloGraus));
	double m[4][4] = {
		{ c, 0, -s, 0 },
		{ 0, 1,  0, 0 },
		{ s, 0,  c, 0 },
		{ 0, 0,  0, 1 } };
	aplicaMatriz(m);
};

void Ponto::RotacaoZ(double anguloGraus){
	double c = cos(grausParaRad(anguloGraus));
	double s = sin(grausParaRad(anguloGraus));
	double m[4][4] = {
		{  c, s, 0, 0 },
		{ -s, c, 0, 0 },
		{  0, 0, 1, 0 },
		{  0, 0, 0, 1 } };
	aplicaMatriz(m);
};

int Ponto::xW2Vp(Janela vp, Janela mundo){
	return ((x - mundo.xMin) / (mundo.xMax - mundo.xMin)) *
		   (vp.xMax - vp.xMin) + vp.xMin;
};

int Ponto::yW2Vp(Janela vp, Janela mundo){
	return (1 - (y - mundo.yMin) / (mundo.yMax - mundo.yMin)) *
		   (vp.yMax - vp.yMin) + vp.yMin;
};

AnsiString Ponto::toString(){
	return "(" + FloatToStr(x) + ","
               + FloatToStr(y) + ")";
};
