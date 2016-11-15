//BK-Steuerung-v4
//S Titz 2016-03-08

//Walze
const int WS = A5;	//Walze Sense
const int WZ = PA_2;	//Walze Zurück
const int WV = PA_4;    //Walze Vor
const int WE = PA_3;    //Walze Enable
//Klappe
const int KS = A4;
const int KV = PF_4;
const int KZ = PD_6;
const int KE = PD_7;
//Lichtschranke
const int LVH = PA_5;
const int LVD = PD_0; //
const int LHH = PE_3;
const int LHD = PD_1;
//SigTerm
const int END = PF_0;
//Schalter
const int SA = PE_4;
const int SZ = PE_5;

//report code
const int AL0 = PF_1;
const int AL1 = PF_2;
const int AL2 = PF_3;

//maxima
const long KM =  3052;    //Stromaufnahme Klappe
const long WM =  2000;    //Stromaufnahme Walze

const int WC =  4;        //Walze wiederholungen
const int KC =  4;        //Klappe wiederholungen

//timer
const long WLV = 3000; //leerlauf walze vor
const long WLZ = 5000; //leerlauf walze zurück
const long LW = 7000 ; //warten bevor klappe zu
//------------------------------------------------------------------
//rest
int klappschwer =0;
long LLVH = 1;
long LLVD = 1;
long LLHH = 1;
long LLHD = 1;

void setup() {
	Serial.begin(9600);
	//IO?
	pinMode(WV, OUTPUT);
	pinMode(WZ, OUTPUT);
	pinMode(WE, OUTPUT);
	pinMode(KV, OUTPUT);
	pinMode(KZ, OUTPUT);
	pinMode(KE, OUTPUT);
	pinMode(END,OUTPUT); 
	pinMode(AL0,OUTPUT);
	pinMode(AL1,OUTPUT);
	pinMode(AL2,OUTPUT); 
	
	pinMode(WS, INPUT);
	pinMode(KS, INPUT);
	pinMode(LVH,INPUT);
	pinMode(LVD,INPUT);
	pinMode(LHH,INPUT);
	pinMode(LHD,INPUT);
	
	pinMode(SA, INPUT_PULLUP);
	pinMode(SZ, INPUT_PULLUP);
}

void wait(long secs) {
  for(long i=0;i<secs;i++) {
    delay(1000);
  }
  Serial.println(secs);
}

void alarm(int error) {
        // 0 000 aus	ALLES OK
        // 1 001 rot	walze zu schwer
        // 2 010 blau	hängt im Ende schaltet nicht ab
        // 3 011 lila	
        // 4 100 gruen	
        // 5 101 orange	
        // 6 110 gelb	
        // 7 111 weiss	

	int alarm0 = error/4;
	error = error-(alarm0*4);
	int alarm1 = error/2;
	int alarm2 = error-(alarm1*2);
	digitalWrite(AL0, alarm0);
	digitalWrite(AL1, alarm1);
	digitalWrite(AL2, alarm2);
}

void klappe_auf() {
	Serial.println("klappe auf");
	digitalWrite(KE, HIGH);
	digitalWrite(KZ, LOW);
	digitalWrite(KV, HIGH);
}

void klappe_zu() {
	Serial.println("klappe zu");
	digitalWrite(KE, HIGH);
	digitalWrite(KZ, HIGH);
	digitalWrite(KV, LOW);
}

void klappe_stop() {
	Serial.println("klappe stop");
	digitalWrite(KE, HIGH);
	digitalWrite(KZ, LOW);
	digitalWrite(KV, LOW);
}

void walze_vor() {
	Serial.println("walze_vor");
	digitalWrite(WE, HIGH);
	digitalWrite(WZ, LOW);
	digitalWrite(WV, HIGH);
}

void walze_zurueck() {
	Serial.println("walze_zurueck");
	digitalWrite(WE, HIGH);
	digitalWrite(WZ, HIGH);
	digitalWrite(WV, LOW);
}

void walze_stop() {
	Serial.println("walze_stop");
	digitalWrite(WE, LOW);
	digitalWrite(WZ, LOW);
	digitalWrite(WV, LOW);
}

int sensor_walze() {
	int ret = 0; 
	for (int i = 0; i <10 ;i++) {
		ret = ret + analogRead(WS);
		delay(10);
    }    
    Serial.print("Sensor Walze: ");
    Serial.print(ret/10);
    Serial.print(" ");
    Serial.println((ret/10) > WM);
    return (ret/10) > WM;
}

int sensor_klappe() {
	int ret = 0; 
	for (int i = 0; i <10 ;i++) {
		ret = ret + analogRead(KS);
		delay(10);
    }
    Serial.print("Sensor Klappe: ");
    Serial.print(ret/10);
    Serial.println((ret/10) > WM);
	return (ret/10) > KM;
}

