/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com
*********/

// Load Wi-Fi library
#include <Arduino.h>
#include <Wire.h>
#include <string>

#define USE_MULTCORE 1

// hw_timer_t *timer = NULL;

// Assign output variables to GPIO pins
const int outputled1 = 2;

const int output_motor_a1 = 33;
const int output_motor_a2 = 25;
const int output_motor_b1 = 26;
const int output_motor_b2 = 27;

int step_con = 1;

int motor_mode = 0;  // 0 -> stop 1 -> right 2 -> left
int motor_speed = 0; // 0 -> slow 1 -> mid 2 -> fast
int delay_time = 550; // 550为最快速
int speed_mid1 = 0;
int speed_mid2 = 0;
int speed = 0;

int direction = 0;
int angle_1 = 0;
int angle_2 = 0;
int angle_3 = 0;
int angle_4 = 0;
int angle_5 = 0;
int angle = 0;  // 输入需转动的角度值

int update = 0;

int motor_con_us = 0;
TwoWire I2Cone = TwoWire(0);

int readValue = 0;
byte readArray[2];
int poscon = 0;
double angle_res = 0.08789;
double angle_mid = 0;
double angle_dee = 0;  // 读取角度值
double angle_set = 180;  // 设定角度值
int turn_num_aim = 0;  // 设定需转动圈数
int turn_num_con = 1;  // 标记已转动圈数

int reset_sign = 0;

int wh_con = 0;
char uart1_res[] = "#0105000000003!";

void motorSERVE(void *xTask1)
{
  while (1)
  {
    if (motor_mode == 0)
    {
      delay(4);
    }
    else if (motor_mode == 1)
    {
      // 每100次循环为360°，0.9°步距角，一次循环为4步距
      // for(int i = 0;i<100;i++)
      // {
      if ((step_con == 1)&&(motor_mode == 1))
      {
        // 2
        digitalWrite(output_motor_a1, HIGH);
        digitalWrite(output_motor_a2, LOW);
        digitalWrite(output_motor_b1, HIGH);
        digitalWrite(output_motor_b2, LOW);
        usleep(delay_time);
        // 3
        digitalWrite(output_motor_a1, LOW);
        digitalWrite(output_motor_a2, LOW);
        digitalWrite(output_motor_b1, HIGH);
        digitalWrite(output_motor_b2, LOW);
        usleep(delay_time);
        // 4
        digitalWrite(output_motor_a1, LOW);
        digitalWrite(output_motor_a2, HIGH);
        digitalWrite(output_motor_b1, HIGH);
        digitalWrite(output_motor_b2, LOW);
        usleep(delay_time);
        // 5
        digitalWrite(output_motor_a1, LOW);
        digitalWrite(output_motor_a2, HIGH);
        digitalWrite(output_motor_b1, LOW);
        digitalWrite(output_motor_b2, LOW);
        usleep(delay_time);
        step_con = 2;
      }
      if ((step_con == 2)&&(motor_mode == 1))
      {
        // 6
        digitalWrite(output_motor_a1, LOW);
        digitalWrite(output_motor_a2, HIGH);
        digitalWrite(output_motor_b1, LOW);
        digitalWrite(output_motor_b2, HIGH);
        usleep(delay_time);
        // 7
        digitalWrite(output_motor_a1, LOW);
        digitalWrite(output_motor_a2, LOW);
        digitalWrite(output_motor_b1, LOW);
        digitalWrite(output_motor_b2, HIGH);
        usleep(delay_time);
        // 8
        digitalWrite(output_motor_a1, HIGH);
        digitalWrite(output_motor_a2, LOW);
        digitalWrite(output_motor_b1, LOW);
        digitalWrite(output_motor_b2, HIGH);
        usleep(delay_time);
        // 1
        digitalWrite(output_motor_a1, HIGH);
        digitalWrite(output_motor_a2, LOW);
        digitalWrite(output_motor_b1, LOW);
        digitalWrite(output_motor_b2, LOW);
        usleep(delay_time);
        step_con = 1;
      }
      // }
      // motor_mode = 0;
    }
    else if (motor_mode == 2)
    {
      // for(int i = 0;i<100;i++)
      // {
      if ((step_con == 1)&&(motor_mode == 2))
      {
        // 2
        digitalWrite(output_motor_a1, HIGH);
        digitalWrite(output_motor_a2, LOW);
        digitalWrite(output_motor_b1, LOW);
        digitalWrite(output_motor_b2, HIGH);
        usleep(delay_time);
        // 3
        digitalWrite(output_motor_a1, LOW);
        digitalWrite(output_motor_a2, LOW);
        digitalWrite(output_motor_b1, LOW);
        digitalWrite(output_motor_b2, HIGH);
        usleep(delay_time);
        // 4
        digitalWrite(output_motor_a1, LOW);
        digitalWrite(output_motor_a2, HIGH);
        digitalWrite(output_motor_b1, LOW);
        digitalWrite(output_motor_b2, HIGH);
        usleep(delay_time);
        // 5
        digitalWrite(output_motor_a1, LOW);
        digitalWrite(output_motor_a2, HIGH);
        digitalWrite(output_motor_b1, LOW);
        digitalWrite(output_motor_b2, LOW);
        usleep(delay_time);
        step_con = 2;
      }
      if ((step_con == 2)&&(motor_mode == 2))
      {
        // 6
        digitalWrite(output_motor_a1, LOW);
        digitalWrite(output_motor_a2, HIGH);
        digitalWrite(output_motor_b1, HIGH);
        digitalWrite(output_motor_b2, LOW);
        usleep(delay_time);
        // 7
        digitalWrite(output_motor_a1, LOW);
        digitalWrite(output_motor_a2, LOW);
        digitalWrite(output_motor_b1, HIGH);
        digitalWrite(output_motor_b2, LOW);
        usleep(delay_time);
        // 8
        digitalWrite(output_motor_a1, HIGH);
        digitalWrite(output_motor_a2, LOW);
        digitalWrite(output_motor_b1, HIGH);
        digitalWrite(output_motor_b2, LOW);
        usleep(delay_time);
        // 1
        digitalWrite(output_motor_a1, HIGH);
        digitalWrite(output_motor_a2, LOW);
        digitalWrite(output_motor_b1, LOW);
        digitalWrite(output_motor_b2, LOW);
        usleep(delay_time);
        step_con = 1;
      }
      // }
      // motor_mode = 0;
    }
    // Serial.println(step_con);
  }
}

