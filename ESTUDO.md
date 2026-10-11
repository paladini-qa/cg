# Guia de Estudo - Translação, Rotação, Circunferência e 3D

Onde está cada coisa:

| Assunto | Arquivo | Função/Método |
|---|---|---|
| Translação 2D | `uPoligono.cpp`, `uCircunferencia.h`, `funcoes.cpp` | `Poligono::translada`, `Circunferencia::translada`, `aplicarTransladar` |
| Rotação 2D (centroide) | `uPoligono.cpp` | `Poligono::rotaciona` |
| Rotação homogênea (pivô) | `uPoligono.cpp` | `Poligono::rotacionaHomogenea` |
| Circunferência | `uCircunferencia.h` | `Circunferencia::desenha` (Bresenham/ponto médio) |
| Matrizes 3D | `uPonto.cpp` | `Translacao`, `Escalonamento`, `RotacaoX/Y/Z`, `aplicaMatriz` |
| 3D no polígono | `uPoligono.cpp` | `translada3D`, `escalona`, `rotacionaX/Y/Z`, `carregaArquivo` |
| Modelos 3D | `modelos/cubo.txt`, `piramide.txt` | formato `N` + N linhas `x y z` |

Fluxo geral: `Menu.cpp` lê os campos da UI -> chama `aplicarXxx` (`funcoes.cpp`) ou o método do `Poligono` -> `display.desenha(...)` redesenha tudo.

---

## 1. Translação

Somar um deslocamento a cada coordenada:

```
x' = x + dx
y' = y + dy
```

Código (`Poligono::translada`):

```cpp
for (int i = 0; i < (int)pontos.size(); i++) {
    pontos[i].x += dx;
    pontos[i].y += dy;
}
```

- Circunferência: só o centro se move (`xc += dx; yc += dy;`). O raio não muda.
- `aplicarTransladar`: se `idx != -1` aplica no objeto selecionado; senão (`-1`) em todos.
  Eixos (`tipo == 'E'`) nunca são movidos.
- Índice: `idx < poligonos.size()` é polígono; depois disso `idx - poligonos.size()` é a circunferência.

Em matriz homogênea (forma usada no 3D, veja seção 5):

```
          | 1  0  0  0 |
[x y z 1]*| 0  1  0  0 | = [x+tx  y+ty  z+tz  1]
          | 0  0  1  0 |
          | tx ty tz 1 |
```

---

## 2. Rotação 2D (em torno do centroide)

Fórmula de rotação em torno da origem, ângulo θ (anti-horário):

```
x' = x*cosθ - y*senθ
y' = x*senθ + y*cosθ
```

`Poligono::rotaciona(angulo)` faz, em ordem:

1. Converte graus para radianos: `rad = angulo * PI / 180`.
2. Calcula o **centroide** (média de x e de y).
3. Para cada ponto: `dx = x - cx`, `dy = y - cy` (leva o centroide à origem).
4. Rotaciona `(dx, dy)`.
5. Soma o centroide de volta (`+ cx`, `+ cy`).

```cpp
pontos[i].x = dx * cosA - dy * sinA + cx;
pontos[i].y = dx * sinA + dy * cosA + cy;
```

Por que usar o centroide? Rotacionar em torno da origem faria o objeto "orbitar" a origem. Rotacionando em torno do próprio centro, ele gira no lugar.

---

## 3. Rotação homogênea (ponto arbitrário)

`Poligono::rotacionaHomogenea(angulo, px, py)` é a mesma ideia, mas o pivô `(px, py)` é escolhido pelo usuário.

Composição de 3 transformações:

```
T(-px,-py)  ->  R(θ)  ->  T(px,py)
```

```cpp
double dx = pontos[i].x - px;      // 1) translada pivo p/ origem
double dy = pontos[i].y - py;
pontos[i].x = dx*cosA - dy*sinA + px;   // 2) rotaciona e 3) volta
pontos[i].y = dx*sinA + dy*cosA + py;
```

> `rotaciona` = caso particular com pivô = centroide.
> Regra do projeto (AGENTS.md): translada para `(px,py)`, rotaciona, volta.

Exemplo: ponto `(3,1)`, pivô `(1,1)`, 90°. `dx=2, dy=0` -> `x' = 0+1 = 1`, `y' = 2+1 = 3` -> `(1,3)`.

---

## 4. Circunferência (Bresenham / ponto médio)

Dados: centro `(xc, yc)` e raio `r`, em coordenadas de **mundo**.

Passos de `Circunferencia::desenha`:

1. Converte centro e raio para a **viewport** (pixels) com `_xW2Vp`, `_yW2Vp`, `_rW2Vp`.
   O raio usa só a escala X (assume proporção 1:1).
2. Começa em `x = 0`, `y = rVp` (topo da circunferência).
3. Parâmetro de decisão inicial: `p = 1 - rVp`.
4. Enquanto `x < y` (só calcula 1 octante = 45°):
   - `p < 0`: ponto médio dentro do círculo -> `x++` (anda para a direita);
     `p += 2*x + 1`.
   - `p >= 0`: ponto médio fora -> `x++` e `y--`;
     `p += 2*(x - y) + 1`.
   - Plota os 8 pontos simétricos.

```cpp
if (p < 0) { x++; }
else       { x++; y--; }
if (p < 0) p += 2 * x + 1;
else       p += 2 * (x - y) + 1;
_plotaOctantes(canvas, xcVp, ycVp, x, y);
```

**Simetria dos 8 octantes** (`_plotaOctantes`): para um ponto `(x,y)` calculado, plota
`(±x, ±y)` e `(±y, ±x)` somados ao centro. Isso dá 8 pontos por iteração.

