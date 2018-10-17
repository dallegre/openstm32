#include "FontPack.h"
#include "icons.h"
#include "CourierNew5x7.h"
#include "CourierNew14x15.h"   //whatever.

// OLED PORTS CONFIG.   put this in a header file.
#define OLED_SPI_DIR P1DIR
#define OLED_SPI_SEL P1SEL
#define OLED_SPI_SEL2 P1SEL2

#define OLED_IO_DIR P2DIR
#define OLED_IO_OUT P2OUT

#define OLED_CS_DIR P2DIR
#define OLED_CS_OUT P2OUT

// OLED GPIO CONFIG
#define OLED_RES BIT1   // P2.1
#define OLED_DC BIT2    // P2.2
#define OLED_CS BIT0    // P2.0
#define OLED_SIMO BIT7  // P1.7
//#define OLED_SOMI BIT6    // P1.6; unused for OLED
#define OLED_SCLK BIT5  // P1.5

// SSD1306 PARAMETERS
#define SSD1306_LCDWIDTH 128
#define SSD1306_LCDHEIGHT 64
#define SSD1306_MAXROWS 7
#define SSD1306_MAXCONTRAST 0xFF

    // command table
#define SSD1306_SETCONTRAST 0x81
#define SSD1306_DISPLAYALLON_RESUME 0xA4
#define SSD1306_DISPLAYALLON 0xA5
#define SSD1306_NORMALDISPLAY 0xA6
#define SSD1306_INVERTDISPLAY 0xA7
#define SSD1306_DISPLAYOFF 0xAE
#define SSD1306_DISPLAYON 0xAF

    // scrolling commands
#define SSD1306_SCROLL_RIGHT 0x26
#define SSD1306_SCROLL_LEFT 0X27
#define SSD1306_SCROLL_VERT_RIGHT 0x29
#define SSD1306_SCROLL_VERT_LEFT 0x2A
#define SSD1306_SET_VERTICAL 0xA3

    // address setting
#define SSD1306_SETLOWCOLUMN 0x00
#define SSD1306_SETHIGHCOLUMN 0x10
#define SSD1306_MEMORYMODE 0x20
#define SSD1306_COLUMNADDRESS 0x21
#define SSD1306_COLUMNADDRESS_MSB 0x00
#define SSD1306_COLUMNADDRESS_LSB 0x7F
#define SSD1306_PAGEADDRESS 0x22
#define SSD1306_PAGE_START_ADDRESS 0xB0

    // hardware configuration
#define SSD1306_SETSTARTLINE 0x40
#define SSD1306_SEGREMAP 0xA1
#define SSD1306_SETMULTIPLEX 0xA8
#define SSD1306_COMSCANINC 0xC0
#define SSD1306_COMSCANDEC 0xC8
#define SSD1306_SETDISPLAYOFFSET 0xD3
#define SSD1306_SETCOMPINS 0xDA

    // timing and driving
#define SSD1306_SETDISPLAYCLOCKDIV 0xD5
#define SSD1306_SETPRECHARGE 0xD9
#define SSD1306_SETVCOMDETECT 0xDB
#define SSD1306_NOP 0xE3

    // power supply configuration
#define SSD1306_CHARGEPUMP 0x8D
#define SSD1306_EXTERNALVCC 0x10
#define SSD1306_SWITCHCAPVCC 0x20


void SSD1306Init( void );

void SSD1306SendCommand( uint8_t *data, int i );
void SSD1306SendData( uint8_t *data, int i );
void setAddress( char page, char column );
void clearScreen(void);
void charDraw(char row, char column, int data);
void stringDraw( uint8_t row, uint8_t column, uint8_t *word);
void pixelDraw(char x, char y);
void pixelClear(char x, char y);
void horizontalLine(char xStart, char xStop, char y);
void verticalLine(char x, char yStart, char yStop);
void imageDraw(const char IMAGE[], char row, char column);
//void circleDraw(char x, char y, char radius);
void circleDraw(register int x, register int y, int r);
void circleClear(register int x, register int y, int r);

void Set_Contrast_Control(unsigned char d);
void Set_Inverse_Display(unsigned char d);
void Set_Display_On_Off(unsigned char d);

void Fill_RAM( uint8_t data);
void Fill_RAM_PAGE(unsigned char page, char data);





uint8_t SSD1306_init[] = {
        SSD1306_DISPLAYOFF,
        SSD1306_SETLOWCOLUMN,
        SSD1306_SETHIGHCOLUMN,
        SSD1306_SETSTARTLINE,
        SSD1306_SETCONTRAST,
        0xcf,
        SSD1306_SEGREMAP,
        SSD1306_NORMALDISPLAY,
        SSD1306_SETMULTIPLEX,
        0x3f,
        SSD1306_SETDISPLAYOFFSET,
        0x00,
        SSD1306_SETDISPLAYCLOCKDIV,
        0x80,
        SSD1306_SETPRECHARGE,
        0xf1,
        SSD1306_SETCOMPINS,
        0x12,
        SSD1306_SETVCOMDETECT,
        0x40,
        SSD1306_CHARGEPUMP,
        0x14,
        SSD1306_DISPLAYON
};