void codSERVE(void *xTask2)
{
  int turn_jug_s = 0;
  int turn_jug_n = 0;
  while (1)
  {
    I2Cone.requestFrom(0X36, (uint8_t)2);
    for (byte i = 0; i < 2; i++)
    {
      readArray[i] = I2Cone.read();
    }

    // depending on the sensor architecture there are different combinations of
    // LSB and MSB register used bits
    // AS5600 uses 0..7 LSB and 8..11 MSB
    // AS5048 uses 0..5 LSB and 6..13 MSB
    // readValue = ( readArray[1] &  lsb_mask );
    // readValue += ( ( readArray[0] & msb_mask ) << lsb_used );
    readValue = readArray[0] * 256 + readArray[1];
    // Serial.println(readValue);
    // Serial1.println(readValue);
    if (readValue > 4076){turn_jug_s = 1;}
    if (turn_jug_s == 1)
    {
      if ((readValue >= 0)&&(readValue < 20))
      {
        turn_num_con = turn_num_con + 1;
        turn_jug_s = 0;
      }
      else if (readValue <= 4076)
      {
        turn_jug_s = 0;
      }
    }  // 顺时针过圈
    if ((readValue >= 0)&&(readValue < 20)){turn_jug_n = 1;}
    if (turn_jug_n == 1)
    {
      if (readValue > 4076)
      {
        turn_num_con = turn_num_con - 1;
        turn_jug_n = 0;
      }
      else if (readValue >= 20)
      {
        turn_jug_n = 0;
      }
    }  // 逆时针过圈
    angle_mid = angle_res * readValue;

    if (turn_num_con >= 0){angle_dee = turn_num_con * 360 + angle_mid;}
    else if (turn_num_con < 0){angle_dee = (turn_num_con + 1) * 360 - (360 - angle_mid);}
    
    // Serial.println(turn_num_con);
    delay(2);
  }
}

