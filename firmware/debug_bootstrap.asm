	LIST p=16f876
	INCLUDE "p16f876.inc"

fsr_save EQU H'0024'
pclath_save EQU H'0025'
status_save EQU H'0026'
w_save EQU H'007F'

	ORG 0x0004

context_save
	MOVWF w_save
	SWAPF STATUS,W
	CLRF STATUS
	MOVWF status_save
	MOVF PCLATH,W
	MOVWF pclath_save
	MOVF FSR,W
	MOVWF fsr_save
start_debug
	BSF PCLATH, 4
	BSF PCLATH, 3
	GOTO 0x1F50
context_restore
	MOVF fsr_save,W
	MOVWF FSR
	MOVF pclath_save,W
	MOVWF PCLATH
	SWAPF status_save,W
	MOVWF STATUS
	SWAPF w_save,F
	SWAPF w_save,W
	
	END
