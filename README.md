# DOCUMENTAÇÃO — ULA Combinacional

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

## 6. Observação

Ao representar o sistema como uma pequena ULA, as três operações podem ser vistas assim:

* **Somador:** retorna `S0, S1, S2, Cout`
* **Complemento:** retorna `~A, ~B`
* **Comparador:** retorna `GT1, GT2, A, B`

Isso permite selecionar a saída desejada com multiplexadores e com uma lógica de controle por opcode enviada pelo Arduino.

hardware digital e usando software apenas para seleção, leitura e exibição dos resultados.

---
## 7. Papel do Arduino no sistema

No projeto, o **Arduino Mega 2560** funciona como a parte de controle, leitura e exibição da ULA.

A lógica principal das operações continua sendo feita no hardware digital, usando o somador, o complemento, o comparador e os multiplexadores. O Arduino entra para:

* enviar o **opcode** da operação para os MUX
* ler os **4 bits finais** vindos dos MUX
* interpretar esses bits dependendo da operação escolhida
* mostrar o resultado em **um display de 7 segmentos em hexadecimal**
* mostrar detalhes no **Monitor Serial**

A ideia geral é:

1. o usuário envia uma sequência binária pelo Serial
2. o Arduino separa essa sequência em opcodes de 2 bits
3. o Arduino envia o opcode para os pinos `OP0` e `OP1`
4. os MUX selecionam as saídas corretas da operação
5. o Arduino lê os pinos `22, 23, 24 e 25`
6. o resultado é convertido e mostrado no display hexadecimal

---

## 8. OpCodes usados

A seleção da operação é feita por 2 bits:

```text
00 -> Soma
01 -> Comparação
10 -> Complemento
11 -> Reservado
```

Esses dois bits são enviados pelo Arduino para os MUX através dos pinos:

| Sinal | Pino no Arduino | Função                            |
| ----- | --------------- | --------------------------------- |
| OP0   | 2               | bit menos significativo do opcode |
| OP1   | 3               | bit mais significativo do opcode  |

---

## 9. Leitura das saídas dos MUX

O Arduino lê sempre os mesmos quatro pinos:

| Pino Arduino | Bit lido | Significado geral   |
| ------------ | -------- | ------------------- |
| 22           | bit 0    | menos significativo |
| 23           | bit 1    | bit intermediário   |
| 24           | bit 2    | bit intermediário   |
| 25           | bit 3    | mais significativo  |

A montagem do valor é feita assim:

```text
valor = bit0 + 2·bit1 + 4·bit2 + 8·bit3
```

Ou seja:

```text
valor = pino22·1 + pino23·2 + pino24·4 + pino25·8
```

---

## 10. Organização dos MUX

Foram usados **4 multiplexadores 8x1**, um para cada bit da saída final.

A organização ficou assim:

| MUX  | Soma | Comparação | Complemento |
| ---- | ---- | ---------- | ----------- |
| MUX0 | S0   | GT1        | ~A          |
| MUX1 | S1   | GT2        | ~B          |
| MUX2 | S2   | A          | —           |
| MUX3 | Cout | B          | —           |

Com isso, cada operação usa os mesmos quatro pinos de leitura do Arduino, mas com significados diferentes.

### 10.1 Soma

Na soma, os quatro MUX formam o resultado completo em 4 bits:

```text
[MUX0, MUX1, MUX2, MUX3] = [S0, S1, S2, Cout]
```

O Arduino lê esses quatro bits e mostra o resultado em hexadecimal no display.

Exemplo:

```text
0011 -> 3
1010 -> A
1111 -> F
```

### 10.2 Comparação

Na comparação, os quatro bits lidos são:

```text
[MUX0, MUX1, MUX2, MUX3] = [GT1, GT2, A, B]
```

A interpretação é:

```text
GT1 = 1 e GT2 = 0 -> A > B
GT1 = 0 e GT2 = 1 -> B > A
GT1 = 0 e GT2 = 0 -> A = B
```

No display, a comparação foi configurada assim:

| Condição | Valor mostrado no display |
| -------- | ------------------------- |
| A > B    | A                         |
| B > A    | b                         |
| A = B    | 0                         |

Foi usado **b minúsculo** porque em display de 7 segmentos a letra `B` maiúscula não fica bem representada. O padrão mais legível é usar `b`.

### 10.3 Complemento

No complemento, os bits que chegam ao Arduino **já são o próprio complemento**.

Então o código não inverte mais os bits.

Ele apenas lê:

```text
MUX0 -> ~A
MUX1 -> ~B
```

E transforma isso em decimal/hexadecimal para mostrar no display.

Exemplo:

```text
~B~A = 00 -> 0
~B~A = 01 -> 1
~B~A = 10 -> 2
~B~A = 11 -> 3
```

