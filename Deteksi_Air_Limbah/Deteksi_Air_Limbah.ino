#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
#define analogInPin A0
#define sensorPin A1
LiquidCrystal_I2C lcd(0x26,20,4);

//Program pH
unsigned long int avgValue; 
float b;
int buf[10],temp=0;

//pin LED (Hijau , Merah)
const int ledPinHijau = 5;
const int ledPinMerah = 6;

// pengenalan serial komunikasi

void setup() {
  Serial.begin(9600);// initialize the lcd 
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.noCursor(); 
  
  lcd.setCursor(0,2);
  lcd.print(" ALAT DETEKSI LIMBAH ");
  lcd.setCursor(0,3);
  lcd.print(" MENGGUNAKAN ANDROID ");
  delay(1000);
  lcd.clear();
  //INDIKATOR LED
  pinMode(ledPinHijau, OUTPUT);
  pinMode(ledPinMerah, OUTPUT);
  // tampilan kondisi sungai
  lcd.setCursor(1,0);
  lcd.print("Deteksi Limbah Cair");
  lcd.setCursor(0, 2);
  lcd.print("pHValue = ");
  lcd.setCursor(0, 3);
  lcd.print("TBD = ");
}

void loop() {
  String minta = "";

  while(Serial.available()>0) {
    minta += char (Serial.read()); 
  }
  minta.trim();

  if (minta == "Ya") {
    kirimdata();
  }
  minta = "";
  delay(1000);
}
 
void kirimdata() {
//program Turbidity
  int sensorValue = analogRead(sensorPin);
  int turbidity = map(sensorValue, 0,640, 100, 0);

  if (turbidity < 10) {
    digitalWrite(7, HIGH);
    digitalWrite(8, LOW);
    digitalWrite(9, LOW);
    lcd.setCursor(8, 3);
    lcd.print(" % ");
  }
  if ((turbidity > 10) && (turbidity < 50)) {
    digitalWrite(7, LOW);
    digitalWrite(8, HIGH);
    digitalWrite(9, LOW);
    lcd.setCursor(8, 3);
    lcd.print(" % ");
  }
  if (turbidity > 50) {
    digitalWrite(7, LOW);
    digitalWrite(8, LOW);
    digitalWrite(9, HIGH);
    lcd.setCursor(8, 3);
    lcd.print(" % ");
  }
///////////////////////////////////////////////////////////////////////////
//program pH
 for(int i=0;i<10;i++) { 
  buf[i]=analogRead(analogInPin);
  delay(10);
 }
 for(int i=0;i<9;i++) {
  for(int j=i+1;j<10;j++){
   if(buf[i]>buf[j]){
    temp=buf[i];
    buf[i]=buf[j];
    buf[j]=temp;
   }
  }
 }
 avgValue=0;
 for(int i=2;i<8;i++)
 avgValue+=buf[i];
 
 float pHVol=(float)avgValue*5.0/1024/4.2;
 float phValue = -5.70 * pHVol +28.5 ;
 phValue=14.2-phValue;

 if(phValue <8) {
  digitalWrite(ledPinHijau, HIGH);
 }
 else if(phValue >8) {
  digitalWrite(ledPinHijau, LOW);
 }
  if(phValue >9) {
  digitalWrite(ledPinMerah, HIGH);
 }
 else if(phValue <9) {
  digitalWrite(ledPinMerah, LOW);
 }
  lcd.setCursor(10, 2);
  lcd.print(phValue, 2);
  lcd.setCursor(6, 3);
  lcd.print(turbidity, 3);
  
  Serial.print(phValue);
  Serial.print("#");
  Serial.println(turbidity, 3);
  delay(500);
  //String datakirim = String(phValue) + "#" + String(turbidity);
  //Serial.println(datakirim);
}
