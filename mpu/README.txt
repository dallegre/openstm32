To build this:

- Install openstm32
- Create a new openstm32 project.
  - select C project
  - select Ac6 STM32 MCU GCC
  - next
  - next
  - select stm32f4
  - select nucleo stm32f446re
- Add the files to the src and inc directories


Circuit:

  - stm32f446re nucleo board
  - ssd1306 display (I used 43oh.com board, sparkfun and adafruit have their board.  They should all work the same.)
  - mpu6050 (again, many different boards, some have special pins like AD0 (lsb of i2c addr))
  - NOTE pins were changed

         connect:
      stm32    sdd1306
     - pa6  to  D/C (data/command) pin
     - pa7  to  CS pin
     - pc10 to  SCLK pin
     - pc12 to  SDIN
     - pb10 to  RES
     - 3v3  to  VCC
     - gnd  to  gnd

          connect:
       stm32   mpu6050
     - pb8  to  SCL
     - pb9  to  SDA
     - gnd  to  gnd
     - 5v   to  5v