---

## 11. Display de 7 segmentos em hexadecimal

No começo do projeto, foram considerados dois displays para mostrar dezena e unidade. Depois, o sistema foi simplificado para usar **apenas um display de 7 segmentos**.

Como a saída final tem 4 bits, o maior valor possível é:

```text
1111₂ = 15₁₀ = F₁₆
```

Por isso, um único display hexadecimal é suficiente para representar qualquer saída de 4 bits.

O display mostra:

```text
0, 1, 2, 3, 4, 5, 6, 7, 8, 9, A, b, C, d, E, F
```

### 11.1 Pinos dos segmentos

O mapeamento final dos segmentos ficou:

| Segmento | Pino Arduino |
| -------- | ------------ |
| A        | 40           |
| B        | 41           |
| C        | 42           |
| D        | 43           |
| E        | 44           |
| F        | 45           |
| G        | 46           |


Com essa configuração, os números passaram a aparecer corretamente no display.

### 11.2 Pino comum do display

Como está sendo usado apenas um dígito do display, foi usado um único pino comum:

```text
DIG_HEX = 47
```

Se for usado o outro lado de um display duplo, basta trocar para outro pino comum, por exemplo:

```text
DIG_HEX = 48
```

### 11.3 Tipo do display

O display usado foi tratado como **ânodo comum**.

Por isso:

* o pino comum do display é ativado em nível alto
* os segmentos acendem em nível baixo

No código, isso ficou separado em duas configurações:

```cpp
const bool COMMON_ANODE = true;
const bool DIGIT_ACTIVE_HIGH = true;
```

---

## 12. Código final do Arduino

