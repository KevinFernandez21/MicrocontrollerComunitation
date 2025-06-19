#include <avr/io.h>
#include <util/delay.h>

// Pines (Puerto B)
#define DATA_PIN PB3  // 11
#define LOAD_PIN PB2  // 10
#define CLK_PIN  PB5  // 13
#define PIN_COMIDA     PD5  // Arduino pin 5
#define PIN_GAMEOVER   PD4  // Arduino pin 4


void setup_pins() {
    DDRB |= (1 << DATA_PIN) | (1 << LOAD_PIN) | (1 << CLK_PIN);
    DDRD |= (1 << PIN_COMIDA) | (1 << PIN_GAMEOVER);  // Configura como salida

}

void setup_buttons() {
    DDRB &= ~((1 << PB0) | (1 << PB1));            // PB0 = pin 8, PB1 = pin 9
    DDRD &= ~((1 << PD7) | (1 << PD6));            // PD7 = pin 7, PD6 = pin 6
    PORTB |= (1 << PB0) | (1 << PB1);              // Pull-up para PB0 y PB1
    PORTD |= (1 << PD7) | (1 << PD6);              // Pull-up para PD7 y PD6
}

uint8_t botonPin9Presionado() {
    return !(PINB & (1 << PB1));
}

uint8_t botonPin8Presionado() {
    return !(PINB & (1 << PB0));
}

uint8_t botonPin7Presionado() {
    return !(PIND & (1 << PD7));
}

uint8_t botonPin6Presionado() {
    return !(PIND & (1 << PD6));
}

void notificarComida() {
    PORTD |= (1 << PIN_COMIDA);
    _delay_ms(50);
    PORTD &= ~(1 << PIN_COMIDA);
}


void notificarGameOver() {
    PORTD |= (1 << PIN_GAMEOVER);
    _delay_ms(100);
    PORTD &= ~(1 << PIN_GAMEOVER);
}


void shiftOut(uint8_t data) {
    for (int8_t i = 7; i >= 0; i--) {
        PORTB &= ~(1 << CLK_PIN);
        if ((data >> i) & 0x01)
            PORTB |= (1 << DATA_PIN);
        else
            PORTB &= ~(1 << DATA_PIN);
        PORTB |= (1 << CLK_PIN);
    }
}

void sendToMAX7219(uint8_t address, uint8_t data) {
    PORTB &= ~(1 << LOAD_PIN);
    shiftOut(address);
    shiftOut(data);
    PORTB |= (1 << LOAD_PIN);
}

void setupMAX7219() {
    sendToMAX7219(0x09, 0x00);
    sendToMAX7219(0x0A, 0x08);
    sendToMAX7219(0x0B, 0x07);
    sendToMAX7219(0x0C, 0x01);
    sendToMAX7219(0x0F, 0x00);
}

const uint8_t font[][5] = {
    {0x7E, 0x09, 0x09, 0x09, 0x7E}, // A
    {0x7F, 0x49, 0x49, 0x49, 0x36}, // B
    {0x3E, 0x41, 0x41, 0x41, 0x22}, // C
    {0x7F, 0x41, 0x41, 0x41, 0x3E}, // D
    {0x7F, 0x49, 0x49, 0x49, 0x41}, // E
    {0x7F, 0x09, 0x09, 0x09, 0x01}, // F
    {0x3E, 0x41, 0x49, 0x49, 0x7A}, // G
    {0x7F, 0x08, 0x08, 0x08, 0x7F}, // H
    {0x00, 0x41, 0x7F, 0x41, 0x00}, // I
    {0x20, 0x40, 0x41, 0x3F, 0x01}, // J
    {0x7F, 0x08, 0x14, 0x22, 0x41}, // K
    {0x7F, 0x40, 0x40, 0x40, 0x40}, // L
    {0x7F, 0x02, 0x04, 0x02, 0x7F}, // M
    {0x7F, 0x04, 0x08, 0x10, 0x7F}, // N
    {0x3E, 0x41, 0x41, 0x41, 0x3E}, // O
    {0x7F, 0x09, 0x09, 0x09, 0x06}, // P
    {0x3E, 0x41, 0x51, 0x21, 0x5E}, // Q
    {0x7F, 0x09, 0x19, 0x29, 0x46}, // R
    {0x46, 0x49, 0x49, 0x49, 0x31}, // S
    {0x01, 0x01, 0x7F, 0x01, 0x01}, // T
    {0x3F, 0x40, 0x40, 0x40, 0x3F}, // U
    {0x1F, 0x20, 0x40, 0x20, 0x1F}, // V
    {0x7F, 0x20, 0x18, 0x20, 0x7F}, // W
    {0x63, 0x14, 0x08, 0x14, 0x63}, // X
    {0x07, 0x08, 0x70, 0x08, 0x07}, // Y
    {0x61, 0x51, 0x49, 0x45, 0x43}, // Z
    {0x00, 0x00, 0x00, 0x00, 0x00}, // espacio
    {0x08, 0x08, 0x08, 0x08, 0x08}, // -
    {0x08, 0x00, 0x08, 0x00, 0x00}, // :
    {0x14, 0x14, 0x14, 0x14, 0x14}  // =
};

