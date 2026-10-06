//---------------------------------------------------------------------------

#ifndef uCircunferenciaH
#define uCircunferenciaH

#include "uPonto.h"   // inclui uJanela.h transitivamente
#include <Vcl.Graphics.hpp>
#include <Vcl.StdCtrls.hpp>
#include <cmath>
//---------------------------------------------------------------------------

class Circunferencia {
public:
    int id;
    double xc, yc, r;   // centro e raio em coordenadas de mundo

    // -----------------------------------------------------------------------
    inline Circunferencia() {
        id = _nextId();
        xc = yc = 0; r = 1;
    }

    inline Circunferencia(double axc, double ayc, double ar) {
        id = _nextId();
        xc = axc; yc = ayc; r = ar;
    }

    // Retorna string descritiva para a listbox
    inline AnsiString toString() {
        return "C" + IntToStr(id) +
               " xc=" + FloatToStr(xc) +
               " yc=" + FloatToStr(yc) +
               " r="  + FloatToStr(r);
    }

    inline void translada(double dx, double dy) {
        xc += dx;
        yc += dy;
    }

    // Desenha usando o algoritmo de Bresenham (Midpoint Circle)
    inline void desenha(TCanvas *canvas, Janela vp, Janela mundo) {
        int xcVp = _xW2Vp(xc, vp, mundo);
        int ycVp = _yW2Vp(yc, vp, mundo);
        int rVp  = _rW2Vp(r,  vp, mundo);
        if (rVp <= 0) return;

        int x = 0;
        int y = rVp;
        _plotaOctantes(canvas, xcVp, ycVp, x, y);
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
            _plotaOctantes(canvas, xcVp, ycVp, x, y);
        }
    }

private:
    // Gerador de IDs sequenciais compartilhado entre os construtores
    inline static int _nextId() {
        static int counter = 100;
        return counter++;
    }

    // Converte X de mundo para viewport
    inline int _xW2Vp(double x, Janela vp, Janela mundo) {
        return (int)(((x - mundo.xMin) / (mundo.xMax - mundo.xMin)) *
                     (vp.xMax - vp.xMin) + vp.xMin);
    }
    // Converte Y de mundo para viewport
    inline int _yW2Vp(double y, Janela vp, Janela mundo) {
        return (int)((1.0 - (y - mundo.yMin) / (mundo.yMax - mundo.yMin)) *
                     (vp.yMax - vp.yMin) + vp.yMin);
    }
    // Converte raio de mundo para viewport (escala X)
    inline int _rW2Vp(double r, Janela vp, Janela mundo) {
        return (int)(r / (mundo.xMax - mundo.xMin) * (vp.xMax - vp.xMin));
    }

    // Plota os 8 pontos simétricos de Bresenham
    inline void _plotaOctantes(TCanvas *canvas, int xc, int yc, int x, int y) {
        canvas->Pixels[xc + x][yc - y] = canvas->Pen->Color;
        canvas->Pixels[xc + x][yc + y] = canvas->Pen->Color;
        canvas->Pixels[xc - x][yc - y] = canvas->Pen->Color;
        canvas->Pixels[xc - x][yc + y] = canvas->Pen->Color;
        canvas->Pixels[xc + y][yc - x] = canvas->Pen->Color;
        canvas->Pixels[xc + y][yc + x] = canvas->Pen->Color;
        canvas->Pixels[xc - y][yc - x] = canvas->Pen->Color;
        canvas->Pixels[xc - y][yc + x] = canvas->Pen->Color;
    }
};

#endif
