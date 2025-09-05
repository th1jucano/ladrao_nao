#include <Arduino.h>
#include <Ultrasonic.h>
#include <Wire.h>
#include <RTClib.h> // Biblioteca para o RTC

#define rele1 2
#define rele2 3
#define buzzer 7

Ultrasonic ultrasom(10, 11); // Trigger e Echo
RTC_DS3231 rtc;

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
  
  if (!rtc.begin()) {
    Serial.println("RTC não encontrado!");
    while (1);
  }

  // rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // Descomente esta linha apenas uma vez para ajustar o RTC com a hora do computador
}

void loop() {
  DateTime agora = rtc.now();
  int horaAtual = agora.hour();

  Serial.print("Hora atual: ");
  Serial.println(horaAtual);

  // Permitir apenas entre 18h e 6h
  if (horaAtual >= 18 || horaAtual < 6) {
    distancia = ultrasom.read();
    Serial.print("Distancia: ");
    Serial.println(distancia);

    if (distancia > 0 && distancia < 80) {
      unsigned long tempoInicio = millis();
      while (millis() - tempoInicio < 42000) {
        Serial.println(millis());
        state = !state;
        digitalWrite(rele1, state ? LOW : HIGH);
        digitalWrite(rele2, state ? HIGH : LOW);
        toogle = !toogle;
        freq = toogle ? 700 : 1200;
        tone(buzzer, freq);
        delay(150);
      }
      // Após 45s, desliga relés e buzzer
      digitalWrite(rele1, HIGH);
      digitalWrite(rele2, HIGH);
      noTone(buzzer);
    }
  } else {
    // Fora do horário: garante tudo desligado
    digitalWrite(rele1, HIGH);
    digitalWrite(rele2, HIGH);
    noTone(buzzer);
  }

  delay(150); // Pequeno delay para evitar sobrecarga
}
