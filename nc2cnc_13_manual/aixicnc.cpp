#include <windows.h>
#include <stdio.h>
//#include "winio.h"
#include "stepmotor.h"
#include "typedef.h"
#include "aixicnc.h"
#include "subs.h"


dword bmp_x_length, bmp_y_length;
dword x_phisical_length;
dword bmp_data_bytes;
dword bmp_file_bytes;
dword bmp_bytes_per_x;
byte* p_bmp_data;


long z_depth = -1000; //   
long z_travel_height = 1000;
long drill_mm_pmin = 250;
long travel_mm_pmin = 500;

extern dword x_steps_per_mm;
extern dword y_steps_per_mm;
extern dword z_steps_per_mm;


extern long cur_x;
extern long cur_y;
extern long cur_z;

long new_x1=0;
long new_y1=0;
long new_z1=0;
dword cur_x1=0;
dword cur_y1=0;
dword cur_z1=0;

char str_com_port[6];

//nc code processing
byte bNcCodeState;
char xyz_value[80];
byte xyz_value_index;
byte bIsXiaoshu;
float f1;
long xyz_um;
long last_speed;

//char nc_filename[80];


word um_per_key = 200;

//-----------------
// show_xyz
//-----------------

void 
show_xyz(void){
	printf("goto abs axis: x=%d.%3d mm\t, y=%d.%3d mm\t, z=%d.%3d mm \t\n", cur_x/1000, cur_x%1000, cur_y/1000, cur_y%1000, cur_z/1000, cur_z%1000); 

}


//-----------------
// IskeyPressed
//-----------------


byte
IskeyPressed(word vk_key){
word wGAKS_return;
	wGAKS_return = GetAsyncKeyState( vk_key);
	// check num0
	if ( wGAKS_return == 0x8001 )	
	{
		return 1;
	}
	else
		return 0;

}
//-----------------
// ProcessNumkey
//-----------------
byte all_enable_sts = 0;
#define PNK_DEBUG 0

void
ProcessNumkey(void){

	while(1){


	if (IskeyPressed(VK_NUMPAD0))
	{

		cur_x = 0;
		cur_y = 0;
		cur_z = 0;
		printf("clear x,y,z to 0,0,0\n");	

	}

	if (IskeyPressed(VK_NUMPAD5))
	{
	
		if (all_enable_sts)
		{
#if PNK_DEBUG == 0
			xyz_disable();
#endif

			printf("disable xyz\n");
		}

		else
		{
#if PNK_DEBUG == 0
			xyz_enable();
#endif

			printf("enable xyz\n");

		}
		all_enable_sts = !all_enable_sts;
	}

	if (IskeyPressed(VK_NUMPAD1))
	{
#if PNK_DEBUG == 0
		zmotor_enable();
#endif

		printf("motor enabled\n");		
	}


	if (IskeyPressed(VK_NUMPAD3))
	{
#if PNK_DEBUG == 0
		zmotor_disable();				
#endif
		printf("motor disable\n");		
	}


	if (IskeyPressed(VK_NUMPAD9))
	{
#if PNK_DEBUG == 0
			goto_abs_axis_um(cur_x, cur_y, cur_z + um_per_key);
#endif
			show_xyz();

	}

	if (IskeyPressed(VK_NUMPAD7))
	{
#if PNK_DEBUG == 0
			goto_abs_axis_um(cur_x, cur_y, cur_z - um_per_key);
#endif
			show_xyz();
	}


	if (IskeyPressed(VK_NUMPAD6))
	{
#if PNK_DEBUG == 0
			goto_abs_axis_um(cur_x + um_per_key, cur_y, cur_z);
#endif
			show_xyz();
	}

	if (IskeyPressed(VK_NUMPAD4))
	{
#if PNK_DEBUG == 0
			goto_abs_axis_um(cur_x - um_per_key, cur_y, cur_z );
#endif
			show_xyz();
	}

	if (IskeyPressed(VK_NUMPAD8))
	{
#if PNK_DEBUG == 0
			goto_abs_axis_um(cur_x , cur_y + um_per_key, cur_z);
#endif
			show_xyz();
	}


	if (IskeyPressed(VK_NUMPAD2))
	{
#if PNK_DEBUG == 0
			goto_abs_axis_um(cur_x , cur_y -um_per_key, cur_z );
#endif
			show_xyz();
	}

	// check num .
	
	if (IskeyPressed(VK_DECIMAL))

	{
		printf("exit ...\n");
		break;
	}



	// check num +

	if (IskeyPressed(VK_ADD))

	{
		um_per_key = um_per_key + 100;

		printf("um_per_key + = %d um \n", um_per_key);
	}

	// check num -
	if (IskeyPressed(VK_SUBTRACT))

	{
	if (um_per_key > 100)
		um_per_key = um_per_key - 100;

		printf("um_per_key - = %d um \n", um_per_key);
	}



	// check num *
	if (IskeyPressed(VK_MULTIPLY))

	{

	}


	// check num /
	if (IskeyPressed(VK_DIVIDE))

	{

	}
	






	}


};

