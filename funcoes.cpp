//---------------------------------------------------------------------------

#pragma hdrstop

#include "funcoes.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
// Transladar
// ---------------------------------------------------------------------------
void aplicarTransladar(Display &display, int idx, double dx, double dy)
{
    if (idx != -1) {
        if (idx < (int)display.poligonos.size()) {
            if (display.poligonos[idx].tipo != 'E')
                display.poligonos[idx].translada(dx, dy);
        } else {
            int circIdx = idx - display.poligonos.size();
            if (circIdx >= 0 && circIdx < (int)display.circunferencias.size()) {
                display.circunferencias[circIdx].translada(dx, dy);
            }
        }
    } else {
        for (int i = 0; i < (int)display.poligonos.size(); i++) {
            if (display.poligonos[i].tipo != 'E')
                display.poligonos[i].translada(dx, dy);
        }
        for (int i = 0; i < (int)display.circunferencias.size(); i++) {
            display.circunferencias[i].translada(dx, dy);
        }
    }
}

// ---------------------------------------------------------------------------
// Rotacionar (origem)
// ---------------------------------------------------------------------------
void aplicarRotacionar(Display &display, int idx, double angulo)
{
    if (idx != -1) {
        if (display.poligonos[idx].tipo != 'E')
            display.poligonos[idx].rotaciona(angulo);
    } else {
        for (int i = 0; i < (int)display.poligonos.size(); i++)
            if (display.poligonos[i].tipo != 'E')
                display.poligonos[i].rotaciona(angulo);
    }
}

// ---------------------------------------------------------------------------
// Rotacao Homogenea (ponto arbitrario)
// ---------------------------------------------------------------------------
void aplicarRotacaoHomogenea(Display &display, int idx, double angulo, double px, double py)
{
    if (idx != -1) {
        if (display.poligonos[idx].tipo != 'E')
            display.poligonos[idx].rotacionaHomogenea(angulo, px, py);
    } else {
        for (int i = 0; i < (int)display.poligonos.size(); i++)
            if (display.poligonos[i].tipo != 'E')
                display.poligonos[i].rotacionaHomogenea(angulo, px, py);
    }
}

// ---------------------------------------------------------------------------
// Refletir
// ---------------------------------------------------------------------------
void aplicarRefletir(Display &display, int idx, int eixo)
{
    if (idx != -1) {
        if (display.poligonos[idx].tipo != 'E')
            display.poligonos[idx].reflete(eixo);
    } else {
        for (int i = 0; i < (int)display.poligonos.size(); i++)
            if (display.poligonos[i].tipo != 'E')
                display.poligonos[i].reflete(eixo);
    }
}

// ---------------------------------------------------------------------------
// Desenhar Circunferencia
// ---------------------------------------------------------------------------
void aplicarCircunferencia(Display &display, double xc, double yc, double r,
                            TCanvas *canvas, const Janela &vp, const Janela &mundo,
                            int tipoReta, bool clipAtivo, TListBox *lbPoligonos)
{
    Circunferencia circ(xc, yc, r);
    display.circunferencias.push_back(circ);
    display.desenha(canvas, vp, mundo, tipoReta, clipAtivo);
    display.mostra(lbPoligonos);
}

// ---------------------------------------------------------------------------
// Recortar (Cohen-Sutherland)
// ---------------------------------------------------------------------------
void aplicarClipping(Display &display, bool &clipAtivo,
                     double cxMin, double cxMax, double cyMin, double cyMax,
                     TCanvas *canvas, const Janela &vp, const Janela &mundo,
                     int tipoReta, TListBox *lbPoligonos)
{
    clipAtivo = !clipAtivo;
    if (clipAtivo)
        display.janelaCorte = Janela(cxMin, cyMin, cxMax, cyMax);

    display.desenha(canvas, vp, mundo, tipoReta, clipAtivo);
    display.mostra(lbPoligonos);
}

