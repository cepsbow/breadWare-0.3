/*
 * 		TODO:
 * 			
 * 			---Def.stayInConnectionMenu value is unpredicatable throughout program flow (FIXED)
 * 			---Add sanitizeInput() function (FIXED - Input Format is fixed by function, use routine as necessary)
 * 			getInp()/getInpV()--Add to getchar(); (Use two functions with separate buffers for multilayer serial menus)
 *			Add a way to search through existing connections to disconnect stuff (TODO STILL)
 *        	prototype i.e.-------> bool checkDupes(int[][], int[][]){}; etc 
 *			Add negative number input detection
 * 			Some bits are erratic with getX
 * 			potentiometer needs attention
 * 			powerSupplyMenu does not retry and has issues with values and selections -- handoff issues with other functions
 * 			
 */

#ifndef serial_h
#define serial_h

#include "pico/stdlib.h"
#include "pico/runtime.h"
#include "pico/time.h"
#include "pico/divider.h"
#include "stdio.h"
#include "pico/float.h"
#include "pico/printf.h"
#include "breadWareDefines.h"
#include "Globals.h"
#include "ctype.h"

#ifdef __cplusplus

/*
Adafruit_MCP4728 mcp;
mcp4728 dac = mcp4728(0); // instantiate mcp4728 object, Device ID = 0

MCP466_DigitalPot pot(mcpAddress);

defines *getDefinesHandle(void)
{
	defines *pnt = &Def;
  	return pnt;
};
bool isUart()
{
	if(uart_is_readable(uart0)) return true;
	else return false;
};
*/

//----------------------------------------------------------------------------------------------//
//--------------------------------General/Class Support Functions-------------------------------//
//----------------------------------------------------------------------------------------------//

int getstayInConnectionMenu(void)
{
	return Def.stayInConnectionMenu;
};

void setstayInConnectionMenu(int value)
{
	Def.stayInConnectionMenu = value;
};



void startUart()
{
	//Startup UART the pico way
	uart_init(uart0, 115200);
	uart_set_baudrate(uart0, 115200);
	gpio_set_function(0, GPIO_FUNC_UART);
	gpio_set_function(1, GPIO_FUNC_UART);
	uart_set_hw_flow(uart0, false, false); //uart_set_hw_flow(UART_ID, BOOL CTS, BOOL RTS);
	uart_set_format(uart0, 8, 1, UART_PARITY_NONE); //uart_set_format(UART_ID, DATA_BITS, STOP_BITS, PARITY) (115200bps 8N1)
	uart_set_fifo_enabled(uart0, false);
	stdio_uart_init();
	//stdio_init_all();
    printf("UART Initialized!\n");
}

void printStuff(int st)
{
	printf("\nOption Selected:\n");
	printf("%3d", st);
	printf("\nError! Invalid main routine selection! Please double check the value above!!\n");
	Def.stayInConnectionMenu = 0;
};

void getInp(int size)
{
	// Set serialBuffer's elements to a sentinel value
	for(int i=0; i < size; i++)
	{
		Def.serialBuffer[i] = 254;
	}
	// Get the necessary number of corrected inputs and show the user what they input
	for(int i=0; i < size; i++)
	{
		do
		{
			Def.serialBuffer[i] = getchar(); 
		}while(Def.serialBuffer[i] == 254);
		// WARNING - There are limitations to this method of interpreting ASCII integers
		// Useful primarily as a single digit value input, expect issues if numbers are used as sentinel values on inputs in functions. (In other words, use something else)
		if( Def.serialBuffer[i] >= 48 && Def.serialBuffer[i] <= 57 )
		{
			Def.serialBuffer[i] -= 48;
			printf("%1d", Def.serialBuffer[i]);
		}
		else if( !isdigit(Def.serialBuffer[i]) )
		{
			tolower(Def.serialBuffer[i]);
			printf("%1c", Def.serialBuffer[i]);
		}
	}
};

void getInpV(int size)
{
	// Set vBuffer's elements to a sentinel value
	for(int i=0; i < size; i++)
	{
		Def.vBuffer[i] = 254;
	}
	// Get the necessary number of corrected inputs and show the user what they input
	for(int i=0; i < size; i++)
	{
		do
		{
			Def.vBuffer[i] = getchar(); 
		}while(Def.vBuffer[i] == 254);
		// WARNING - There are limitations to this method of interpreting ASCII integers
		// Useful primarily as a single digit value input, expect issues if numbers are used as sentinel values on inputs in functions. (In other words, use something else)
		if( Def.vBuffer[i] >= 48 && Def.vBuffer[i] <= 57 )
		{
			Def.vBuffer[i] -= 48;
			printf("%2d", Def.vBuffer[i]);
		}
		else if( !isdigit(Def.vBuffer[i]) )
		{
			tolower(Def.vBuffer[i]);
			printf("%1c", Def.vBuffer[i]);
		}
	}
};

//----------------------------------------------------------------------------------------------//
//---------------------------------Serial Communications Class----------------------------------//
//----------------------------------------------------------------------------------------------//

class serialComm
{
	public:
      
        void startUp(void)
        {
            //Set array pins to outputs and hold them low
            gpio_set_dir(AY0, true);
            gpio_pull_down(AY0);
            gpio_set_dir(AY1, true);
            gpio_pull_down(AY1);
            gpio_set_dir(AY2, true);
            gpio_pull_down(AY2);
      
            gpio_set_dir(AX0, true);
            gpio_pull_down(AX0);
            gpio_set_dir(AX1, true);
            gpio_pull_down(AX1);
            gpio_set_dir(AX2, true);
            gpio_pull_down(AX2);
            gpio_set_dir(AX3, true);
            gpio_pull_down(AX3);
      
            gpio_set_dir(CS_A, true);
            gpio_pull_down(CS_A);
            gpio_set_dir(CS_B, true);
            gpio_pull_down(CS_B);
            gpio_set_dir(CS_C, true);
            gpio_pull_down(CS_C);
            gpio_set_dir(CS_D, true);
            gpio_pull_down(CS_D);
            gpio_set_dir(CS_E, true);
            gpio_pull_down(CS_E);
            gpio_set_dir(CS_F, true);
            gpio_pull_down(CS_F);
            gpio_set_dir(CS_G, true);
            gpio_pull_down(CS_G);
            gpio_set_dir(CS_H, true);
            gpio_pull_down(CS_H);
            gpio_set_dir(CS_I, true);
            gpio_pull_down(CS_I);
      
            gpio_set_dir(RESET, true);    //all of this just sets up all these pins as outputs and drives them LOW
            gpio_pull_up(RESET); //I'm pretty sure Arduino IDE does this automatically but I wouldn't count on it
            sleep_ms(40);
            gpio_pull_down(RESET);
      
            gpio_set_dir(6, true); //(PIN_PF2 on ATmega)
            gpio_pull_down(6);
            gpio_set_dir(7, true); //(PIN_PF3 on ATmega)
            gpio_pull_down(7);
      
            gpio_set_dir(15, true); //(PIN_PA2 on ATmega)
      
            gpio_set_dir(15, true); //(PIN_PA3 on ATmega)
      
            gpio_set_dir(25, true); //(PIN_PD3 on ATmega)
            gpio_pull_down(25);
      
            gpio_set_dir(PWR_NEG_EN, true);
            gpio_pull_up(PWR_NEG_EN);
      
            gpio_set_dir(DATA, true);
            gpio_pull_up(DATA);
            gpio_set_dir(STROBE, true);
            //gpio_pull_up(STROBE);
      
            clearConnections();
      
			powerSupply(0, 0, 0);
            //printf("Yeet!");
            printf("\nGPIO & POWER SUPPLY INITIALIZED!!!\n");
      }


