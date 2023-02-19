#include <dht11.h>
#include <LiquidCrystal_I2C.h>
#include <SFE_BMP180.h>
#include <Wire.h>

struct cihaz_pinleri {
  byte pinler[2]={3,4};
};
struct veriler {
  double sicaklik,nem,basinc,rakim,deniz_seviyesi_basinci;
  int yukseklik=913; // BULUNULAN BÖLGENİN DENİZ SEVİYESİNDEN YÜKSEKLİĞİ
  double gelen_veriler[4];
  unsigned long zaman=0;
};
struct veriler veri;
struct cihaz_pinleri pin;
bool ekran_durum=false;
LiquidCrystal_I2C lcd(0x27,16,2);
SFE_BMP180 bmp;
dht11 dht;

void setup() {
  Serial.begin(9600);
  Wire.begin();lcd.init();lcd.backlight();
  bmp.begin();
  if(!Serial)
  {
    while(!bool(0));
  }
  else
  {
    for(int i=0; i<2; i++)
    {
      if(i==1) {pinMode(pin.pinler[i],OUTPUT);}
      else {pinMode(pin.pinler[i],INPUT);}
    }
    gorsel_ekran();
  }

}

void loop() {
  
  sistem(veri.yukseklik);
  delay(1);
}

void gorsel_ekran() {

  if(!bool(0))
  {
    lcd.setCursor(1,0);lcd.print("HAVA ISTASYONU");
    lcd.setCursor(3,1);
    for(int i=0; i<10; i++){
      lcd.print("*");delay(100);
    }
  }
}


void sistem(int yukseklik) {
  if(!bool(0))
  {
    dht.read(pin.pinler[0]);
    veri.nem=(double)dht.humidity;
    bmp.startTemperature();
    delay(1000);
    bmp.getTemperature(veri.sicaklik);
    bmp.startPressure(3);
    delay(1000);bmp.getPressure(veri.basinc,veri.sicaklik);
    veri.basinc=(veri.basinc*0.0295333727,2)*(.03); //inHg inç civa birimi 1 inHg ise  yaklaşık 0.03 atm demektir.
    
    veri.deniz_seviyesi_basinci=bmp.sealevel(veri.basinc,yukseklik); 
    veri.rakim=bmp.altitude(veri.basinc,veri.deniz_seviyesi_basinci);  
  }
  if(!bool(0))
  {
    lcd.clear();
    lcd.setCursor(0,0);lcd.print("SICAKLIK(C)=");lcd.print(veri.sicaklik);
    lcd.setCursor(0,1);lcd.print("NEM(%)=");lcd.print(veri.nem);
    delay(2000);lcd.clear();
    lcd.setCursor(0,0);lcd.print("BASINC(atm)=");lcd.print(veri.basinc);
    lcd.setCursor(0,1);lcd.print("RAKIM(m)=");lcd.print(veri.rakim);
    delay(2000);lcd.clear();lcd.noBacklight();;delay(10000);digitalWrite(pin.pinler[1],HIGH);delay(1000);digitalWrite(pin.pinler[1],LOW);
    lcd.setCursor(0,0);lcd.backlight();lcd.print("OLCUM YAPILIYOR"); 
  }
}
