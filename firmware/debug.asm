	LIST p=16f876
	INCLUDE "p16f876.inc"

CLCK EQU D'6'
DAT EQU D'7'
INBUG EQU D'7'
BIGBUG EQU H'000F'
ICKBUG EQU H'000E'
PORTBALT EQU H'0006'
TRISBALT EQU H'0006'

retadr EQU H'0020'
count1 EQU H'0021'
count2 EQU H'0022'
buf EQU H'0023'
fsr_save EQU H'0024'
pclath_save EQU H'0025'
status_save EQU H'0026'
w_save EQU H'007F'

	ORG 0x1F50

debug_handler
	CLRF STATUS
	BSF STATUS, RP0
	BSF STATUS, RP1
	BCF ICKBUG, 5 ; disable single step
	BTFSS ICKBUG, INBUG
	GOTO debug_bypass
	
wait_ack
	BCF STATUS, RP0

	BCF PORTBALT, CLCK

	BSF STATUS, RP0

	BSF TRISBALT, CLCK
	BCF TRISBALT, DAT

	BCF STATUS, RP0
	BCF PORTBALT, DAT
send_break_addr
	BCF STATUS, RP1
	MOVLW D'8'
	MOVWF count1
	MOVLW D'2'
	MOVWF count2
send_break_addr_load_word
	BTFSS count2, 0
	GOTO send_break_addr_set_bigbug
	BSF STATUS, RP0
	BSF STATUS, RP1
	MOVF ICKBUG, 0
send_break_addr_mov_word
	BCF STATUS, RP0
	BCF STATUS, RP1
	MOVWF buf
send_break_addr_bits
	CLRF retadr
	GOTO send_addr_bits

send_break_addr_bits_post
	MOVLW D'5'
	MOVWF count1

	DECFSZ count2
	GOTO send_break_addr_load_word
	GOTO prompt_cmd
send_break_addr_set_bigbug
	BSF STATUS, RP0
	BSF STATUS, RP1
	MOVF BIGBUG, 0
	GOTO send_break_addr_mov_word
	
send_addr_bits
	BSF STATUS, RP1
	BTFSS PORTBALT, CLCK
	GOTO send_addr_bits
	BCF PORTBALT, DAT
	BCF STATUS, RP1
	BTFSC buf, 0
	GOTO set_bit_high
set_bits_rot
	RRF buf, 1
send_addr_bit_fall
	BSF STATUS, RP1
	BTFSC PORTBALT, CLCK
	GOTO send_addr_bit_fall
	BCF STATUS, RP1
	DECFSZ count1
	GOTO send_addr_bits
	BTFSS retadr, 0
	GOTO send_break_addr_bits_post
	GOTO prompt_cmd
set_bit_high
	BSF STATUS, RP1
	BSF PORTBALT, DAT
	BCF STATUS, RP1
	GOTO set_bits_rot

prompt_cmd
	BSF STATUS, RP1
	BTFSS PORTBALT, CLCK
	GOTO prompt_cmd
prompt_cmd_low
	BSF STATUS, RP0
	BSF TRISBALT, DAT
	BCF STATUS, RP0
	BTFSC PORTBALT, CLCK
	GOTO prompt_cmd_low
	
	BTFSS PORTBALT, DAT
	GOTO debug_end

prompt_cmd_sec
	BTFSS PORTBALT, CLCK
	GOTO prompt_cmd_sec
prompt_cmd_sec_low
	BTFSC PORTBALT, CLCK
	GOTO prompt_cmd_sec_low

	BTFSS PORTBALT, DAT
	GOTO prompt_mem_access
	GOTO prompt_breakpoint
	
prompt_breakpoint
	BCF STATUS, RP1
	CLRF buf
	CLRF retadr
	MOVLW D'8'
	MOVWF count1

	GOTO prompt_data
prompt_breakpoint_lowdatacollect
	MOVF buf, 0

	BSF STATUS, RP1
	BSF STATUS, RP0
	MOVWF BIGBUG
	BCF STATUS, RP1
	BCF STATUS, RP0
	CLRF buf
	MOVLW D'5'
	MOVWF count1
	INCF retadr
	GOTO prompt_data
prompt_breakpoint_highdatacollect
	RRF buf, 1
	RRF buf, 1
	RRF buf, 1
	MOVF buf, 0

	BSF STATUS, RP1
	BSF STATUS, RP0
	MOVWF ICKBUG
	BCF STATUS, RP1
	BCF STATUS, RP0
	GOTO debug_end

prompt_mem_access
	BCF STATUS, RP1
	CLRF buf
	MOVLW D'2'
	MOVWF retadr
	MOVLW D'1'
	MOVWF count1

	GOTO prompt_data
prompt_mem_access_irpcollect
	BSF STATUS, IRP
	BTFSS buf, 7
	BCF STATUS, IRP

	CLRF buf
	MOVLW D'8'
	MOVWF count1
	INCF retadr
	GOTO prompt_data
prompt_mem_access_datacollect
	MOVF buf, 0
	MOVWF FSR
prompt_mem_access_deliver
	MOVF INDF, 0

	MOVWF buf
	MOVLW D'1'
	MOVWF retadr
	MOVLW D'8'
	MOVWF count1

	BSF STATUS, RP0
	BSF STATUS, RP1
	BCF TRISBALT, DAT
	BCF STATUS, RP0
	BCF STATUS, RP1

	GOTO send_addr_bits

prompt_data
	BCF STATUS, RP1
	RRF buf, 1
	BCF buf, 7
	BSF STATUS, RP1
prompt_data_high
	BTFSS PORTBALT, CLCK
	GOTO prompt_data_high
prompt_data_low
	BTFSC PORTBALT, CLCK
	GOTO prompt_data_low
	
	BTFSS PORTBALT, DAT
	GOTO prompt_data_end
	BCF STATUS, RP1
	BSF buf, 7

prompt_data_end:
	BCF STATUS, RP1
	DECFSZ count1
	GOTO prompt_data

	BTFSC retadr, 1
	GOTO prompt_data_end_lastret
	BTFSS retadr, 0
	GOTO prompt_breakpoint_lowdatacollect
	GOTO prompt_breakpoint_highdatacollect
prompt_data_end_lastret
	BTFSS retadr, 0
	GOTO prompt_mem_access_irpcollect
	GOTO prompt_mem_access_datacollect

debug_end
	CLRF STATUS
	BSF STATUS, RP1
	BSF PORTBALT, DAT
	BCF STATUS, RP1

	MOVF fsr_save,W
	MOVWF FSR
	MOVF pclath_save,W
	MOVWF PCLATH
	SWAPF status_save,W
	MOVWF STATUS
	SWAPF w_save,f
	SWAPF w_save,w
	RETURN
debug_bypass
	CLRF PCLATH
	CLRF STATUS
	GOTO 0x0F


	END