      int findPath(int breadRow1, int breadRow2)
      {
          //Add a way to search through existing connections to disconnect stuff
          //checkDupes(GbreadRow1, GbreadRow2);
          if (breadRow1 == breadRow2)
          {
            return -1;
          }
    
          if (breadRow1 > breadRow2)
          {
            int breadRowTemp1 = breadRow1;
            breadRow1 = breadRow2;
            breadRow2 = breadRowTemp1;
          }
    
          int breadRow1Unchanged = breadRow1;
          int breadRow2Unchanged = breadRow2;
    
          int pathChip1 = Def.chipToBreadboardMap[breadRow1][0];
          int pathChip2 = Def.chipToBreadboardMap[breadRow2][0];
          //int tempPathChip = 0;
    
          uint8_t Xpin = 0;
          uint8_t Ypin1 = Def.chipToBreadboardMap[breadRow1][1];
    
          uint8_t Ypin2 = Def.chipToBreadboardMap[breadRow2][1];
    
          int repeatedConnectionFlag = 0;
          int altPathCounter = 0;
          int altPathChoice = 0;
    
          int XofChip1 = 0;
          int XofChip2 = 0;
    
          int hopLocation[2][7] = {{0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0}};
    
          // {UNCONNchip, connected chip 1, X pin to chip 1, used X connections chip 1, connected chip 2, 
          // X pin to chip 2, used X connections chip 2}
    
          for (int i = 0; i < 17; i++)
          {
			// Conditionals for sorting chip path exceptions
            if (pathChip1 == pathChip2)
            {
                for (int k = 1; k < 16; k += 2)
                {
                  if (Def.availableConnections[pathChip1][k] == 1 && Def.availableConnections[pathChip2][k] == 1)
                  {
                    Xpin = k;
                      Def.availableConnections[pathChip1][k] = 0;
                      Def.availableConnections[pathChip1][k] = 0;
                      Def.connections[pathChip1][Ypin1][k] = breadRow2;
                      Def.connections[pathChip2][Ypin2][k] = breadRow1;
                      printf("\nConnected on same chip via X pin \n");
                      break;
                  }
                }
                break;
            }
    
            if (Def.matrixArrayX[pathChip1][i] == chipToChar(pathChip2))
            {
                for (int k = 0; k < 4; k++)
				{
                  if (Def.connections[pathChip1][Ypin1][k] == breadRow2)
                  {
                      Xpin = i;
                      repeatedConnectionFlag = 1;
                      printf("Same\n");
                      break;
                  }
                  else if (Def.connections[pathChip1][Ypin1][k] == -1 && Def.connections[pathChip2][Ypin2][k] == -1)
                  {
                      Def.connections[pathChip1][Ypin1][k] = breadRow2;
                      Def.connections[pathChip2][Ypin2][k] = breadRow1;
					  printf("Different\n");
					  break;
                  }
                }
    
                if (Def.availableConnections[pathChip1][i] == Def.dataLine || repeatedConnectionFlag == 1)
                {
                  Xpin = i;
                  Def.availableConnections[pathChip1][i] = !Def.dataLine;
                  Def.availableConnections[pathChip2][i] = !Def.dataLine;
    
                  printf("*");
                  break;
                }
                printf("\n");
    
                printf("?");
            }
            printf("\n!");
			
			
			// Primary sorting loop for chips
            if (i >= 16)
            {
    
                for (int l = 0; l < 4; l++)
                {
    
                  int found = 0;
    
                  for (int m = 0; m < 17; m += 4)
                  {
                      if (Def.matrixArrayX[Def.unconnChips[l]][m] == chipToChar(pathChip2))
                      {
                        XofChip1 = m;
                        printf("\n%1c\n", chipToChar(Def.unconnChips[l]));
                        found += 1;
                      }
                      if (Def.matrixArrayX[Def.unconnChips[l]][m] == chipToChar(pathChip1))
                      {
                        XofChip2 = m;
                        printf("%1c", Def.unconnChips[l]);
                        found += 1;
                      }
    
                      if (found == 2)
                      {
                        hopLocation[altPathCounter][0] = Def.unconnChips[l];
                        hopLocation[altPathCounter][1] = pathChip1;
                        hopLocation[altPathCounter][4] = pathChip2;
    
                        for (int n = 0; n < 4; n++)
                        {
                            if (Def.UNCONNconnections[l][XofChip1 + n] == -1)
                            { //make it the X pin in UCONNconnections
                              hopLocation[altPathCounter][2] = XofChip2 + n;
                              hopLocation[altPathCounter][3] = n;
                              break;
                            }
                        }
                        for (int p = 0; p < 4; p++)
                        {
                            if (Def.UNCONNconnections[l][XofChip2 + p] == -1)
                            { //make it the X pin in UCONNconnections
                              hopLocation[altPathCounter][5] = XofChip1 + p;
                              hopLocation[altPathCounter][6] = p;
                              printf("%1d", p);
                              break;
                            }
                        }
                        found = 0;
                        for (int hop = 0; hop < 7; hop++) 
                        {
                          printf("%1d", hopLocation[altPathCounter][hop]);
                          printf("\t");
                        } 
                        printf("\n");
                        altPathCounter += 1;
                    }
                }
              }
			 
              if (altPathCounter >= 2)
              {
                if (hopLocation[0][3] <= hopLocation[1][3] && hopLocation[0][6] <= hopLocation[1][6])
                {
                    altPathChoice = 0;
                }
                else if (hopLocation[0][3] >= hopLocation[1][3] && hopLocation[0][6] >= hopLocation[1][6])
                {
                    altPathChoice = 1;
                }
                else if (hopLocation[0][3] == 4 || hopLocation[0][6] == 4)
                {
                  altPathChoice = 1;
                }
                else if (hopLocation[1][3] == 4 || hopLocation[1][6] == 4)
                {
                    altPathChoice = 0;
                }
                else if ((hopLocation[1][3] + hopLocation[1][6]) < (hopLocation[0][3] + hopLocation[0][6]))
                {
                    altPathChoice = 1;
                }
                else
                {
                    altPathChoice = 0;
                }
                //printf("%2d", hopLocation[altPathChoice][3]);
                //printf("%2d", hopLocation[altPathChoice][6]);
              }
			
			  int chipBackToUNCONNchip = 0;
          
              for (int u = 0; u < 4; u++)
              {
				if (hopLocation[altPathChoice][0] == Def.unconnChips[u])
                {
                    chipBackToUNCONNchip = u;
                }
              }
    
			  Def.availableConnections[hopLocation[altPathChoice][0]][hopLocation[altPathChoice][2]] = 0;
              Def.availableConnections[hopLocation[altPathChoice][0]][hopLocation[altPathChoice][5]] = 0;
              Def.availableConnections[pathChip1][hopLocation[altPathChoice][2]] = 0;
              Def.availableConnections[pathChip2][hopLocation[altPathChoice][5]] = 0;
    
              for (int q = 0; q < 4; q++)
              {
                if (Def.connections[pathChip1][Ypin1][q] == -1 && Def.connections[pathChip2][Ypin2][q] == -1)
                {
                    Def.connections[pathChip1][Ypin1][q] = breadRow2Unchanged;
                    Def.connections[pathChip2][Ypin2][q] = breadRow1Unchanged;
                    break;
                }
              }
    
              for (int r = 0; r < 4; r++)
              {
                if (Def.connections[hopLocation[altPathChoice][0]][Def.unconnYpins[hopLocation[altPathChoice][0]]][r] == -1)
                {
                    Def.connections[hopLocation[altPathChoice][0]][Def.unconnYpins[hopLocation[altPathChoice][0]]][r] = breadRow1Unchanged;
                    Def.connections[hopLocation[altPathChoice][0]][Def.unconnYpins[hopLocation[altPathChoice][0]]][r + 1] = breadRow2Unchanged;
                    break;
                }
              }
    
              if (Def.dataLine == 1)
              {
					Def.UNCONNconnections[chipBackToUNCONNchip][hopLocation[altPathChoice][2]] = breadRow1Unchanged;
					Def.UNCONNconnections[chipBackToUNCONNchip][hopLocation[altPathChoice][5]] = breadRow2Unchanged;
              }
			  else
			  {
					Def.UNCONNconnections[chipBackToUNCONNchip][hopLocation[altPathChoice][2]] = -1;
					Def.UNCONNconnections[chipBackToUNCONNchip][hopLocation[altPathChoice][5]] = -1;
			  }
    
			  //printf("%2d", altPathChoice);
    
		  	  //printf("%2d", altPathCounter);
    
			  printf("\n%1c", chipToChar(pathChip1));
			  printf("\t");
			  printf("%2d", hopLocation[altPathChoice][2]);
			  printf("\t");
			  printf("%2d\n", Ypin1);
    
			  printf("\n%1c", chipToChar(hopLocation[altPathChoice][0]));
			  printf("\t");
			  printf("%2d", hopLocation[altPathChoice][2]);
			  printf("\t");
			  printf("%2d\n", Def.unconnYpins[hopLocation[altPathChoice][0]]);
    
			  //printf(" ");
    
			  printf("\n%1c", chipToChar(hopLocation[altPathChoice][0]));
			  printf("\t");
			  printf("%2d", hopLocation[altPathChoice][5]);
			  printf("\t");
			  printf("%2d\n", Def.unconnYpins[hopLocation[altPathChoice][0]]);
    
			  printf("\n%1c", chipToChar(pathChip2));
			  printf("\t");
			  printf("%2d", hopLocation[altPathChoice][5]);
			  printf("\t");
			  printf("%2d\n", Ypin2);
    
    
			  setAddress(hopLocation[altPathChoice][2], Def.unconnYpins[hopLocation[altPathChoice][0]], chipToChar(hopLocation[altPathChoice][0]));
			  setAddress(hopLocation[altPathChoice][2], Ypin1, chipToChar(pathChip1));
    
			  setAddress(hopLocation[altPathChoice][5], Def.unconnYpins[hopLocation[altPathChoice][0]], chipToChar(hopLocation[altPathChoice][0]));
			  setAddress(hopLocation[altPathChoice][5], Ypin2, chipToChar(pathChip2));
    
			  //printf("%2d", altPathChoice);
			  //break;
		    }
          }
    
		  if (altPathCounter == 0)
		  {
			  printf("\n%1c", chipToChar(pathChip1));
        	  printf("\t");
        	  //printf("%2d", Xaddr1);
        	  printf(" ");
        	  printf("%2d", Xpin);
        	  printf("\t");
        	  //printf("%2d", Yaddr1);
        	  printf(" ");
        	  printf("%2d\n", Ypin1);
    
        	  setAddress(Xpin, Ypin1, chipToChar(pathChip1));
    
        	  setAddress(Xpin, Ypin2, chipToChar(pathChip2));
    
        	  printf("\n%1c", chipToChar(pathChip2));
        	  printf("\t");
        	  //printf(Def.Xaddr2, "%02d");
        	  printf(" ");
        	  printf("%2d", Xpin);
        	  printf("\t");
        	  //printf(Def.Yaddr2, "%02d");
        	  printf(" ");
        	  printf("%2d\n", Ypin2);
      	  }
		 
      	  return 0;
      };