```cpp
// ULA com 1 display em hexadecimal

const int OP0 = 2;
const int OP1 = 3;

const int IN0 = 22;
const int IN1 = 23;
const int IN2 = 24;
const int IN3 = 25;

const bool COMMON_ANODE = true;
const bool DIGIT_ACTIVE_HIGH = true;

const int SEG_A = 40;
const int SEG_B = 41;
const int SEG_C = 42;
const int SEG_D = 43;
const int SEG_E = 44;
const int SEG_F = 45;
const int SEG_G = 46;

const int DIG_HEX = 47;

const int MAX_OPCODES = 64;
uint8_t programOpcodes[MAX_OPCODES];
int programLength = 0;
int pc = 0;

String lineBuffer = "";
int currentValueToDisplay = 0;

struct MuxData {
  int b0;
  int b1;
  int b2;
  int b3;
};

// ordem: a b c d e f g
const bool hexMap[16][7] = {
  {1,1,1,1,1,1,0}, // 0
  {0,1,1,0,0,0,0}, // 1
  {1,1,0,1,1,0,1}, // 2
  {1,1,1,1,0,0,1}, // 3
  {0,1,1,0,0,1,1}, // 4
  {1,0,1,1,0,1,1}, // 5
  {1,0,1,1,1,1,1}, // 6
  {1,1,1,0,0,0,0}, // 7
  {1,1,1,1,1,1,1}, // 8
  {1,1,1,1,0,1,1}, // 9
  {1,1,1,0,1,1,1}, // A
  {0,0,1,1,1,1,1}, // b
  {1,0,0,1,1,1,0}, // C
  {0,1,1,1,1,0,1}, // d
  {1,0,0,1,1,1,1}, // E
  {1,0,0,0,1,1,1}  // F
};

void writeOpcode(uint8_t opcode) {
  digitalWrite(OP0, opcode & 0x01);
  digitalWrite(OP1, (opcode >> 1) & 0x01);
}

MuxData readInputs() {
  MuxData data;
  data.b0 = digitalRead(IN0);
  data.b1 = digitalRead(IN1);
  data.b2 = digitalRead(IN2);
  data.b3 = digitalRead(IN3);
  return data;
}

int readNibble() {
  MuxData data = readInputs();
  return (data.b0 << 0) | (data.b1 << 1) | (data.b2 << 2) | (data.b3 << 3);
}

void printInputBits() {
  MuxData data = readInputs();

  Serial.print("Bits lidos 22 23 24 25 -> ");
  Serial.print(data.b0);
  Serial.print(" ");
  Serial.print(data.b1);
  Serial.print(" ");
  Serial.print(data.b2);
  Serial.print(" ");
  Serial.println(data.b3);

  Serial.print("Binario [25..22] = ");
  Serial.print(data.b3);
  Serial.print(data.b2);
  Serial.print(data.b1);
  Serial.println(data.b0);
}

int readSumValue() {
  return readNibble();
}

int readComplementValue() {
  MuxData data = readInputs();

  int y0 = data.b0;
  int y1 = data.b1;

  Serial.print("Complemento recebido bruto: ");
  Serial.print(y1);
  Serial.println(y0);

  return (y0 << 0) | (y1 << 1);
}

int readComparisonDisplayValue() {
  MuxData data = readInputs();

  int gt1 = data.b0;
  int gt2 = data.b1;
  int a   = data.b2;
  int b   = data.b3;

  Serial.print("Comparacao -> GT1=");
  Serial.print(gt1);
  Serial.print(" GT2=");
  Serial.print(gt2);
  Serial.print(" A=");
  Serial.print(a);
  Serial.print(" B=");
  Serial.print(b);
  Serial.print(" | Relacao: ");

  if (gt1 == 1 && gt2 == 0) {
    Serial.println("A > B");
    return 10;
  }

  if (gt1 == 0 && gt2 == 1) {
    Serial.println("B > A");
    return 11;
  }

  if (gt1 == 0 && gt2 == 0) {
    Serial.println("A = B");
    return 0;
  }

  Serial.println("estado invalido");
  return 15;
}

void writeSegmentPin(int pin, bool on) {
  if (COMMON_ANODE) {
    digitalWrite(pin, on ? LOW : HIGH);
  } else {
    digitalWrite(pin, on ? HIGH : LOW);
  }
}

void setSegmentsForHex(int value) {
  if (value < 0) value = 0;
  if (value > 15) value = 15;

  writeSegmentPin(SEG_A, hexMap[value][0]);
  writeSegmentPin(SEG_B, hexMap[value][1]);
  writeSegmentPin(SEG_C, hexMap[value][2]);
  writeSegmentPin(SEG_D, hexMap[value][3]);
  writeSegmentPin(SEG_E, hexMap[value][4]);
  writeSegmentPin(SEG_F, hexMap[value][5]);
  writeSegmentPin(SEG_G, hexMap[value][6]);
}

void enableDigit(bool on) {
  if (DIGIT_ACTIVE_HIGH) {
    digitalWrite(DIG_HEX, on ? HIGH : LOW);
  } else {
    digitalWrite(DIG_HEX, on ? LOW : HIGH);
  }
}

void turnOffAllSegments() {
  writeSegmentPin(SEG_A, false);
  writeSegmentPin(SEG_B, false);
  writeSegmentPin(SEG_C, false);
  writeSegmentPin(SEG_D, false);
  writeSegmentPin(SEG_E, false);
  writeSegmentPin(SEG_F, false);
  writeSegmentPin(SEG_G, false);
}

void refreshDisplay() {
  int value = currentValueToDisplay;
  if (value < 0) value = 0;
  if (value > 15) value = 15;

  setSegmentsForHex(value);
  enableDigit(true);
}

bool isBinaryString(const String& s) {
  if (s.length() == 0) return false;

  for (unsigned int i = 0; i < s.length(); i++) {
    if (s[i] != '0' && s[i] != '1') return false;
  }

  return true;
}

void clearProgram() {
  programLength = 0;
  pc = 0;
}

void parseProgramBits(const String& bits) {
  clearProgram();

  int usable = bits.length() - (bits.length() % 2);

  for (int i = 0; i < usable && programLength < MAX_OPCODES; i += 2) {
    char b1 = bits[i];
    char b0 = bits[i + 1];

    uint8_t opcode = ((b1 == '1') ? 2 : 0) | ((b0 == '1') ? 1 : 0);
    programOpcodes[programLength++] = opcode;
  }
}

void showProgram() {
  Serial.print("Programa carregado: ");

  if (programLength == 0) {
    Serial.println("(vazio)");
    return;
  }

  for (int i = 0; i < programLength; i++) {
    Serial.print((programOpcodes[i] >> 1) & 1);
    Serial.print(programOpcodes[i] & 1);
    Serial.print(' ');
  }

  Serial.println();
}

void executeNextOpcode() {
  if (programLength == 0) {
    Serial.println("Nenhum programa carregado.");
    return;
  }

  if (pc >= programLength) {
    Serial.println("Fim do programa.");
    return;
  }

  uint8_t opcode = programOpcodes[pc++];
  writeOpcode(opcode);

  delay(20);

  switch (opcode) {
    case 0b00: {
      printInputBits();

      int value = readSumValue();
      currentValueToDisplay = value;

      Serial.print("OP 00 - SOMA -> decimal ");
      Serial.print(value);
      Serial.print(" | hex ");
      Serial.println(value, HEX);
      break;
    }

    case 0b01: {
      printInputBits();

      int value = readComparisonDisplayValue();
      currentValueToDisplay = value;

      Serial.print("OP 01 - COMPARACAO -> display ");
      if (value == 10) {
        Serial.println("A");
      } else if (value == 11) {
        Serial.println("b");
      } else if (value == 0) {
        Serial.println("0");
      } else {
        Serial.println("F");
      }
      break;
    }

    case 0b10: {
      printInputBits();

      int value = readComplementValue();
      currentValueToDisplay = value;

      Serial.print("OP 10 - COMPLEMENTO -> decimal ");
      Serial.print(value);
      Serial.print(" | hex ");
      Serial.println(value, HEX);
      break;
    }

    case 0b11: {
      printInputBits();

      currentValueToDisplay = 0;
      Serial.println("OP 11 - RESERVADO -> 0");
      break;
    }
  }
}

void processLine(String cmd) {
  cmd.trim();

  if (cmd.length() == 0) return;

  if (cmd == "*") {
    executeNextOpcode();
    return;
  }

  if (cmd.equalsIgnoreCase("reset")) {
    clearProgram();
    writeOpcode(0);
    currentValueToDisplay = 0;
    Serial.println("Programa apagado.");
    return;
  }

  if (cmd.equalsIgnoreCase("show")) {
    showProgram();
    return;
  }

  if (isBinaryString(cmd)) {
    parseProgramBits(cmd);
    Serial.print("Programa carregado com ");
    Serial.print(programLength);
    Serial.println(" opcode(s).");
    showProgram();
    return;
  }

  Serial.println("Comando invalido.");
}

void setup() {
  Serial.begin(9600);

  pinMode(OP0, OUTPUT);
  pinMode(OP1, OUTPUT);

  pinMode(IN0, INPUT);
  pinMode(IN1, INPUT);
  pinMode(IN2, INPUT);
  pinMode(IN3, INPUT);

  pinMode(SEG_A, OUTPUT);
  pinMode(SEG_B, OUTPUT);
  pinMode(SEG_C, OUTPUT);
  pinMode(SEG_D, OUTPUT);
  pinMode(SEG_E, OUTPUT);
  pinMode(SEG_F, OUTPUT);
  pinMode(SEG_G, OUTPUT);

  pinMode(DIG_HEX, OUTPUT);

  writeOpcode(0);
  currentValueToDisplay = 0;

  enableDigit(false);
  turnOffAllSegments();

  Serial.println("=== ULA combinacional - Mega 2560 ===");
  Serial.println("Usando 1 display em hexadecimal.");
  Serial.println("Comparacao: A>B mostra A, B>A mostra b, iguais mostra 0.");
  Serial.println("Complemento: le os bits brutos e converte direto.");
  Serial.println("Lendo apenas 22, 23, 24 e 25.");
  Serial.println("Envie algo como 000110");
  Serial.println("Isso vira: [00] [01] [10]");
  Serial.println("Depois envie * para executar uma por vez.");
  Serial.println("Comandos:");
  Serial.println("*      -> executa a proxima instrucao");
  Serial.println("show   -> mostra o programa");
  Serial.println("reset  -> limpa tudo");
}

void loop() {
  refreshDisplay();

  while (Serial.available() > 0) {
    char c = (char)Serial.read();

    if (c == '\n' || c == '\r') {
      if (lineBuffer.length() > 0) {
        processLine(lineBuffer);
        lineBuffer = "";
      }
    } else {
      lineBuffer += c;
    }
  }
}
```

