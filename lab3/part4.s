.global _start
_start:
	LDR		R4, =0xff200020 //address of seg7
	LDR		R5, =0xff200050 //address of push buttons
	MOV	R1, #0 //stores DD
	MOV	R10, #0	// stores SS
	MOV	R3, #0	// indicator for counter running
	
	//BELOW SECTION SETS UP THE HARDWARE TIMER
    LDR    R8, =0xFFFEC600    // timer base address
    LDR R6, =2000000    // Load value (timer frequency = 200 MHz); timer period = 1/200MHz = 5e-9; timer will reach delay in 50e6 * 5e-9 = 0.25 sec
    STR    R6, [R8]    // write to a timer load register
    MOV R6, #0b011    // bit pattern (A=1, E=1)
    STR    R6, [R8, #0x8]    // write to control register (stat timer)

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
	CMP R10, #59
	BEQ RESET_TIMER
	B	COUNT_CONTINUE
	
COUNT_CONTINUE:
	CMP	R1, #99
	MOVEQ	R1, #0 // restart if r1 at MAX
	ADDEQ	R10, #1	// INCREMENT R2
	ADDNE	R1, #1	// else add one TO R1
	B	DISPLAY

RESET_TIMER:
	// CHECK IF SECONDS IS 99, 
	CMP	R1, #99
	BNE	COUNT_CONTINUE
	MOV	R1, #0 // restart if r1 && R2 at MAX
	MOV	R10, #0
	B	MAIN_LOOP
	

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


	// new shit
	
	MOV     R0, R10          // pass R10 to DIVIDE
	BL      DIVIDE          // ones digit will be in R0; tens digit in R1
	MOV     R9, R1          // save the tens digit
	
	MOV		R1, R0			// pass R0 to seg7_code
	BL        SEG7_CODE
    LSL        R2, #16            // left shift 1s digit 16 bits (12-23)
    ORR        R8, R2            // ORR is bitwise OR operation; bitwise number from R6 stored in R4 byte 2
    MOV        R1, R9            // retrieve the tens digit, get bit code
    
    BL        SEG7_CODE
    LSL        R2, #24            // left shift 1s digit 24 bits (24-31)
    ORR        R8, R2            // ORR is bitwise OR operation; bitwise number from R6 stored in R4 byte 3
	
	STR	R8, [R4]
	
	MOV	R1, R3
	POP	{R0, R2, R3, R8, R9}
	B	BUTTON_DOWN
	

BUTTON_DOWN:
	// wait tilL key is released
	LDR	R0, [R5]
	CMP	R0, #0
	BEQ DO_DELAY
	BGT	BUTTON_DOWN

DO_DELAY:            
	LDR    R0, [R8, #0xC]    //read timer status register
	CMP    R0, #0
    BEQ DO_DELAY
    STR    R0, [R8, #0xC] //reset timer bit flag bit (clear F to 0 by writing 1)
	B	MAIN_LOOP

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