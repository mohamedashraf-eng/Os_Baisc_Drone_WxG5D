//====================================== LIBRARY / HEADER ===========================
  #ifndef WxBMP180_H  // protect from overdef.
  #define WxBMP180_H
  #include "Arduino.h" 
//====================================== START Section 1: BMP180 =========================== 
//=================== PUBLIC:: PREPROCCESOR - BMP180 ====================

//=================== CLASS DEFENTION ====================

  class WxBMP180
  {
    private:
//============= PRIVATE OBJECTS ==========
//============= PRIVATE METHODS ==========   
    public:
//============= PUBLIC OBJECTS ==========
//============= PUBLIC METHODS ==========
      WxBMP180(bool GWxBMP180STATE = false);

      void INIT(void);
      
  };
//====================================== END : BMP180 =========================== 
//****************************************************************************************************************************
  #endif
