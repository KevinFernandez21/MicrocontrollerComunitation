#line 1 "C:/Users/argen/OneDrive/Documentos/8 Semestre/Embebidos/Deber1Embebidos.c"
void entrada()
{
 Sound_Play(523, 100);
 delay_ms(100);
 Sound_Play(659, 100);
 delay_ms(100);
 Sound_Play(784, 300);
 delay_ms(150);
 Sound_Play(1046, 100);
 delay_ms(100);
 Sound_Play(988, 100);
 delay_ms(100);
 Sound_Play(880, 400);
 Sound_Play(523, 100);
 delay_ms(100);
 Sound_Play(659, 100);
 delay_ms(100);
 Sound_Play(784, 300);
 delay_ms(150);
 Sound_Play(1046, 100);
 delay_ms(100);
 Sound_Play(988, 100);
 delay_ms(100);
 Sound_Play(880, 400);

}

void perder()
{
 Sound_Play(262, 200);
 delay_ms(200);
 Sound_Play(220, 300);
 delay_ms(250);
 Sound_Play(196, 500);
 delay_ms(300);

}
void ganar()
{
 Sound_Play(523, 200);
 delay_ms(100);
 Sound_Play(659, 200);
 delay_ms(100);
 Sound_Play(784, 250);
 delay_ms(150);
 Sound_Play(1046, 300);
 delay_ms(100);
 Sound_Play(523, 200);
 delay_ms(100);
 Sound_Play(659, 200);
 delay_ms(100);
 Sound_Play(784, 250);
 delay_ms(150);
 Sound_Play(1046, 300);
 delay_ms(200);
}
void main() {

 Sound_Init(&PORTC, 3);
 ANSEL = 0;
 ANSELH = 0;
 C1ON_bit = 0;
 C2ON_bit = 0;
 TRISB = 0xF8;

 while (1) {
 if (Button(&PORTB,7,1,1))

 entrada();
 while (RB7_bit) ;

 if (Button(&PORTB,6,1,1))

 perder();
 while (RB6_bit) ;

 if (Button(&PORTB,5,1,1))

 ganar();
 while (RB5_bit) ;
 }
}
