	/* Program that counts consecutive 1's */

          .text                   // executable code follows
          .global _start                  
_start:                             
          MOV     R4, #TEST_NUM   // load the data word ...
          MOV     R5, #0        
		  
MAIN_LOOP:
		  LDR	R1, [R4]
		  CMP	R1, #0
		  
		  BEQ	END
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

END:      B       END      

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
