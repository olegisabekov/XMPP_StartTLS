#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include "Xmpp.h"

#define INFO_LED GPIO_NUM_32

static const char root_cert[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIFazCCA1OgAwIBAgIRAIIQz7DSQONZRGPgu2OCiwAwDQYJKoZIhvcNAQELBQAw
TzELMAkGA1UEBhMCVVMxKTAnBgNVBAoTIEludGVybmV0IFNlY3VyaXR5IFJlc2Vh
cmNoIEdyb3VwMRUwEwYDVQQDEwxJU1JHIFJvb3QgWDEwHhcNMTUwNjA0MTEwNDM4
WhcNMzUwNjA0MTEwNDM4WjBPMQswCQYDVQQGEwJVUzEpMCcGA1UEChMgSW50ZXJu
ZXQgU2VjdXJpdHkgUmVzZWFyY2ggR3JvdXAxFTATBgNVBAMTDElTUkcgUm9vdCBY
MTCCAiIwDQYJKoZIhvcNAQEBBQADggIPADCCAgoCggIBAK3oJHP0FDfzm54rVygc
h77ct984kIxuPOZXoHj3dcKi/vVqbvYATyjb3miGbESTtrFj/RQSa78f0uoxmyF+
0TM8ukj13Xnfs7j/EvEhmkvBioZxaUpmZmyPfjxwv60pIgbz5MDmgK7iS4+3mX6U
A5/TR5d8mUgjU+g4rk8Kb4Mu0UlXjIB0ttov0DiNewNwIRt18jA8+o+u3dpjq+sW
T8KOEUt+zwvo/7V3LvSye0rgTBIlDHCNAymg4VMk7BPZ7hm/ELNKjD+Jo2FR3qyH
B5T0Y3HsLuJvW5iB4YlcNHlsdu87kGJ55tukmi8mxdAQ4Q7e2RCOFvu396j3x+UC
B5iPNgiV5+I3lg02dZ77DnKxHZu8A/lJBdiB3QW0KtZB6awBdpUKD9jf1b0SHzUv
KBds0pjBqAlkd25HN7rOrFleaJ1/ctaJxQZBKT5ZPt0m9STJEadao0xAH0ahmbWn
OlFuhjuefXKnEgV4We0+UXgVCwOPjdAvBbI+e0ocS3MFEvzG6uBQE3xDk3SzynTn
jh8BCNAw1FtxNrQHusEwMFxIt4I7mKZ9YIqioymCzLq9gwQbooMDQaHWBfEbwrbw
qHyGO0aoSCqI3Haadr8faqU9GY/rOPNk3sgrDQoo//fb4hVC1CLQJ13hef4Y53CI
rU7m2Ys6xt0nUW7/vGT1M0NPAgMBAAGjQjBAMA4GA1UdDwEB/wQEAwIBBjAPBgNV
HRMBAf8EBTADAQH/MB0GA1UdDgQWBBR5tFnme7bl5AFzgAiIyBpY9umbbjANBgkq
hkiG9w0BAQsFAAOCAgEAVR9YqbyyqFDQDLHYGmkgJykIrGF1XIpu+ILlaS/V9lZL
ubhzEFnTIZd+50xx+7LSYK05qAvqFyFWhfFQDlnrzuBZ6brJFe+GnY+EgPbk6ZGQ
3BebYhtF8GaV0nxvwuo77x/Py9auJ/GpsMiu/X1+mvoiBOv/2X/qkSsisRcOj/KK
NFtY2PwByVS5uCbMiogziUwthDyC3+6WVwW6LLv3xLfHTjuCvjHIInNzktHCgKQ5
ORAzI4JMPJ+GslWYHb4phowim57iaztXOoJwTdwJx4nLCgdNbOhdjsnvzqvHu7Ur
TkXWStAmzOVyyghqpZXjFaH3pO3JLF+l+/+sKAIuvtd7u+Nxe5AW0wdeRlN8NwdC
jNPElpzVmbUq4JUagEiuTDkHzsxHpFKVK7q4+63SM1N95R1NbdWhscdCb+ZAJzVc
oyi3B43njTOQ5yOf+1CceWxG1bQVs5ZufpsMljq4Ui0/1lvh+wjChP4kqKOJ2qxq
4RgqsahDYVvTH9w7jXbyLeiNdd8XM2w9U/t7y0Ff/9yi0GE44Za4rF2LN9d11TPA
mRGunUHBcnWEvgJBQl9nJEiU0Zsnvgc/ubhPgXRR4Xq37Z0j4r7g1SgEEzwxA57d
emyPxgcYxn/eR44/KJ4EBs+lVDR3veyJm+kXQ99b21/+jh5Xos1AnX5iItreGCc=
-----END CERTIFICATE-----
)EOF";

