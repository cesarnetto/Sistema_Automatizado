#include "meArm.h"
#include <Servo.h>

// pinos para MeArm
int basePin = 11;
int elbowPin = 10;
int shoulderPin = 9;
int gripperPin = 6;

// pinos para sensor de cor
int s2 = 7; // pino de sensor de cor s2 para Arduino pin 7
int s3 = 8; // pino de sensor de cor s3 para Arduino pino 8
int outPin = 4; // pino de sensor de cor OUT para Arduino pino 4

// braço do MeArm: obtém parâmetros abaixo do ik_calibration
meArm arm(
      180,0, -pi/2, pi/2, // Base (-90 a 90 graus)
      125,37, pi/4, 3*pi/4, // Ombro (45 a 135 graus)
       89,44, 0, -pi/4, // Cotovelo (0 a -45 graus)
       //40,120, pi/2, 0); // Pinça aberta para fechar (90 a 0 graus)
       40,140, pi/2, 0); // Pinça aberta para fechar (90 a 0 graus)

boolean DEBUG = true;

void setup() {
  arm.begin(basePin, shoulderPin, elbowPin, gripperPin);
  pinMode(s2, OUTPUT);
  pinMode(s3, OUTPUT); 
  pinMode(outPin, INPUT); // do sensor torna-se entrada para arduino

Serial.begin(9600); // liga a porta serial

  // move o braço para a posição inicial
  arm.gotoPoint(0,80,60);
  delay(1000);
  arm.closeGripper();
  delay(100);
}

void loop() {

 // ***** pegue um token de cor *****

  arm.openGripper();

  // localização do token
  arm.gotoPoint(-131,131,-17); 
  delay(100); 

  //pegue isso
  arm.closeGripper();
  delay(100);

  // traga-o
  arm.gotoPoint(-130,130,35);

  // pegue o sensor de cor
  arm.gotoPoint(-172,0,35); 
  delay(2000);
}
  // identificar cor
  char color = get_color();

  switch(color) {
    case 'r': //vermelho
     arm.gotoPoint(70,170,50);
      delay(500); //pause
      arm.openGripper();
      break;
    case 'y':
      arm.gotoPoint(110,170,50);
      delay(500); //pause
      arm.openGripper();
      break;
    case 'g':
      arm.gotoPoint(170,170,50);
      delay(500); //pause
      arm.openGripper();
      break;
    case 'u': //não identificado. rejeitar
    arm.gotoPoint(-180,50,50);
      delay(500); //pause
      arm.openGripper();
      break;  

  delay(500);
  return;
}

char get_color() {

  int R;
  int G;
  int B;

  char color;

  // lê o componente vermelho
  digitalWrite(s2, LOW);
  digitalWrite(s3, LOW);
  R = pulseIn(outPin, LOW);

  // lê componente verde
  digitalWrite(s2, HIGH);
  digitalWrite(s3, HIGH);
  G = pulseIn(outPin, LOW);

  // vamos ler o componente azul
  digitalWrite(s2, LOW);
  digitalWrite(s3, HIGH);
  B = pulseIn(outPin, LOW);
}  

  // *********** calibração requerida ***************
  // O sensor de cor é muito sensível à luz ambiente. Você deve calibrar o seguinte intervalo de valores para chips verdes, vermelhos e amarelos.
  // Os seguintes valores são obtidos quando um token de cor é colocado em appprox 1 polegada do sensor
  //Cor
  // Os valores RGB para tokens são:
  // vermelho: 30<R<60, 70<G<120, 50<B<90
  // amarelo: 10<R<25, 10<G<40, 20<B<50
  // verde: 40<R<70, 20<G<50, 20<B<50 

if((R>40) && (R<85) && (G>90) && (G<220) && (B>70) && (B<160)){    // red
    {
      if (DEBUG) Serial.println("Color is RED");
      color = 'r';
    }
  else if((R>10) && (R<30) && (G>10) && (G<40) && (B>15) && (B<50))  // yellow
    {
      if (DEBUG) Serial.println("Color is YELLOW");
      color = 'y';
    }
  else if((R>60) && (R<130) && (G>20) && (G<70) && (B>30) && (B<80))  // green
    {
      Serial.println("Color is GREEN"); 
      color = 'g';
    }
  else 
    {  
      if (DEBUG) {
        Serial.print("R = ");
        Serial.print(R);
        Serial.print("  G = ");
        Serial.print(G);
        Serial.print("  B = ");
        Serial.println(B);
      } 
      color = 'u'; //não identificado
    }
  return color;
}       
