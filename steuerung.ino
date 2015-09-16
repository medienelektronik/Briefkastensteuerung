// Briefkastensreuerung Stand: 23.08.2015
// Written by Matthias Teeken

// Definieren der Hardwarepins zu Variablen
//       Variable      = Hardwarepin         
//_______________________________________________
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
const int LS1         = PE_3;  //Lichtschranke vorne
const int LS11        = PD_0;  //Lichtschranke vorne Dunkel
const int LS2         = PA_6;  //Lichtschranke hinten
const int LS21        = PD_1;  //Lichtschranke hinten Dunkel

// Pin für das Abschalten der Spannung
const int Aus_sig    = PF_2;

// LED StatusBelegung
const int LED_green  = PF_1;
//const int LED_blue   = PF_2;
const int LED_red    = PF_3;

//___________________________________________

//Definieren von Variablen und deren Anfangswerte 
int W_Sense_value     = 0;
int W_Sense_value_map = 0;
int W_Sense_max       = 3900; // Einstellen des Wertes zum ändern der Walzenrichtung
int W_Sense_mean      = 0;

int K_Sense_value     = 0;
int K_Sense_value_map = 0;
int K_Sense_max       = 3900; //Einstellen des Wertes zum wieder öffnen der Klappe
int K_Sense_mean      = 0;

unsigned long t_start      = 0;
unsigned long t_aktuell    = 0;
unsigned long t_soll_Walze = 5000; //Zeit des Waltenzennachlaufs in ms
unsigned long t_soll       = 0; 

void setup() 
{
  
// Definieren der Hardwarepins als Ein- oder Ausgang
  pinMode(Walze_Vor,     OUTPUT);
  pinMode(Walze_Sens,    INPUT);
  pinMode(Walze_Back,    OUTPUT);
  pinMode(Walze_Enable,  OUTPUT);
  
  pinMode(Klappe_Vor,     OUTPUT);
  pinMode(Klappe_Sens,    INPUT);
  pinMode(Klappe_Back,    OUTPUT);
  pinMode(Klappe_Enable,  OUTPUT);
    
  pinMode(LS1, INPUT);
  pinMode(LS11, INPUT);
  pinMode(LS2, INPUT);
  pinMode(LS21, INPUT);

// Definieren der Hardwarepins mit Pullup-Widerstand
  pinMode(Schalter_auf,    INPUT_PULLUP);
  pinMode(Schalter_zu,     INPUT_PULLUP);

// Definiieren des Aussignalschalterpins  
  pinMode(Aus_sig, OUTPUT); 
 
// Definieren der LED Status LED's
  pinMode(LED_green, OUTPUT);
  pinMode(LED_red,   OUTPUT);
//  pinMode(LED_blue,  OUTPUT);
    
// Definieren der Anfangszustände der verwendeten HArdwarepins, alle auf LOW 
  digitalWrite(Walze_Vor,    LOW);
  digitalWrite(Walze_Back,   LOW);
  digitalWrite(Walze_Enable, LOW);
  digitalWrite(Klappe_Vor,    LOW);
  digitalWrite(Klappe_Back,   LOW);
  digitalWrite(Klappe_Enable, LOW);
  digitalWrite(Aus_sig,       LOW);
  digitalWrite(LED_green,       LOW);
  digitalWrite(LED_red,       LOW);
//  digitalWrite(Aus_sig,       LOW);
  

}

