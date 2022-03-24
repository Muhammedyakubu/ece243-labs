/* Program that counts consecutive 1's */

          .text                   // executable code follows
          .global _start                  
_start:                             
          MOV     R4, #TEST_NUM   // load the data word ...
          MOV     R5, #0        
		  
MAIN_LOOP:
		  LDR	R1, [R4], #4
		  CMP	R1, #0
		  BEQ	END
		  BL	RESET_START
		  CMP	R5, R0
		  MOVLT	R5, R0
		  
		  B	MAIN_LOOP

END:      B       END      

RESET_START:     MOV     R0, #0          // R0 will hold the result
ONES:     CMP     R1, #0          // loop until the data contains no more 1's
          MOVEQ     PC, LR             
          LSR     R2, R1, #1      // perform SHIFT, followed by AND
          AND     R1, R1, R2      
          ADD     R0, #1          // count the string length so far
          B       ONES   

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
