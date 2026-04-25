// ULA com 1 display só em hexadecimal

const int OP0 = 2;
const int OP1 = 3;

const int IN0 = 22;
const int IN1 = 23;
const int IN2 = 24;
const int IN3 = 25;

// display anodo comum
const bool COMMON_ANODE = true;

// se o comum do display ligar com HIGH, deixa true
const bool DIGIT_ACTIVE_HIGH = true;

// mapeamento atual dos segmentos
const int SEG_A = 40;
const int SEG_B = 41;
const int SEG_C = 42;
const int SEG_D = 43;
const int SEG_E = 44;
const int SEG_F = 45;
const int SEG_G = 46;

// usa um lado só do display
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

// escreve o opcode
void writeOpcode(uint8_t opcode) {
  digitalWrite(OP0, opcode & 0x01);
  digitalWrite(OP1, (opcode >> 1) & 0x01);
}

// lê os 4 bits vindos dos mux
MuxData readInputs() {
  MuxData data;
  data.b0 = digitalRead(IN0);
  data.b1 = digitalRead(IN1);
  data.b2 = digitalRead(IN2);
  data.b3 = digitalRead(IN3);
  return data;
}

// monta um nibble com 22 como LSB e 25 como MSB
int readNibble() {
  MuxData data = readInputs();
  return (data.b0 << 0) | (data.b1 << 1) | (data.b2 << 2) | (data.b3 << 3);
}

// printa os bits lidos
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

// soma
int readSumValue() {
  return readNibble();
}

// complemento
// os bits que chegam ja sao o complemento
// entao aqui so le e transforma em decimal
int readComplementValue() {
  MuxData data = readInputs();

  int y0 = data.b0;
  int y1 = data.b1;

  Serial.print("Complemento recebido bruto: ");
  Serial.print(y1);
  Serial.println(y0);

  return (y0 << 0) | (y1 << 1);
}

// comparacao
// GT1 = b0
// GT2 = b1
// A   = b2
// B   = b3
//
// display:
// A > B -> A
// B > A -> b
// A = B -> 0
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
    return 10; // A
  }

  if (gt1 == 0 && gt2 == 1) {
    Serial.println("B > A");
    return 11; // b
  }

  if (gt1 == 0 && gt2 == 0) {
    Serial.println("A = B");
    return 0; // 0
  }

  Serial.println("estado invalido");
  return 15; // F para erro
}

// escreve segmento individual
void writeSegmentPin(int pin, bool on) {
  if (COMMON_ANODE) {
    digitalWrite(pin, on ? LOW : HIGH);
  } else {
    digitalWrite(pin, on ? HIGH : LOW);
  }
}

// escreve um valor hexadecimal no display
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

// liga ou desliga o display
void enableDigit(bool on) {
  if (DIGIT_ACTIVE_HIGH) {
    digitalWrite(DIG_HEX, on ? HIGH : LOW);
  } else {
    digitalWrite(DIG_HEX, on ? LOW : HIGH);
  }
}

// apaga todos os segmentos
void turnOffAllSegments() {
  writeSegmentPin(SEG_A, false);
  writeSegmentPin(SEG_B, false);
  writeSegmentPin(SEG_C, false);
  writeSegmentPin(SEG_D, false);
  writeSegmentPin(SEG_E, false);
  writeSegmentPin(SEG_F, false);
  writeSegmentPin(SEG_G, false);
}

// atualiza o display
void refreshDisplay() {
  int value = currentValueToDisplay;
  if (value < 0) value = 0;
  if (value > 15) value = 15;

  setSegmentsForHex(value);
  enableDigit(true);
}

// checa se a string so tem 0 e 1
bool isBinaryString(const String& s) {
  if (s.length() == 0) return false;

  for (unsigned int i = 0; i < s.length(); i++) {
    if (s[i] != '0' && s[i] != '1') return false;
  }

  return true;
}

// limpa o programa
void clearProgram() {
  programLength = 0;
  pc = 0;
}

// transforma a string em opcodes de 2 bits
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

// mostra o programa carregado
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

// executa a proxima instrucao
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

// processa o que vier do serial
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
