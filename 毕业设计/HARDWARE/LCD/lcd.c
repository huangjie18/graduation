#include "lcd.h"
#include "stdlib.h"
#include "lcdfont.h"
#include "delay.h"

u8 LCD_GRAM[160][256];
u8 gram[160][128];

/*********************更新160*128显存******************/
void LCD_Refresh_Gram(void)
{
	u8 i, n;
	LCD_WR_command(0x2c);    //开启GRAM 自动增加地址
	for(i=0;i<160;i++)
	{
		for(n=0;n<128;n++)
		{
			if(gram[i][n])
			{
					LCD_WR_data(0xff,0xff);
			}
			else
			{
					LCD_WR_data(0xff,0xff);
			}  //每个像素需要16bit
		}
	}
}

/********************写命令*********************/
void LCD_WR_command(u8 addr1)
{
	u16 i, addr2 = 0,temp;
	for(i=0;i<=7;i++)  //颠倒顺序
	{
		addr2 = addr2 << 1;
		addr2 = addr2 | (addr1 & 1);
		addr1 = addr1 >> 1;
	}
	addr2=addr2<<8;
	RD = 1;
	RS = 0;
	CS = 0;
	WR = 1;
	temp = (GPIOE->IDR&0xff00);  //提取引脚的现有数据
	GPIOE->BSRRH = temp;     //全部清零
	GPIOE->BSRRL = addr2;	//写入数据
	WR = 0;
	WR = 1;
	
	CS = 1;
}

/*********************写命令参数*********************/
void LCD_WR_para(u8 para)
{
	u16 i, addr2 = 0,temp;
	for(i=0;i<=7;i++)
	{
		addr2 = addr2 << 1;
		addr2 = addr2 | (para & 1);
		para = para >> 1;
	}
	addr2=addr2<<8;
	RD = 1;
	RS = 1;
	CS = 0;
	WR = 1;
	temp = (GPIOE->IDR&0xff00);
	GPIOE->BSRRH = temp;
	GPIOE->BSRRL = addr2;	
	WR = 0;
	WR = 1;
	CS = 1;
}

/********************写数据**************************/
void LCD_WR_data(u8 data_1,u8 data_2)
{
	u16 i,j, addr2 = 0,addr3 = 0,temp2,temp3;
	for(i=0;i<=7;i++)
	{
		addr2 = addr2 << 1;
		addr2 = addr2 | (data_1 & 1);
		data_1 = data_1 >> 1;
	}
	for(j=0;j<=7;j++)
	{
		addr3 = addr3 << 1;
		addr3 = addr3 | (data_2 & 1);
		data_2 = data_2 >> 1;
	}
	addr2 = addr2<<8;
	addr3 = addr3<<8;
	RD = 1;
	RS = 1;
	
	CS = 0;
	WR = 1;
	temp2 = (GPIOE->IDR&0xff00);
	GPIOE->BSRRH = temp2;
	GPIOE->BSRRL = addr2;
	WR = 0;                    //第一个数据
	WR = 1;
	CS = 1;
	
	CS = 0;
	WR = 1;
	temp3 = (GPIOE->IDR&0xff00);
	GPIOE->BSRRH = temp3;
	GPIOE->BSRRL = addr3;
	WR = 0;                 //第二个数据
	WR = 1;
	CS = 1;
}

