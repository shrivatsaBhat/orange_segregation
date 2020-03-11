#include <Servo.h>
#include <LiquidCrystal.h>

//----------------------------------define pin configuration--------------------------------------------------------------------------
const int rs = 12;
const int en = 11;                                          //(pwm pin)
const int d4 = 5;                                           //(pwm pin)
const int d5 = 4; 
const int d6 = 3;                                           //(pwm pin)
const int d7 = 2;
const int servo_1 = 6;                                      //assign servo pin number (pwm pin)                                             
const int servo_2 = 7;                                                                                                            
const int IR1 = 16;                                         //IR sensor 1     (pin 16 = A3)                                                          
const int IR2 = 8;                                          //IR sensor 2     (pin 17 = A4)
const int trigPin = 9;                                      // defines ultrasonic pins numbers (pwm pin)
const int echoPin = 10;                                     //(pwm pin)                       
const int pi_mtr1 = 14;                                     // raspiberry input pin to controll motor 1    (pin 14 = A0)                         
const int pi_mtr2 = 15;                                     // raspiberry input pin to controll motor 2    (pin 15 = A1) 
const int to_raspi = 13                         
//-----------------------------------------------------------------------------------------------------------------------------------
                                                            // initialize the library by associating any needed LCD interface pin
                                                            //with the arduino pin number it is connected to 
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

Servo orange_servo;                                          // create servo object to control a servo
Servo green_servo;

int servo_pos = 0;                                          //set servo position

long duration;                                              // define variable for ultrasonics
int distance;                                               // (distance unit centi meter)

int orangec = 0;                                            // variable to store orange color count
int greenc = 0;                                             // variable to store green color count

void setup(){  
                                                            // put your setup code here, to run once:
  pinMode(pi_mtr1,INPUT);                                   //assign pi output as arduino input
  pinMode(pi_mtr2,INPUT);
  pinMode(to_raspi, OUTPUT);                                //trigger output to pi
  digitalWrite(to_raspi, LOW);
  
  pinMode(IR1,INPUT);                                       // assign IR sensor as input 
  pinMode(IR2,INPUT);

  pinMode(trigPin, OUTPUT);                                 // Sets the trigPin as an Output
  pinMode(echoPin, INPUT);                                  // Sets the echoPin as an Input
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
                                                            //put your main code here, to run repeatedly:
  if(distance < 20){
    digitalWrite(to_raspi, HIGH);                           //send raspi as object is detected 
  }
  
  int servo_mtr1 = digitalRead(pi_mtr1);                    //read from rapibery pi 
  int servo_mtr2 = digitalRead(pi_mtr2);                    // read from raspiberry pi
  Serial.println("motor 1 = " + String(servo_mtr1));
  Serial.println("motor 2 = " + String(servo_mtr2));

  if(servo_mtr1 == 1){
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
  
  if(servo_mtr2 == 1){
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
//-----------------------------------------------------------------------------------------------------------------------------------
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
  lcd.print(value);
}

void ultrasonic_distance(void){
  digitalWrite(trigPin, LOW);                             // Clears the trigPin
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);                            // Sets the trigPin on HIGH state for 10 micro seconds
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);                      // Reads the echoPin, returns the sound wave travel time in microseconds
  distance= duration*0.034/2;                             // Calculating the distance
  Serial.print("Distance: ");                             // Prints the distance on the Serial Monitor
  Serial.println(distance);
}
