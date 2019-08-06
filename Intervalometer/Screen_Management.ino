/*---------------------------------------------*/
/*       VARIABLE for Screen management        */
/*---------------------------------------------*/

char Pic_count_char[8];
char Time_count_char[8];
char Interval_char[8];

/*---------------------------------------------*/
/*     FUNCTION for Screen management          */
/*---------------------------------------------*/

void Screen_Initialize(){
  u8g2.begin();

  u8g2.setFontMode(1);
  u8g2.setFont(u8g2_font_6x12_tf);
  u8g2.setDrawColor(1);

  u8g2.drawStr(40,20,"Pairing");
  u8g2.drawStr(20,30,"in progress ...");
  u8g2.drawStr(10,50,"Restart remote if");
  u8g2.drawStr(30,60,"no results");

  u8g2.sendBuffer();
}

void Screen_Displayer()
{
  u8g2.clearBuffer();          // clear the internal memory
  
  //grid
  u8g2.setDrawColor(1);
  u8g2.drawLine(42,0,42,23);
  u8g2.drawLine(85,0,85,23);
  u8g2.drawLine(0,23,128,23);

/*
  //box
  u8g2.drawLine(0,0,0,64);
  u8g2.drawLine(0,63,128,63);
  u8g2.drawLine(127,0,127,64);
  u8g2.drawLine(0,0,128,0);
*/

  //modes
  
  u8g2.setFontMode(1);
  u8g2.setFont(u8g2_font_6x12_tf);
  u8g2.setDrawColor(1);
  switch (Remote_mode)
  {
    case 0:
    
      u8g2.drawBox(0, 0, 42, 23);
      
      if (connected and TL.TimeLapse_ON){
        u8g2.drawStr(0,35,"Recording");
      } else if (connected){
        u8g2.drawStr(0,35,"Connected");
      } else {
        u8g2.drawStr(0,35,"Disconnected");
      }
      
      u8g2.drawStr(0,49,"Interval :");
      dtostrf(float(TL.Interval)/1000.0,4,1,Interval_char);
      char Interval_s_char[8];
      strcpy(Interval_s_char,Interval_char);
      strcat(Interval_s_char,"s");
      u8g2.drawStr(60,49,Interval_s_char);
      
      u8g2.drawStr(0,63,"Count :");
      itoa(TL.Pic_count,Pic_count_char,10);
      dtostrf(float(TL.Pic_count)/24.0,7,2,Time_count_char);
      u8g2.drawStr(50,63,Pic_count_char);
      char Time_count_s_char[20];
      strcpy(Time_count_s_char,Time_count_char);
      strcat(Time_count_s_char,"s");
      u8g2.drawStr(60,63,Time_count_s_char);

      break;
    case 1:
      u8g2.drawBox(43, 0, 42, 23);

      if (connected){
        u8g2.drawStr(0,35,"Connected");
      } else {
        u8g2.drawStr(0,35,"Disconnected");
      }

      break;
    case 2:
      u8g2.drawBox(86, 0, 42, 23);

      if (connected){
        u8g2.drawStr(0,35,"Connected");
      } else {
        u8g2.drawStr(0,35,"Disconnected");
      }

      u8g2.drawStr(10,50,"Connect external");
      u8g2.drawStr(10,60,"pins to trigger");
      
      break;
  }

  u8g2.setFontMode(1);
  u8g2.setFont(u8g2_font_ncenB08_tf);
  u8g2.setDrawColor(2);
  u8g2.drawStr(1,10,"Time");
  u8g2.drawStr(10,20,"lapse");
  u8g2.drawStr(46,10,"Single");
  u8g2.drawStr(55,20,"shot");
  u8g2.drawStr(87,10,"Ext.");
  u8g2.drawStr(87,20,"Trigger");
  
  u8g2.sendBuffer();          // transfer internal memory to the display
}
