;*****************************************************************************************
;************************** Function Generator using 8051 Assembly code ******************
;*****************************************************************************************

;******************************* Ports and Pins configuration ****************************

DAC EQU P0
KEYPAD_PORT EQU P1
C1 BIT P1.0
C2 BIT P1.1
C3 BIT P1.2
C4 BIT P1.3
RO4 BIT P1.4
RO3 BIT P1.5
RO2 BIT P1.6
RO1 BIT P1.7
LCD_PORT EQU P3
Rs BIT P2.6
E BIT P2.7
SAB BIT P2.5									;SELECT AGAIN push button

SIGNAL_TYPE EQU 90H  							;a place in RAM to hold the value of signal type
DELAY_VALUE EQU 0AAH							;a place in RAM to hold the value of delay
TEMP EQU 0BBH									;a place in RAM to hold temporary value
FREQ EQU 0CCH									;a place in RAM to hold frequency value in decimal
REG_DEB EQU 0DDH								;a place in RAM to hold old value of pressed button for debounce

;*****************************************************************************************

;********************************* MESSAGES AND KEYPAD VALUES ****************************

ORG 0F100H                						;ERROR MESSAGE                        
	ERR: DB 'ERROR!',0	

ORG 0F200H                                        ;WELCOME MESSAGE
	WELCOME_MSG: DB 'HELLO WORLD!',0

ORG 0F300H                         				;HZ MESSAGE
	HZ_MSG1: DB 'KHz',0
		
ORG 0F400H                                        ;KEYBOARD BUTTONS
	ROW1: DB '7','8','9','/',0
	ROW2: DB '4','5','6','*',0
	ROW3: DB '1','2','3','-',0
	ROW4: DB 'C','0','.','+',0      			

ORG 0F500H                                        ;ENTER FREQUENCY MESSAGE
	FREQUENCY_MSG: DB 'ENTER F =',0

ORG 0F600H										;ENTER SIGNAL TYPE MESSAGE
	SIGNAL_TYPE_MSG: DB 'ENTER SIG TYP:',0
		
ORG 0F700H										;SINE WAVE VALUES IN LOOKUP-TABLE FORM
	SIN_FUN_VALUES: DB 128,150,172,192,210,226,239,248,253,255,253,248,239,226,210,192,172,150,128,106,84,64,46,30,17,8,2,0,2,8,17,30,46,64,84,106,128
		
ORG 0F800H										;OUT OF RANGE MESSAGE
	OUT_OF_RANGE_MESSAGE: DB ' OUT OF RANGE! ',0
	
ORG 0F900H										;SAWTOOTH MESSAGE
	SAWTOOTH_NAME: DB 'SAWTOOTH' ,0
		
ORG 0FA00H										;SQUARE MESSAGE
	SQUARE_NAME: DB 'SQUARE' ,0					
		
ORG 0FB00H										;SINE MESSAGE
	SINE_NAME: DB 'SINE' ,0
		
ORG 0FC00H
	DISPLAY_STATUS_SIG_TYP: DB 'SIG TYP:' ,0

ORG 0FD00H
	DISPLAY_STATUS_FREQ: DB 'F = ' ,0

;*****************************************************************************************

;*********************************** USER-DEFINED FUNCTIONS ******************************

; HELLO MESSAGE DISPLAY FUNCTION

HELLO_MSG:
MOV DPTR,#0F200H					
AGAIN_HELLO_MSG:
CLR A
MOVC A,@A+DPTR
JZ HERE_HELLO_MSG
LCALL LCD_DATA
INC DPTR
SJMP AGAIN_HELLO_MSG
HERE_HELLO_MSG:                                          
LCALL DELAY
LCALL DELAY
RET

;*****************************************************************************************

; ENTER SIGNAL TYPE VALUE MESSAGE DISPLAY FUNCTION

SIG_TYP_MSG:
MOV DPTR,#0F600H					
AGAIN_SIG_TYP_MSG:
CLR A
MOVC A,@A+DPTR
JZ HERE_SIG_TYP_MSG
LCALL LCD_DATA
INC DPTR
SJMP AGAIN_SIG_TYP_MSG
HERE_SIG_TYP_MSG:
LCALL DELAY
LCALL DELAY
RET