const char mensaje[] = " DIFICULTAD: A=FACIL B=MEDIO C=DIFICIL ";

int getCharIndex(char c) {
    if (c >= 'A' && c <= 'Z') return c - 'A';
    if (c == ' ') return 26;
    if (c == '-') return 27;
    if (c == ':') return 28;
    if (c == '=') return 29;
    return 26;
}

void mostrarScroll(const char *texto) {
    uint8_t buffer[300] = {0};
    uint16_t index = 0;

    for (uint8_t i = 0; texto[i] != '\0'; i++) {
        int idx = getCharIndex(texto[i]);
        for (uint8_t j = 0; j < 5; j++) buffer[index++] = font[idx][j];
        buffer[index++] = 0x00;
    }

    for (uint16_t pos = 0; pos < index - 8; pos++) {
        if (botonPin9Presionado() || botonPin8Presionado() || botonPin7Presionado() || botonPin6Presionado())
            return;

        for (uint8_t row = 0; row < 8; row++) {
            uint8_t out = 0;
            for (uint8_t bit = 0; bit < 8; bit++) {
                if (buffer[pos + bit] & (1 << row)) out |= (1 << bit);
            }
            sendToMAX7219(row + 1, out);
        }
        _delay_ms(100);
    }
}

void apagarMatriz() {
    for (uint8_t i = 1; i <= 8; i++) {
        sendToMAX7219(i, 0x00);
    }
}

// Generador LFSR de 8 bits para números pseudoaleatorios entre 0-7
uint8_t lfsr = 0xAC;

uint8_t random8() {
    lfsr ^= (lfsr << 7);
    lfsr ^= (lfsr >> 5);
    lfsr ^= (lfsr << 3);
    return lfsr & 0x07;
}

void mostrarGameOver() {
    const char *msg = "  GAME OVER  ";
    uint8_t buffer[300] = {0};
    uint16_t index = 0;

    for (uint8_t i = 0; msg[i] != '\0'; i++) {
        int idx = getCharIndex(msg[i]);
        for (uint8_t j = 0; j < 5; j++) buffer[index++] = font[idx][j];
        buffer[index++] = 0x00;
    }

    for (uint16_t pos = 0; pos < index - 8; pos++) {
        for (uint8_t row = 0; row < 8; row++) {
            uint8_t out = 0;
            for (uint8_t bit = 0; bit < 8; bit++) {
                if (buffer[pos + bit] & (1 << row)) out |= (1 << bit);
            }
            sendToMAX7219(row + 1, out);
        }
        _delay_ms(100);
    }
    notificarGameOver();

    _delay_ms(1000);
        apagarMatriz();
}


