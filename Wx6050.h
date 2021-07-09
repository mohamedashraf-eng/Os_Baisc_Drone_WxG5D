//====================================== LIBRARY / HEADER ===========================
  #ifndef Wx6050_H  // protect from overdef.
  #define Wx6050_H
  #include "Arduino.h" 
//====================================== START Section 1: MPU6050 =========================== 
//=================== PUBLIC:: PREPROCCESOR - MPU6050 ====================
  #include <Wire.h>
  #define CLK_Hz          400000    // I2C Data clock for Communciation between STM32 and MPU-6050. 
  #define MS_REG_ADDR     0x68     //  I2C MASTER-SLAVE UNIQUE ADDRESS CONTROL for MPU-6050.
      
  #define PW_MGMT_REG     0x6B    //   Power Management Register Address for Gyroscope data for MPU-6050.
  #define RQ_SETxPWMGMT   0x00   //    Setting SLEEP register to 0. (NOT SLEEP MODE)
    
  #define Gyro_ADDR       0x1B    //   MPU-6050 Regester Address for Gyroscope configuration.
  #define RQ_SETxGyro     0x08   //    Request setting to adjust the Gyro mode.   (to full scale +/- 500d DPS)
  #define Gyro_DATA_REG   0x43  //     Starting the register for Gyro Readings.
     
  #define ACCEL_ADDR      0x1C      //   MPU-6050 Regester Address for Accelrometer configuration.
  #define RQ_SETxACCEL    0x10     //    Request setting to adjust the Accel mode. (to Setting the accel to +/- 8g)
  #define ACCEL_DATA_REG  0x3B    //     Starting the register for Accel Readings.
    
  #define TEMP_DATA_REG   0x41  // Starting the register for Temp Readings.
//=================== CLASS DEFENTION ====================
  class Wx6050
  {
    private:
//============= PRIVATE OBJECTS ==========
//============= PRIVATE METHODS ==========   
    public:
//============= PUBLIC OBJECTS ==========
//============= PUBLIC METHODS ==========
      Wx6050(bool GWx6050STATE = false);
      
      void INIT(void);
      void DATA(void);
      void ACCUARCY(void);
      void outputs(void);
  }; // end Wx6050 Class.  
//====================================== END : MPU6050 =========================== 
//****************************************************************************************************************************
  #endif