void harSERVE(void *xTask3)
{
  while (1)
  {
    if (Serial2.available())
    {
      // Serial.println("###");
      update = 1;
      int rx_con = 0;
      for (int i = 0;i < 18;i++)
      {
        if ((i > 0)&&(i < 14))
        {
          rx_con = Serial2.read() - 48;
          if(rx_con == 0){uart1_res[i] = '0';}
          else if(rx_con == 1){uart1_res[i] = '1';}
          else if(rx_con == 2){uart1_res[i] = '2';}
          else if(rx_con == 3){uart1_res[i] = '3';}
          else if(rx_con == 4){uart1_res[i] = '4';}
          else if(rx_con == 5){uart1_res[i] = '5';}
          else if(rx_con == 6){uart1_res[i] = '6';}
          else if(rx_con == 7){uart1_res[i] = '7';}
          else if(rx_con == 8){uart1_res[i] = '8';}
          else if(rx_con == 9){uart1_res[i] = '9';}
        }
        else
        {
          Serial2.read();
        }
      }
      
      Serial1.println(uart1_res);
      Serial.println(uart1_res);
    }

    if ((update == 1)&&(uart1_res[13] == '3'))
    {
      if(uart1_res[1] == '0')
      {
        digitalWrite(outputled1, HIGH);
      }
      else if(uart1_res[1] == '1')
      {
        digitalWrite(outputled1, LOW);
      }

      // if(uart1_res[2] == '0')
      // {
      //   motor_mode = 2;
      // }
      // else if(uart1_res[2] == '1')
      // {
      //   motor_mode = 0;
      //   reset_sign = 0;
      // }
      // else if(uart1_res[2] == '2')
      // {
      //   motor_mode = 1;
      // }

      // if(uart1_res[3] == '0'){speed_mid1 = 0;}
      // else if(uart1_res[3] == '1'){speed_mid1 = 1;}
      // else if(uart1_res[3] == '2'){speed_mid1 = 2;}

      // if(uart1_res[4] == '0'){speed_mid2 = 0;}
      // else if(uart1_res[4] == '1'){speed_mid2 = 1;}
      // else if(uart1_res[4] == '2'){speed_mid2 = 2;}
      // else if(uart1_res[4] == '3'){speed_mid2 = 3;}
      // else if(uart1_res[4] == '4'){speed_mid2 = 4;}
      // else if(uart1_res[4] == '5'){speed_mid2 = 5;}
      // else if(uart1_res[4] == '6'){speed_mid2 = 6;}
      // else if(uart1_res[4] == '7'){speed_mid2 = 7;}
      // else if(uart1_res[4] == '8'){speed_mid2 = 8;}
      // else if(uart1_res[4] == '9'){speed_mid2 = 9;}

      if((uart1_res[6] == '1')||(uart1_res[6] == '2'))
      {
        if (uart1_res[6] == '1'){direction = 1;}
        else if (uart1_res[6] == '2'){direction = 2;}

        if(uart1_res[7] == '0'){angle_1 = 0;}
        else if(uart1_res[7] == '1'){angle_1 = 1;}
        else if(uart1_res[7] == '2'){angle_1 = 2;}
        else if(uart1_res[7] == '3'){angle_1 = 3;}
        else if(uart1_res[7] == '4'){angle_1 = 4;}
        else if(uart1_res[7] == '5'){angle_1 = 5;}
        else if(uart1_res[7] == '6'){angle_1 = 6;}
        else if(uart1_res[7] == '7'){angle_1 = 7;}
        else if(uart1_res[7] == '8'){angle_1 = 8;}
        else if(uart1_res[7] == '9'){angle_1 = 9;}

        if(uart1_res[8] == '0'){angle_2 = 0;}
        else if(uart1_res[8] == '1'){angle_2 = 1;}
        else if(uart1_res[8] == '2'){angle_2 = 2;}
        else if(uart1_res[8] == '3'){angle_2 = 3;}
        else if(uart1_res[8] == '4'){angle_2 = 4;}
        else if(uart1_res[8] == '5'){angle_2 = 5;}
        else if(uart1_res[8] == '6'){angle_2 = 6;}
        else if(uart1_res[8] == '7'){angle_2 = 7;}
        else if(uart1_res[8] == '8'){angle_2 = 8;}
        else if(uart1_res[8] == '9'){angle_2 = 9;}

        if(uart1_res[9] == '0'){angle_3 = 0;}
        else if(uart1_res[9] == '1'){angle_3 = 1;}
        else if(uart1_res[9] == '2'){angle_3 = 2;}
        else if(uart1_res[9] == '3'){angle_3 = 3;}
        else if(uart1_res[9] == '4'){angle_3 = 4;}
        else if(uart1_res[9] == '5'){angle_3 = 5;}
        else if(uart1_res[9] == '6'){angle_3 = 6;}
        else if(uart1_res[9] == '7'){angle_3 = 7;}
        else if(uart1_res[9] == '8'){angle_3 = 8;}
        else if(uart1_res[9] == '9'){angle_3 = 9;}

        if(uart1_res[10] == '0'){angle_4 = 0;}
        else if(uart1_res[10] == '1'){angle_4 = 1;}
        else if(uart1_res[10] == '2'){angle_4 = 2;}
        else if(uart1_res[10] == '3'){angle_4 = 3;}
        else if(uart1_res[10] == '4'){angle_4 = 4;}
        else if(uart1_res[10] == '5'){angle_4 = 5;}
        else if(uart1_res[10] == '6'){angle_4 = 6;}
        else if(uart1_res[10] == '7'){angle_4 = 7;}
        else if(uart1_res[10] == '8'){angle_4 = 8;}
        else if(uart1_res[10] == '9'){angle_4 = 9;}

        if(uart1_res[11] == '0'){angle_5 = 0;}
        else if(uart1_res[11] == '1'){angle_5 = 1;}
        else if(uart1_res[11] == '2'){angle_5 = 2;}
        else if(uart1_res[11] == '3'){angle_5 = 3;}
        else if(uart1_res[11] == '4'){angle_5 = 4;}
        else if(uart1_res[11] == '5'){angle_5 = 5;}
        else if(uart1_res[11] == '6'){angle_5 = 6;}
        else if(uart1_res[11] == '7'){angle_5 = 7;}
        else if(uart1_res[11] == '8'){angle_5 = 8;}
        else if(uart1_res[11] == '9'){angle_5 = 9;}
      }

      if(uart1_res[12] == '1')
      {
        reset_sign = 1;
      }

      angle = angle_1 * 10000 + angle_2 * 1000 + angle_3 * 100 + angle_4 * 10 + angle_5;
      // speed = speed_mid1 * 10 + speed_mid2;
      // delay_time = speed;

      // Serial.println(speed);
      // Serial.println(direction);
      // Serial.println(angle);
      // Serial.println(reset_sign);
      update = 0;
    }
    delay(10);
  }
}

