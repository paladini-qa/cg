//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Menu.h"
#include "uJanela.h"
#include "uPonto.h"
#include "uPoligono.h"
#include "uDisplay.h"
#include "uCircunferencia.h"
#include <stack>

#include "funcoes.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;

Janela vp(0, 0, 500, 500), mundo(-250, -250, 250, 250);
Ponto aux;
Poligono pol;
Display display;
bool inicio     = false;
int  contId     = 0;
int  tipoReta   = 0;
bool clipAtivo  = false;
std::stack<Janela> historicoMundo;

//---------------------------------------------------------------------------
double xVp2Mundo(int xVp, Janela vp, Janela mundo)
{
    return (xVp - vp.xMin) / (vp.xMax - vp.xMin) * (mundo.xMax - mundo.xMin) + mundo.xMin;
}

double yVp2Mundo(int yVp, Janela vp, Janela mundo)
{
    return (1 - (yVp - vp.yMin) / (vp.yMax - vp.yMin)) * (mundo.yMax - mundo.yMin) + mundo.yMin;
}

//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
    : TForm(Owner)
{
    Image1->Canvas->Brush->Color = clWhite;
    Image1->Canvas->FillRect(Rect(0, 0, 500, 500));
    Image1->Canvas->Pen->Color = clBlack;
    Image1->Canvas->Pen->Width = 1;

    recriaEixos(display, mundo);
    display.desenha(Image1->Canvas, vp, mundo, tipoReta, clipAtivo);

    cbOperacao->ItemIndex = 0;

    // Esconde todos os grupos de parametros
    Graus->Visible       = false;
    Edit3->Visible       = false;
    RadioGroup2->Visible = false;
    LabelPx->Visible     = false;
    LabelPy->Visible     = false;
    EditPx->Visible      = false;
    EditPy->Visible      = false;
    LabelXc->Visible     = false;
    LabelYc->Visible     = false;
    LabelR->Visible      = false;
    EditXc->Visible      = false;
    EditYc->Visible      = false;
    EditR->Visible       = false;
    Label1->Visible      = false;
    Label2->Visible      = false;
    Label3->Visible      = false;
    Label4->Visible      = false;
    edXmin->Visible      = false;
    edXmax->Visible      = false;
    edYmin->Visible      = false;
    edYmax->Visible      = false;

    // Mostra campos de Transladar por padrao
    Label5->Visible = true;
    Edit1->Visible  = true;
    Label6->Visible = true;
    Edit2->Visible  = true;
}

//---------------------------------------------------------------------------
void __fastcall TForm1::Image1MouseMove(TObject *Sender, TShiftState Shift, int X, int Y)
{
    lbvp->Caption    = "(" + IntToStr(X) + "," + IntToStr(Y) + ")";
    lbmundo->Caption = "(" + FloatToStr(xVp2Mundo(X, vp, mundo)) + ","
                           + FloatToStr(yVp2Mundo(Y, vp, mundo)) + ")";
}

//---------------------------------------------------------------------------
void __fastcall TForm1::Button1Click(TObject *Sender)
{
    inicio = true;
}

//---------------------------------------------------------------------------
void __fastcall TForm1::Image1MouseDown(TObject *Sender, TMouseButton Button,
                                         TShiftState Shift, int X, int Y)
{
    if (inicio) {
        if (Button == mbLeft) {
            aux.x = xVp2Mundo(X, vp, mundo);
            aux.y = yVp2Mundo(Y, vp, mundo);
            pol.pontos.push_back(aux);
            pol.desenha(Image1->Canvas, vp, mundo, tipoReta);
            pol.mostra(ListBox1);
        }
        else if (Button == mbRight) {
            inicio   = false;
            pol.id   = contId++;
            pol.tipo = 'N';
            display.poligonos.push_back(pol);
            pol.pontos.clear();
            display.desenha(Image1->Canvas, vp, mundo, tipoReta, clipAtivo);
            display.mostra(lbPoligonos);
        }
    }
}

//---------------------------------------------------------------------------
void __fastcall TForm1::lbPoligonosClick(TObject *Sender)
{
    if (lbPoligonos->ItemIndex != -1)
        display.poligonos[lbPoligonos->ItemIndex].mostra(ListBox1);
}

//---------------------------------------------------------------------------
void __fastcall TForm1::RadioGroup1Click(TObject *Sender)
{
    tipoReta = RadioGroup1->ItemIndex;
    display.desenha(Image1->Canvas, vp, mundo, tipoReta, clipAtivo);
}

//---------------------------------------------------------------------------
void __fastcall TForm1::Button3Click(TObject *Sender)
{
    std::vector<Poligono> eixos;
    for (int i = 0; i < (int)display.poligonos.size(); i++)
        if (display.poligonos[i].tipo == 'E')
            eixos.push_back(display.poligonos[i]);
    display.poligonos = eixos;

    pol.pontos.clear();
    inicio = false;
    display.circunferencias.clear();
    ListBox1->Items->Clear();
    lbPoligonos->Items->Clear();

    clipAtivo = false;
    display.desenha(Image1->Canvas, vp, mundo, tipoReta, false);
}

//---------------------------------------------------------------------------
void __fastcall TForm1::Edit1Change(TObject *Sender)  { }
void __fastcall TForm1::Edit2Change(TObject *Sender)  { }

