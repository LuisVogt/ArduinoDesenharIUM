#include <Stepper.h>
#include <Servo.h>

const int stepsPerRevolution = 500;
const int passo_min = 1;
const int passos_para_volta_completa = 2000;
const int velocidade = 2000;
const int number_of_sides_circle = 16;
//com essa configuração aproximadamente 2000 passos são necessários para uma volta completa

//Inicializa a biblioteca utilizando as portas de 8 a 11 para
//ligacao ao motor
Stepper myStepper(stepsPerRevolution, A1, A3, A2, A4);
Stepper myStepper2(stepsPerRevolution, 9, 11, 10, 12);
Servo s;
int porta_servo = 2;
int posicao_atual_x = 0;
int posicao_atual_y = 0;

void setup()
{
  Serial.begin(9600);
  //Determina a velocidade inicial do motor
  myStepper.setSpeed(60);
  myStepper2.setSpeed(60);
  s.attach(porta_servo);
  s.write(0);
}

void lowerServo()
{
  s.write(180);
  /*for(int i = 0; i < 90; i++)
    {
    s.write(i);
    }*/
}

void raiseServo()
{
  s.write(0);
  /*  for(int i = 90; i >= 0; i--)
    {
    s.write(i);
    }*/
}

void goToPoint(int x, int y)
{
  int vetor_x = x - posicao_atual_x;
  int vetor_y = y - posicao_atual_y;
  stepVetor(vetor_x, vetor_y);
}

void drawPolygonByRadius(int centerX, int centerY, int radius, int n_sides, float rotate_angle = 0, int sides_to_draw = 9999, bool onBase = true)
{
  //Essa função desenha os polígonos achando os vértices através de um centro e um raio.
  //O ângulo calculado é o ângulo entre o ponto central e dois vértices adjacentes quaisquer em radianos.
  float angle = (2 * PI) / n_sides;
  //3*PI/2 é 270°, e somar metade do ângulo do polinômio garante que um dos lados dele sempre esteja alinhado com o eixo x.
  //Somar um ângulo escolhido pelo usuário permite que ele rotacione o polinômio conforme seja necessário.
  float start_angle;
  if (onBase)
    start_angle = (3 * PI / 2) + (angle / 2) + rotate_angle;
  else
    start_angle = rotate_angle;
  //Cálculo dos pontos iniciais. Eles são guardados em variáveis diferentes dos pontos subsequentes para permitir que o programa
  //una o último vertice ao primeiro sem se preocupar com erros de ponto flutuantes.
  int init_x = centerX + cos(start_angle) * radius;
  int init_y = centerY + sin(start_angle) * radius;

  int next_x = 0;
  int next_y = 0;

  //Leva os eixos para o ponto inicial com a caneta levanta e em seguida a abaixa.
  raiseServo();
  stepVetor(init_x - posicao_atual_x, init_y - posicao_atual_y);
  lowerServo();

  if (sides_to_draw > n_sides)
    sides_to_draw = n_sides;

  for (int i = 1; i <= sides_to_draw ; i++)
  {
    next_x = centerX + cos(start_angle + angle * i) * radius;
    next_y = centerY + sin(start_angle + angle * i) * radius;

    stepVetor(next_x - posicao_atual_x, next_y - posicao_atual_y);
  }
  if (n_sides == sides_to_draw)
    stepVetor(init_x - posicao_atual_x, init_y - posicao_atual_y);
}

void drawPolygonBySide(int centerX, int centerY, int side, int n_sides, float rotate_angle = 0, int sides_to_draw = 9999, bool onBase = true)
//Calcula o raio do polígono baseado no tamanho do lado e chama a função de desenhar polígono
{
  float angle_center = (2 * PI) / n_sides;
  int radius = (side / 2) / sin(angle_center / 2);

  drawPolygonByRadius(centerX, centerY, radius, n_sides, rotate_angle, sides_to_draw, onBase);

}

void drawLineByCoordinate(int startX, int startY, int endX, int endY)
{
  raiseServo();

  stepVetor(startX - posicao_atual_x, startY - posicao_atual_y);

  lowerServo();

  stepVetor(endX - startX, endY - startY);
}

void drawLineByAngle(int startX, int startY, int distance, int angle)
{
  int endX = startX + distance * cos(angle);
  int endY = startY + distance * sin(angle);

  drawLineByCoordinate(startX, startY, endX, endY);
}

