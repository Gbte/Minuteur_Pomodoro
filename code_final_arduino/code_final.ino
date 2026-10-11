#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <math.h>

// ---------- OLED ----------
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_SDA 8
#define OLED_SCL 9

Adafruit_SSD1306 display(
  SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1
);

// ---------- Boutons et buzzer ----------
#define BTN_FOCUS 3
#define BTN_BREAK 4
#define BUZZER_PIN 5

// ---------- Durees ----------
const unsigned long FOCUS_TIME = 25UL * 60UL * 1000UL;
const unsigned long BREAK_TIME = 5UL * 60UL * 1000UL;

// ---------- Etats ----------
enum TimerMode {
  IDLE,
  FOCUS,
  BREAK_TIME_MODE,
  FINISHED
};

TimerMode mode = IDLE;

unsigned long startTime = 0;
unsigned long duration = 0;
unsigned long lastDisplayUpdate = 0;
unsigned long lastWelcomeUpdate = 0;

bool lastFocusButton = HIGH;
bool lastBreakButton = HIGH;

unsigned long lastFocusChange = 0;
unsigned long lastBreakChange = 0;

const unsigned long DEBOUNCE_MS = 40;

// ---------- Sons ----------
void beepStart() {
  tone(BUZZER_PIN, 2000, 180);
  delay(230);
}

void beepEnd() {
  for (int i = 0; i < 3; i++) {
    tone(BUZZER_PIN, 2500, 200);
    delay(300);
  }
  noTone(BUZZER_PIN);
}

// ---------- Texte centre ----------
void centeredText(const char* text, int y, int size) {
  display.setTextSize(size);

  int16_t x1, y1;
  uint16_t w, h;

  display.getTextBounds(
    text, 0, y, &x1, &y1, &w, &h
  );

  int x = (SCREEN_WIDTH - w) / 2;
  display.setCursor(x, y);
  display.print(text);
}

// ---------- Horloge animee ----------
void drawClock(int cx, int cy, int radius, int frame) {
  display.drawCircle(cx, cy, radius, SSD1306_WHITE);

  // Graduations de l'horloge
  for (int i = 0; i < 12; i++) {
    float angle = i * 30.0 * PI / 180.0;

    int x1 = cx + cos(angle) * (radius - 3);
    int y1 = cy + sin(angle) * (radius - 3);

    int x2 = cx + cos(angle) * (radius - 1);
    int y2 = cy + sin(angle) * (radius - 1);

    display.drawLine(x1, y1, x2, y2, SSD1306_WHITE);
  }

  // Aiguille animee
  float angle = (frame % 60) * 6.0 * PI / 180.0;

  int handX = cx + cos(angle - PI / 2) * (radius - 5);
  int handY = cy + sin(angle - PI / 2) * (radius - 5);

  display.drawLine(cx, cy, handX, handY, SSD1306_WHITE);
  display.fillCircle(cx, cy, 2, SSD1306_WHITE);
}

// ---------- Ecran d'accueil anime ----------
void drawWelcome() {
  unsigned long now = millis();

  // Actualisation de l'animation toutes les 100 ms
  if (now - lastWelcomeUpdate < 100) {
    return;
  }

  lastWelcomeUpdate = now;
  int frame = now / 100;

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  // Horloge animee
  drawClock(64, 17, 13, frame);

  // Titre centre
  centeredText("POMODORO", 36, 2);

  // Message d'accueil : pas de points animes
  centeredText("PRET A TRAVAILLER", 56, 1);

  display.display();
}

// ---------- Affichage simple ----------
void showScreen(const char* title, const char* message) {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  display.setTextSize(2);
  display.setCursor(0, 5);
  display.println(title);

  display.setTextSize(1);
  display.setCursor(0, 38);
  display.println(message);

  display.display();
}

// ---------- Demarrer le minuteur ----------
void startTimer(TimerMode newMode, unsigned long newDuration) {
  mode = newMode;
  duration = newDuration;
  startTime = millis();
  lastDisplayUpdate = 0;

  beepStart();
}

// ---------- Decompte : conserve l'affichage d'origine ----------
void updateTimer() {
  if (mode != FOCUS && mode != BREAK_TIME_MODE) {
    return;
  }

  unsigned long elapsed = millis() - startTime;

  if (elapsed >= duration) {
    mode = FINISHED;
    beepEnd();
    showScreen("TERMINE !", "Appuie sur un bouton");
    return;
  }

  unsigned long remaining = (duration - elapsed + 999) / 1000;
  int minutes = remaining / 60;
  int seconds = remaining % 60;

  if (millis() - lastDisplayUpdate >= 200) {
    lastDisplayUpdate = millis();

    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);

    display.setTextSize(1);
    display.setCursor(0, 0);

    if (mode == FOCUS) {
      display.println("CONCENTRATION");
    } else {
      display.println("PAUSE");
    }

    display.drawLine(0, 13, 127, 13, SSD1306_WHITE);

    display.setTextSize(3);
    display.setCursor(16, 24);

    if (minutes < 10) display.print("0");
    display.print(minutes);
    display.print(":");
    if (seconds < 10) display.print("0");
    display.print(seconds);

    display.setTextSize(1);
    display.setCursor(0, 55);

    if (mode == FOCUS) {
      display.print("Reste concentre !");
    } else {
      display.print("Repose-toi !");
    }

    display.display();
  }
}

// ---------- Lecture des boutons ----------
void readButtons() {
  unsigned long now = millis();

  bool focusReading = digitalRead(BTN_FOCUS);
  bool breakReading = digitalRead(BTN_BREAK);

  // Bouton gauche : concentration
  if (focusReading != lastFocusButton &&
      now - lastFocusChange >= DEBOUNCE_MS) {

    lastFocusChange = now;
    lastFocusButton = focusReading;

    if (focusReading == LOW) {
      startTimer(FOCUS, FOCUS_TIME);
      showScreen("FOCUS", "Concentration 25 min");
    }
  }

  // Bouton droit : pause
  if (breakReading != lastBreakButton &&
      now - lastBreakChange >= DEBOUNCE_MS) {

    lastBreakChange = now;
    lastBreakButton = breakReading;

    if (breakReading == LOW) {
      startTimer(BREAK_TIME_MODE, BREAK_TIME);
      showScreen("PAUSE", "Repos pendant 5 min");
    }
  }
}

// ---------- Initialisation ----------
void setup() {
  Serial.begin(115200);

  pinMode(BTN_FOCUS, INPUT_PULLUP);
  pinMode(BTN_BREAK, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);

  noTone(BUZZER_PIN);

  Wire.begin(OLED_SDA, OLED_SCL);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("Erreur OLED : verifier le cablage !");
    while (true) {
      delay(100);
    }
  }

  display.clearDisplay();
  display.display();

  Serial.println("Minuteur Pomodoro pret !");
}

// ---------- Boucle principale ----------
void loop() {
  readButtons();

  if (mode == IDLE) {
    drawWelcome();
  } else {
    updateTimer();
  }
}
