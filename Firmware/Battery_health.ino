#include <SparkFunBQ27441.h>
#include <Adafruit_NeoPixel.h>

// Constants
#define DATAPIN            2       // Pin connected to the NeoPixel data input
#define NUMPIXELS          20      // Number of NeoPixels in the LED bar
#define BATTERY_CAPACITY   1300    // Battery full capacity (mAh) - adjust based on your battery
#define SERIAL_BAUD_RATE   115200  // Serial communication baud rate

// NeoPixel object
Adafruit_NeoPixel BARS = Adafruit_NeoPixel(NUMPIXELS, DATAPIN, NEO_GRB + NEO_KHZ800);

// Global variables
unsigned int Charge = 0;           // Battery State of Charge (SoC)
unsigned int health = 0;           // Battery State of Health (SoH)
unsigned int PreviousCharge = 0;   // Previous SoC value for comparison
int R_SOC = 0, G_SOC = 0, B_SOC = 0; // RGB values for SoC LED bar
int R_SOH = 0, G_SOH = 0, B_SOH = 0; // RGB values for SoH LED bar

// Function to initialize the BQ27441 fuel gauge
void InitBQ27441() {
  if (!lipo.begin()) { // Initialize communication with the BQ27441 IC
    Serial.println("Error: Unable to communicate with BQ27441.");
    Serial.println("Check wiring and try again.");
    delay(500);
    while (1); // Halt if communication fails
  }
  Serial.println("Successfully Connected to BQ27441!");
  Serial.println("SOC | voltage | current | capacity | power | SOH");
  lipo.setCapacity(BATTERY_CAPACITY); // Set battery capacity
}

// Function to log battery data
void BatteryDataLogging() {
  // Get battery data from BQ27441
  unsigned int volts = lipo.voltage();             // Battery voltage (mV)
  int current = lipo.current(AVG);                 // Average current (mA)
  unsigned int fullCapacity = lipo.capacity(FULL); // Full capacity (mAh)
  unsigned int capacity = lipo.capacity(REMAIN);   // Remaining capacity (mAh)
  int power = lipo.power();                        // Average power draw (mW)
  health = lipo.soh();                             // State of Health (SoH)

  // Print battery data to Serial Monitor
  String toPrint = String(Charge) + "%   | ";
  toPrint += String(volts) + " mV | ";
  toPrint += String(current) + " mA | ";
  toPrint += String(capacity) + " / ";
  toPrint += String(fullCapacity) + " mAh | ";
  toPrint += String(power) + " mW | ";
  toPrint += String(health) + "%";
  Serial.println(toPrint);
}

// Function to set LED bar color based on a parameter (SoC or SoH)
void BarsColor(unsigned int parameter, int* r, int* g, int* b) {
  if (parameter > 0 && parameter < 20) {
    *r = 100; *g = 0; *b = 0;    // Red for low values
  } else if (parameter >= 20 && parameter < 40) {
    *r = 100; *g = 8; *b = 0;    // Orange for medium-low values
  } else if (parameter >= 40 && parameter < 60) {
    *r = 100; *g = 90; *b = 0;   // Yellow for medium values
  } else if (parameter >= 60 && parameter < 80) {
    *r = 28; *g = 100; *b = 0;   // Light green for medium-high values
  } else if (parameter >= 80 && parameter <= 100) {
    *r = 0; *g = 100; *b = 0;    // Green for high values
  }
}

// Function to update the State of Health (SoH) LED bar
void SOHLedBarsIndicators() {
  BatteryDataLogging(); // Log battery data
  BarsColor(health, &R_SOH, &G_SOH, &B_SOH); // Set color based on SoH

  // Update LED bar for SoH
  for (int i = 20 - (health / 10); i < NUMPIXELS; i++) {
    BARS.setPixelColor(i, BARS.Color(R_SOH, G_SOH, B_SOH));
    delay(2);
  }

  // Handle partial segments (e.g., 23% -> 2 full LEDs + 1 partial LED)
  if ((health % 10) != 0) {
    int partialPixel = 20 - (health / 10) - 1;
    int brightnessFactor = 20 - (2 * (health % 10));
    BARS.setPixelColor(partialPixel, BARS.Color(R_SOH / brightnessFactor, G_SOH / brightnessFactor, B_SOH / brightnessFactor));
    delay(2);
  }
  BARS.show(); // Update the LED bar
}

// Function to update the State of Charge (SoC) LED bar
void SOCLedBarsIndicators() {
  Charge = lipo.soc(); // Get current State of Charge (SoC)

  // Update LED bar only if SoC has changed
  if (Charge != PreviousCharge) {
    PreviousCharge = Charge;
    BatteryDataLogging(); // Log battery data
    BarsColor(Charge, &R_SOC, &G_SOC, &B_SOC); // Set color based on SoC

    // Clear the first 10 LEDs (SoC bar)
    for (int i = 0; i < 10; i++) {
      BARS.setPixelColor(i, BARS.Color(0, 0, 0));
      delay(1);
    }
    BARS.show();

    // Update LED bar for SoC
    for (int i = 0; i < Charge / 10; i++) {
      BARS.setPixelColor(i, BARS.Color(R_SOC, G_SOC, B_SOC));
      delay(1);
    }

    // Handle partial segments (e.g., 23% -> 2 full LEDs + 1 partial LED)
    if ((Charge % 10) != 0) {
      int partialPixel = Charge / 10;
      int brightnessFactor = 20 - (2 * (Charge % 10));
      BARS.setPixelColor(partialPixel, BARS.Color(R_SOC / brightnessFactor, G_SOC / brightnessFactor, B_SOC / brightnessFactor));
      delay(2);
    }
    BARS.show(); // Update the LED bar
  }
}

void setup() {
  Serial.begin(SERIAL_BAUD_RATE); // Initialize Serial communication
  BARS.begin(); // Initialize NeoPixel LED bar

  // Turn off all LEDs initially
  for (int i = 0; i < NUMPIXELS; i++) {
    BARS.setPixelColor(i, BARS.Color(0, 0, 0));
    delay(2);
  }
  BARS.show();

  InitBQ27441(); // Initialize BQ27441 fuel gauge
  delay(50);
  SOHLedBarsIndicators(); // Display initial SoH
}

void loop() {
  SOCLedBarsIndicators(); // Update SoC LED bar
  delay(100); // Small delay to reduce CPU usage
}