/*********************清屏******************/
void LCD_Clear(void)
{
	u8 i;u16 n;
	for(i=0;i<160;i++)
	{
		for(n=0;n<128;n++)
		{
			gram[i][n] = 0x00;
		}
	}
	LCD_Refresh_Gram();
}

 /***************************显示三色表格**************/
 void LCD_DrawPoint()
{
	
	u8 i=0,j=0,temp=0,a=0,temp1 =0;
	LCD_WR_command(0x2a);              //Horizontal Address Start Position
	LCD_WR_para(0x00);
	LCD_WR_para(0x00);
	LCD_WR_para(0x00);
	LCD_WR_para(0x7f);
	LCD_WR_command(0x2b);              //Vertival Address end Position
	LCD_WR_para(0x00);
	LCD_WR_para(0x22);
	LCD_WR_para(0x00);
	LCD_WR_para(0x62);
	LCD_WR_command(0x2c);
	 for(i=0;i<64;i++)
	 {
		 if(i<21)
		 {
			for(j=0;j<128;j++)
			 {
				LCD_WR_data(0xf8,0x00); 
			 }
		 }
		 else if(i<42)
		 {
			 for(j=0;j<128;j++)
			 {
				 LCD_WR_data(0x07,0xe0);
			 }
		 }
		 else
		 {
			 for(j=0;j<128;j++)
			 {
				 LCD_WR_data(0x00,0x1f);
			 }
		 }
	 }
 }

 /***********************显示图片************************/
 void lcd_showpic(void)
 {
	u8 b,temp,count;
	u16 i=0,a=0;
	LCD_WR_command(0x2a);              //Horizontal Address Start Position
	LCD_WR_para(0x00);
	LCD_WR_para(0x00);
	LCD_WR_para(0x00);
	LCD_WR_para(0x7f);
	LCD_WR_command(0x2b);              //Vertival Address end Position
	LCD_WR_para(0x00);
	LCD_WR_para(0x22);
	LCD_WR_para(0x00);
	LCD_WR_para(0x62);
	LCD_WR_command(0x2c);
	 for(i=0;i<8960;i+=2)
	 {
		LCD_WR_data(gImage_1[i],gImage_1[i+1]);
	 }
	LCD_qu(0,35,31,64);
	for(a=0;a<464;a++)
	{
		temp = wuyidaxue[a];
		if(count>115)
		{
			b++;
			LCD_qu(b*32,35,(b+1)*32-1,64);
			count = 0;
		}
		count++;
		for(i=0;i<8;i++)
		{
				
			if(temp&0x01) LCD_WR_data(0x00,0x00);
			else LCD_WR_data(0X99,0XC6);
			temp >>= 1;
		}
	}
 }
/***********************显示字符串***********************/
void LCD_Showstring(u8 x,u8 y,const u8 *p)
{
	u8 j,a,temp,c,chr;
	while((*p<='~')&&(*p>=' '))
	{
		chr = *p;
		chr=chr-' ';
		for(j=0;j<16;j++)
		{
			temp = PIC3[chr][j];
			for(a=0;a<8;a++)
			{
				c = 0;
				c = temp&(0x01<<a);
				if(c)
				{
					gram[y+j][x+a] = 1;
				}
				else
				{
					gram[y+j][x+a] = 0;
				}
			}
		}
		p++;
		x = x+8;
	}
}
/******m^n的值******************/
u32 mypow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}				  