void conSERVE(void *xTask4)
{
  double angle_err = 0;
  double angle_err_last = 0;
  double angle_err_last2 = 0;
  double speed = 550;
  double p = 70;
  double i = 0.5;
  double d = 0;
  double speed_out = 0;
  int con = 0;
  // double turn_judge = 3;

  while (1)
  {
    if (reset_sign == 1)
    {
      if (con == 0)
      {
        angle_set = 180;
        con = 1;
      }
    }
    else if(direction == 1)
    {
      if (con == 0)
      {
        angle_set = angle_set - angle;
        con = 1;
      }
    }
    else if(direction == 2)
    {
      if (con == 0)
      {
        angle_set = angle_set + angle;
        con = 1;
      }
    }

    // Serial.print(angle_set);
    // Serial.print(",");
    // Serial.println(angle_dee);
    
    angle_err = angle_dee - angle_set;
    if (angle_err < 0)
    {
      speed_out = -(p * angle_err +  i * (angle_err + angle_err_last) + d * (angle_err_last - angle_err_last2));
      speed = (int)(3450 - speed_out);
      if (speed <=400 ){speed = 400;}
      // Serial.println(speed);
      delay_time = speed;
      motor_mode = 1;
    }
    if (angle_err > 0)
    {
      speed_out = p * angle_err +  i * (angle_err + angle_err_last) + d * (angle_err_last - angle_err_last2);
      speed = (int)(3450 - speed_out);
      if (speed <=400 ){speed = 400;}
      // Serial.println(speed);
      delay_time = speed;
      motor_mode = 2;
    }
    
    if ((angle_err < 0.9)&&(angle_err > -0.9))
    {
      if (reset_sign == 1)
      {
        reset_sign = 0;
      }
      direction = 0;
      motor_mode = 0;
      con = 0;
    }
    
    delay(10);
  }
}

