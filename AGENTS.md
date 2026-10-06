# Instruções para Agentes

## Contexto
- C++ com VCL (C++ Builder, `Project1.cbproj`). Projeto acadêmico de Computação Gráfica.
- Código e comentários em **português** (sem acentos em comentários, seguindo o existente).

## Estrutura
- `Menu.*`: formulário `TForm1` (UI/eventos). Apenas lê a UI e delega.
- `funcoes.*`: transformações, clipping, zoom, curvas.
- `uPonto`, `uPoligono`, `uCircunferencia`, `uDisplay`, `uJanela`: primitivas e display.
- `modelos/`: modelos 3D em texto. `fonte/`: material de aula (somente leitura).

## Convenções
- Guardas `#ifndef xxxH` e separadores `//-----`.
- Handlers com `__fastcall`, declarados em `__published` de `Menu.h`.
- Mantenha `Menu.dfm` e `Menu.h` consistentes; registre novos `.cpp` no `.cbproj`.
- Preserve comentários existentes não relacionados à alteração.

## Algoritmos
- Forward Differences: só somas no laço (`DesenhaCurvaFwdDiff`), `delta t = 1/n`; curvas exigem ≥ 4 pontos.
- Cohen-Sutherland: `TOP=8, BOTTOM=4, RIGHT=2, LEFT=1`.
- Rotação homogênea: translada para `(px,py)`, rotaciona, volta.

## Restrições
- Não editar/commitar `__history/`, `__recovery/`, `Win64x/`.
- Sem testes automatizados: valide compilando na IDE e visualmente.
