#include "chromance.h"
#include "const.h"
#include "mapping.h"
#include "ripple.h"

// methods called by WLED (can be inlined as they are called only once but if you call them explicitly define them out of class)

byte ledColors[40][14][3];     // LED buffer - each ripple writes to this, then we write this to the strips
constexpr float decay = 0.97;  // Multiply all LED's by this amount each tick to create fancy fading tails

// These ripples are endlessly reused so we don't need to do any memory management
#define numberOfRipples 30
Ripple ripples[numberOfRipples] = {
  Ripple(0),
  Ripple(1),
  Ripple(2),
  Ripple(3),
  Ripple(4),
  Ripple(5),
  Ripple(6),
  Ripple(7),
  Ripple(8),
  Ripple(9),
  Ripple(10),
  Ripple(11),
  Ripple(12),
  Ripple(13),
  Ripple(14),
  Ripple(15),
  Ripple(16),
  Ripple(17),
  Ripple(18),
  Ripple(19),
  Ripple(20),
  Ripple(21),
  Ripple(22),
  Ripple(23),
  Ripple(24),
  Ripple(25),
  Ripple(26),
  Ripple(27),
  Ripple(28),
  Ripple(29),
};

void ChromanceUserMod::setup() {
  // do your set-up here
  Serial.println("Hello from my Chromance!");
  initDone = true;
}


void ChromanceUserMod::connected() {
  Serial.println("Connected to WiFi!");
}


void ChromanceUserMod::loop() {

  // Fade all dots to create trails
  for (int strip = 0; strip < 40; strip++) {
    for (int led = 0; led < 14; led++) {
      for (int i = 0; i < 3; i++) {
        ledColors[strip][led][i] *= decay;
      }
    }
  }

  for (int i = 0; i < numberOfRipples; i++) {
    ripples[i].advance(ledColors);
  }

  for (int segment = 0; segment < 40; segment++) {
    for (int fromBottom = 0; fromBottom < 14; fromBottom++) {
      int strip_no = ledAssignments[segment][0];
      int led = round(fmap(
                        fromBottom,
                        0, 13,
                        ledAssignments[segment][2], ledAssignments[segment][1]));
      strip.getSegment(strip_no).setPixelColor(
        led,
        ledColors[segment][fromBottom][0],
        ledColors[segment][fromBottom][1],
        ledColors[segment][fromBottom][2]);
    }
  }

  strip.trigger();
}


void ChromanceUserMod::addToJsonInfo(JsonObject& root)
{
    // if "u" object does not exist yet wee need to create it
    JsonObject user = root["u"];
    if (user.isNull()) user = root.createNestedObject("u");

    //this code adds "u":{"ExampleUsermod":[20," lux"]} to the info object
    //int reading = 20;
    //JsonArray lightArr = user.createNestedArray(FPSTR(_name))); //name
    //lightArr.add(reading); //value
    //lightArr.add(F(" lux")); //unit

    // if you are implementing a sensor usermod, you may publish sensor data
    //JsonObject sensor = root[F("sensor")];
    //if (sensor.isNull()) sensor = root.createNestedObject(F("sensor"));
    //temp = sensor.createNestedArray(F("light"));
    //temp.add(reading);
    //temp.add(F("lux"));
}


void ChromanceUserMod::addToJsonState(JsonObject& root)
{
    if (!initDone || !enabled) return;  // prevent crash on boot applyPreset()

    JsonObject usermod = root[FPSTR(_name)];
    if (usermod.isNull()) usermod = root.createNestedObject(FPSTR(_name));

    //usermod["user0"] = userVar0;
}


void ChromanceUserMod::readFromJsonState(JsonObject& root)
{
    if (!initDone) return;  // prevent crash on boot applyPreset()

    JsonObject usermod = root[FPSTR(_name)];
    if (!usermod.isNull()) {
    // expect JSON usermod data in usermod name object: {"ExampleUsermod:{"user0":10}"}
    userVar0 = usermod["user0"] | userVar0; //if "user0" key exists in JSON, update, else keep old value
    }
    // you can as well check WLED state JSON keys
    //if (root["bri"] == 255) Serial.println(F("Don't burn down your garage!"));
}


void ChromanceUserMod::addToConfig(JsonObject& root)
{
    JsonObject top = root.createNestedObject(FPSTR(_name));
    top[FPSTR(_enabled)] = enabled;
    //save these vars persistently whenever settings are saved
    top["great"] = userVar0;
    top["testBool"] = testBool;
    top["testInt"] = testInt;
    top["testLong"] = testLong;
    top["testULong"] = testULong;
    top["testFloat"] = testFloat;
    top["testString"] = testString;
    JsonArray pinArray = top.createNestedArray("pin");
    pinArray.add(testPins[0]);
    pinArray.add(testPins[1]); 
}


