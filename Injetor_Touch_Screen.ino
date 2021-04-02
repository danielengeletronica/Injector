#include <TimerOne.h>
#include <MCUFRIEND_kbv.h>
#include <Adafruit_GFX.h> //Biblioteca grafica
#include <TouchScreen.h>  //Biblioteca Touch

#define YP A3 // Y+ is on Analog1
#define XM A2 // X- is on Analog2
#define YM 9 // Y- is on Digital7
#define XP 8 // X+ is on Digital6

#define TS_MINX 118
#define TS_MINY 92
#define TS_MAXX 906
#define TS_MAXY 951

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

#define LCD_RESET A4 //Pode ser conectado ao pino reset do Arduino
#define LCD_CS A3   // Chip Select
#define LCD_CD A2  // Command/Data
#define LCD_WR A1  // LCD Write
#define LCD_RD A0 // LCD Read

//Definicao de cores
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

MCUFRIEND_kbv tft;

#define MINPRESSURE 10
#define MAXPRESSURE 1000

int menu = 1;

float potenciometro = 0;
float tensao_pot = 0;
int barra = 0;
int barra2 = 0;

float rpm = 0;
float rps = 0;
float tempo = 0;                            
int contador = 0;


void interrupcao()
{
  contador = contador + 1;                      

  if (menu == 4) //Sinal 60-2 Hall---->                    
  {
    if(contador >= 0 && contador <= 116)                   
    {
      digitalWrite(31,!digitalRead(31));         
      digitalWrite(33,HIGH);                      
    }
    if(contador >= 116)                           
    {
      digitalWrite(31,LOW);                       
      digitalWrite(33,LOW);                        
      if(contador >= 119)
      {
        contador = 0;                             
      } 
    }    
  }

  if (menu == 8) //Sinal 44-4 Hall---->
  {
    if(contador >= 0 && contador <= 80)
    {
      digitalWrite(31,!digitalRead(31));
      digitalWrite(33,HIGH);
    }
    if(contador >= 80)
    {
      digitalWrite(31,LOW);
      digitalWrite(33,LOW);
      if(contador >= 87)
      {
        contador = 0;
      } 
    }    
  }

  if (menu == 9) //Sinal 60-2 Indutivo---->
  {
    if (contador >= 0 && contador <= 116)
    {
      digitalWrite(31, !digitalRead(31));
      if(digitalRead(31) == 1)
      {
        digitalWrite(35,LOW);
      } 
      if(digitalRead(31) == 0)
      {
        digitalWrite(35,HIGH);  
      }
    }
    if (contador > 116)
    {
      digitalWrite(31, LOW);
      digitalWrite(35,LOW);
      
      
      if (contador >= 119)
      {
        contador = 0;
      }
    }
  }
  
  if (menu == 27) //Sinal 44-4 Indutivo---->
  {
    if (contador >= 0 && contador <= 80)
    {
      digitalWrite(31, !digitalRead(31));
      if(digitalRead(31) == 1)
      {
        digitalWrite(35,LOW);
      } 
      if(digitalRead(31) == 0)
      {
        digitalWrite(35,HIGH);  
      }
    }
    if (contador > 80)
    {
      digitalWrite(31, LOW);
      digitalWrite(35,LOW);
      
      
      if (contador >= 87)
      {
        contador = 0;
      }
    }
  }
   


}


void setup()
{
  pinMode(A7, INPUT); 
  pinMode(31,OUTPUT);
  pinMode(33,OUTPUT);
  pinMode(35,OUTPUT);

  tft.reset();

  uint16_t identifier = tft.readID();

  tft.begin(identifier);
  tft.fillScreen(BLACK);
  tft.setRotation(1);
  delay(1000);

  Timer1.initialize(200000);                               
  Timer1.attachInterrupt(interrupcao); 

  tft.setTextColor(WHITE);
  tft.setTextSize(3);
  tft.setCursor(40, 140);
  tft.println("MV injecao eletronica");

  delay(2000); 
  tft.fillScreen(BLACK);
  menu = 1;
}


