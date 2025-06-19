// Beep con pulsos rápidos para que suene limpio en buzzer activo
void beepPattern(unsigned short repeticiones, unsigned short onTime, unsigned short offTime) {
    unsigned short i;
    for (i = 0; i < repeticiones; i++) {
        RD0_bit = 1;
        Delay_ms(onTime);
        RD0_bit = 0;
        Delay_ms(offTime);
    }
}

// Sonido de comida (corto, tipo "waka")
void waka_waka_sound() {
    beepPattern(2, 50, 100);  // 2 beeps cortos
}

// Sonido de game over (3 beeps más marcados)
void game_over_sound() {
    beepPattern(3, 150, 150); // 3 beeps más largos
}

void main() {
    TRISD0_bit = 0; // RD0 = salida (buzzer activo)
    TRISD1_bit = 1; // RD1 = entrada comida
    TRISD2_bit = 1; // RD2 = entrada game over

    RD0_bit = 0;

    while (1) {
        if (RD1_bit == 1) {
            waka_waka_sound();
            while (RD1_bit == 1); // Espera que se suelte
        }

        if (RD2_bit == 1) {
            game_over_sound();
            while (RD2_bit == 1); // Espera que se suelte
        }
    }
}
