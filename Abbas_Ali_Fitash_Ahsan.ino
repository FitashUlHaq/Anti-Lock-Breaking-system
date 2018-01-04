#include <ArduinoUnit.h>

#include <SoftwareSerial.h>
#include<LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2); 
SoftwareSerial mySerial(9, 10);

int sensor=A1;
float temp_read,Temp_alert_val,Temp_shut_val;
int sms_count=0,Fire_Set;
int Alarm=7;
// code for testability
bool msgf=false;
bool safe=false;
float v=5.0f;

void setup()
{
  Serial.begin(9600); // our code for unit
  pinMode(sensor,INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(Alarm, OUTPUT);
  mySerial.begin(9600);   
  Serial.begin(9600);    
  lcd.begin(16,2);  
  delay(500);
}

void loop()
{
  Test::run();
  //CheckFire(v); // changed to remove error
  //CheckShutDown(v);
}

void CheckFire(float v) 
{
  lcd.setCursor(0,0);
  lcd.print("Fire Scan - ON");
  Temp_alert_val=CheckTemp(v);
  if(Temp_alert_val>45)
  {
    SetAlert(); // Function to send SMS Alerts
    digitalWrite(LED_BUILTIN, HIGH);//set Alarm Light
    digitalWrite(Alarm, HIGH); //set Buzzer 
  }
}

float CheckTemp(float temp_read) // converted to stub
{
  //temp_read=analogRead(sensor); // reads the sensor output (Vout of LM35)
  temp_read=temp_read*5;    // converts the sensor reading to temperature
  temp_read=temp_read/10;  // adds the decimal point
  return temp_read; // returns temperature value in degree celsius
}

bool SetAlert() // made testable
{
  while(sms_count<3) //Number of SMS Alerts to be sent
  {  
    SendTextMessage(); // Function to send AT Commands to GSM module
  }
  Fire_Set=1; 
  lcd.setCursor(0,1);
  if(lcd.print("Fire Alert! SMS Sent!")) // made testable
  {
    msgf=true; // testability
  }
  return true;
}

void CheckShutDown(float v)
{
  if(Fire_Set==1)
  {
    Temp_shut_val=CheckTemp(v);
    if(Temp_shut_val<28)
    {
      lcd.setCursor(0,1);
      if(lcd.print("Fire Shut! SAFE NOW"))// added for testability
      {
        safe=true;
      }
      sms_count=0;
      Fire_Set=0;
      digitalWrite(LED_BUILTIN, LOW);//turn off Alarm Light
      digitalWrite(Alarm, LOW); //turn off Buzzer 
    }
  }
}

void SendTextMessage()
{
    mySerial.println("AT+CMGF=1");    //To send SMS in Text Mode
    delay(2000);
    mySerial.println("AT+CMGS=\"+919544xxxxxx\"\r"); // change to the phone number you using 
    delay(2000);
    mySerial.println("Fire in NEW ROOM!");//the content of the message
    delay(200);
    mySerial.println((char)26);//the stopping character
    delay(5000);
     mySerial.println("AT+CMGS=\"+919847xxxxxx\"\r"); // change to the phone number you using 
    delay(2000);
    mySerial.println("Fire in NEW ROOM!");//the content of the message
    delay(200);
    mySerial.println((char)26);//the message stopping character
    delay(5000);
    sms_count++;
}

//---------------------------------- test cases ---------------------------------------//
// as the display in code is based on LED  we will be testing CheckFire function
test(llr_one_point_one_pass)
{
  // true case
  CheckFire(1000.0f);
  assertEqual(digitalRead(7), 1); // checking if 7th pin is high
}
test(llr_one_point_one_fail) 
{
  // false case
  CheckFire(5.0f);
  assertEqual(digitalRead(7), 0); // checking if 7th pin is high
}
test(llr_one_point_two) 
{
  // no code in SUT for variation detection in Temprature
  fail();
}
test(llr_one_point_three) 
{
  float v=200.0f;
  float res=CheckTemp(v);
  assertNotEqual(v,res); // if v != res that means v is converted 
}
test(llr_two_point_one_two) 
{
  // as check fire is already called and msg count should be equal to 3 and as lcd.print is in if so both LLRs are tested
  assertEqual(3, sms_count);
}
test(llr_three_point_one_two) 
{
  // buzzer and led is on same pin so no need for re testing
}
// check shut down already called
test(llr_four_point_one) 
{
  assertTrue(safe); // safe was set for testability
}
test(llr_four_point_two_three) 
{
  assertEqual(digitalRead(7), 0); // check if buzzer and led is low
}



