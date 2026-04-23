# README — ULA Combinacional

## Visão geral

Este projeto reúne três blocos lógicos principais:

* um **somador de 3 bits**
* um **complemento de 1 em 2 bits**
* um **comparador de 1 bit**

As entradas usadas no sistema são:

* **Somador:** `A0, A1, A2, B0, B1, B2`
* **Complemento:** `A, B`
* **Comparador:** `A, B`

As saídas são:

* **Somador:** `S0, S1, S2, Cout`
* **Complemento:** `~A, ~B`
* **Comparador:** `GT1 = A·~B` e `GT2 = ~A·B`

---

## 1. Somador de 3 bits

O somador de 3 bits pode ser visto como a ligação em cascata de:

* 1 meio somador no bit menos significativo
* 2 somadores completos nos bits seguintes

Na prática, ele realiza:

```text
A2 A1 A0
+ B2 B1 B0
-----------
Cout S2 S1 S0
```

### 1.1 Bit 0

Como o bit 0 não recebe carry de entrada, ele funciona como um meio somador.

#### Tabela verdade de S0 e C1

| A0 | B0 | S0 | C1 |
| -- | -- | -- | -- |
| 0  | 0  | 0  | 0  |
| 0  | 1  | 1  | 0  |
| 1  | 0  | 1  | 0  |
| 1  | 1  | 0  | 1  |

#### Mapa K de S0

`S0 = A0 ⊕ B0`

| A0 \ B0 | 0 | 1 |
| ------- | - | - |
| 0       | 0 | 1 |
| 1       | 1 | 0 |

#### Função simplificada de S0

```text
S0 = A0'B0 + A0B0'
```

#### Mapa K de C1

`C1 = A0 · B0`

| A0 \ B0 | 0 | 1 |
| ------- | - | - |
| 0       | 0 | 0 |
| 1       | 0 | 1 |

#### Função simplificada de C1

```text
C1 = A0B0
```

---

### 1.2 Bit 1

O bit 1 recebe `A1`, `B1` e o carry `C1` vindo do bit anterior.

#### Tabela verdade do somador completo

| A1 | B1 | C1 | S1 | C2 |
| -- | -- | -- | -- | -- |
| 0  | 0  | 0  | 0  | 0  |
| 0  | 0  | 1  | 1  | 0  |
| 0  | 1  | 0  | 1  | 0  |
| 0  | 1  | 1  | 0  | 1  |
| 1  | 0  | 0  | 1  | 0  |
| 1  | 0  | 1  | 0  | 1  |
| 1  | 1  | 0  | 0  | 1  |
| 1  | 1  | 1  | 1  | 1  |

#### Mapa K de S1

`S1 = A1 ⊕ B1 ⊕ C1`

| A1 \ B1C1 | 00 | 01 | 11 | 10 |
| --------- | -- | -- | -- | -- |
| 0         | 0  | 1  | 0  | 1  |
| 1         | 1  | 0  | 1  | 0  |

#### Função simplificada de S1

```text
S1 = A1 ⊕ B1 ⊕ C1
```

Forma soma de produtos:

```text
S1 = A1'B1'C1 + A1'B1C1' + A1B1'C1' + A1B1C1
```

#### Mapa K de C2

`C2 = A1B1 + A1C1 + B1C1`

| A1 \ B1C1 | 00 | 01 | 11 | 10 |
| --------- | -- | -- | -- | -- |
| 0         | 0  | 0  | 1  | 0  |
| 1         | 0  | 1  | 1  | 1  |

#### Função simplificada de C2

```text
C2 = A1B1 + A1C1 + B1C1
```

---

### 1.3 Bit 2

O bit 2 recebe `A2`, `B2` e o carry `C2`.

#### Tabela verdade do somador completo

| A2 | B2 | C2 | S2 | Cout |
| -- | -- | -- | -- | ---- |
| 0  | 0  | 0  | 0  | 0    |
| 0  | 0  | 1  | 1  | 0    |
| 0  | 1  | 0  | 1  | 0    |
| 0  | 1  | 1  | 0  | 1    |
| 1  | 0  | 0  | 1  | 0    |
| 1  | 0  | 1  | 0  | 1    |
| 1  | 1  | 0  | 0  | 1    |
| 1  | 1  | 1  | 1  | 1    |

#### Mapa K de S2

`S2 = A2 ⊕ B2 ⊕ C2`

| A2 \ B2C2 | 00 | 01 | 11 | 10 |
| --------- | -- | -- | -- | -- |
| 0         | 0  | 1  | 0  | 1  |
| 1         | 1  | 0  | 1  | 0  |

#### Função simplificada de S2

```text
S2 = A2 ⊕ B2 ⊕ C2
```

Forma soma de produtos:

```text
S2 = A2'B2'C2 + A2'B2C2' + A2B2'C2' + A2B2C2
```

#### Mapa K de Cout

`Cout = A2B2 + A2C2 + B2C2`

| A2 \ B2C2 | 00 | 01 | 11 | 10 |
| --------- | -- | -- | -- | -- |
| 0         | 0  | 0  | 1  | 0  |
| 1         | 0  | 1  | 1  | 1  |

#### Função simplificada de Cout