      bool setAddress(uint8_t Xaddr, uint8_t Yaddr, char chip)
      {
      	  uint8_t XaddrFixed = Def.MTfuckedUpTruthTable[Xaddr];
      	  //printf("%2d", XaddrFixed);
      	  gpio_set_dir(AX3, true);
      	  gpio_pull_down(AX3);
		  gpio_set_dir(AX2, true);
      	  gpio_pull_down(AX2);
      	  gpio_set_dir(AX1, true);
      	  gpio_pull_down(AX1);
      	  gpio_set_dir(AX0, true);
      	  gpio_pull_down(AX0);
      
      	  gpio_set_dir(AY2, true);
      	  gpio_pull_down(AY2);
      	  gpio_set_dir(AY1, true);
      	  gpio_pull_down(AY1);
      	  gpio_set_dir(AY0, true);
      	  gpio_pull_down(AY0);
    
      	  selectChip(chip); //datasheet says CS_ should be set while setting up address lines, not sure why though
      	  sleep_ms(10);
      
      	  if(XaddrFixed & 0b00001000)
      	  {
          	  gpio_pull_up(AX3);
          }//this only writes the line high if that bit is set in binary value of Xaddr
          //printf("%4c %2d %8d", HIGH, XaddrFixed, B00001000 );
          if(XaddrFixed & 0b00000100)
      	  {
        	  gpio_pull_up(AX2); //for example Xaddr = 6 or B00000110      
      	  }
          //printf("%4c %2d %8d", HIGH, XaddrFixed, B00000100, );
          if(XaddrFixed & 0b00000010)
      	  {
			  gpio_pull_up(AX1); //this bitwise ANDs Xaddr and a binary value with just one bit set
      	  }
          //printf("%4c %2d %8d", HIGH, XaddrFixed, B00000010);
          if(XaddrFixed & 0b00000001)
      	  {
        	  gpio_pull_up(AX0); //so we get          Xaddr   00000110
      	  }
          //printf("%4c %2d %8d", HIGH, XaddrFixed, B00000001);
    
          // !!!!!!!!!!!!!!!!!!!!!!   AND bit selector   00001000 = 0   !!!!!!!!!!!!!!!!!!!!!!  
		  if(Yaddr & 0b00000100)
      	  {
        	  gpio_pull_up(AY2); //then we AND that again with HIGH (which is just interpreted as 1)
      	  }
          if(Yaddr & 0b00000010)
      	  {
        	  gpio_pull_up(AY1); //to get 1(HIGH) && 0(the result from above) = 0 (which is LOW)
      	  }
          if(Yaddr & 0b00000001)
      	  {
        	  gpio_pull_up(AY0); //we do that for each bit to end up with the address lines LLLLLHHL
      	  }
      	  sleep_ms(10); //allow the lines to settle
    
      	  strobeItIn(); //we could probably do this without passing "chip" and just do all the strobing on both together,
      	  //                    but I don't necessarily want to make MT8816 ignore that many garbage signals with CS_ LOW
      	  //deselectChip();
    
      	  return true;
      };
  
