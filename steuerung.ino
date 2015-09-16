// These constants won't change.  They're used to give names
// to the pins used:

// Walze
const int Walze_Sens   = A9;  // Messen der Spannung
const int Walze_Vor    = PA_2;  // Motor_Vor
const int Walze_Back   = PA_4;  // Motor_Back
const int Walze_Enable = PA_3;  // Enable Signal

//Klappe
const int Klappe_Sens   = A8;  // Messen der Spannung
const int Klappe_Vor    = PD_6;  // Motor_Vor
const int Klappe_Back   = PF_4;  // Motor_Back
const int Klappe_Enable = PD_7;  // Enable Signal

//Endschalter
const int Schalter_auf   = PD_2;
const int Schalter_zu    = PD_3;

//Lichtschranke
const int LS1         = PE_3; //Lichtschranke vorne 
const int LS2         = PA_6; //Lichtschranke hinten

const int Aus_sig    = PF_2;

int Walze            = 1;
int Klappe           = 2;
int vor              = 0;
int back             = 1;
int start            = 1;
int stopp            = 0;
int counter          = 0;

int W_Sense_value     = 0;
int W_Sense_value_map = 0;
int W_Sense_max       = 3900;

int K_Sense_value     = 0;
int K_Sense_value_map = 0;
int K_Sense_max       = 3900;

unsigned long t_start   = 0;
unsigned long t_aktuell = 0;
unsigned long t_soll    = 0; //Zeit des Waltenzennachlaufs in ms

void setup() 
{
  pinMode(Walze_Vor,     OUTPUT);
  pinMode(Walze_Sens,    INPUT);
  pinMode(Walze_Back,    OUTPUT);
  pinMode(Walze_Enable,  OUTPUT);
  
  pinMode(Klappe_Vor,     OUTPUT);
  pinMode(Klappe_Sens,    INPUT);
  pinMode(Klappe_Back,    OUTPUT);
  pinMode(Klappe_Enable,  OUTPUT);
    
  pinMode(Schalter_auf,    INPUT_PULLUP);
  pinMode(Schalter_zu,     INPUT_PULLUP);
    
  pinMode(LS1, INPUT);
  pinMode(LS2, INPUT);
  
  digitalWrite(Walze_Vor,    LOW);
  digitalWrite(Walze_Back,   LOW);
  digitalWrite(Walze_Enable, LOW);
  digitalWrite(Klappe_Vor,    LOW);
  digitalWrite(Klappe_Back,   LOW);
  digitalWrite(Klappe_Enable, LOW);
  
// initialize serial communications at 9600 bps:
  Serial.begin(9600);
}

void loop() 
{  
   
//Klappe öffnen
//________________________________________________
   
   open_again:
   digitalWrite(Aus_sig, LOW);
   digitalWrite(Klappe_Back, LOW);
   digitalWrite(Klappe_Vor, HIGH);
   
   while(digitalRead(Schalter_auf) == HIGH){     
     digitalWrite(Klappe_Vor, HIGH);
     digitalWrite(Klappe_Enable, HIGH);    
   }
   digitalWrite(Klappe_Vor, LOW);
   digitalWrite(Klappe_Enable, LOW);   
//________________________________________________


//Walze_einziehen
//________________________________________________
t_start   = millis();
t_aktuell = millis();
t_soll    = 10000;
 while(t_aktuell < (t_start+t_soll) || digitalRead(LS1) == LOW || digitalRead(LS2) == LOW ) 
 {  
  digitalWrite(Walze_Vor, HIGH);
  digitalWrite(Walze_Enable, HIGH);
  t_aktuell = millis();
  
  
  
  W_Sense_value = analogRead(Walze_Sens);
  W_Sense_value_map = map(W_Sense_value,0,4000,0,1000);
  
  Serial.print("sensor Walze= " );                       
  Serial.print(W_Sense_value);      
  Serial.print("\t output = ");      
  Serial.println(W_Sense_value_map);
  delay(10);

 //   Wenn packet zu dick, dass Motor blockiert zurückfahren !!!
  if (W_Sense_value >= W_Sense_max)
  {
    digitalWrite(Walze_Enable, HIGH);
    digitalWrite(Walze_Vor, LOW);
    digitalWrite(Walze_Back, HIGH);
    delay(2000);
    digitalWrite(Walze_Back, LOW);
  } 
 }
  digitalWrite(Walze_Vor, LOW);
  digitalWrite(Walze_Enable, LOW);
//________________________________________________

//Klappe schießen
//________________________________________________
delay(1000);
  digitalWrite(Klappe_Enable, HIGH);
  digitalWrite(Klappe_Back, HIGH);
   delay (1000);
   
   while(digitalRead(Schalter_zu) == HIGH){     
     digitalWrite(Klappe_Back, HIGH);
     digitalWrite(Klappe_Enable, HIGH);
              
      K_Sense_value = analogRead(Klappe_Sens);
      K_Sense_value_map = map(K_Sense_value,0,4000,0,1000);
      Serial.print("sensor Klappe= " );                       
      Serial.print(W_Sense_value);      
      Serial.print("\t output = ");      
      Serial.println(W_Sense_value_map);
      delay(10);
     
     if (digitalRead(LS1) == LOW || K_Sense_value >= K_Sense_max){
       digitalWrite(Klappe_Back,LOW);       
       goto open_again;
       }       
   }
   digitalWrite(Klappe_Back, LOW);
   digitalWrite(Klappe_Enable, LOW);
   
   digitalWrite(Aus_sig, HIGH);
   delay(2000);
   
//________________________________________________ 
}