void SSD1306Init(void){
    SSD1306SendCommand(SSD1306_init, sizeof SSD1306_init);
}



void setAddress( char page, char column )
{

    uint8_t pageAddress[] = {SSD1306_PAGE_START_ADDRESS};
    uint8_t columnAddress[] = { SSD1306_COLUMNADDRESS, SSD1306_COLUMNADDRESS_MSB, SSD1306_COLUMNADDRESS_LSB };

    if (page > SSD1306_MAXROWS)
    {
        page = SSD1306_MAXROWS;
    }

    if (column > SSD1306_LCDWIDTH)
    {
        column = SSD1306_LCDWIDTH;
    }

    pageAddress[0] = SSD1306_PAGE_START_ADDRESS | (SSD1306_MAXROWS - page);

    columnAddress[0] = SSD1306_COLUMNADDRESS;
    columnAddress[1] = SSD1306_COLUMNADDRESS_MSB | column;
    columnAddress[2] = SSD1306_COLUMNADDRESS_LSB;

    //__no_operation();

    SSD1306SendCommand(pageAddress, 1);
    SSD1306SendCommand(columnAddress, 3);

}

void clearScreen(void)
{
    uint8_t ramData[] = {0x00};

    char i,j;

    for(i=0; i < 8 ;i++)
    {
        setAddress(i,0);

        for(j=0; j < SSD1306_LCDWIDTH; j++)
        {
            SSD1306SendData(ramData, 1);
        }
    }
}


void charDraw(char row, char column, int data)
{
    int h;

    if (row > SSD1306_MAXROWS)
    {
        row = SSD1306_MAXROWS;
    }

    if (column > SSD1306_LCDWIDTH)
    {
        column = SSD1306_LCDWIDTH;
    }

    if ( data < 32 || data > 129 )
    {
        data = '.';
    }

    h = (data - 32) * 6;

    setAddress(row, column);
    SSD1306SendData( (uint8_t *)FONT6x8 + h, 6 );

}


void stringDraw( uint8_t row, uint8_t column, uint8_t *word)
{
    uint8_t a = 0;

    if (row > SSD1306_MAXROWS)
    {
        row = SSD1306_MAXROWS;
    }

    if (column > SSD1306_LCDWIDTH)
    {
        column = SSD1306_LCDWIDTH;
    }

    while (word[a] != 0)
    {
        if (word[a] != 0x0A)
        {
            if (word[a] != 0x0D)
            {
                charDraw(row, column, word[a]);

                //__no_operation();

                column += 6;

                if (column >= (SSD1306_LCDWIDTH - 6 ))
                {
                    column = 0;
                    if ( row < SSD1306_MAXROWS )
                        row++;
                    else
                        row = 0;
                }

            }
            else
            {
                if ( row < SSD1306_MAXROWS )
                    row++;
                else
                    row = 0;
                column = 0;
            }
            a++;
        }
    }
}

void pixelDraw(char x, char y)
{
    char page, temp;
    uint8_t coordinate[] = {0x00};

    if (x > SSD1306_LCDWIDTH - 1)
    {
        x = SSD1306_LCDWIDTH - 1;
    }

    if (y > SSD1306_LCDHEIGHT - 1)
    {
        y = SSD1306_LCDHEIGHT - 1;
    }

    page = y / 8;
    temp = 0x80 >> (y % 8);
    coordinate[0] = temp;

    setAddress(page,x);
    SSD1306SendData(coordinate, 1);

}


//new
void pixelClear(char x, char y)
{
    char page;
    uint8_t coordinate[] = {0x00};

    if (x > SSD1306_LCDWIDTH - 1)
    {
        x = SSD1306_LCDWIDTH - 1;
    }

    if (y > SSD1306_LCDHEIGHT - 1)
    {
        y = SSD1306_LCDHEIGHT - 1;
    }

    page = y / 8;
    setAddress(page,x);
    SSD1306SendData(coordinate, 1);

}


void horizontalLine(char xStart, char xStop, char y)
{
    char temp = 0;
    char page, a;
    uint8_t ramData[] = { 0x00 };

    if (xStart > SSD1306_LCDWIDTH - 1)
    {
        xStart = SSD1306_LCDWIDTH - 1;
    }

    if (xStop > SSD1306_LCDWIDTH - 1)
    {
        xStop = SSD1306_LCDWIDTH - 1;
    }

    if (y > SSD1306_LCDHEIGHT - 1)
    {
        y = SSD1306_LCDHEIGHT - 1;
    }

    if (xStart > xStop)
    {
        temp = xStart;
        xStart = xStop;
        xStop = temp;
    }

    page = y / 8;
    temp = 0x80 >> (y %8);

    a = xStart;
    ramData[0] = temp;

    setAddress(page, xStart);

    while (a <= xStop)
    {
        SSD1306SendData(ramData, 1);
        a++;
    }
}

