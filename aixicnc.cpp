//-------------------------------------------------
// Copyright by Aixi Wang (aixi.wang@hotmail.com)
//
//-------------------------------------------------

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

//extern dword x_steps_per_mm;
//extern dword y_steps_per_mm;
//extern dword z_steps_per_mm;
extern dword			x_um_per_circle;
extern dword			y_um_per_circle;
extern dword			z_um_per_circle;
extern dword			steps_per_circle;

extern dword xyz_delta;
extern dword timeout_cnt;

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

byte NO_COM_SIMULATE_SUPPORT = 0;
//char nc_filename[80];
char last_ncode = 0;
long new_x, new_y, new_z;

byte nc_filenames[256 * 256];
word nc_files = 0;

byte LoadNcList(char* str_nc_list);
void list_ncfiles(void);
void get_config_from_input(void);

//
//	printf("\nz_drill_height = %d *0.001 mm, z_travel_height = %d * 0.001 mm\n", z_depth, z_travel_height);
//	printf("drill_mm_pmin = %dmm/sec, travel_mm_pmin = %dmm/sec\n", drill_mm_pmin/60, travel_mm_pmin/60);
//	printf("x_steps_per_mm = %d, y_steps_per_mm = %d, z_steps_per_mm = %d\n", x_steps_per_mm, y_steps_per_mm, z_steps_per_mm);		
//

typedef struct{
  char			comport[32];
  long			z_depth;
  long			z_travel_height;
  long			drill_mm_pmin;
  long			travel_mm_pmin;
  dword			x_um_per_circle;
  dword			y_um_per_circle;
  dword			z_um_per_circle;
  dword			steps_per_circle;

  dword			xyz_delta;
  dword			timeout_cnt;
} ConfigParam;

ConfigParam configparam;


//-----------------
// aixiCNC
//-----------------

