//BK-Steuerung-v1
//S Titz

//Walze
const int WS = A9;
const int WV = PA_2;
const int WZ = PA_4;
const int WE = PA_3;
//Lichtschranke
const int LVH = PE_3;
const int LVD = PD_0;
const int LHH = PA_6;
const int LHD = PD_1;
//Klappe
const int KS = A8;
const int KV = PD_6;
const int KZ = PF_4;
const int KE = PD_7;
//SigTerm
const int END = PF_2;
//Schalter
const int SA = PD_2;
const int SZ = PD_3;

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
	pinMode(LVH,INPUT);
	pinMode(LVD,INPUT);
	pinMode(LHH,INPUT);
	pinMode(LHD,INPUT);
	
	pinMode(SA, INPUT_PULLUP);
	pinMode(SZ, INPUT_PULLUP);
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
	digitalWrite(KE, LOW);
	digitalWrite(KZ, LOW);
	digitalWrite(KV, LOW);
}

int sensor_walze() {
	int ret = 0; 
	for (int i = 0; i <10 ;i++) {
		ret = ret + analogRead(WS);
		delay(10);
    }
	return (ret/10);
}

int sensor_klappe() {
	int ret = 0; 
	for (int i = 0; i <10 ;i++) {
		ret = ret + analogRead(KS);
		delay(10);
    }
	return (ret/10);
}

int schalter_auf() {
	return digitalRead(SA);
}

int schalter_zu() {
	return digitalRead(SZ);
}

void kill_all() {
	Serial.println("Kill em All!!!")
	digitalWrite(END, HIGH);
}

void loop() {
	klappe_auf();
	
	//bis endschalter
	//dann klappe stop
	//walze an
}