// ---------------------------------------------------------------------------
// Zoom - auxiliar: recria os eixos conforme o mundo atual
// ---------------------------------------------------------------------------
void recriaEixos(Display &display, const Janela &mundo)
{
    std::vector<Poligono> outros;
    for (int i = 0; i < (int)display.poligonos.size(); i++)
        if (display.poligonos[i].tipo != 'E')
            outros.push_back(display.poligonos[i]);
    display.poligonos = outros;

    Poligono eixo;

    eixo.id = -1;
    eixo.tipo = 'E';
    eixo.pontos.push_back(Ponto(0, mundo.yMin));
    eixo.pontos.push_back(Ponto(0, mundo.yMax));
    display.poligonos.insert(display.poligonos.begin(), eixo);
    eixo.pontos.clear();

    eixo.id = -2;
    eixo.tipo = 'E';
    eixo.pontos.push_back(Ponto(mundo.xMin, 0));
    eixo.pontos.push_back(Ponto(mundo.xMax, 0));
    display.poligonos.insert(display.poligonos.begin(), eixo);
}

// ---------------------------------------------------------------------------
// Zoom In
// ---------------------------------------------------------------------------
void aplicarZoomIn(Display &display, Janela &mundo, std::stack<Janela> &historico,
                   double xMin, double yMin, double xMax, double yMax,
                   TCanvas *canvas, const Janela &vp, int tipoReta, bool clipAtivo)
{
    historico.push(mundo);
    mundo.xMin = xMin;
    mundo.yMin = yMin;
    mundo.xMax = xMax;
    mundo.yMax = yMax;
    recriaEixos(display, mundo);
    display.desenha(canvas, vp, mundo, tipoReta, clipAtivo);
}

// ---------------------------------------------------------------------------
// Zoom Out
// ---------------------------------------------------------------------------
void aplicarZoomOut(Display &display, Janela &mundo, std::stack<Janela> &historico,
                    TEdit *edXmin, TEdit *edXmax, TEdit *edYmin, TEdit *edYmax,
                    TCanvas *canvas, const Janela &vp, int tipoReta, bool clipAtivo)
{
    if (!historico.empty()) {
        mundo = historico.top();
        historico.pop();
        edXmin->Text = FloatToStr(mundo.xMin);
        edXmax->Text = FloatToStr(mundo.xMax);
        edYmin->Text = FloatToStr(mundo.yMin);
        edYmax->Text = FloatToStr(mundo.yMax);
        recriaEixos(display, mundo);
        display.desenha(canvas, vp, mundo, tipoReta, clipAtivo);
    }
}

// ---------------------------------------------------------------------------
// Curvas: Hermite (tipo=8) e Bezier (tipo=9)
// ---------------------------------------------------------------------------
bool aplicarCurva(Display &display, int idx, int tipo, int &contId)
{
    if (idx == -1 || (int)display.poligonos[idx].pontos.size() < 4)
        return false;

    Poligono pOrig = display.poligonos[idx];
    Ponto p1 = pOrig.pontos[0];
    Ponto p2 = pOrig.pontos[1];
    Ponto p3 = pOrig.pontos[2];
    Ponto p4 = pOrig.pontos[3];

    int n = 100;
    double passo = 1.0 / n;

    double ax, bx, cx, dx;
    double ay, by, cy, dy;

    if (tipo == 8) {
        double R1x = p2.x - p1.x;
        double R1y = p2.y - p1.y;
        double R4x = p4.x - p3.x;
        double R4y = p4.y - p3.y;

        ax = ( 2*p1.x) + (-2*p4.x) + R1x + R4x;
        bx = (-3*p1.x) + ( 3*p4.x) + (-2*R1x) + (-R4x);
        cx = R1x;
        dx = p1.x;

        ay = ( 2*p1.y) + (-2*p4.y) + R1y + R4y;
        by = (-3*p1.y) + ( 3*p4.y) + (-2*R1y) + (-R4y);
        cy = R1y;
        dy = p1.y;
    } else {
        ax = (-1*p1.x) + ( 3*p2.x) + (-3*p3.x) + ( 1*p4.x);
        bx = ( 3*p1.x) + (-6*p2.x) + ( 3*p3.x);
        cx = (-3*p1.x) + ( 3*p2.x);
        dx =     p1.x;

        ay = (-1*p1.y) + ( 3*p2.y) + (-3*p3.y) + ( 1*p4.y);
        by = ( 3*p1.y) + (-6*p2.y) + ( 3*p3.y);
        cy = (-3*p1.y) + ( 3*p2.y);
        dy =     p1.y;
    }

    Poligono curva;
    curva.id   = contId++;
    curva.tipo = 'C';

    for (double t = 0; t <= 1.0; t += passo) {
        double t3 = t * t * t;
        double t2 = t * t;
        double t1 = t;

        double x = (ax * t3) + (bx * t2) + (cx * t1) + dx;
        double y = (ay * t3) + (by * t2) + (cy * t1) + dy;

        curva.pontos.push_back(Ponto(x, y));
    }

    display.poligonos.push_back(curva);
    return true;
}

