// Definiciones de notas musicales (periodos en microsegundos)
#define DO    1915   // C4 - 261 Hz
#define RE    1700   // D4 - 294 Hz
#define MI    1519   // E4 - 330 Hz
#define FA    1432   // F4 - 349 Hz
#define SOL   1275   // G4 - 392 Hz
#define LA    1136   // A4 - 440 Hz
#define SI    1014   // B4 - 494 Hz
#define DO_H   956   // C5 - 523 Hz

// Configuración de pines
#define BUZZER      PORTD.RD0
#define BUZZER_TRIS TRISD.TRISD0
#define BTN_COMIDA  PORTD.RD1
#define BTN_GAMEOVER PORTD.RD2

// SOLUCIÓN 1: Delay manual con bucles
void custom_delay_ms(unsigned int ms) {
    unsigned int i, j;
    for (i = 0; i < ms; i++) {
        for (j = 0; j < 1000; j++) {
            asm nop;  // Ajusta este valor según tu frecuencia de oscilador
        }
    }
}

void custom_delay_us(unsigned int us) {
    unsigned int i;
    for (i = 0; i < us; i++) {
        asm nop;  // Ajusta según tu oscilador
    }
}

// Beep con delays personalizados
void beepPattern(unsigned short repeticiones, unsigned int onTime, unsigned int offTime) {
    unsigned short i;
    for (i = 0; i < repeticiones; i++) {
        BUZZER = 1;
        custom_delay_ms(onTime);
        //delay_ms(onTime);
        BUZZER = 0;
        custom_delay_ms(offTime);
    }
}

// SOLUCIÓN 2: Función con casos específicos para delays comunes
void beepPattern_fixed(unsigned short repeticiones, unsigned char delay_type) {
    unsigned short i;
    for (i = 0; i < repeticiones; i++) {
        BUZZER = 1;
        switch(delay_type) {
            case 1: Delay_ms(50); break;   // Corto
            case 2: Delay_ms(100); break;  // Medio
            case 3: Delay_ms(150); break;  // Largo
            default: Delay_ms(100); break;
        }
        BUZZER = 0;
        switch(delay_type) {
            case 1: Delay_ms(50); break;
            case 2: Delay_ms(100); break;
            case 3: Delay_ms(150); break;
            default: Delay_ms(100); break;
        }
    }
}

// Función para tocar una nota musical con delay personalizado
void playNote(unsigned int frequency_delay, unsigned short duration) {
    unsigned long cycles = (unsigned long)duration * 500UL / frequency_delay;
    unsigned long i;

    for (i = 0; i < cycles; i++) {
        BUZZER = 1;
        custom_delay_us(frequency_delay);
        BUZZER = 0;
        custom_delay_us(frequency_delay);
    }
}

// SOLUCIÓN 3: Versiones específicas para cada sonido
void beep_short() {
    unsigned short i;
    for (i = 0; i < 2; i++) {
        BUZZER = 1;
        Delay_ms(50);
        BUZZER = 0;
        Delay_ms(100);
    }
}

void beep_long() {
    unsigned short i;
    for (i = 0; i < 3; i++) {
        BUZZER = 1;
        Delay_ms(150);
        BUZZER = 0;
        Delay_ms(150);
    }
}

// Melodía de inicio estilo Pac-Man
void startup_melody() {
    // Primera parte - escala ascendente
    playNote(SI, 200);
    playNote(DO_H, 200);
    playNote(RE, 200);
    playNote(MI, 200);
    playNote(FA, 200);
    playNote(SOL, 200);
    playNote(LA, 200);
    playNote(SI, 400);

    Delay_ms(100);

    // Segunda parte - más rápida
    playNote(LA, 150);
    playNote(SOL, 150);
    playNote(FA, 150);
    playNote(MI, 150);
    playNote(RE, 150);
    playNote(DO, 300);

    Delay_ms(200);

    // Final triunfante
    playNote(DO, 100);
    playNote(MI, 100);
    playNote(SOL, 100);
    playNote(DO_H, 400);
}

// Melodía alternativa estilo retro gaming
void retro_startup() {
    // Arpegios ascendentes
    playNote(DO, 100);
    playNote(MI, 100);
    playNote(SOL, 100);
    playNote(DO_H, 200);

    Delay_ms(50);

    playNote(RE, 100);
    playNote(FA, 100);
    playNote(LA, 100);
    playNote(RE, 200);

    Delay_ms(100);

    // Melodía principal
    playNote(SOL, 150);
    playNote(LA, 150);
    playNote(SI, 150);
    playNote(DO_H, 300);
    playNote(SI, 150);
    playNote(LA, 150);
    playNote(SOL, 300);
}

// Fanfarria de inicio corta y potente
void power_up_fanfare() {
    playNote(DO, 80);
    playNote(DO, 80);
    playNote(SOL, 120);
    playNote(SOL, 120);
    playNote(LA, 120);
    playNote(LA, 120);
    playNote(SOL, 250);

    Delay_ms(100);

    playNote(FA, 120);
    playNote(FA, 120);
    playNote(MI, 120);
    playNote(MI, 120);
    playNote(RE, 120);
    playNote(RE, 120);
    playNote(DO, 250);
}

// Sonido de comida usando función específica
void waka_waka_sound() {
    beep_short();  // O usa: beepPattern_fixed(2, 1);
}

// Sonido de game over usando función específica
void game_over_sound() {
    beep_long();   // O usa: beepPattern_fixed(3, 3);
}

// Sonido de victoria
void victory_sound() {
    playNote(DO, 150);
    playNote(MI, 150);
    playNote(SOL, 150);
    playNote(DO_H, 300);
    playNote(SOL, 150);
    playNote(DO_H, 300);
}

// Función principal
void main() {
    // ========================================
    // CONFIGURACIÓN INICIAL
    // ========================================

    // Configurar pines
    BUZZER_TRIS = 0;        // RD0 como salida (buzzer)
    TRISD.TRISD1 = 1;       // RD1 como entrada (botón comida)
    TRISD.TRISD2 = 1;       // RD2 como entrada (botón game over)

    // Estado inicial
    BUZZER = 0;             // Buzzer apagado

    // Configurar puerto D
    PORTD = 0x00;           // Limpiar puerto D

    // ========================================
    // MÚSICA DE INICIO
    // ========================================

    Delay_ms(500);          // Pausa inicial

    // Selecciona UNA de estas melodías:
    startup_melody();       // Melodía principal
    // retro_startup();     // Alternativa retro
    // power_up_fanfare();  // Fanfarria corta

    Delay_ms(1000);         // Pausa antes del bucle principal

    // ========================================
    // BUCLE PRINCIPAL
    // ========================================

    while (1) {
        // Botón de comida
        if (BTN_COMIDA == 1) {
            waka_waka_sound();
            while (BTN_COMIDA == 1); // Anti-rebote simple
            Delay_ms(50);            // Debounce adicional
        }

        // Botón de game over
        if (BTN_GAMEOVER == 1) {
            game_over_sound();
            while (BTN_GAMEOVER == 1); // Anti-rebote simple
            Delay_ms(50);              // Debounce adicional
        }

        // Pequeña pausa para no saturar el procesador
        Delay_ms(10);
    }
}