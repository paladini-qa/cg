# Computação Gráfica — Editor 2D/3D (C++ Builder)

Aplicação VCL (RAD Studio / C++ Builder, `Project1.cbproj`, Win64x) para estudo de Computação Gráfica: rasterização de retas, polígonos, transformações, clipping, curvas e wireframes 3D simples.

## Funcionalidades

- Rasterização de retas (DDA / Bresenham) e circunferências
- Polígonos com seleção em lista (`lbPoligonos`)
- Transformações 2D: translação, rotação (origem e ponto arbitrário, coordenadas homogêneas), reflexão
- Janela/viewport, zoom in/out com histórico
- Clipping de retas (Cohen-Sutherland)
- Curvas de Hermite e Bézier (+ Forward Differences)
- Modelos 3D (cubo, pirâmide) carregados de `modelos/*.txt`

## Estrutura

| Arquivo | Responsabilidade |
|---|---|
| `Project1.cpp`, `Project1.cbproj` | Entrada e projeto |
| `Menu.cpp/.h/.dfm` | Formulário principal (`TForm1`), eventos de UI |
| `funcoes.cpp/.h` | Lógica de aplicação: transformações, clipping, zoom, curvas |
| `uPonto`, `uPoligono`, `uCircunferencia` | Primitivas geométricas |
| `uDisplay`, `uJanela` | Lista de objetos do display; janela mundo/viewport |
| `modelos/` | Modelos 3D em texto |
| `fonte/` | Material de aula (slides, PDFs) |

Ignorar: `__history/`, `__recovery/`, `Win64x/` (gerados pela IDE/build).

## Build

1. Abrir `Project1.cbproj` no RAD Studio / C++ Builder.
2. Build e Run (F9).

---

# Instruções para Agentes (AGENTS)

## Contexto
- Linguagem: C++ com VCL (Embarcadero). Código e comentários em **português** (sem acentos em comentários de código, seguindo o existente).
- Projeto acadêmico de CG; priorize clareza e fidelidade aos algoritmos de aula.

## Convenções
- Cabeçalhos com guardas `#ifndef xxxH` e separadores `//-----`.
- Handlers de eventos usam `__fastcall` e são declarados na seção `__published` de `Menu.h`.
- Lógica de domínio vai em `funcoes.cpp`/classes `u*.cpp`; `Menu.cpp` só lê a UI e delega.
- Preserve comentários existentes não relacionados à alteração.
- Ao adicionar um novo `.cpp`, registre-o em `Project1.cbproj`.
- Alterações em componentes visuais devem manter `Menu.dfm` e `Menu.h` consistentes.

## Algoritmos
- Curvas Forward Differences: apenas somas no laço de desenho (`DesenhaCurvaFwdDiff`); passo `delta t = 1/n`. Curvas exigem polígono com ≥ 4 pontos.
- Cohen-Sutherland: bits `[TOP|BOTTOM|RIGHT|LEFT]` = `8|4|2|1`.
- Rotação homogênea: translada para `(px,py)`, rotaciona, translada de volta.

## Restrições
- Não editar nem commitar `__history/`, `__recovery/`, `Win64x/`.
- Não modificar arquivos em `fonte/` (material de referência).
- Não há testes automatizados; valide compilando na IDE e testando visualmente.