//-----------------
// aixiCNC
//-----------------

void 
aixiCNC(char* str_nc){

//int i,j;
float f1;
dword i;
char param_char[256];
char com_str[5] = "com";


	printf("\n-----------------------------------\n");
    printf("NC2CNC Golden 13 Manual       \n");
    printf("Copyright(c) szwax888  \n");
    printf("e-mail: szwax888@126.com		  \n");
	printf("-----------------------------------\n");
	//xyz_disable();

	//if (arg

	//test_key();

	
/*

	
	printf("\n============== STEP1 ===============\n");	
	if (LoadNcCode(str_nc) == 0)
		printf("load %s fail!\n", str_nc);
	else
		printf("load %s ok!\n", str_nc);

*/

#if PNK_DEBUG == 1
	ProcessNumkey();
#endif

		
	//printf("any key to continue!\n");
	//getchar();
	
	printf("\n============== STEP2 ===============\n");	


	printf("1. Please connect your CNC driver board to sCNC board\n");


	//////////////////////////////////////////////////////////////////
	//long z_depth = 500; //   
	//long z_travel_height = 500;
	printf("--------------------------------------------\n");	
	printf("COM port ? (for example: COM1, ENTER key to use COM2):");
	gets(param_char);
	if (strlen(param_char)>0)
	{
		//sscanf(param_char, "%f",&f1);
		//x_phisical_length = (long)f1;
		strcat(com_str, param_char);
		
		strcpy(str_com_port,com_str);
		//puts(str_com_port);
	}
	else
		strcpy(str_com_port,"com2");


	if(!InitSerial(str_com_port))
	{
		printf("Init com port %s fail\n", str_com_port);
		getchar();

		exit(1);
	}
	else
		printf("Init com port %s ok\n", str_com_port);
	
	xyz_disable();
	zmotor_disable();

	
	printf("--------------------------------------------\n");	
	printf("x_steps/mm ? (for example 80, ENTER key to use default value):");
	
	gets(param_char);
	if (strlen(param_char)>0)
	{
		sscanf(param_char, "%f",&f1);
		x_steps_per_mm = (long)(f1);

	}

	printf("--------------------------------------------\n");	
	printf("y_steps/mm ? (for example 266, ENTER key to use default value):");
	gets(param_char);
	if (strlen(param_char)>0)
	{
		sscanf(param_char, "%f",&f1);
	y_steps_per_mm = (long)(f1);
	}


	printf("--------------------------------------------\n");	
	printf("z_steps/mm ? (for example 160, ENTER key to use default value):");
	gets(param_char);
	if (strlen(param_char)>0)
	{
		sscanf(param_char, "%f",&f1);
		z_steps_per_mm = (long)(f1);
	}

	printf("--------------------------------------------\n");	
	printf("drill ? mm/sec(for example 1, ENTER key to use default value) :");
	gets(param_char);
	if (strlen(param_char)>0)
	{
		sscanf(param_char, "%f",&f1);
		drill_mm_pmin = (long)f1*60;
	}
	else
		drill_mm_pmin = 60;

	printf("--------------------------------------------\n");	
	printf("travel ? mm/sec(for example 2, ENTER key to use default value) :");
	gets(param_char);
	if (strlen(param_char)>0)
	{
		sscanf(param_char, "%f",&f1);
		travel_mm_pmin = (long)f1 * 60;
	}	
	else
		travel_mm_pmin = 120;

#if 0

	printf("--------------------------------------------\n");	
	printf("drilling axis-z height ? mm(for example -1, ENTER key to use default value):");
	gets(param_char);
	if (strlen(param_char)>0)
	{
		sscanf(param_char, "%f",&f1);
		z_depth = (long)(f1 * 1000);	
	}

	printf("--------------------------------------------\n");	
	printf("travel axis-z height ? mm(for example 1, ENTER key to use default value):");
	gets(param_char);
	if (strlen(param_char)>0)
	{
		sscanf(param_char, "%f",&f1);
		z_travel_height = (long)(f1 * 1000);
	}

#endif



#if 0
	printf("--------------------------------------------\n");	
	printf("x_phisical_length ? mm (ENTER key to use %d mm as x, 0.1mm/pixel) :", bmp_x_length/10 );
	gets(param_char);
	if (strlen(param_char)>0)
	{
		sscanf(param_char, "%f",&f1);
		x_phisical_length = (long)(f1*1000);
	}
	else
		x_phisical_length = bmp_x_length * 100;
#endif

	


	printf("\nz_drill_height = %d *0.001 mm, z_travel_height = %d * 0.001 mm\n", z_depth, z_travel_height);
	printf("drill_mm_pmin = %dmm/sec, travel_mm_pmin = %dmm/sec\n", drill_mm_pmin/60, travel_mm_pmin/60);
	printf("x_steps_per_mm = %d, y_steps_per_mm = %d, z_steps_per_mm = %d\n", x_steps_per_mm, y_steps_per_mm, z_steps_per_mm);
//	printf("x_phisical_length = %d 0.001mm\n", x_phisical_length);
		

	// check serial relay control board


	set_x_mm_pmin(drill_mm_pmin);
	set_y_mm_pmin(drill_mm_pmin);
	set_z_mm_pmin(drill_mm_pmin);
			
	last_speed = drill_mm_pmin;


#if 0

	printf("\n============== STEP3 ===============\n");	
	xyz_disable();
	printf("please adjust cnc to machine 0,0,0, any key to start drilling ... \n");
	getchar();
	//getchar();
	xyz_enable();
	goto_abs_axis_um(0, 0, z_travel_height);


	printf("begin to drill  ... \n");
	printf("any key ... \n");

	getchar();
	//getchar();
	zmotor_enable();
#endif

	//process_bmp_data();

#if 0	
	goto_abs_axis_um(10*1000,10*1000, 10*1000);
#endif

/////////////////////////////////////////////////////////////////////////////////	

#if 0	
	bNcCodeState = 0;
	p_bmp_data[bmp_file_bytes] = 0;

	for(i=0; i<bmp_file_bytes+1; i++)
		NcCodeStateMachine(p_bmp_data[i]);
/////////////////////////////////////////////////////////////////////////////////



	free(p_bmp_data);
#endif

#if PNK_DEBUG == 0
	ProcessNumkey();
#endif









	printf("\n============== STEP4 ===============\n");	
	
	// drill working
//	goto_abs_axis_um(0, 0, z_travel_height);

	xyz_disable();
	zmotor_disable();

//	printf("job over !");
//	printf("any key to exit!\n");
//getchar();


};



