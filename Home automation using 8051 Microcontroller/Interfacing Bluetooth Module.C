#include<reg51.h>


sbit Fan=P2^0;

sbit Light=P2^1;

sbit buzzer = P2^4;

sbit Gas_sensor = P3^2;


 char str;

 char Charin=0;


void delay(int time)

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


void Serialbegin()

{

   TMOD=0x20;

   SCON=0x50;

   TH1=0xfd;

   TR1=1;

}


void main()

{
	buzzer = 0;
	Gas_sensor = 1;

  P2=0x00;

  Serialbegin();

  Serialprintln("System Ready...");

  delay(50);

  while(1)

  {

    while(!RI);

    Charin=SBUF;

    str=Charin;

    RI=0;

      if(str=='1')

      {

        Fan=1;

        Serialprintln(" Fan ON");

        delay(50);

      }

      else if(str=='2')

      {

        Fan=0;

        Serialprintln(" Fan OFF");

        delay(50);

      }


       else if(str=='3')

      {

        Light=1;

        Serialprintln(" Light ON");

        delay(50);

      }


       else if(str=='4')

      {

        Light=0;

        Serialprintln(" Light OFF");

        delay(50);

      }
			
			if ( Gas_sensor == 0 )
			{
				buzzer = 1;
				delay(1000);
			}

      str=0;

  }

}