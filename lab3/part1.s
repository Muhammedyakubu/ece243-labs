.global _start
_start:
	LDR		R4, =0xff200020 //address of seg7
	LDR		R5, =0xff200050 //address of push buttons
	MOV	R1, #0 //stores current number

MAIN_LOOP:

	LDR	R0, [R5]
	
	// A bunch of if else statements
	
	// if nothing pressed restart loop
	CMP	R0, #0
	BEQ MAIN_LOOP
	
	// if has just been reset, R1 is -1, display 0
	CMP	R1, #-1
	BEQ	RESET0
	
	CMP	R0, #1
	BEQ RESET0	
	
	// for keys 1-3
	CMP	R0, #4
	
	// if lt 4 then R0 = 2 => key 1 pressed, add one
	BLT	ADD_ONE
	
	BEQ	SUB_ONE
	
	// if gt 4 then R0 = 8 => key 3 pressed, clear
	BGT	CLEAR_DISPLAY
	


CLEAR_DISPLAY:
	// just set seg7 to 0s and set the flag
	MOV	R1, #0
	STR	R1, [R4]
	
	MOV R1, #-1
	B	BUTTON_DOWN

RESET0:
	// set current number to zero and display
	MOV	R1, #0
	B	DISPLAY

ADD_ONE:
	CMP R1, #9
	BEQ	BUTTON_DOWN	// do nothing if r1 at MAX
	
	// else increment by 1 & display
	ADD	R1, #1
	B	DISPLAY

SUB_ONE:
	CMP R1, #0
	BEQ	BUTTON_DOWN	// do nothing if r1 at MIN
	
	// else sub 1 and display
	SUB	R1, #1
	B	DISPLAY

DISPLAY:
	BL	SEG7_CODE
	STR	R2, [R4]

BUTTON_DOWN:
	// wait tilL key is released
	LDR	R0, [R5]
	CMP	R0, #0
	BEQ MAIN_LOOP
	B	BUTTON_DOWN



SEG7_CODE:  MOV     R2, #BIT_CODES  
            ADD     R2, R1         // index into the BIT_CODES "array"
            LDRB    R2, [R2]       // load the bit pattern (to be returned)
            MOV     PC, LR 

BIT_CODES:  .byte   0b00111111, 0b00000110, 0b01011011, 0b01001111, 0b01100110
            .byte   0b01101101, 0b01111101, 0b00000111, 0b01111111, 0b01100111
            .skip   2      // pad with 2 bytes to maintain word alignment
			
.end