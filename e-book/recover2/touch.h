#ifndef _TOUCH_H_
#define _TOUCH_H_
int  touch_open(void);
int touch_close(void);
int get_touch(void);
struct point get_touch_with_point(void);

struct point{
	int x,y;
	
}start,end;


#endif

