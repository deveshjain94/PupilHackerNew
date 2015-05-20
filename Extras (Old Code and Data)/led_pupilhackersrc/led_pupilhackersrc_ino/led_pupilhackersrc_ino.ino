int whl=6;
int whr=5;
int wvl=9;
int wvr=10;
int ir=11;
int il=3;
int ic=13;
int ig=2;


void setup()
{
  pinMode(whl,OUTPUT);
  pinMode(wvl,OUTPUT);
  pinMode(ir,OUTPUT);
  pinMode(il,OUTPUT);
  pinMode(ic,OUTPUT);
  pinMode(ig,OUTPUT);
  pinMode(whr,OUTPUT);
  pinMode(wvr,OUTPUT);
  pinMode(7,OUTPUT);
  pinMode(4,OUTPUT);
  Serial.begin(9600);
  digitalWrite(ig,LOW);
  analogWrite(il,250);
  analogWrite(ir,250);
  analogWrite(ic,250);
  digitalWrite(7,HIGH);
  digitalWrite(4,HIGH);
  Serial.print("0");
}


void loop()
{
  if(Serial.available())
  {
    if(Serial.read()=='s')
    {
      delay(10000);
  analogWrite(whl,20);  //62 lux
  analogWrite(wvl,20);
  Serial.print("1");
  delay(1000);
  analogWrite(whl,0); //error scope of 20 -30 lux
  analogWrite(wvl,0);
  Serial.print("0");
  delay(3000);
  analogWrite(whr,20); //62 lux
  analogWrite(wvr,20);
  Serial.print("2");
  delay(1000);
  analogWrite(whr,0);
  analogWrite(wvr,0);
  Serial.print("0");
  delay(3000);
  analogWrite(whl,50); //50 lux
  analogWrite(wvl,50);
  Serial.print("3");
  delay(1000);
  analogWrite(whl,0);
  analogWrite(wvl,0);
  Serial.print("0");
  delay(3000);
  analogWrite(whr,50); //123 lux
  analogWrite(wvr,50);
  Serial.print("4");
  delay(1000);
  analogWrite(whr,0);
  analogWrite(wvr,0);
  Serial.print("0");
  delay(3000);
  analogWrite(whl,100); //236 lux
  analogWrite(wvl,100);
  Serial.print("5");
  delay(1000);
  analogWrite(whl,0);
  analogWrite(wvl,0);
  Serial.print("0");
  delay(3000);
  analogWrite(whr,100); //350 lux
  analogWrite(wvr,100);
  Serial.print("6");
  delay(1000);
  analogWrite(whr,0);
  analogWrite(wvr,0);
  Serial.print("0");
  delay(3000);
  analogWrite(whl,150);
  analogWrite(wvl,150);
  Serial.print("7");
  delay(1000);
  analogWrite(whl,0);
  analogWrite(wvl,0);
  Serial.print("0");
  delay(3000);
  analogWrite(whr,150);  
  analogWrite(wvr,150);
  Serial.print("8");
  delay(1000);
  analogWrite(whr,0);
  analogWrite(wvr,0);
  Serial.print("0");
  delay(3000);
    }
  }
}