void setup()
{
  Serial.begin(115200);
  Serial1.begin(115200);
  Serial2.begin(115200);
  // Initialize the output variables as outputs
  pinMode(outputled1, OUTPUT);
  pinMode(output_motor_a1, OUTPUT);
  pinMode(output_motor_a2, OUTPUT);
  pinMode(output_motor_b1, OUTPUT);
  pinMode(output_motor_b2, OUTPUT);
  // Set outputs to LOW
  digitalWrite(outputled1, LOW);
  digitalWrite(output_motor_a1, LOW);
  digitalWrite(output_motor_a2, LOW);
  digitalWrite(output_motor_b1, LOW);
  digitalWrite(output_motor_b2, LOW);

  I2Cone.begin(21, 22, 100000);
  I2Cone.beginTransmission(0X36);
  I2Cone.write(0X0C);
  I2Cone.endTransmission(false);

  // timer = timerBegin(3, 80, true);
  // timerAttachInterrupt(timer, &TimerEvent, true);
  // timerAlarmWrite(timer, 200, true);
  // timerAlarmEnable(timer); //	使能定时器

  digitalWrite(outputled1, LOW);

#if !USE_MULTCORE

  xTaskCreate(
      motorSERVE,   /* Task function. */
      "motorSERVE", /* String with name of task. */
      4096,         /* Stack size in bytes. */
      NULL,         /* Parameter passed as input of the task */
      1,            /* Priority of the task.(configMAX_PRIORITIES - 1 being the highest, and 0 being the lowest.) */
      NULL);        /* Task handle. */

  xTaskCreate(
      codSERVE,   /* Task function. */
      "codSERVE", /* String with name of task. */
      4096,       /* Stack size in bytes. */
      NULL,       /* Parameter passed as input of the task */
      2,          /* Priority of the task.(configMAX_PRIORITIES - 1 being the highest, and 0 being the lowest.) */
      NULL);      /* Task handle. */

  xTaskCreate(
      harSERVE,   /* Task function. */
      "harSERVE", /* String with name of task. */
      4096,       /* Stack size in bytes. */
      NULL,       /* Parameter passed as input of the task */
      3,          /* Priority of the task.(configMAX_PRIORITIES - 1 being the highest, and 0 being the lowest.) */
      NULL);      /* Task handle. */

  xTaskCreate(
      conSERVE,   /* Task function. */
      "conSERVE", /* String with name of task. */
      4096,       /* Stack size in bytes. */
      NULL,       /* Parameter passed as input of the task */
      2,          /* Priority of the task.(configMAX_PRIORITIES - 1 being the highest, and 0 being the lowest.) */
      NULL);      /* Task handle. */

#else

  //最后一个参数至关重要，决定这个任务创建在哪个核上.PRO_CPU 为 0, APP_CPU 为 1,或者 tskNO_AFFINITY 允许任务在两者上运行.
  xTaskCreatePinnedToCore(motorSERVE, "motorSERVE", 4096, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(codSERVE, "codSERVE", 4096, NULL, 2, NULL, 0);
  xTaskCreatePinnedToCore(harSERVE, "harSERVE", 4096, NULL, 4, NULL, 0);
  xTaskCreatePinnedToCore(conSERVE, "conSERVE", 4096, NULL, 3, NULL, 0);

#endif
}

void loop()
{
}
