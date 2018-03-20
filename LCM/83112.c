
#include "user.h"
#include "ssd2828.h"

#ifdef TD4310

void SSD_init()
{
	
	
}





void LCD_SSD_SET(void)
{
	Lcd_MODE = MIPI_SINGLE;   //
	Lcd_LCDH = 1080;		  //
	Lcd_LCDV = 2160;	     
	Lcd_HBPD = 40;		      
	Lcd_HFPD = 110; 
	Lcd_HSPW = 2;

	Lcd_VBPD = 12; 
	Lcd_VFPD = 25; 
	Lcd_VSPW = 2;  
	
	Lcd_fps = 56;
	Dsi_LANE = 4;
  Dsi_mbps = 0;
	Lcd_DCLK = (Lcd_fps * ((Lcd_LCDH + Lcd_HBPD + Lcd_HFPD + Lcd_HSPW) * (Lcd_LCDV + Lcd_VBPD + Lcd_VFPD + Lcd_VSPW)))/1000000;

	Lcd_DCLK_EDGE = DCLK_RISING_EDGE;         //上升沿锁存数据	  
	Lcd_HSYNC_POLARITY = HSYNC_ACTIVE_LOW;    //HSYNC脉冲低电平有效  
	Lcd_VSYNC_POLARITY = VSYNC_ACTIVE_LOW;    //VSYNC脉冲低电平有效  
	Lcd_DE_POLARITY = DE_ACTIVE_HIGH;         //DE信号低电平有效;	 	   
	LcdDrvInit();	
}

void LCD_INIT(void)
{
	 u8 regdat;

	//Generic_Long_Write_3P(0xFF,0x98,0x81,0x00);
	Generic_Short_Write_1P(0x35,0x00);
	Generic_Short_Write_1P(0x51,0x80);   //CABC
	Generic_Short_Write_1P(0x53,0x24);
	Generic_Short_Write_1P(0x55,0x00);
	
	Generic_Short_Write_1P(0xB0,0x04);
	Generic_Long_Write_5P(0xDE,0x01,0x00,0x0F,0xFF,0x05);
	
	DCS_Short_Write_NP(0x11);
	Delay_ms(150);
	DCS_Short_Write_NP(0x29);
	Delay_ms(50);
	
	DCS_Short_Read_NP(0x0a,1,&regdat);
	printf("lcm ID:0x%x\r\n",regdat);
}

#endif

