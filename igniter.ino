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
#define RED_PIN       (5)
#define GREEN_PIN     (12)
#define BLUE_PIN      (13)
#define WHITE_PIN     (14)
#define IR_PIN        (4)
#define UNUSED_PIN    (16)

#define PWM_MAX       (255)
#define PWM_MIN       (0)
#define PWM_FREQUENCY (500)
#define IR_REMOTE     1

#ifdef IR_REMOTE
// IRREMOTE defined
/*
FF906F  FFB847  FFF807  FFB04F
FF9867  FFD827  FF8877  FFA857
FFE817  FF48B7  FF6897  FFB24D
FF02FD  FF32CD  FF20DF  FF00FF
FF50AF  FF7887  FF708F  FF58A7
FF38C7  FF28D7  FFF00F  FF30CF
*/

#define KEY44_BRIGHTER 0xFF906F
#define KEY44_DIMMER 0xFFB847
#define KEY44_OFF 0xFFF807
#define KEY44_ON 0xFFB04F
#define KEY44_RED 0xFF9867
#define KEY44_GREEN 0xFFD827
#define KEY44_BLUE 0xFF8877
#define KEY44_WHITE 0xFFA857
#define KEY44_C1 0xFFE817
#define KEY44_C2 0xFF48B7
#define KEY44_C3 0xFF6897
#define KEY44_C4 0xFF02FD
#define KEY44_C5 0xFF32CD
#define KEY44_C6 0xFF20DF
#define KEY44_C7 0xFF50AF
#define KEY44_C8 0xFF7887
#define KEY44_C9 0xFF708F
#define KEY44_C10 0xFF38C7
#define KEY44_C11 0xFF28D7
#define KEY44_C12 0xFFF00F
#define KEY44_FLASH 0xFFB24D
#define KEY44_STROB 0xFF00FF
#define KEY44_FADE 0xFF58A7
#define KEY44_SMOOT 0xFF30CF

unsigned char pass = 0;

#define RECV_PIN  IR_PIN
#include <IRremoteESP8266.h>    // https://github.com/markszabo/IRremoteESP8266 - v2.5.4
#include <IRrecv.h>
#include <IRutils.h>
IRrecv irrecv(RECV_PIN);
decode_results results;
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
  // from now on, a simple amp meter shows
  // an average of 20mA with a Wemos D1 mini
}

/* void stopWiFiAndSleep() {
  WiFi.persistent(false);
  WiFi.disconnect();
  WiFi.mode(WIFI_OFF);
  WiFi.forceSleepBegin();
  delay(1);
}
 */
unsigned int period = 1000;
JLed leds[] = {
    JLed(RED_PIN).Breathe(period).Forever(),
    JLed(GREEN_PIN).Breathe(period).Forever(),
    JLed(BLUE_PIN).Breathe(period).Forever(),
    JLed(WHITE_PIN).Breathe(period).Forever()};
JLedSequence* sequence = NULL;

JLedSequence* changePeriod(JLedSequence* seq, unsigned int period)
{
  Serial.print("\nperiod: ");  Serial.println(period);
  leds[0].Breathe(period);
  leds[1].Breathe(period);
  leds[2].Breathe(period);
  leds[3].Breathe(period);

  if (seq)
    delete seq;
  seq = new JLedSequence(JLedSequence::eMode::PARALLEL, leds);
  return seq;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.setDebugOutput(true);

  // stopWiFiAndSleep();

  while(!Serial);
  Serial.println("\nIgniter v0.2\r\n Dariomas 2020 / 07");
  sequence = changePeriod(sequence, period);

  #ifdef IR_REMOTE
  irrecv.enableIRIn();
  #endif

  Serial.println("\nsleeping 0.5s");
  delay(500);

}


void loop() {
// output the battery value
  // static int Batt;
  // Batt = ESP.getVcc();
  // Serial.print("Battery voltage is: ");
  // Serial.println(Batt);
  // // If the battery is discharged don't go any further!!!
//  if(Batt < 3000){
//     // Deep sleep for as long as you can
//     ESP.deepSleep(ESP.deepSleepMax());
//  }
  //delay(3000);  // refresh the page automatically every 3 sec
  #ifdef IR_REMOTE
  if (irrecv.decode(&results)) {
    Serial.println((unsigned long)results.value, HEX);
    Serial.println(pass);
    irrecv.resume(); // Receive the next value
    switch (results.value) {
      case KEY44_BRIGHTER:
        if (pass == 3)
          sequence = changePeriod(sequence, period += 1000);
        pass = 0;
        break;
      case KEY44_DIMMER:
        if (pass == 3)
          sequence = changePeriod(sequence, period -= 1000);
        pass = 0;
        break;
      case KEY44_ON:
        sequence->Reset();
        pass = 0;
        break;
      case KEY44_OFF:
        sequence->Stop(); //off
        pass = 0;
        break;
      case KEY44_RED:
        if (pass == 2)
          pass = 0;
          // TODO: read flash !
        else
          pass = 0;
        break;
      case KEY44_GREEN:
        pass = 0;
        break;
      case KEY44_BLUE:
        pass = 0;
        break;
      case KEY44_WHITE:
         // on full
        if (pass == 2)
          pass = 0;
          // TODO: save flash !
        else
          pass = 0;
        break;
      case KEY44_C1:
        // dark orange
        pass = 0;
        break;
      case KEY44_C2:
        // greeny
        pass = 0;
        break;
      case KEY44_C3:
        // bluey
        pass = 0;
        break;
      case KEY44_C4:
        // orangy
        pass = 0;
        break;
      case KEY44_C5:
        // greeny blue
        pass = 0;
        break;
      case KEY44_C6:
        // purply blue
        pass = 0;
        break;
      case KEY44_C7:
        // pale yellow
        pass = 0;
        break;
      case KEY44_C8:
        // bluer green
        pass = 0;
        break;
      case KEY44_C9:
        // purple
        pass = 0;
        break;
      case KEY44_C10:
        // yellow
        pass = 0;
        break;
      case KEY44_C11:
        //  some blue green colour
        pass = 0;
        break;
      case KEY44_C12:
        // pink
        if (pass == 0)
          ++pass;
        else
          pass = 0;
        break;
      case KEY44_FLASH:
        if (pass == 0)
          pass = 3;
        else if (pass == 2)
          period = 1000;
        else
          pass = 0;
        break;
      case KEY44_STROB:
        if (pass == 1)
          ++pass;
        else
          pass = 0;
        break;
      case KEY44_FADE:
        pass = 0;
        break;
      case KEY44_SMOOT:
        pass = 0;
        break;
      case 0xFFFFFF:
        break;
    }
  }

  #endif


  sequence->Update();
}