void funcionA() {
    typedef struct {
        uint8_t x;
        uint8_t y;
    } Segmento;

    Segmento snake[64];
    uint8_t longitud = 2;
    snake[0] = (Segmento){4, 3};
    snake[1] = (Segmento){3, 3};
    snake[2] = (Segmento){2, 3};
    snake[3] = (Segmento){1, 3};

    uint8_t direccion = 1;
    uint8_t ultima_direccion = 1;

    Segmento comida;
    comida.x = random8();
    comida.y = random8();

    while (1) {
        // Leer botones
        if (botonPin8Presionado() && ultima_direccion != 1) direccion = 0;
        if (botonPin7Presionado() && ultima_direccion != 0) direccion = 1;
        if (botonPin9Presionado() && ultima_direccion != 3) direccion = 2;
        if (botonPin6Presionado() && ultima_direccion != 2) direccion = 3;

        ultima_direccion = direccion;

        // Mover cuerpo (de cola a cabeza)
        for (int i = longitud - 1; i > 0; i--) {
            snake[i] = snake[i - 1];
        }

        // Mover cabeza
        switch (direccion) {
            case 0: snake[0].x = (snake[0].x == 0) ? 7 : snake[0].x - 1; break;
            case 1: snake[0].x = (snake[0].x + 1) % 8; break;
            case 2: snake[0].y = (snake[0].y == 0) ? 7 : snake[0].y - 1; break;
            case 3: snake[0].y = (snake[0].y + 1) % 8; break;
        }

        for (uint8_t i = 1; i < longitud; i++) {
            if (snake[0].x == snake[i].x && snake[0].y == snake[i].y) {
            mostrarGameOver(); //  Muestra mensaje
            return;             // Sale del juego
        }
        }

        // Verificar si comió
        if (snake[0].x == comida.x && snake[0].y == comida.y) {
            if (longitud < 64) longitud++;
            notificarComida();
            comida.x = random8();
            comida.y = random8();
        }

        // Crear buffer de pantalla
        uint8_t buffer[8] = {0};

        // Dibujar cuerpo de la serpiente
        for (uint8_t i = 0; i < longitud; i++) {
            buffer[snake[i].y] |= (1 << snake[i].x);
        }

        // Dibujar comida
        buffer[comida.y] |= (1 << comida.x);

        // Enviar a matriz
        for (uint8_t fila = 0; fila < 8; fila++) {
            sendToMAX7219(fila + 1, buffer[fila]);
        }

        _delay_ms(400);
    }
}






void funcionB() {
    typedef struct {
        uint8_t x;
        uint8_t y;
    } Segmento;

    Segmento snake[64];
    uint8_t longitud = 2;
    snake[0] = (Segmento){4, 3};
    snake[1] = (Segmento){3, 3};
    snake[2] = (Segmento){2, 3};
    snake[3] = (Segmento){1, 3};

    uint8_t direccion = 1;
    uint8_t ultima_direccion = 1;

    Segmento comida;
    comida.x = random8();
    comida.y = random8();

    while (1) {
        // Leer botones
        if (botonPin8Presionado() && ultima_direccion != 1) direccion = 0;
        if (botonPin7Presionado() && ultima_direccion != 0) direccion = 1;
        if (botonPin9Presionado() && ultima_direccion != 3) direccion = 2;
        if (botonPin6Presionado() && ultima_direccion != 2) direccion = 3;

        ultima_direccion = direccion;

        // Mover cuerpo (de cola a cabeza)
        for (int i = longitud - 1; i > 0; i--) {
            snake[i] = snake[i - 1];
        }

        // Mover cabeza
        switch (direccion) {
            case 0: snake[0].x = (snake[0].x == 0) ? 7 : snake[0].x - 1; break;
            case 1: snake[0].x = (snake[0].x + 1) % 8; break;
            case 2: snake[0].y = (snake[0].y == 0) ? 7 : snake[0].y - 1; break;
            case 3: snake[0].y = (snake[0].y + 1) % 8; break;
        }

        for (uint8_t i = 1; i < longitud; i++) {
            if (snake[0].x == snake[i].x && snake[0].y == snake[i].y) {
            mostrarGameOver(); //  Muestra mensaje
            return;             // Sale del juego
        }
        }

        // Verificar si comió
        if (snake[0].x == comida.x && snake[0].y == comida.y) {
            if (longitud < 64) longitud++;
            notificarComida();
            comida.x = random8();
            comida.y = random8();
        }

        // Crear buffer de pantalla
        uint8_t buffer[8] = {0};

        // Dibujar cuerpo de la serpiente
        for (uint8_t i = 0; i < longitud; i++) {
            buffer[snake[i].y] |= (1 << snake[i].x);
        }

        // Dibujar comida
        buffer[comida.y] |= (1 << comida.x);

        // Enviar a matriz
        for (uint8_t fila = 0; fila < 8; fila++) {
            sendToMAX7219(fila + 1, buffer[fila]);
        }

        _delay_ms(200);
    }
}

