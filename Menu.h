//---------------------------------------------------------------------------

#ifndef MenuH
#define MenuH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ComCtrls.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	TLabel *lbvp;
	TLabel *lbmundo;
	TImage *Image1;
	TButton *Button1;
	TListBox *ListBox1;
	TListBox *lbPoligonos;
	TEdit *edXmin;
	TEdit *edXmax;
	TEdit *edYmin;
	TEdit *edYmax;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TLabel *Label4;
	TRadioGroup *RadioGroup1;
	TButton *Button3;

	TEdit *Edit1;
	TLabel *Label5;
	TEdit *Edit2;
	TLabel *Label6;
	TComboBox *cbOperacao;
	TButton *BtnAplicar;
	// Parâmetros de transformação (visibilidade controlada em runtime)
	TLabel *Graus;
	TEdit *Edit3;
	TRadioGroup *RadioGroup2;
	TLabel *LabelPx;
	TLabel *LabelPy;
	TEdit *EditPx;
	TEdit *EditPy;
	TLabel *LabelXc;
	TLabel *LabelYc;
	TLabel *LabelR;
	TEdit *EditXc;
	TEdit *EditYc;
	TEdit *EditR;

	void __fastcall Image1MouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall Image1MouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall lbPoligonosClick(TObject *Sender);
	void __fastcall RadioGroup1Click(TObject *Sender);
	void __fastcall Button3Click(TObject *Sender);
	void __fastcall Edit2Change(TObject *Sender);
	void __fastcall Edit1Change(TObject *Sender);
	void __fastcall cbOperacaoChange(TObject *Sender);
	void __fastcall BtnAplicarClick(TObject *Sender);



private:	// User declarations
public:		// User declarations
	__fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
