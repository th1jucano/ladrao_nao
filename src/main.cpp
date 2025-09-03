#include <Arduino.h>
#include <Ultrasonic.h>

#define rele1 2
#define rele2 3
#define buzzer 7

Ultrasonic ultrasom(10,11); // Trigger e echo                         

bool state = false;
int freq = 2600;
bool toogle = false;
float distancia = 0;

void setup() {
  Serial.begin(9600);
  pinMode(rele1, OUTPUT);
  pinMode(rele2, OUTPUT);
  digitalWrite(rele1, HIGH);
  digitalWrite(rele2, HIGH);
}

void loop() {
  distancia = ultrasom.read();
  Serial.println(distancia);

  if (distancia > 0 && distancia < 70) {
    unsigned long tempoInicio = millis();
    while (millis() - tempoInicio < 45000) { // millis continua, tempoInicio para
      Serial.println(millis());
      state = !state; // Corrigido
      digitalWrite(rele1, state ? LOW : HIGH); // rele 1 sim? agora não
      digitalWrite(rele2, state ? HIGH : LOW);
      toogle = !toogle; // Corrigido
      freq = toogle ? 700 : 1200;
      tone(buzzer, freq);
      delay(150); // Adicionado delay
    }
    // Após 45s, desliga relés e buzzer
    digitalWrite(rele1, HIGH);
    digitalWrite(rele2, HIGH);
    noTone(buzzer);
  }
} 