CC = iccavr
CFLAGS =  -IC:\icc\include\ -e -DATMega8  -l -g -Mavr_enhanced_small -Wa-W 
ASFLAGS = $(CFLAGS)  -Wa-g
LFLAGS =  -LC:\icc\lib\ -g -Wl-W -bfunc_lit:0x26.0x2000 -dram_end:0x45f -bdata:0x60.0x45f -dhwstk_size:16 -beeprom:1.512 -fihx_coff -S2
FILES = main.o drs.o serial.o 

drs:	$(FILES)
	$(CC) -o drs $(LFLAGS) @drs.lk  
main.o: C:/icc/include/iom8v.h C:/icc/include/macros.h C:\DOCUME~1\awang\Desktop\Aixi_个人\myCNC\sCNC\fw\v0.01/typedef.h C:\DOCUME~1\awang\Desktop\Aixi_个人\myCNC\sCNC\fw\v0.01/main.h C:\DOCUME~1\awang\Desktop\Aixi_个人\myCNC\sCNC\fw\v0.01/typedef.h\
 C:\DOCUME~1\awang\Desktop\Aixi_个人\myCNC\sCNC\fw\v0.01/serial.h C:\DOCUME~1\awang\Desktop\Aixi_个人\myCNC\sCNC\fw\v0.01/typedef.h C:\DOCUME~1\awang\Desktop\Aixi_个人\myCNC\sCNC\fw\v0.01/drs.h\
 C:\DOCUME~1\awang\Desktop\Aixi_个人\myCNC\sCNC\fw\v0.01/typedef.h
main.o:	C:\DOCUME~1\awang\Desktop\Aixi_个人\myCNC\sCNC\fw\v0.01\main.c
	$(CC) -c $(CFLAGS) C:\DOCUME~1\awang\Desktop\Aixi_个人\myCNC\sCNC\fw\v0.01\main.c
drs.o: C:/icc/include/iom8v.h C:/icc/include/macros.h C:\DOCUME~1\awang\Desktop\Aixi_个人\myCNC\sCNC\fw\v0.01/typedef.h C:\DOCUME~1\awang\Desktop\Aixi_个人\myCNC\sCNC\fw\v0.01/serial.h C:\DOCUME~1\awang\Desktop\Aixi_个人\myCNC\sCNC\fw\v0.01/typedef.h\
 C:\DOCUME~1\awang\Desktop\Aixi_个人\myCNC\sCNC\fw\v0.01/main.h C:\DOCUME~1\awang\Desktop\Aixi_个人\myCNC\sCNC\fw\v0.01/typedef.h C:\DOCUME~1\awang\Desktop\Aixi_个人\myCNC\sCNC\fw\v0.01/drs.h\
 C:\DOCUME~1\awang\Desktop\Aixi_个人\myCNC\sCNC\fw\v0.01/typedef.h
drs.o:	C:\DOCUME~1\awang\Desktop\Aixi_个人\myCNC\sCNC\fw\v0.01\drs.c
	$(CC) -c $(CFLAGS) C:\DOCUME~1\awang\Desktop\Aixi_个人\myCNC\sCNC\fw\v0.01\drs.c
serial.o: C:/icc/include/iom8v.h C:/icc/include/macros.h C:\DOCUME~1\awang\Desktop\Aixi_个人\myCNC\sCNC\fw\v0.01/typedef.h C:\DOCUME~1\awang\Desktop\Aixi_个人\myCNC\sCNC\fw\v0.01/serial.h\
 C:\DOCUME~1\awang\Desktop\Aixi_个人\myCNC\sCNC\fw\v0.01/typedef.h C:\DOCUME~1\awang\Desktop\Aixi_个人\myCNC\sCNC\fw\v0.01/main.h C:\DOCUME~1\awang\Desktop\Aixi_个人\myCNC\sCNC\fw\v0.01/typedef.h
serial.o:	C:\DOCUME~1\awang\Desktop\Aixi_个人\myCNC\sCNC\fw\v0.01\serial.c
	$(CC) -c $(CFLAGS) C:\DOCUME~1\awang\Desktop\Aixi_个人\myCNC\sCNC\fw\v0.01\serial.c
