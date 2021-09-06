
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define wifi_ssid "xxxxxxxx"
#define wifi_password "xxxxxxxx"

#define mqtt_server "soldier.cloudmqtt.com"
#define mqtt_port 18468
#define mqtt_user "xxxxxxxx"
#define mqtt_password "xxxxxxxx"

#define in_topic "test/"
#define out_topic "/light/out"
#define in_led 1

WiFiClient espClient;
PubSubClient client;

void setup() {
  Serial.begin(115200);

  setup_wifi();
  client.setClient(espClient);
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  pinMode(in_led, OUTPUT);
  digitalWrite(in_led, HIGH);
}

void setup_wifi() {
  delay(10);

  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(wifi_ssid);

  WiFi.begin(wifi_ssid, wifi_password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");

    if (client.connect("ESP8266Client", mqtt_user, mqtt_password)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");

      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
 Serial.print("Message arrived [");
 Serial.print(topic);
 Serial.print("] ");
 for (int i = 0; i < length; i++) {
  char receivedChar = (char)payload[i];
  Serial.print(receivedChar);
  if (receivedChar == '0')
   digitalWrite(in_led, LOW);
  if (receivedChar == '1')
   digitalWrite(in_led, HIGH);
 }
 Serial.println();
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }

  client.loop();
  client.publish(out_topic, String(random(2)).c_str(), true);
  delay(1000);
  client.subscribe(in_topic);
  delay(1000);
}
