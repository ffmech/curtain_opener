#include <IRremote.h>

const int RECV_PIN = 7;
const int motorPin1 = 9;  // IN1 pin of L298N connected to pin 9
const int motorPin2 = 8;  // IN2 pin of L298N connected to pin 8
int nofd_tm = 0;
IRrecv irrecv(RECV_PIN);
decode_results results;
unsigned long key_value = 0;
unsigned long crnt_time = 0;
unsigned long open_time = 0;
unsigned long close_time = 0;
unsigned long time_delta = 0;
bool time_set = false;
bool open_set = false;
bool close_set = false;
bool calibrated = false;
bool counted = false;
unsigned long counter = 0;
unsigned long at_start = 0;
char time_display[5] = "0000";
int count_dig = 0;

void setup(){
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  Serial.begin(9600);
  irrecv.enableIRIn();
  irrecv.blink13(true);

  if (not time_set) {
    Serial.println("Enter Current time: ");
  }

}

void loop(){

  if (not time_set) {
    set_time(time_set, crnt_time, "Enter opening time: ");
    if (time_set) {
      time_delta = millis()
    }
  } else if (not open_set) {
    set_time(open_set, open_time, "Enter closing time: ");
  } else if (not close_set) {
    set_time(close_set, close_time, "Open curtains desired position using arrows on the remote, then press OK");
  } else if (not calibrated) {
    calibrate()
  } else if (not counted)

}

void set_time(bool which_time, unsigned long which_set, char mesg[]){
  int chosen_num = constant_reading;
  if (isdigit(chosen_num)){
    if (count_dig==2){
      Serial.print(":")
    }
    time_display[count_dig] = chosen_num;
    Serial.print("chosen_num")
    count_dig += 1;
  }

  if (count_dig == 5){
    get_time(which_set);
    which_time = true;
    Serial.println(mesg);
    count_dig = 0;
  }

}

void motor_cw(){
  digitalWrite(motorPin1, HIGH);  // Set IN1 high
  digitalWrite(motorPin2, LOW);   // Set IN2 low
}

void motor_ccw(){
  digitalWrite(motorPin1, LOW);  // Set IN1 low
  digitalWrite(motorPin2, HIGH);   // Set IN2 high
}

void motor_stop(){
  digitalWrite(motorPin1, LOW);   // Set IN1 low
  digitalWrite(motorPin2, LOW);   // Set IN2 low
}

void constant_reading(char resu){
  //resu = decode_ir_sig();
  if (resu != "F"){
    nofd_tm = 0;

    switch(resu){
      case "R":
        //Serial.println("open");
        motor_cw();
        break ;
      case "L":
        //Serial.println("close");
        motor_ccw();
        break ;
      default:
        //Serial.println("Stopped");
        motor_stop();
        break; 
    }
  }
  else if (nofd_tm >= 20000) {
    nofd_tm = 0;
    //Serial.println("Stopped");
    motor_stop();
  }
  else {
    nofd_tm += 1;
  } 
}

void convert_to_time(){
  unsigned long currentMillis = millis();
  unsigned long seconds = currentMillis / 1000;
  unsigned long minutes = seconds / 60;
  unsigned long hours = minutes / 60;
  unsigned long days = hours / 24;
  currentMillis %= 1000;
  seconds %= 60;
  minutes %= 60;
  hours %= 24;
}

unsigned long get_time(unsigned long this_set){
  this_set = time_display[0]*60*60*10;
  this_set += time_display[1]*60*60;
  this_set += time_display[2]*60*10;
  this_set += time_display[3]*60;
  return this_set;
}

void calibrate() {
  //move and if ok pressed stop and then count
  char rosolts = decode_ir_sig();
  if (rosolts == "K") {
    calibrated = true;
    nofd_tm = 0;
    Serial.println("Close to the desired position and press OK");
  } else {
    constant_reading(rosolts);
  }
}

void count_secs() {
  char rosolts = decode_ir_sig();
  if (rosolts == "K" and counter > 0) {
    counted = true;
  }
}

void sec_difference(char side) {
  if (not counted) {
    return 0;
  }

  counter < millis - at_start 

  if (side == "L" and counter < 0.1)
}

char decode_ir_sig(){
  if (irrecv.decode(&results)){
    nofd_tm = 0;
    char butn = "N";
    //Serial.println(results.value, HEX);

    if (results.value == 0XFFFFFFFF){
      results.value = key_value;
    }

    switch(results.value){
      case 0xFF10EF:
        butn = "R";
        break ;
      case 0xFF5AA5:
        butn =  "L";
        break ;
      default:
        butn = "N"
        break; 
    }
    key_value = results.value;

    //digitalWrite(motorPin1, LOW);   // Set IN1 low
    //digitalWrite(motorPin2, LOW);   // Set IN2 low

    irrecv.resume();
    return butn;
  } else {
    return "F";
  }
}

// right FF10EF
// left FF5AA5