;*****************************************************************************************

; ENTER SIGNAL TYPE NAME MESSAGE DISPLAY FUNCTION

SIGNAL_TYPE_STR:
MOV A,SIGNAL_TYPE
CJNE A,#1,NOT_SAWTOOTH_NAME								;IF (A!=1) GOTO NOT_SAWTOOTH_NAME ELSE DISPLAY "SAWTOOTH"

LCALL LCD_2ND_LINE										;DISPLAY "SAWTOOTH" IN LCD'S SECOND LINE (A=1)
MOV DPTR,#0F900H					
AGAIN_SAW_NAME:
CLR A
MOVC A,@A+DPTR
JZ HERE_SAW_NAME
LCALL LCD_DATA
INC DPTR
SJMP AGAIN_SAW_NAME
HERE_SAW_NAME:
LCALL DELAY
LCALL DELAY
RET

NOT_SAWTOOTH_NAME:							
CJNE A,#2,NOT_SQUARE_NAME								;IF (A!=2) GOTO NOT_SQUARE_NAME ELSE DISPLAY "SQUARE"

LCALL LCD_2ND_LINE										;DISPLAY "SQUARE" IN LCD'S SECOND LINE (A=2)
MOV DPTR,#0FA00H					
AGAIN_SQR_NAME:
CLR A
MOVC A,@A+DPTR
JZ HERE_SQR_NAME
LCALL LCD_DATA
INC DPTR
SJMP AGAIN_SQR_NAME
HERE_SQR_NAME:
LCALL DELAY
LCALL DELAY
RET

NOT_SQUARE_NAME:
CJNE A,#3,ERROR_DISPLAY									;IF (A!=3) GOTO ERROR_DISPLAY ELSE DISPLAY "SINE"

LCALL LCD_2ND_LINE										;DISPLAY "SINE" IN LCD'S SECOND LINE (A=3)
MOV DPTR,#0FB00H					
AGAIN_SIN_NAME:
CLR A
MOVC A,@A+DPTR
JZ HERE_SIN_NAME
LCALL LCD_DATA
INC DPTR
SJMP AGAIN_SIN_NAME
HERE_SIN_NAME:
LCALL DELAY
LCALL DELAY
RET

ERROR_DISPLAY:
LCALL ERROR_MESSAGE
RET

;*****************************************************************************************

; ENTER FREQUENCY VALUE MESSAGE DISPLAY FUNCTION

ENT_FREQ_MSG:
LCALL LCD_INIT
MOV DPTR,#0F500H
AGAIN_FREQ_MSG:
CLR A
MOVC A,@A+DPTR
JZ HERE_FREQ_MSG
LCALL LCD_DATA
INC DPTR
SJMP AGAIN_FREQ_MSG
HERE_FREQ_MSG:
RET

;*****************************************************************************************

; FREQUENCY VALUE PROCESSING FUNCTION

GET_FREQUENCY:
LCALL LCD_2ND_LINE
LCALL DELAY

;FOR FIRST DIGIT "HIGH"
LCALL READ_KEY
LCALL DELAY
LCALL LCD_DATA
ANL A,#0FH
MOV 31H,A

;FOR SECOND DIGIT  "LOW"
LCALL DELAY       
LCALL READ_KEY
LCALL DELAY
LCALL LCD_DATA
ANL A,#0FH
MOV 30H,A
MOV B,31H
ORL A,B
MOV FREQ,A

LCALL DELAY
LCALL DELAY
RET

; NOW WE GOT THE FREQUENCY'S DECIMAL VALUE IN 31 (HIGH) & 30 (LOW)

;*****************************************************************************************

; HZ MESSAGE DISPLAY FUNCTION

HZ_MSG:
MOV DPTR,#0F300H
HZ_AGAIN:
CLR A
MOVC A,@A+DPTR
JZ OUT_MSG
LCALL LCD_DATA
INC DPTR
SJMP HZ_AGAIN
OUT_MSG:

LCALL DELAY
LCALL DELAY
LCALL LCD_INIT
RET

;*****************************************************************************************

; ERROR MESSAGE DISPLAY FUNCTION

ERROR_MESSAGE:
LCALL LCD_INIT
MOV DPTR,#0F100H
AGAIN_ERROR:
CLR A
MOVC A,@A+DPTR
JZ HERE_ERROR
LCALL LCD_DATA
INC DPTR
SJMP AGAIN_ERROR
HERE_ERROR:
LCALL DELAY
LCALL DELAY
LCALL LCD_INIT
LJMP START_AGAIN
RET

;*****************************************************************************************

; OUT OF RANGE MESSAGE DISPLAY FUNCTION

OORM:
LCALL LCD_INIT
MOV DPTR,#0F800H
AGAIN_OORM:
CLR A
MOVC A,@A+DPTR
JZ HERE_OORM
LCALL LCD_DATA
INC DPTR
SJMP AGAIN_OORM
HERE_OORM:
LCALL DELAY
LCALL DELAY
LCALL LCD_INIT
RET

;*****************************************************************************************

; SAWTOOTH WAVEFORM GENERATION FUNCTION

FUN_SAWTOOTH:
AGAIN_SAW:
LCALL CALC_DEL_SAW								;SUBROUTINE THAT CALCULATE THE TIMER VALUE TO GENERATE THE SUITABLE DELAY TIME 
MOV R7,#5										;R7 = NUMBER OF SAWTOOTH WAVEFORM'S LEVELS
MOV B,#51										;B  = STEP VALUE
MOV A,#00H										;A  = 0 (INITIAL VALUE)
REPEAT_FUN_SAW:
MOV DAC,A										;OUTPUT DIGITAL VALUE FOR CURRENT LEVEL OF SAWTOOTH WAVEFORM
ADD A,B											;A  = A + B (INCREAMENT THE ACC VALUE BY STEP VALUE)
LCALL DELAY_SAWTOOTH							;SUBROUTINE THAT GENERATE THE DELAY USING TIMERS
DJNZ R7,REPEAT_FUN_SAW							;IF (R7!=0) GOTO REPEAT_FUN_SAW
JB SAB,SELECT_AGAIN_L							;IF (SAB==1) GOTO SELECT AGAIN 			`#FEATURE  :)
SJMP AGAIN_SAW									;GOTO AGAIN_SAW TO REPEAT SUBROUTINE
RET												;RETURN OPERATION

;*****************************************************************************************

; SAWTOOTH DELAY CALCULATION FUNCTION

CALC_DEL_SAW:									;timer value = one's complement (200/f) #EQUATION
MOV A,#200										;A  = 200 (decimal) (numerator)
MOV B,FREQ										;B  = f   (decimal) (denumerator)
DIV AB											;A  = A/B ( integer result )  			#DESIGN FLAW   :(
CPL A											;A  = one's complement (A)
MOV DELAY_VALUE,A 								;DELAY_VALUE = A  (FOR FURTHER PROCESSING)
RET												;RETURN OPERATION

;*****************************************************************************************

; SAWTOOTH DELAY POLLING USING TIMER 1 FUNCTION

DELAY_SAWTOOTH:									;NOTICE THAT THE TIMERS ARE ALREADY CONFIGURED IN THE START OF MAIN PROGRAM
MOV TH1,DELAY_VALUE								;TH1  = DELAY_VALUE
SETB TR1										;TR1  = 1   ( START TIMER )				#FEATURE    (=:<) (CUTE RABIT)
BACK_SAW: JNB TF1,BACK_SAW						;IF (TF1!=1) GOTO BACK_ST 			(POLLING METHOD USING TF1)
RET												;RETURN OPERATION

;*****************************************************************************************

; SQUARE WAVEFORM GENERATION FUNCTION

FUN_SQUARE:
LCALL CALC_DEL_SQU								;SUBROUTINE THAT CALCULATE THE TIMER VALUE TO GENERATE THE SUITABLE DELAY TIME

AGAIN_SQR:
MOV DAC,#0										;OUTPUT LOW DIGITAL VALUE OF SQUARE WAVEFORM								
LCALL DELAY_SQUARE								;SUBROUTINE THAT GENERATE THE DELAY USING TIMERS
MOV DAC,#255									;OUTPUT HIGH DIGITAL VALUE OF SQUARE WAVEFORM	
LCALL DELAY_SQUARE								;SUBROUTINE THAT GENERATE THE DELAY USING TIMERS
JB SAB,SELECT_AGAIN_L							;IF (SAB==1) GOTO SELECT AGAIN 				 #FEATURE  :)
SJMP AGAIN_SQR									;GOTO AGAIN_SQR TO REPEAT SUBROUTINE
RET												;RETURN OPERATION

;*****************************************************************************************

; SQUARE DELAY CALCULATION FUNCTION

CALC_DEL_SQU:									;timer value = one's complement (2*(250/f))	 #EQUATION
MOV A,#250										;A  = 200 (decimal) (numerator)
MOV B,FREQ										;B  = f   (decimal) (denumerator)
DIV AB											;A  = A/B ( integer result )  	  			 #DESIGN FLAW   :(
MOV B,#2										;B  = 2   (decimal)
MUL AB											;A  = A*B (decimal)	
CPL A											;A  = one's complement (A)
MOV DELAY_VALUE,A 								;DELAY_VALUE = A  (FOR FURTHER PROCESSING)
RET												;RETURN OPERATION

;*****************************************************************************************

; SQUARE DELAY POLLING FUNCTION

DELAY_SQUARE:									;NOTICE THAT THE TIMERS ARE ALREADY CONFIGURED IN THE START OF MAIN PROGRAM
MOV TH1,DELAY_VALUE								;TH1  = DELAY_VALUE
SETB TR1										;TR1  = 1   ( START TIMER )				#FEATURE    (=:<) (SECOND CUTE RABIT)
BACK_SQU: JNB TF1,BACK_SQU						;IF (TF1!=1) GOTO BACK_ST 			    (POLLING METHOD USING TF1)
RET												;RETURN OPERATION

;*****************************************************************************************

; SINE WAVEFORM GENERATION FUNCTION

FUN_SINE:										;LOOK-UP TABLE IS USED TO GENERATE SINE WAVEFORM EFFICIENTLY
AGAIN_SINE: 									
MOV DPTR,#0F700H									;DPTR  = 700H        (WHERE SINE VALUES IS STORED IN RAM)
MOV R2,#36										;R2    = NUMBER OF LEVELS IN SINGLE PERIOD (360/10 = 36) 
BACK_SINE:
CLR A											;A     = 0           (TO START FETCHING SINE VALUES FROM INDEX 700H)
MOVC A,@A+DPTR									;A     = DATA STORED IN ADDRESS = DPTR'S VALUE
MOV DAC,A										;OUTPUT DIGITAL VALUE FOR CURRENT LEVEL OF SAWTOOTH WAVEFORM
INC DPTR										;INCREMENT DPTR'S VALUE FOR NEXT SINE VALUE
DJNZ R2, BACK_SINE								;IF (R2!=0) GOTO BACK_SINE
JB SAB,SELECT_AGAIN_L							;;IF (SAB==1) GOTO SELECT AGAIN 				 #FEATURE  :)
SJMP AGAIN_SINE									;GOTO AGAIN_SQR TO REPEAT SUBROUTINE
RET												;RETURN OPERATION

SELECT_AGAIN_L:
LJMP SELECT_AGAIN								;JUMP TO MAIN PROGRAM TO SELECT NEW WAVEFORM PARAMETERS 

;*****************************************************************************************

; DISPLAY CURRENT STATUS OF OUTPUT SIGNAL FUNCTION

DISPLAY_STATUS:

MOV DPTR,#0FC00H
AGAIN_DIS_ST_SIG_TYP:
CLR A
MOVC A,@A+DPTR
JZ HERE_DIS_ST_SIG_TYP
LCALL LCD_DATA
INC DPTR
SJMP AGAIN_DIS_ST_SIG_TYP
HERE_DIS_ST_SIG_TYP:

LCALL SIGNAL_TYPE_STR

LCALL LCD_2ND_LINE
MOV DPTR,#0FD00H
AGAIN_DIS_ST_FREQ:
CLR A
MOVC A,@A+DPTR
JZ HERE_DIS_ST_FREQ
LCALL LCD_DATA
INC DPTR
SJMP AGAIN_DIS_ST_FREQ
HERE_DIS_ST_FREQ:

MOV A,30H
ORL A,#30H
LCALL LCD_DATA

MOV A,31H
ORL A,#30H
LCALL LCD_DATA

MOV A,#00H
LCALL LCD_DATA

LCALL HZ_MSG
RET

;*****************************************************************************************

;******************************** 		MAIN PROGRAM 		******************************

ORG 00H
LJMP MAIN

; INITIATE THE PROGRAM

ORG 30H              

MAIN:
START_AGAIN:	
MOV P0,#0FFH									;P0 is connected to KEYPAD as an input port	
SETB SAB										;SAB "P3.4" is connected to PUSH BUTTON as an input pin
MOV KEYPAD_PORT,#0FH                        	;INITIALIZE KEYPAD'S ROWS AS OUTPUT PINS AND KEYPAD'S COLUMNS AS INPUT PINS
LCALL LCD_INIT									;INITIALIZE 16x2 LCD
MOV TMOD,#20H									;INITIALIZE TIMER 1 IN MODE 2 FOR FURTHER PROCESSING

LCALL HELLO_MSG									;DISPLAY HELLO MESSAEG ON LCD	

SELECT_AGAIN:
LCALL LCD_INIT									;REINITIALIZE LCD FOR DISPLAYING NEW DATA

; INPUT FREQUENCY READING & PROCESSING

LCALL ENT_FREQ_MSG								;DISPLAY ENTER FREQUENCY MESSAGE ON LCD
LCALL GET_FREQUENCY								;GET FREQUENCY VALUE SUBROUTINE 
LCALL HZ_MSG									;DISPLAY HZ MESSAGE ON LCD

; SIGNAL TYPE READING & PROCESSING

LCALL SIG_TYP_MSG								;DISPLAY ENTER SIGNAL TYPE MESSAGE ON LCD
LCALL READ_KEY									;GET SIGNAL TYPE FROM KEYPAD
LCALL LCD_DATA									;DISPLAY SIGNAL TYPE VALUE ON LCD
ANL A,#0FH            							;CONVERT ASCII TO HEX
MOV SIGNAL_TYPE,A								;SAVE IN RAM FOR FURTHER PROCESSING
LCALL SIGNAL_TYPE_STR							;A SUBROUTINE TO DISPLAY SIGNAL TYPE NAME ON LCD'S SECOND LINE 

; WAVE GENERATION PROCESS BASED ON SIGNAL TYPE VALUE AND INPUT FREQUENCY

MOV A,SIGNAL_TYPE

CJNE A,#1,NOT_FUN_SAWTOOTH
LCALL FUN_SAWTOOTH
LCALL DISPLAY_STATUS
BACK_SAB_1:JB SAB,BACK_SAB_1
LJMP SELECT_AGAIN

NOT_FUN_SAWTOOTH:
CJNE A,#2,NOT_FUN_SQUARE
LCALL FUN_SQUARE
LCALL DISPLAY_STATUS
BACK_SAB_2:JB SAB,BACK_SAB_2
LJMP SELECT_AGAIN

NOT_FUN_SQUARE:
CJNE A,#3,ERROR
LCALL FUN_SINE
LCALL DISPLAY_STATUS
BACK_SAB_3:JB SAB,BACK_SAB_3
LJMP SELECT_AGAIN

ERROR:
LCALL ERROR_MESSAGE
LJMP START_AGAIN


;************************************ END OF MAIN PROGRAM ********************************

;*****************************************************************************************



;*****************************************************************************************

;************************************ KEYPAD INTERFACING *********************************

READ_KEY:

CHECK_1:                                        ;CHECK IF A BUTTON WAS PRESSED
CLR RO1
CLR RO2
CLR RO3
CLR RO4

MOV A,KEYPAD_PORT
ANL A,#0FH
CJNE A,#0FH,CHECK_2
SJMP CHECK_1
CHECK_2:                                        ;MAYBE IT WAS JUST A SPIKE NOISE SO, WAIT FOR DEBOUNCE AND CHECK AGAIN
ACALL DEBOUNCE
ACALL DEBOUNCE
MOV A,KEYPAD_PORT
ANL A,#0FH
CJNE A,#0FH,CHECK_ROW
SJMP CHECK_1

CHECK_ROW:                                      ;CHECK IN WHICH ROW THE PRESSED BUTTON IS
CLR RO1
SETB RO2
SETB RO3
SETB RO4
MOV A,KEYPAD_PORT
CJNE A,#01111111B,ROW_1

SETB RO1
CLR RO2
SETB RO3
SETB RO4
MOV A,KEYPAD_PORT
CJNE A,#10111111B,ROW_2

SETB RO1
SETB RO2
CLR RO3
SETB RO4
MOV A,KEYPAD_PORT
CJNE A,#11011111B,ROW_3

SETB RO1
SETB RO2
SETB RO3
CLR RO4
MOV A,KEYPAD_PORT
CJNE A,#11101111B,ROW_4

LJMP CHECK_1

ROW_1:                                          ;PUT THE ADDRESS OF THE RIGHT ROW IN DPTR
MOV DPTR,#ROW1
SJMP FIND
ROW_2:
MOV DPTR,#ROW2
SJMP FIND
ROW_3:
MOV DPTR,#ROW3
SJMP FIND
ROW_4:
MOV DPTR,#ROW4
SJMP FIND

FIND:                                           ;FIND IN WHICH COLUMN THE PRESSED BUTTON IS
RRC A
JNC MATCH
INC DPTR
SJMP FIND

MATCH:                                          ;TAKE THE NUNBER AND WRITE IT ON THE LCD
CLR A
MOVC A,@A+DPTR
CJNE A,#'C',PRINT
LCALL LCD_INIT
LJMP START_AGAIN
RET
PRINT:
RET

;*****************************************************************************************

;*********************************** LCD INTERFACING *************************************
LCD_INIT:
MOV A,#80H                                      ;FORCE CURSOR TO 1ST LINE
ACALL LCD_COMMAND
MOV A,#38H                                      ;2 LINES AND 5*7 MATRIX
ACALL LCD_COMMAND
MOV A,#0EH                                      ;DISPLAY ON, CURSOR BLINKING
ACALL LCD_COMMAND
MOV A,#01H                                      ;CLEAR DISPLAY SCREEN
ACALL LCD_COMMAND
RET
LCD_2ND_LINE:
MOV A,#0C0H                                      ;START 2ND LINE 
ACALL LCD_COMMAND
RET
LCD_COMMAND:                                    ;APPLY A COMMAND TO THE LCD
MOV LCD_PORT,A
CLR Rs
SETB E
NOP
CLR E
ACALL DELAY
RET
LCD_DATA:                                       ;WRITE TO THE LCD
MOV LCD_PORT,A
SETB Rs
SETB E
NOP
NOP
CLR E
ACALL DELAY
ACALL DELAY
RET

DELAY:                                          
MOV R3,#255
LOOP2: MOV R2,#255
LOOP1: DJNZ R2,LOOP1
DJNZ R3,LOOP2
RET

DEBOUNCE:
MOV R3,#60
LOOP4: MOV R2,#255
LOOP3: DJNZ R2,LOOP3
DJNZ R3,LOOP4
RET

SL_CURSOR:
MOV A,#10H                                      ;SHIFT LEFT CURSOR 
ACALL LCD_COMMAND
RET

;*****************************************************************************************
;**********************************    END OF PROGRAM    *********************************
;*****************************************************************************************

END_PROGRAM:

END