---

## 13. Explicação do código

### 13.1 Escrita do opcode

A função `writeOpcode()` envia o opcode para os pinos `OP0` e `OP1`.

Esses dois sinais vão para todos os MUX e escolhem qual operação será exibida na saída.

### 13.2 Leitura dos bits finais

A função `readInputs()` lê os pinos `22, 23, 24 e 25`.

A função `readNibble()` transforma esses quatro bits em um valor numérico de 0 a 15.

### 13.3 Soma

Na soma, o Arduino apenas lê os quatro bits e mostra o valor hexadecimal correspondente.

```text
Soma -> [S0, S1, S2, Cout]
```

### 13.4 Comparação

Na comparação, o Arduino lê `GT1`, `GT2`, `A` e `B`.

Depois mostra no display:

```text
A > B -> A
B > A -> b
A = B -> 0
```

### 13.5 Complemento

No complemento, os bits recebidos já são os bits complementados.

Por isso, o código não usa `!` para inverter.

Ele apenas faz:

```text
valor = ~A + 2·~B
```

E mostra esse valor em hexadecimal.

### 13.6 Display hexadecimal

A tabela `hexMap` define quais segmentos acendem para cada valor.

A ordem usada é:

```text
A, B, C, D, E, F, G
```

Como o display é de ânodo comum, a função `writeSegmentPin()` inverte a lógica automaticamente:

```text
segmento ligado -> LOW
segmento desligado -> HIGH
```

---

## 14. Resultado

