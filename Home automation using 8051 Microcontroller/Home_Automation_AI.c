#include <reg51.h>

// Define LCD port
#define LCD P0

// Define LCD control pins
sbit rs = P2^6;
sbit en = P2^7;

// Function prototypes
void lcdcmd(unsigned char x);
void lcdinit(void);
void datawrt(unsigned char y);
void lcd_delete(void);
void lcdstring(unsigned char *ptr);

// Define keypad port
#define keyport P1

// Keypad variables
unsigned char colloc, rowloc;
unsigned char keypad[4][4];

// Function to read keypad input
unsigned char keypad_read(void);

// Define fan, TV, buzzer, and gas sensor pins
sbit Fan = P2^0;
sbit TV = P2^1;
sbit buzzer = P2^4;
sbit Gas_sensor = P3^2;

// Define serial communication pins
sbit RxD = P3^0;
sbit TxD = P3^1;

// Serial communication functions
void Serialbegin(void);
void Serialwrite(char byte);
void Serialprintln(char *p);

// Delay function
void delay(unsigned int x);

// Password variables
char pass[4] = "4201";
unsigned char counter = 0;

// Function to accept or reject password
void accept(void);
void wrong(void);

void main(void)
{
    // Initialize variables
    unsigned char i = 0, str, passed = 1, Charin = 0;
    unsigned char input[5] = {0}; // Initialize input array

    // Initialize LCD and keypad
    lcdinit();
    keyport = 0xF0; // Set port direction as input-output
    buzzer = 0;
    Gas_sensor = 1;

    // Display welcome message on LCD
    lcdstring(" hello world! ");
    delay(2000);
    lcdcmd(1);
    lcdstring("HOME AUTOMATION");
    lcdcmd(0xc0);
    lcdstring("    SYSTEM    ");
    delay(3000);
    lcdcmd(1);

start_over:

    // Prompt user to enter password
    lcdstring("Enter Password:");
    lcdcmd(0xc0);

    // Read keypad input
    while (i < 4)
    {
        input[i] = keypad_read();
        i++;
    }
    input[4] = '\0'; // Null-terminate the input string

    // Verify password
    i = 0;
    while (i < 4)
    {
        if (input[i] != pass[i])
        {
            passed = 0;
        }
        i++;
    }

    // Handle password verification result
    if (passed == 1)
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
        if (counter == 3)
        {
            while (1);
        }
        else
        {
            goto start_over;
        }
    }

    // Initialize serial communication
    Serialbegin();
    Serialprintln("System Ready...");
    delay(50);

    // Enable external interrupt 0
    IT0 = 1; // Set the external interrupt 0 to trigger on falling edge
    EX0 = 1; // Enable external interrupt 0
    EA = 1; // Enable global interrupts

    // Main loop
    while (1)
    {
        while (!RI);
        Charin = SBUF;
        str = Charin;
        RI = 0;

        // Handle serial input
        if (str == '1')
        {
            Fan = 1;
            Serialprintln(" Fan ON ");
            delay(50);
        }
        else if (str == '2')
        {
            Fan = 0;
            Serialprintln(" Fan OFF ");
            delay(50);
        }
        else if (str == '3')
        {
            TV = 1;
            Serialprintln(" TV ON ");
            delay(50);
        }
        else if (str == '4')
        {
            TV = 0;
            Serialprintln(" TV OFF ");
            delay(50);
        }

        str = 0;
    }
}

// Delay function
void delay(unsigned int time)
{
    unsigned int i, j;
    for (i = 0; i < time; i++)
        for (j = 0; j < 1275; j++);
}

// Serial write function
void Serialwrite(char byte)
{
   SBUF = byte;
    while (!TI);
    TI = 0;
}

// Serial print function
void Serialprintln(char *p)
{
    while (*p)
    {
        Serialwrite(*p);
        p++;
    }
    Serialwrite(0x0d);
}

// Serial initialization function
void Serialbegin(void)
{
    TMOD = 0x20;
    SCON = 0x50;
    TH1 = 0xfd;
    TR1 = 1;
}

// LCD command function
void lcdcmd(unsigned char x)
{
    LCD = x; // Send the command to Port 0 on which 16*2 LCD is connected
    rs = 0; // Make RS = 0 for command
    en = 1; // Send a HIGH to LOW pulse on Enable(E) pin to start command write operation
    delay(1);
    en = 0;
}

// LCD data function
void lcddata(unsigned char y)
{
    LCD = y; // Send the data to Port 0 on which 16*2 LCD is connected
    rs = 1; // Make RS = 1 for data
    en = 1; // Send a HIGH to LOW pulse on Enable(E) pin to start data write operation
    delay(1);
    en = 0;
}

// LCD initialization function
void lcdinit(void)
{
    LCD = 0x00;
    lcdcmd(0x38); // 8bits & 2lines
    lcdcmd(0x0e); // Display on cursor blinking
    lcdcmd(0x01); // Clear screen
    lcdcmd(0x80); // Force cursor to 1st line
}

// LCD string function
void lcdstring(unsigned char *ptr)
{
    while (*ptr)
    {
        lcddata(*ptr);
        ptr++;
    }
}

// Keypad read function
unsigned char keypad_read(void)
{
    do
    {
        keyport = 0xF0; // Check for any key press
        colloc = keyport;
        colloc &= 0xF0; // Mask port for column read only
    } while (colloc!= 0xF0);

    do
    {
        do
        {
            delay(20); // 20ms key debounce time
            colloc = (keyport & 0xF0); // Read status of column
        } while (colloc == 0xF0); // Check for any key press

        delay(1);
        colloc = (keyport & 0xF0);
    } while (colloc == 0xF0);

    while (1)
    {
        // Check for rows
        keyport = 0xFE; // Check for pressed key in 1st row
        colloc = (keyport & 0xF0);
        if (colloc!= 0xF0)
        {
            rowloc = 0;
            break;
        }

        keyport = 0xFD; // Check for pressed key in 2nd row
        colloc = (keyport & 0xF0);
        if (colloc!= 0xF0)
        {
            rowloc = 1;
            break;
        }

        keyport = 0xFB; // Check for pressed key in 3rd row
        colloc = (keyport & 0xF0);
        if (colloc!= 0xF0)
        {
            rowloc = 2;
            break;
        }

        keyport = 0xF7; // Check for pressed key in 4th row
        colloc = (keyport & 0xF0);
        if (colloc!= 0xF0)
        {
            rowloc = 3;
            break;
        }
    }

    if (colloc == 0xE0)
    {
        return (keypad[rowloc][0]);
    }
    else if (colloc == 0xD0)
    {
        return (keypad[rowloc][1]);
    }
    else if (colloc == 0xB0)
    {
        return (keypad[rowloc][2]);
    }
    else
    {
        return (keypad[rowloc][3]);
    }
}

// External interrupt handler
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
        while (1);
    }
    // Clear the interrupt flag to enable further interrupts
    EA = 0; // Disable global interrupts
    EX0 = 0; // Clear the external interrupt flag 
}