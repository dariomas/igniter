// demonstrate the use of WiFi.mode(SHUTDOWN/RESUME)

// current on wemos d1 mini (including: ldo, usbserial chip):
// ~85mA during normal operations
// ~30mA during wifi shutdown
//  ~5mA during deepsleep
#include <ESP8266WiFi.h>

//Infrared Input: 4
//Red: 14
//Green: 12
//Blue: 5
//White: 13
//Unused: 16
#include <jled.h>
static const int redPin = 14;//D5;
static const int greenPin = 12;//D1;
static const int bluePin = 5;//D6;
#define RED_PIN       5
#define GREEN_PIN     14
#define BLUE_PIN      12
#define WHITE_PIN     13

#define PWM_MAX       255
#define PWM_MIN       0
#define PWM_FREQUENCY 500

#ifdef IR_REMOTE
#define RECV_PIN  4
#include <IRremoteESP8266.h>    // https://github.com/markszabo/IRremoteESP8266 - v2.5.4
#include <IRrecv.h>
#include <IRutils.h>
#endif

  // configured to read the V3.3 voltage
ADC_MODE(ADC_VCC);

// preinit() is called before system startup
// from nonos-sdk's user entry point user_init()
void preinit() {
  // Global WiFi constructors are not called yet
  // (global class instances like WiFi, Serial... are not yet initialized)..
  // No global object methods or C++ exceptions can be called in here!
  //The below is a static class method, which is similar to a function, so it's ok.
  ESP8266WiFiClass::preinitWiFiOff();
}

void stopWiFiAndSleep() {
  WiFi.persistent(false);
  WiFi.disconnect();
  WiFi.mode(WIFI_OFF);
  WiFi.forceSleepBegin();
  delay(1);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  
  while(!Serial);
  Serial.println("\nsleeping 5s");
  // during this period, a simple amp meter shows
  // an average of 20mA with a Wemos D1 mini
  // a DSO is needed to check #2111
  delay(5000);

}

void loop() {
// output the battery value
  static int Batt;
  Batt = ESP.getVcc();
  Serial.print("Battery voltage is: ");  
  Serial.println(Batt);
  // If the battery is discharged don't go any further!!!
//  if(Batt < 3000){
//     // Deep sleep for as long as you can
//     ESP.deepSleep(ESP.deepSleepMax());
//  }
  delay(3000);  // refresh the page automatically every 3 sec


}
