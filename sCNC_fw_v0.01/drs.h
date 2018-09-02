#ifndef	_DRS_H
#define	_DRS_H
#include "typedef.h"

void ChkOnOffKey(void);
void tDrsMain(void);
void ChkAdValue(void);
word GetADC(byte c);
void  DoErr(void);
void  DoOff(void);
void  ChkHotWaterTimeout(void);
void  ChkAddWaterTimeout(void);
void  ChkErrAlarmTimeout(void);

void	SetRlyAddWater(byte c);
void	SetRlyAddHot(byte c);	
void    SetSpk(byte c);
void	SetLedLwt(byte c);
void   	SetLedRun(byte c);
void   	SetLedPwr(byte c);
void	SetLedErr(byte c);	
	
void    SetAllOff(void);

void	ChkLowWater(void);
void	ChkHighWater(void);
void	ChkHotWater(void);
void	DRS_Init(void);

#endif	/* _DRS_H */