      int lookForPointedConns(void)
      {
    
      	  int testFreq = 20000;
      	  unsigned long duration;
    
      	  //tone(0, testFreq);
    
      	  int probePath[8] = {0, 0, 0, 0, 0, 0, 0, 0};
      	  int probePathChoice[8] = {0, 0, 0, 0, 0, 0, 0, 0}; //
      	  //int testXpin = 0;
		  printf("\n");
      	  for (int i = 0; i < 8; i++)
      	  {
    
        	  for (int k = 0; k < 4; k++)
        	  {
    
				if (Def.connections[i][Def.unconnYpins[i]][k] != -1)
				{
					probePath[i]++;
				}
        	  }
    
        	  printf("%2d\t", probePath[i]);
        	  if(i == 4)
        	  {
				  printf("\n");
			  }
      	  }
      	  printf("\n");
    
      	  for (int i = 0; i < 8; i++)
      	  {
    
        	  if (i == 0 || i == 3 || i == 4 || i == 7)
        	  {
    
				if (probePath[i] == 0 && probePath[(i + 4) % 8] == 0)
				{
    
					for (int j = 0; j < 4; j++)
					{
						probePathChoice[j] = i;
						printf("%2d\t", probePathChoice[j]);
					}
					for (int j = 4; j < 8; j++)
					{
						probePathChoice[j] = (i + 4) % 8;
						printf("%2d\t", probePathChoice[j]);
					}
					printf("\n");
					break;
				}
          	  else if (probePath[i] == 0 && probePath[(i + 3) % 8] == 0)
          	  {
					for (int j = 0; j < 4; j++)
					{
						probePathChoice[j] = i;
						printf("%2d\t", probePathChoice[j]);
					}
					for (int j = 4; j < 8; j++)
					{
						probePathChoice[j] = (i + 3) % 8;
						printf("%2d\t", probePathChoice[j]);
					}
					printf("\n");
					break;
			  }
			}
		  }
    
      	  /*
      	  for (int i = 0; i < 8; i++) 
      	  {
    
			if (i == 1 || i == 2 || i == 5 || i == 6) 
			{
				if (probePath [i] == 0 && i < 4) 
				{
					probePathChoice [8] = i;
					printf("\n");
				} 
				else if (probePath [i] == 0 && i >= 4) 
				{
					probePathChoice [9] = i;
					printf("\n");
				}
			}
          }
          */
    
		  //setAddress(8, 6, 'I'); // PC0 to breadboard pin B16
    
      	  for (int i = 0; i < 8; i++)
      	  {
        	  //int offset = 0;
        	  if (i == 0)
        	  {
				Def.dataLine = 1;
				setAddress(8, probePathChoice[0], 'I');
				setAddress(12, probePathChoice[4], 'I');
    
				printf("\n%2d", probePathChoice[0]);
				printf(" ");
				printf("%2d", probePathChoice[4]);
			  }
        	  else if (i == 4)
        	  {
    
				Def.dataLine = 0;
				setAddress(8, probePathChoice[0], 'I');
				setAddress(12, probePathChoice[4], 'I');
    
				Def.dataLine = 1;
				setAddress(8, probePathChoice[4], 'I');
				setAddress(12, probePathChoice[0], 'I');
    
				printf(" ");
				printf("%\n2d", probePathChoice[4]);
				printf(" ");
				printf("%2d", probePathChoice[0]);
				printf(" ");
        	  }
        	  /*
          	  for (int k = 0; k < 4; k++) 
          	  {
				if (Def.availableConnections [probePathChoice[i]][k*(i%4)] == 1) 
				{
					offset = k;
				} 
				else if (Def.availableConnections [probePathChoice[i]][k * 4] == 1)
				{
					
				}
			  }
			  */
    
        	  for (int j = 0; j < 8; j++)
        	  {
				Def.dataLine = 1;
				if(setAddress((i % 4) * 4, 7, chipToChar(probePathChoice[i % 4]))){};
    
				if(setAddress((i % 4) * 4, j, chipToChar(probePathChoice[((i % 4) + 4) % 8]))){};
    
				duration = 0;
				// ****************************************** ARDUINO IDE FUNCTION <------: pulseIn(3, HIGH, 200);
				//if (duration > 5) 
				//{
				printf("\n%2d %2d %2d", i, j ,duration);
				
				//}
				Def.dataLine = 0;
				if(setAddress((i % 4) * 4, 7, chipToChar(probePathChoice[i % 4]))){};
    
				if(setAddress((i % 4) * 4, j, chipToChar(probePathChoice[((i % 4) + 4) % 8]))){};
			  }
      	  }
		  Def.dataLine = 0;
      	  if(setAddress(8, probePathChoice[0], 'I')){};
      	  if(setAddress(12, probePathChoice[4], 'I')){};
    
      	  if(setAddress(8, probePathChoice[4], 'I')){};
      	  if(setAddress(12, probePathChoice[0], 'I')){};
      	  //noTone(16);
    
      	  return duration;
      };
  
      void strobeItIn(void)
      {
      	  if (Def.dataLine == 0)
      	  {
        	  gpio_pull_down(DATA);
      	  }
      	  else
      	  {
        	  gpio_pull_up(DATA);
      	  }
    
      	  //gpio_set_dir(STROBE, true);
      	  sleep_ms(2);       //Hold time in the datasheet for the MT8816 says this only needs to be 10 nanoseconds
      	  gpio_set_dir(STROBE, true);
      	  gpio_pull_up(STROBE); //but we're not super concerned with speed so I'll give it 1000X more just to be safe
      	  sleep_ms(5);       //Strobe has to be a minimum of 20 nanoseconds, but I dont want to think about the
      	  //printf("!!!!!!!!!!!!");
      	  gpio_pull_down(STROBE); //fact that light only travels like 4 meters in that time through copper
      	  //gpio_set_dir(STROBE, true);
      	  sleep_ms(2);
      	  //gpio_pull_down(DATA);
      	  //sleep_ms(30);
      };
  
      void showSpecialFunctionsMenu(void)
      {
		  printf("\n+++++++++++++++++++++\n");
      	  printf("Power Supplies ON \t\ti\n");
      	  printf("Power Supplies OFF \t\to\n");
      	  printf("Potentiometer \t\t\tp\n");
      	  printf("Analog Read\t\t\tv\n");
      	  printf("GPIO \t\t\t\tg\n");
      	  printf("Back to Menu\t\t\tm\n");
      	  printf("+++++++++++++++++++++\n");
      	  printf("::>\t");
      };

