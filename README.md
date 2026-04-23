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

## 6. Observação final

Ao representar o sistema como uma pequena ULA, as três operações podem ser vistas assim:

* **Somador:** retorna `S0, S1, S2, Cout`
* **Complemento:** retorna `~A, ~B`
* **Comparador:** retorna `GT1, GT2, A, B`

Isso permite selecionar a saída desejada com multiplexadores e com uma lógica de controle por opcode enviada pelo Arduino.

# Arduino, código e displays

## 7. Papel do Arduino no sistema

No projeto, o **Arduino Mega 2560** funciona como a parte de controle e leitura da ULA.

Ele tem três funções principais:

* enviar o **opcode** da operação para o hardware
* ler os **4 bits de saída** vindos dos multiplexadores
* mostrar o resultado nos **displays de 7 segmentos** e no **monitor serial**

A ideia geral do funcionamento é a seguinte:

1. o Arduino recebe um comando pelo serial
2. esse comando é quebrado em opcodes de 2 bits
3. cada opcode é enviado para as linhas de seleção `OP0` e `OP1`
4. os multiplexadores liberam a saída correspondente à operação escolhida
5. o Arduino lê os bits finais nas portas `22, 23, 24 e 25`
6. o valor é mostrado no display e, no caso da comparação, também é detalhado no monitor serial

### 7.1 Operações controladas por opcode

A seleção da operação é feita com 2 bits:

```text
00 -> soma
01 -> comparação
10 -> complemento
11 -> reservado
```

### 7.2 Entradas e saídas usadas no Arduino

#### Saídas de seleção

* `OP0` -> pino 2
* `OP1` -> pino 3

Esses dois pinos saem do Arduino e vão para as linhas de seleção dos 4 MUX.

#### Entradas vindas dos MUX

* `22` -> bit 0
* `23` -> bit 1
* `24` -> bit 2
* `25` -> bit 3

Ou seja, o Arduino sempre lê o resultado final usando os mesmos quatro pinos.

A leitura é feita na ordem:

```text
22 = bit menos significativo
23 = bit 1
24 = bit 2
25 = bit mais significativo
```

Então o valor binário lido é montado assim:

```text
valor = b0 + 2·b1 + 4·b2 + 8·b3
```

### 7.3 Como o Arduino interpreta cada operação

O significado desses quatro bits depende da operação selecionada.

#### Soma

```text
[MUX0, MUX1, MUX2, MUX3] = [S0, S1, S2, Cout]
```

#### Comparação

```text
[MUX0, MUX1, MUX2, MUX3] = [GT1, GT2, A, B]
```

#### Complemento

```text
[MUX0, MUX1] = [~A, ~B]
```

No caso da comparação, o Arduino usa:

* `GT1` para saber se `A > B`
* `GT2` para saber se `B > A`
* `A` e `B` para mostrar os valores originais lidos

A interpretação fica assim:

```text
GT1 = 1 e GT2 = 0 -> A > B
GT1 = 0 e GT2 = 1 -> B > A
GT1 = 0 e GT2 = 0 -> A = B
```

---

## 8. Código do Arduino

Abaixo está o código usado para controlar a ULA, selecionar as operações, ler os bits vindos dos MUX e exibir o resultado.

