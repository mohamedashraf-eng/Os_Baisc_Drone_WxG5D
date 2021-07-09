//====================================== HEADERS ===========================  
  #include "Wx6050.h"
//********************************-> START: MPU6050 <-*************************************
//====================================== MPU6050 PRIVATE:: METHODS ===========================  
    void Wx6050_SETUP();
    void MPU_SETUP();
    void GYRO_SETUP();
    void ACCEL_SETUP();
    void GYRO_DATA();
    void ACCEL_DATA();
//====================================== MPU6050 PRIVATE:: OBJECTS ===========================  
   int Gyro_x, Gyro_y, Gyro_z;                              // Gyroscope variables.
   long ACCEL_x, ACCEL_y, ACCEL_z, ACCEL_TVECTOR;          // Accelerometer variables.
      
   int TEMPERATURE;             // Temperature variables
   
   long GyroCal_x, GyroCal_y, GyroCal_z;    // Filtering Variables.
    
   float PITCH_ANGLE , ROLL_ANGLE;        // Pitch & Rol Variables.
      
   boolean SET_Gyro_ANGLES;
      
   float ROLL_ANGLE_ACCEL , PITCH_ANGLE_ACCEL;
   float OUTPUT_PA , OUTPUT_RA;
//================================================================================================================== 
  Wx6050::Wx6050(bool GWx6050STATE){}
//============================ PUBLIC:: MAIN METHODS ================================================================
  void Wx6050::INIT(void)  // MPU6050 INITLAZING.
  {
    Wire.setClock(CLK_Hz);                       // Setting Clock speed for I2C clock bus for 400kHz.
    Wire.begin();                               //  Start the I2C Serial Communication as (MASTER).
    delay(150);                                //   START DELAY TIME.
    
    Wx6050_SETUP();
    MPU_SETUP();
    GYRO_SETUP();
    ACCEL_SETUP();
  }
  void Wx6050::DATA(void) // MPU6050 COLLECTING DATA.
  {
    GYRO_DATA();
    ACCEL_DATA();
  }
//------------------------------------------------------------------------------------------------------------------ 
  void Wx6050::ACCUARCY(void) //MPU6050 FILTERING DATA.
  { 
    Gyro_x -= GyroCal_x;                                                  //Subtract the offset calibration value from the raw gyro_x value
    Gyro_y -= GyroCal_y;                                                 //Subtract the offset calibration value from the raw gyro_y value
    Gyro_z -= GyroCal_z;                                                //Subtract the offset calibration value from the raw gyro_z value
    
//Gyro angle calculations.
//0.0000611 = 1 / (250Hz / 65.5).
    PITCH_ANGLE += Gyro_x * 0.0000611;                                     //Calculate the traveled pitch angle and add this to the angle_pitch variable
    ROLL_ANGLE  += Gyro_y * 0.0000611;                                    //Calculate the traveled roll angle and add this to the angle_roll variable
    
//0.000001066 = 0.0000611 * (3.142(PI) / 180degr) The Arduino sin function is in radians.
    PITCH_ANGLE += ROLL_ANGLE  * sin(Gyro_z * 0.000001066);                //If the IMU has yawed transfer the roll angle to the pitch angel
    ROLL_ANGLE  -= PITCH_ANGLE * sin(Gyro_z * 0.000001066);               //If the IMU has yawed transfer the pitch angle to the roll angel
    
    //Accelerometer angle calculations
    ACCEL_TVECTOR = sqrt((ACCEL_x*ACCEL_x) + (ACCEL_y*ACCEL_y )+ (ACCEL_z*ACCEL_z));  //Calculate the total accelerometer vector
//57.296 = 1 / (3.142 / 180) The Arduino asin function is in radians.
    PITCH_ANGLE_ACCEL = asin((float)ACCEL_y/ACCEL_TVECTOR) *  57.296;        //Calculate the pitch angle
    ROLL_ANGLE_ACCEL  = asin((float)ACCEL_x/ACCEL_TVECTOR) * -57.296;       //Calculate the roll angle
    
    //Place the MPU-6050 spirit level and note the values in the following two lines for calibration
    PITCH_ANGLE_ACCEL -= 0.0;                                                //Accelerometer calibration value for pitch
    ROLL_ANGLE_ACCEL  -= 0.0;                                               //Accelerometer calibration value for roll
  
    if(SET_Gyro_ANGLES){                                                     //If the IMU is already started
      PITCH_ANGLE = PITCH_ANGLE * 0.9996  + PITCH_ANGLE_ACCEL * 0.0004;     //Correct the drift of the gyro pitch angle with the accelerometer pitch angle
      ROLL_ANGLE  = ROLL_ANGLE  * 0.9996  + ROLL_ANGLE_ACCEL  * 0.0004;    //Correct the drift of the gyro roll angle with the accelerometer roll angle
    }
    else{                                                                   //At first start
      PITCH_ANGLE = PITCH_ANGLE_ACCEL;                                     //Set the gyro pitch angle equal to the accelerometer pitch angle 
      ROLL_ANGLE = ROLL_ANGLE_ACCEL;                                      //Set the gyro roll angle equal to the accelerometer roll angle 
      SET_Gyro_ANGLES = true;                                            //Set the IMU started flag
    }
    
//To dampen the pitch and roll angles a complementary filter is used.
    OUTPUT_PA = OUTPUT_PA * 0.9 + PITCH_ANGLE * 0.1;       //Take 90% of the output pitch value and add 10% of the raw pitch value
    OUTPUT_RA = OUTPUT_RA * 0.9 + ROLL_ANGLE  * 0.1;      //Take 90% of the output roll value and add 10% of the raw roll value
  }
