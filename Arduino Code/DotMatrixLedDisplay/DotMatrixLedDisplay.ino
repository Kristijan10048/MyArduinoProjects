//We always have to include the library
#include "LedControlMS.h"

/*
Now we need a LedControl to work with.
***** These pin numbers will probably not work with your hardware *****
pin 12 is connected to the DataIn
pin 11 is connected to the CLK
pin 10 is connected to LOAD
We have only a single MAX72XX.

modified 20 Dec 2017
by Kristijan Petkov

*/
#define NBR_MTX 4

LedControl lc = LedControl(11, 13, 10, NBR_MTX);

String text = "QRGNFDZB";
int digitCounter = 4;

/* we always wait a bit between updates of the display */
unsigned long delaytime = 300;


/**************************************************************************************************
Function:   setup
Parameters : 
	None
	Return val : none
Desc : Initializes objects
****************************************************************************************************/
void setup() {
	/*
	The MAX72XX is in power-saving mode on startup,
	we have to do a wakeup call
	*/
	Serial.begin(9600);
	Serial.println("Setup");
	digitCounter = 0;
	for (int i = 0; i < NBR_MTX; i++)
	{
		lc.shutdown(i, false);

		/* Set the brightness to a medium values */
		lc.setIntensity(i, 1);

		/* and clear the display */
		lc.clearDisplay(i);
	}
}

/**************************************************************************************************
Function:  Main Loop	
	Return val : none
****************************************************************************************************/
void loop()
{
	for (int i = 0; i < text.length() - 4; i++)
	{
		digitCounter++;
		if (digitCounter > 9)
			digitCounter = 0;

		//lc.displayChar(i, lc.getCharArrayPosition(ch));


		for (int charPos = 3; charPos >= 0; charPos--)
		{
			//get current character
			char currChar = text[i];

			//scrol current char
			ScrollLeft(charPos, text[i]);
			ScrollLeft(charPos, text[i + 1]);
			ScrollLeft(charPos, text[i]);
			ScrollLeft(charPos, text[i]);
		}
	}

	delay(1000);
	lc.clearAll();
	delay(200);
}

/**************************************************************************************************
Function: ScrollLeft
Parameters :
	int iPos - Position of the character
	char ch - A character to scrol to the left
	Return val : none

Description : Given character scrolls to the left
****************************************************************************************************/
void ScrollLeft(int iPos, char ch)
{
	int pos = lc.getCharArrayPosition(ch);
	for (int scroll = 1; scroll <= 7; scroll++)
	{
		int col = 7;
		for (int i = scroll; i < 6; i++)
		{
			lc.setColumn(iPos, col, alphabetBitmap[pos][i]);
			col--;
		}

		delay(500);
		lc.clearDisplay(iPos);
	}
}

/**************************************************************************************************
Function: TODO ScrollRight
Parameters :
int iPos - Position of the character
cahr ch - A character to scrol to the left
Return val : none

Description : Given character scrolls to the left
****************************************************************************************************/
void ScrollRight(char ch)
{
	int pos = lc.getCharArrayPosition(ch);
	for (int scroll = 0; scroll < 8; scroll++)
	{
		for (int i = 0; i < 6; i++)
		{
			if (scroll + i < 8)
				lc.setRow(0, scroll + i, alphabetBitmap[pos][i]);
		}

		delay(300);
		lc.clearDisplay(0);
	}
}