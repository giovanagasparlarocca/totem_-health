#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

// Pinos do display
#define TFT_DC 2
#define TFT_CS 15
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

// Pinos dos botões
#define BTN_UP     4
#define BTN_DOWN   5
#define BTN_LEFT   25
#define BTN_RIGHT  26
#define BTN_ENTER  21

// Layout do teclado virtual (6 colunas x 5 linhas = 30 teclas)
const char keys[] = "ABCDEF"
                    "GHIJKL"
                    "MNOPQR"
                    "STUVWX"
                    "YZ<-.";
                    // O = tecla "O" normal, 
                    // '.' será tratado como OK

int selectedIndex = 0;
String typedText = "";
bool needsRedraw = true;

// Função para desenhar o teclado
void drawKeyboard() {
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextSize(2);

  // Texto digitado
  tft.setCursor(10, 5);
  tft.setTextColor(ILI9341_WHITE);
  tft.print("Nome: ");
  tft.print(typedText);

  // Configurações do teclado
  int keyWidth = 40;
  int keyHeight = 35;
  int startX = 20;
  int startY = 30;
  int spacing = 4;

  for (int i = 0; i < 30; i++) {
    int row = i / 6;
    int col = i % 6;
    int x = startX + col * (keyWidth + spacing);
    int y = startY + row * (keyHeight + spacing);

    if (i == selectedIndex) {
      tft.fillRect(x, y, keyWidth, keyHeight, ILI9341_YELLOW);
      tft.setTextColor(ILI9341_BLACK);
    } else {
      tft.drawRect(x, y, keyWidth, keyHeight, ILI9341_WHITE);
      tft.setTextColor(ILI9341_WHITE);
    }

    tft.setCursor(x + 6, y + 8);

    if (keys[i] == '.') {  
      tft.print("OK");  // mostra OK na última tecla
    } else {
      tft.print(keys[i]);
    }
  }
}

// Função para tratar o pressionamento de tecla
void handleKeyPress(char key) {
  if (key == '<') {
    if (typedText.length() > 0) {
      typedText.remove(typedText.length() - 1);
    }
  } else if (key == '.') {  // tecla OK
    tft.fillScreen(ILI9341_GREEN);
    tft.setCursor(50, 100);
    tft.setTextColor(ILI9341_BLACK);
    tft.setTextSize(3);
    tft.print("Cadastro OK!");
    delay(2000);
    typedText = "";
  } else if (key != '-') {
    typedText += key;
  }
}

void setup() {
  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BTN_DOWN, INPUT_PULLUP);
  pinMode(BTN_LEFT, INPUT_PULLUP);
  pinMode(BTN_RIGHT, INPUT_PULLUP);
  pinMode(BTN_ENTER, INPUT_PULLUP);

  tft.begin();
  tft.setRotation(1);
  drawKeyboard();
}

void loop() {
  static int lastUp = HIGH, lastDown = HIGH, lastLeft = HIGH, lastRight = HIGH, lastEnter = HIGH;

  int currentUp = digitalRead(BTN_UP);
  int currentDown = digitalRead(BTN_DOWN);
  int currentLeft = digitalRead(BTN_LEFT);
  int currentRight = digitalRead(BTN_RIGHT);
  int currentEnter = digitalRead(BTN_ENTER);

  int row = selectedIndex / 6;
  int col = selectedIndex % 6;

  // Navegação
  if (lastUp == HIGH && currentUp == LOW) {
    if (row > 0) { row--; needsRedraw = true; }
  }
  if (lastDown == HIGH && currentDown == LOW) {
    if (row < 4) { row++; needsRedraw = true; }
  }
  if (lastLeft == HIGH && currentLeft == LOW) {
    if (col > 0) { col--; needsRedraw = true; }
  }
  if (lastRight == HIGH && currentRight == LOW) {
    if (col < 5) { col++; needsRedraw = true; }
  }

  selectedIndex = row * 6 + col;
  if (selectedIndex >= 30) selectedIndex = 29; // evita posição inválida

  // Seleção
  if (lastEnter == HIGH && currentEnter == LOW) {
    handleKeyPress(keys[selectedIndex]);
    needsRedraw = true;
  }

  if (needsRedraw) {
    drawKeyboard();
    needsRedraw = false;
  }

  lastUp = currentUp;
  lastDown = currentDown;
  lastLeft = currentLeft;
  lastRight = currentRight;
  lastEnter = currentEnter;
}
