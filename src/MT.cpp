#include "serial.h"

//This class initializes serial communication routines and backend connections.
serialComm scomm;

int main()
{
	//This function initilizes uart0 and sets up stdio
	startUart();
	//This function enables GPIO and various pins for other functions
	scomm.startUp();
	
	while(1)
	{
		//This checks against a function that passes a variable from the local instance of the global structure 'defines{} Def' in serial.h
		switch(getstayInConnectionMenu())
		{
			case 0:
				//This is the primary menu with follow up selection
				scomm.showMenuOptions();
				scomm.selectMenuItem();
				break;
			case 1:
				//This is the applications path & circuit connection/disconnection menu
				scomm.getX();
				break;
			case 2:
				//This is the menu for the power supply and additional functionality
				scomm.showSpecialFunctionsMenu();
				scomm.selectMenuItem();
				break;
			default:
				//This prints the current value of Def.stayInConnectionMenu and warns the user.
				printStuff(getstayInConnectionMenu());
				break;
		}
	}
	
}
