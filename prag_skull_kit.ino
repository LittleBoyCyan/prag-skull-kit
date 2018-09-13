/***************************************/
/*       프래그 스튜디오 해골 키트 1.0       */
/*               2018.08               */
/*       developed by 리틀보이사이언       */
/***************************************/

#include <Servo.h>

Servo skullMouse;

/////////////커스텀 설정 부분/////////////

const int countMin = 2; // 카운트 최솟값
const int countMax = 20; // 카운트 최댓값
int servoSpeed = 4; // 서보모터 스피드 (1~8 숫자 적을 수록 빠름)
int servoAngleMin = 0; // 서보모터 최소 각도 (0~180도)
int servoAngleMax = 180; // 서보모터 최대 각도 (0~180도)
int bombNum = 5; // 폭탄 터졌을 때 서보모터 움직이는 횟수

/////////////////////////////////////


const int sensorNum = 3; // 센서 연결한 아두이노 포트
const int buttonNum = 4; // 버튼 연결한 아두이노 포트
const int ledNum = 5; // LED 연결한 아두이노 포트
const int servoNum = 6; // 서모보터 연결한 아두이노 포트

boolean sensor; // 센서 들어오는 값 변수
boolean button; // 버튼 들어오는 값 변수

boolean buttonFlag = 0; // 버튼 상태 판단 변수
boolean sensorFlag = 0; // 센서 상태 판단 변수

boolean bombReady = 0; // 폭탄 터진 상태 판단 변수

int bombCounter = 0; // 미리 랜덤으로 설정한 폭탄 카운트 값
int counter = 0; // 현재 카운트 되고 있는 값


void setup() {
  pinMode(sensorNum, INPUT); // 적외선 센서
  pinMode(buttonNum, INPUT); // 리셋 버튼
  pinMode(ledNum, OUTPUT); // LED 2개

  randomSeed(analogRead(0)); // 랜덤변수 생성자

  //Serial.begin(9600);
}

void loop() {

  sensor = digitalRead(sensorNum); //센서 값을 sensor 변수에 저장
  button = digitalRead(buttonNum); //버튼 값을 button 변수에 저장

  //// 버튼 눌러서 폭탄이 터지는 카운트 숫자를 리셋하는 조건문 ///
  if (button && !buttonFlag) { // 버튼이 눌릴 경우 실행
    resetCount();
    ledTwinkle(3, 130); // ledTwinkle(LED깜박임 횟수, 속도 100~500)
    bombReady = true;
    buttonFlag = true;
  } else if (!button) {
    buttonFlag = false;
  }


  if (bombReady) { // 시작하기 전에 무조건 버튼을 한번 눌러야만 하도록
    //// 센서에 감지되면 카운트 숫자가 올라가는 조건문 ////
    if (!sensor && !sensorFlag) {
      counter++;
      sensorFlag = true;
      digitalWrite(ledNum, HIGH);
    } else if (sensor) {
      sensorFlag = false;
      digitalWrite(ledNum, LOW);
    }

    //// 센서 감지에 의해 카운트 된 숫자와 폭탄 카운트 숫자가 일치하면 폭탄을 터트리는 조건문 ////
    if (counter != 0 && counter == bombCounter) {
      bomb(servoAngleMax, servoSpeed, bombNum); // bomb(서보 움직임 각도, 스피드, 횟수)
      //ledTwinkle(6, 300); // ledTwinkle(LED깜박임 횟수, 속도 100~500)
      bombReady = false;
    } else if (sensorFlag) {
      delay(500); // 센서 감지된 직후 지연 시간
    }

  }

/*
  Serial.print("counter:");
  Serial.print(counter);
  Serial.print("   ");
  Serial.print("sensorFlag:");
  Serial.print(sensorFlag);
  Serial.print("   ");
  Serial.print("bombCounter: ");
  Serial.print(bombCounter);
  Serial.println();
*/
}


//// 폭탄 터트리기 함수 - 서보 모터 움직이기 (스피드, 횟수) ////
void bomb(int angle, int motorSpeed, int num) {
  skullMouse.attach(servoNum);
  for (int j = 0; j < num; j++) {
    for (int i = servoAngleMin; i < angle; i++) {
      skullMouse.write(i);
      delay(motorSpeed);
    }
    delay(300);
    for (int i = angle; i > servoAngleMin; i--) {
      skullMouse.write(i);
      delay(motorSpeed);
    }
    delay(300);
  }
  skullMouse.detach();
}


void resetCount() { // 카운트를 리셋하는 함수
  counter = 0;
  bombCounter = random(countMin, countMax);
}


void ledTwinkle(int num, int speedTwinkle) { // LED 깜박이는 함수
  for (int i = 0; i < num; i++) {
    digitalWrite(ledNum, HIGH);
    delay(speedTwinkle);
    digitalWrite(ledNum, LOW);
    delay(speedTwinkle);
  }
}