int schalter_auf() { //Schalter SA =H  SZ=L ändern mit ! Zeichen	
        pinMode(SA, INPUT_PULLUP);
        if(!digitalRead(SA)) {
          Serial.println("Schalter auf nein");
          return 0;
        }else{
          Serial.println("Schalter auf ja");
          return 1;
        }
}

int schalter_zu() {
	pinMode(SZ, INPUT_PULLUP);
        if(!digitalRead(SZ)) {
          Serial.println("Schalter zu nein");
          return 0;//Schalter ZU=H  ZU=L ändern mit ! Zeichen
        }else{
          Serial.println("Schalter zu ja");
          return 1;
        }
}

void kill_all(int al=2) {
	Serial.println("Kill em All!!!");
	digitalWrite(END, HIGH);
        alarm(al);
        while(true) {
          delay(250);
        }
}

int lichtschrank_vorn_hell() {
    int t = digitalRead(LVH);
    if (t == LLVH)
        return false;
    LLVH = t;
    return true;
}

int lichtschrank_vorn_dunkel() {
    int t = digitalRead(LVD);
    if (t == LLVD)
        return false;
    LLVD = t;
    return true;
}

int lichtschrank_hinten_hell() {
    int t = digitalRead(LHH);
    if (t == LLHH)
        return false;
    LLHH = t;
    return true;
}

int lichtschrank_hinten_dunkel() {
    int t = digitalRead(LHD);
    if (t == LLHD)
        return false;
    LLHD = t;
    return true;
}

int lichtschranken() {
    Serial.print("Lichtschranken Test: ");
    int lsvh = lichtschrank_vorn_hell();
    int lsvd = lichtschrank_vorn_dunkel();
    int lshh = lichtschrank_hinten_hell();
    int lshd = lichtschrank_hinten_dunkel();
    Serial.print(lsvh);
    Serial.print(" ");
    Serial.print(lsvd);
    Serial.print(" ");
    Serial.print(lshh);
    Serial.print(" ");
    Serial.println(lshd);
    if(lsvh == HIGH || lsvd == HIGH || lshh == HIGH || lshd == HIGH) {
        return HIGH;
    }else {
        return LOW;
    }
}

void loop() {
        long start = 0; //speichert den millisekunden wert für diverse timer
	int schwer = 0; // zählt wie oft in dem durchgang ein paket zu schwer einzuziehen war
	int durchgang = HIGH; // speichert on noch ein einzugsdurchgang geplant ist
        digitalWrite(END,0);
        
        Serial.println("Init!!!(4.0 stable)");
      // klappe öffnen
        if(klappschwer < KC) {
	klappe_auf();
        while(!schalter_auf()) {
          delay(50);
        }
        klappe_stop();
      // ----
      // Walze vor
        walze_vor();
        delay(250);
	while(durchgang == HIGH) {
                //hier wird geklärt ob es noch einen durchgang gibt
		durchgang = LOW; //durchgang löschen
		start = millis(); // starttimer initiieren
                Serial.print("Start Var = ");
                Serial.println(start);
                Serial.print("lauf bis");
                Serial.println(start+WLV);
		while((start+WLV)>millis() && schwer < WC) {
			// solang der timer noch nicht abgelaufen ist und der zu schwer counter nicht bis maximum gelaufen ist
			if(lichtschranken() == HIGH) {
				//wenn die Lichtschranken was neues sehen, wird der Timer zurück gesetzt
                                delay(250);
				start = millis();
			}//*/
			
			if(sensor_walze()) {
                                delay(250);
                                if(sensor_walze()) {
                                    Serial.println("Zu schwer");
                			//wenn einzug zu schwer wird ein zu schwer gezählt
		    		    schwer++;
				    walze_zurueck();
				    while((start+WLZ)>millis()) {
					//lässt den rückwärtstimer ablaufen und versucht dann wieder vor zu fahren
					// dann greift der schwercounter oben 
				    }  
				    walze_vor();
                                }
			}//*/
		} 
		//walz stoppen, solang nicht zu sehen ist
		walze_stop();

                if(schwer >= WC) {
                     kill_all(1);    
                }else{
          		start = millis(); // init nachlauf Timer
        		while((start+LW)>millis() && durchgang == LOW) {
        			if(lichtschranken() == HIGH) {// todo lichtschranke
        				// wenn lichtschranken was sehen noch einen neuen durchlauf
        				durchgang = HIGH;
                                        walze_vor();
        			}//*/
        		}
                }
	}
        
        klappe_zu();
        delay(250);
        while(!schalter_zu() && durchgang == LOW) {
          if(sensor_klappe() && !schalter_zu()) {
            durchgang = HIGH;
            klappschwer++;
          }
          delay(100);
          
          //if(lichtschranken()) {
          //  durchgang = HIGH;
          //}
        }
        klappe_stop();
        }else{
          kill_all(2);
        }

        if(durchgang == LOW) {
          kill_all(0);
        }
}
