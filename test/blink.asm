	LIST p=16f876
	INCLUDE "p16f876.inc"

	CBLOCK 0x20
CNT1
CNT2
CNT3
	ENDC
	
	ORG 0x00
	GOTO main

	ORG 0x04
	GOTO main


main	BCF STATUS, RP0
	BCF STATUS, RP1
	CLRF PORTC

	BSF STATUS, RP0
	CLRF TRISC ; all pins are outputs

	BCF STATUS, RP0
	GOTO loop

setcnt	MOVLW 0xFF
	MOVWF CNT1
	MOVLW 0xFF
	MOVWF CNT2
	MOVLW 0x01
	MOVWF CNT3
	RETURN

delay	DECFSZ CNT1,1
	GOTO delay
	DECFSZ CNT2,1
	GOTO delay
	DECFSZ CNT3,1
	GOTO delay
	RETURN

loop	MOVLW 0x04 ; set all pins high
	MOVWF PORTC
	CALL setcnt
	CALL delay
	MOVLW 0x00
	MOVWF PORTC
	CALL setcnt
	CALL delay
	GOTO loop

	END
