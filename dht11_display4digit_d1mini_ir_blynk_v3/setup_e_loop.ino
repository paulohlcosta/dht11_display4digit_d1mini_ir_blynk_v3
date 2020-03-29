void setup()
{
  pinMode(LED_BUILTIN, OUTPUT); //D4
  digitalWrite(LED_BUILTIN, LOW); //ACENDE LED INDICADOR
  pinMode(BUTTONPIN, INPUT_PULLUP); //momentary button
  pinMode(A0, INPUT); //leitura da carga da bateria
  pinMode(IR_GND, OUTPUT); //NESSE PINO PASSA O NEGATIVO DO LED IR (por causa da posicao
  digitalWrite(IR_GND, LOW);
  display.setBrightness(0);  //7 set the diplay to maximum brightness (0-7)(OLD=0x0f)
  
  Blynk.begin(auth, ssid, pass);
  // You can also specify server, examples:
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);

  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());         // Send the IP address of the ESP8266 to the computer
  Blynk.virtualWrite((char) V0, WiFi.localIP());  //puts IP to V0

  
  dht.begin();

  // Setup a function to be called every second 
  // Esses timers contam SIMULTANEAMENTE
  timer.setInterval(30000L, sendSensor);
  timer.setInterval(2000L, showClock);

  irsend.begin();
  
  /***** MODULO WIFI E RELOGIO *****/
  Serial.begin(9600);
  Serial.println("dht11_display4digit_d1mini_ir_blynk_v3");
  configTime(timezone, dst, "pool.ntp.org","time.nist.gov");
  while(!time(nullptr)){
     Serial.print("*");
     delay(50);
  }
  digitalWrite(LED_BUILTIN, HIGH); //apaga
}

void loop()
{
  Blynk.run();
  timer.run();
  botaofisico();
}