      void getX(void)
      {
		  int rows, posone, postwo;
      	  
		  //This retrieves how many rows and ensures only digits are entered.
		  printf("\nPlease enter how many rows to modify:\n::>\t");
      	  getInp(2);
      	  if( Def.serialBuffer[0] >= 10 || !isdigit(Def.serialBuffer[1]) >= 10 )
      	  {
			  printf("\nPlease enter digits only!\n::>\t");
			  Def.stayInConnectionMenu = 1;
			  return;
		  }
		  Def.serialBuffer[0] *= 10;
      	  rows = Def.serialBuffer[0] + Def.serialBuffer[1];
      	  if (rows > 64)
      	  {
			  printf("\nPlease enter a number lower than 64!!!\n::>\t");
		  }
      	  
      	  if (Def.dataLine == 0)
      	  {
        	  printf("\nDisconnect ");
      	  }
      	  else
      	  {
        	  printf("\nConnect ");
      	  }
      	  printf("%2d rows\n", rows);
      	  
      	  printf("These entries must be two digits each (i.e. ::> 02,12)\nThe comma can be anything, it's simply there for readability.\n");

      	  // This loop takes in the rows to be edited and checks for various flags like power and ground.
		  for(int j=0; j < rows; j++)
		  {
				int bottomX = 0;
				int bottomY = 0;
				int pwr = 0;
				int gnd = 0;
				int incomingValueX = 0;
				int incomingValueY = 0;
				
				printf("\nPlease enter the values (##,##) for row %2d\n::>\t", j);
				getInpV(5);
				
				if (Def.vBuffer[0] == 'b' || Def.vBuffer[1] == 'b' || Def.vBuffer[3] == 'b' || Def.vBuffer[4] == 'b')
				{ 
					bottomX = 1;
				}
				else
				{
					bottomY = 1;
				}
				
				if (bottomX == 1)
				{
					incomingValueX += 30;
				}
				
				if (bottomY == 1)
				{
					incomingValueY += 30;
				}
				
				if (Def.vBuffer[0] == 'p' || Def.vBuffer[1] == 'p' || Def.vBuffer[3] == 'p' || Def.vBuffer[4] == 'p' )
			    {
					pwr = 1;
				}
				if (Def.vBuffer[0] == 'g' || Def.vBuffer[1] == 'g' || Def.vBuffer[3] == 'g' || Def.vBuffer[4] == 'g' )
				{
					gnd = 1;
				}
				
				if (pwr == 1 && gnd == 1)
				{
					printf("\nDon't short power directly to ground! Retrying...\n");
					Def.stayInConnectionMenu = 1;
					return;
				}
				if( Def.vBuffer[0] <= 9 && Def.vBuffer[1] <= 9 )
				{
					posone = ( Def.vBuffer[0] * 10 ) + Def.vBuffer[1];
					incomingValueX += posone;
				}
				if( Def.vBuffer[3] <= 9  && Def.vBuffer[4] <= 9 )
				{
					postwo = ( Def.vBuffer[3] * 10 ) + Def.vBuffer[4];
					incomingValueY += postwo;
				}
				
				//this checks to make sure the values you entered aren't above the maximum size specified in Xsize
			    if ( incomingValueX >= Def.Xsize )
	       	    { 
	         	    printf("\nPlease enter a number for X between 0 and ");
	         	    printf("%2d", Def.Xsize - 1);
	           	    return;
	       	    }
	       	    
	       	    if ( incomingValueY >= Def.Ysize )
				{ 
					printf("\nPlease enter a number for Y between 0 and ");
					printf("%2d", Def.Ysize - 1);
					return;
				}
				
				if (pwr == 1) 
			    {
			 	    if (incomingValueX >= 8 && incomingValueX <= 39) 
			 	    {
						  incomingValueY = 31;
				    } 
				    else 
				    {
						  incomingValueY = 63;
				    }
			    }
				if (gnd == 1) 
				{
					if (incomingValueX >= 24 && incomingValueX <= 55) 
					{
						  incomingValueY = 32;
					} 
					else 
					{
						  incomingValueY = 0;
					}
				}
				
				//now that we know our inputs are good, we can put them into Xaddr and Yaddr
				Def.GbreadRow1 = incomingValueX; 
				//setGbread1(Def.GbreadRow1);
				Def.GbreadRow2 = incomingValueY;
				//setGbread2(Def.GbreadRow2);
				
				// Check path for sanity
				if (findPath(Def.GbreadRow1, Def.GbreadRow2) == -1)
				{
					return;
				}
				
		  }
		  // Drop to basic menu for verification and mods
		  bool retry = true;
		  do
		  {
	     	  switch( getXMenu() )
	     	  {
				  case 0:
					printf("\nRetrying...\n");
					retry = true;
					break;
				  case 1:
					retry = false;
					break;
				  default:
					printf("\nIssue with menu function detected!\n");
					retry = true;
					break;
			  }
		  }while(retry);
      };
      
      int getXMenu(void)
      {
		  printf("\nPlease select from the following options:\n");
		  printf("Go to main menu \t(m)\nDisconnect rows \t(d}\nConnect rows \t(c)\n");
		  printf("Show connections \t(y)\nShow Arrays \t(x)\nClear Connections \t(r)\n");
		  printf("::>\t");
		  getInp(1);
      	  switch (Def.serialBuffer[0])
          {
				case 'm':
					printf("\n");
					Def.stayInConnectionMenu = 0;
					return 1;
					break;
				case 'd':
					Def.dataLine = 0;
					getInp(1);
					printf("\nDisconnect %2d rows\n", Def.serialBuffer[0]);
					return 1;
					break;					
				case 'c':
					Def.dataLine = 1;
					getInp(1);
					printf("\nConnect %2d rows\n", Def.serialBuffer[0]);
					return 1;
					break;
				case 'y':
					showConnections();
					return 0;
					break;
				case 'x':
					showArrays();
					return 0;
					break;
				case 'r':
					clearConnections();
					printf("\nCleared!\n");
					return 0;
					break;
				/*
				case 't':
					break;
				case 'b':
					break;
				*/
				default:
					printf("\nInvalid value: %1d\n");
					printf("\nPlease make a valid selection, lower case only!\n(m,d,c,y,x,r,t,b)\n::> ");
					return 0;
					break;
		  }
	  };
    
      void showMenuOptions(void)
      {
      	  printf("\n-------------------------\n");
      	  printf("Connect Nodes \t\t\tc\n");
      	  printf("Disconnect Nodes \t\td\n");
      	  printf("Show Frontend Connections \ty\n");
      	  printf("Show Backend Connections \tx\n");
      	  printf("Reset Connections\t\tr\n");
      	  printf("Special Funtions Menu\t\ts\n");
      	  printf("Back to Menu\t\t\tm\n");
      	  printf("Enter any number to begin editing connections\n");
      	  printf("-------------------------\n");
      	  printf("::>\t");
      };
    
      void selectMenuItem(void)
      {
		  
		  //this waits for you choose an item
      	  getInp(1);
      	  
      	  //Def.altPath = -1;
      	  
       	  if (isdigit(Def.serialBuffer[0]) >= 0 && Def.serialBuffer[0] <= 9)
       	  {
         	  Def.dataLine = 1;
         	  Def.stayInConnectionMenu = 1;
         	  return;
       	  }
    
       	  //switch (tolower(Def.serialBuffer[0]))
       	  switch(Def.serialBuffer[0])
       	  {
           	  case 'm':
             	  Def.stayInConnectionMenu = 0;
             	  return;
           	  case 'c':
             	  Def.dataLine = 1;
             	  Def.stayInConnectionMenu = 1;
             	  return;
           	  case 'd':
             	  Def.dataLine = 0;
             	  Def.stayInConnectionMenu = 1;
             	  return;
           	  case 'r':
             	  clearConnections();
             	  sleep_ms(20);
             	  printf("\nCleared!\n");
             	  break;
           	  case 'x':
             	  showArrays();
             	  break;
           	  case 'y':
             	  showConnections();
             	  break;
           	  case 's':
             	  Def.stayInConnectionMenu = 2;
             	  return;
           	  case 'i':
             	  powerSupplyMenu();
             	  break;
           	  case 'o':
             	  powerSupply(0,0,0);
             	  break;
           	  case 'p':
             	  potentiometer();
             	  break;
           	  case 'f':
             	  lookForPointedConns();
             	  break;
			  default:
				  printf("\nPlease enter a valid option!!!\n");
				  Def.stayInConnectionMenu = 0;
				  return;
				  
       	  }
      
      };
    
