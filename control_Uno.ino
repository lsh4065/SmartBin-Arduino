#define DIR_1 10
#define PWM_1 11
#define DIR_2 52
#define PWM_2 53
#define openDIR_1 40
#define openPWM_1 45
#define openDIR_2 41
#define openPWM_2 46
#define driverPUL 22
#define driverDIR_1 23
#define driverDIR_2 24
#define magnetic_sensor 20
#define solenoid 21
#define door 50
unsigned long x_start = 0, x_end = 0, x_during = 0, x_back = 0;
unsigned long y_start = 0, y_end = 0, y_during = 0, y_back = 0;
unsigned long z_start = 0, z_end = 0, z_during = 0, z_go_up = 0;
boolean setdir, x, y;
char going_pos_x = 0, going_pos_y = 0;
char present_pos_x = '2', present_pos_y = 'b';
char insert_pos = 0;
int z = 1;
int pin_x = 0, pin_y = 0;
int cds_x = 500, cds_y = 500;
void setup() {
    Serial.begin(9600);
    pinMode(2, OUTPUT);
    pinMode(3, OUTPUT);
    pinMode(4, OUTPUT);
    pinMode(5, OUTPUT);
    pinMode(6, OUTPUT);
    pinMode(7, OUTPUT);
    pinMode(8, OUTPUT);
    pinMode(9, OUTPUT);
    pinMode(DIR_1, OUTPUT);
    pinMode(PWM_1, OUTPUT);
    pinMode(DIR_2, OUTPUT);
    pinMode(PWM_2, OUTPUT);
    pinMode(openDIR_1, OUTPUT);
    pinMode(openPWM_1, OUTPUT);
    pinMode(openDIR_2, OUTPUT);
    pinMode(openPWM_2, OUTPUT);
    pinMode(driverPUL, OUTPUT);
    pinMode(driverDIR_1, OUTPUT);
    pinMode(driverDIR_2, OUTPUT);
    pinMode(magnetic_sensor, INPUT);
    pinMode(solenoid, OUTPUT);
    pinMode(door, OUTPUT);
    digitalWrite(solenoid, HIGH);
    digitalWrite(door, LOW);
}

void loop() {
    if (Serial.read() == 0 && magnetic_sensor ==  1){
      digitalWrite(door, HIGH);
    }
    if (Serial.available()) {
        if (z == 1) {
            z_start = millis();
            digitalWrite(3, HIGH);
            for (cds_x = 500; cds_x > 400; cds_x = analogRead(A0)) {
                digitalWrite(driverDIR_1, setdir);
                digitalWrite(driverDIR_2, !setdir);
                digitalWrite(driverPUL, HIGH);
                delayMicroseconds(100);
                digitalWrite(driverPUL, LOW);
                delayMicroseconds(100);
            }
            z_end = millis();
            digitalWrite(3, LOW);
            z_during = z_end - z_start;
            setdir = !setdir;
            z = 0;
            delay(1000);
        }
        insert_pos = Serial.read();
        if (48 < insert_pos && insert_pos < 54) {
            going_pos_x = insert_pos;
            analogWrite(PWM_1, 100);
            pin_x = int(going_pos_x) - 47;
            digitalWrite(pin_x, HIGH);
            if (going_pos_x > present_pos_x) x = !x;
            digitalWrite(DIR_1, x);
            x_start = millis();
        }
        if (96 < insert_pos && insert_pos < 100) {
            going_pos_y = insert_pos;
            analogWrite(PWM_2, 100);
            pin_y = int(going_pos_y) - 90;
            digitalWrite(pin_y, HIGH);
            if (going_pos_y > present_pos_y) y = !y;
            digitalWrite(DIR_2, y);
            y_start = millis();
        }
    }
    cds_x = analogRead(A0);
    cds_y = analogRead(A1);
    if (cds_x < 100) {
      digitalWrite(pin_x, LOW);
      analogWrite(PWM_1, 0);
      present_pos_x = going_pos_x;
      x_end = millis();
    }
    if (cds_y < 100) {
      digitalWrite(pin_y, LOW);
      analogWrite(PWM_2, 0);
      present_pos_y = going_pos_y;
      y_end = millis();
    }
    if (present_pos_x == going_pos_x && present_pos_y == going_pos_y && present_pos_x != '2' ){
      delay(1000);
      digitalWrite(solenoid, LOW);
      analogWrite(openPWM_1, 30);
      analogWrite(openPWM_2, 30);
      digitalWrite(openDIR_1, LOW);
      digitalWrite(openDIR_2, HIGH);
      delay(3000);
      analogWrite(openPWM_1, 0);
      analogWrite(openPWM_2, 0);
      delay(1000);
      analogWrite(openPWM_1, 30);
      analogWrite(openPWM_2, 30);
      digitalWrite(openDIR_1, HIGH);
      digitalWrite(openDIR_2, LOW);
      delay(3000);
      analogWrite(openPWM_1, 0);
      analogWrite(openPWM_2, 0);
      digitalWrite(solenoid, HIGH);
      delay(1000);
      x = !x;
      y = !y;
      analogWrite(PWM_1, 100);
      analogWrite(PWM_1, 100);
      digitalWrite(DIR_1, x);
      digitalWrite(DIR_2, y);
      x_during = x_end - x_start;
      y_during = y_end - y_start;
      for(x_back = millis(); millis() - x_back < x_during;){
        
      }
      analogWrite(PWM_1, 0);
      delay(500);
      for(y_back = millis(); millis() - y_back < y_during;){
        
      }
      analogWrite(PWM_1, 0);
      delay(500);
      for(z_go_up = millis(); millis() - z_go_up < z_during;){
         digitalWrite(driverDIR_1, setdir);
         digitalWrite(driverDIR_2, !setdir);
         digitalWrite(driverPUL, HIGH);
         delayMicroseconds(100);
         digitalWrite(driverPUL, LOW);
         delayMicroseconds(100);
      }
      z = 1;
      setdir = !setdir;
      digitalWrite(door, LOW);
    }
}
