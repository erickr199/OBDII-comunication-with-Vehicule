#include "FinalProyOBD_InterfacecesCompu.h"

//==============================================================================
//
// Title:		FinalProyOBD_InterfacecesCompu
// Purpose:		Serial communication with a vehicle using OBD II with the ELM327
//              Computer Interfaces Final project
//              Team members:
//              Erick Osvaldo Castelazo Guerra - A01273870
//				Jesus Salvador Lopez Ortega - A01702361
//				Hugo Mora Mora - A01702099
//
// Created on:	28/11/2020 at 11:20:35 a. m. by Erick Castelazo.
// Copyright:	ITESM. All Rights Reserved.
//
//==============================================================================

//==============================================================================
// Include files

#include <rs232.h>
#include <ansi_c.h>
#include <cvirte.h>		
#include <userint.h>
#include "toolbox.h"
#include "FinalProyOBD_InterfacecesCompu.h"

//==============================================================================
// Constants
#define COM_PORT 3
//==============================================================================
// Types

//==============================================================================
// Static global variables
static int panelHandle = 0;

//Commands Declarations

char AT_reset[6]		 = {0x41,0x54,0x20,0x5A,0x0D};     //AT Z<CR>
char AT_searchProtocol[9]= {0x41,0x54,0x20,0x53,0x50,0x20,0x30,0x0D};//AT SP 0<CR>
char AT_firmware[6]      = {0x41,0x54,0x20,0x49,0x0D};     //AT I<CR>  
char AT_batteryVolt[7]   = {0x41,0x54,0x20,0x52,0x56,0x0D};//AT RV<CR>
char OBD_coolant[7]      = {0x30,0x31,0x20,0x30,0x35,0x0D};//01 05<CR>  1
char OBD_rpm[7]		     = {0x30,0x31,0x20,0x30,0x43,0x0D};//01 0C<CR>  2
char OBD_speed[7]		 = {0x30,0x31,0x20,0x30,0x44,0x0D};//01 0D<CR>  1
char OBD_throttle[7]	 = {0x30,0x31,0x20,0x31,0x31,0x0D};//01 11<CR>  1
char OBD_fuelPreassure[7]= {0x30,0x31,0x20,0x30,0x41,0x0D};//01 0A<CR>  1
char OBD_engineLoad[7]	 = {0x30,0x31,0x20,0x30,0x34,0x0D};//01 04<CR>  1
char OBD_IAT[7]	         = {0x30,0x31,0x20,0x30,0x46,0x0D};//01 0F<CR>  1
char OBD_MAF[7]		     = {0x30,0x31,0x20,0x31,0x30,0x0D};//01 10<CR>  2
char OBD_manifold[7]     = {0x30,0x31,0x20,0x30,0x42,0x0D};//01 0B<CR>  1  

//Los arrays son un elemento mas grande para que su ulitmo elemento sea un caracter null//
//==============================================================================
// Function prorotypes
int Initialize(void); // Function that opens comm port and initializes it
// Static functions

//==============================================================================
// Global variables
unsigned char Enable = 0; // Initialization of comm port flag 

//==============================================================================
// Global functions

/// HIFN The main entry-point function.
int main (int argc, char *argv[])
{
	int error = 0;
	
	/* initialize and load resources */
	nullChk (InitCVIRTE (0, argv, 0));
	errChk (panelHandle = LoadPanel (0, "FinalProyOBD_InterfacecesCompu.uir", PANEL));
	
	/* display the panel and run the user interface */
	errChk (DisplayPanel (panelHandle));
	errChk (RunUserInterface ());

Error:
	/* clean up */
	if (panelHandle > 0)
		DiscardPanel (panelHandle);
	return 0;
}

//==============================================================================
// User defined functions

//Function that open Comm port// 
int Initialize ()
{
	
          OpenComConfig (COM_PORT, "", 38400, 0, 8, 1, 512, 512);
		  Enable = 1;	  
		  //Clean serial port from trash
	      FlushInQ(COM_PORT);
	      FlushOutQ(COM_PORT);
	      //Set another port timeout; too slow and project lags, too fast and readings are incorrect 
	      SetComTime (COM_PORT,0.25);
		  //No handshake is needed
		  Delay(1);
		  
		  	/*
			//Tx: send command
			ComWrt(COM_PORT,AT_reset,6);         //AT Z: reset
			Delay(1);
			//Tx: send command
			ComWrt(COM_PORT,AT_searchProtocol,8);//AT SP 0: search for protocol
			Delay(1);
			*/
		  
	return 0;
}


