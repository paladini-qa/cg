object Form1: TForm1
  AlignWithMargins = True
  Left = 0
  Top = 0
  Align = alClient
  Caption = 'Form1'
  ClientHeight = 819
  ClientWidth = 990
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Segoe UI'
  Font.Style = []
  Scaled = False
  TextHeight = 15
  object lbvp: TLabel
    Left = 3
    Top = 509
    Width = 23
    Height = 15
    Caption = '(0,0)'
  end
  object lbmundo: TLabel
    Left = 480
    Top = 509
    Width = 23
    Height = 15
    Caption = '(0,0)'
  end
  object Label1: TLabel
    Left = 0
    Top = 573
    Width = 26
    Height = 15
    Caption = 'xMin'
    Visible = False
  end
  object Label2: TLabel
    Left = 190
    Top = 573
    Width = 27
    Height = 15
    Caption = 'xMax'
    Visible = False
  end
  object Label3: TLabel
    Left = 0
    Top = 621
    Width = 27
    Height = 15
    Caption = 'yMin'
    Visible = False
  end
  object Label4: TLabel
    Left = 190
    Top = 621
    Width = 28
    Height = 15
    Caption = 'yMax'
    Visible = False
  end
  object Label5: TLabel
    Left = 3
    Top = 573
    Width = 7
    Height = 15
    Margins.Left = 4
    Margins.Top = 4
    Margins.Right = 4
    Margins.Bottom = 4
    Caption = 'X'
  end
  object Label6: TLabel
    Left = 139
    Top = 573
    Width = 7
    Height = 15
    Margins.Left = 4
    Margins.Top = 4
    Margins.Right = 4
    Margins.Bottom = 4
    Caption = 'Y'
  end
  object Graus: TLabel
    Left = 1
    Top = 648
    Width = 30
    Height = 15
    Margins.Left = 4
    Margins.Top = 4
    Margins.Right = 4
    Margins.Bottom = 4
    Caption = 'Graus'
  end
  object LabelPx: TLabel
    Left = 0
    Top = 676
    Width = 12
    Height = 15
    Margins.Left = 4
    Margins.Top = 4
    Margins.Right = 4
    Margins.Bottom = 4
    Caption = 'Px'
  end
  object LabelPy: TLabel
    Left = 139
    Top = 676
    Width = 13
    Height = 15
    Margins.Left = 4
    Margins.Top = 4
    Margins.Right = 4
    Margins.Bottom = 4
    Caption = 'Py'
  end
  object LabelXc: TLabel
    Left = 0
    Top = 676
    Width = 13
    Height = 15
    Margins.Left = 4
    Margins.Top = 4
    Margins.Right = 4
    Margins.Bottom = 4
    Caption = 'Xc'
  end
  object LabelYc: TLabel
    Left = 139
    Top = 676
    Width = 12
    Height = 15
    Margins.Left = 4
    Margins.Top = 4
    Margins.Right = 4
    Margins.Bottom = 4
    Caption = 'Yc'
  end
  object LabelR: TLabel
    Left = 278
    Top = 676
    Width = 7
    Height = 15
    Margins.Left = 4
    Margins.Top = 4
    Margins.Right = 4
    Margins.Bottom = 4
    Caption = 'R'
  end
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 502
    Height = 502
    TabOrder = 0
    object Image1: TImage
      Left = 1
      Top = 1
      Width = 500
      Height = 500
      Align = alClient
      OnMouseDown = Image1MouseDown
      OnMouseMove = Image1MouseMove
      ExplicitLeft = 0
      ExplicitTop = 3
    end
  end
  object Button1: TButton
    Left = 548
    Top = 170
    Width = 121
    Height = 31
    Margins.Left = 4
    Margins.Top = 4
    Margins.Right = 4
    Margins.Bottom = 4
    Caption = 'Iniciar'
    TabOrder = 1
    OnClick = Button1Click
  end
  object ListBox1: TListBox
    Left = 548
    Top = 380
    Width = 121
    Height = 121
    Margins.Left = 4
    Margins.Top = 4
    Margins.Right = 4
    Margins.Bottom = 4
    ItemHeight = 15
    TabOrder = 2
  end
  object lbPoligonos: TListBox
    Left = 700
    Top = 380
    Width = 121
    Height = 121
    ItemHeight = 15
    TabOrder = 3
    OnClick = lbPoligonosClick
  end
  object edXmin: TEdit
    Left = 0
    Top = 591
    Width = 80
    Height = 23
    TabOrder = 4
    Text = '-250'
    Visible = False
  end
  object edXmax: TEdit
    Left = 190
    Top = 591
    Width = 80
    Height = 23
    TabOrder = 5
    Text = '250'
    Visible = False
  end
  object edYmin: TEdit
    Left = 0
    Top = 639
    Width = 80
    Height = 23
    TabOrder = 6
    Text = '-250'
    Visible = False
  end
  object edYmax: TEdit
    Left = 190
    Top = 639
    Width = 80
    Height = 23
    TabOrder = 7
    Text = '250'
    Visible = False
  end
  object RadioGroup1: TRadioGroup
    Left = 548
    Top = 0
    Width = 273
    Height = 154
    Margins.Left = 4
    Margins.Top = 4
    Margins.Right = 4
    Margins.Bottom = 4
    Caption = 'TipoDeReta'
    ItemIndex = 0
    Items.Strings = (
      'LineTo'
      'DDA'
      'Bresenham')
    TabOrder = 8
    OnClick = RadioGroup1Click
  end
  object Button3: TButton
    Left = 548
    Top = 222
    Width = 121
    Height = 31
    Margins.Left = 4
    Margins.Top = 4
    Margins.Right = 4
    Margins.Bottom = 4
    Caption = 'Limpar'
    TabOrder = 9
    OnClick = Button3Click
  end
  object Edit1: TEdit
    Left = 0
    Top = 595
    Width = 121
    Height = 23
    TabOrder = 10
    Text = '250'
    OnChange = Edit1Change
  end
  object Edit2: TEdit
    Left = 136
    Top = 595
    Width = 121
    Height = 23
    TabOrder = 11
    Text = '250'
    OnChange = Edit2Change
  end
  object cbOperacao: TComboBox
    Left = 0
    Top = 543
    Width = 380
    Height = 23
    Style = csDropDownList
    TabOrder = 12
    OnChange = cbOperacaoChange
    Items.Strings = (
      'Transladar'
      'Rotacionar'
      'Rot. Homog'#195#170'nea'
      'Refletir'
      'Desenhar Circunfer'#195#170'ncia'
      'Recortar (Cohen-Sutherland)'
      'Zoom In'
      'Zoom Out'
      'Desenhar Curva (Hermite)'
      'Desenhar Curva (B'#195#169'zier)'
      'Curva Fwd Diff (Hermite)'
      'Transladar 3D'
      'Escalonar 3D'
      'Rotacionar X'
      'Rotacionar Y'
      'Rotacionar Z')
  end
  object BtnAplicar: TButton
    Left = 272
    Top = 595
    Width = 108
    Height = 23
    Margins.Left = 4
    Margins.Top = 4
    Margins.Right = 4
    Margins.Bottom = 4
    Caption = 'Aplicar'
    TabOrder = 14
    OnClick = BtnAplicarClick
  end
  object Edit3: TEdit
    Left = 1
    Top = 648
    Width = 121
    Height = 23
    TabOrder = 13
    Text = '0'
    OnChange = Edit1Change
  end
  object RadioGroup2: TRadioGroup
    Left = 0
    Top = 668
    Width = 121
    Height = 96
    Margins.Left = 4
    Margins.Top = 4
    Margins.Right = 4
    Margins.Bottom = 4
    Caption = 'Eixo'
    ItemIndex = 0
    Items.Strings = (
      'Eixo X'
      'Eixo Y'
      'Origem'
      'y = x')
    TabOrder = 15
  end
  object EditPx: TEdit
    Left = 0
    Top = 694
    Width = 121
    Height = 23
    TabOrder = 17
    Text = '0'
  end
  object EditPy: TEdit
    Left = 139
    Top = 694
    Width = 121
    Height = 23
    TabOrder = 18
    Text = '0'
  end
  object EditXc: TEdit
    Left = 0
    Top = 694
    Width = 121
    Height = 23
    TabOrder = 16
    Text = '0'
  end
  object EditYc: TEdit
    Left = 139
    Top = 694
    Width = 121
    Height = 23
    TabOrder = 20
    Text = '0'
  end
  object LabelZ: TLabel
    Left = 390
    Top = 573
    Width = 7
    Height = 15
    Caption = 'Z'
    Visible = False
  end
  object EditZ: TEdit
    Left = 390
    Top = 595
    Width = 80
    Height = 23
    TabOrder = 21
    Text = '0'
    Visible = False
  end
  object BtnCubo: TButton
    Left = 548
    Top = 270
    Width = 121
    Height = 31
    Caption = 'Importar Cubo'
    TabOrder = 22
    OnClick = BtnCuboClick
  end
  object BtnPiramide: TButton
    Left = 548
    Top = 320
    Width = 121
    Height = 31
    Caption = 'Importar Piramide'
    TabOrder = 23
    OnClick = BtnPiramideClick
  end
  object EditR: TEdit
    Left = 278
    Top = 694
    Width = 90
    Height = 23
    TabOrder = 19
    Text = '50'
  end
end