void loop()
{
  int fabricante1=0;
  int fabricante2=0;
  
   while(menu == 1)
  {
   
    TSPoint p = ts.getPoint();
    pinMode(XM, OUTPUT);
    digitalWrite(XM, LOW);
    pinMode(YP, OUTPUT);
    digitalWrite(YP, HIGH);
    pinMode(YM, OUTPUT);
    digitalWrite(YM, LOW);
    pinMode(XP, OUTPUT);
    digitalWrite(XP, HIGH);
   
    tft.drawRoundRect(15, 15, 240, 50, 5, WHITE);
    tft.drawRoundRect(15, 75, 240, 50, 5, WHITE);
    tft.drawRoundRect(350, 255, 50, 50, 5, WHITE);
    p.x = map(p.x, TS_MAXX, TS_MINX, 0, 320);
    p.y = map(p.y, TS_MAXY, TS_MINY, 0, 480);

    if(fabricante1 == 0)
    {
       tft.setTextColor(RED);
       tft.setTextSize(3);
       tft.setCursor(25, 27);
       tft.print("fabricante 1");  
    }  
    if(fabricante1 == 1)
    {
       tft.setTextColor(YELLOW);
       tft.setTextSize(3);
       tft.setCursor(25, 27);
       tft.print("fabricante 1");  
    }

    if(fabricante2 == 0)
    {
       tft.setTextColor(RED);
       tft.setTextSize(3);
       tft.setCursor(25, 87);
       tft.print("fabricante 2");  
    }  
    if(fabricante2 == 1)
    {
       tft.setTextColor(YELLOW);
       tft.setTextSize(3);
       tft.setCursor(25, 87);
       tft.print("fabricante 2");  
    }
 
    tft.setTextColor(RED);
    tft.setTextSize(3);
    tft.setCursor(368, 270);
    tft.print("S");  
   
  
    if (p.z > MINPRESSURE && p.z < MAXPRESSURE)
    {

       if(p.x > 15 && p.x < 50 && p.y > 15 && p.y < 240)
       {
          delay(100);
          fabricante1 = !fabricante1; 
          fabricante2 = 0;
          
       }
       if(p.x > 75 && p.x < 110 && p.y > 15 && p.y < 240)
       {
          delay(100);
          fabricante2 = !fabricante2;
        fabricante1 =0;
       }
       
       
       if(p.x > 255 && p.x < 305 && p.y > 350 && p.y < 400)
       {
          delay(100);
          if(fabricante1 == 1){menu = 2;tft.fillScreen(BLACK); fabricante1 = 0;}
          if(fabricante2 == 1){menu = 3;tft.fillScreen(BLACK); fabricante2 = 0;}
       }
       
     }
  
  }
  
  while(menu == 2)
  {
    TSPoint p = ts.getPoint();
    pinMode(XM, OUTPUT);
    digitalWrite(XM, LOW);
    pinMode(YP, OUTPUT);
    digitalWrite(YP, HIGH);
    pinMode(YM, OUTPUT);
    digitalWrite(YM, LOW);
    pinMode(XP, OUTPUT);
    digitalWrite(XP, HIGH);
   
    tft.drawRoundRect(15, 15, 240, 50, 5, WHITE);
    tft.drawRoundRect(15, 75, 240, 50, 5, WHITE);
    tft.drawRoundRect(350, 255, 50, 50, 5, WHITE);
    
    p.x = map(p.x, TS_MAXX, TS_MINX, 0, 320);
    p.y = map(p.y, TS_MAXY, TS_MINY, 0, 480);

    if(fabricante1 == 0)
    {
       tft.setTextColor(RED);
       tft.setTextSize(3);
       tft.setCursor(25, 27);
       tft.print("modelo 1");  
    }  
    if(fabricante1 == 1)
    {
       tft.setTextColor(YELLOW);
       tft.setTextSize(3);
       tft.setCursor(25, 27);
       tft.print("modelo 1");  
    }

    if(fabricante2 == 0)
    {
       tft.setTextColor(RED);
       tft.setTextSize(3);
       tft.setCursor(25, 87);
       tft.print("modelo 2");  
    }  
    if(fabricante2 == 1)
    {
       tft.setTextColor(YELLOW);
       tft.setTextSize(3);
       tft.setCursor(25, 87);
       tft.print("modelo 2");  
    }
 
    tft.setTextColor(RED);
    tft.setTextSize(3);
    tft.setCursor(368, 270);
    tft.print("S");  
   
  
    if (p.z > MINPRESSURE && p.z < MAXPRESSURE)
    {

       if(p.x > 15 && p.x < 50 && p.y > 15 && p.y < 240)
       {
          delay(100);
          fabricante1 = !fabricante1; 
          fabricante2 = 0;
          
       }
       if(p.x > 75 && p.x < 110 && p.y > 15 && p.y < 240)
       {
          delay(100);
          fabricante2 = !fabricante2;
          fabricante1 =0;
       }
       
       
       if(p.x > 255 && p.x < 305 && p.y > 350 && p.y < 400)
       {
          delay(100);
          if(fabricante1 == 1){menu = 4;tft.fillScreen(BLACK);fabricante1 = 0;}
          if(fabricante2 == 1){menu = 8;tft.fillScreen(BLACK);fabricante2 = 0;}
       }
       if(p.x > 255 && p.x < 305 && p.y > 285 && p.y < 335)
       {
          delay(100);
          tft.fillScreen(BLACK);
          menu = 1;
       }
       
     }  
  }
  
  while(menu == 3)
  {
    TSPoint p = ts.getPoint();
    pinMode(XM, OUTPUT);
    digitalWrite(XM, LOW);
    pinMode(YP, OUTPUT);
    digitalWrite(YP, HIGH);
    pinMode(YM, OUTPUT);
    digitalWrite(YM, LOW);
    pinMode(XP, OUTPUT);
    digitalWrite(XP, HIGH);
   
    tft.drawRoundRect(15, 15, 240, 50, 5, WHITE);
    tft.drawRoundRect(15, 75, 240, 50, 5, WHITE);
    tft.drawRoundRect(350, 255, 50, 50, 5, WHITE);
    
    p.x = map(p.x, TS_MAXX, TS_MINX, 0, 320);
    p.y = map(p.y, TS_MAXY, TS_MINY, 0, 480);

    if(fabricante1 == 0)
    {
       tft.setTextColor(RED);
       tft.setTextSize(3);
       tft.setCursor(25, 27);
       tft.print("modelo 3");  
    }  
    if(fabricante1 == 1)
    {
       tft.setTextColor(YELLOW);
       tft.setTextSize(3);
       tft.setCursor(25, 27);
       tft.print("modelo 3");  
    }

    if(fabricante2 == 0)
    {
       tft.setTextColor(RED);
       tft.setTextSize(3);
       tft.setCursor(25, 87);
       tft.print("modelo 4");  
    }  
    if(fabricante2 == 1)
    {
       tft.setTextColor(YELLOW);
       tft.setTextSize(3);
       tft.setCursor(25, 87);
       tft.print("modelo 4");  
    }
 
    tft.setTextColor(RED);
    tft.setTextSize(3);
    tft.setCursor(368, 270);
    tft.print("S");  
    
    tft.setTextColor(RED);
    tft.setTextSize(3);
    tft.setCursor(295, 270);
    tft.print("<-");
   
  
    if (p.z > MINPRESSURE && p.z < MAXPRESSURE)
    {

       if(p.x > 15 && p.x < 50 && p.y > 15 && p.y < 240)
       {
          delay(100);
          fabricante1 = !fabricante1; 
          fabricante2 = 0;
          
       }
       if(p.x > 75 && p.x < 110 && p.y > 15 && p.y < 240)
       {
          delay(100);
          fabricante2 = !fabricante2;
        fabricante1 =0;
       }
       
       
       if(p.x > 255 && p.x < 305 && p.y > 350 && p.y < 400)
       {
          delay(100);
          if(fabricante1 == 1){menu = 9;tft.fillScreen(BLACK);fabricante1=0;}
          if(fabricante2 == 1){menu = 27;tft.fillScreen(BLACK);fabricante2=0;}
       }
       
       if(p.x > 255 && p.x < 305 && p.y > 285 && p.y < 335)
       {
          delay(100);
          tft.fillScreen(BLACK);
          menu = 1;
       }   
    
     }  
  }
  
  while(menu == 4)
  {
    
    
    TSPoint p = ts.getPoint();
    pinMode(XM, OUTPUT);
    digitalWrite(XM, LOW);
    pinMode(YP, OUTPUT);
    digitalWrite(YP, HIGH);
    pinMode(YM, OUTPUT);
    digitalWrite(YM, LOW);
    pinMode(XP, OUTPUT);
    digitalWrite(XP, HIGH);

    p.x = map(p.x, TS_MAXX, TS_MINX, 0, 320);
    p.y = map(p.y, TS_MAXY, TS_MINY, 0, 480);
    
    tft.drawRoundRect(415, 255, 50, 50, 5, WHITE);
    
    tft.setTextColor(YELLOW);
    tft.setTextSize(3);
    tft.setCursor(425, 270);
    tft.print("<-");

    tft.fillRoundRect(40, 106, barra, 106, 5,  RED);
    tft.fillRoundRect(barra2, 106, (400-barra), 106, 5,  BLACK);
    tft.drawRoundRect(40, 10, 400, 50, 5, WHITE);
    
    tft.setTextColor(BLUE);
    tft.setTextSize(2);
    tft.setCursor(50, 20);
    tft.print("Modelo 1 (Hall 60-2)");    

    float potenciometro = analogRead(A7);
    delay(10);
    tensao_pot = ((5.0 / 1023.0) * (potenciometro) * 400.0);

    barra = map(tensao_pot,0,2000,0,400);
    barra2 = 40 + barra;

    rpm = 10.0 + tensao_pot;                                       

    rps = rpm / 60.0;
    tempo = (1 / (72.0 * rps)) * 1000000.0;                          
    Timer1.setPeriod(tempo);
  
    if (p.z > MINPRESSURE && p.z < MAXPRESSURE)
    {
       if(p.x > 255 && p.x < 305 && p.y > 415 && p.y < 465)
       {
          delay(100);
          tft.fillScreen(BLACK);
          menu = 1;
       }
    }
  }          

  while(menu == 8)
  {
    
    TSPoint p = ts.getPoint();
    pinMode(XM, OUTPUT);
    digitalWrite(XM, LOW);
    pinMode(YP, OUTPUT);
    digitalWrite(YP, HIGH);
    pinMode(YM, OUTPUT);
    digitalWrite(YM, LOW);
    pinMode(XP, OUTPUT);
    digitalWrite(XP, HIGH);

    p.x = map(p.x, TS_MAXX, TS_MINX, 0, 320);
    p.y = map(p.y, TS_MAXY, TS_MINY, 0, 480);
    
    tft.drawRoundRect(415, 255, 50, 50, 5, WHITE);
    
    tft.setTextColor(YELLOW);
    tft.setTextSize(3);
    tft.setCursor(425, 270);
    tft.print("<-");

    tft.fillRoundRect(40, 106, barra, 106, 5,  RED);
    tft.fillRoundRect(barra2, 106, (400-barra), 106, 5,  BLACK);
    tft.drawRoundRect(40, 10, 400, 50, 5, WHITE);
    
    tft.setTextColor(BLUE);
    tft.setTextSize(2);
    tft.setCursor(50, 20);
    tft.print("Modelo 2 Hall 44 - 4"); 
       

    float potenciometro = analogRead(A7);
    delay(10);
    tensao_pot = ((5.0 / 1023.0) * (potenciometro) * 400.0);

    barra = map(tensao_pot,0,2000,0,400);
    barra2 = 40 + barra;

    rpm = 10.0 + tensao_pot;                                       

    rps = rpm / 60.0;
    tempo = (1 / (72.0 * rps)) * 1000000.0;                          
    Timer1.setPeriod(tempo);
  
    if (p.z > MINPRESSURE && p.z < MAXPRESSURE)
    {
       if(p.x > 255 && p.x < 305 && p.y > 415 && p.y < 465)
       {
          delay(100);
          tft.fillScreen(BLACK);
          menu = 1;
       }
    }
  } 
  
  while(menu == 9)
  {
  
  
    TSPoint p = ts.getPoint();
    pinMode(XM, OUTPUT);
    digitalWrite(XM, LOW);
    pinMode(YP, OUTPUT);
    digitalWrite(YP, HIGH);
    pinMode(YM, OUTPUT);
    digitalWrite(YM, LOW);
    pinMode(XP, OUTPUT);
    digitalWrite(XP, HIGH);

    p.x = map(p.x, TS_MAXX, TS_MINX, 0, 320);
    p.y = map(p.y, TS_MAXY, TS_MINY, 0, 480);
    
    tft.drawRoundRect(415, 255, 50, 50, 5, WHITE);
    
    tft.setTextColor(YELLOW);
    tft.setTextSize(3);
    tft.setCursor(425, 270);
    tft.print("<-");

    tft.fillRoundRect(40, 106, barra, 106, 5,  RED);
    tft.fillRoundRect(barra2, 106, (400-barra), 106, 5,  BLACK);
    tft.drawRoundRect(40, 10, 400, 50, 5, WHITE);
    
    tft.setTextColor(BLUE);
    tft.setTextSize(2);
    tft.setCursor(50, 20);
    tft.print("Modelo 1 Ind 60-2");    
    
    float potenciometro = analogRead(A7);
    delay(10);
    tensao_pot = ((5.0 / 1023.0) * (potenciometro) * 400.0);

    barra = map(tensao_pot,0,2000,0,400);
    barra2 = 40 + barra;

    rpm = 10.0 + tensao_pot;                                       

    rps = rpm / 60.0;
    tempo = (1 / (72.0 * rps)) * 1000000.0;                          
    Timer1.setPeriod(tempo);
  
    if (p.z > MINPRESSURE && p.z < MAXPRESSURE)
    {
       if(p.x > 255 && p.x < 305 && p.y > 415 && p.y < 465)
       {
          delay(100);
          tft.fillScreen(BLACK);
          menu = 1;
       }
    }
  }
  
  while(menu == 27)
  {
    
    TSPoint p = ts.getPoint();
    pinMode(XM, OUTPUT);
    digitalWrite(XM, LOW);
    pinMode(YP, OUTPUT);
    digitalWrite(YP, HIGH);
    pinMode(YM, OUTPUT);
    digitalWrite(YM, LOW);
    pinMode(XP, OUTPUT);
    digitalWrite(XP, HIGH);

    p.x = map(p.x, TS_MAXX, TS_MINX, 0, 320);
    p.y = map(p.y, TS_MAXY, TS_MINY, 0, 480);
    
    tft.drawRoundRect(415, 255, 50, 50, 5, WHITE);
    
    tft.setTextColor(YELLOW);
    tft.setTextSize(3);
    tft.setCursor(425, 270);
    tft.print("<-");

    tft.fillRoundRect(40, 106, barra, 106, 5,  RED);
    tft.fillRoundRect(barra2, 106, (400-barra), 106, 5,  BLACK);
    tft.drawRoundRect(40, 10, 400, 50, 5, WHITE);
    
    tft.setTextColor(BLUE);
    tft.setTextSize(2);
    tft.setCursor(50, 20);
    tft.print("Modelo 2 Indutivo 44 - 4");   

    float potenciometro = analogRead(A7);
    delay(10);
    tensao_pot = ((5.0 / 1023.0) * (potenciometro) * 400.0);

    barra = map(tensao_pot,0,2000,0,400);
    barra2 = 40 + barra;

    rpm = 10.0 + tensao_pot;                                       

    rps = rpm / 60.0;
    tempo = (1 / (72.0 * rps)) * 1000000.0;                          
    Timer1.setPeriod(tempo);
  
    if (p.z > MINPRESSURE && p.z < MAXPRESSURE)
    {
       if(p.x > 255 && p.x < 305 && p.y > 415 && p.y < 465)
       {
          delay(100);
          tft.fillScreen(BLACK);
          menu = 1;
       }
    }
  }
  
}
