#include <SparkFunBQ27441.h>
#include <Adafruit_NeoPixel.h>

#define DATAPIN            2
#define NUMPIXELS          20

Adafruit_NeoPixel BARS = Adafruit_NeoPixel(NUMPIXELS, DATAPIN, NEO_GRB + NEO_KHZ800);

const unsigned int BATTERY_CAPACITY = 1300; // Battery full capacity (depend on the used Cell)
unsigned int Charge=0;                      // Battery initial State of charge set to zero
unsigned int health=0;                      // Battery initial State of Health set to zero 
unsigned int PreviousCharge=0;
int R_SOC=0,G_SOC=0,B_SOC=0;
int R_SOH=0,G_SOH=0,B_SOH=0;
void InitBQ27441(void)
{
  if (!lipo.begin()) //Init the communication to the BQ27441 IC
  {
  // If communication fails, the below message will be displayed.
    Serial.println("Error: Unable to communicate with BQ27441.");
    Serial.println("  Check wiring and try again.");
    delay(500);
    while (1) ;
  }
  Serial.println("Successfully Connected to BQ27441!");
  Serial.println("SOC | voltage | current |   capacity   |  power  | SOH");
  lipo.setCapacity(BATTERY_CAPACITY);
}

void BatteryDataLogging()
{
  // Get all the Battery Cell data from BQ27441-G1A
  //Charge = lipo.soc();                                // Read state-of-charge (%)
  unsigned int volts = lipo.voltage();             // Read battery voltage (mV)
  int current = lipo.current(AVG);                 // Read average current (mA)
  unsigned int fullCapacity = lipo.capacity(FULL); // Read full capacity (mAh)
  unsigned int capacity = lipo.capacity(REMAIN);   // Read remaining capacity (mAh)
  int power = lipo.power();                        // Read average power draw (mW)
  health = lipo.soh();                             // Read state-of-health (%)

  // Print out all those data:
  String toPrint = String(Charge) + "%   | ";
  toPrint += String(volts) + " mV | ";
  toPrint += String(current) + " mA | ";
  toPrint += String(capacity) + " / ";
  toPrint += String(fullCapacity) + " mAh | ";
  toPrint += String(power) + " mW | ";
  toPrint += String(health) + "%";
  
  Serial.println(toPrint);
}

void BarsColor(unsigned int parameter, int* r, int* g, int* b)
{
  if(parameter>0 && parameter < 20)
  { 
    *r=100;
    *g=0;
    *b=0;
  }
  if(parameter>=20 && parameter < 40)
  { 
    *r=100;
    *g=8;
    *b=0;
  }
  if(parameter>=40 && parameter < 60)
  { 
    *r=100;
    *g=90;
    *b=0;
  }
  if(parameter>=60 && parameter < 80)
  { 
    *r=28;
    *g=100;
    *b=0;
  }
  if(parameter>=80 && parameter <= 100)
  { 
    *r=0;
    *g=100;
    *b=0;
  }
}

void SOHLedBarsIndicators()
{
  BatteryDataLogging();
  BarsColor(health, &R_SOH, &G_SOH, &B_SOH);
  for(int i=20-(health/10) ; i < NUMPIXELS ; i++)
  {
    BARS.setPixelColor(i, BARS.Color(R_SOH,G_SOH,B_SOH)); // Moderately bright green color.
    delay(2);
  }
  if((health%10)!=0)
  {
    BARS.setPixelColor((20-(health/10))-1, BARS.Color(R_SOH/(20-(2*(health%10))),G_SOH/(20-(2*(health%10))),B_SOH/(20-(2*(health%10)))));
    delay(2);
  }
  BARS.show();
}

void SOCLedBarsIndicators()
{
  Charge = lipo.soc(); 
  if(Charge!=PreviousCharge)
  {
    PreviousCharge=Charge;
    BatteryDataLogging();
    BarsColor(Charge, &R_SOC, &G_SOC, &B_SOC);
    for(int i=0;i<10;i++)
    {
      BARS.setPixelColor(i, BARS.Color(0,0,0)); // Moderately bright green color.
      delay(1);
    }
    BARS.show();
    for(int i=0 ; i < Charge/10 ; i++)
    {
      BARS.setPixelColor(i, BARS.Color(R_SOC,G_SOC,B_SOC)); // Moderately bright green color.
      delay(1);
    }
    if((Charge%10)!=0)
    {
      BARS.setPixelColor(Charge/10, BARS.Color(R_SOC/(20-(2*(Charge%10))),G_SOC/(20-(2*(Charge%10))),B_SOC/(20-(2*(Charge%10)))));
      delay(2);
    }
    BARS.show();
  }
}
void setup()
{
  Serial.begin(115200);
  BARS.begin();
  for(int i=0;i<20;i++)
  {
    BARS.setPixelColor(i, BARS.Color(0,0,0)); // Moderately bright green color. 
    delay(2);
  }
  BARS.show();
  InitBQ27441();
  delay(50);
  SOHLedBarsIndicators();
}

void loop() 
{
  SOCLedBarsIndicators();
  delay(100);
}
