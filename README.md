Stratagem hero game from helldivers 2

mcu board - nucleo 474re (can be ported to other nucleo boards or mcu)
additional parts:
1) lcd 20x4 ,i2c or pins doesnt matter,
   for smaller you need to adjust label positions
2) board with 4 buttons
   all gpio pins need pull up , buttons connected to gnd 
Used peripherals:
gpio , i2c , rng

To port it to other mcu you may need to consider changing rng to rand and use seed from systick
