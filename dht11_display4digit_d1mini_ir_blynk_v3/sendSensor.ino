// This function sends Arduino's up time every second to Virtual Pin (5).
// In the app, Widget's reading frequency should be set to PUSH. This means
// that you define how often to send data to Blynk App.
void sendSensor()
{
  h = dht.readHumidity();
  t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit

  if(isnan(h) || isnan(t)) {
    display.setSegments(SEG_FAIL);
    Serial.println("Failed to read from DHT sensor!");
    delay(50);
    return;
  } else {
    t_exibir = (int) t;       //atualiza dados para exibir no Serial e no display
    h_exibir = (int) h; 
        
    Serial.print(t_exibir);
    Serial.print(" C ");  
    Serial.print(h_exibir);
    Serial.println("% de umidade. Enviando para servidor Blynk...");
    
  // You can send any value at any time.
  // Please don't send more that 10 values per second.    
    Blynk.virtualWrite((int) V5, h);
    Blynk.virtualWrite((int) V6, t);

  }
}
