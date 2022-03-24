          .text                   // executable code follows
          .global _start                  
_start:                             
          MOV     R4, #TEST_NUM   // load the data word ...
          MOV     R5, #0        
		  
MAIN_LOOP:
		  LDR	R1, [R4]
		  CMP	R1, #0
		  
		  BEQ	DISPLAY
		  BL	BEGIN_ONES
		  CMP	R5, R0
		  MOVLT	R5, R0
		  
		  LDR	R1, [R4]
		  BL	BEGIN_ZEROS
		  CMP	R6, R0
		  MOVLT	R6, R0
		  
		  LDR	R1, [R4]
		  BL	ALTERNATE
		  
		  // handle alternate return value
		  LDR	R1, [R4, #4]!
		  B	MAIN_LOOP

BEGIN_ONES:     MOV	R3, R1
				MOV	R0, #0          // R0 will hold the result
				
ONES:     CMP     R3, #0          // loop until the data contains no more 1's
          MOVEQ     PC, LR             
          LSR     R2, R3, #1      // perform SHIFT, followed by AND
          AND     R3, R3, R2      
          ADD     R0, #1          // count the string length so far
          B       ONES
		  
BEGIN_ZEROS:
			MOV	R0, #0
			MVN	R3, R1
			B	ONES
			
ALTERNATE:	PUSH {LR, R8, R9}
			MOV	R0, #0
			LDR	R8, =0x55555555
			MOV	R9, R1
			
			EOR R1, R9, R8
			BL	BEGIN_ZEROS
			CMP	R7, R0
		  	MOVLT	R7, R0
			
			EOR R1, R9, R8
			BL	BEGIN_ONES
			CMP	R7, R0
		  	MOVLT	R7, R0
			
			POP {LR, R8, R9}
			MOV PC, LR
			
			
			
			/* Display R5 on HEX1-0, R6 on HEX3-2 and R7 on HEX5-4 */
DISPLAY:    LDR     R8, =0xFF200020 // base address of HEX3-HEX0
            MOV     R0, R5          // display R5 on HEX1-0
            BL      DIVIDE          // ones digit will be in R0; tens digit in R1
            MOV     R9, R1          // save the tens digit
            
            BL      SEG7_CODE       
            MOV     R4, R0          // save bit code
            MOV     R0, R9          // retrieve the tens digit, get bit code
            
            BL      SEG7_CODE       
            LSL     R0, #8            // left shift 1s digit 8 bits (8-15)
            ORR     R4, R0            // ORR is bitwise OR operation; bitwise number from R5 stored in R4 byte 0-1
            
            MOV        R0, R6            // display R6 on HEX3-2
            BL        DIVIDE            // ones digit will be in R0; tens digit in R1
            MOV        R9, R1            // save the tens digit
            
            BL        SEG7_CODE
            LSL        R0, #16            // left shift 1s digit 16 bits (12-23)
            ORR        R4, R0            // ORR is bitwise OR operation; bitwise number from R6 stored in R4 byte 2
            MOV        R0, R9            // retrieve the tens digit, get bit code
            
            BL        SEG7_CODE
            LSL        R0, #24            // left shift 1s digit 24 bits (24-31)
            ORR        R4, R0            // ORR is bitwise OR operation; bitwise number from R6 stored in R4 byte 3
            STR        R4, [R8]        // we want to store the relevant bit pattern to the assigned memory address to display on HEX display
                                    // display the numbers from R6 and R5
                                    
            LDR     R8, =0xFF200030 // base address of HEX5-HEX4
            MOV        R0, R7            // display R7 on HEX4-5
            BL        DIVIDE            // ones digit will be in R0; tens digit in R1
            MOV        R9, R1            // save the tens digit
            
            BL        SEG7_CODE
            MOV        R4, R0            // save bit code 
            MOV        R0, R9            // retrieve the tens digit, get bit code
            
            BL        SEG7_CODE
            LSL        R0, #8            // left shift 1s digit 8 bits
            ORR        R4, R0            // ORR is bitwise OR operation
            STR     R4, [R8]        // display the number from R7
            
END:        B        END

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

/* Subroutine to convert the digits from 0 to 9 to be shown on a HEX display.
 *    Parameters: R0 = the decimal value of the digit to be displayed
 *    Returns: R0 = bit patterm to be written to the HEX display
 */

SEG7_CODE:  MOV     R1, #BIT_CODES  
            ADD     R1, R0         // index into the BIT_CODES "array"
            LDRB    R0, [R1]       // load the bit pattern (to be returned)
            MOV     PC, LR              

BIT_CODES:  .byte   0b00111111, 0b00000110, 0b01011011, 0b01001111, 0b01100110
            .byte   0b01101101, 0b01111101, 0b00000111, 0b01111111, 0b01100111
            .skip   2      // pad with 2 bytes to maintain word alignment

/* code for Part III (not shown) */
/* Program that counts consecutive 1's */
			

TEST_NUM: .word   0x103fe00f
			.word   0x103fe00f 
			.word   0x0000000f
			.word   0x000000ff
			.word   0x00000fff
			.word   0x0000ffff
			.word   0x000fffff
			.word   0x00ffffff
			.word   0x0fffffff
			.word   0xffffffff
			.word   0x00000009
			.word   0x0000000a
			.word   0x0000ffff
			.word 	0x0

          .end                            