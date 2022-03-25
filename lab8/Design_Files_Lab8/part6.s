.define LED_ADDRESS 0x10 
.define HEX_ADDRESS 0x20 
.define SW_ADDRESS 0x30
.define SPEED_1 50000000
.define SPEED_2 25000000
.define SPEED_3 12500000
.define SPEED_4 6500000

MAIN_LOOP:
    // poll the switches into r1
    mvt r2, #SW_ADDRESS
    ld  r1, [r2]

    sub r1, #1
    beq SET_SPEED_1

    sub r1, #1
    beq SET_SPEED_2

    sub r1, #1
    beq SET_SPEED_3

    sub r1, #1
    beq SET_SPEED_4

    b SET_SPEED_1 // if not any of the above, then set to default speed   


// set delay in r0
SET_SPEED_1: 
    mv r0, SPEED_1
    b  DELAY
SET_SPEED_2: 
    mv r0, SPEED_2
    b  DELAY
SET_SPEED_3:
    mv r0, SPEED_3
    b  DELAY
SET_SPEED_4:
    mv r0, SPEED_4
    b  DELAY

DELAY:
    sub r0, #1
    bne DELAY
    b   MAIN_LOOP
     