//--------------------------
// NcCodeStateMachine
//--------------------------
//byte bNcCodeState;
//char xyz_value[80];
//byte xyz_value_index;
//byte bIsXiaoshu;
//float f1;
//dword xyz_um;
char last_ncode;
long new_x, new_y;


void
NcCodeStateMachine(unsigned char c){

	switch (bNcCodeState){

//--------------------------------------------------	
	case	0:
		switch (c){
		case 'X':
				bNcCodeState = 1;
				xyz_value_index = 0;
				last_ncode = 'X';

				break;
		case 'Y':
				bNcCodeState = 2;
				xyz_value_index = 0;
				last_ncode = 'Y';
				break;
		case 'Z':
				bNcCodeState = 3;
				xyz_value_index = 0;
				last_ncode = 'Z';
				break;

		case 'N':
				bNcCodeState = 4;
				xyz_value_index = 0;
				last_ncode = 'N';
				break;


		default:
				bNcCodeState = 0;
				last_ncode = ' ';
				break;

		}

			break;

//--------------------------------------------------	
		
	case	1: // x
	case	2: // y
	case	3: // z
	case	4: // N

		bIsXiaoshu = 0;
		switch(c){
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		case '-':

				xyz_value[xyz_value_index] = c;
				xyz_value_index++;

				break;
		case '.':
				xyz_value[xyz_value_index] = c;
				xyz_value_index++;

				bIsXiaoshu = 1;
				break;	


		default:
//////////////////////////////////////////////////////////////////////
				xyz_value[xyz_value_index] = 0;
				sscanf(xyz_value,"%f",&f1);
				//printf("xyz_value= %f\n",f1);
				

				xyz_um = long(f1*1000);

				//=====================================================

				switch (last_ncode){
				case 'X':			
									//printf("X=%d um\n",xyz_um);
									//goto_abs_axis_um(xyz_um,cur_y, cur_z);
									new_x = xyz_um;
						break;
				case 'Y':
									//printf("Y=%d um\n",xyz_um);
									//goto_abs_axis_um(cur_x ,xyz_um*(-1), cur_z);
									//goto_abs_pt_pt_um(cur_x,cur_y, new_x, xyz_um);
									//goto_abs_axis_um(new_x ,xyz_um, cur_z);
									new_y = xyz_um;

									if ( (xyz_um >= 0) && (last_speed != travel_mm_pmin) )
									{
										set_x_mm_pmin(travel_mm_pmin);
										set_y_mm_pmin(travel_mm_pmin);
										set_z_mm_pmin(travel_mm_pmin);

										last_speed = travel_mm_pmin;
									}
									
									if ( (xyz_um < 0) && (last_speed == travel_mm_pmin) )
									{
										set_x_mm_pmin(drill_mm_pmin);
										set_y_mm_pmin(drill_mm_pmin);
										set_z_mm_pmin(drill_mm_pmin);

										last_speed = drill_mm_pmin;
									}

									//
									if (cur_z > 0)
										goto_abs_axis_um(new_x,new_y, cur_z);
									else
									{	
										if ( (new_x == cur_x) || (new_y == cur_y) )
											goto_abs_axis_um(new_x,new_y, cur_z);
										else
											goto_abs_pt_pt_um(cur_x,cur_y, new_x, new_y);
	
									}
									
									break;
				case 'Z':
									//printf("Z=%d um\n",xyz_um);

									if ( (xyz_um >= 0) && (last_speed != travel_mm_pmin) )
									{
										set_x_mm_pmin(travel_mm_pmin);
										set_y_mm_pmin(travel_mm_pmin);
										set_z_mm_pmin(travel_mm_pmin);

										last_speed = travel_mm_pmin;
									}
									
									if ( (xyz_um < 0) && (last_speed == travel_mm_pmin) )
									{
										set_x_mm_pmin(drill_mm_pmin);
										set_y_mm_pmin(drill_mm_pmin);
										set_z_mm_pmin(drill_mm_pmin);

										last_speed = drill_mm_pmin;
									}

									goto_abs_axis_um(new_x,new_y, xyz_um);




						break;
				case 'N':
						printf("N%d ",xyz_um/1000);
						break;
				
				default:
						break;
				}


				//=====================================================

				switch (c){
				case 'X':
						bNcCodeState = 1;
						xyz_value_index = 0;
						last_ncode = 'X';

						break;
				case 'Y':
						bNcCodeState = 2;
						xyz_value_index = 0;
						last_ncode = 'Y';
						break;
				case 'Z':
						bNcCodeState = 3;
						xyz_value_index = 0;
						last_ncode = 'Z';
						break;
				default:
						last_ncode = ' ';
						bNcCodeState = 0;
						break;
				}

				//getchar();
				break;			
		}

			break;




//--------------------------------------------------	

	default:
			bNcCodeState = 0;
			break;

	}

}

//----------------------
// LoadNcCode
//----------------------
#define MAX_NC_LENGTH 64*1024*10
byte
LoadNcCode(char* str_nc1){
	//bmp_file_bytes = MAX_NC_LENGTH;


	//process_bmp_data(byte* bmp_data,dword x, dword y);
	p_bmp_data  = (byte*)(malloc(MAX_NC_LENGTH+1));
	if (read_file_to_ram(str_nc1, p_bmp_data, MAX_NC_LENGTH, &bmp_file_bytes))
	{
		printf("read nc data %d bytes!\n", bmp_file_bytes);
		return 1;
		//p_bmp_data += 0x3e;
	}

	else
	{
			//free(p_bmp_data);
		printf("read cnc.nc data fail!\n");
		//return 0;
		//::ExitProcess(1);
		getchar();
		exit(1);

	}
	


}

	