//---------------------------------------------------------------------------
void __fastcall TForm1::cbOperacaoChange(TObject *Sender)
{
    // Esconde tudo
    Label5->Visible = false; Edit1->Visible = false;
    Label6->Visible = false; Edit2->Visible = false;
    Graus->Visible       = false; Edit3->Visible       = false;
    RadioGroup2->Visible = false;
    LabelPx->Visible = false; EditPx->Visible = false;
    LabelPy->Visible = false; EditPy->Visible = false;
    LabelXc->Visible = false; EditXc->Visible = false;
    LabelYc->Visible = false; EditYc->Visible = false;
    LabelR->Visible  = false; EditR->Visible  = false;
    Label1->Visible = false; edXmin->Visible = false;
    Label2->Visible = false; edXmax->Visible = false;
    Label3->Visible = false; edYmin->Visible = false;
    Label4->Visible = false; edYmax->Visible = false;

    switch (cbOperacao->ItemIndex) {
        case 0: // Transladar
            Label5->Visible = true; Edit1->Visible = true;
            Label6->Visible = true; Edit2->Visible = true;
            break;
        case 1: // Rotacionar
            Graus->Visible = true; Edit3->Visible = true;
            break;
        case 2: // Rotacao Homogenea
            Graus->Visible   = true;  Edit3->Visible  = true;
            LabelPx->Visible = true;  EditPx->Visible = true;
            LabelPy->Visible = true;  EditPy->Visible = true;
            break;
        case 3: // Refletir
            RadioGroup2->Visible = true;
            break;
        case 4: // Circunferencia
            LabelXc->Visible = true; EditXc->Visible = true;
            LabelYc->Visible = true; EditYc->Visible = true;
            LabelR->Visible  = true; EditR->Visible  = true;
            break;
        case 5: // Clipping
        case 6: // Zoom In
        case 7: // Zoom Out
            Label1->Visible = true; edXmin->Visible = true;
            Label2->Visible = true; edXmax->Visible = true;
            Label3->Visible = true; edYmin->Visible = true;
            Label4->Visible = true; edYmax->Visible = true;
            break;
        case 8: // Hermite
        case 9: // Bezier
        case 10: // Hermite (Forward Differences)
            break;
    }
}

//---------------------------------------------------------------------------
void __fastcall TForm1::BtnAplicarClick(TObject *Sender)
{
    int idx = lbPoligonos->ItemIndex;

    switch (cbOperacao->ItemIndex) {
        case 0: { // Transladar
            double dx = StrToFloatDef(Edit1->Text, 0.0);
            double dy = StrToFloatDef(Edit2->Text, 0.0);
            aplicarTransladar(display, idx, dx, dy);
            break;
        }
        case 1: { // Rotacionar
            double angulo = StrToFloatDef(Edit3->Text, 0.0);
            aplicarRotacionar(display, idx, angulo);
            break;
        }
        case 2: { // Rotacao Homogenea
            double angulo = StrToFloatDef(Edit3->Text, 0.0);
            double px     = StrToFloatDef(EditPx->Text, 0.0);
            double py     = StrToFloatDef(EditPy->Text, 0.0);
            aplicarRotacaoHomogenea(display, idx, angulo, px, py);
            break;
        }
        case 3: { // Refletir
            int eixo = RadioGroup2->ItemIndex;
            if (eixo < 0) eixo = 0;
            aplicarRefletir(display, idx, eixo);
            break;
        }
        case 4: { // Circunferencia
            double xc = StrToFloatDef(EditXc->Text, 0.0);
            double yc = StrToFloatDef(EditYc->Text, 0.0);
            double r  = StrToFloatDef(EditR->Text,  50.0);
            if (r <= 0) { ShowMessage("Raio deve ser maior que zero!"); return; }
            aplicarCircunferencia(display, xc, yc, r,
                                  Image1->Canvas, vp, mundo,
                                  tipoReta, clipAtivo, lbPoligonos);
            return;
        }
        case 5: { // Clipping
            double cxMin = StrToFloatDef(edXmin->Text, mundo.xMin);
            double cxMax = StrToFloatDef(edXmax->Text, mundo.xMax);
            double cyMin = StrToFloatDef(edYmin->Text, mundo.yMin);
            double cyMax = StrToFloatDef(edYmax->Text, mundo.yMax);
            aplicarClipping(display, clipAtivo, cxMin, cxMax, cyMin, cyMax,
                            Image1->Canvas, vp, mundo, tipoReta, lbPoligonos);
            return;
        }
        case 6: { // Zoom In
            double xMin = StrToFloat(edXmin->Text);
            double yMin = StrToFloat(edYmin->Text);
            double xMax = StrToFloat(edXmax->Text);
            double yMax = StrToFloat(edYmax->Text);
            aplicarZoomIn(display, mundo, historicoMundo, xMin, yMin, xMax, yMax,
                          Image1->Canvas, vp, tipoReta, clipAtivo);
            return;
        }
        case 7: { // Zoom Out
            aplicarZoomOut(display, mundo, historicoMundo,
                           edXmin, edXmax, edYmin, edYmax,
                           Image1->Canvas, vp, tipoReta, clipAtivo);
            return;
        }
        case 8: // Hermite
        case 9: { // Bezier
            if (!aplicarCurva(display, idx, cbOperacao->ItemIndex, contId)) {
                ShowMessage("Selecione um poligono com pelo menos 4 pontos para gerar a curva!");
                return;
            }
            break;
        }
        case 10: { // Hermite (Forward Differences)
            if (!ConfiguraE_DesenhaCurvaFwdDiff(display, idx, 100, contId)) {
                ShowMessage("Selecione um poligono com pelo menos 4 pontos para gerar a curva!");
                return;
            }
            break;
        }
    }

    display.desenha(Image1->Canvas, vp, mundo, tipoReta, clipAtivo);
    if (idx != -1)
        display.poligonos[idx].mostra(ListBox1);
}
//---------------------------------------------------------------------------
