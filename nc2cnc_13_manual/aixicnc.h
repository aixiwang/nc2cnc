#if !defined(AIXICNC_H)
#define AIXICNC_H

void aixiCNC(char* str_nc);

//byte LoadMonoBmp(void);
//void process_bmp_data(void);
//byte isBlackPoint(dword x, dword y);
//byte find_new_black_pt(long* px1, long* py1);
//byte find_next_black_pt(long x1, long  y1, long* px2, long* py2);


byte LoadNcCode(char* str_nc1);
void NcCodeStateMachine(unsigned char c);

#endif