bool ChromanceUserMod::readFromConfig(JsonObject& root)
{
    // default settings values could be set here (or below using the 3-argument getJsonValue()) instead of in the class definition or constructor
    // setting them inside readFromConfig() is slightly more robust, handling the rare but plausible use case of single value being missing after boot (e.g. if the cfg.json was manually edited and a value was removed)

    JsonObject top = root[FPSTR(_name)];

    bool configComplete = !top.isNull();

    configComplete &= getJsonValue(top["great"], userVar0);
    configComplete &= getJsonValue(top["testBool"], testBool);
    configComplete &= getJsonValue(top["testULong"], testULong);
    configComplete &= getJsonValue(top["testFloat"], testFloat);
    configComplete &= getJsonValue(top["testString"], testString);

    // A 3-argument getJsonValue() assigns the 3rd argument as a default value if the Json value is missing
    configComplete &= getJsonValue(top["testInt"], testInt, 42);  
    configComplete &= getJsonValue(top["testLong"], testLong, -42424242);

    // "pin" fields have special handling in settings page (or some_pin as well)
    configComplete &= getJsonValue(top["pin"][0], testPins[0], -1);
    configComplete &= getJsonValue(top["pin"][1], testPins[1], -1);

    return configComplete;
}

void ChromanceUserMod::appendConfigData()
{
    oappend(SET_F("addInfo('")); oappend(String(FPSTR(_name)).c_str()); oappend(SET_F(":great")); oappend(SET_F("',1,'<i>(this is a great config value)</i>');"));
    oappend(SET_F("addInfo('")); oappend(String(FPSTR(_name)).c_str()); oappend(SET_F(":testString")); oappend(SET_F("',1,'enter any string you want');"));
    oappend(SET_F("dd=addDropdown('")); oappend(String(FPSTR(_name)).c_str()); oappend(SET_F("','testInt');"));
    oappend(SET_F("addOption(dd,'Nothing',0);"));
    oappend(SET_F("addOption(dd,'Everything',42);"));
}

void ChromanceUserMod::handleOverlayDraw()
{
    //strip.setPixelColor(0, RGBW32(0,0,0,0)) // set the first pixel to black
}

bool ChromanceUserMod::handleButton(uint8_t b) {
    yield();
    // ignore certain button types as they may have other consequences
    if (!enabled
    || buttonType[b] == BTN_TYPE_NONE
    || buttonType[b] == BTN_TYPE_RESERVED
    || buttonType[b] == BTN_TYPE_PIR_SENSOR
    || buttonType[b] == BTN_TYPE_ANALOG
    || buttonType[b] == BTN_TYPE_ANALOG_INVERTED) {
    return false;
    }

    bool handled = false;
    // do your button handling here
    return handled;
}


#ifndef WLED_DISABLE_MQTT
bool ChromanceUserMod::onMqttMessage(char* topic, char* payload) {
    // check if we received a command
    //if (strlen(topic) == 8 && strncmp_P(topic, PSTR("/command"), 8) == 0) {
    //  String action = payload;
    //  if (action == "on") {
    //    enabled = true;
    //    return true;
    //  } else if (action == "off") {
    //    enabled = false;
    //    return true;
    //  } else if (action == "toggle") {
    //    enabled = !enabled;
    //    return true;
    //  }
    //}
    return false;
}

void ChromanceUserMod::onMqttConnect(bool sessionPresent) {
    // do any MQTT related initialisation here
    //publishMqtt("I am alive!");
}
#endif


void ChromanceUserMod::onStateChange(uint8_t mode) {
    // do something if WLED state changed (color, brightness, effect, preset, etc)
}

uint16_t ChromanceUserMod::getId()
{
    return USERMOD_ID_CHROMANCE;
}


// add more strings here to reduce flash memory usage
const char ChromanceUserMod::_name[]    PROGMEM = "ChromanceUserMod";
const char ChromanceUserMod::_enabled[] PROGMEM = "enabled";


// implementation of non-inline member methods

void ChromanceUserMod::publishMqtt(const char* state, bool retain)
{
#ifndef WLED_DISABLE_MQTT
  //Check if MQTT Connected, otherwise it will crash the 8266
  if (WLED_MQTT_CONNECTED) {
    char subuf[64];
    strcpy(subuf, mqttDeviceTopic);
    strcat_P(subuf, PSTR("/example"));
    mqtt->publish(subuf, 0, retain, state);
  }
#endif
}
