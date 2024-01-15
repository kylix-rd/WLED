#pragma once

#include "wled.h"

/*
 * Usermods allow you to add own functionality to WLED more easily
 * See: https://github.com/Aircoookie/WLED/wiki/Add-own-functionality
 * 
 * This is an example for a v2 usermod.
 * v2 usermods are class inheritance based and can (but don't have to) implement more functions, each of them is shown in this example.
 * Multiple v2 usermods can be added to one compilation easily.
 * 
 * Creating a usermod:
 * This file serves as an example. If you want to create a usermod, it is recommended to use usermod_v2_empty.h from the usermods folder as a template.
 * Please remember to rename the class and file to a descriptive name.
 * You may also use multiple .h and .cpp files.
 * 
 * Using a usermod:
 * 1. Copy the usermod into the sketch folder (same folder as wled00.ino)
 * 2. Register the usermod by adding #include "usermod_filename.h" in the top and registerUsermod(new MyUsermodClass()) in the bottom of usermods_list.cpp
 */

#define heartbeatLockout 500  // Heartbeats that happen within this many milliseconds are ignored
#define heartbeatDelta 300  // Drop in reflected IR that constitutes a heartbeat

#define randomPulsesEnabled true  // Fire random rainbow pulses from random nodes
#define cubePulsesEnabled true  // Draw cubes at random nodes
#define starburstPulsesEnabled true  // Draw starbursts
#define simulatedBiometricsEnabled false  // Simulate heartbeat and EDA ripples

#define autoPulseTimeout 5000  // If no heartbeat is received in this many ms, begin firing random/simulated pulses
#define randomPulseTime 2000  // Fire a random pulse every (this many) ms
#define autoPulseChangeTime 30000

#define simulatedHeartbeatBaseTime 600  // Fire a simulated heartbeat pulse after at least this many ms
#define simulatedHeartbeatVariance 200  // Add random jitter to simulated heartbeat
#define simulatedEdaBaseTime 1000  // Same, but for inward EDA pulses
#define simulatedEdaVariance 10000

//class name. Use something descriptive and leave the ": public Usermod" part :)
class ChromanceUserMod : public Usermod {
 private:
  static ChromanceUserMod* this_mod;
  // Private class members. You can declare variables and functions only accessible to your usermod here
  bool enabled = false;
  bool initDone = false;
  unsigned long lastTime = 0;

  // Segments
  bool segment_state[40];

  // Simulations/Features
  bool randomPulses = randomPulsesEnabled;
  bool cubePulses = cubePulsesEnabled;
  bool starburstPulses = starburstPulsesEnabled;
  bool simulatedBiometrics = simulatedBiometricsEnabled;

  unsigned long lastHeartbeat;  // Track last heartbeat so we can detect noise/disconnections

  unsigned long lastRandomPulse;
  byte lastAutoPulseNode = 255;

  byte numberOfAutoPulseTypes = randomPulsesEnabled + cubePulsesEnabled + starburstPulsesEnabled;
  byte currentAutoPulseType = 255;
  unsigned long lastAutoPulseChange;

  unsigned long nextSimulatedHeartbeat = millis();
  unsigned long nextSimulatedEda = millis();

  // string that are used multiple time (this will save some flash memory)
  static const char _name[];
  static const char _enabled[];
  static const char _randomPulses[];
  static const char _cubePulses[];
  static const char _starburstPulses[];
  static const char _simulatedBiometrics[];
  static const char _segmentState[];

  // any private methods should go here (non-inline method should be defined out of class)
  void publishMqtt(const char* state, bool retain = false); // example for publishing MQTT message


  public:

    ChromanceUserMod();

    // non WLED related methods, may be used for data exchange between usermods (non-inline methods should be defined out of class)

    /**
     * Enable/Disable the usermod
     */
    inline void enable(bool enable) { enabled = enable; }

    /**
     * Get usermod enabled/disabled state
     */
    inline bool isEnabled() { return enabled; }

    // in such case add the following to another usermod:
    //  in private vars:
    //   #ifdef USERMOD_EXAMPLE
    //   MyExampleUsermod* UM;
    //   #endif
    //  in setup()
    //   #ifdef USERMOD_EXAMPLE
    //   UM = (MyExampleUsermod*) usermods.lookup(USERMOD_ID_EXAMPLE);
    //   #endif
    //  somewhere in loop() or other member method
    //   #ifdef USERMOD_EXAMPLE
    //   if (UM != nullptr) isExampleEnabled = UM->isEnabled();
    //   if (!isExampleEnabled) UM->enable(true);
    //   #endif


    // methods called by WLED (can be inlined as they are called only once but if you call them explicitly define them out of class)

    /*
     * setup() is called once at boot. WiFi is not yet connected at this point.
     * readFromConfig() is called prior to setup()
     * You can use it to initialize variables, sensors or similar.
     */
    void setup() override;

    /*
     * connected() is called every time the WiFi is (re)connected
     * Use it to initialize network interfaces
     */
    void connected() override;


    /*
     * loop() is called continuously. Here you can check for events, read sensors, etc.
     * 
     * Tips:
     * 1. You can use "if (WLED_CONNECTED)" to check for a successful network connection.
     *    Additionally, "if (WLED_MQTT_CONNECTED)" is available to check for a connection to an MQTT broker.
     * 
     * 2. Try to avoid using the delay() function. NEVER use delays longer than 10 milliseconds.
     *    Instead, use a timer check as shown here.
     */
    void loop() override;

    uint16_t mode_chromance();

