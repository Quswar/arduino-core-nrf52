/*
  CTSServer.ino
  
  Written by Chiara Ruggeri (chiara@arduino.org)

  This example shows the server capabilities of the
  BLECentralRole library.
  It acts as a CTS server and provides the current date and
  time to a peripheral connected to it.
  The complementary example, CTSClient.ino, can be found in
  File->Examples->BLE->Peripheral menu.
  To calculate current time and date RTC library is used.
  Please note that RTC library doesn't take care about the
  day of the week and the fraction of a second, so this
  informations are, for default, Monday (as day of the week)
  and 0 (as fraction of a second).
  The informations are transmitted as a string accordingly
  to CTS service specifications. To have further informations
  about CTS service please refer to the Bluetooth specifications:
  https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.service.current_time.xml

  This example code is in the public domain.
  
*/

#include <BLECentralRole.h>
#include <RTCInt.h>

// create central instance
BLECentralRole                   bleCentral                  = BLECentralRole();

// create service with UUID compliant to CTS service
BLEService                       ctsService                  = BLEService("1805");

// create a generic characteristics with UUID and properties compliant to CTS service 
//                                                                              ( UUID,  Properties,          length)
BLECharacteristic                ctsCharacteristic           = BLECharacteristic("2a2b", BLERead | BLENotify, 9);

RTCInt rtc;  //create an RTCInt type object


void setup() {
  Serial.begin(9600);
  delay(2000);
  Serial.println("start");
  rtc.begin(TIME_H24); //init RTC in 24 hour mode
  
  //time settings
  rtc.setHour(15,0);  //setting hour
  rtc.setMinute(02);  //setting minute
  rtc.setSecond(0);   //setting second
  
  
  rtc.setDay(16);     //setting day
  rtc.setMonth(1);    //setting month
  rtc.setYear(17);    //setting year
    
  // add service and characteristics
  bleCentral.addAttribute(ctsService);
  bleCentral.addAttribute(ctsCharacteristic);
  // assign event handlers for central events
  bleCentral.setEventHandler(BLEScanReceived, receiveAdvPck);  
  bleCentral.setEventHandler(BLEConnected, bleCentralConnectHandler);
  bleCentral.setEventHandler(BLEDisconnected, bleCentralDisconnectHandler);
 
  // set scan parameters
  // interval and window in 0.625 ms increments 
  bleCentral.setScanInterval(3200); // 2 sec
  bleCentral.setScanWindow(800);  // 0.5 sec

  // begin initialization and start scanning
  bleCentral.begin(); 
  
  Serial.println("Scanning...");
}

void loop() {
  if(bleCentral.connected()){
    rtc.getDate();      //getting date in local structure (local_date)
    rtc.getTime();      //getting time in local structure(local_time)
    
    // prepare data to send   
    unsigned char time[9];
    // year has to be splitted in two bytes
    uint16_t year = 2000 + rtc.date.year;
    time[0] = (uint8_t)(year & 0x00FF);
    time[1] = (uint8_t)((year &0xFF00) >> 8);
    
    time[2] = rtc.date.month;
    time[3] = rtc.date.day;
    time[4] = rtc.time.hour;
    time[5] = rtc.time.minute;
    time[6] = rtc.time.second;
    time[7] = 1; // rtc doesn't take into account the day of the week. Here Monday is used.
    time[8] = 0; // rtc doesn't take into account the fraction of a second. Here 0 is used.
    
    // update characteristic value
    ctsCharacteristic.setValue((unsigned char *)time, 9);
    
    }
    
    // update the value every second
    delay(1000);
}

void receiveAdvPck(BLENode& node){
  char advertisedName[31];
  byte len;
  // search for a device that advertises "CTS-Client" name
  node.getFieldInAdvPck(BLE_GAP_AD_TYPE_SHORT_LOCAL_NAME, advertisedName, len);
  if(len == 0) // field not found
      node.getFieldInAdvPck(BLE_GAP_AD_TYPE_COMPLETE_LOCAL_NAME, advertisedName, len);
  if(len != 0){ // the field was found
  Serial.println(advertisedName);
    if(!strcmp(advertisedName, "CTS-Client"))
      // Name found. Connect to the peripheral
      bleCentral.connect(node);
   } 
}

void bleCentralConnectHandler(BLENode& node) {
  // node connected event handler
  Serial.print("Connected event, central: ");
  Serial.println(node.address());
}

void bleCentralDisconnectHandler(BLENode& node) {
  // node disconnected event handler
  Serial.print("Disconnected event, central: ");
  Serial.println(node.address());
}