void drawPoint(int X, int Y)
{
  raiseServo();
  stepVetor(X - posicao_atual_x, Y - posicao_atual_y);
  lowerServo();
}
//void drawCircle(int centerX, int centerY, int radius)
//{
//  Serial.println("Start circle");
//  raiseServo();
//  goToPoint(centerX/2+radius,centerY);
//  lowerServo();
//  for(float t = 0; t < PI; t += 0.05)
//  {
////    Serial.println("cos,center,current:");
////    Serial.print(cos(t));
////    Serial.print("///");
////    Serial.print(centerX);
////    Serial.print("///");
////    Serial.println(posicao_atual_x);
//
////    Serial.println("sen,center,current:");
////    Serial.print(sin(t));
////    Serial.print("///");
////    Serial.print(centerY);
////    Serial.print("///");
////    Serial.println(posicao_atual_y);
//
//    stepVetor((centerX + radius*cos(t)-posicao_atual_x), (centerY + radius*sin(t)-posicao_atual_y));
//    //stepVetor((centerX + radius*cos(t)-posicao_atual_x), 0);
//  }
//  raiseServo();
//  Serial.println("End circle");
//}

void testServo()
{
  for (int i = 0; i < 180; i++)
  {
    s.write(i);
    delay(50);
  }
}

void stepVetor(int x, int y)
{
  posicao_atual_x += x;
  posicao_atual_y += y;
  int temp_max;
  int temp_div;
  Stepper* main_stepper;
  Stepper* secondary_stepper;

  bool zeroSecondary = false;

  int negativeX = 1;
  int negativeY = 1;
  int negativeMain;
  int negativeSecondary;
  if (x < 0) {
    negativeX = -1;
  }
  if (y < 0) {
    negativeY = -1;
  }

  if (x == 0 && y == 0)
  {
    return;
  }
  else if (x == 0 || y == 0)
  {
    zeroSecondary = true;
  }

  if (abs(x) > abs(y))
  {
    temp_max = abs(x);
    temp_div = abs(x) / abs(y);

    main_stepper = &myStepper;
    secondary_stepper = &myStepper2;

    negativeMain = negativeX;
    negativeSecondary = negativeY;
  }
  else
  {
    temp_max = abs(y);
    temp_div = abs(y) / abs(x);

    secondary_stepper = &myStepper;
    main_stepper = &myStepper2;

    negativeSecondary = negativeX;
    negativeMain = negativeY;

  }



  for (int i = 0; i < temp_max; i++)
  {
    main_stepper->step(passo_min * negativeMain);
    if (zeroSecondary)
    {
      continue;
    }
    if (i % temp_div == 0)
    {
      secondary_stepper->step(passo_min * negativeSecondary);
    }
  }
}

void serialEvent()
{
  byte bufferSerial[Serial.available()];
  int index = 0;
  int bufferSize = Serial.readBytes(bufferSerial, Serial.available());

  int centerX = passos_para_volta_completa * 10;
  int centerY = passos_para_volta_completa * 10;
  int radius = passos_para_volta_completa * 5;

  //avisa que começou a desenhar
  Serial.write(byte(60));
  
  switch (bufferSerial[0])
  {
    case 1:
      drawHappyFace(centerX, centerY, radius);
      break;
    case 2:
      drawSadFace(centerX, centerY, radius);
      break;
    case 3:
      drawNeutralFace(centerX, centerY, radius);
      break;
    case 4:
      drawAngryFace(centerX, centerY, radius);
      break;
    case 5:
      drawFearfulFace(centerX, centerY, radius);
      break;
    case 6:
      drawSurprisedFace(centerX, centerY, radius);
      break;
    case 7:
      drawSickFace(centerX, centerY, radius);
      break;

  }

  if (index >= bufferSize)
    return;
}

void drawFace(int centerX, int centerY, int radius)
{
  //Face
  drawPolygonByRadius(centerX, centerY, radius, number_of_sides_circle);
  int eyes = (radius / 5) * 2;
  //Right Eye
  drawPolygonByRadius(centerX + eyes, centerY + eyes, radius / 5, number_of_sides_circle);
  drawPoint(centerX + eyes, centerY + eyes);
  //Left Eye
  drawPolygonByRadius(centerX - eyes, centerY + eyes, radius / 5, number_of_sides_circle);
  drawPoint(centerX - eyes, centerY + eyes);
  //Nose
  drawPolygonByRadius(centerX, centerY, radius / 15, 3, 0, 2, true);
}

void drawAngryEyebrows(int centerX, int centerY, int radius)
{
  int eyes = (radius / 5) / 2;
  drawLineByCoordinate(centerX - eyes - radius / 5, centerY + eyes + (radius / 5) * 2, centerX - eyes + radius / 5, centerY + eyes + radius / 5);
  drawLineByCoordinate(centerX + eyes + radius / 5, centerY + eyes + radius / 5, centerX + eyes - radius / 5, centerY + eyes + (radius / 5) * 2);
}

