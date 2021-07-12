  /* This is advanced micro drone that can handle many functions/features. Code developed by [Copyrights@Mohamed A.Wx)].
 *----------------------------------------------------------------------------------------------------------------------------
 *********************************          Features  & Sensors           *********************
 *----------------------------------------------------------------------------------------------------------------------------
 * 0: (- MPU6050 Sensor   -) =>  *    - Gyroscope / Accelrometer.
 * 1: (- Barometer Sensor -) =>  *    - Pressure / Height.
 * 2: (- -) =>  * 
 * 3: (- -) =>  *
 * 4: (- -) =>  *
 * 5: (- -) =>  *
 * 6: (- -) =>  *
 * 7: (- -) =>  *
 * 8: (- -) =>  *
 *---------------------------------------------------------------------------------------------------------------------------- */
 /*===========================================> CODE_START <=====================================================*/
 /********************************************* Libraries / Headers ***************************************/
  #include "Wx6050.h"
  #include "WxBMP180.h"
  #include "WxMOTORS.h"
/*=============================================================================================================================*/
  Wx6050   wx6050(true);
  WxBMP180 bmp180(true);
/*=============== GENERAL PURPOSE DECLARATION ===========*/
  unsigned long long Wx6050_T = 0;
  unsigned long long WxBMP180_T = 0;
  unsigned long long TIME2 = 0;
  unsigned long long TIME3 = 0;
  unsigned long long TIME4 = 0;
/*=============================================================================================================================*/
  void setup(void) 
  {  Serial.begin(115000); // Serial Communication with baurdrate: 11.5 kbit/s
  
//==> START: MPU6050/Wx6050 <==
    wx6050.INIT(); // MPU6050 initlaizing function.
//==> END: MPU6050/Wx6050 <==




  Wx6050_T = micros(); WxBMP180_T = micros(); TIME2 = millis(); TIME3 = millis(); TIME4 = millis();}
/*=============================================================================================================================*/
  void loop(void) 
  {
//==> START: MPU6050/Wx6050 <==
      wx6050.DATA();       // Getting data from mpu6050 sensor.
      wx6050.ACCUARCY();  // filtering and modifying the data collected.
      wx6050.outputs();
     
     while(micros() - Wx6050_T < 4000);                                    //Wait until the loop_timer reaches 4000us (250Hz) before starting the next loop
      Wx6050_T = micros();                                               //Reset the loop timer
//==> END: MPU6050/Wx6050 <==

//==> START: Barometer/Wx180 <==

//==> END: Barometer/WxBMP180 <==
  }
/*==============================================================================================================================*/
 //===========================================================>CODE_END<=================================================================================
