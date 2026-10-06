//---------------------------------------------------------------------------

#pragma hdrstop

#include "uCircunferencia.h"
#include "uPonto.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

static int contIdCirc = 100; // IDs de circunferências começam em 100

Circunferencia::Circunferencia() {
    id = contIdCirc++;
    xc = yc = 0;
    r  = 1;
}

Circunferencia::Circunferencia(double axc, double ayc, double ar) {
    id = contIdCirc++;
    xc = axc;
    yc = ayc;
    r  = ar;
}

// Converte coordenada de mundo para viewport (inteiro)
static int xW2Vp(double x, Janela vp, Janela mundo) {
    return (int)(((x - mundo.xMin) / (mundo.xMax - mundo.xMin)) *
                 (vp.xMax - vp.xMin) + vp.xMin);
}
static int yW2Vp(double y, Janela vp, Janela mundo) {
    return (int)((1.0 - (y - mundo.yMin) / (mundo.yMax - mundo.yMin)) *
                 (vp.yMax - vp.yMin) + vp.yMin);
}

// Escala do raio: usa apenas a escala x (assume aspecto 1:1)
static int rW2Vp(double r, Janela vp, Janela mundo) {
    return (int)(r / (mundo.xMax - mundo.xMin) * (vp.xMax - vp.xMin));
}

// Plota os 8 pontos simétricos de Bresenham (simetria octante)
void Circunferencia::desenhaPonto(TCanvas *canvas, Janela vp, Janela mundo,
                                  int xcVp, int ycVp, int x, int y) {
    canvas->Pixels[xcVp + x][ycVp - y] = canvas->Pen->Color;
    canvas->Pixels[xcVp + x][ycVp + y] = canvas->Pen->Color;
    canvas->Pixels[xcVp - x][ycVp - y] = canvas->Pen->Color;
    canvas->Pixels[xcVp - x][ycVp + y] = canvas->Pen->Color;
    canvas->Pixels[xcVp + y][ycVp - x] = canvas->Pen->Color;
    canvas->Pixels[xcVp + y][ycVp + x] = canvas->Pen->Color;
    canvas->Pixels[xcVp - y][ycVp - x] = canvas->Pen->Color;
    canvas->Pixels[xcVp - y][ycVp + x] = canvas->Pen->Color;
}

// Algoritmo de Bresenham (Midpoint Circle) conforme slide
void Circunferencia::desenha(TCanvas *canvas, Janela vp, Janela mundo) {
    int xcVp = xW2Vp(xc, vp, mundo);
    int ycVp = yW2Vp(yc, vp, mundo);
    int rVp  = rW2Vp(r,  vp, mundo);

    int x = 0;
    int y = rVp;
    desenhaPonto(canvas, vp, mundo, xcVp, ycVp, x, y);
    int p = 1 - rVp;

    while (x < y) {
        if (p < 0) {
            x++;
        } else {
            x++;
            y--;
        }
        if (p < 0)
            p += 2 * x + 1;
        else
            p += 2 * (x - y) + 1;
        desenhaPonto(canvas, vp, mundo, xcVp, ycVp, x, y);
    }
}

AnsiString Circunferencia::toString() {
    return "C" + IntToStr(id) +
           " xc=" + FloatToStr(xc) +
           " yc=" + FloatToStr(yc) +
           " r="  + FloatToStr(r);
}