void 
aixiCNC(char* str_nc){

//int i,j;

float f1;
dword i;
char param_char[256];
word d;


	printf("\n-----------------------------------\n");
    printf("NC2CNC 3D (NC FILE LIST) V0.07.05.2  \n");
    printf("Copyright(c) Aixi Wang                \n");
    printf("e-mail: aixi.wang@hotmail.com		  \n");
    printf("Usage: nc2cnc nc.txt SIM  -- simulation mode		  \n");
    printf("       nc2cnc nc.txt      -- interpreter cnc.nc	  \n");

	printf("-----------------------------------\n");
	//xyz_disable();

	//if (arg
	


	printf("\n============== STEP1 ===============\n");	
//	if (LoadNcCode(str_nc) == 0)
	if (LoadNcList(str_nc) == 0)
		printf("load nc list %s fail!\n", str_nc);
	else
		printf("load nc list %s ok!\n", str_nc);

	
	list_ncfiles();
	
	dword file_length;

	if (read_file_to_ram("nc.cfg", (byte*)(&configparam), sizeof(configparam), &file_length))
	{
		// read param from cfg file
		strcpy (str_com_port, (char*)(&configparam.comport));
		z_depth = configparam.z_depth;
		z_travel_height = configparam.z_travel_height;
		drill_mm_pmin = configparam.drill_mm_pmin;
		travel_mm_pmin = configparam.travel_mm_pmin;
		x_um_per_circle= configparam.x_um_per_circle;
		y_um_per_circle = configparam.y_um_per_circle;
		z_um_per_circle = configparam.z_um_per_circle;
		steps_per_circle = configparam.steps_per_circle;


		xyz_delta = configparam.xyz_delta;
		timeout_cnt = configparam.timeout_cnt;
	}
	else
	{	// input param from manual, and save it to cfg file
		get_config_from_input();
		
		strcpy ((char*)(&configparam.comport), str_com_port);
		configparam.z_depth = z_depth;
		configparam.z_travel_height = z_travel_height;
		configparam.drill_mm_pmin = drill_mm_pmin;
		configparam.travel_mm_pmin = travel_mm_pmin;
		
		configparam.x_um_per_circle = x_um_per_circle;
		configparam.y_um_per_circle = y_um_per_circle;
		configparam.z_um_per_circle = z_um_per_circle;
		 configparam.steps_per_circle = steps_per_circle;


		//configparam.x_steps_per_mm = x_steps_per_mm;
		//configparam.y_steps_per_mm = y_steps_per_mm;
		//configparam.z_steps_per_mm = z_steps_per_mm;


		configparam.xyz_delta = xyz_delta;
		configparam.timeout_cnt = timeout_cnt;

		if (dump_ram_to_file("nc.cfg", (byte*)(&configparam), sizeof(configparam)))
			printf("write config param to nc.txt succesfully\n");
		else
			printf("write config param fail.\n");
	}

	printf("=================  Config Parameter Dump Start =============================\n");
	printf("com port: %s \n", str_com_port);
	printf("\nz_drill_height = %d *0.001 mm, z_travel_height = %d * 0.001 mm\n", z_depth, z_travel_height);
	printf("drill_mm_pmin = %dmm/sec, travel_mm_pmin = %dmm/sec\n", drill_mm_pmin/60, travel_mm_pmin/60);
	printf("xyz_delta = %d\n", xyz_delta);
printf("timeout_cnt = %d\n", timeout_cnt);
	printf("x_um_per_circle = %d, y_um_per_circle= %d, z_um_per_circle = %d, steps_per_circle=%d\n", x_um_per_circle, y_um_per_circle, z_um_per_circle, steps_per_circle);
	//printf("x_steps_per_mm = %d, y_steps_per_mm= %d, z_steps_per_mm = %d, \n", steps_per_circle*1000/x_um_per_circle, steps_per_circle*1000/y_um_per_circle, steps_per_circle*1000/z_um_per_circle);
	
	printf("=================  Config Parameter Dump End  =============================\n");


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



	// check serial relay control board

	set_x_mm_pmin(drill_mm_pmin);
	set_y_mm_pmin(drill_mm_pmin);
	set_z_mm_pmin(drill_mm_pmin);
			
	last_speed = drill_mm_pmin;


	printf("\n============== STEP3 ===============\n");	
	xyz_disable();
	printf("please adjust cnc to machine 0,0,0, any key to start drilling ... \n");
	getchar();
	//getchar();
	xyz_enable();
	(0, 0, z_travel_height);
	printf("begin to drill  ... \n");
	printf("any key ... \n");

	getchar();
	//getchar();


	//process_bmp_data();

#if 0	
	(10*1000,10*1000, 10*1000);
#endif

/////////////////////////////////////////////////////////////////////////////////
	


	for(d = 0; d < nc_files; d++)
	{

	strcpy(str_nc, (const char*)&nc_filenames[d*256]);
	printf("########  Step %d, %s Start #############\n", d+1, str_nc);

	if (LoadNcCode(str_nc) == 0)
		printf("load nc list %s fail!\n", str_nc);
	else
		printf("load nc list %s ok!\n", str_nc);
		
		
	bNcCodeState = 0;
	p_bmp_data[bmp_file_bytes] = 0;

	new_x = cur_x;
	new_y = cur_y;
	new_z = cur_z;

	zmotor_enable();

	for(i=0; i<bmp_file_bytes+1; i++)
		NcCodeStateMachine(p_bmp_data[i]);
/////////////////////////////////////////////////////////////////////////////////
	free(p_bmp_data);

	printf("######## Step %d, %s End, any key to continue ...  #############\n", d+1, str_nc);
	zmotor_disable();
//	(cur_x, cur_y, z_travel_height);
//	(0, 0, z_travel_height);
//	(0, 0, 0);
	goto_abs_pt_pt_um_3d(cur_x, cur_y, cur_z, cur_x, cur_y, z_travel_height);
	goto_abs_pt_pt_um_3d(cur_x, cur_y, cur_z, 0, 0, z_travel_height);
	goto_abs_pt_pt_um_3d(cur_x, cur_y, cur_z, 0, 0, 0);
	
	printf("delay 10 seconds ...\n");
//	getchar();
	::Sleep(10000);

	}


	printf("\n============== STEP4 ===============\n");	
	
	// drill working
	// (0, 0, 0);
	goto_abs_pt_pt_um_3d(cur_x, cur_y, cur_z, 0, 0, z_travel_height);
	goto_abs_pt_pt_um_3d(cur_x, cur_y, cur_z, 0, 0, 0);

	zmotor_disable();


	printf("\n============== STEP5 ===============\n");	
	puts("any key to disable xyz_lock ...");
	getchar();

	
	xyz_disable();

	printf("job over !");
	printf("any key to exit!\n");
	getchar();


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
byte last_code = 0;


void
NcCodeStateMachine(unsigned char c){

	if ((c == 0x0a) && (last_code == 0x0d))
		goto_abs_pt_pt_um_3d(cur_x, cur_y, cur_z, new_x, new_y, new_z);

	last_code = c;

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
			//	printf("xyz_value= %f\n",f1);
				

				xyz_um = long(f1*1000);

				//=====================================================

				switch (last_ncode){
				case 'X':			
									//printf("X=%d um\n",xyz_um);
									//(xyz_um,cur_y, cur_z);
									new_x = xyz_um;
						break;
				case 'Y':
									//printf("Y=%d um\n",xyz_um);
									//(cur_x ,xyz_um*(-1), cur_z);
									//goto_abs_pt_pt_um(cur_x,cur_y, new_x, xyz_um);
									//(new_x ,xyz_um, cur_z);
									new_y = xyz_um;

									//
									//if (cur_z > 0)
									//goto_abs_pt_pt_um(cur_x,cur_y, new_x, new_y);
	
									
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

									new_z = xyz_um;



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

	


//----------------------
// LoadNcCode
//----------------------
#define MAX_NC_FIELS 256
byte
LoadNcList(char* str_nc_list){
	//bmp_file_bytes = MAX_NC_LENGTH;
	byte* p_filelist;
	dword file_length;
	int i, j, k;

	//process_bmp_data(byte* bmp_data,dword x, dword y);
	p_filelist  = (byte*)(malloc(MAX_NC_FIELS * MAX_NC_FIELS));
	for(i=0; i<MAX_NC_FIELS * MAX_NC_FIELS; i++)
		p_filelist[i] = 0;

	if (read_file_to_ram(str_nc_list, p_filelist, MAX_NC_FIELS*MAX_NC_FIELS, &file_length))
	{
		printf("read nc data %d bytes!\n", file_length);
		
		j = 0;
		k = 0;
		for(i=0; i<file_length; i++)
		{
			//printf("%d, 0x%x\n", i, p_filelist[i]);
			//if ((p_filelist[i] == 0x0d) && (p_filelist[i+1] == 0x0a))
			if (p_filelist[i] == 0x0a)
			{ 
				j = j + 1;
				//i = i + 1;
				k = 0;
			}
			else
			{
				if (p_filelist[i] != 0x0d)
				{
					nc_filenames[j*256+k] = p_filelist[i];
					k++;
				}
			}
		}
		
		
		if (p_filelist[file_length-1] != 0x0a)
			nc_files = j + 1;
		else
			nc_files = j;

		return 1;
		//p_bmp_data += 0x3e;
	}

	else
	{
		return 0;
	}
	
}

//-------------------
// list_ncfile
//-------------------
void list_ncfiles(void){
	int i;
	printf("------------------------------------------------------\n");
	printf("nc files = %d\n", nc_files);
	for(i = 0; i< nc_files; i++)
		printf("Step %d, File: %s\n", i+1, &nc_filenames[i*256]);
//		puts((const char*)(&nc_filenames[i*256]));
//			printf("%d, 0x%x\n", i, nc_filenames[i*256]);
	printf("------------------------------------------------------\n");

}

//--------------------------
// get_config_from_input
//--------------------------

void get_config_from_input(void){
	char param_char[256];
	printf("\n============== STEP2 ===============\n");	
	printf("1. Please connect your CNC driver board to sCNC board\n");

if (NO_COM_SIMULATE_SUPPORT == 0)
{
	//////////////////////////////////////////////////////////////////
	printf("--------------------------------------------\n");	
	printf("COM port ? (for example: 3)");
	gets(param_char);
	if (strlen(param_char)>0)
	{
		//sscanf(param_char, "%f",&f1);
		//x_phisical_length = (long)f1;
		strcpy(str_com_port, "com");
		strcat(str_com_port,param_char);

	}
	else
		strcpy(str_com_port,"com3");

}

	printf("--------------------------------------------\n");	
	printf("x_um_per_circle ? (for example 20000)");
	
	gets(param_char);
	if (strlen(param_char)>0)
	{
		sscanf(param_char, "%f",&f1);
		x_um_per_circle = (dword)(f1);

	}

	printf("--------------------------------------------\n");	
	printf("y_um_per_circle? (for example 6000)");
	gets(param_char);
	if (strlen(param_char)>0)
	{
		sscanf(param_char, "%f",&f1);
	y_um_per_circle = (dword)(f1);
	}


	printf("--------------------------------------------\n");	
	printf("z_um_per_circle ? (for example 10000):");
	gets(param_char);
	if (strlen(param_char)>0)
	{
		sscanf(param_char, "%f",&f1);
		z_um_per_circle = (dword)(f1);
	}

	printf("--------------------------------------------\n");	
	printf("steps_per_circle ? (for example 1600):");
	gets(param_char);
	if (strlen(param_char)>0)
	{
		sscanf(param_char, "%f",&f1);
		steps_per_circle = (dword)(f1);
	}


	printf("--------------------------------------------\n");	
	printf("drill ? mm/sec(for example 30) :");
	gets(param_char);
	if (strlen(param_char)>0)
	{
		sscanf(param_char, "%f",&f1);
		drill_mm_pmin = (long)f1*60;
	}
	else
		drill_mm_pmin = 30 * 60;

	printf("--------------------------------------------\n");	
	printf("travel ? mm/sec(for example 30) :");
	gets(param_char);
	if (strlen(param_char)>0)
	{
		sscanf(param_char, "%f",&f1);
		travel_mm_pmin = (long)f1 * 60;
	}	
	else
		travel_mm_pmin = 30 * 60;


	printf("--------------------------------------------\n");	
	printf("xyz step delta (um) (for example 50) :");
	gets(param_char);
	if (strlen(param_char)>0)
	{
		sscanf(param_char, "%f",&f1);
		xyz_delta = (dword)f1;
	}	
	else
		xyz_delta = 50;


	printf("--------------------------------------------\n");	
	printf("timeout_cnt (for example 5000) :");
	gets(param_char);
	if (strlen(param_char)>0)
	{
		sscanf(param_char, "%f",&f1);
		timeout_cnt = (dword)f1;
	}	
	else
		timeout_cnt = 5000;

#if 1

	printf("--------------------------------------------\n");	
	printf("drilling axis-z height ? mm(for example -1):");
	gets(param_char);
	if (strlen(param_char)>0)
	{
		sscanf(param_char, "%f",&f1);
		z_depth = (long)(f1 * 1000);	
	}
	else
	{
		z_depth = (long)(-1 * 1000);	
	}
#endif

#if 1
	printf("--------------------------------------------\n");	
	printf("travel axis-z height ? mm(for example 3):");
	gets(param_char);
	if (strlen(param_char)>0)
	{
		sscanf(param_char, "%f",&f1);
		z_travel_height = (long)(f1 * 1000);
	}
	else
	{
		z_travel_height = (long)(3 * 1000);

	}

#endif

}