void verticalLine(char x, char yStart, char yStop)
{
    char temp1 = 0, temp2 = 0;
    char page1, page2, pageStart;
    char a;
    uint8_t ramData1[] = {0x00};
    uint8_t ramData2[] = {0x00};

    if (x > SSD1306_LCDWIDTH - 1)
    {
        x = SSD1306_LCDWIDTH - 1;
    }

    if (yStart > SSD1306_LCDHEIGHT - 1)
    {
        yStart = SSD1306_LCDHEIGHT - 1;
    }

    if (yStop > SSD1306_LCDHEIGHT - 1)
    {
        yStop = SSD1306_LCDHEIGHT - 1;
    }

        if (yStart > yStop)
        {
            temp1 = yStart;
            yStart = yStop;
            yStop = temp1;
        }

    page1 = yStart / 8;
    page2 = yStop / 8;
    pageStart = yStart % 8;

    if (page1 != page2)
    {
        if ( pageStart > 0)
        {
            temp1 = 0xFF00 >> pageStart;
            temp1 = temp1 ^ 0xFF;
        }
        else
        {
            temp1 = 0xFF;
        }
    }
    else
    {
        temp1 = 0;

        a = yStart - (page1 * 8);
        a = 0xFF00 >> a;
        temp2 = temp2 ^ a;
    }

    ramData1[0] = temp1;

    setAddress(page1,x);
    SSD1306SendData(ramData1, 1);

    a = page1 + 1;
    ramData1[0] = 0xFF;

    while (a < page2)
    {
        setAddress(a, x);
        SSD1306SendData(ramData1, 1);
        a++;
    }

    temp2 = 8 - (yStop % 8);
    temp2--;
    temp2 = 0xFF << temp2;

    ramData2[0] = temp2;

    setAddress(page2, x);
    SSD1306SendData(ramData2, 1);
}

void imageDraw(const char IMAGE[], char row, char column)
{
    char a, height, width;

    width = IMAGE[0];
    height = IMAGE[1];

    for ( a = 0; a < height; a++)
    {
        setAddress(row + a, column);
        SSD1306SendData((uint8_t *)IMAGE + 2 + a * width, width);
    }
}

void circleDraw(register int x, register int y, int r)
{
    register int xx = -r;
    register int yy = 0;
    register int e = 2 - (2 * r);
    do {
        pixelDraw(x - xx, y + yy);
        pixelDraw(x - yy, y - xx);
        pixelDraw(x + xx, y - yy);
        pixelDraw(x + yy, y + xx);
        if(e >  xx) e += ((++xx << 1) + 1);
        if(e <= yy) e += ((++yy << 1) + 1);
    } while (xx < 0);
}


//new
void circleClear(register int x, register int y, int r)
{
    register int xx = -r;
    register int yy = 0;
    register int e = 2 - (2 * r);
    do {
        pixelClear(x - xx, y + yy);
        pixelClear(x - yy, y - xx);
        pixelClear(x + xx, y - yy);
        pixelClear(x + yy, y + xx);
        if(e >  xx) e += ((++xx << 1) + 1);
        if(e <= yy) e += ((++yy << 1) + 1);
    } while (xx < 0);
}

void Fill_RAM2( uint8_t data )				//changed to FILL_RAM2 because of conflicting types
{
   unsigned char i,j;

   uint8_t ramData[] = { 0x00};

   ramData[0] = data;

   for(i=0; i < 8 ;i++)
   {
      setAddress(i,0);

      for(j=0; j < SSD1306_LCDWIDTH; j++)
      {
         SSD1306SendData(ramData, 1);
      }
   }
}

void Fill_RAM_PAGE(unsigned char page, char data)
{
   unsigned char j;

   uint8_t ramData[] = { 0x00 };

   ramData[0] = data;

   setAddress(page,0);

   for(j=0; j < SSD1306_LCDWIDTH ;j++)
   {
      SSD1306SendData(ramData, 1);
   }
}

void Set_Contrast_Control(unsigned char d)
{
	uint8_t ramData[] = {SSD1306_SETCONTRAST, 0x00};
    ramData[1] = d;

    SSD1306SendCommand(ramData, 2);
}

void Set_Inverse_Display(unsigned char d)
{
    // 0 = normal, 1 = inverted

    uint8_t ramData[] = {SSD1306_NORMALDISPLAY};
    ramData[0] = ramData[0] + d;

    SSD1306SendCommand(ramData, 1);
}

void Set_Display_On_Off(unsigned char d)
{
    // 0 = off, 1 = on

	uint8_t ramData[] = {SSD1306_DISPLAYOFF};
    ramData[0] = ramData[0] + d;

    SSD1306SendCommand(ramData, 1);
}
