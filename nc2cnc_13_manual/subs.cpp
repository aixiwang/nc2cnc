//---------------------------------
// misc routines
//---------------------------------
//#include "stdafx.h"
//#include "winsock2.h" 

#include "subs.h"





HANDLE Serial;

//---------------------------
// AlarmBeep
//---------------------------
UINT AlarmBeep(void *pArg)
{
	int i,j;
	//while(bJaanState != JA_CEFANG){
	//if ()
	//{
	for(j=0; j<10; j++)
	{
		for (i = 800; i<1500; i=i+50)
			::Beep(i, 10);

		for (i = 1500; i>800; i=i-50)
			::Beep(i, 10);
	//}		
	//}
	}

	//bitAlarmBeepRun = 0;
	return 1;
}



//---------------------------
// dump_ram_to_file
//---------------------------
int
dump_ram_to_file(char * str_filename, byte * addr, dword bytes)
{
   FILE    * fp;
   dword   i;
   byte    c;

   if ( !( fp=fopen(str_filename, "wb")) )
      {  printf("Open file %s error. \n", str_filename);
         return 0; };

        fseek(fp, 0, SEEK_SET);
        for (i = 0; i< bytes; i++)
        {  
			  c = *(addr+i);
              fwrite(&c, 1, 1, fp); 
		}

        fclose(fp);

        return 1;

}

//----------------------
// read_file_to_ram
//----------------------
int
read_file_to_ram(char * str_filename, byte * addr, dword bytes,dword* rel_bytes)
{
   FILE    * fp;
   dword   i;
   byte    c;

   *rel_bytes = 0;

   if ( !( fp=fopen(str_filename, "rb")) )
      {  printf("Open file %s error. \n", str_filename);
         return 0; };

        fseek(fp, 0, SEEK_SET);
        for (i = 0; i< bytes; i++)
        {  
			  
              fread(&c, 1, 1, fp); 
  			  if (feof(fp) )
			 		return 1;
			  else
			  {
				  *(addr+i) = c;
			  (*rel_bytes)++;

			  }

		}

        fclose(fp);

        return 1;

}

//-------------------------
// print_256bs_hex      
//-------------------------

void
print_256bs_hex (byte * p_byte_buffer, dword virtual_address)
{
        int     i,j;
		byte    c,d;
       printf("\nPhysical Base Address: %lx\n", virtual_address);
        for (i = 0; i < 16; i++)
        {
           for (j = 0; j < 16; j++)
           {   c = p_byte_buffer[i * 16 + j];

	       d = c & 0x0f;

	       printf("%1x",c>>4);
	       printf("%1x",d );
	       putchar(' '); }
	   printf("   ");
	   for (j = 0; j < 16; j++)
           {   c = p_byte_buffer[i * 16 + j];
	       if ( c !=0x0a && c != 0x0d && c != 0x07 && c!=0x0c && c!=0x09)
	       printf("%c",c); }

	   putchar('\n'); }

}





//-----------------------------------------------------------------------------
// Open and configure the serial port, and put a handle to it in the global
// Serial. Print an error and exit if something fails.
//-----------------------------------------------------------------------------
byte InitSerial(char* str3)
{

    Serial = CreateFile( str3, GENERIC_READ | GENERIC_WRITE, 0, NULL,
                     OPEN_EXISTING, 0, NULL);

    if (Serial == INVALID_HANDLE_VALUE) {
        //printf("can't get port\n");
        return 0;
    }

    DCB dcb;

    if(!GetCommState(Serial, &dcb)) {
        //printf("can't get params\n");
        return 0;
    }

    dcb.BaudRate = CBR_57600;
    //dcb.BaudRate = CBR_115200;
    dcb.ByteSize = 8;
    dcb.Parity   = NOPARITY;
    dcb.StopBits = ONESTOPBIT;

    if(!SetCommState(Serial, &dcb)) {
        //printf("can't set params\n");
        return 0;
    }

    COMMTIMEOUTS cto;
    cto.ReadIntervalTimeout = 100;
    cto.ReadTotalTimeoutMultiplier = 0;
    cto.ReadTotalTimeoutConstant = 5000;

    cto.WriteTotalTimeoutMultiplier = 0;
    cto.WriteTotalTimeoutConstant = 0;

    if(!SetCommTimeouts(Serial, &cto)) {
        //printf("couldn't set timeouts\n");
        return 0;
    }

	return 1;
}



//---------------------
// uart_rx
//---------------------

byte uart_rx(byte* pByte)
{

    DWORD r;

    if(!ReadFile(Serial, pByte, 1, &r, NULL)) 
	{
        return 0;
	}
	else
		return 1;



}
		
//---------------------
// uart_tx
//---------------------
extern byte bCmdStr[16];
extern byte bCmdIdx;

