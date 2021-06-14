/*
 * Conectarea pinolor la placa Arduino UNO:
 * LCD RS -> 11
 * LCD E (Enable) -> 12
 * LCD DB4 -> 4
 * LCD DB5 -> 5
 * LCD DB6 -> 6
 * LCD DB7 -> 7
 * LCD RW -> GND
 * LCD VSS -> GND
 * LCD V0 -> Rezistor 700hm -> GND
 * LCD VCC -> 5V
 */

// include libraria pentru LCD
#include <LiquidCrystal.h>

// asociere pini Arduino <-> LCD
const byte RS = 11, EN = 12;
const byte DB4 = 4, DB5 = 5, DB6 = 6, DB7 = 7;
LiquidCrystal lcd(RS, EN, DB4, DB5, DB6, DB7);

int contor = 0;

int ore = 0;
int minute = 0;
int secunde = 0;

int unitati = 0;
int zeci = 0;
int sute = 0;

bool butonPornire = false;
bool butonOprire = false;

void setup()
{
  // definire numar linii si coloane pentru LCD
    lcd.begin(16, 2);
    // afisare mesaj pe prima linie a LCD-ului
    lcd.print("Timer: ");
  
    // setare pini butoane 
    DDRD |= (1 << 2) | (1 << 3);
    
    // configurare intreruperi externe
    EICRA |= (1 << ISC11) | (1 << ISC10) | (1 << ISC01) |
      (1 << ISC00);
    EIMSK |= (1 << INT1) | (1 << INT0);
    EIFR |= (0 << INTF1) | (0 << INTF0);
    PCICR |= (0 << PCIE2) | (0 << PCIE1) | (0 << PCIE0);
  
    // setare OCR pentru intrerupere
    OCR1A = 24999;
    // porneste CTC mode
    // TCCR1B = registru de control
    TCCR1B |= (1 << WGM12);
    // setare CS12 si CS10 bits pentru prescalar pe 64 de biti
    TCCR1B = (0 << CS12) | (1 << CS10);
    // activare intreruperi globale
    TIMSK1 |= (1 << OCIE1A);
  
    // activare intreruperi globale
    SREG |= (1<<SREG_I);
  
    
  
}

void loop()
{
  // daca butonul este apasat cronometrul este resetat
  if (butonOprire == true) 
  {
    ore = 0;
    minute = 0;
    secunde = 0;
    
    unitati = 0;
    zeci = 0;
    sute = 0;
   
    butonPornire = false;
    butonOprire = false;
    }
  
  lcd.setCursor(0, 1);
  if (ore < 10)
  {
    lcd.print(0);
    lcd.setCursor(1,1);
  }
    lcd.print(ore);
    lcd.setCursor(2,1);
    lcd.print(":");
    lcd.setCursor(3,1);
  
  if (minute < 10)
  {
    lcd.print(0);
    lcd.setCursor(4,1);
  }
  lcd.print(minute);
  lcd.setCursor(5,1);
  lcd.print(":");
  lcd.setCursor(6,1);
  
  if(secunde<10)
  {
    lcd.print(0);
    lcd.setCursor(7,1);
  }
  lcd.print(secunde);
  lcd.setCursor(8,1);
  lcd.print(":");
  lcd.setCursor(9,1);
  lcd.print(unitati);
  lcd.setCursor(10,1);
  lcd.print(zeci);
  lcd.setCursor(11,1);
  lcd.print(sute);
  
}

ISR(TIMER1_COMPA_vect)
{
  
  // dezactivare intreruperi globale
  SREG &= ~(1 << SREG_I);
  
  // pornim cronometrul
  if(butonPornire==true)
  {
    // folosim unitati, zeci si sute pentru milisecundele
    // din cronometru
    ++unitati;
    if(unitati == 10)
    {
      zeci++;
      unitati %= 10;
    }
    if(zeci == 10)
    {
      sute++;
      zeci%=10;
    }
    if(sute == 10)
    {
      secunde++;
       sute%= 10;
    }
    // daca secundele ajung la valoarea de 60 
    // se incrementeaza minutele cu +1
    if(secunde == 60)
    {
      minute++;
      secunde %= 60;
    }
    // daca minutele ajung la valoarea de 60 
    // se incrementeaza orele cu +1
    if(minute == 60)
    {
      ore++;
      minute %= 60;
    }
     // daca orele ajung la valoarea de 24
    // se incrementeaza minutele cu +1
    // iar restul valorilor se reseteaza
    if (ore == 24)
    {
      sute == 0;
      zeci == 0;
      unitati == 0;
      
      secunde == 0;
      minute == 0;
      ore == 0;
      sute++;
    }
  }
  //activare intreruperi globale
 SREG |= (1 << SREG_I);
 }

 // rutina de intrerupere pentru INT0 
 ISR(INT0_vect)
  {
    // dezactivare intreruperi globale
    SREG &= ~(1 << SREG_I);
   
    if (butonPornire ==false)
    {
      butonPornire = true;
    }  
    else 
    {
      butonPornire=false;
    } 
   // activare intreruperi globale
    SREG |= (1 << SREG_I);
  }

  // rutine intrerupere pentru INT1
  ISR (INT1_vect)
  {
    // dezactivare intreruperi globale
    SREG &= ~(1 << SREG_I);
    
    if(butonPornire==true)
    {
      butonPornire=false;
      butonOprire =true;
    }  
    // activare intreruperi globale
    SREG |= (1 << SREG_I);
  }
