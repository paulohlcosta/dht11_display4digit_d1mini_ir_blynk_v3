/* RELOGIO
 * CONTROLE REMOTO DO AR CONDICIONADO PHILCO
 * CONEXAO COM BLYNK
 * TEMPERATURA E UMIDADE
 * MEDE CARGA DA BATERIA
 * aprovado nos testes
 * ----------------------------------------------------
 *  
 * placa WEMOS D1 MINI
 * ENVIA DADOS AO SERVIDOR BLYNK ATRAVES DOS PINOS VIRTUAIS V5, V6 e V7
 * FAZ A LEITURA DOS PINOS VIRTUAIS (BOTOES) V1, V2 e V3
 * NAO PODE USAR delay PQ ATRAPALHA OS TIMERS, USAR OS TIMERS
 * 
 * NECESSARIO SUBSTITUIR A BIBLIOTECA IRremote PELA IRremoteESP8266
 */

#define DHTPIN D1 // What digital pin we're connected to
#define IRPIN D7  // pino do LED infravermelho (transistor - controla o positivo)
#define CLK D2    //Set the CLK pin connection to the display
#define BUTTONPIN D3        //o pino D3 eh um dos unicos que tem PULLUP
//#define LED_BUILTIN D4    //o pino D4 (LED) eh um dos unicos que tem PULLUP
#define IR_GND D5
#define DIO D6    //Set the DIO pin connection to the display


// Comment this out to disable prints and save space 
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
#include <IRremoteESP8266.h> //ESSA BIBLIOTECA É PARA ESP8266, NAO SUPORTA receber IR
#include <IRsend.h>
IRsend irsend(IRPIN);

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "5CKxFEGca2EJz9N2m0fu-C1bMLAwBvqV"; //HomeOffice

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Sem sinal";
char pass[] = "07628702";

unsigned long previousMillis = 0;
const long interval = 2500; //to refresh the display

// Uncomment whatever type you're using!
#define DHTTYPE DHT11     // DHT 11
//#define DHTTYPE DHT22   // DHT 22, AM2302, AM2321
//#define DHTTYPE DHT21   // DHT 21, AM2301

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

float h = 0.00;
float t = 0.00;
int h_exibir = 0;
int t_exibir = 0;
int sequencia = 0;

#include <TM1637Display.h>

uint8_t carac[1];

const uint8_t SEG_FAIL[] = {
  SEG_A | SEG_E | SEG_F | SEG_G,                   // F
  SEG_A | SEG_B | SEG_C | SEG_E | SEG_F | SEG_G,   // A
  SEG_E | SEG_F,                                   // I
  SEG_D | SEG_E | SEG_F                            // L
  };

TM1637Display display(CLK, DIO);

/***** MODULO RELOGIO *****/
#include <time.h>
int timezone = 20 * 3600; //horario cuiaba
int dst = 0;

/***** BOTAO FISICO *******/
boolean comandodavez = false; //true para ligar, false para desligar
boolean lastState = true; //true is button released, false is button pressed

//---------------------------------------------CONFIGURACAO DOS PINOS VIRTUAIS (leitura)
BLYNK_WRITE(V1) {                          //NO APP CONFIGURAR COMO BOTAO MODO PUSH
  boolean state = param.asInt();           //leitura do pino virtual
  digitalWrite(LED_BUILTIN, !state);       //controle do led pelo pino V1 
}

BLYNK_WRITE(V2) {                          //NO APP CONFIGURAR COMO BOTAO MODO SWITCH
  int state = param.asInt();               //leitura do pino virtual 
  if (state) { 
    ligarAr(); 
    Blynk.virtualWrite(V2, 0);             //retorna o switch para a posição off
  } 
}
  
BLYNK_WRITE(V3) {                          //NO APP CONFIGURAR COMO BOTAO MODO SWITCH
  int state = param.asInt();               //leitura do pino virtual
  if (state) {
    desligarAr(); 
    Blynk.virtualWrite(V3, 0);             //retorna o switch para a posição off
  } 
}