      char chipToChar(int chip)
      {
       	  switch (chip)
       	  {
           	  case 0:
             	  return 'A';
           	  case 1:
             	  return 'B';
           	  case 2:
             	  return 'C';
           	  case 3:
             	  return 'D';
           	  case 4:
             	  return 'E';
           	  case 5:
             	  return 'F';
           	  case 6:
             	  return 'G';
           	  case 7:
             	  return 'H';
           	  case 8:
             	  return 'I';
           	  default:
             	  return '0';
       	  }
      };
    
      void showArrays(void)
      {
       	  for (int i = 0; i < 8; i++)
       	  {
         	  printf("\nChip ");
         	  printf("%1c", chipToChar(i));
         	  printf("\n");
    
			  for (int j = 0; j < 16; j++)
           	  {
					if (chipToChar(i) < Def.matrixArrayX[i][j])
					{
						printf("%1c ", chipToChar(i));
						printf("%1c", Def.matrixArrayX[i][j]);
					}
					else
					{
						printf("%1c ", Def.matrixArrayX[i][j]);
						printf("%1c", chipToChar(i));
					}
					printf(" ");
					printf("%1c", Def.availableConnections[i][j]);
					printf("\t");
					if(j == 7)
					{
						printf("\n");
					}
			  }
         	  printf("\n");
       	  }
       	  printf("\n");
      };
    
      void showConnections(void)
      {
       	  int chipOrder[9] = {0, 1, 2, 3, 7, 6, 5, 4};
       	  int count = 0;
       	  int countConn = 0;
       	  int countDig = 0;
    
       	  for (int i = 0; i < 8; i++)
       	  {
    
         	  (i == 4) ? printf("\n") : printf("\n");
    
         	  printf("\nChip ");
         	  printf("%1c", chipToChar(chipOrder[i]));
         	  printf("\n");
    
         	  for (int j = 0; j < 8; j++)
         	  {
				countDig = 0;
				countConn = 0;
				//this is a nightmare
				if (count <= 6 && count >= 0)
				{ 
					//printf(" ");
					printf("%2d", count + 1);
				}
				else if (count == 7)
				{
					printf("0 *");
				}
				else if (count <= 38 && count >= 32)
				{
					printf("\b"); //backspace
					printf("b");
					printf("%2d", count - 31);
				}
				else if (count == 39)
				{
					printf("\bb0* ");
				}
				else if (count >= 32)
				{
					printf("\b");
					printf("b");
					printf("%2d", count - 32);
				}
				else
				{
					/// printf(" ");
					printf("%2d", count);
				}
				if (count == 31 || count == 63)
				{
					printf("*");
				}
				else if (count != 7 && count != 39)
				{
					printf(" ");
				}
    
				if ((count <= 9 || (count >= 32 && count <= 41)) && count != 7 && count != 39)
				{
					printf(" ");
				}
    
				count++;
				printf(" ");
				if (Def.connections[chipOrder[i]][j][0] == -1 && Def.connections[chipOrder[i]][j][1] == -1 && Def.connections[chipOrder[i]][j][2] == -1 && Def.connections[chipOrder[i]][j][3] == -1)
				{
					printf(".\t");
				}
				else
				{
					countDig = 0;
					for (int k = 0; k < 4; k++)
					{
						if (Def.connections[chipOrder[i]][j][k] != -1)
						{
							if (countConn >= 1 && countConn < 4)
							{
								printf("','");
								countDig++;
							}
							countConn++;
							//printf("%2d", Def.connections[chipOrder[i]][j][k]);
    
							if (Def.connections[chipOrder[i]][j][k] >= 32)
							{
								printf("b");
								printf("%2d", Def.connections[chipOrder[i]][j][k] - 30);
								// if ((Def.connections[chipOrder[i]][j][k] - 30) > 9) 
								//{
								//  	countDig++;
								//}
							}
							else
							{
								//printf(" ");
								printf("b");
								printf("%2d", Def.connections[chipOrder[i]][j][k]);
							}
							if (Def.connections[chipOrder[i]][j][k] > 9)
							{
								countDig++;
							}
							countDig++;
						}
					}
					if (countDig <= 3)
					{
						printf("\t");
						//printf("%2d", countDig);
						countDig = 0;
					}
				}
				printf("\t");
			  }
			  //if (count == 32) printf("\n");
		  }
      };
      
      //asserts whichever chip select line we send it 
      void selectChip(char chip)
      {
		  chip = chipToChar(chip);
      	  gpio_pull_down(CS_A);
      	  gpio_pull_down(CS_B);
      	  gpio_pull_down(CS_C);
      	  gpio_pull_down(CS_D);
      	  gpio_pull_down(CS_E);
      	  gpio_pull_down(CS_F);
      	  gpio_pull_down(CS_G);
      	  gpio_pull_down(CS_H);
      	  gpio_pull_down(CS_I);
      	  sleep_ms(2);
      	  //printf("%1c", chipToChar(chip));
    
      	  if (chip == 'A' || chip == 'a')
      	  {
        	  gpio_pull_up(CS_A); //datasheet says CS_ should be set while setting up address lines, not sure why though
        	  //printf("%01c", chipToChar(chip));
        	  return;
      	  }
      	  if (chip == 'B' || chip == 'b')
      	  {
        	  gpio_pull_up(CS_B);
        	  return;
      	  }
      	  if (chip == 'C' || chip == 'c')
      	  {
        	  gpio_pull_up(CS_C);
        	  return;
      	  }
      	  if (chip == 'D' || chip == 'd')
      	  {
        	  gpio_pull_up(CS_D);
        	  return;
      	  }
      	  if (chip == 'E' || chip == 'e')
      	  {
        	  gpio_pull_up(CS_E);
        	  return;
      	  }
      	  if (chip == 'F' || chip == 'f')
      	  {
        	  gpio_pull_up(CS_F);
        	  return;
      	  }
      	  if (chip == 'G' || chip == 'g')
      	  {
        	  gpio_pull_up(CS_G);
        	  return;
      	  }
      	  if (chip == 'H' || chip == 'h')
      	  {
        	  gpio_pull_up(CS_H);
        	  return;
     	  }
      	  if (chip == 'I' || chip == 'i')
      	  {
        	  gpio_pull_up(CS_I);
        	  return;
      	  }
      };
    
	  //this should be fairly obvious
      void deselectChip(void)
      { 
		  gpio_pull_down(CS_A);
      	  gpio_pull_down(CS_B);
      	  gpio_pull_down(CS_C);
      	  gpio_pull_down(CS_D);
      	  gpio_pull_down(CS_E);
      	  gpio_pull_down(CS_F);
      	  gpio_pull_down(CS_G);
      	  gpio_pull_down(CS_H);
      	  gpio_pull_down(CS_I);
      	  return;
      };
      
