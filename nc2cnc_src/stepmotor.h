//-------------------------------------------------
// Copyright by Aixi Wang (aixi.wang@hotmail.com)
//
//-------------------------------------------------


#if !defined(STEPMOTOR_H)
#define STEPMOTOR_H

#include "typedef.h"

void xyz_disable(void);
void xyz_enable(void);


void x_go_um(dword um, byte direction);
void y_go_um(dword um, byte direction);
void z_go_um(dword um, byte direction);

void set_x_mm_pmin(dword mms);
void set_y_mm_pmin(dword mms);
void set_z_mm_pmin(dword mms);

//void (long abs_x, long abs_y, long abs_z);
void myDelayUs(long n_us);
void zmotor_disable(void);
void zmotor_enable(void);
void goto_abs_pt_pt_um(long x1,long y1, long x2, long y2);
void goto_abs_pt_pt_um_3d(long x1,long y1, long z1, long x2,long y2, long z2);


#endif