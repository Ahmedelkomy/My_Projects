


#include <WiFi.h>
#include<MQTT_protocol.h>




/***************************GLobal Func ***********************************/

void callback(char* topic, char* payload, unsigned int length);
void FIXED(void);
/******************************************************************************/


//#define devices 2


/***************************MACRO***********************************/

#define green D6                                    //Green
#define Orange D2                                    //Orange
#define blue D5                                    //BLUE
#define red D1                                    //Red
#define buzzer D0                                    //buzzer
#define IR D4                                     // D4 FOR 2
/******************************************************************************/


/***************************GLobal Var ***********************************/
char Estimate_Time = 0;
int CHECK_RAND = 0;
int randm_with_off = 0;
int random_number = 0;
char* mqtt_server = "test.mosquitto.org";
int tim = 0;
String MIX;
String Confirmation_devices;
static char arra_Confirmation_devices[20] = {0};
static char arra[20] = {0};
static char OLD_STATE_A[20] = {0};
static char ONTIME[20] = {0};
static char COLOR[20] = {0};
static char Server_IP[20] = {0};
static char SWITCH[20] = "fals";
static char OFFTIME[20] = {0};
int SYSTEM_START = 0;
int SYSTEM_STOPPED = 0;
int i = 0;
int call = 0;
int devices = 0;
int MULTIPLE_DEVICES = 0;
int flag = 0;
int flag1 = 0;
int flag2 = 1;
int flag3 = 0;
int flag4 = 0;
int flag5 = 0;
int sensor = 1;
int buzzzer = 0;
int buzzzer_STATS = 0;
int cnt = 0;
int DEVIC_NO = 0;
int NOT_CNT = 0;
int OLD_STATE = 0;
int SENT_INJ_FOR_START = 0;
int DELAY_B_TWO_DEV = 1;
int Check_Random_Number = 0;

/******************************************************************************/

WiFiClient espclient;
PubSubClient client(mqtt_server, 1883, callback, espclient);




void IRsensor()
{
  sensor = digitalRead(IR);
  if ((sensor == 0) && (flag == 1))
  {
    cnt++;
    dtostrf ((cnt), 4, 0, arra);
    client.publish("COUNT4", arra);                         //change
    client.publish("GENRAL_COUNT", "1");
    dtostrf ((tim * 10), 4, 0, arra);
    client.publish("CUT_OFF4", arra);                       //change
    SENT_INJ_FOR_START = 1;
    if ((devices == 100) || (DELAY_B_TWO_DEV == 1))
    {
      dtostrf ((OLD_STATE), 4, 0, OLD_STATE_A);
      client.publish("CUT_ALL_DEVICES", OLD_STATE_A);
    }
    else
    {
      digitalWrite(green , LOW);
      digitalWrite(Orange , LOW);
      digitalWrite(blue , LOW);
      digitalWrite(red , LOW);
      delay(10);
      client.publish("INJ_FOR_START", ONTIME);
    }
    tim = 0;
    flag = 0;
  }
  if (flag == 1)
  {
    tim++;
    if ((Estimate_Time == 1))
    {
      if (devices == 100)
      {
      }
      else
      {
        dtostrf ((tim * 10), 4, 0, arra);
        client.publish("CUT_OFF4", arra);                        //change
      }
      NOT_CNT++;
      dtostrf ((NOT_CNT), 4, 0, arra);
      client.publish("NOT_CNT4", arra);                       //change
      client.publish("NOT_CNT_TOTAL", "1");
      tim = 0;
      Estimate_Time = 0;
      flag = 0;

    }

  }
}

void Config() {
  pinMode(green, OUTPUT);
  pinMode(Orange, OUTPUT);
  pinMode(blue, OUTPUT);
  pinMode(red, OUTPUT);
  pinMode(buzzer, OUTPUT);

  pinMode(IR, INPUT);

  digitalWrite(green, 0);
  digitalWrite(Orange, 0);
  digitalWrite(blue, 0);
  digitalWrite(red, 0);
  digitalWrite(buzzer, 0);

  sens_func.attach(0.01, IRsensor);


  Serial.begin(115200);
  Serial.print("connecting");
  WiFi.begin("ReFit-KTF", "12345677");       //SSID,PASSWORD
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print("./");
  }
  Serial.println();

  reconnect();
  pinMode(LED_BUILTIN, OUTPUT);

}