Com as mudanças feitas, o sistema ficou assim:

* o Arduino lê apenas os pinos `22, 23, 24 e 25`
* a saída é exibida em apenas **um display de 7 segmentos**
* o valor é mostrado em **hexadecimal**
* a soma pode mostrar de `0` até `F`
* a comparação mostra `A`, `b` ou `0`
* o complemento usa os bits brutos já complementados

# Implementação com CIs e Relatório de Testes

## 15. Lista de CIs e componentes

A tabela abaixo resume os principais componentes usados na montagem final do projeto.

| Quantidade | Componente             | Função no projeto                                                                   |
| ---------: | ---------------------- | ----------------------------------------------------------------------------------- |
|          1 | Arduino Mega 2560      | Controle dos opcodes, leitura dos MUX, alimentação `5V`, `GND` e pinagem do display |
|          1 | Somador de 4 bits      | Bloco somador usado para realizar a soma binária de 3 bits                          |
|          4 | MUX 8x1                | Seleção das saídas de cada operação para leitura no Arduino                         |
|          2 | NOT                    | Inversão de sinais no complemento e no comparador                                   |
|          1 | AND                    | Implementação das expressões do comparador e habilitação lógica dos sinais          |
|          1 | Display de 7 segmentos | Exibição do resultado em hexadecimal                                                |
|         10 | LEDs                   | Indicação visual das entradas: 6 do somador, 2 do comparador e 2 do complemento     |
|         17 | Resistores de 330Ω     | Limitação de corrente dos LEDs e do display de 7 segmentos                          |
|     Vários | Jumpers                | Conexões entre Arduino, protoboard, CIs, MUX e display                              |
|          1 | Protoboard             | Montagem física do circuito                                                         |

O Arduino Mega foi usado também como ponto de referência para:

* `GND`
* `VCC = 5V`
* pinos digitais de controle
* pinos digitais de leitura
* pinos digitais do display de 7 segmentos

---

## 16. Visão geral da implementação

O projeto funciona como uma pequena ULA combinacional controlada pelo Arduino.

As operações disponíveis são:

```text
00 -> Soma
01 -> Comparação
10 -> Complemento
11 -> Reservado
```

O Arduino envia o opcode pelos pinos:

```text
OP0 -> pino 2
OP1 -> pino 3
```

Esses sinais selecionam qual operação será enviada pelos MUX para os pinos de leitura do Arduino.

O Arduino lê sempre os mesmos quatro pinos:

```text
pino 22 -> bit 0
pino 23 -> bit 1
pino 24 -> bit 2
pino 25 -> bit 3
```

A montagem do valor lido é feita assim:

```text
valor = bit0 + 2·bit1 + 4·bit2 + 8·bit3
```

---

## 17. Entradas externas — chaves e LEDs

As entradas dos operandos são feitas manualmente por chaves ou push-buttons.

Cada entrada possui:

* uma chave ligada ao `+5V`
* um resistor de pull-down para manter nível lógico `0` quando a chave não está acionada
* um LED de indicação visual
* um resistor de `330Ω` em série com o LED

Esquema básico de cada entrada:

```text
+5V
 |
[Chave]
 |
 |---- fio de entrada do circuito
 |
 |---- [LED] ---- [330Ω] ---- GND
 |
[pull-down]
 |
GND
```

### 17.1 Resumo das entradas

| Bloco       | Entradas               | Total de chaves | LEDs |
| ----------- | ---------------------- | --------------- | ---- |
| Somador     | A0, A1, A2, B0, B1, B2 | 6               | 6    |
| Comparador  | A, B                   | 2               | 2    |
| Complemento | X0, X1                 | 2               | 2    |
| Total       | —                      | 10              | 10   |

---

## 18. Implementação do somador

O somador foi implementado usando um **bloco somador de 4 bits**.

Como o projeto soma dois números de **3 bits**, apenas três bits do somador são utilizados. O quarto bit é mantido em nível lógico `0`.


### 18.1 Estratégia de uso

O somador de 4 bits recebe dois operandos:

```text
A3 A2 A1 A0
B3 B2 B1 B0
```

No projeto, usamos apenas 3 bits úteis:

```text
A0, A1, A2
B0, B1, B2
```

O quarto bit do somador fica fixado em `0`.

Mapeamento usado:

| Entrada do somador de 4 bits | Sinal do projeto | Observação        |
| ---------------------------- | ---------------- | ----------------- |
| A1                           | A0               | bit 0 de A        |
| A2                           | A1               | bit 1 de A        |
| A3                           | A2               | bit 2 de A        |
| A4                           | GND              | bit não usado     |
| B1                           | B0               | bit 0 de B        |
| B2                           | B1               | bit 1 de B        |
| B3                           | B2               | bit 2 de B        |
| B4                           | GND              | bit não usado     |
| C0 / Carry-in                | GND              | sem carry inicial |