void drawApprehensiveEyebrows(int centerX, int centerY, int radius)
{
  int eyes = (radius / 5) / 2;
  drawLineByCoordinate(centerX + eyes - radius / 5, centerY + eyes + (radius / 5) * 2, centerX + eyes + radius / 5, centerY + eyes + radius / 5);
  drawLineByCoordinate(centerX - eyes + radius / 5, centerY + eyes + radius / 5, centerX - eyes - radius / 5, centerY + eyes + (radius / 5) * 2);
}

void drawHappyMouth(int centerX, int centerY, int radius)
{
  drawPolygonByRadius(centerX, centerY + radius / 5, radius, number_of_sides_circle, (PI * 2) / 5, number_of_sides_circle / 5, false);
}

void drawSadMouth(int centerX, int centerY, int radius)
{
  drawPolygonByRadius(centerX, centerY - (radius / 5) * 7, radius, number_of_sides_circle, PI + (PI * 2) / 5, number_of_sides_circle / 5, false);
}

void drawSickMouth(int centerX, int centerY, int radius)
{
  drawPolygonByRadius(centerX, centerY - (radius / 5) * 7, radius, number_of_sides_circle, PI + (PI * 2) / 16, number_of_sides_circle / 8, false);
  drawPolygonByRadius(centerX + (radius / 5) * 2.5, centerY - (radius / 5) * (12 / 7), (radius / 5) * 2, number_of_sides_circle,   PI / 4, number_of_sides_circle / 2, false);
  drawPolygonByRadius(centerX - (radius / 5) * 2.5, centerY - (radius / 5) * (12 / 7), (radius / 5) * 2, number_of_sides_circle, 3 * PI / 4, number_of_sides_circle / 2, false);
}

void drawSurprisedMouth(int centerX, int centerY, int radius)
{
  //drawPolygonByRadius(centerX, centerY - (radius / 5) * 2, radius / 5, number_of_sides_circle,0,number_of_sides_circle,false);
  drawPolygonByRadius(centerX, centerY - (radius/5)*2,radius/5,number_of_sides_circle);
}

void drawNeutralMouth(int centerX, int centerY, int radius)
{
  drawLineByCoordinate(centerX - radius / 6, centerY - (radius / 5) * 2, centerX + radius / 6, centerY - (radius / 5) * 2);
}

void drawHappyFace(int centerX, int centerY, int radius)
{
  drawFace(centerX, centerY, radius);
  drawHappyMouth(centerX, centerY, radius);
}

void drawSadFace(int centerX, int centerY, int radius)
{
  drawFace(centerX, centerY, radius);
  drawSadMouth(centerX, centerY, radius);
}

void drawNeutralFace(int centerX, int centerY, int radius)
{
  drawFace(centerX, centerY, radius);
  drawNeutralMouth(centerX, centerY, radius);
}

void drawAngryFace(int centerX, int centerY, int radius)
{
  drawSadFace(centerX, centerY, radius);
  drawAngryEyebrows(centerX, centerY, radius);
}

void drawFearfulFace(int centerX, int centerY, int radius)
{
  drawSadFace(centerX, centerY, radius);
  drawApprehensiveEyebrows(centerX, centerY, radius);
}

void drawSurprisedFace(int centerX, int centerY, int radius)
{
  drawFace(centerX, centerY, radius);
  drawSurprisedMouth(centerX, centerY, radius);
}

void drawSickFace(int centerX, int centerY, int radius)
{
  drawFace(centerX, centerY, radius);
  drawApprehensiveEyebrows(centerX, centerY, radius);
  drawSickMouth(centerX, centerY, radius);
}

void drawBasicHair(int centerX, int centerY, int radius)
{
  for (float i = PI / 4; i < PI * 3 / 4; i += PI / 16)
  {
    drawLineByAngle(centerX, centerY + radius, radius / 5, i);
  }
}

void loop()
{
  //drawPolygonByRadius(1000, 1000, passos_para_volta_completa, 4, 0, 2);
  /*for(int i = 3; i < 10; i++)
    {
    drawPolygon(0,0,500,i);
    delay(500);
    }*/
  //drawCircle(0,0,1000);
  //myStepper.step(passos_para_volta_completa);
  //stepVetor(passos_para_volta_completa,0);
  //stepVetor(0,passos_para_volta_completa);
  //stepVetor(-passos_para_volta_completa,0);
  //stepVetor(0,-passos_para_volta_completa);
  
  //avisa que está disponível
  Serial.write(byte(58));
  delay(1000);
}
