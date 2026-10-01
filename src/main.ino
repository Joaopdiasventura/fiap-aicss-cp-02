
#include <Arduino.h>

const char monitoredDomain[] = "github.com";
const char referenceIpv4[] = "140.82.112.3";

const int buttonPin = 4;
const int blueLedPin = 22;
const int redLedPin = 23;

const unsigned long debounceDelayMs = 50;

bool stableButtonState = LOW;
bool lastButtonReading = LOW;
unsigned long lastDebounceMs = 0;

bool waitingForIp = false;
String serialBuffer = "";
bool skipNextLf = false;

void turnOffLeds() {
  digitalWrite(blueLedPin, LOW);
  digitalWrite(redLedPin, LOW);
}

void showBlueOnly() {
  digitalWrite(redLedPin, LOW);
  digitalWrite(blueLedPin, HIGH);
}

void showRedOnly() {
  digitalWrite(blueLedPin, LOW);
  digitalWrite(redLedPin, HIGH);
}

void printHeader() {
  Serial.println();
  Serial.println("=== NetGuard - Verificacao de dominio e IPv4 ===");
  Serial.print("Dominio monitorado: ");
  Serial.println(monitoredDomain);
}

void requestIpInput() {
  waitingForIp = true;
  serialBuffer = "";
  skipNextLf = false;

  turnOffLeds();
  printHeader();

  Serial.println("Informe o endereco IPv4 e pressione ENTER:");
}

bool buttonPressed() {
  bool reading = digitalRead(buttonPin);
  unsigned long currentMs = millis();

  if (reading != lastButtonReading) {
    lastDebounceMs = currentMs;
    lastButtonReading = reading;
  }

  if (currentMs - lastDebounceMs < debounceDelayMs) return false;

  if (reading != stableButtonState) {
    stableButtonState = reading;
    return stableButtonState == HIGH;
  }

  return false;
}
bool readSerialLine(String &line) {
  while (Serial.available() > 0) {
    char currentChar = Serial.read();

    if (skipNextLf && currentChar == '\n') {
      skipNextLf = false;
      continue;
    }

    skipNextLf = false;

    if (currentChar == '\r' || currentChar == '\n') {
      if (currentChar == '\r') {
        skipNextLf = true;
      }

      serialBuffer.trim();

      line = serialBuffer;
      serialBuffer = "";
      return true;
    }

    if (isPrintable(currentChar)) {
      serialBuffer += currentChar;
    }
  }

  return false;
}

bool isValidIpv4(const String &ip) {
  int partValue = 0;
  int partDigits = 0;
  int dotCount = 0;

  for (unsigned int i = 0; i < ip.length(); i++) {
    char currentChar = ip.charAt(i);

    if (currentChar >= '0' && currentChar <= '9') {
      if (partDigits == 3) return false;

      partValue = partValue * 10 + (currentChar - '0');
      partDigits++;

      if (partValue > 255) return false;

      continue;
    }

    if (currentChar == '.') {
      if (partDigits == 0 || dotCount >= 3) return false;

      dotCount++;
      partValue = 0;
      partDigits = 0;
      continue;
    }

    return false;
  }

  return dotCount == 3 && partDigits > 0;
}

void printResult(const String &ip, const char result[]) {
  Serial.println();

  Serial.print("Dominio monitorado: ");
  Serial.println(monitoredDomain);

  Serial.print("IP informado: ");
  Serial.println(ip);

  Serial.print("Resultado: ");
  Serial.println(result);

  Serial.println();
  Serial.println("Pressione o botao para iniciar uma nova verificacao.");
}

void verifyIp(const String &ip) {
  if (!isValidIpv4(ip)) {
    showRedOnly();
    printResult(ip, "IPv4 INVALIDO");

    waitingForIp = false;
    return;
  }

  if (ip.equals(referenceIpv4)) {
    showBlueOnly();
    printResult(ip, "IP CORRETO");
  } else {
    showRedOnly();
    printResult(ip, "IP DIFERENTE DA REFERENCIA");
  }

  waitingForIp = false;
}

void setup() {
  Serial.begin(115200);

  pinMode(buttonPin, INPUT);

  pinMode(blueLedPin, OUTPUT);
  pinMode(redLedPin, OUTPUT);

  turnOffLeds();

  Serial.println("NetGuard inicializado.");
  Serial.println("Pressione o botao para iniciar a verificacao.");
}

void loop() {
  if (buttonPressed()) requestIpInput();

  if (!waitingForIp) return;

  String ip;

  if (readSerialLine(ip)) verifyIp(ip);
}