//Adjust the recived ASCII to have the hex value
//equivalent to what the ASCII represent
unsigned char asciiToHex (unsigned char a)
{
    //adjust ASCII between 0 - 9
    if ((a>='0')&&(a<='9'))
        a = a-0x30;
    //adjust ASCII between A - F
    if ((a>='A')&&(a<='F'))
        a = a - 0x37;
    return a;
}



//Function to read the coolant Temperature//
void Read_Coolant(void){
	//variables that store the recived bytes 
	char response[28]={0};
	unsigned char b1,b2,Byte; 
	
	int Temp; 
	
	//Tx: send command
	ComWrt(COM_PORT,OBD_coolant,6);
	//Rx: read response
	ComRd(COM_PORT,response,28);
	
	b1= asciiToHex(response[12]);//H
	b2= asciiToHex(response[13]);//L

	Byte = (16*b1)+b2;
	
	//Process data
	Temp = Byte-40;	//°C
	
	//display data
	SetCtrlVal(panelHandle,PANEL_GUI_COOLANT,Temp);
}



//Function to read Vehicle Speed
void Read_Speed(void){
	//variables that store the recived bytes 
	char response[28]={0};
	unsigned char b1,b2,Byte; 
	
	unsigned int speed;
	
	//Tx: send command
	ComWrt(COM_PORT,OBD_speed,6);
	//Rx: read response
	ComRd(COM_PORT,response,28);
	
	b1= asciiToHex(response[12]);//H
	b2= asciiToHex(response[13]);//L

	Byte = (16*b1)+b2;	//km/h
	
	
	//Process data
	speed=Byte;
	//display data
	SetCtrlVal(panelHandle,PANEL_GUI_SPEED,speed);

}


//Function to read engine RPM
void Read_RPM(void){
	//variables that store the recived bytes 
	char response[34]={0};
	unsigned char b1,b2,b3,b4,ByteH,ByteL; 
	
	float rpm; 
	
	//Tx: send command
	ComWrt(COM_PORT,OBD_rpm,6);
	//Rx: read response
	ComRd(COM_PORT,response,34);
	
	b1= asciiToHex(response[12]);//H
	b2= asciiToHex(response[13]);//L

	ByteH = (16*b1)+b2;
	
	b3= asciiToHex(response[15]);//H
	b4= asciiToHex(response[16]);//L

	ByteL = (16*b3)+b4;

	
	//Process data
	rpm = ((ByteH*256)+ByteL)/4;	//rpm
	
	//display data
	SetCtrlVal(panelHandle,PANEL_GUI_RPM,rpm/1000);
	
}


//Function to read vehicle Throttle position
void Read_Throttle(void){
	//variables that store the recived bytes 
	char response[28]={0};
	unsigned char b1,b2,Byte; 
	
	double throttle; 
	
	//Tx: send command
	ComWrt(COM_PORT,OBD_throttle,6);
	//Rx: read response
	ComRd(COM_PORT,response,28);
	
	b1= asciiToHex(response[12]);//H
	b2= asciiToHex(response[13]);//L

	Byte = (16*b1)+b2;
	
	//Process data
	throttle = (Byte*100.0)/255.0;	//%
	
	//display data
	SetCtrlVal(panelHandle,PANEL_GUI_THROTTLE,throttle);


}


//Function to read Fuel Preassure
void Read_FuelPereassure(void){
	//variables that store the recived bytes 
	char response[28]={0};
	unsigned char b1,b2,Byte; 
	
	unsigned int fuelPre; 
	
	//Tx: send command
	ComWrt(COM_PORT,OBD_fuelPreassure,6);
	//Rx: read response
	ComRd(COM_PORT,response,28);
	
	
	//Check if sensor exists on vehicle
	if (response[6]==0x4E)
	{
		//If sensor is not found
		SetCtrlVal(panelHandle,PANEL_GUI_FUELPRE,0);
		SetCtrlAttribute (panelHandle, PANEL_FUELPRE_ERROR, ATTR_VISIBLE, 1);
		
	}
	else
	{
		b1= asciiToHex(response[12]);//H
		b2= asciiToHex(response[13]);//L

		Byte = (16*b1)+b2;
	
		//Process data
		fuelPre = Byte*3;	//kPa(gauge)
	
		//display data
		SetCtrlVal(panelHandle,PANEL_GUI_FUELPRE,fuelPre);
	}
	
}