void funcionC() {
    typedef struct {
        uint8_t x;
        uint8_t y;
    } Segmento;

    Segmento snake[64];
    uint8_t longitud = 2;
    snake[0] = (Segmento){4, 3};
    snake[1] = (Segmento){3, 3};

    uint8_t direccion = 1;
    uint8_t ultima_direccion = 1;

    Segmento comida1, comida2;
    comida1.x = random8();
    comida1.y = random8();
    comida2.x = random8();
    comida2.y = random8();

    while (1) {
        // Leer botones
        if (botonPin8Presionado() && ultima_direccion != 1) direccion = 0;
        if (botonPin7Presionado() && ultima_direccion != 0) direccion = 1;
        if (botonPin9Presionado() && ultima_direccion != 3) direccion = 2;
        if (botonPin6Presionado() && ultima_direccion != 2) direccion = 3;

        ultima_direccion = direccion;

        // Mover cuerpo
        for (int i = longitud - 1; i > 0; i--) {
            snake[i] = snake[i - 1];
        }

        // Mover cabeza
        switch (direccion) {
            case 0: snake[0].x = (snake[0].x == 0) ? 7 : snake[0].x - 1; break;
            case 1: snake[0].x = (snake[0].x + 1) % 8; break;
            case 2: snake[0].y = (snake[0].y == 0) ? 7 : snake[0].y - 1; break;
            case 3: snake[0].y = (snake[0].y + 1) % 8; break;
        }

        // Verificar colisión con el cuerpo
        for (uint8_t i = 1; i < longitud; i++) {
            if (snake[0].x == snake[i].x && snake[0].y == snake[i].y) {
                mostrarGameOver();
                return;
            }
        }

        // Verificar si comió comida1
        if (snake[0].x == comida1.x && snake[0].y == comida1.y) {
            if (longitud < 64) longitud++;
            notificarComida();
            comida1.x = random8();
            comida1.y = random8();
        }

        // Verificar si comió comida2
        if (snake[0].x == comida2.x && snake[0].y == comida2.y) {
            if (longitud < 64) longitud++;
            notificarComida();
            comida2.x = random8();
            comida2.y = random8();
        }

        // Crear buffer
        uint8_t buffer[8] = {0};

        // Dibujar cuerpo
        for (uint8_t i = 0; i < longitud; i++) {
            buffer[snake[i].y] |= (1 << snake[i].x);
        }

        // Dibujar ambas comidas
        buffer[comida1.y] |= (1 << comida1.x);
        buffer[comida2.y] |= (1 << comida2.x);

        // Enviar a matriz
        for (uint8_t fila = 0; fila < 8; fila++) {
            sendToMAX7219(fila + 1, buffer[fila]);
        }

        _delay_ms(100);
    }
}


void funcionD() {
    const char *mensaje = "FIN DE JUEGO";
    uint8_t buffer[300] = {0};
    uint16_t index = 0;

    for (uint8_t i = 0; mensaje[i] != '\0'; i++) {
        int idx = getCharIndex(mensaje[i]);
        for (uint8_t j = 0; j < 5; j++) buffer[index++] = font[idx][j];
        buffer[index++] = 0x00;
    }

    for (uint16_t pos = 0; pos < index - 8; pos++) {
        for (uint8_t row = 0; row < 8; row++) {
            uint8_t out = 0;
            for (uint8_t bit = 0; bit < 8; bit++) {
                if (buffer[pos + bit] & (1 << row)) out |= (1 << bit);
            }
            sendToMAX7219(row + 1, out);
        }
        _delay_ms(100);
    }

    _delay_ms(1000);
    apagarMatriz();
}


int main(void) {
    setup_pins();
    setupMAX7219();
    setup_buttons();

    while (1) {
        mostrarScroll(mensaje);
        if (botonPin9Presionado()) {
            funcionA();
            break;
        }
        if (botonPin8Presionado()) {
            funcionB();
            break;
        }
                if (botonPin7Presionado()) {
            funcionC();
            break;
        }
        if (botonPin6Presionado()) {
            funcionD();
            break;
        }
    }

    while (1); // Fin
}