void callback(char* topic, byte * payload, unsigned int length1) {
  Serial.print("message arrived[");
  Serial.print(topic);
  Serial.println("]");
  



  if (strcmp(topic, "ONLED") == 0)
  {
    flag1 = atoi((const char *)payload);
    if (flag1 == devices)
    {
      CHECK_RAND = devices;
      flag3 = devices;
      flag5 = devices;
      DEVIC_NO = devices;
      Estimate_Time = 0;
      SENT_INJ_FOR_START = 0;
    }
    else
    {
      flag3 = 0;
      flag5 = 0;
      flag1 = 0;
      DEVIC_NO = 0;
    }

    if (Check_Random_Number == 2)
    {
      random_number = random(2);
    }
    else if (Check_Random_Number == 3)
    {
      random_number = random(3);
    }

    else if (Check_Random_Number == 4)
    {
      random_number = random(4);
    }
    else
      random_number = random(7);


    for (int i = 0; i < length1; i++) {
      payload[i] = 0;
    }
  }
  else if (strcmp(topic, "SWITCH") == 0)
  {

    SWITCH [i] = payload[i];
    for (int i = 0; i < length1; i++) {
      payload[i] = 0;
    }
  }
  else if (strcmp(topic, "FIXED_COLOR") == 0)
  {
    COLOR [i] = payload[i];
    for (int i = 0; i < length1; i++) {
      payload[i] = 0;
    }
  }
  else if (strcmp(topic, "MULTIPLE_DEVICES") == 0)
  {
    MULTIPLE_DEVICES = atoi((const char *)payload);
    if (MULTIPLE_DEVICES == devices)
    {
      OLD_STATE = devices;
      devices = 100;                                         //to turn on spcific devices that has device (1)
      if ((payload[2] == 82 || 71 || 66 || 79 || 67 || 76 || 72))
      { COLOR[0] = payload[2];
        if ((payload[1] == 90))
        {
          buzzzer_STATS = 1;
          buzzzer = 1;
        }
        else if (payload[1] == 120)
        {
          buzzzer_STATS = 0;
          buzzzer = 0;
        }
        client.unsubscribe("MULTIPLE_DEVICES");
      }
    }
    for (int i = 0; i < length1; i++) {
      payload[i] = 0;
    }
  }

  else if (strcmp(topic, "SYSTEM_MODE") == 0)
  {
    digitalWrite(green , LOW);
    digitalWrite(Orange , LOW);
    digitalWrite(blue , LOW);
    digitalWrite(red , LOW);
    SYSTEM_START = atoi((const char *)payload);
    if (SYSTEM_START == devices)
    {
      CHECK_RAND = devices;
      flag3 = devices;
      SENT_INJ_FOR_START = 0;
      random_number = random(7);
    }

    else if (payload[1] == 70 )                     //(F) FOR ASCII
      SYSTEM_STOPPED = 1;

    else
    {
      client.publish("NOT_CNT_TOTAL", "2");
      client.publish("GENRAL_COUNT", "2");
      NOT_CNT = 0 ;
      dtostrf ((NOT_CNT), 4, 0, arra);
      client.publish("NOT_CNT4", arra);                  //change
      cnt = 0;
      dtostrf ((cnt), 4, 0, arra);
      client.publish("COUNT4", arra);                    // change
      SYSTEM_STOPPED = 0;
    }

    if (SYSTEM_START == 100)
    {
      client.publish("NOT_CNT_TOTAL", "2");
      client.publish("GENRAL_COUNT", "2");
      NOT_CNT = 0 ;
      dtostrf ((NOT_CNT), 4, 0, arra);
      client.publish("NOT_CNT4", arra);                  //change
      cnt = 0;
      dtostrf ((cnt), 4, 0, arra);
      client.publish("COUNT4", arra);                    // change
      SYSTEM_STOPPED = 0;
    }
    for (int i = 0; i < length1; i++) {
      payload[i] = 0;
    }
  }

  else if (strcmp(topic, "OFFLED") == 0)
  {
    if (payload[1] == 70 )
    {
      flag2 = 3;
    }
    else
      Estimate_Time = 0;

    if ((Check_Random_Number == 2) && (devices == 100))
    {
      randm_with_off = random(2);
    }
    else if ((Check_Random_Number == 3) && (devices == 100))
    {
      randm_with_off = random(3);
    }

    else if ((Check_Random_Number == 4) && (devices == 100))
    {
      randm_with_off = random(4);
    }
    else if ((Check_Random_Number >= 5) && (devices == 100))
      randm_with_off = random(7);

    CHECK_RAND = devices;
    for (int i = 0; i < length1; i++) {
      payload[i] = 0;
    }

  }
  else if (strcmp(topic, "W_FOR_OFF") == 0)
  {
    if (payload[1] == 70 )
    {
      flag4 = 1;
      SENT_INJ_FOR_START = 0;
    }
    if (OLD_STATE >= 2 )
    {
      flag4 = 0;
    }
    CHECK_RAND = devices;
    for (int i = 0; i < length1; i++) {
      payload[i] = 0;
    }

  }

  else if (strcmp(topic, "ON_TIME") == 0)
  {
    for (int i = 0; i < 20; i++) {
      ONTIME [i] = 0;
    }
    for (int i = 0; i < length1; i++) {

      ONTIME [i] = payload[i];
    }
    for (int i = 0; i < length1; i++) {
      payload[i] = 0;
    }

  }
  else if (strcmp(topic, "OFF_TIME") == 0)
  {

    for (int i = 0; i < 20; i++) {
      OFFTIME [i] = 0;
    }
    for (int i = 0; i < length1; i++) {

      OFFTIME [i] = payload[i];
    }
    for (int i = 0; i < length1; i++) {
      payload[i] = 0;
    }

  }
  else if (strcmp(topic, "DEVICES_TX_RX") == 0)
  {
    devices = atoi((const char *)payload);
    client.unsubscribe("DEVICES_TX_RX");
    MIX = devices;
    Confirmation_devices = "DEVICE " + MIX + " CONNECTED";
    Confirmation_devices.toCharArray(arra_Confirmation_devices, 20);
    client.publish("CONNECTION", arra_Confirmation_devices);
    for (int i = 0; i < length1; i++) {
      arra_Confirmation_devices[i] = 0;
    }
    digitalWrite(red , HIGH);
    for (int i = 0; i < length1; i++) {
      payload[i] = 0;
    }
  }
  else if (strcmp(topic, "RESET_DEFINITION") == 0)
  {
    digitalWrite(IR, 1);
    client.publish("DEVICES_COUNTER_FOR_SYSTEM", "2");

    delay(4000);                                                  //must change for each device

    client.subscribe("DEVICES_TX_RX");
    client.subscribe("MULTIPLE_DEVICES");
    client.publish("DEVICES_COUNTER_FOR_SYSTEM", "1");
    client.publish("My_New_Phone", Server_IP);
    OLD_STATE = 0;
    buzzzer = 1;
    buzzzer_STATS = 1;
    for (int i = 0; i < length1; i++) {
      payload[i] = 0;
    }
  }
  else if (strcmp(topic, "BUZZER") == 0)
  {
    buzzzer = atoi((const char *)payload);
    if (buzzzer == 1)
    {
      buzzzer_STATS = 1;
      buzzzer = 1;
    }
    else if (buzzzer == 2)
    {
      buzzzer = 1;
      digitalWrite(buzzer , LOW);
    }
    else if (buzzzer == 3)
    {
      buzzzer = 0;
      buzzzer_STATS = 0;
    }
    for (int i = 0; i < length1; i++) {
      payload[i] = 0;
    }
  }


  else if (strcmp(topic, "DELAY_B_TWO_DEV") == 0)
  {
    DELAY_B_TWO_DEV = atoi((const char *)payload);
    if (DELAY_B_TWO_DEV == 1)
    {
      DELAY_B_TWO_DEV = 1;
    }
    else
      DELAY_B_TWO_DEV = 0;
    for (int i = 0; i < length1; i++) {
      payload[i] = 0;
    }
  }

  else if (strcmp(topic, "Check_Random_Number") == 0)
  {
    Check_Random_Number = atoi((const char *)payload);
    Serial.println (Check_Random_Number);
    for (int i = 0; i < length1; i++) {
      payload[i] = 0;
    }
  }
  //Serial.println();



  else if (strcmp(topic, "server") == 0)
  {
    for (int i = 0; i < 20; i++) {
      Server_IP [i] = 0;
    }
    for (int i = 0; i < length1; i++) {

      Server_IP [i] = payload[i];
    }
    for (int i = 0; i < length1; i++) {
      payload[i] = 0;
    }
    
    client.disconnect();
    mqtt_server = Server_IP;
    client.setServer(Server_IP, 1883);
    client.setCallback(callback);

    reconnect();


  }

}