A saída usada é:

| Saída do somador | Função no projeto |
| ---------------- | ----------------- |
| S1               | S0                |
| S2               | S1                |
| S3               | S2                |
| S4 / Cout        | Cout              |

### 18.2 Bloco lógico do somador

```text
A0 ----┐
A1 ----┤
A2 ----┤
       │
       │      ┌────────────────────┐
       ├─────▶│                    │────▶ S0 ---- MUX0
       │      │   SOMADOR 4 BITS   │────▶ S1 ---- MUX1
       ├─────▶│                    │────▶ S2 ---- MUX2
       │      │                    │────▶ Cout -- MUX3
B0 ----┤      └────────────────────┘
B1 ----┤
B2 ----┘

A4 = 0
B4 = 0
Carry-in = 0
```

### 18.3 Código lógico do bloco somador

A ideia do somador pode ser representada de forma abstrata assim:

```text
resultado = A + B
```

Onde:

```text
A = A0 + 2·A1 + 4·A2
B = B0 + 2·B1 + 4·B2
```

E o resultado final possui 4 bits:

```text
resultado = Cout S2 S1 S0
```

No Arduino, esses bits chegam pelos MUX e são interpretados assim:

```text
S0   -> pino 22
S1   -> pino 23
S2   -> pino 24
Cout -> pino 25
```

O valor exibido no display hexadecimal é montado por:

```text
valor = S0 + 2·S1 + 4·S2 + 8·Cout
```

### 18.4 Conexões do somador

| Sinal | Origem           | Destino |
| ----- | ---------------- | ------- |
| S0    | saída do somador | MUX0    |
| S1    | saída do somador | MUX1    |
| S2    | saída do somador | MUX2    |
| Cout  | saída do somador | MUX3    |

---

## 19. Implementação do comparador

O comparador usa dois sinais de entrada:

```text
A
B
```

Ele gera duas saídas:

```text
GT1 = A · B'
GT2 = A' · B
```

Onde:

```text
GT1 = 1 -> A > B
GT2 = 1 -> B > A
GT1 = 0 e GT2 = 0 -> A = B
```

### 19.1 Lógica

```text
GT1 = A AND NOT(B)
GT2 = NOT(A) AND B
```

### 19.2 Implementação com CIs

```text
B ---- NOT ---- B'

A ----┐
      ├── AND ---- GT1 ---- MUX0
B' ---┘
```

```text
A ---- NOT ---- A'

A' ---┐
      ├── AND ---- GT2 ---- MUX1
B ----┘
```

Os valores originais `A` e `B` também são enviados aos MUX para conferência pelo Arduino:

```text
A ---- MUX2
B ---- MUX3
```

### 19.3 Conexões do comparador

| Componente | Entrada 1 | Entrada 2 | Saída |
| ---------- | --------- | --------- | ----- |
| NOT 1      | B         | —         | B'    |
| NOT 2      | A         | —         | A'    |
| AND 1      | A         | B'        | GT1   |
| AND 2      | A'        | B         | GT2   |

### 19.4 Saída no display

Na versão final, o Arduino interpreta `GT1` e `GT2` e mostra:

| Condição | Display |
| -------- | ------- |
| A > B    | A       |
| B > A    | b       |
| A = B    | 0       |

Foi usado `b` minúsculo porque é a forma mais legível de representar B em um display de 7 segmentos.

---

## 20. Implementação do complemento

O complemento inverte dois bits de entrada:

```text
X0
X1
```

As saídas são:

```text
Y0 = NOT(X0)
Y1 = NOT(X1)
```

### 20.1 Lógica

```text
Y0 = X0'
Y1 = X1'
```

### 20.2 Implementação com CIs

```text
X0 ---- NOT ---- Y0 ---- MUX0
X1 ---- NOT ---- Y1 ---- MUX1
```

No projeto final, o Arduino considera que os bits recebidos nessa operação **já são o complemento**.

Então o código não inverte novamente esses bits.

Ele apenas lê os bits brutos:

```text
Y0 -> pino 22
Y1 -> pino 23
```

E monta:

```text
valor = Y0 + 2·Y1
```

### 20.3 Conexões do complemento

| Componente | Entrada | Saída    | Destino |
| ---------- | ------- | -------- | ------- |
| NOT 1      | X0      | Y0 = X0' | MUX0    |
| NOT 2      | X1      | Y1 = X1' | MUX1    |

---

## 21. Implementação dos multiplexadores

Foram usados **4 MUX 8x1**.

Cada MUX representa um bit da saída final.

O Arduino lê sempre:

```text
MUX0 -> pino 22
MUX1 -> pino 23
MUX2 -> pino 24
MUX3 -> pino 25
```

### 21.1 Organização dos sinais nos MUX

| MUX  | Operação 00 — Soma | Operação 01 — Comparação | Operação 10 — Complemento | Saída para Arduino |
| ---- | ------------------ | ------------------------ | ------------------------- | ------------------ |
| MUX0 | S0                 | GT1                      | Y0 = X0'                  | pino 22            |
| MUX1 | S1                 | GT2                      | Y1 = X1'                  | pino 23            |
| MUX2 | S2                 | A                        | 0                         | pino 24            |
| MUX3 | Cout               | B                        | 0                         | pino 25            |

### 21.2 Seleção dos MUX

Os MUX usam os sinais de seleção vindos do Arduino:

```text
OP1 OP0
```

Tabela de seleção:

| OP1 | OP0 | Operação selecionada |
| --- | --- | -------------------- |
| 0   | 0   | Soma                 |
| 0   | 1   | Comparação           |
| 1   | 0   | Complemento          |
| 1   | 1   | Reservado            |

Como o MUX é 8x1 e o projeto usa apenas quatro opções, a terceira linha de seleção pode ficar fixa em `0`.

---

## 22. Implementação do display hexadecimal

A versão final usa apenas **um display de 7 segmentos**.

Como a saída lida pelo Arduino possui 4 bits, o maior valor possível é:

```text
1111₂ = 15₁₀ = F₁₆
```

Por isso, um único display hexadecimal representa todos os resultados possíveis.

### 22.1 Mapeamento final dos segmentos

| Segmento | Pino Arduino |
| -------- | ------------ |
| A        | 40           |
| B        | 41           |
| C        | 42           |
| D        | 43           |
| E        | 44           |
| F        | 45           |
| G        | 46           |


### 22.2 Display usado

O display foi tratado como **ânodo comum**.

No código:

```cpp
const bool COMMON_ANODE = true;
const bool DIGIT_ACTIVE_HIGH = true;
```

Isso significa:

```text
segmento ligado    -> LOW
segmento desligado -> HIGH
pino comum ligado  -> HIGH
```

---

## 23. Relatório de testes

A seguir estão os testes principais realizados para validar a montagem.

---

## T01 — Seleção de operação

Objetivo: verificar se o opcode enviado pelo Arduino seleciona corretamente a operação nos MUX.

| Opcode | Operação esperada | Saídas selecionadas |
| ------ | ----------------- | ------------------- |
| 00     | Soma              | S0, S1, S2, Cout    |
| 01     | Comparação        | GT1, GT2, A, B      |
| 10     | Complemento       | Y0, Y1, 0, 0        |
| 11     | Reservado         | 0                   |

Status:

```text
T01 -> Aprovado
```

---

## T02 — Soma binária

Objetivo: testar o somador de 3 bits usando o bloco somador de 4 bits.

### Casos testado

```text
A = 101₂ = 5₁₀
B = 011₂ = 3₁₀
Esperado: 5 + 3 = 8
```
```text
A = 111₂ = 7₁₀
B = 111₂ = 7₁₀
Esperado: 7 + 7 = 14
```

### Resultados

```text
Cout S2 S1 S0 = 1000
Display = 8
```
```text
Cout S2 S1 S0 = 1110
Display = E
```

### Leitura pelos MUX

```text
pino 22 -> S0 = 0
pino 23 -> S1 = 0
pino 24 -> S2 = 0
pino 25 -> Cout = 1
```
```text
pino 22 -> S0 = 0
pino 23 -> S1 = 1
pino 24 -> S2 = 1
pino 25 -> Cout = 1
```

O valor montado pelo Arduino é:

```text
[25..22] = 1000₂ = 8₁₀ = 8₁₆
```
```text
[25..22] = 1110₂ = 14₁₀ = E₁₆
```

| Campo             | Valor1   | Valor2    |
| ----------------- | -------- | --------  |
| Opcode            | 00       | 00        |  
| Entrada A         | 101      | 111       |
| Entrada B         | 011      | 111       |
| Resultado binário | 1000     | 1110      |
| Resultado decimal | 8        | 14        |
| Display           | 8        | E         |
| Status            | Aprovado | Aprovado  |

---

## T03 — Comparação lógica

Objetivo: verificar se o comparador identifica corretamente `A > B`, `B > A` e `A = B`.

A saída da comparação é organizada assim:

```text
pino 22 -> GT1
pino 23 -> GT2
pino 24 -> A
pino 25 -> B
```

### Casos testados