const unsigned long WaitTime = 1000;

char ssid[40] = "XXXXXX";
char password[40] = "";
char ntpServerName[100] = "ntp.local.lan";
char Server[20] = "xmpp.example.com";
uint16_t port = 5222;
char tz_zone_info[40] = "SAMT-4";

WiFiClientSecure client;

char* recipient = "xxx@xmpp.example.com";
XMPP xmpp("xxx", "xxxxxx", "sensor", Server, recipient);

void debug_str( const char* intro, const char* message) 
{
	Serial.print(intro);
	Serial.print(" ");
	Serial.println(message);
	Serial.flush();
}
void setup_wifi()
{
  debug_str("Connecting to ", ssid);
  WiFi.mode(WIFI_STA); // Set ESP32 to Station mode (to connect to an AP) [1, 6]
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  { // Wait for connection
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  debug_str("WiFi connected IP address:", WiFi.localIP().toString().c_str());
}

void setup() 
{
  pinMode( INFO_LED, OUTPUT );
  delay(1000);
  Serial.begin(115200);
  while (!Serial)
    delay(10);
  delay(5000); // my usb over network
  digitalWrite( INFO_LED, HIGH);
  Serial.println("Starting ...");
  // ... WiFi connection setup ...
  setup_wifi();

  debug_str("", "Configure ntp time...");
  configTzTime(tz_zone_info, ntpServerName, "time.google.com", "pool.ntp.org");
  // Wait for time to be set
  while(time(nullptr) < 1510592825) 
  { // A timestamp after the function was introduced
    delay(100);
    Serial.print("*");
  }
  debug_str("", "\nTime synchronized");

  xmpp.setSerial(&Serial);
  xmpp.setClient(&client);

  client.setCACert(root_cert);
  // Inform the library that we want to start in plain text mode first
  client.setInsecure(); 
  client.setPlainStart();
  
  if (client.connect(Server, port)) 
  {
    debug_str("", "Connected to server in plain mode");
    // Send the STARTTLS command
    if(!xmpp.startTls())
    {
      debug_str("", "No startTLS mode");
      return;
    }
    client.startTLS();
    if(xmpp.connect())
      debug_str("", "XMPP connected");
    else
      debug_str("", "XMPP connection failed");
  } 
  else 
    debug_str("", "Connection failed");
}
void loop() 
{
  static unsigned long prev_mil = 0;
  static unsigned int counter_mil = 0;
  static uint8_t step = 0;
  unsigned long mil = millis();
  if( !step )
  {
    if( xmpp.getState() != CLOSED )
      xmpp.sendMessage(recipient, "Привет", "chat");
    step++;
  }
  if((mil - prev_mil) > WaitTime)
  {
    prev_mil = mil;
    counter_mil++;
  }
  if(counter_mil == 40 && step == 1)
  {
    if( xmpp.getState() != CLOSED )
      xmpp.sendMessage(recipient, "Пока!", "chat");
    step++;
  }
  if(counter_mil > 50 && step == 2)
  {
    digitalWrite( INFO_LED, LOW);
    xmpp.closeStream();
    client.flush();
    client.stop();
    counter_mil = 0;
    step++;
  }
}