byte uart_tx(byte c)
{
	


        dword written;
		byte b;
		b = c;
        if (!WriteFile(Serial, &b, 1, &written, NULL)){
			return 0;	
		}
		
		else
		{
			if (c == '[')
				bCmdIdx = 0;
			else
				bCmdIdx++;
			
			if (bCmdIdx >= 10)
				bCmdIdx = 0;

			bCmdStr[bCmdIdx] = c;

			return 1;
		}


}

//---------------------
// delayms
//---------------------

void
delayms(dword i){
	Sleep(i);
}

//---------------------
// hex_to_bin
//---------------------

byte 
hex_to_bin( char value ) {

	unsigned char result;
	result =value - '0';
	if ( result > 9 ) result -= ('A' - '9' - 1);

	return( result );

}

//---------------------
// hexit
//---------------------

char 
hexit( unsigned char value ) {

	char result;
	result = value & 0x0F;
	result += '0';
	if (result > '9') result += ('A' - '9' - 1);

	return( result );

}

#if 0

//--------------------
// InitGetParamFromFile
//--------------------
void
InitGetParamFromFile(void){
	int i;
	byte c = 0;

	for(i = 0; i< sizeof(struParam_t); i++)
	{	
		pStruParam[i] = 0;
	}


	if (read_file_to_ram("config.bin",  pStruParam, sizeof(struParam_t)) == 0) 
	{
		    bitParamOk = 0;
			Beep(2000, 500);
			Sleep(1000);
			Beep(2000, 500);

	}
	else
	{
	//	EncodeParam();
		for(i = 0; i< sizeof(struParam_t); i++)
		{	
			c += pStruParam[i];
		}

		if (c == 0)
		{
			bitParamOk = 1;
		}
		else
		{
			bitParamOk = 0;
			Beep(2000, 500);
		//	Sleep(1000);
		//	Beep(2000, 500);

		}	
		
	}

}
#endif











#if 0
//--------------------
// SerialThread
//--------------------
UINT SerialThread(void *pArg){

	//byte bBuf[8];
	byte c;
	int j;
	j = 8;
	while(1)
	{




		if ( uart_rx(&c) == 1)
		{
				
			if(InputFromHostStateMachine(c))
			{
			//	AfxMessageBox("OK");	
			// available package has been received, clean the other same package

				iFifoIndex = 0;
			}
			else
			{
				//Beep(500, 10);
			}	

		}
		

	}

	return 0;

}
#endif





void
process_a_line_output1(byte* in_buf, byte* out_buf){

	int i,j;
	int iStrLen;
	char strTemp1[32];
	static word wPYOffset = 0;

	//iStrLen = strlen((char*)in_buf);

	iStrLen = 0;

	for(i=0; i<1000; i++)
	{
		if (in_buf[i] != 0)
			iStrLen++;
	}	

	wPYOffset += iStrLen + 1;

	for(i=0; i<1000; i++)
		out_buf[i] = 0;


	for(j=0; j<iStrLen; j++)
	{	
		//out_buf[j] = in_buf[j];
		sprintf( (char*)(strTemp1),"0x%x, ", in_buf[j]);
		strcat( (char*)out_buf, strTemp1 );


	}
		strcat( (char*)out_buf, " 0, // OFFSET:");
		sprintf( (char*)(strTemp1),"hex=0x%x", wPYOffset);
		strcat( (char*)out_buf, strTemp1);

	//AfxMessageBox((char*)in_buf);
	//AfxMessageBox((char*)out_buf);
	//strcpy((char*)out, (char*)in);
}




//--------------------------
// FindSubStr
// return : 0: not find
//          non-0: position 
//			
//--------------------------

word FindSubStr(char* pStr1, char* pStr2)
{
	word i,j;
	word w_str_len1, w_str_len2;

	w_str_len1 = strlen(pStr1);
	w_str_len2 = strlen(pStr2);

	if (w_str_len1 < w_str_len2)
		return 0;

	for(i = 0; i < (w_str_len1 - w_str_len2); i++)
	{
		if (pStr1[i] == pStr2[0])
		{
			j = 0;
			while( (j< w_str_len2) && (pStr1[i+j] == pStr2[j]) )
			{
				j++;
			}

			if (j >= w_str_len2)
				return	i+1;
			else
			{		
			//	FormatDwordToStr(strTemp,j);
			//	AfxMessageBox(strTemp);
			}


		}


	}


	return 0;
}

//--------------------------
// FindSubStr
// return : 0: not find
//          non-0: position, offset ( from 0 )
//			
//--------------------------
word
ContinueFindXChar(char* pStr1, word n, char c){
	word i,j;
	i = 0;
	j = 0;
	while( (i<n)  && (pStr1[j] != 0) )
	{
		if (pStr1[j] == c)
			i++;
		j++;

	}

	if (i>=n)
		return j;
	else
		return 0;
}

