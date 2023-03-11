//процедура обработки звонка и смс
//-----------------------------------------------------------------------------
void incoming_call_sms()
{
  byte ch = 0;
  delay(200);
  while (mySerial.available()) {         //сохраняем входную строку в переменную val
    ch = mySerial.read();
    val += char(ch);
    delay(10);
  }
  //----------------------- определение факта приема СМС и сравнение номера с заданным
  if (val.indexOf("+CMT") > -1)           //если обнаружен СМС (для определения звонка вместо "+CMT" вписать "RING", трубку он не берет, но реагировать на факт звонка можно)
  {

    // для очистки памяти от старых смс-ок, очистка есть при перезапуске, но скидывать клему не хочется
    if ((val.indexOf(MASTER) > -1 || val.indexOf(MASTER2) > -1) && val.indexOf("delete sms") > -1) //если СМС от хозяина и содержит текст запроса
    {
      if (val.indexOf(MASTER) > -1) sms("Delete SMS OK", MASTER); // отвечаем смской
      else sms("Delete SMS OK", MASTER2); // отвечаем смской
      val = "";
      delay(1000);
      sendAtCmd("AT+CMGD=1,4");
    }
    else if ((val.indexOf(MASTER) > -1 || val.indexOf(MASTER2) > -1) && val.indexOf("relay on") > -1 && state == false) //если СМС от хозяина и содержит текст запроса
    {
      digitalWrite(power, HIGH);
      if (val.indexOf(MASTER) > -1) sms("RELAY ON OK", MASTER); // отвечаем смской
      else sms("RELAY ON OK", MASTER2); // отвечаем смской
      val = "";
      state = true;
      digitalWrite(LED, state);
      EEPROM.update(1,state);
      timer = 0;
      EEPROM.update(2,1);
    }
    else if ((val.indexOf(MASTER) > -1 || val.indexOf(MASTER2) > -1) && val.indexOf("relay off") > -1 && state == true) //если СМС от хозяина и содержит текст запроса
    {
      digitalWrite(power, LOW);
      if (val.indexOf(MASTER) > -1) sms("RELAY OFF OK", MASTER); // отвечаем смской
      else sms("RELAY OFF OK", MASTER2); // отвечаем смской
      val = "";
      state = false;
      digitalWrite(LED, state);
      EEPROM.update(1,state);
      timer = 0;
      EEPROM.update(2,1);
    }
    
    #ifdef USE_TIMER
    else if ((val.indexOf(MASTER) > -1 || val.indexOf(MASTER2) > -1) && val.indexOf("timer ") > -1)
      {
      String timerTmp = val.substring(54);
      timer = timerTmp.toInt();
      //Serial.println(timer);
      timerTmp = (String)timer;
      timer = timer*60*1000+millis();
      if(timer != 0) {
        digitalWrite(power, HIGH);
        state = true;
        digitalWrite(LED, state);
        EEPROM.update(1,state);
        EEPROM.update(2,0);
        if (val.indexOf(MASTER) > -1) sms("TIMER ON " + timerTmp + " MIN", MASTER); // отвечаем смской
        else sms("TIMER ON " + timerTmp + " MIN", MASTER2); // отвечаем смской
        }
        val = "";
      }
    #endif

    else if ((val.indexOf(MASTER) > -1 || val.indexOf(MASTER2) > -1) && val.indexOf("temper") > -1) //если СМС от хозяина и содержит текст запроса температуры
    {
      String message = (String)currentTemper();
      if (val.indexOf(MASTER) > -1) sms("Temperature: " + message + "'C", MASTER); // в СМС отправляем текущую температуру
      else sms("Temperature: " + message + "'C", MASTER2); // в СМС отправляем текущую температуру
      val = "";
    }

    #ifdef USE_TERMOSTAT
    else if ((val.indexOf(MASTER) > -1 || val.indexOf(MASTER2) > -1) && val.indexOf("termostat ") > -1)
      {
      String heatTmp = val.substring(58);
      heaterVal = heatTmp.toInt();
      //Serial.println(heaterVal);
      EEPROM.update(3,heaterVal);
      if(heaterVal != 0) {
        String message = (String)heaterVal; // Массив для вывода
        if (val.indexOf(MASTER) > -1) sms("TERMOSTAT ON " + message + "'C", MASTER); // отвечаем смской
        else sms("TERMOSTAT ON " + message + "'C", MASTER2); // отвечаем смской
        }
        val = "";
      }
    #endif
      
    else if (val.indexOf("new master") > -1)
      {
      MASTER = val.substring(10, 21);
      //Serial.println(MASTER);
      update_master_eeprom(10);
      sms("Master Nomer izmenen", MASTER); // отвечаем смской
      val = "";
      }

    else if (val.indexOf("new master2") > -1)
      {
      MASTER2 = val.substring(10, 21);
      //Serial.println(MASTER2);
      update_master2_eeprom(30);
      sms("Master Nomer izmenen", MASTER2); // отвечаем смской
      val = "";
      }

    #ifdef USE_readNumberSIM
    else if (val.indexOf("SIM master N") > -1)
      {
      val="";
      readNumberSIM();
      //Serial.println(MASTER);
      sms("Master Nomer izmenen", MASTER); // отвечаем смской
      }
      #endif
      
    else
    {
      val = "";
    }
  }
  else if (val.indexOf("RING") > -1)           //если обнаружен звонок
  {
    {
      if (val.indexOf(MASTER) > -1 || val.indexOf(MASTER2) > -1)         //если звонок от хозяина

      {
        delay(500); // выждем, чтоб модем перешел в режим ожидания, иначе просадка напряжения возможна и перезагрузка
        if (state == LOW)
        {
          digitalWrite(power, HIGH);
          //delay(100);
          //char message[15] = "RELAY ON OK"; // Массив для вывода
          //sms(message, MASTER); // отвечаем смской
          val = "";
          delay(400);
          state = true;
          digitalWrite(LED, state);
          EEPROM.update(1,state);
          timer = 0;
          EEPROM.update(2,1);
        }
        else if (state == HIGH)
        {
          digitalWrite(power, LOW);
          //delay(100);
          //char message[15] = "RELAY OFF OK"; // Массив для вывода
          //sms(message, MASTER); // отвечаем смской
          val = "";
          delay(400);
          state = false;
          digitalWrite(LED, state);
          EEPROM.update(1,state);
          timer = 0;
          EEPROM.update(2,1);
        }
        sendAtCmd("ATH0");     // сбросить вызов

      }
      else
      {
        delay(10500);
        sendAtCmd("ATH");
      }
    }
  }
  val = "";
}

