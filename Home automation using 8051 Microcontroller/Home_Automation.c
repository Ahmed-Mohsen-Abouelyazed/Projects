#include<reg51.h>

#define LCD P0
sbit rs=P2^6;
sbit en=P2^7;
void lcdcmd (unsigned char x);
void lcdinit(void);
void datawrt (unsigned char y);
void lcd_delete(void);
void lcdstring(unsigned char *ptr);

#define keyport P1
unsigned char colloc, rowloc;
unsigned char keypad[4][4];
unsigned char keypad_read(void);

sbit Fan = P2^0;
sbit TV = P2^1;

sbit buzzer = P2^4;
sbit Gas_sensor = P3^2;

sbit RxD = P3^0;
sbit TxD = P3^1;
void Serialbegin(void);
void Serialwrite(char byte);
void Serialprintln(char *p);

void delay(unsigned int x);

char pass[4] = "4201";
void accept(void);
void wrong(void);
unsigned char counter = 0;

void main(void)
{
		unsigned char i = 0,str,passed = 1,Charin = 0;
		unsigned char input[5] = {0}; // Initialize input array
	
    lcdinit();
		keyport=0xF0;			/*set port direction as input-output*/
		buzzer = 0;
		Gas_sensor = 1;
		
    lcdstring(" hello world! ");
		delay(2000);
		lcdcmd(1);
    lcdstring("HOME AUTOMATION");
		lcdcmd(0xc0);
		lcdstring("    SYSTEM    ");
		delay(3000);
    lcdcmd(1);
		
		start_over:
		
		lcdstring("Enter Password:");
		lcdcmd(0xc0);
		while(i < 4)
		{
				input[i] = keypad_read();
				i++;
		}
		input[4] = '\0'; // Null-terminate the input string
		
		i = 0;
		while(i < 4)
		{
				if(input[i]!=pass[i])
				{
						passed = 0;
				}
		}

		if(passed == 1)
		{
				lcdcmd(1);
				lcdstring("Welcome, Ahmed");
				lcdcmd(0xc0);
				lcdstring("Password Accept");
				delay(2000);
				lcdcmd(1);
		}
		else
		{
				buzzer = 0;
				lcdcmd(1);
				lcdstring("Wrong Passkey");
				lcdcmd(0xc0);
				lcdstring("Access Denied");
				delay(2000);
				lcdcmd(1);
				buzzer = 1; // Reset buzzer
				counter++;
				if(counter == 3)
				{
						while(1);
				}
				else
				{
						goto start_over;
				}
		}
			
		Serialbegin();
		Serialprintln("System Ready...");
		delay(50);
		
		IT0 = 1; // Set the external interrupt 0 to trigger on falling edge
    EX0 = 1; // Enable external interrupt 0
    EA = 1; // Enable global interrupts
		
		while(1)
		{		
			while(!RI);
			Charin=SBUF;
			str=Charin;
			RI=0;
			
			if(str=='1')
			{
				Fan=1;
				Serialprintln(" Fan ON ");
				delay(50);
			}
			
			else if(str=='2')
			{
				Fan=0;
				Serialprintln(" Fan OFF ");
				delay(50);
			}

			 else if(str=='3')
			{
				TV=1;
				Serialprintln(" TV ON ");
				delay(50);
			}

			 else if(str=='4')
			{
				TV=0;
				Serialprintln(" TV OFF ");
				delay(50);
			}
			
			str=0;
		}
}

void delay(unsigned int time)
	{
	 unsigned int i,j;
	 for(i=0;i<time;i++)
	 for(j=0;j<1275;j++);
	}

void Serialwrite(char byte)
	{
		SBUF=byte;
		while(!TI);
		TI=0;
	}

void Serialprintln(char *p)
	{
		while(*p)
		{
			Serialwrite(*p);
			p++;
		}
		Serialwrite(0x0d);
	}