      //when you send a reset pulse, all previous connections are cleared on whichever chip is CS_ed but we'll do both for now
      void clearConnections(void)
      { 
		  printf("\nClearing connections now...\n");
		  gpio_pull_up(CS_A);
      	  gpio_pull_up(CS_B);
      	  gpio_pull_up(CS_C);
      	  gpio_pull_up(CS_D);
      	  gpio_pull_up(CS_E);
      	  gpio_pull_up(CS_F);
      	  gpio_pull_up(CS_G);
      	  gpio_pull_up(CS_H);
      	  gpio_pull_up(CS_I);
      	  gpio_pull_up(RESET);
      	  sleep_ms(20); //datasheet says 40 nanoseconds minimum, this is a lot more than that
      	  gpio_pull_down(RESET);
    
      	  gpio_pull_down(CS_A);
      	  gpio_pull_down(CS_B);
      	  gpio_pull_down(CS_C);
      	  gpio_pull_down(CS_D);
      	  gpio_pull_down(CS_E);
     	  gpio_pull_down(CS_F);
      	  gpio_pull_down(CS_G);
      	  gpio_pull_down(CS_H);
      	  gpio_pull_down(CS_I);
    
      	  for (int i = 0; i < 8; i++)
      	  {
        	  for (int j = 0; j < 8; j++)
        	  {
				for (int k = 0; k < 4; k++)
				{
					Def.connections[i][j][k] = -1;
				}
			  }
      	  }
    
      	  for (int i = 0; i < 8; i++)
      	  {
        	  for (int j = 0; j < 16; j++)
        	  {
					Def.availableConnections[i][j] = 1;
        	  }
      	  }
    
      	  for (int i = 0; i < 4; i++)
      	  {
        	  for (int j = 0; j < 16; j++)
        	  {
					Def.UNCONNconnections[i][j] = -1;
        	  }
      	  }
      	  printf("\nAll connections have been flushed!!!\n");
      };
    
      void clearBuffers(void)
      {
		  printf("\nClearing buffers now...\n");
		  sleep_ms(5);
		  //clears the serial buffers by filling them with spaces
      	  for (int i = 0; i < Def.bufLength; i++)
      	  { 
        	  Def.serialBuffer[i] = ' ';
        	  Def.serialBufferX[i] = ' ';
        	  Def.serialBufferY[i] = ' ';
      	  }
      	  /* this is just a trick to clear the serial buffer (if we typed in a really long string it would just keep entering them one after another)
      	  while (uart_is_readable(uart0)) 
      	  {    
				uart_getc(uart0);                    //it reads the serial buffer and then throws away the values, just to clear it
				sleep_ms(100);
          }
          */
		  uart_set_break(uart0, true); //maybe it's just easier to stop and start the serial connection
		  sleep_ms(10);
		  startUart();
		  
		  printf("\nAll buffers have been flushed!!!\n");
      };
    
      void powerSupply(int onOff, int channel, float voltage)
      {
          int setting = abs(voltage*(4095/5));
          gpio_set_dir(27, true);
          gpio_set_dir(28, true);
          if (abs(Def.powerSupplyVoltages[0]) > 0.1 || abs(Def.powerSupplyVoltages[2]) > 0.1 )
          {
				gpio_pull_down(27);
          } 
          else 
          {
				gpio_pull_down(27);
          }
          if (abs(Def.powerSupplyVoltages[1]) > 0.1 || abs(Def.powerSupplyVoltages[3]) > 0.1 )
          {
				gpio_pull_down(28);
          } 
          else 
          {
				gpio_pull_down(28);
          }
        
        
          if (onOff == 1)
          {
				//Def.dataLine = 1;
				//gpio_set_dir(, true);
				//gpio_pull_up(PWR_ENABLE2);
				//gpio_set_dir(20, true);
				//gpio_pull_down(20);
				//gpio_set_dir(21, true);
				//gpio_pull_down(21);
        
				gpio_pull_down(25);
        
				//printf("Adafruit MCP4728 test!");
        
				// Try to initialize!
				/*
				if (!mcp.begin(0x61))
				{
					printf("Failed to find MCP4728 chip");
					mcp.begin(0x61);
				}
				if (channel == 0) 
				{
					mcp.setChannelValue(MCP4728_CHANNEL_C, 0);
					mcp.setChannelValue(MCP4728_CHANNEL_A, setting-82);
        
				} 
				else if (channel == 1) 
				{
					mcp.setChannelValue(MCP4728_CHANNEL_B, setting-550);
        
				} 
				else if (channel == 2) 
				{
				mcp.setChannelValue(MCP4728_CHANNEL_A, 0);
				mcp.setChannelValue(MCP4728_CHANNEL_C, (setting/1.604)-100);
        
				} 
				else if (channel == 3) 
				{
					mcp.setChannelValue(MCP4728_CHANNEL_D, (setting/1.604)-164);
				}
				*/ //Disable until library conversion 
        
          }
          else if (onOff == 0)
          {
                Def.powerSupplyVoltages[0] = 0;
                Def.powerSupplyVoltages[1] = 0;
                Def.powerSupplyVoltages[2] = 0;
                Def.powerSupplyVoltages[3] = 0;
            /*
                mcp.setChannelValue(MCP4728_CHANNEL_A, 0);
                mcp.setChannelValue(MCP4728_CHANNEL_B, 0);
                mcp.setChannelValue(MCP4728_CHANNEL_C, 0);
                mcp.setChannelValue(MCP4728_CHANNEL_D, 0);
            */ //Disable until library conversion
            
                //Def.dataLine = 0;
                //gpio_pull_up(PWR_ENABLE2);
                gpio_pull_down(26);
                gpio_pull_down(27);
          }
          else
          {
				return;
          }
      };
      
      void printPower(void)
      {
          printf("\n\tCurrent Values\n");
          printf("Top Negative \t\t\t");
          printf("0.000000");
          printf("V\n");
        
          printf("Top Positive \t\t\t");
          printf("%1f", (Def.powerSupplyVoltages[0] + Def.powerSupplyVoltages[2]));
          printf("V\n");
        
          printf("\nBottom Negative \t\t");
          if (Def.powerSupplyVoltages[3] < 0) {printf("\b");}
          printf("%1f", Def.powerSupplyVoltages[3]);
          printf("V\n");
        
          printf("Bottom Positive \t\t");
          printf("%1f", Def.powerSupplyVoltages[1]);
          printf("V");
          printf("\n");
	  };
        
