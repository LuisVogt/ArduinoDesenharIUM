#include <Stepper.h> 
#include <Servo.h>

const int stepsPerRevolution = 500; 
const int passo_min = 1;
const int passos_para_volta_completa = 2000;
const int velocidade = 2000;
//com essa configuração aproximadamente 2000 passos são necessários para uma volta completa

//Inicializa a biblioteca utilizando as portas de 8 a 11 para 
//ligacao ao motor 
Stepper myStepper(stepsPerRevolution, A1,A3,A2,A4); 
Stepper myStepper2(stepsPerRevolution, 9,11,10,12); 
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
  s.write(90);
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

void drawPoligon(int centerX, int centerY, int radius, int n_sides)
{
  float angle = ((n_sides-2)*PI)/n_sides;
  int start_angle = angle;//angle/2;
  int init_x = centerX + cos(start_angle)*radius;
  int init_y = centerY + sin(start_angle)*radius;
  
  int next_x = 0;
  int next_y = 0;

  Serial.print("Angle:");
  Serial.println(angle/(2*PI));
  
  raiseServo();
  stepVetor(init_x - posicao_atual_x, init_y - posicao_atual_y);
  lowerServo();

  for (int i = 1; i < n_sides ; i++)
  {
    next_x = centerX + cos(start_angle+angle*i)*radius;
    next_y = centerY + sin(start_angle+angle*i)*radius;

    stepVetor(next_x - posicao_atual_x, next_y - posicao_atual_y);
  }

  stepVetor(init_x - posicao_atual_x, init_y - posicao_atual_y);
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
  for(int i = 0; i < 180; i++)
  {
    s.write(i);
    delay(50);
  }
}

void stepVetor(int x, int y)
{
  Serial.print(x);
  Serial.print(",");
  Serial.println(y);
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
  if(x<0){
    negativeX = -1;
  }
  if(y<0){
    negativeY = -1;
  }
  
  if(x == 0 && y == 0)
  {
    return;
  }
  else if(x == 0 || y == 0)
  {
    zeroSecondary = true;
  }
  
  if(abs(x)>abs(y))
  {
    temp_max = abs(x);
    temp_div = abs(x)/abs(y);

    main_stepper = &myStepper;
    secondary_stepper = &myStepper2;
    
    negativeMain = negativeX;
    negativeSecondary = negativeY;
  }
  else
  {
    temp_max = abs(y);
    temp_div = abs(y)/abs(x);
    
    secondary_stepper = &myStepper;
    main_stepper = &myStepper2;
    
    negativeSecondary = negativeX;
    negativeMain = negativeY;

  }

  
  
  for(int i = 0; i<temp_max; i++)
  {
    main_stepper->step(passo_min*negativeMain);
    if(zeroSecondary)
    {
      continue;
    }
    if(i%temp_div==0)
    {
      secondary_stepper->step(passo_min*negativeSecondary);
    }
  }
}

bool teste = false;

void loop() 
{ 
 Serial.print("Teste");

  
  drawPoligon(1000,1000,500,4);
  /*for(int i = 3; i < 10; i++)
  {
    drawPoligon(0,0,500,i);
    delay(500);
  }*/
 //drawCircle(0,0,1000);  
 //myStepper.step(passos_para_volta_completa);
 //stepVetor(passos_para_volta_completa,0);
 //stepVetor(0,passos_para_volta_completa);
 //stepVetor(-passos_para_volta_completa,0);
 //stepVetor(0,-passos_para_volta_completa);
 delay(10000);
}