//Function to read engine load value
void Read_EngineLoad(void){
	//variables that store the recived bytes 
	char response[28]={0};
	unsigned char b1,b2,Byte; 
	
	unsigned int engineLoad; 
	
	//Tx: send command
	ComWrt(COM_PORT,OBD_engineLoad,6);
	//Rx: read response
	ComRd(COM_PORT,response,28);
	
	b1= asciiToHex(response[12]);//H
	b2= asciiToHex(response[13]);//L

	Byte = (16*b1)+b2;
	
	//Process data
	engineLoad = (Byte*100)/255;  //%
	
	//display data
	SetCtrlVal(panelHandle,PANEL_GUI_LOAD,engineLoad);
	
	
}


void Read_IAT(void){
	//variables that store the recived bytes 
	char response[28]={0};
	unsigned char b1,b2,Byte; 
	
	double iat; 
	
	//Tx: send command
	ComWrt(COM_PORT,OBD_IAT,6);
	//Rx: read response
	ComRd(COM_PORT,response,28);
	
	b1= asciiToHex(response[12]);//H
	b2= asciiToHex(response[13]);//L

	Byte = (16*b1)+b2;
	
	//Process data
	iat = Byte-40;  //°C
	
	//display data
	SetCtrlVal(panelHandle,PANEL_GUI_IAT,iat);

}


void Read_MAF(void){
	//variables that store the recived bytes 
	char response[34]={0};
	unsigned char b1,b2,b3,b4,ByteH,ByteL; 
	
	double maf; 
	
	//Tx: send command
	ComWrt(COM_PORT,OBD_MAF,6);
	//Rx: read response
	ComRd(COM_PORT,response,34);
	
	b1= asciiToHex(response[12]);//H
	b2= asciiToHex(response[13]);//L

	ByteH = (16*b1)+b2;
	
	b3= asciiToHex(response[15]);//H
	b4= asciiToHex(response[16]);//L

	ByteL = (16*b3)+b4;

	
	//Process data
	maf = ((ByteH*256.0)+ByteL)/100.0;  //grams/sec
	
	//display data
	SetCtrlVal(panelHandle,PANEL_GUI_MAF,maf);	


}


void Read_Manifold(void){
	//variables that store the recived bytes 
	char response[28]={0};
	unsigned char b1,b2,Byte; 
	
	unsigned int manifold;
	
	//Tx: send command
	ComWrt(COM_PORT,OBD_manifold,6);
	//Rx: read response
	ComRd(COM_PORT,response,28);
	
	
	
	//Check if sensor exists on vehicle	
	if (response[6]==0x4E)
	{
		//If sensor is not found
		SetCtrlVal(panelHandle,PANEL_GUI_MANIFOLD,0);
		SetCtrlAttribute (panelHandle, PANEL_MANIFOLD_ERROR, ATTR_VISIBLE, 1);
		
	}
	else
	{
		b1= asciiToHex(response[12]);//H
		b2= asciiToHex(response[13]);//L

		Byte = (16*b1)+b2;	//kPa
	
	
		//Process data
		manifold=Byte;
		//display data
		SetCtrlVal(panelHandle,PANEL_GUI_MANIFOLD,manifold);
		
	}
	
}



//==============================================================================
// UI callback function prototypes

/// HIFN Exit when the user dismisses the panel.
int CVICALLBACK panelCB (int panel, int event, void *callbackData,
		int eventData1, int eventData2)
{
	if (event == EVENT_CLOSE)
		QuitUserInterface (0);
	return 0;
}


int CVICALLBACK TimerCallback (int panel, int control, int event,
							   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_TIMER_TICK:
			// Preventes program from getting a NON-FATAL RUN TIME ERROR Messages when program terminates
			SetBreakOnLibraryErrors (0);
			
			if (Enable == 0)
			{
			  Initialize();
			}
			else 
			{	
				//Llamada Funciones OBDII 	
				Read_Coolant();
				Read_Speed();
				Read_RPM(); 
				Read_Throttle();
				Read_FuelPereassure();
				Read_EngineLoad();
				Read_IAT();
				Read_MAF();
				Read_Manifold();
				
			}
			
			// Enable NON-FATAL RUN TIME ERROR Messages to show (default)
			SetBreakOnLibraryErrors (1);
			break;
	}
	return 0;
}





int CVICALLBACK QuitCallback (int panel, int control, int event,
							  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			CloseCom(COM_PORT);
			QuitUserInterface (0);
			break;
			
		case EVENT_RIGHT_CLICK:

			break;
	}
	return 0;
}