// ---------------------------------------------------------------------------
// Curvas por Forward Differences (Diferencas Finitas)
// ---------------------------------------------------------------------------

// Matriz base de Hermite (M_H). Geometria: [P1 P4 R1 R4]^T
static const double MH[4][4] = {
    {  2, -2,  1,  1 },
    { -3,  3, -2, -1 },
    {  0,  0,  1,  0 },
    {  1,  0,  0,  0 }
};

// Produto matriz 4x4 por vetor 4x1: r = M * v
static void multiplicaMatriz(const double M[4][4], const double v[4], double r[4])
{
    for (int i = 0; i < 4; i++) {
        r[i] = 0;
        for (int j = 0; j < 4; j++)
            r[i] += M[i][j] * v[j];
    }
}

// Equivalente ao desenheAte(x, y): em vez de tracar a reta direto no canvas,
// acrescenta o ponto a curva; o Display liga os pontos ao desenhar.
static void desenheAte(Poligono &curva, double x, double y)
{
    curva.pontos.push_back(Ponto(x, y));
}

// ---------------------------------------------------------------------------
// Nucleo do algoritmo: usa EXCLUSIVAMENTE somas no laco (sem multiplicacoes
// nem potencias). A cada passo:
//     x   <- x   + Dx
//     Dx  <- Dx  + D2x
//     D2x <- D2x + D3x     (D3x e constante para um polinomio cubico)
// e o mesmo para y.
//   n   : numero de segmentos (delta t = 1/n)
//   x,y : ponto inicial, em t = 0
//   Dx, D2x, D3x (e Dy, D2y, D3y): diferencas iniciais de 1a, 2a e 3a ordem
// ---------------------------------------------------------------------------
void DesenhaCurvaFwdDiff(Poligono &curva, int n,
                         double x, double Dx, double D2x, double D3x,
                         double y, double Dy, double D2y, double D3y)
{
    desenheAte(curva, x, y);   // ponto inicial (t = 0)

    int i = 0;
    while (i < n) {
        x += Dx;
        y += Dy;

        // Atualiza da ordem mais baixa para a mais alta, usando o valor
        // ainda nao atualizado da ordem seguinte.
        Dx  += D2x;
        D2x += D3x;
        Dy  += D2y;
        D2y += D3y;

        desenheAte(curva, x, y);
        i++;
    }
}

// ---------------------------------------------------------------------------
// Preparacao: coeficientes e deltas iniciais.
//
// 1) Coeficientes: [a b c d]^T = M * G, separadamente para x e para y, com
//    x(t) = a*t^3 + b*t^2 + c*t + d.
//
// 2) Deltas iniciais. Com t = delta t = 1/n e f(t) = a*t^3 + b*t^2 + c*t + d:
//      f(0)    = d
//      Df(0)   = f(t) - f(0)           = a*t^3 + b*t^2 + c*t
//      D2f(0)  = Df(t) - Df(0)
//              = f(2t) - 2f(t) + f(0)  = 6a*t^3 + 2b*t^2
//      D3f(0)  = D2f(t) - D2f(0)       = 6a*t^3
//    Como f e cubico, D3f e constante (diferencas de 4a ordem sao nulas), por
//    isso o laco de desenho precisa apenas de somas.
//
// 3) Chama DesenhaCurvaFwdDiff com os valores iniciais.
//
// Curva de Hermite: G = [P1 P4 R1 R4]^T, com R1 = p2-p1 e R4 = p4-p3.
// Usa os 4 primeiros pontos do poligono idx. Retorna false se tiver menos de 4.
// ---------------------------------------------------------------------------
bool ConfiguraE_DesenhaCurvaFwdDiff(Display &display, int idx,
                                    int n, int &contId)
{
    if (idx == -1 || n < 1 || (int)display.poligonos[idx].pontos.size() < 4)
        return false;

    Poligono pOrig = display.poligonos[idx];
    Ponto p1 = pOrig.pontos[0];
    Ponto p2 = pOrig.pontos[1];
    Ponto p3 = pOrig.pontos[2];
    Ponto p4 = pOrig.pontos[3];

    double gx[4] = { p1.x, p4.x, p2.x - p1.x, p4.x - p3.x };
    double gy[4] = { p1.y, p4.y, p2.y - p1.y, p4.y - p3.y };
    const double (*M)[4] = MH;

    // Passo 1: coeficientes
    double cx[4], cy[4];
    multiplicaMatriz(M, gx, cx);
    multiplicaMatriz(M, gy, cy);
    double ax = cx[0], bx = cx[1], ccx = cx[2], dx = cx[3];
    double ay = cy[0], by = cy[1], ccy = cy[2], dy = cy[3];

    // Passo 2: deltas iniciais (calculo unico, fora do laco de desenho)
    double t  = 1.0 / n;
    double t2 = t * t;
    double t3 = t2 * t;

    double Dx  = ax * t3 + bx * t2 + ccx * t;
    double D2x = 6 * ax * t3 + 2 * bx * t2;
    double D3x = 6 * ax * t3;

    double Dy  = ay * t3 + by * t2 + ccy * t;
    double D2y = 6 * ay * t3 + 2 * by * t2;
    double D3y = 6 * ay * t3;

    // Passo 3: gera a curva
    Poligono curva;
    curva.id   = contId++;
    curva.tipo = 'C';

    DesenhaCurvaFwdDiff(curva, n, dx, Dx, D2x, D3x, dy, Dy, D2y, D3y);

    display.poligonos.push_back(curva);
    return true;
}

