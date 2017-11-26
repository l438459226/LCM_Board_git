
/*-----------------Includes--------------------------------------------------*/

#include "user.h"
#include "usmart.h"
#include "malloc.h"
/*----------------------------------------------------------------------------*/

struct load_pic
{
	u8 pictype;
	u32 color;
	char name[24];		//图片名字	 
};	

struct load_pic loadpic[]	= 
{
/*
 	253,0,"HD_03.bmp",	//255加载的是图片	 20
	254,0,"HD_03.bmp",	//20
	255,0,"HD_03.bmp",
	*/
	0,0x404040,"color",
	12,0,"Img_ColorBar",
	3,0,"Img_Box",
	12,0,"Img_ColorBar",
	5,0,"Img_Box",
	7,0,"Img_Box",
	255,0,"HD_03.bmp",
	0,0x808080,"color",
	3,0,"Img_Box",
	12,0,"Img_ColorBar",
	5,0,"Img_Box",
	7,0,"Img_Box",
};


u8 MsgCmd;
u8 hidr;

u32 frame_max;
u32 frame_delay_ms[32];   //
u32 dip_on = 1;
u32 auto_mode = 0; 
u32 frame = 0;
u32 frame_delay_cnt = 0;
void Pic_switch(void);


u8 pic_load(u8 Display)
{
	 u8 i;
	 u8 r,g,b;
	 u8 picnum;
	
	 picnum = sizeof(loadpic)/sizeof(struct load_pic);
	 printf("picnum:%d\n",picnum);
	 for(i=0;i<picnum;i++)
	 {
	 	LcdDrvSetFrame(i);
	 	switch(loadpic[i].pictype)
		{
		   case 0:
				b = loadpic[i].color;
				g = loadpic[i].color>>8;
				r = loadpic[i].color>>16;
				Img_Full(r,g,b);		//
		   break;

		   case 1:	Img_CT();break;

		   case 2:	Img_Chcker58();	break;

		   case 3:	Img_Box(); break;

		   case 4:	Img_Gray256_V(); break;

		   case 5:	Img_Gray256_H(); break;

		   case 6:	Img_RED256_H();	break;

		   case 7:	Img_GREEN256_H(); break;

		   case 8:	Img_BLUE256_H(); break;

		   case 9:	Img_RED256_V();	break;

		   case 10:	Img_GREEN256_V(); break;

		   case 11:	Img_BLUE256_V(); break;

		   case 12:	Img_ColorBar();	break;

		   case 13:	Img_ColorBar(); break;

		   case 14:	Flicker(); break;

		   case 15:	MAX_Current(); break;

		   case 16:	two_windows(); break;

		   case 253:OutPutSdCardFileRollingover(loadpic[i].name); break;

		   case 254:OutPutSdCardFileOverturn(loadpic[i].name); break;

		   case 255:OutPutSdCardFile(loadpic[i].name); break;

		   default :  	break;
		}
	 	if(Display)	{LcdDrvDisplayFrame(i);	frame_delay_ms[i] = 1000;}
	 }
	 return picnum;
}


/*******************************************************************************
*??? : main
*??   : ???
*??   : ?
*??   : ?
*******************************************************************************/
int main(void)
{
	u8 i;
//	int nn;
	u8 *srambuf;
	u8 datemp;

	SystemInit();	          //
	Usart_Init(115200);
	printf("OK\r\n");

	my_mem_init(SRAMIN);

 	LCDGPIO_Configuration();	  //GPIO配置
 	NVIC_Configuration();		//SD卡中断
	delay_init(72);			 //SysTick_Init();
	usmart_dev.init(72);	//usmart_dev.scan();	调试组件
	
	Key_Init(ENCODER_DI);  	//初始化按键
   	Delay_ms(200);
	SDcard_init();		//SD Init  
	LCD_SSD_SET();		//设置lcd porch值
  	DisplayON_PanelInit();	  //初始化SSD2828  初始化模组
	printf("ssd:0x%x\r\n",ssd_rdata(0xb0));
	usb_hid_mass_init();	//hid
	Delay_ms(800);	
	
	frame_max = pic_load(1); //加载图片时显示图片

	datemp = my_mem_perused(SRAMIN);
	printf("perused1:%d",i);
	printf("perused2:%d",datemp);

	srambuf = mymalloc(SRAMIN,1024*20);	
	i = my_mem_perused(SRAMIN);
	printf("i perused:%d",i);
	myfree(SRAMIN,srambuf);
   
	datemp = my_mem_perused(SRAMIN);
	printf("free perused:%d",datemp);
	
	SD_Init();	
	printfk(0,0,FONT1632,GREEN,RED,1,"ssd:0x%x ",ssd_rdata(0xb0));
	printfk(0,1,FONT1632,GREEN,RED,1,"mem_perused:%d ",i);
	printfk(0,2,FONT1632,GREEN,RED,1,"mem_perused:%d ",datemp);
	while (1)
	{
		usmart_dev.scan();
		Pic_switch();
		Delay_ms(20);
	}
}

void Pic_switch(void)
{
			Key_Read();			//
			switch(KeyTrg)
			{
				case KEY_4: // KEY_4  Display ON/OFF
					if(dip_on == 0)
					{			      //Display ON 		
						frame_delay_cnt = 0;
						DisplayON_PanelInit();	//
						dip_on = 1;
						uTimerH=0;
						uTimerL=0;
						picDisBeforeBurn=1;
						printfk(2,3,FONT2448,GREEN,RED,1,"lizhongjillo!!@$%^*");
					}else{	//Display OFF
						dip_on = 0;
						DisplayOFF();  //
						LcdDrvDisplayFrame(frame = 0);
						BurnStart=0;
						picDisBeforeBurn=0;
					}
				  auto_mode = 0; //
					break;	

				case KEY_3:	   //
					if((auto_mode == 1) || (dip_on == 0)){break;} //
					if(dip_on == 0){break;} //
					if(frame < (frame_max - 1)){frame++;}
					else {frame = 0;}
						LcdDrvDisplayFrame(frame);
					break;
	
				case KEY_2:	  //
					if((auto_mode == 1) || (dip_on == 0)){break;} //
				    // exit_sleep();
				    if(dip_on == 0){break;} //
					if(frame > 0){frame--;}
					else {frame = frame_max-1;}
					LcdDrvDisplayFrame(frame);
					break;
					
				case KEY_1:			//
					if(auto_mode == 1) auto_mode=0;
					else    auto_mode=1;
					break;	
			}
		if((auto_mode == 1) && (_1ms_ok == 1) && (dip_on == 1)) 
		{
		  _1ms_ok = 0;
			if(++frame_delay_cnt >= frame_delay_ms[frame])  
			{
			    frame_delay_cnt = 0;
				if(frame < (frame_max - 1)){frame++;}
				else {frame = 0;}
				LcdDrvDisplayFrame(frame);	
			}
		}
}