void loop() 
{  
   
//Klappe öffnen
//________________________________________________
   
   open_again: //Sprungmarke, zum wieder öffnen.
   digitalWrite(Aus_sig, LOW); // Abschaltsignal ausschalten
   // Signal geben um aufzufahren.
   digitalWrite(Klappe_Back, LOW);
   digitalWrite(Klappe_Vor, HIGH);

//Beim Auffahren der Klappe leuchtet StatusLED GRÜN
   digitalWrite(LED_green, HIGH);
   digitalWrite(LED_red, LOW);
   
     while(digitalRead(Schalter_auf) == LOW){     
       digitalWrite(Klappe_Vor, HIGH);
       digitalWrite(Klappe_Enable, HIGH);            
 }
     // Abschalten der Signale für die Klappe
     digitalWrite(Klappe_Vor, LOW);
     digitalWrite(Klappe_Enable, LOW);

   digitalWrite(LED_green, LOW);
   digitalWrite(LED_red, LOW);
   
//________________________________________________


//Walze_einziehen
//________________________________________________
t_start   = millis();
t_aktuell = millis();
 while(t_aktuell < (t_start+t_soll_Walze) || digitalRead(LS1) == LOW || digitalRead(LS2) == LOW || digitalRead(LS11) == LOW || digitalRead(LS21) == LOW )
 {  
 //STaus LED leuchtet ROT wenn Walze einfährt
   digitalWrite(LED_green, LOW);
   digitalWrite(LED_red, HIGH);
 
   
  digitalWrite(Walze_Vor, HIGH);
  digitalWrite(Walze_Enable, HIGH);
  t_aktuell = millis();
       
// Mittelwert von 10 Messwerten aufnehmen in 100ms prüfen, ob Packet zu dick ist.
      for (int x = 0; x <10 ;x++) {
      W_Sense_value = analogRead(Walze_Sens);
      W_Sense_mean = W_Sense_mean + W_Sense_value;
      delay(10);
      }
      W_Sense_mean = W_Sense_mean /10;
  

//   Wenn packet zu dick, dass Motor blockiert zurückfahren für 2sec!!!
  if (W_Sense_mean >= W_Sense_max)
  {
    digitalWrite(Walze_Enable, HIGH);
    digitalWrite(Walze_Vor, LOW);
    digitalWrite(Walze_Back, HIGH);
    delay(2000); //Zeit des Zurückfahrens definieren
    digitalWrite(Walze_Back, LOW);
  } 
 }
  // Stoppen der Walze
  digitalWrite(Walze_Vor, LOW);
  digitalWrite(Walze_Enable, LOW);
  
   digitalWrite(LED_green, LOW);
   digitalWrite(LED_red, LOW);
  
//________________________________________________

//Klappe schließen
//________________________________________________
  delay(1000);
  // Signal für Klappe Schießen
  
   digitalWrite(LED_green, HIGH);
   digitalWrite(LED_red, HIGH);
  
  digitalWrite(Klappe_Enable, HIGH);
  digitalWrite(Klappe_Back, HIGH);
  delay (1000);
   
   // Solange der Schalter_zu kein Signal gibt, zufahren
   while(digitalRead(Schalter_zu) == HIGH){     
     digitalWrite(Klappe_Back, HIGH);
     digitalWrite(Klappe_Enable, HIGH);
      
      // Mittelwert von 10 Messwerten aufnehmen
      for (int x = 0; x <10 ;x++) {
      K_Sense_value = analogRead(Klappe_Sens);
      K_Sense_mean = K_Sense_mean + K_Sense_value;
      delay(10);
      }
      K_Sense_mean = K_Sense_mean /10;       
      
     // Vergleich ob Mittelwert des Signals über dem Max Wert liegt.
     // Wenn das der Fall ist oder Lichtschranke LS1 was sieht, wieder auffahren.
     if (digitalRead(LS1) == LOW || K_Sense_mean >= K_Sense_max){  // || digitalRead(LS11) == LOW
       digitalWrite(Klappe_Back,LOW);
       //STausLED wieder löschen
       digitalWrite(LED_green, HIGH);
       digitalWrite(LED_red, HIGH);
       
       goto open_again;
       }       
   }
   digitalWrite(Klappe_Back, LOW);
   digitalWrite(Klappe_Enable, LOW);   
   digitalWrite(Aus_sig, HIGH);
   delay(2000);   
//________________________________________________ 
}
