//BK-Steuerung-v1
//S Titz

//Walze
const int WS = A5;	//Walze Sense
const int WV = PA_2;	//Walze Vor
const int WZ = PA_4;
const int WE = PA_3;
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
const int END = PF_2;
//Schalter
const int SA = PE_4;
const int SZ = PE_5;

//maxima
const long KM =  14000;
const long WM =  2000;
const int WC =  5;

//timer
const long WLV = 5000; //leerlauf walze vor
const long WLZ = 5000; //leerlauf walze zurück
const long LW = 3000 ; //warten bevor klappe zu

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
	
	pinMode(WS, INPUT);
	pinMode(KS, INPUT);
	pinMode(LVH,INPUT_PULLUP);
	pinMode(LVD,INPUT_PULLUP);
	pinMode(LHH,INPUT_PULLUP);
	pinMode(LHD,INPUT_PULLUP);
	
	pinMode(SA, INPUT_PULLUP);
	pinMode(SZ, INPUT_PULLUP);
}

void wait(long secs) {
  for(long i=0;i<secs;i++) {
    delay(1000);
  }
  Serial.println(secs);
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

int schalter_auf() {
	pinMode(SA, INPUT_PULLUP);
        if(digitalRead(SA)) {
          Serial.println("Schalter auf nein");
          return 0;
        }else{
          Serial.println("Schalter auf ja");
          return 1;
        }
}

int schalter_zu() {
	pinMode(SZ, INPUT_PULLUP);
        if(digitalRead(SZ)) {
          Serial.println("Schalter zu nein");
          return 0;
        }else{
          Serial.println("Schalter zu ja");
          return 1;
        }
}

void kill_all() {
	Serial.println("Kill em All!!!");
	digitalWrite(END, HIGH);
        while(true) {
          delay(250);
        }
}

int lichtschrank_vorn_hell() {
		return digitalRead(LVH);
}

int lichtschrank_vorn_dunkel() {
		return digitalRead(LVD);
}

int lichtschrank_hinten_hell() {
		return digitalRead(LHH);
}

int lichtschrank_hinten_dunkel() {
		return digitalRead(LHD);
}

int lichtschranken() {
	if(lichtschrank_vorn_hell() == HIGH|| lichtschrank_vorn_dunkel() == HIGH || lichtschrank_hinten_hell() == HIGH || lichtschrank_hinten_dunkel() == HIGH) {
                Serial.println("Lichtschranke getriggert");
		return HIGH;
	}else
		return LOW;
}

void loop() {
  Serial.print(lichtschrank_vorn_hell());
  Serial.print(" ");
  Serial.print(lichtschrank_vorn_dunkel());
  Serial.print(" ");
  Serial.print(lichtschrank_hinten_hell());
  Serial.print(" ");
  Serial.println(lichtschrank_hinten_dunkel());
}