```text
Cout = A2B2 + A2C2 + B2C2
```

---

### 1.4 Equações finais do somador de 3 bits

```text
S0   = A0 ⊕ B0
C1   = A0B0

S1   = A1 ⊕ B1 ⊕ C1
C2   = A1B1 + A1C1 + B1C1

S2   = A2 ⊕ B2 ⊕ C2
Cout = A2B2 + A2C2 + B2C2
```

---

## 2. Complemento de 1 em 2 bits

O bloco de complemento apenas inverte cada bit de entrada.

Entradas:

* `A`
* `B`

Saídas:

* `~A`
* `~B`

### 2.1 Tabela verdade

| A | B | ~A | ~B |
| - | - | -- | -- |
| 0 | 0 | 1  | 1  |
| 0 | 1 | 1  | 0  |
| 1 | 0 | 0  | 1  |
| 1 | 1 | 0  | 0  |

### 2.2 Mapa K de ~A

| A \ B | 0 | 1 |
| ----- | - | - |
| 0     | 1 | 1 |
| 1     | 0 | 0 |

#### Função simplificada

```text
~A = A'
```

### 2.3 Mapa K de ~B

| A \ B | 0 | 1 |
| ----- | - | - |
| 0     | 1 | 0 |
| 1     | 1 | 0 |

#### Função simplificada

```text
~B = B'
```

---

## 3. Comparador de 1 bit

O comparador foi definido por duas saídas:

* `GT1 = A·~B`  → indica `A > B`
* `GT2 = ~A·B`  → indica `B > A`

Quando as duas saídas são `0`, então `A = B`.

### 3.1 Tabela verdade

| A | B | GT1 = A·~B | GT2 = ~A·B | Relação |
| - | - | ---------- | ---------- | ------- |
| 0 | 0 | 0          | 0          | A = B   |
| 0 | 1 | 0          | 1          | B > A   |
| 1 | 0 | 1          | 0          | A > B   |
| 1 | 1 | 0          | 0          | A = B   |

### 3.2 Mapa K de GT1

| A \ B | 0 | 1 |
| ----- | - | - |
| 0     | 0 | 0 |
| 1     | 1 | 0 |

#### Função simplificada

```text
GT1 = A·B'
```

### 3.3 Mapa K de GT2

| A \ B | 0 | 1 |
| ----- | - | - |
| 0     | 0 | 1 |
| 1     | 0 | 0 |

#### Função simplificada

```text
GT2 = A'·B
```

---

## 4. Resumo final das funções simplificadas

### Somador

```text
S0   = A0'B0 + A0B0'
C1   = A0B0

S1   = A1 ⊕ B1 ⊕ C1
C2   = A1B1 + A1C1 + B1C1

S2   = A2 ⊕ B2 ⊕ C2
Cout = A2B2 + A2C2 + B2C2
```

### Complemento

```text
~A = A'
~B = B'
```

### Comparador

```text
GT1 = A·B'
GT2 = A'·B
```

---

## 5. Uso dos multiplexadores no projeto

Para juntar as saídas das operações e mandar tudo para o Arduino usando as mesmas quatro entradas, foram usados **4 multiplexadores 8x1**.

A ideia é simples: como o resultado final lido pelo Arduino ocupa até 4 bits, cada MUX guarda **um bit de saída** de cada operação. Assim, dependendo do código de operação enviado pelo Arduino, os quatro MUX passam a saída correspondente daquela operação.

A organização ficou assim:

* **MUX 0:** `S0`, `GT1`, `~A`
* **MUX 1:** `S1`, `GT2`, `~B`
* **MUX 2:** `S2`, `A`
* **MUX 3:** `Cout`, `B`

No caso da soma, isso forma o resultado completo em 4 bits:

```text
MUX 0 -> S0
MUX 1 -> S1
MUX 2 -> S2
MUX 3 -> Cout
```

No caso do complemento, apenas os dois primeiros MUX são usados com informação útil:

```text
MUX 0 -> ~A
MUX 1 -> ~B
```

No caso da comparação, além de indicar a relação lógica entre as entradas, o sistema também retorna os próprios valores originais de `A` e `B`:

```text
MUX 0 -> GT1
MUX 1 -> GT2
MUX 2 -> A
MUX 3 -> B
```

Com isso, quando o Arduino envia o serial code da comparação, ele consegue ler:

* se `A > B`
* se `B > A`
* se `A = B`
* e também os valores de `A` e `B` para conferência

A interpretação funciona assim:

* `GT1 = 1` e `GT2 = 0`  → `A > B`
* `GT1 = 0` e `GT2 = 1`  → `B > A`
* `GT1 = 0` e `GT2 = 0`  → `A = B`

Essa abordagem facilita bastante o projeto, porque o Arduino sempre lê o resultado final pelas mesmas entradas, mudando apenas a operação selecionada.

---

## 6. Observação final

Ao representar o sistema como uma pequena ULA, as três operações podem ser vistas assim:

* **Somador:** retorna `S0, S1, S2, Cout`
* **Complemento:** retorna `~A, ~B`
* **Comparador:** retorna `GT1, GT2, A, B`

Isso permite selecionar a saída desejada com multiplexadores e com uma lógica de controle por opcode enviada pelo Arduino.