      void powerSupplyMenu(void)
      {
          float val1, val2;
          bool retry = true;
          do
          {
			  printPower();
				
			  printf("\nChoose rail to edit:\n0:(+-)Dual Default Top & Bottom (5V+COM)\n1:(+-)Top Rails\n2:(+-)Bottom Rails\nChoose dual default to set both as 5v and Gnd\n::> ");  
			  getInp(1);
          
	          if( Def.serialBuffer[0] = 0	)
	          {
	                Def.powerSupplyVoltages[0] = 5;
	                Def.powerSupplyVoltages[1] = 5;
	                Def.powerSupplyVoltages[2] = 0;
	                Def.powerSupplyVoltages[3] = 0;
	                powerSupply(1,0,Def.powerSupplyVoltages[0]);
	                powerSupply(1,1,Def.powerSupplyVoltages[1]);
	                //powerSupply(1,2,Def.powerSupplyVoltages[2]);
	                powerSupply(1,3,Def.powerSupplyVoltages[3]);
	                printPower();
	                printf("\nEnter y to continue\n::>\t");
	                getInpV(1);
	                if(Def.vBuffer[0] = 'y')
	                {
						retry = false;
					}
	           }
	           else if( Def.serialBuffer[0] = 1	)
	           {
	                printf("\nEnter four digits for voltage Top Positive Rail\n(0V - 8.0V enter n for negative)\n::>\t");
	               	getInpV(4);
	                    
					if (Def.vBuffer[0] >= 0 && Def.vBuffer[0] <= 5) 
					{
						val1 += Def.vBuffer[0];
						val1 += (Def.vBuffer[1] * 0.1);
						val1 += (Def.vBuffer[2] * 0.01);
						val1 += (Def.vBuffer[3] * 0.001);
						Def.powerSupplyVoltages[0] = val1;
						Def.powerSupplyVoltages[2] = 0;
						powerSupply(1,0,Def.powerSupplyVoltages[0]);
						printPower();
						printf("\nEnter y to continue\n::>\t");
		                getInpV(1);
		                if(Def.vBuffer[0] = 'y')
		                {
							retry = false;
						}
					} 
					else if ( Def.vBuffer[0] >= 5 && Def.vBuffer[0] <= 8 )
					{
						val1 += Def.vBuffer[0];
						val1 += (Def.vBuffer[1] * 0.1);
						val1 += (Def.vBuffer[2] * 0.01);
						val1 += (Def.vBuffer[3] * 0.001);           
						Def.powerSupplyVoltages[2] = val1;
						Def.powerSupplyVoltages[0] = 0;
						powerSupply(1,2,Def.powerSupplyVoltages[2]);
						printPower();
						printf("\nEnter y to continue\n::>\t");
		                getInpV(1);
		                if(Def.vBuffer[0] = 'y')
		                {
							retry = false;
						}
					} 
					else 
					{
						printf("\nPlease enter a voltage between 0V - 8.0V only!");
						printPower();
					}    
				}
	            else if( Def.serialBuffer[0] = 2 )
	            {
	                printf("\nEnter four digits for voltage Bottom Positive Rail\n(0V - 8.0V enter n for negative)\n::>\t");
	                getInpV(4);
	                if (Def.vBuffer[0] >= 0 && Def.vBuffer[0] <= 8) 
					{
						val1 += Def.vBuffer[0];
						val1 += (Def.vBuffer[1] * 0.1);
						val1 += (Def.vBuffer[2] * 0.01);
						val1 += (Def.vBuffer[3] * 0.001); 
						Def.powerSupplyVoltages[1] = val1;
						powerSupply(1,1,Def.powerSupplyVoltages[1]);
						printf("\nEnter y to continue\n::>\t");
		                getInpV(1);
		                if(Def.vBuffer[0] = 'y')
		                {
							retry = false;
						}
					} 
					else if (Def.vBuffer[0] < 0)
	                {
	                    if (abs(Def.vBuffer[0]) >= 0 && abs(Def.vBuffer[0]) <= 8) 
	                    {
							Def.powerSupplyVoltages[3] =  0 - abs(Def.vBuffer[0]);
							powerSupply(1,3,Def.powerSupplyVoltages[3]);
							printPower();
							printf("\nEnter y to continue\n::>\t");
			                getInpV(1);
			                if(Def.vBuffer[0] = 'y')
			                {
								retry = false;
							}
	                    } 
	                    else 
	                    {
	                        printf("\nIncorrect Values set! Please try again.\n");
	                        printf("Please enter a voltage between 0V - -8.0V");
	                    }
	                }
	           }      
	           else if( Def.serialBuffer[0] = 'm' )
	           {
	                retry = false;
					Def.stayInConnectionMenu = 0;
	                return;
	           }
	           else if( Def.serialBuffer[0] = 'f' )
	           {
	                retry = false;
					Def.stayInConnectionMenu = 1;
	                return;  
	           }
	           else
	           {
	                printf("\nIncorrect option selected... %1d %1c", Def.serialBuffer[0], Def.serialBuffer[0]);
	                retry = true;
	           }
		  }while(retry);
        
          // powerSupply(1,0,Def.powerSupplyVoltages[0]);
          // powerSupply(1,1,Def.powerSupplyVoltages[1]);
          // powerSupply(1,2,Def.powerSupplyVoltages[2]);
          // powerSupply(1,3,Def.powerSupplyVoltages[3]);
      };	
        
        
      void potentiometer(void)
      {
          char serialBuffer2[8];
                  
          printf("\nSelect Pot a or b(lower case only):\n::>\t");
          getInp(1);

          printf("\nSelect resistance between pin A and the wiper in Kohms (50K max):\n::>\t");
          
          getInpV(1);
        
          int wiperPosition = atoi(Def.vBuffer);
		  char potLetter = Def.serialBuffer[0];
          if (tolower(potLetter == 'a'))
          {
        
            //pot.mcpWrite(volatileWiper0, wiperPosition * 0.194); //*************************DISABLED LIBRARY FUNCTION
          }
          else if (tolower(potLetter == 'b'))
          {
        
            //pot.mcpWrite(volatileWiper1, wiperPosition * 0.194);
          }

          printf("\nChoose row for wiper (p for Power, g for Ground, x for unconnected)\n::>\t");
		  
          getInpV(1);
        
          int potWiper = 0;
          int wiperPin = 5;
          if (potLetter == 'b')
          {
            wiperPin = 8;
          }
        
          if (isdigit(Def.vBuffer[0]) == true)
          {
            //int potWiper = atoi(Def.vBuffer);
          }
          else
          {
	            switch (Def.vBuffer[0])
	            {
			            case 'p':
			              potWiper = -1;
			              if(setAddress(wiperPin, 56, 'I')){};
			              break;
			            case 'g':
			              potWiper = -1;
			              if(setAddress(wiperPin, 32, 'I')){};
			              break;
			            case 'X':
			              potWiper = -1;
			              break;
			            default:
			              potWiper = -1;
			              break;
	            }
          }
          if (potWiper >= 0)
          {
            findPath(48, potWiper);
          }
          
          printf("\nChoose row for pin A\n::>\t");
          getInp(1);
          
          wiperPosition = atoi(Def.serialBuffer);
          int APin = 4;
          if (potLetter == 'b')
          {
            APin = 7;
          }
          
          printf("\nChoose row for wiper (p for Power, g for Ground, x for unconnected)\n::>\t");
		  
          getInpV(1);
          
          int potPinA = atoi(Def.vBuffer);
          if (isdigit(Def.vBuffer[0]) == true)
          {
            //int potPinA = atoi(serialBuffer2);
          }
          else
          {
				switch (Def.vBuffer[0])
				{
					case 'p':
						potPinA = -1;
						if(setAddress(APin, 56, 'I')){};
						break;
					case 'g':
						potPinA = -1;
						if(setAddress(APin, 32, 'I')){};
						break;
					case 'X':
						potPinA = -1;
						break;
					default:
						potPinA = -1;
				}
          }
          if (potPinA >= 0)
          {
            findPath(40, potPinA);
          }
          
          printf("\nChoose row for pin B");
          getInp(1);
          wiperPosition = atoi(Def.serialBuffer);
          int BPin = 5;
          if (potLetter == 'b')
          {
            BPin = 9;
          }
          int potPinB = atoi(Def.serialBuffer);
          if (isdigit(Def.serialBuffer[0]) == true)
          {
            //int potPinB = atoi(serialBuffer2);
          }
          else
          {
				switch (Def.serialBuffer[0])
				{
					case 'p':
						potPinB = -1;
						if(setAddress(BPin, 56, 'I')){};
						break;
					case 'g':
						potPinB = -1;
						if(setAddress(BPin, 32, 'I')){};
						break;
					case 'X':
						potPinB = -1;
						break;
					default:
						potPinB = -1;
						break;
				}
          }
          if (potPinB >= 0)
          {
				findPath(16, potPinB);
          }
        };
        
        void begin()
        {
				//mcp.begin(0x61);
				return;
        };

};
// --------------------------------------- END OF SERIAL CLASS --------------------------------------- //

        

#endif
#endif
