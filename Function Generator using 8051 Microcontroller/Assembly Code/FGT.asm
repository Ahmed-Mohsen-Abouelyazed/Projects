DAC EQU P1
SAB BIT P2.5
	
FREQ EQU 80H
DELAY_VALUE EQU 90H

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

ORG 0030H
	SELECT_AGAIN_L:
	MOV FREQ,#20D
	LCALL FUN_SQUARE
	H:SJMP H
END	