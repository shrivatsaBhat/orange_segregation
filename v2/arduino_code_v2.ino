//---------------------------------------Include library--------------------------------------
#include <Servo.h>
#include <CircularBuffer.h>

//----------------------------------Define pin configuration----------------------------------
/*(pin number need to change during connection)*/
#define IR1 16
#define IR2 8
#define trigPin 9   //(pwm pin) 
#define echoPin 10    //(pwm pin) 
#define servoPin_1 6                                               
#define servoPin_2 7
#define to_raspi 13
#define pi_mtr1 14
#define pi_mtr2 15
#define relayPin 2

//--------------------------------------Define variables--------------------------------------
int servo_pos = 0;
long duration = 0;
int distance = 0;

//-----------------------------------Library initailization-----------------------------------
Servo servo_1;  
Servo servo_2;
CircularBuffer<int, 10> orange_buffer;   //orange_buffer capacity 10 integer sample
CircularBuffer<int, 10> green_buffer;   //green_buffer capacity 10 integer sample

//-----------------------------------Setup code, to run once----------------------------------
void setup(){
  Serial.begin(115200);
  pinMode(IR1,INPUT);
  pinMode(IR2,INPUT);
  pinMode(relayPin,OUTPUT);
  pinMode(trigPin,OUTPUT);
  pinMode(echoPin,INPUT);
  pinMode(pi_mtr1,INPUT);                                   
  pinMode(pi_mtr2,INPUT);
  pinMode(to_raspi, OUTPUT);                             
  digitalWrite(to_raspi, LOW);
  servo_1.attach(servoPin_1);
  servo_2.attach(servoPin_2); 
}

//--------------------------------Main code, to run repeatedly--------------------------------
void loop(){
  if(distance < 20){    //check whether fruit is near to camera
    relayState("ON");    //relay on
    digitalWrite(to_raspi, HIGH);   //send signal to raspi
    println("Signal sent to raspberry pi...");
    if((pi_mtr1 == HIGH)&&(pi_mtr2 == LOW)){    //orange colour detected 
      orange_buffer.unshift(1);   //push 1 to buffer
      green_buffer.unshift(0);    //push 0 to buffer
      digitalWrite(to_raspi,LOW);   //stop sending signal to raspi
      relayState("OFF");    //relay off
      delay(100);
      println("detected ORANGE");
    }
    else if((pi_mtr1 == LOW)&&(pi_mtr2 == HIGH)){   //green colour detected
      orange_buffer.unshift(0);   //push 0 to buffer
      green_buffer.unshift(1);    //push 1 to buffer
      digitalWrite(to_raspi,LOW);   //stop sending signal to raspi
      relayState("OFF");    //relay off
      delay(100);
      println("detected GREEN");
    }
    else{
      digitalWrite(to_raspi,LOW);   //stop sending signal to raspi
      digitalWrite(relayPin,LOW);    //relay off
      orange_buffer.unshift(0);   //push 0 to buffer
      green_buffer.unshift(0);    //push 0 to buffer
      delay(100);
    }
  }

  if(digitalRead(IR1)== HIGH){
    if(!orange_buffer.isEmpty()){   //check buffer is not empty
      if(orange_buffer.pop()){   //return last value
        relayState("ON");
        servo_rotate_0to180("servo_1");
      }
    }
    delay(100);
    servo_rotate_180to0("servo_1");   //servo back to position
    relayState("OFF");
    println("ORANGE colour orange is segrigated...");
  }
  if(digitalRead(IR2)== HIGH){
    if(!green_buffer.isEmpty()){   //check buffer is not empty
      if(green_buffer.pop()){   //return last value
        relayState("ON");
        servo_rotate_0to180("servo_2");
      }
    }
    delay(100);
    servo_rotate_180to0("servo_2");
    relayState("OFF");
    println("GREEN colour orange is segrigated...");
  }
}

//-------------------------------------Function defination------------------------------------
void print(String text){
  Serial.print(text);
}
void println(String text){
  Serial.println(text);
}
void ultrasonic_distance(void){
  digitalWrite(trigPin, LOW);   // Clears the trigPin
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);    // Sets the trigPin on HIGH state for 10 micro seconds
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);    // Reads the echoPin, returns the sound wave travel time in microseconds
  distance= duration*0.034/2;   // Calculating the distance
  Serial.print("Distance: ");     // Prints the distance on the Serial Monitor
  Serial.println(distance);
}
void servo_rotate_0to180(String servo){
  if(servo == "servo_1"){
    for (servo_pos = 45; servo_pos <= 160; servo_pos += 1){  // goes from 0 degrees to 180 degrees in steps of 1 degree
      servo_1.write(servo_pos);   // tell servo to go to position in variable 'servo_pos'
      delay(15);    
    }
  }
  if(servo == "servo_2"){
    for(servo_pos = 45; servo_pos <= 160; servo_pos +=1){
      servo_2.write(servo_pos);
      delay(15);
    }
  }
}
void servo_rotate_180to0(String servo){
  if(servo == "servo_1"){
    for (servo_pos = 160; servo_pos >= 45; servo_pos -= 1) {  //goes from 180 degrees to 0 degrees
      servo_1.write(servo_pos);   //tell servo to go to position in variable 'servo_pos'
      delay(15);    //waits 15ms for the servo to reach the position
    }   
  }
  if(servo == "servo_2"){
    for (servo_pos = 160; servo_pos >= 45; servo_pos -= 1) {
      servo_2.write(servo_pos);     
      delay(15);       
    }   
  }
}
void relayState(String state){
  if(state == "ON"){
    digitalWrite(relayPin, HIGH);
  }
  if(state == "OFF"){
    digitalWrite(relayPin, LOW);
  }
}