void Serialbegin(void)
	{
		 TMOD=0x20;
		 SCON=0x50;
		 TH1=0xfd;
		 TR1=1;
	}

void lcdcmd (unsigned char x)

	{ 
		 LCD=x;  //send the command to Port 0 on which 16*2 lcd is connected
		 rs=0;  //make RS = 0 for command
		 en=1;  //send a HIGH to LOW pulse on Enable(E) pin to start commandwrite operation 
		 delay(1);
		 en=0;
	}

void lcddata (unsigned char y)

	{ 
		 LCD=y; //send the data to Port 0 on which 16*2 lcd is connected
		 rs=1; //make RS = 1 for command
		 en=1; //send a HIGH to LOW pulse on Enable(E) pin to start datawrite operation
		 delay(1);
		 en=0;
	}
	
void lcdinit (void)
{
	LCD = 0x00;
	lcdcmd(0x38); //8bits & 2lines
	lcdcmd(0x0e); // display on cursor blinking
	lcdcmd(0x01); // clear screen
	lcdcmd(0x80); //force curcor to 1st line
}
	
void lcdstring(unsigned char *ptr)
	{
		while(*ptr)
		{
			lcddata(*ptr);
			ptr++;
		}
	}
	
	unsigned char keypad[4][4] = {{'.','7','4','1'},
															{'0','8','5','2'},
															{'=','9','6','3'},
															{'+','-','*','/'} };

unsigned char keypad_read(void)
{
	do
	{
		keyport = 0xF0;				
		colloc = keyport;
		colloc&= 0xF0;	/* mask port for column read only */
	}while(colloc != 0xF0);		/* read status of column */
	
	do
	{
		do
		{
			delay(20);	/* 20ms key debounce time */
			colloc = (keyport & 0xF0);	/* read status of column */
		}while(colloc == 0xF0);	/* check for any key press */
		
		delay(1);
		colloc = (keyport & 0xF0);
	}while(colloc == 0xF0);
			
	while(1)
	{
		/* now check for rows */
		keyport= 0xFE;											/* check for pressed key in 1st row */
		colloc = (keyport & 0xF0);
		if(colloc != 0xF0)
		{
			rowloc = 0;
			break;
		}

		keyport = 0xFD;									/* check for pressed key in 2nd row */
		colloc = (keyport & 0xF0);
		if(colloc != 0xF0)
		{
			rowloc = 1;
			break;
		}
			
	keyport = 0xFB;			/* check for pressed key in 3rd row */
	colloc = (keyport & 0xF0);
	if(colloc != 0xF0)
	{
		rowloc = 2;
		break;
	}

	keyport = 0xF7;			/* check for pressed key in 4th row */
	colloc = (keyport & 0xF0);
	if(colloc != 0xF0)
	{
		rowloc = 3;
		break;
	}	
}
	
	if(colloc == 0xE0)
	{
		return(keypad[rowloc][0]);
	}		
	else if(colloc == 0xD0)
	{
		return(keypad[rowloc][1]);
	}	
	else if(colloc == 0xB0)
	{
		return(keypad[rowloc][2]);
	}	
	else
	{
		return(keypad[rowloc][3]);
	}	
}

void smoke_detected(void)
{
	lcdcmd(1);
	lcdstring("Fire alarm is ON");
	lcdcmd(0xc0);
	lcdstring("Evacuate NOW!");
	buzzer = 0;
	while(1);
}

void external_interrupt_handler(void) interrupt 0
{
    // Code to execute when the external interrupt occurs
		if (Gas_sensor == 0)
    {
        // Turn on buzzer and display message on LCD
        buzzer = 1;
        lcdcmd(1);
        lcdstring("Gas Detected");
        lcdcmd(0xc0);
        lcdstring("System in Hold");
				while(1);
    }
    // ...// Clear the interrupt flag to enable further interrupts
    EA = 0; // Disable global interrupts
    EX0 = 0; // Clear the external interrupt flag 0
    EA = 1; // Enable global interrupts
}