Observações:
- Só usa inteiros (somas e multiplicações por 2), sem `sqrt`, `sin` ou `cos`.
- `if (rVp <= 0) return;` evita desenhar raio nulo.
- Em pixels o eixo Y cresce para baixo, por isso `yc - y` é "acima" do centro.
- Circunferência não gira (rotação não se aplica); só translada.

---

## 5. Parte 3D

### 5.1 Ponto 3D e coordenadas homogêneas

`Ponto` agora tem `x, y, z`. O construtor 2D define `z = 0`.

O ponto é tratado como vetor **linha** `[x y z 1]` multiplicado por matriz 4x4
(convenção de linha: o vetor vem à esquerda, a translação fica na última linha).

```cpp
void Ponto::aplicaMatriz(const double m[4][4]){
    double v[4] = { x, y, z, 1.0 };
    double r[4] = { 0, 0, 0, 0 };
    for (int c = 0; c < 4; c++)
        for (int l = 0; l < 4; l++)
            r[c] += v[l] * m[l][c];     // r = v * m
    x = r[0]; y = r[1]; z = r[2];
}
```

Por que o `1` extra? Permite expressar a **translação** como multiplicação de matriz,
então todas as transformações podem ser compostas por multiplicação.

### 5.2 Matrizes (convenção de linha)

**Escalonamento**
```
| sx 0  0  0 |
| 0  sy 0  0 |
| 0  0  sz 0 |
| 0  0  0  1 |
```

**Rotação em X** (plano YZ) - X fica fixo
```
| 1  0   0  0 |
| 0  c   s  0 |
| 0 -s   c  0 |
| 0  0   0  1 |
```

**Rotação em Y** (plano XZ) - Y fica fixo
```
|  c  0 -s  0 |
|  0  1  0  0 |
|  s  0  c  0 |
|  0  0  0  1 |
```

**Rotação em Z** (plano XY) - igual à rotação 2D
```
|  c  s  0  0 |
| -s  c  0  0 |
|  0  0  1  0 |
|  0  0  0  1 |
```

onde `c = cos(θ)`, `s = sin(θ)`. A coluna do eixo fixo é identidade. O sinal do `-s` em Y
é invertido em relação a X e Z (regra da mão direita), pois o ciclo é X->Y->Z->X.

> Dica de estudo: ao usar a convenção de coluna (livros), as matrizes são a **transposta**
> das daqui (translação na última coluna, `s` e `-s` trocam de lugar).

### 5.3 Transformações no polígono

- `translada3D`: aplica `Translacao(dx,dy,dz)` em todos os pontos.
- `escalona`, `rotacionaX/Y/Z`: **em torno do centroide**, usando a macro
  `TRANSFORMA_NO_CENTROIDE`:

```
1) calcula centroide (cx, cy, cz)
2) para cada ponto: Translacao(-cx,-cy,-cz)   // leva ao centro
3)                  Transformacao (escala ou rotacao)
4)                  Translacao(cx,cy,cz)      // volta
```

Sem isso, escalar um objeto longe da origem também o "afastaria" da origem.

### 5.4 Carregando modelos

`Poligono::carregaArquivo` usa `fscanf`. Formato de `modelos/cubo.txt`:

```
16                 <- quantidade de pontos
-50 -50 -50        <- x y z
50 -50 -50
...
```

- Tenta o caminho e também `..\`, `..\..\`, `..\..\..\` (exe roda de `Win64x\Debug`).
- Lê tudo num vetor temporário e só então atribui a `pontos` (não corrompe em caso de erro).
- Os 16 pontos do cubo formam **uma polilinha contínua** que percorre todas as 12 arestas
  (algumas repetidas). Por isso um polígono simples (ligando ponto a ponto) desenha o cubo.
- O handler `importaModelo` (`Menu.cpp`) cria o polígono com `tipo = 'N'`.

### 5.5 Como o 3D aparece na tela

`Ponto::xW2Vp` / `yW2Vp` usam apenas `x` e `y`. Ou seja, o desenho é uma
**projeção ortográfica** sobre o plano XY (descarta `z`).

Consequências:
- `RotacaoZ` gira visivelmente o objeto.
- `RotacaoX`/`RotacaoY` alteram `z`, mas a tela só mostra o "achatamento" (cos) em y/x.
- Sem perspectiva: objetos mais distantes não ficam menores.
- Translação em `z` não muda nada visível.

---

## 6. Resumo rápido (para prova)

| Transformação | Fórmula |
|---|---|
| Translação | `x+dx`, `y+dy` |
| Escala | `x*sx`, `y*sy`, `z*sz` |
| Rotação Z | `x' = x cosθ - y senθ`, `y' = x senθ + y cosθ` |
| Rotação em torno de P | `T(-P) · R · T(P)` |
| Homogêneo | acrescenta `w=1`; translação vira multiplicação de matriz |
| Bresenham círculo | `p0 = 1 - r`; `p<0: p += 2x+1`; senão `p += 2(x-y)+1`, `y--` |

## 7. Exercícios sugeridos

1. Rotacione `(4,2)` 90° em torno de `(1,1)` (resposta: `(0,4)`).
2. Calcule à mão os pontos do círculo `r = 5` no 1º octante pelo algoritmo (`p0 = -4`).
3. Escreva a matriz que escala 2x em torno do ponto `(10,10,0)`.
4. Por que `RotacaoY` e `RotacaoX` têm sinais diferentes de `s`?
5. Altere `aplicaMatriz` para a convenção de coluna e ajuste as matrizes.
