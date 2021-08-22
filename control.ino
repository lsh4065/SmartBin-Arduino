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
unsigned long z_start = 0, z_end = 0, z_during = 0, z_back = 0;
boolean setdir, x, y, state = 0, x_end, y_end, open_end, open_x, open_y;
char going_pos_x = 0, going_pos_y = 0;
char present_pos_x = '2', present_pos_y = 'b';
char insert_pos = 0, z = '1';
int pin_x = 0, pin_y = 0;
int cds_x = 500, cds_y = 500;
char serial = 0;
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
  //idle, working 시리얼 통신 
  if(state == 0)
  {
    Serial.write('i');
  }
  if(state == 1)
  {
    Serial.write('w');
  }
  
  if(Serial.available())
  {
    serial = Serial.read();
  }
  if(serial == 's' && magnetic_sensor == 1) //문 잠길때
  {
    state = !state;
    digitalWrite(door, HIGH);
    serial = 'n';

    z_start = millis();
    digitalWrite(3, HIGH);
    for(cds_x; cds_x > 400; cds_x = analogRead(A0))//움직판 아래로 이동
    {
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
    delay(500);
    z = '0';
  }
  if('1' <= serial && serial <= '6')//x좌표 레이저 켜고 모터 이동
  {
    going_pos_x = serial;
    pin_x = int(going_pos_x) - 47;
    digitalWrite(pin_x,HIGH);
    if(going_pos_x > present_pos_x) x = !x;
    digitalWrite(DIR_1, x);
    analogWrite(PWM_1, 100);
  }
  if('a' <= serial && serial <= 'c')//y좌표 레이저 켜고 모터 이동
  {
    going_pos_y = serial;
    pin_y = int(going_pos_y) - 90;
    digitalWrite(pin_y, HIGH);
    if(going_pos_y > present_pos_y) y = !y;
    digitalWrite(DIR_2, y);
    analogWrite(PWM_2, 100);
  }
  cds_x = analogRead(A0);
  cds_y = analogRead(A1);
  if(cds_x < 100) // x 레이저 닿을 때 정지
  {
    digitalWrite(pin_x, LOW);
    analogWrite(PWM_1, 0);
    present_pos_x = going_pos_x;
    open_x = !open_x;
  }
  if(cds_y < 100) // y 레이저 닿을 때 정지
  {
    digitalWrite(pin_y, LOW);
    analogWrite(PWM_2, 0);
    present_pos_y = going_pos_y;
    open_y = !open_y;
  }
  if(open_x = 1 && open_y == 1)//움직판 투척문 열고 닫기
  {
    delay(500);
    digitalWrite(solenoid, LOW);
    analogWrite(openPWM_1, 60);
    analogWrite(openPWM_2, 60);
    digitalWrite(openDIR_1, LOW);
    digitalWrite(openDIR_2, HIGH);
    delay(1500);
    analogWrite(openPWM_1, 0);
    analogWrite(openPWM_2, 0);
    delay(1000);
    analogWrite(openPWM_1, 60);
    analogWrite(openPWM_2, 60);
    digitalWrite(openDIR_1, HIGH);
    digitalWrite(openDIR_2, LOW);
    delay(1500);
    analogWrite(openPWM_1, 0);
    analogWrite(openPWM_2, 0);
    digitalWrite(solenoid, HIGH);
    delay(500);
    open_end = !open_end;
  }
  if(open_end == 1)//원래 위치 복귀
  {
    x = !x;
    y = !y;
    digitalWrite(3, HIGH);
    digitalWrite(8, HIGH);
    going_pos_x = '2';
    going_pos_y = 'b';
    analogWrite(PWM_1, 100);
    analogWrite(PWM_2, 100);
  }
  if(going_pos_x == '2' && present_pos_y == 'b' && present_pos_x == '2' && z == '0')
  {
    delay(500);
    for(z_back = millis(); millis() - z_back < z_during;)
    {
      digitalWrite(driverDIR_1, setdir);
      digitalWrite(driverDIR_2, !setdir);
      digitalWrite(driverPUL, HIGH);
      delayMicroseconds(100);
      digitalWrite(driverPUL, LOW);
      delayMicroseconds(100);
    }
    z = '1';
    setdir = !setdir;
    digitalWrite(door, LOW);
    state = !state;
  }
}
