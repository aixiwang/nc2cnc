#include <windows.h>
#include <stdio.h>
#include "winio.h"
#include "aixicnc.h"

char nc_filename[80];

//void main()
int main(int argc, char* argv[])
{
 // DWORD dwPortVal;
 // DWORD dwMemVal;
  bool bResult;
 // HANDLE hPhysicalMemory;
 // PBYTE pbLinAddr;

  // Call InitializeWinIo to initialize the WinIo library.

  //bResult = InitializeWinIo();

  //if (bResult)
  //{
    // Under Windows NT/2000/XP, after calling InitializeWinIo,
    // you can call _inp/_outp instead of using GetPortVal/SetPortVal

//    GetPortVal(0x378, &dwPortVal, 4);
//    SetPortVal(0x378, 10, 4);
	//printf("argc = %d, %s, %s\n", argc, argv[0], argv[1]);

	if (argc == 2)
	{
		strcpy(nc_filename, argv[1]);

	}
	else
		strcpy(nc_filename, "cnc.nc");

    aixiCNC(nc_filename);
	return 0;
/*
    // Map physical addresses 0xA0000 - 0xAFFFF into the linear address space
    // of the application. The value returned from the call to MapPhysToLin is
    // a linear address corresponding to physical address 0xA0000. In case of
    // an error, the return value is NULL.

    pbLinAddr = MapPhysToLin((PBYTE)0xA0000, 65536, &hPhysicalMemory);

    if (pbLinAddr)
    {
      // Now we can use pbLinAddr to access physical address 0xA0000

      *pbLinAddr = 10;

      // When you're done with pbLinAddr, call UnmapPhysicalMemory

      UnmapPhysicalMemory(hPhysicalMemory, pbLinAddr);
    }

    // Instead of using MapPhysToLin, we can use GetPhysLong/SetPhysLong

    GetPhysLong((PBYTE)0xA0000, &dwMemVal);

    SetPhysLong((PBYTE)0xA0000, 10);

    // When you're done using WinIo, call ShutdownWinIo

  */
	

    //ShutdownWinIo();
  //}
  //else
  //{
  //  printf("Error during initialization of WinIo.\n");
  //  exit(1);
  //}
}

