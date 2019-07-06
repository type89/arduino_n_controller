#include <L298N.h>
#include <TimerOne.h>
#include <Wire.h>
#include <MultiFuncShield.h>

//pin definition
#define ENA 44
#define IN1 40
#define IN2 42

int base_duty;

//create a motor instance
L298N motor(ENA, IN1, IN2);

void setup() {
  Timer1.initialize();  
  MFS.initialize(&Timer1); // initialize multi-function shield library
  MFS.beep(5,5,1,1,10);
  char title[]="   N-Controller    ";
  char t[5];
   for (int i=0; i <= 19; i++){
      strncpy( t, title + i, 4 ); //titleの先頭+iの位置から4文字をtにコピー
      t[5] = '\0';            //取り出した文字数分の最後に'\0'を入れる
      MFS.write(t);
      delay(200);
   }
  Serial.begin(9600);
  MFS.beep(5,5,1,1,10);
  MFS.write("STOP");
  base_duty = 0;
}

void loop(){
  
  if(Serial.available() > 0){  
    String str = Serial.readStringUntil(';');
  
    if(str.substring(0,1) == "F"){
      String arg1 = str.substring(1,4);
      int duty = arg1.toInt(); 
      String direction ="F";
      if(duty > base_duty){
        base_duty = notch_up(direction, duty, base_duty);
      }else
        base_duty = notch_down(direction, duty, base_duty);
    }

    if(str.substring(0,1) == "B"){
      String arg1 = str.substring(1,4);
      int duty = arg1.toInt();
      String direction ="B";
      if(duty < base_duty){
        base_duty = notch_down(direction, duty, base_duty);  
      }else
        base_duty = notch_up(direction, duty, base_duty);
    }
   
    if(str.substring(0,1) == "S"){
      Serial.println("STOP");
      base_duty = notch_stop();
      MFS.beep();
      MFS.write("STOP");
    }
  }
}

int notch_up(String direction, int duty, int base_duty){
   //Serial.println("Up to duty"); 
   //Serial.println(duty);
   //Serial.println("-----");  
   for (base_duty; base_duty <= duty; base_duty++){
        motor.setSpeed(base_duty);

        String temp = direction + String(base_duty);
        if(direction == "F"){
          motor.forward(); 
        }else{
          motor.backward();
        }
        //Serial.println(temp);    
        char buf[5];
        temp.toCharArray(buf, 5);
        MFS.write(buf);
        delay(100);
   }
    Serial.println("base_duty"); 
    Serial.println(base_duty); 
    return base_duty;
}

int notch_down(String direction, int duty, int base_duty){
   //Serial.println("Down to duty"); 
   //Serial.println(duty);
   //Serial.println("-----"); 
   for (base_duty; base_duty >= duty; base_duty--){
        motor.setSpeed(base_duty);
        String temp = direction + String(base_duty);
        if(direction == "F"){
          motor.forward(); 
        }else{
          motor.backward();
        }
        //Serial.println(temp);    
        char buf[5];
        temp.toCharArray(buf, 5);
        MFS.write(buf);
        delay(100);
   }
    Serial.println("base_duty"); 
    Serial.println(base_duty); 
    return base_duty;
}
int notch_stop(){
  motor.stop();
  base_duty = 0;
  Serial.println("base_duty"); 
  Serial.println(base_duty); 
  return base_duty;
}