```cpp
const int OP0 = 2;
const int OP1 = 3;

const int IN0 = 22;
const int IN1 = 23;
const int IN2 = 24;
const int IN3 = 25;

const bool COMMON_ANODE = false;

const int SEG_A = 40;
const int SEG_B = 41;
const int SEG_C = 42;
const int SEG_D = 43;
const int SEG_E = 44;
const int SEG_F = 45;
const int SEG_G = 46;

const int DIG_TENS = 47;
const int DIG_UNITS = 48;

const int MAX_OPCODES = 64;
uint8_t programOpcodes[MAX_OPCODES];
int programLength = 0;
int pc = 0;

String lineBuffer = "";
int currentValueToDisplay = 0;

const bool digitMap[10][7] = {
  {1,1,1,1,1,1,0},
  {0,1,1,0,0,0,0},
  {1,1,0,1,1,0,1},
  {1,1,1,1,0,0,1},
  {0,1,1,0,0,1,1},
  {1,0,1,1,0,1,1},
  {1,0,1,1,1,1,1},
  {1,1,1,0,0,0,0},
  {1,1,1,1,1,1,1},
  {1,1,1,1,0,1,1}
};

struct MuxData {
  int b0;
  int b1;
  int b2;
  int b3;
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

int readSumValue() {
  return readNibble();
}

int readComparisonValue() {
  MuxData data = readInputs();
  int a = data.b2;
  int b = data.b3;
  return (a || b) ? 1 : 0;
}

void reportComparisonRelation() {
  MuxData data = readInputs();

  int gt1 = data.b0;
  int gt2 = data.b1;
  int a = data.b2;
  int b = data.b3;

  Serial.print("Comparacao: A=");
  Serial.print(a);
  Serial.print(" B=");
  Serial.print(b);
  Serial.print(" | GT1=");
  Serial.print(gt1);
  Serial.print(" GT2=");
  Serial.print(gt2);
  Serial.print(" | Relacao: ");

  if (gt1 == 1 && gt2 == 0) {
    Serial.println("A > B");
  } else if (gt1 == 0 && gt2 == 1) {
    Serial.println("B > A");
  } else if (gt1 == 0 && gt2 == 0) {
    Serial.println("A = B");
  } else {
    Serial.println("estado invalido");
  }
}

int readComplementValue() {
  MuxData data = readInputs();
  return (data.b0 << 0) | (data.b1 << 1);
}

void setSegmentsForDigit(int d) {
  if (d < 0 || d > 9) d = 0;

  bool segState[7];
  for (int i = 0; i < 7; i++) {
    segState[i] = digitMap[d][i];
    if (COMMON_ANODE) {
      segState[i] = !segState[i];
    }
  }

  digitalWrite(SEG_A, segState[0]);
  digitalWrite(SEG_B, segState[1]);
  digitalWrite(SEG_C, segState[2]);
  digitalWrite(SEG_D, segState[3]);
  digitalWrite(SEG_E, segState[4]);
  digitalWrite(SEG_F, segState[5]);
  digitalWrite(SEG_G, segState[6]);
}

void enableDigit(int pin, bool on) {
  if (COMMON_ANODE) {
    digitalWrite(pin, on ? LOW : HIGH);
  } else {
    digitalWrite(pin, on ? HIGH : LOW);
  }
}

void turnOffAllSegments() {
  digitalWrite(SEG_A, COMMON_ANODE ? HIGH : LOW);
  digitalWrite(SEG_B, COMMON_ANODE ? HIGH : LOW);
  digitalWrite(SEG_C, COMMON_ANODE ? HIGH : LOW);
  digitalWrite(SEG_D, COMMON_ANODE ? HIGH : LOW);
  digitalWrite(SEG_E, COMMON_ANODE ? HIGH : LOW);
  digitalWrite(SEG_F, COMMON_ANODE ? HIGH : LOW);
  digitalWrite(SEG_G, COMMON_ANODE ? HIGH : LOW);
}

void refreshDisplay() {
  static bool showTens = false;
  static unsigned long lastMuxMicros = 0;

  unsigned long now = micros();
  if (now - lastMuxMicros < 2000) return;
  lastMuxMicros = now;

  int value = currentValueToDisplay;
  if (value < 0) value = 0;
  if (value > 99) value = 99;

  int tens = value / 10;
  int units = value % 10;

  enableDigit(DIG_TENS, false);
  enableDigit(DIG_UNITS, false);

  if (showTens) {
    if (tens == 0 && value < 10) {
      turnOffAllSegments();
    } else {
      setSegmentsForDigit(tens);
      enableDigit(DIG_TENS, true);
    }
  } else {
    setSegmentsForDigit(units);
    enableDigit(DIG_UNITS, true);
  }

  showTens = !showTens;
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
      int value = readSumValue();
      currentValueToDisplay = value;

      Serial.print("OP 00 - SOMA -> ");
      Serial.println(value);
      break;
    }

    case 0b01: {
      int value = readComparisonValue();
      currentValueToDisplay = value;

      Serial.print("OP 01 - COMPARACAO -> maior valor = ");
      Serial.println(value);
      reportComparisonRelation();
      break;
    }

    case 0b10: {
      int value = readComplementValue();
      currentValueToDisplay = value;

      Serial.print("OP 10 - COMPLEMENTO -> ");
      Serial.println(value);
      break;
    }

    case 0b11: {
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

  pinMode(DIG_TENS, OUTPUT);
  pinMode(DIG_UNITS, OUTPUT);

  writeOpcode(0);
  currentValueToDisplay = 0;

  enableDigit(DIG_TENS, false);
  enableDigit(DIG_UNITS, false);
  turnOffAllSegments();

  Serial.println("=== ULA combinacional - Mega 2560 ===");
  Serial.println("Lendo apenas as entradas 22, 23, 24 e 25.");
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

### 8.1 Resumo do que o código faz

O código foi dividido em algumas partes principais.

#### Escrita do opcode

A função `writeOpcode()` coloca o valor de `OP0` e `OP1` nas saídas do Arduino. Esse valor é usado para escolher qual operação os MUX devem entregar.

#### Leitura das entradas dos MUX

As funções `readInputs()` e `readNibble()` fazem a leitura dos pinos `22, 23, 24 e 25` e montam um número de 4 bits.

#### Leitura por operação

Existem funções separadas para cada caso:

* `readSumValue()`
* `readComparisonValue()`
* `readComplementValue()`

Isso deixa a lógica mais organizada, porque cada opcode pode interpretar os mesmos 4 pinos de um jeito diferente.

#### Execução de instruções

A função `executeNextOpcode()` pega o próximo opcode carregado, envia para o hardware, espera os sinais estabilizarem e depois faz a leitura correta da operação.

#### Controle pelo monitor serial

O usuário pode digitar uma sequência binária no monitor serial, por exemplo:

```text
000110
```

Essa sequência é quebrada assim:

```text
00 01 10
```

Depois, cada vez que `*` é enviado, o Arduino executa a próxima instrução.

#### Atualização do display

A função `refreshDisplay()` atualiza continuamente os dois displays de 7 segmentos usando multiplexação.

---

## 9. Uso dos dois displays de 7 segmentos

O sistema usa **dois displays de 7 segmentos** para mostrar o valor final lido pelo Arduino.

Esses dois displays representam:

* **unidade**
* **dezena**

Assim, o sistema pode exibir valores de `0` até `99`.

### 9.1 Por que usar dois displays

Como a saída do circuito pode ser mostrada em forma decimal, usar dois displays facilita a visualização do resultado.

Exemplos:

* soma `3 + 4 = 7` -> mostra `07` ou apenas `7`
* soma `5 + 6 = 11` -> mostra `11`
* complemento `10` -> mostra `2`
* comparação -> pode mostrar `0` ou `1`, dependendo do valor carregado para exibição

### 9.2 Organização dos segmentos

Os segmentos do display foram ligados aos pinos:

* `SEG_A = 40`
* `SEG_B = 41`
* `SEG_C = 42`
* `SEG_D = 43`
* `SEG_E = 44`
* `SEG_F = 45`
* `SEG_G = 46`

Os pinos que habilitam cada dígito são:

* `DIG_TENS = 47` -> display da dezena
* `DIG_UNITS = 48` -> display da unidade

### 9.3 Como os dois displays funcionam juntos

Os dois displays compartilham as mesmas linhas de segmento `A` até `G`.

O que muda é qual display está ativado naquele instante.

Então o Arduino faz o seguinte:

1. desliga os dois displays
2. liga os segmentos da dezena e ativa só o display da dezena
3. desliga de novo
4. liga os segmentos da unidade e ativa só o display da unidade
5. repete isso muito rápido

Esse processo é chamado de **multiplexação**.

Como a troca é muito rápida, o olho humano enxerga os dois displays ligados ao mesmo tempo.

### 9.4 Separação entre dezena e unidade

O valor final fica salvo em `currentValueToDisplay`.

Depois ele é separado assim:

```text
dezena = valor / 10
unidade = valor % 10
```

Exemplo para o número `14`:

```text
dezena = 14 / 10 = 1
unidade = 14 % 10 = 4
```

### 9.5 Tabela dos dígitos

No código, a tabela `digitMap` guarda quais segmentos devem acender para cada número de `0` a `9`.

Exemplo:

* para mostrar `0`, todos os segmentos acendem, menos o `G`
* para mostrar `1`, acendem apenas os segmentos `B` e `C`
* para mostrar `8`, todos os segmentos acendem

### 9.6 Zero à esquerda

O código também evita mostrar a dezena quando ela é zero e o valor é menor que 10.

Por isso, em vez de aparecer:

```text
05
```

pode aparecer apenas:

```text
5
```

Isso deixa a exibição mais natural.

---

## 10. Integração entre Arduino, MUX e displays

O fluxo completo do sistema pode ser resumido assim:

1. o usuário envia uma sequência binária pelo monitor serial
2. o Arduino separa essa sequência em opcodes
3. o Arduino envia o opcode para `OP0` e `OP1`
4. os 4 multiplexadores colocam a saída certa nos pinos `22, 23, 24 e 25`
5. o Arduino lê esses 4 bits
6. o valor lido é convertido e salvo
7. o resultado é mostrado nos displays de 7 segmentos
8. no caso da comparação, o monitor serial também informa a relação entre `A` e `B`

Esse modelo permitiu usar o Arduino como unidade de controle do sistema, mantendo a parte lógica principal no hardware digital e usando software apenas para seleção, leitura e exibição dos resultados.

---
