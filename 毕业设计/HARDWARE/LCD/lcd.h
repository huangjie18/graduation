#ifndef __LCD_H
#define __LCD_H
#include "sys.h"
#include "stdlib.h"

#define RST PBout(15)
#define RS  PDout(10)
#define WR  PDout(9)
#define RD  PDout(8)
#define DB0 PEout(15)
#define DB1 PEout(14)
#define DB2 PEout(13)
#define DB3 PEout(12)
#define DB4 PEout(11)
#define DB5 PEout(10)
#define DB6 PEout(9)
#define DB7 PEout(8)
#define CS  PEout(7)


/*****************函数声名***********************************/
void LCD_Refresh_Gram(void);
 void lcd_showpic(void);
void LCD_num(u8 x,u8 y,double num,u8 dec);
u32 mypow(u8 m,u8 n);
void LCD_WR_command(u8 addr1);
u32 num_panduan(u32 num);
void LCD_Showstring(u8 x,u8 y,const u8 *p);
void LCD_WR_para(u8 para);
void LCD_WR_data(u8 data_1,u8 data_2);
void LCD_Clear(void);
void LCD_Init();
void LCD_DrawPoint();
void LCD_qu(u8 x,u8 y,u8 xend,u8 yend);
void showchar(u8 x,u8 y,u8 num);
void showstring(u8 x,u8 y,const u8 *p);
void showshuzi(u8 x,u8 y,u16 num);
void showchar2(u8 x,u8 y,u16 num);
void qingqu(u8 x,u8 y);
#endif



