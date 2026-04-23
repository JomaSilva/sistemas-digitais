// ULA com leitura apenas nas portas 22, 23, 24 e 25

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

// Escreve o opcode no hardware
void writeOpcode(uint8_t opcode) {
  digitalWrite(OP0, opcode & 0x01);
  digitalWrite(OP1, (opcode >> 1) & 0x01);
}

// Lê as 4 entradas vindas do mux
MuxData readInputs() {
  MuxData data;
  data.b0 = digitalRead(IN0);
  data.b1 = digitalRead(IN1);
  data.b2 = digitalRead(IN2);
  data.b3 = digitalRead(IN3);
  return data;
}

// Converte 22..25 em valor de 4 bits
int readNibble() {
  MuxData data = readInputs();
  return (data.b0 << 0) | (data.b1 << 1) | (data.b2 << 2) | (data.b3 << 3);
}

// Soma: 22=S0, 23=S1, 24=S2, 25=S3
int readSumValue() {
  return readNibble();
}

// Comparação: 22=GT1, 23=GT2, 24=A, 25=B
int readComparisonValue() {
  MuxData data = readInputs();
  int a = data.b2;
  int b = data.b3;
  return (a || b) ? 1 : 0;
}

// Mostra a relação entre A e B
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

// Complemento: 22=Y0, 23=Y1
int readComplementValue() {
  MuxData data = readInputs();
  return (data.b0 << 0) | (data.b1 << 1);
}

// Liga os segmentos do dígito
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

// Liga ou desliga um dígito
void enableDigit(int pin, bool on) {
  if (COMMON_ANODE) {
    digitalWrite(pin, on ? LOW : HIGH);
  } else {
    digitalWrite(pin, on ? HIGH : LOW);
  }
}

// Apaga todos os segmentos
void turnOffAllSegments() {
  digitalWrite(SEG_A, COMMON_ANODE ? HIGH : LOW);
  digitalWrite(SEG_B, COMMON_ANODE ? HIGH : LOW);
  digitalWrite(SEG_C, COMMON_ANODE ? HIGH : LOW);
  digitalWrite(SEG_D, COMMON_ANODE ? HIGH : LOW);
  digitalWrite(SEG_E, COMMON_ANODE ? HIGH : LOW);
  digitalWrite(SEG_F, COMMON_ANODE ? HIGH : LOW);
  digitalWrite(SEG_G, COMMON_ANODE ? HIGH : LOW);
}

// Atualiza o display multiplexado
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

// Verifica se a string é binária
bool isBinaryString(const String& s) {
  if (s.length() == 0) return false;

  for (unsigned int i = 0; i < s.length(); i++) {
    if (s[i] != '0' && s[i] != '1') return false;
  }
  return true;
}

// Limpa o programa
void clearProgram() {
  programLength = 0;
  pc = 0;
}

// Transforma os bits digitados em opcodes
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

// Mostra os opcodes carregados
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

// Executa o próximo opcode
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

// Processa o que vier do serial
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
