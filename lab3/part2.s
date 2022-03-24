.global _start
_start:
	LDR		R4, =0xff200020 //address of seg7
	LDR		R5, =0xff200050 //address of push buttons
	MOV	R1, #0 //stores current number
	MOV	R3, #0	// indicator for counter running

MAIN_LOOP:
	LDR	R0, [R5]
	
	// if KEY pressed toggle state
	CMP	R0, #0
	BNE TOGGLE_STATE
	
	// else count up
	CMP	R3, #0
	BNE	COUNT_UP
	
	B	MAIN_LOOP


TOGGLE_STATE:
	CMP	R3, #0
	ADDEQ	R3, #1
	SUBGT	R3, #1
	B	BUTTON_DOWN
	
COUNT_UP:
	CMP	R1, #99
	MOVEQ	R1, #0 // restart if r1 at MAX
	ADDNE	R1, #1	// else add one
	B	DISPLAY

// display R1 on HEX1-0
DISPLAY:
	PUSH	{R0, R2, R3, R8, R9}
	MOV		R3, R1			// save R1
	MOV     R0, R1          // pass R1 to DIVIDE
	BL      DIVIDE          // ones digit will be in R0; tens digit in R1
	MOV     R9, R1          // save the tens digit
	
	MOV		R1, R0			// pass R0 to seg7_code
	BL      SEG7_CODE       
	MOV     R8, R2          // save bit code
	MOV     R1, R9          // retrieve the tens digit, get bit code
	
	BL      SEG7_CODE       
	LSL     R2, #8            // left shift 1s digit 8 bits (8-15)
	ORR     R8, R2            // ORR is bitwise OR operation; bitwise number from R5 stored in R4 byte 0-1

	BL	SEG7_CODE
	STR	R8, [R4]
	
	MOV	R1, R3
	POP	{R0, R2, R3, R8, R9}

BUTTON_DOWN:
	// wait tilL key is released
	LDR	R0, [R5]
	CMP	R0, #0
	BEQ DO_DELAY
	BGT	BUTTON_DOWN
	
DO_DELAY: 
	LDR R7, =500000 // for CPUlator use =500000 | fpga use =200000000
SUB_LOOP: 
	SUBS R7, R7, #1 
	BNE SUB_LOOP
	BEQ	MAIN_LOOP
	
// Input in R1 
// returns in R2
SEG7_CODE:  MOV     R2, #BIT_CODES  
            ADD     R2, R1         // index into the BIT_CODES "array"
            LDRB    R2, [R2]       // load the bit pattern (to be returned)
            MOV     PC, LR 
			
/* Subroutine to perform the integer division R0 / 10.
 * Returns: quotient in R1, and remainder in R0 */
DIVIDE:     MOV    R2, #0
CONT:       CMP    R0, #10
            BLT    DIV_END
            SUB    R0, #10
            ADD    R2, #1
            B      CONT
DIV_END:    MOV    R1, R2     // quotient in R1 (remainder in R0)
            MOV    PC, LR

BIT_CODES:  .byte   0b00111111, 0b00000110, 0b01011011, 0b01001111, 0b01100110
            .byte   0b01101101, 0b01111101, 0b00000111, 0b01111111, 0b01100111
            .skip   2      // pad with 2 bytes to maintain word alignment
	
.end