void reconnect() {
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  while (!client.connected()) {
    if (client.connect(arra_Confirmation_devices)) {
      Serial.println("connected");
      client.publish("DEVICES_COUNTER_FOR_SYSTEM", "1");
      SUBSC();

    }
    else {
      Serial.print("failed,rc=");
      Serial.println(client.state());
      delay(500);
    }
  }
}
void loop() {
  if (!client.connected()) {
    reconnect();
  }

  client.loop();

  if ((SYSTEM_STOPPED == 1))
  {
    client.publish("INJ_FOR_START", "timer 0");
    client.publish("INJ_FOR_WAIT", "timer 0");
    client.publish("INJ_FOR_START", "timer 0");
    client.publish("INJ_FOR_WAIT", "timer 0");
    client.publish("INJ_FOR_START", "timer 0");
    client.publish("INJ_FOR_WAIT", "timer 0");
    SYSTEM_STOPPED = 0;
    CHECK_RAND = 0;
    Estimate_Time = 0;
    digitalWrite(green , LOW);
    digitalWrite(Orange , LOW);
    digitalWrite(blue , LOW);
    digitalWrite(red , LOW);
    digitalWrite(buzzer , LOW);
    flag = 0;
    flag1 = 0;
    flag2 = 0;
    flag3 = 0;
    flag4 = 0;
    flag5 = 0;
    cnt = 0;
    NOT_CNT = 0 ;

  }

  else if ((CHECK_RAND == devices) && (SWITCH[0] == 102) && (devices != 0))   //ascii of (f)
  {
    RANDOM_SYSTEM();
    CHECK_RAND = 0;
  }
  else if ((SWITCH[0] == 116) && (devices != 0))  //ascii of (t)
  {
    FIXED();
  }
}