/******************显示数字***********/
void LCD_num(u8 x,u8 y,double num,u8 dec)
{
	u8 temp,num1,j,a,c,e;
	u32 num2;
	if((int)num == num)e=1;
	else e=0;
	if(e)
	{
		if(num<=9)
		{
			num1 = num+'0'-' ';
			for(j=0;j<16;j++)
			{
				temp = PIC3[num1][j];
				for(a=0;a<8;a++)
				{
					c = 0;
					c = temp&(0x01<<a);
					if(c)
					{
						gram[y+j][x+a] = 1;
					}
					else
					{
						gram[y+j][x+a] = 0;
					}
				}
			}
		}
		else
		{
			u8 b,temp1,count=0;
			u32 i=1;
			do
			{
				++count;
				i *= 10;
			}while(i<=num);
			for(b=0;b<count;b++)
			{
				temp1=((int)num/mypow(10,count-b-1))%10;
				num1 = temp1+'0'-' ';
				for(j=0;j<16;j++)
				{
					temp = PIC3[num1][j];
					for(a=0;a<8;a++)
					{
						c = 0;
						c = temp&(0x01<<a);
						if(c)
						{
							gram[y+j][x+a] = 1;
						}
						else
						{
							gram[y+j][x+a] = 0;
						}
					}
				}
				x += 8;
			}
		}
	}
	else
	{
		u8 f,t;
		num2 = (int)num;
		if(num2<=9)
		{
			num1 = num2+'0'-' ';
			for(j=0;j<16;j++)
			{
				temp = PIC3[num1][j];
				for(a=0;a<8;a++)
				{
					c = 0;
					c = temp&(0x01<<a);
					if(c)
					{
						gram[y+j][x+a] = 1;
					}
					else
					{
						gram[y+j][x+a] = 0;
					}
				}
			}
			x += 8;
		}
		else
		{
			u8 b,temp1,count=0;
			u32 i=1;
			do
			{
				++count;
				i *= 10;
			}while(i<=num2);
			for(b=0;b<count;b++)
			{
				temp1=(num2/mypow(10,count-b-1))%10;
				num1 = temp1+'0'-' ';
				for(j=0;j<16;j++)
				{
					temp = PIC3[num1][j];
					for(a=0;a<8;a++)
					{
						c = 0;
						c = temp&(0x01<<a);
						if(c)
						{
							gram[y+j][x+a] = 1;
						}
						else
						{
							gram[y+j][x+a] = 0;
						}
					}
				}
				x += 8;
			}
		}
		for(j=0;j<16;j++)
		{
			temp = PIC3[14][j];
			for(a=0;a<8;a++)
			{
				c = 0;
				c = temp&(0x01<<a);
				if(c)
				{
					gram[y+j][x+a] = 1;
				}
				else
				{
					gram[y+j][x+a] = 0;
				}
			}
		}
		x += 8;
		for(f=0; f<dec; f++)
		{
			t = (int)(num*mypow(10,f+1))%10;
			num1=t+'0'-' ';
			for(j=0; j<16; j++)
			{
				temp = PIC3[num1][j];
				for(a=0; a<8; a++)
				{
					c = 0;
					c = temp&(0x01<<a);
					if(c)
					{
						gram[y+j][x+a] = 1;
					}
					else
					{
						gram[y+j][x+a] = 0;
					}
				}
			}
			x += 8;
		}

	}
}
	
	
/************开区******************/
void LCD_qu(u8 x,u8 y,u8 xend,u8 yend)
{
	
	LCD_WR_command(0x2a);    //Column address set 左右宽度
	LCD_WR_para(0x00);
	LCD_WR_para(x);
	LCD_WR_para(0x00);
	LCD_WR_para(xend);
	LCD_WR_command(0x2b);    //Row Address Set   上下宽度
	LCD_WR_para(0x00);
	LCD_WR_para(y+0x22);     //去掉不显示部分
	LCD_WR_para(0x00);
	LCD_WR_para(yend+0x22);
	LCD_WR_command(0x2c);
}
/************显示字符************/
void showchar(u8 x,u8 y,u8 num)
{
	u8 i,j,temp;
	LCD_qu(x,y,x+8-1,y+16-1);      //设置字体的大小
	for(j=0;j<16;j++)
	{
		temp = PIC3[num][j];  //提取对应字符字模数据
		for(i=0;i<8;i++)     //八位二进制数据
		{
			if(y<21)
			{
				if(temp&0x01) LCD_WR_data(0x00,0x00);
				else LCD_WR_data(0xf8,0x00);
				temp >>= 1;
			}
			else if(y<42)
			{
				if(temp&0x01) LCD_WR_data(0x00,0x00);
				else LCD_WR_data(0x07,0xe0);
				temp >>= 1;
			}
			else
			{
				if(temp&0x01) LCD_WR_data(0x00,0x00);
				else LCD_WR_data(0x00,0x1f);
				temp >>= 1;
			}
		}
	}

}
/************显示字符串********16*8的数组*****/
void showstring(u8 x,u8 y,const u8 *p)
{
	u8 chr;
	while((*p<='~')&&(*p>=' '))
	{
		chr = *p;
		chr=chr-' ';
		showchar(x,y,chr);   
		p++;
		x+=8;
	}
}

/************显示数字***********/
void showshuzi(u8 x,u8 y,u16 num)
{
	u8 b,temp1,count=0,num1;
	u32 i=1;
	qingqu(x,y);
	do
	{
		++count;
		i *= 10;
	}while(i<=num);    //统计数据一共有多少位
	for(b=0;b<count;b++)
	{
		temp1=((int)num/mypow(10,count-b-1))%10; //提取每一位
		num1 = temp1+'0'-' '; //提取字模数组中相对应数据
		showchar(x,y,num1);  //显示字符
		x+=8;
	}
}
/**************数字清区*******************/
//清除四个字节的大小和重新写背景色
void qingqu(u8 x,u8 y)
{
	u8 i,j;
	LCD_qu(x,y,x+32-1,y+16-1);
	if(y<21)
	{
		for(j=0;j<16;j++)
		for(i=0;i<32;i++)
		{
			LCD_WR_data(0xf8,0x00);
		}
	}
	else if(y<42)
	{
		for(j=0;j<16;j++)
		for(i=0;i<32;i++)
		{
			LCD_WR_data(0x07,0xe0);
		}
	}
	else
	{
		for(j=0;j<16;j++)
		for(i=0;i<32;i++)
		{
			LCD_WR_data(0x00,0x1f);
		}		
	}
}

