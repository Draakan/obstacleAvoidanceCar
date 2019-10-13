int inputPin = A0;
int outputPin = A1;
#define Lpwm_pin  5
#define Rpwm_pin  10
int pinLB = 2;
int pinLF = 4 ;
int pinRB = 7;
int pinRF = 8;
unsigned char Lpwm_val = 200; //speed
unsigned char Rpwm_val = 200; //speed
int servopin = 3;
int myangle;
int pulsewidth;
unsigned char DuoJiao = 60;

void servopulse(int servopin, int myangle)
{
  pulsewidth = (myangle * 11) + 500;
  digitalWrite(servopin, HIGH);
  delayMicroseconds(pulsewidth);
  digitalWrite(servopin, LOW);
  delay(20 - pulsewidth / 1000);
}

void Set_servopulse(int set_val)
{
 for(int i = 0; i <= 10; i++)
   servopulse(servopin, set_val);
}

void M_Control_IO_config(void)
{
  pinMode(pinLB, OUTPUT);
  pinMode(pinLF, OUTPUT);
  pinMode(pinRB, OUTPUT);
  pinMode(pinRF, OUTPUT);
  pinMode(Lpwm_pin, OUTPUT); 
  pinMode(Rpwm_pin, OUTPUT);  
}

void Set_Speed(unsigned char Left,unsigned char Right)
{
  analogWrite(Lpwm_pin, Left);
  analogWrite(Rpwm_pin, Right);
}

void advance()
{
  digitalWrite(pinRB, LOW);
  digitalWrite(pinRF, HIGH);
  digitalWrite(pinLB, LOW);
  digitalWrite(pinLF, HIGH); 
}

void turnR()
{
  digitalWrite(pinRB, LOW);
  digitalWrite(pinRF, HIGH);
  digitalWrite(pinLB, HIGH);
  digitalWrite(pinLF, LOW);
}

void turnL()
{
  digitalWrite(pinRB, HIGH);
  digitalWrite(pinRF, LOW );
  digitalWrite(pinLB, LOW);
  digitalWrite(pinLF, HIGH);
}

void stopp()
{
  digitalWrite(pinRB, HIGH);
  digitalWrite(pinRF, HIGH);
  digitalWrite(pinLB, HIGH);
  digitalWrite(pinLF, HIGH);
}

void back()
{
  digitalWrite(pinRB, HIGH);    
  digitalWrite(pinRF, LOW);
  digitalWrite(pinLB, HIGH);
  digitalWrite(pinLF, LOW);
}

void Self_Control(void)
{
  int H;

  Set_servopulse(DuoJiao);

  H = Ultrasonic_Ranging();
  delay(300);   

  if(Ultrasonic_Ranging() < 35)         
  {
    stopp();              
    delay(100);
    back();               
    delay(50);
  }
           
  if(Ultrasonic_Ranging() < 60)        
  {
    stopp();  
    delay(100);            
    Set_servopulse(5);
    int L = ask_pin_L();
    delay(300);
    Set_servopulse(177);
    int R = ask_pin_R();
    delay(300);      

    if(ask_pin_L() > ask_pin_R())   
    {
      back(); 
      delay(100);      
      turnL();
      delay(400);                  
      stopp();  
      delay(50);            
      Set_servopulse(DuoJiao);
      H = Ultrasonic_Ranging();
      delay(500); 
    }

    if(ask_pin_L()  <= ask_pin_R())   
    {
      back();  
      delay(100);  
      turnR(); 
      delay(400);   
      stopp();  
      delay(50);            
      Set_servopulse(DuoJiao);
      H = Ultrasonic_Ranging();
      delay(300);        
    }

    if (ask_pin_L()  < 35 && ask_pin_R() < 35)   
    {
      stopp();            
      delay(50);
      back(); 
      delay(50);                   
    }
  }
  else advance();               
}

int Ultrasonic_Ranging()
{ 
  int old_distance;
  digitalWrite(outputPin, LOW);  
  delayMicroseconds(2); 
  digitalWrite(outputPin, HIGH); 
  delayMicroseconds(10); 
  digitalWrite(outputPin, LOW);    
  int distance = pulseIn(inputPin, HIGH);
  distance = distance / 58;  
  return distance;
} 

int ask_pin_L()    
{ 
  int old_Ldistance;
  digitalWrite(outputPin, LOW);  
  delayMicroseconds(2); 
  digitalWrite(outputPin, HIGH); 
  delayMicroseconds(10); 
  digitalWrite(outputPin, LOW);    
  int Ldistance = pulseIn(inputPin, HIGH); 
  Ldistance = Ldistance / 58;  
  return Ldistance;
}

int ask_pin_R()
{ 
  int old_Rdistance;
  digitalWrite(outputPin, LOW);  
  delayMicroseconds(2); 
  digitalWrite(outputPin, HIGH);
  delayMicroseconds(10); 
  digitalWrite(outputPin, LOW);    
  int Rdistance = pulseIn(inputPin, HIGH); 
  Rdistance = Rdistance / 58;  
  return Rdistance;
} 

void setup() 
{ 
  pinMode(servopin, OUTPUT);
  M_Control_IO_config();
  Set_Speed(Lpwm_val, Rpwm_val);
  Set_servopulse(DuoJiao);
  pinMode(inputPin, INPUT);
  pinMode(outputPin, OUTPUT);
  Serial.begin(9600);
  stopp();
} 

void loop() 
{  
  Self_Control();   
}