//============================ PRIVATE:: MAIN METHODS - BUILD IN CPP ================================================================
//------------------------------------------------------------------------------------------------------------------
  void MPU_SETUP(void) // SETUP THE SENSOR TO START COLLECTING DATA.
  {
    Wire.beginTransmission(MS_REG_ADDR); // Start the communication with (MPU-6050).
    Wire.write(PW_MGMT_REG);            //  Accessing the register 6B - Power Management.
    Wire.write(RQ_SETxPWMGMT);         //   Setting SLEEP register to 0.
    Wire.endTransmission();           //    Terminate the connection with (MPU-6050).  
  }
//------------------------------------------------------------------------------------------------------------------
// Because MPU-6050 chip based on one Processor. So its unquie register to start SLAVE mode is "0x68". [DATA]
  void GYRO_SETUP(void) //SETUP THE MPU6050 Gyroscope register TO START COLLECTING DATA.
  {
    Wire.beginTransmission(MS_REG_ADDR);             // Start the communication with (MPU-6050).
    Wire.write(Gyro_ADDR);                          //  Accessing the register 1B - Gyroscope Configuration.
    Wire.write(RQ_SETxGyro);                       //   Set register "0x1C" to zero (to INIT the MPU-6050).
    Wire.endTransmission();                       //    Terminate the connection with (MPU-6050).
  }
//------------------------------------------------------------------------------------------------------------------

  void ACCEL_SETUP(void) //SETUP THE MPU6050 ACCELROMETER register TO START COLLECTING DATA.
  {  
    Wire.beginTransmission(MS_REG_ADDR);             // Start the communication with (MPU-6050).
    Wire.write(ACCEL_ADDR);                         //  Accessing the register 1C - Accelerometer Configuration.
    Wire.write(RQ_SETxACCEL);                      //   Set register "0x6B" to zero (to INIT the MPU-6050).
    Wire.endTransmission();                       //    Terminate the connection with (MPU-6050).  
  }
//------------------------------------------------------------------------------------------------------------------

  void GYRO_DATA(void) // START GETTING THE DATA FROM MPU6050 Gyroscope register. (shifting the data)
  {
    Wire.beginTransmission(MS_REG_ADDR);                    // Start the I2C communication with (MPU-6050). "0x68"
    Wire.write(Gyro_DATA_REG);                             //  Sent byte to start data communication. "0x43"
    Wire.endTransmission();                               //   Terminate the connection with (MPU-6050).

    Wire.requestFrom(0x68,6);
    
    while(Wire.available() < 6);   // Wait until the register get the data. #ENDWHILE
    Gyro_x = Wire.read() << 8 | Wire.read(); // shift high byte left and add low and high byte for ACCEL_x-axis. <<0000 0000 : 0000 0000 Shifting right.
    Gyro_y = Wire.read() << 8 | Wire.read(); // shift high byte left and add low and high byte for ACCEL_y-axis. <<0000 0000 : 0000 0000 Shifting right.
    Gyro_z = Wire.read() << 8 | Wire.read(); // shift high byte left and add low and high byte for ACCEL_z-axis. <<0000 0000 : 0000 0000 Shifting right.
  }
//------------------------------------------------------------------------------------------------------------------
  void ACCEL_DATA(void) // START GETTING THE DATA FROM MPU6050 ACCELROMETER register. (shifting the data)
  {
    Wire.beginTransmission(MS_REG_ADDR);                    // Start the communication with (MPU-6050). "0x68"
    Wire.write(ACCEL_DATA_REG);                            //  Sent byte to start communication. "0x43"
    Wire.endTransmission();                               //   Terminate the connection with (MPU-6050).

    Wire.requestFrom(0x68,6);
    
    while(Wire.available() < 6);   // Wait until the register get the data. #ENDWHILE
    ACCEL_x = Wire.read() << 8 | Wire.read(); // shift high byte left and add low and high byte for ACCEL_x-axis. <<0000 0000 : 0000 0000 Shifting right.
    ACCEL_y = Wire.read() << 8 | Wire.read(); // shift high byte left and add low and high byte for ACCEL_y-axis. <<0000 0000 : 0000 0000 Shifting right.
    ACCEL_z = Wire.read() << 8 | Wire.read(); // shift high byte left and add low and high byte for ACCEL_z-axis. <<0000 0000 : 0000 0000 Shifting right.
  }
//------------------------------------------------------------------------------------------------------------------
  void Wx6050_SETUP(void) // SETUP THE Wx6050 DATA COLLECT INIT.
  {
//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-  
     for (int COUNTER = 0; COUNTER < 2000 ; COUNTER++)  //Run this code 2000 times
        {                  
           GYRO_DATA(); // Defining the class of data function.
           GyroCal_x += Gyro_x;                                    //Add the gyro x-axis offset to the gyro_x_cal variable
           GyroCal_y += Gyro_y;                                   //Add the gyro y-axis offset to the gyro_y_cal variable
           GyroCal_z += Gyro_z;                                  //Add the gyro z-axis offset to the gyro_z_cal variable   
           delay(3);                                            //Delay 3us to simulate the 250Hz program loop
        }
     GyroCal_x /= 2000;                                       //Divide the gyro_x_cal variable by 2000 to get the avarage offset
     GyroCal_y /= 2000;                                      //Divide the gyro_y_cal variable by 2000 to get the avarage offset
     GyroCal_z /= 2000;                                     //Divide the gyro_z_cal variable by 2000 to get the avarage offset
  }
//------------------------------------------------------------------------------------------------------------------ 
void Wx6050::outputs(void)
  {
     Serial.print("OUTPUT PITCH ANGLE = ");     Serial.print(OUTPUT_PA);
     Serial.print("\tOUTPUT ROLL ANGLE  = ");   Serial.println(OUTPUT_RA);
  }
//********************************-> END MPU6050 <-************************************* 
