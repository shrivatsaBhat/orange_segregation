#include <Servo.h>
#include <LiquidCrystal.h>

//----------------------------------------------------------------------------------------------------------------------------------
const int servo_1 = 6;                                      //assign servo pin number                                               
const int servo_2 = 7;                                                                                                            
const int IR1 = 13;                                         //IR sensor 1                                                           
const int IR2 = 8;                                          //IR sensor 2                                                           
const int pi_mtr1 = 10;                                     // raspiberry input pin to controll motor 1                             
const int pi_mtr2 =  9;                                     // raspiberry input pin to controll motor 2                             
//-----------------------------------------------------------------------------------------------------------------------------------
                                                            // initialize the library by associating any needed LCD interface pin
                                                            //with the arduino pin number it is connected to 
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

Servo orange_servo;                                          // create servo object to control a servo
Servo green_servo;

int servo_pos = 0;                                          //set servo position

int orangec = 0;                                            // variable to store orange color count
int greenc = 0;                                             // variable to store green color count

void setup(){  
                                                            // put your setup code here, to run once:
  pinMode(pi_mtr1,INPUT);                                   //assign pi output as arduino input
  pinMode(pi_mtr2,INPUT);
  
  pinMode(IR1,INPUT);                                       // assign IR sensor as input 
  pinMode(IR2,INPUT);
                                                            //initial setup for 16X2 LCD display
  lcd.begin(16, 2);                                
  lcd_print_orange(orangec);                                //set LCD ORANGE value
  lcd_print_green(greenc);                                  //set LCD GREEN value
                                                            // initial setup for servo
  orange_servo.attach(servo_1);
  green_servo.attach(servo_2); 
                                                            //set the serial communication baud rate 
  Serial.begin(115200);
}

void loop() {
                                                            // put your main code here, to run repeatedly:
  int mtr1 = digitalRead(pi_mtr1);                          //read from rapibery pi 
  int mtr2 = digitalRead(pi_mtr2);                          // read from raspiberry pi
  Serial.println("motor 1 = " + String(mtr1));
  Serial.println("motor 2 = " + String(mtr2));

  if(mtr1 == 1){
    orangec = orangec + 1;                                  // increament count by one for orange 
    Serial.println("-->ORANGE COLOUR DETECTED \n COUNT = " + String(orangec));
    lcd_print_orange(orangec);
   
    if(IR1 == 0){
      Serial.println("IR DETECTED = 0");
      orange_servo_rotate_180to0();
      orange_servo_rotate_0to180();
    }
    else {
      Serial.println("IR DETECTED = 1");
    }
  }
  
  if(mtr2 == 1){
    greenc = greenc + 1;
    Serial.println("-->GREEN COLOUR DETECTED \n COUNT = " + String(greenc));
    lcd_print_green(greenc);

    if(IR2 == 0){
      Serial.println("IR DETECTED = 0");
      green_servo_rotate_180to0();                                   
      green_servo_rotate_0to180();
    }
    else {
      Serial.println("IR DETECTED = 1");
    }
  }

  delay(1000);                                              // delay of 1 second
}
//---------------------------------------------------------------------------------------
void orange_servo_rotate_180to0(void){
  for (servo_pos = 160; servo_pos >= 45; servo_pos -= 1) {  // goes from 180 degrees to 0 degrees
    orange_servo.write(servo_pos);                          // tell servo to go to position in variable 'servo_pos'
    delay(15);                                              // waits 15ms for the servo to reach the position
  }   
}
void orange_servo_rotate_0to180(void){
  for (servo_pos = 45; servo_pos <= 160; servo_pos += 1) {  // goes from 0 degrees to 180 degrees in steps of 1 degree
    orange_servo.write(servo_pos);                          // tell servo to go to position in variable 'servo_pos'
    delay(15);    
    }
}
void green_servo_rotate_180to0(void){
  for (servo_pos = 160; servo_pos >= 45; servo_pos -= 1) { // goes from 180 degrees to 0 degrees
    green_servo.write(servo_pos);                          // tell servo to go to position in variable 'servo_pos'
    delay(15);                                             // waits 15ms for the servo to reach the position
  }   
}
void green_servo_rotate_0to180(void){
  for (servo_pos = 45; servo_pos <= 160; servo_pos += 1) { // goes from 0 degrees to 180 degrees in steps of 1 degree
    green_servo.write(servo_pos);                          // tell servo to go to position in variable 'servo_pos'
    delay(15);                                             // waits 15ms for the servo to reach the 
    }
}
void lcd_print_orange(int value){
  lcd.setCursor(0,0);                                     //sets the cursor at row 0 column 0
  lcd.print("ORANGE-");                                   //write on LCD display
  lcd.print(value);
}
void lcd_print_green(int value){
  lcd.setCursor(2,1);                                     //sets the cursor at row 2 column 1
  lcd.print("GREEN-");                                    //write on LCD display
  lcd.print(greenc);
}