| Caso | A | B | GT1 | GT2 | Relação | Display | Status   |
| ---- | - | - | --- | --- | ------- | ------- | -------- |
| 1    | 0 | 0 | 0   | 0   | A = B   | 0       | Aprovado |
| 2    | 0 | 1 | 0   | 1   | B > A   | b       | Aprovado |
| 3    | 1 | 0 | 1   | 0   | A > B   | A       | Aprovado |
| 4    | 1 | 1 | 0   | 0   | A = B   | 0       | Aprovado |

Status:

```text
T03 -> Aprovado
```

---

## T04 — Complemento de 1

Objetivo: testar o bloco de complemento de 2 bits.

No projeto final, os bits que chegam ao Arduino já estão complementados pelo hardware.

Logo:

```text
pino 22 -> Y0 = X0'
pino 23 -> Y1 = X1'
```

O Arduino monta:

```text
valor = Y0 + 2·Y1
```

### Casos testados

| X1 | X0 | Y1 = X1' | Y0 = X0' | Valor binário Y1Y0 | Display | Status   |
| -- | -- | -------- | -------- | ------------------ | ------- | -------- |
| 0  | 0  | 1        | 1        | 11                 | 3       | Aprovado |
| 0  | 1  | 1        | 0        | 10                 | 2       | Aprovado |
| 1  | 0  | 0        | 1        | 01                 | 1       | Aprovado |
| 1  | 1  | 0        | 0        | 00                 | 0       | Aprovado |

Status:

```text
T04 -> Aprovado
```

---

## T05 — Display hexadecimal

Objetivo: verificar se o display representa corretamente os valores de `0` até `F`.

Valores esperados:

```text
0, 1, 2, 3, 4, 5, 6, 7, 8, 9, A, b, C, d, E, F
```


| Teste                 | Resultado |
| --------------------- | --------- |
| Display hexadecimal   | Aprovado  |

Status:

```text
T05 -> Aprovado
```

---

## T06 — Execução pelo Monitor Serial

Objetivo: verificar se o Arduino carrega e executa opcodes enviados pelo Monitor Serial.

Comando testado:

```text
000110
```

Divisão feita pelo Arduino:

```text
00 01 10
```

Ou seja:

```text
00 -> Soma
01 -> Comparação
10 -> Complemento
```

Fluxo observado:

| Passo | Ação                 | Resultado esperado         |
| ----- | -------------------- | -------------------------- |
| 1     | Enviar `000110`      | Programa carrega 3 opcodes |
| 2     | Enviar `*`           | Executa soma               |
| 3     | Enviar `*`           | Executa comparação         |
| 4     | Enviar `*`           | Executa complemento        |
| 5     | Enviar `*` novamente | Fim do programa            |

Status:

```text
T06 -> Aprovado
```

---

## T07 — Demonstração completa

Objetivo: validar o sistema completo em sequência.

| Passo | Ação                            | Resultado observado                                |
| ----- | ------------------------------- | -------------------------------------------------- |
| 1     | Configurar entradas manuais     | chaves definem os valores de entrada               |
| 2     | Enviar programa pelo Serial     | opcodes carregados                                 |
| 3     | Enviar `*`                      | Arduino envia opcode para os MUX                   |
| 4     | MUX seleciona operação          | saída correta chega em 22, 23, 24 e 25             |
| 5     | Arduino interpreta os bits      | resultado calculado conforme operação              |
| 6     | Display atualiza                | valor aparece em hexadecimal                       |
| 7     | Monitor Serial imprime detalhes | bits lidos e operação executada aparecem no Serial |

Status:

```text
T07 -> Aprovado
```

---

## 24. Resumo geral dos testes

| Teste | Descrição                   | Resultado |
| ----- | --------------------------- | --------- |
| T01   | Seleção de operação         | Aprovado  |
| T02   | Soma binária                | Aprovado  |
| T03   | Comparação lógica           | Aprovado  |
| T04   | Complemento de 1            | Aprovado  |
| T05   | Display hexadecimal         | Aprovado  |
| T06   | Execução via Monitor Serial | Aprovado  |
| T07   | Demonstração completa       | Aprovado  |

Todos os testes principais foram executados e validados com sucesso.

---

## 25. Conclusão da implementação

A implementação final integrou lógica combinacional, multiplexadores e controle por Arduino.

O circuito realiza:

* soma de dois números de 3 bits usando um bloco somador de 4 bits
* comparação lógica entre dois bits
* complemento de 1 em dois bits
* seleção de operação por opcode
* leitura das saídas por apenas 4 pinos do Arduino
* exibição final em um display de 7 segmentos hexadecimal

A solução final ficou mais simples visualmente por usar apenas um display, já que a saída de 4 bits pode ser representada diretamente em hexadecimal.

Além disso, o Monitor Serial auxilia na validação do circuito, mostrando os bits lidos, a operação executada e o resultado interpretado pelo Arduino.

