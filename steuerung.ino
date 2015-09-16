//BK-Steuerung-v1
//S Titz

//Walze
const int WS = A9;
const int WV = PA_2;
const int WZ = PA_4;
const int WE = PA_3;

//Klappe
const int KS = A8;
const int KV = PD_6;
const int KZ = PF_4;
const int KE = PD_7;

//Schalter
const int SA = PD_2;
const int SZ = PD_3;

//Lichtschranke
const int LVH = PE_3;
const int LVD = PD_0;
const int LHH = PA_6;
const int LHD = PD_1;

//SigTerm
const int END = PF_2;

function klappe_auf() {}
function klappe_zu() {}
function klappe_stop() {}

function walze_vor() {}
function walze_zurueck() {}
function walze_stop() {}

function sensor_walze() {}
function sensor_klappe() {}



void setup()
{
  // anfangs werte
  
}

void loop()
{
  //klappe auf
  //bis endschalter
  //dann klappe stop
  //walze an
  
}
