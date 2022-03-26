.define LED_ADDRESS 0x10 
.define HEX_ADDRESS 0x20 
.define SW_ADDRESS 0x30
.define SPEED_1 0xFF
.define SPEED_2 0x7F
.define SPEED_3 0x3F
.define SPEED_4 0x1F


    mv r3, #0x00
MAIN_LOOP:
    mvt r4, #LED_ADDRESS
    // r3 holds the external counter value
    add r3, #1
    st  r3, [r4]

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
    mvt r0, #SPEED_1
    b  DELAY
SET_SPEED_2: 
    mvt r0, #SPEED_2
    b  DELAY
SET_SPEED_3:
    mvt r0, #SPEED_3
    b  DELAY
SET_SPEED_4:
    mvt r0, #SPEED_4
    b  DELAY

DELAY:
    sub r0, #1
    bne DELAY
    b   MAIN_LOOP
     