void SUBSC(void)
{
  client.subscribe("ONLED");
  client.subscribe("OFFLED");
  client.subscribe("W_FOR_OFF");
  client.subscribe("ON_TIME");
  client.subscribe("OFF_TIME");
  client.subscribe("SYSTEM_MODE");
  client.subscribe("FIXED_COLOR");
  client.subscribe("SWITCH");
  client.subscribe("DEVICES_TX_RX");
  client.subscribe("RESET_DEFINITION");
  client.subscribe("MULTIPLE_DEVICES");
  client.subscribe("BUZZER");
  client.subscribe("DELAY_B_TWO_DEV");
  client.subscribe("Check_Random_Number");
  client.subscribe("server");
}

void FIXED (void)
{
  if ((COLOR[0] == 71) && (flag1 == devices))
  {
    client.publish("GREEN", "GREEN");
    flag = 1;
    digitalWrite(green , HIGH);
    if ((buzzzer == 1) && (buzzzer_STATS == 1))
    {
      digitalWrite(buzzer , HIGH);
    }
    flag1 = 0;

  }
  if (((flag2 == 3) && (flag3 == devices)))
  {
    digitalWrite(green , LOW);
    digitalWrite(Orange , LOW);
    digitalWrite(blue , LOW);
    digitalWrite(red , LOW);
    Estimate_Time = 1;
    SYSTEM_START = 0;
    client.publish("INJ_FOR_WAIT", OFFTIME);
    flag2 = 0;
    flag3 = 0;
  }
  if (((flag4 == 1) && (flag5 == devices) && (SENT_INJ_FOR_START == 0)) || ((SYSTEM_START == devices) && (flag3 == devices) && (SENT_INJ_FOR_START == 0)))
  {
    client.publish("INJ_FOR_START", ONTIME);
    SYSTEM_START = 0;
    flag3 = 0;
    flag5 = 0;
    flag4 = 0;


  }


  else if ((COLOR[0] == 79) && (flag1 == devices))
  {
    client.publish("Orange", "Orange");
    flag = 1;
    digitalWrite(Orange , HIGH);
    if ((buzzzer == 1) && (buzzzer_STATS == 1))
    {
      digitalWrite(buzzer , HIGH);
    }
    flag1 = 0;
  }
  else if ((COLOR[0] == 66) && (flag1 == devices))
  {
    client.publish("BLUE", "BLUE");
    flag = 1;
    digitalWrite(blue , HIGH);
    if ((buzzzer == 1) && (buzzzer_STATS == 1))
    {
      digitalWrite(buzzer , HIGH);
    }
    flag1 = 0;
  }

  else if ((COLOR[0] == 82) && (flag1 == devices))
  {
    client.publish("RED", "RED");
    flag = 1;
    digitalWrite(red , HIGH);
    if ((buzzzer == 1) && (buzzzer_STATS == 1))
    {
      digitalWrite(buzzer , HIGH);
    }
    flag1 = 0;
  }
  /*                              HH*/
  else if ((COLOR[0] == 67) && (flag1 == devices))
  {
    client.publish("CYAN", "CYAN");
    flag = 1;
    digitalWrite(green , HIGH);
    digitalWrite(blue , HIGH);
    if ((buzzzer == 1) && (buzzzer_STATS == 1))
    {
      digitalWrite(buzzer , HIGH);
    }
    flag1 = 0;
  }
  else if ((COLOR[0] == 76) && (flag1 == devices))
  {
    client.publish("LIGHTGREEN", "LIGHTGREEN");
    flag = 1;
    digitalWrite(green , HIGH);
    digitalWrite(red , HIGH);
    if ((buzzzer == 1) && (buzzzer_STATS == 1))
    {
      digitalWrite(buzzer , HIGH);
    }
    flag1 = 0;
  }
  else if ((COLOR[0] == 72) && (flag1 == devices))
  {

    client.publish("HOTPINK", "HOTPINK");
    flag = 1;
    digitalWrite(red , HIGH);
    digitalWrite(blue , HIGH);
    if ((buzzzer == 1) && (buzzzer_STATS == 1))
    {
      digitalWrite(buzzer , HIGH);
    }
    flag1 = 0;
  }
  else if ((COLOR[0] == 85) && (flag1 == devices))
  {
    flag = 1;
    digitalWrite(blue , HIGH);
    digitalWrite(Orange , HIGH);
    if ((buzzzer == 1) && (buzzzer_STATS == 1))
    {
      digitalWrite(buzzer , HIGH);
    }
    flag1 = 0;
  }
  else if ((COLOR[0] == 65) && (flag1 == devices))                    //65->(A)
  {
    flag = 1;
    digitalWrite(green , HIGH);
    digitalWrite(blue , HIGH);
    digitalWrite(red , HIGH);
    if ((buzzzer == 1) && (buzzzer_STATS == 1))
    {
      digitalWrite(buzzer , HIGH);
    }
    flag1 = 0;
  }
  flag4 = 0;
  flag2 = 0;


}