/****************LCD初始化命令*******************/
 void LCD_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOE, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|
	GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOE,&GPIO_InitStructure);//初始化GPIO
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10;
	GPIO_Init(GPIOD,&GPIO_InitStructure);
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	RST = 0;    //复位
	delay_ms(20);  //复位延时
	RST = 1;
	delay_ms(100);
	
	LCD_WR_command(0x11);          //Sleep out
	delay_ms(120);
	
	LCD_WR_command(0xb1);          //In normal mode
	LCD_WR_para(0x01);
	LCD_WR_para(0x2c);
	LCD_WR_para(0x2d);
	
	LCD_WR_command(0xb2);					//In Idle mode
	LCD_WR_para(0x01);
	LCD_WR_para(0x2c);
	LCD_WR_para(0x2d);
	
	LCD_WR_command(0xb3);					//In partial mode
	LCD_WR_para(0x01);
	LCD_WR_para(0x2c);
	LCD_WR_para(0x2d);
	LCD_WR_para(0x01);
	LCD_WR_para(0x2c);
	LCD_WR_para(0x2d);
	
    LCD_WR_command(0xb4);              //Display inversion  control
    LCD_WR_para(0x07);

    LCD_WR_command(0xc0);              //Power control setting
    LCD_WR_para(0xa2);
    LCD_WR_para(0x02);
    LCD_WR_para(0x84);
	      
    LCD_WR_command(0xc1);              //Power control setting
    LCD_WR_para(0xc5);

    LCD_WR_command(0xc2);              //In normal mode
    LCD_WR_para(0x0a);
    LCD_WR_para(0x00);

    LCD_WR_command(0xc3);              //In Idle mode
    LCD_WR_para(0x8a);
    LCD_WR_para(0x2a);

    LCD_WR_command(0xc4);              //In partial mode
    LCD_WR_para(0x8a);
    LCD_WR_para(0xee);

    LCD_WR_command(0xc5);              //VCOM control 1
    LCD_WR_para(0x0e);	//4f

    LCD_WR_command(0x36);              //Memory data access control扫描方式
   //LCD_WR_para(0x00);				//竖屏
	LCD_WR_para(0x60);				//横屏							       
    LCD_WR_command(0x26);       //Gamma curve select
    LCD_WR_para(0x04);
    
    LCD_WR_command(0xe0);              //Driver Output Control(1)
    LCD_WR_para(0x0f);
    LCD_WR_para(0x1a);
    LCD_WR_para(0x0f);
    LCD_WR_para(0x18);
    LCD_WR_para(0x2f);
    LCD_WR_para(0x28);
    LCD_WR_para(0x20);
    LCD_WR_para(0x22);
    LCD_WR_para(0x1f);
    LCD_WR_para(0x1b);
    LCD_WR_para(0x23);
    LCD_WR_para(0x37);
    LCD_WR_para(0x00);
    LCD_WR_para(0x07);
    LCD_WR_para(0x07);
    LCD_WR_para(0x10);
	     
    LCD_WR_command(0xe1);              //Driver Output Control(1)
    LCD_WR_para(0x0f);
    LCD_WR_para(0x1b);
    LCD_WR_para(0x0f);
    LCD_WR_para(0x17);
    LCD_WR_para(0x33);
    LCD_WR_para(0x2c);
    LCD_WR_para(0x29);
    LCD_WR_para(0x2e);
    LCD_WR_para(0x30);
    LCD_WR_para(0x30);
    LCD_WR_para(0x39);
    LCD_WR_para(0x3f);
    LCD_WR_para(0x00);
    LCD_WR_para(0x07);  
    LCD_WR_para(0x03);  
    LCD_WR_para(0x10);     
   
    LCD_WR_command(0xf0);              //LCD Driveing control
    LCD_WR_para(0x01);     

    LCD_WR_command(0xf6);              //LCD Driveing control
    LCD_WR_para(0x00);     

    LCD_WR_command(0x3a);         //65k mode      
    LCD_WR_para(0x05);  //

    LCD_WR_command(0x13); 		//Partial off
	
//	LCD_WR_command(0x20);  
//	LCD_WR_command(0x21);	//display inversion on
    LCD_WR_command(0x29);   //Display on
    delay_ms(40);
	LCD_WR_command(0x2a);    //Column address set 左右宽度
	LCD_WR_para(0x00);
	LCD_WR_para(0x00);
	LCD_WR_para(0x00);
	LCD_WR_para(0x7f);
	LCD_WR_command(0x2b);    //Row Address Set   上下宽度
	LCD_WR_para(0x00);
	LCD_WR_para(0x00);
	LCD_WR_para(0x00);
	LCD_WR_para(0x9f);

	LCD_Clear();
	delay_ms(20);
}