// ---------------------------------------------------------------------------
// Cohen-Sutherland Line Clipping
//
// Referencia: Foley et al., "Computer Graphics: Principles and Practice",
//             e slides CG3 da disciplina.
//
// A janela de recorte e definida por clip.xMin, clip.xMax, clip.yMin, clip.yMax
// (coordenadas do mundo - y cresce para cima).
// ---------------------------------------------------------------------------

// Retorna o codigo de regiao de um ponto (xp, yp) em relacao a janela clip
int cohenSutherlandCodigo(double xp, double yp, const Janela &clip)
{
    int codigo = CS_INSIDE;

    if      (xp < clip.xMin) codigo |= CS_LEFT;
    else if (xp > clip.xMax) codigo |= CS_RIGHT;

    if      (yp < clip.yMin) codigo |= CS_BOTTOM;
    else if (yp > clip.yMax) codigo |= CS_TOP;

    return codigo;
}

// Retorna true  -> segmento visivel (total ou parcialmente); x1,y1,x2,y2 atualizados.
// Retorna false -> segmento totalmente fora.
bool cohenSutherlandClip(double &x1, double &y1,
                         double &x2, double &y2,
                         const Janela &clip)
{
    int cod1 = cohenSutherlandCodigo(x1, y1, clip);
    int cod2 = cohenSutherlandCodigo(x2, y2, clip);

    while (true)
    {
        if ((cod1 | cod2) == CS_INSIDE)
        {
            // Ambos dentro - aceitar trivialmente
            return true;
        }
        else if ((cod1 & cod2) != CS_INSIDE)
        {
            // Ambos do mesmo lado de uma borda - rejeitar trivialmente
            return false;
        }
        else
        {
            // Segmento cruza pelo menos uma borda - calcular intersecao
            int codExt = (cod1 != CS_INSIDE) ? cod1 : cod2;

            double x = 0.0, y = 0.0;
            double dx = x2 - x1;
            double dy = y2 - y1;

            if (codExt & CS_TOP)
            {
                x = x1 + dx * (clip.yMax - y1) / dy;
                y = clip.yMax;
            }
            else if (codExt & CS_BOTTOM)
            {
                x = x1 + dx * (clip.yMin - y1) / dy;
                y = clip.yMin;
            }
            else if (codExt & CS_RIGHT)
            {
                y = y1 + dy * (clip.xMax - x1) / dx;
                x = clip.xMax;
            }
            else if (codExt & CS_LEFT)
            {
                y = y1 + dy * (clip.xMin - x1) / dx;
                x = clip.xMin;
            }

            // Substitui o ponto externo pelo ponto de intersecao
            if (codExt == cod1)
            {
                x1   = x;
                y1   = y;
                cod1 = cohenSutherlandCodigo(x1, y1, clip);
            }
            else
            {
                x2   = x;
                y2   = y;
                cod2 = cohenSutherlandCodigo(x2, y2, clip);
            }
        }
    }
}