void RANDOM_SYSTEM(void)
{
  if (((random_number == 0) && (flag1 == devices) && (flag1 != 100)) || ( (flag1 == 100) && (randm_with_off == 0 ) ))
  {
    client.publish("GREEN", "GREEN");
    flag = 1;
    digitalWrite(green , HIGH);
    if ((buzzzer == 1) && (buzzzer_STATS == 1))
    {
      digitalWrite(buzzer , HIGH);
    }
    flag1 = 0 ;
  }
  if (((flag2 == 3) && (flag3 == devices)))
  {
    digitalWrite(green , LOW);
    digitalWrite(Orange , LOW);
    digitalWrite(blue , LOW);
    digitalWrite(red , LOW);
    Estimate_Time = 1;
    SYSTEM_START = 0;
    client.publish("INJ_FOR_WAIT", OFFTIME);
    flag2 = 0;
    flag3 = 0;
  }
  if (((flag4 == 1) && (devices != 0) && (flag5 == devices) && (SENT_INJ_FOR_START == 0)) || ((devices != 0) && (SYSTEM_START == devices) && (flag3 == devices) && (SENT_INJ_FOR_START == 0)))
  {
    client.publish("INJ_FOR_START", ONTIME);
    SYSTEM_START = 0;
    flag3 = 0;
    flag5 = 0;
    flag4 = 0;
  }
  else if (((random_number == 1) && (flag1 == devices) && (flag1 != 100)) || ( (flag1 == 100) && (randm_with_off == 1 ) ))

  {
    client.publish("BLUE", "BLUE");
    flag = 1;
    digitalWrite(blue , HIGH);
    if ((buzzzer == 1) && (buzzzer_STATS == 1))
    {
      digitalWrite(buzzer , HIGH);
    }
    flag1 = 0 ;
  }


  else if (((random_number == 2) && (flag1 == devices) && (flag1 != 100)) || ( (flag1 == 100) && (randm_with_off == 2 ) ))
  {
    client.publish("RED", "RED");
    flag = 1;
    digitalWrite(red , HIGH);
    if ((buzzzer == 1) && (buzzzer_STATS == 1))
    {
      digitalWrite(buzzer , HIGH);
    }
    flag1 = 0 ;
  }

  else if (((random_number == 3) && (flag1 == devices) && (flag1 != 100)) || ( (flag1 == 100) && (randm_with_off == 3 ) ))

  {
    client.publish("Orange", "Orange");
    flag = 1;
    digitalWrite(Orange , HIGH);
    if ((buzzzer == 1) && (buzzzer_STATS == 1))
    {
      digitalWrite(buzzer , HIGH);
    }
    flag1 = 0 ;
  }

  else if (((random_number == 4) && (flag1 == devices) && (flag1 != 100)) || ( (flag1 == 100) && (randm_with_off == 4 ) ))

  {
    client.publish("CYAN", "CYAN");
    flag = 1;
    digitalWrite(green , HIGH);
    digitalWrite(blue , HIGH);
    if ((buzzzer == 1) && (buzzzer_STATS == 1))
    {
      digitalWrite(buzzer , HIGH);
    }
    flag1 = 0;
  }

  else if (((random_number == 5) && (flag1 == devices) && (flag1 != 100)) || ( (flag1 == 100) && (randm_with_off == 5 ) ))

  {    client.publish("LIGHTGREEN", "LIGHTGREEN");
    flag = 1;
    digitalWrite(green , HIGH);
    digitalWrite(red , HIGH);
    if ((buzzzer == 1) && (buzzzer_STATS == 1))
    {
      digitalWrite(buzzer , HIGH);
    }
    flag1 = 0;
  }
  else if (((random_number == 6) && (flag1 == devices) && (flag1 != 100)) || ( (flag1 == 100) && (randm_with_off == 6 ) ))

  {
    client.publish("HOTPINK", "HOTPINK");
    flag = 1;
    digitalWrite(red , HIGH);
    digitalWrite(blue , HIGH);
    if ((buzzzer == 1) && (buzzzer_STATS == 1))
    {
      digitalWrite(buzzer , HIGH);
    }
    flag1 = 0;
  }

  flag4 = 0;
  flag2 = 0;


}



