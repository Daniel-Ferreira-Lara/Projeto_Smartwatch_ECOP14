# 1 "delay.c"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 288 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "C:/Program Files (x86)/Microchip/MPLABX/v5.35/packs/Microchip/PIC18Fxxxx_DFP/1.2.26/xc8\\pic\\include\\language_support.h" 1 3
# 2 "<built-in>" 2
# 1 "delay.c" 2

void atraso(unsigned char N) {
    volatile unsigned char j, k, i, h;
    for (h = 0; h < N; h++)

            for (j = 0; j < 41; j++) {
                for (k = 0; k < 3; k++);
            }
}
