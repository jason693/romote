#ifndef _LCD_H_
#define _LCD_H_

int lcd_open(void);
int lcd_close(void);
void lcd_draw_point(int x,int y,int color);
void lcd_draw_block(int x,int y,int length,int high,int color);
void lcd_draw_full_circle(int x,int y,int r,int color);
void lcd_draw_empty_circle(int x,int y,int r,int color);
void draw_word(unsigned char *name,int x,int y, int len,int size,int color);

#endif 

