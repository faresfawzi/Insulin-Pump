float Sens=100; //mV/mT, this is 12.5 for A4 version
float Stc=0.0012; //%/Celcius
float Ta=25;
float Vq=5/2; //half of sensor input voltage
//float B1;
float B2;
float Distime[] = {};
float Vout1;
float Vout2;
int HallPin1=A0;
//int HallPin2=A2; 
float Bact;

float frequency=9;//Hz
const float dutyCycle=0.4;


float offTime=1000.0/frequency*(1-dutyCycle);
float onTime=1000.0/frequency*dutyCycle;

//float offTime=33;
//float onTime=12.5;

float power;
float time_on;
float time_off;
float total_time;
int  Writestatus = 200;
#define HallPin A4



void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200); 
 // pinMode(HallPin,INPUT);
  //pinMode (en, OUTPUT);
  pinMode(7,OUTPUT);
  pinMode(8,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(12,OUTPUT);
  pinMode(2,OUTPUT);
  digitalWrite(7,LOW);
  digitalWrite(8,LOW);
  digitalWrite(12,LOW);
  digitalWrite(2,LOW);  
}

void loop() {
  // put your main code here, to run repeatedly:

       Pump();

}


void Pump()
{


  digitalWrite(7,LOW);
  digitalWrite(8,HIGH);
//  Serial.println("CLOSED");
  Vout1=(analogRead(HallPin1)*5.0)/1023.0;
//  B1=(Vout1-Vq)/(Sens*(1+Stc*(Ta-25)));

    
  digitalWrite(2,LOW); 
  digitalWrite(12,HIGH);
 // Vout2=(analogRead(HallPin2)*5.0)/1023.0;
//  B2=(Vout2-Vq)/(Sens*(1+Stc*(Ta-25)));
  Serial.println("HERE");
//  Serial.print(',');
//  Serial.println(Vout2);
  

  delay(onTime);
  Serial.println("pumpoff");
  digitalWrite(12,LOW); 
  digitalWrite(2,LOW);
  Vout1=(analogRead(HallPin1)*5.0)/1023.0;
//  B1=(Vout1-Vq)/(Sens*(1+Stc*(Ta-25)));


  digitalWrite(7,HIGH);
  digitalWrite(8,LOW);
//  Serial.println("NEUTRAL");
 // Vout2=(analogRead(HallPin2)*5.0)/1023.0;
//  B2=(Vout2-Vq)/(Sens*(1+Stc*(Ta-25)));
  Serial.println(Vout1);
//  Serial.print(',');
//  Serial.println(Vout2);
  

  delay(offTime/10);

  
  digitalWrite(12,LOW); 
  digitalWrite(2,LOW);
  Vout1=(analogRead(HallPin1)*5.0)/1023.0;
  Bact=(Vout1-Vq)/(Sens*(1+Stc*(Ta-25)));


  digitalWrite(7,LOW);
  digitalWrite(8,LOW);
//  Serial.println("NEUTRAL");
//  Vout2=(analogRead(HallPin2)*5.0)/1023.0;
//  B2=(Vout2-Vq)/(Sens*(1+Stc*(Ta-25)));
  Serial.println(Vout1);
//  Serial.print(',');
//  Serial.println(Vout2);
  
  

  //delay((9*offTime)/10);
  delay(20000);
}

/*void HallSensor()
{
  
  float vsum = 0;
  float bsum = 0;
  float a[]={};
  float asum;
  //Serial.print("Inside of function\n");
  for (int i = 0; i <= 50; i++)
  {
    a[i]=analogRead(HallPin);
    asum=asum+a[i];
    Vout[i]=(a[i]*5.0)/1023.0;
    vsum = vsum + Vout[i];
    B[i]=(Vout[i]-Vq)*1000/(Sens*(1+Stc*(Ta-25)));
    bsum = bsum + B[i];
 }
 //Serial.println(asum/50);
 Serial.println(vsum/50);
 Serial.println(bsum/50); 
     
}*/
