
void showClock() 
{ 
  if(sequencia == 0) {
    display.showNumberDec(t_exibir, false);
      carac[0] = 0b01111000;  // LETRA t 
      display.setSegments(carac, 1, 0); //segSegments(array, número de itens, posicao)
  }

  if(sequencia == 1) {
    display.showNumberDec(h_exibir, false);
      carac[0] = 0b00011100;  // LETRA u
      display.setSegments(carac, 1, 0); 
  }

/***** MODULO RELOGIO *****/
  if(sequencia > 1) {
    time_t now = time(nullptr);
    struct tm* p_tm = localtime(&now);
    Serial.print(p_tm->tm_hour);
    Serial.print(":");
    Serial.println(p_tm->tm_min);    
  
    uint8_t data[] = {0x0, 0x0, 0x0, 0x0}; 
    display.setSegments(data);    
    //display.showNumberDec(,true,4,3); //true leading zeros, exibir 4 digitos, comeca na casa 3
    display.showNumberDecEx((p_tm->tm_hour)*100 + (p_tm->tm_min),64,true); //mostrar os dois pontos colon
    
    if(sequencia == 4) sequencia = -1;
  }
  sequencia++;


//---------------------------------------------------TENSAO DA BATERIA 
  float voltage = (float) analogRead(A0)*(2.0 * 3.3 / 1023.0);
  float v_min = 2.5; //tensao da bateria 0%
  float v_max = 4.2; //tensao da bateria 100%
  int bat = (voltage - v_min) * 100 / (v_max - v_min); //percentual

  Blynk.virtualWrite((int) V7, bat);
  Serial.print(voltage);
  Serial.print(" V na bateria = ");
  Serial.print(bat);
  Serial.println("% de carga; Blynk atualizado.");
}