    /*
     * addToJsonInfo() can be used to add custom entries to the /json/info part of the JSON API.
     * Creating an "u" object allows you to add custom key/value pairs to the Info section of the WLED web UI.
     * Below it is shown how this could be used for e.g. a light sensor
     */
    void addToJsonInfo(JsonObject& root) override;


    /*
     * addToJsonState() can be used to add custom entries to the /json/state part of the JSON API (state object).
     * Values in the state object may be modified by connected clients
     */
    void addToJsonState(JsonObject& root) override;


    /*
     * readFromJsonState() can be used to receive data clients send to the /json/state part of the JSON API (state object).
     * Values in the state object may be modified by connected clients
     */
    void readFromJsonState(JsonObject& root) override;


    /*
     * addToConfig() can be used to add custom persistent settings to the cfg.json file in the "um" (usermod) object.
     * It will be called by WLED when settings are actually saved (for example, LED settings are saved)
     * If you want to force saving the current state, use serializeConfig() in your loop().
     * 
     * CAUTION: serializeConfig() will initiate a filesystem write operation.
     * It might cause the LEDs to stutter and will cause flash wear if called too often.
     * Use it sparingly and always in the loop, never in network callbacks!
     * 
     * addToConfig() will make your settings editable through the Usermod Settings page automatically.
     *
     * Usermod Settings Overview:
     * - Numeric values are treated as floats in the browser.
     *   - If the numeric value entered into the browser contains a decimal point, it will be parsed as a C float
     *     before being returned to the Usermod.  The float data type has only 6-7 decimal digits of precision, and
     *     doubles are not supported, numbers will be rounded to the nearest float value when being parsed.
     *     The range accepted by the input field is +/- 1.175494351e-38 to +/- 3.402823466e+38.
     *   - If the numeric value entered into the browser doesn't contain a decimal point, it will be parsed as a
     *     C int32_t (range: -2147483648 to 2147483647) before being returned to the usermod.
     *     Overflows or underflows are truncated to the max/min value for an int32_t, and again truncated to the type
     *     used in the Usermod when reading the value from ArduinoJson.
     * - Pin values can be treated differently from an integer value by using the key name "pin"
     *   - "pin" can contain a single or array of integer values
     *   - On the Usermod Settings page there is simple checking for pin conflicts and warnings for special pins
     *     - Red color indicates a conflict.  Yellow color indicates a pin with a warning (e.g. an input-only pin)
     *   - Tip: use int8_t to store the pin value in the Usermod, so a -1 value (pin not set) can be used
     *
     * See usermod_v2_auto_save.h for an example that saves Flash space by reusing ArduinoJson key name strings
     * 
     * If you need a dedicated settings page with custom layout for your Usermod, that takes a lot more work.  
     * You will have to add the setting to the HTML, xml.cpp and set.cpp manually.
     * See the WLED Soundreactive fork (code and wiki) for reference.  https://github.com/atuline/WLED
     * 
     * I highly recommend checking out the basics of ArduinoJson serialization and deserialization in order to use custom settings!
     */
    void addToConfig(JsonObject& root) override;


    /*
     * readFromConfig() can be used to read back the custom settings you added with addToConfig().
     * This is called by WLED when settings are loaded (currently this only happens immediately after boot, or after saving on the Usermod Settings page)
     * 
     * readFromConfig() is called BEFORE setup(). This means you can use your persistent values in setup() (e.g. pin assignments, buffer sizes),
     * but also that if you want to write persistent values to a dynamic buffer, you'd need to allocate it here instead of in setup.
     * If you don't know what that is, don't fret. It most likely doesn't affect your use case :)
     * 
     * Return true in case the config values returned from Usermod Settings were complete, or false if you'd like WLED to save your defaults to disk (so any missing values are editable in Usermod Settings)
     * 
     * getJsonValue() returns false if the value is missing, or copies the value into the variable provided and returns true if the value is present
     * The configComplete variable is true only if the "exampleUsermod" object and all values are present.  If any values are missing, WLED will know to call addToConfig() to save them
     * 
     * This function is guaranteed to be called on boot, but could also be called every time settings are updated
     */
    bool readFromConfig(JsonObject& root) override;


    /*
     * appendConfigData() is called when user enters usermod settings page
     * it may add additional metadata for certain entry fields (adding drop down is possible)
     * be careful not to add too much as oappend() buffer is limited to 3k
     */
    void appendConfigData() override;


    /*
     * handleOverlayDraw() is called just before every show() (LED strip update frame) after effects have set the colors.
     * Use this to blank out some LEDs or set them to a different color regardless of the set effect mode.
     * Commonly used for custom clocks (Cronixie, 7 segment)
     */
    void handleOverlayDraw() override;


    /**
     * handleButton() can be used to override default button behaviour. Returning true
     * will prevent button working in a default way.
     * Replicating button.cpp
     */
    bool handleButton(uint8_t b) override;
  

#ifndef WLED_DISABLE_MQTT
    /**
     * handling of MQTT message
     * topic only contains stripped topic (part after /wled/MAC)
     */
    bool onMqttMessage(char* topic, char* payload) override;

    /**
     * onMqttConnect() is called when MQTT connection is established
     */
    void onMqttConnect(bool sessionPresent) override;
#endif


    /**
     * onStateChanged() is used to detect WLED state change
     * @mode parameter is CALL_MODE_... parameter used for notifications
     */
    void onStateChange(uint8_t mode) override;


    /*
     * getId() allows you to optionally give your V2 usermod an unique ID (please define it in const.h!).
     * This could be used in the future for the system to determine whether your usermod is installed.
     */
    uint16_t getId() override;

   //More methods can be added in the future, this example will then be extended.
   //Your usermod will remain compatible as it does not need to implement all methods from the Usermod base class!
  static ChromanceUserMod* Get();
};
