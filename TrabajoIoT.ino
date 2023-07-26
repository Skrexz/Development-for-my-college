#include <dummy.h>

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP32Servo.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

Servo myservo;  // crea el objeto servo

// GPIO de salida del servo
static const int servoPin = 13;

// Reemplazar con tus credenciales de Wifi
const char* ssid = "HUAWEI P20 lite";
const char* password = "elcabro123";

// Inicializa Bot Telegram
#define BOTtoken "6234887660:AAE-0I55ju9Rw9TIyGbJX8vCLRameYINUQA"  // Tu Bot Token (Obtener de Botfather)

#define CHAT_ID "5879430854"

WiFiClientSecure client;

UniversalTelegramBot bot(BOTtoken, client);

void handleNewMessages(int numNewMessages) {

  for (int i = 0; i < numNewMessages; i++) {
    // Chat id of the requester
    String chat_id = String(bot.messages[i].chat_id);
    if (chat_id != CHAT_ID) {
      bot.sendMessage(chat_id, "Usuario no autorizado", "");
      continue;
    }

    String text = bot.messages[i].text;

    if (text == "/comida") {
      bot.sendMessage(chat_id, "Alimentando", "");
      try {
        myservo.write(90);
        delay(4000);
        myservo.write(360);
      } catch (String error) {
        Serial.println(error);
      }
    }
  }
}

void setup() {
  Serial.begin(115200);

  myservo.attach(servoPin);  // vincula el servo en el servoPin

  // Conecta a red WiFi con SSID y password
  Serial.print("Conectado a ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  client.setCACert(TELEGRAM_CERTIFICATE_ROOT);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Muestra IP local
  Serial.println("");
  Serial.println("WiFi conectado.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  bot.sendMessage(CHAT_ID, "Bot iniciado", "");
}

void loop() {
  int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

  while (numNewMessages) {
    handleNewMessages(numNewMessages);
    numNewMessages = bot.getUpdates(bot.last_message_received + 1);
  }
}
