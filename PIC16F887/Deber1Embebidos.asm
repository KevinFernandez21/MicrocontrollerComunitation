
_beep_ms1:

;Deber1Embebidos.c,1 :: 		void beep_ms1() {
;Deber1Embebidos.c,2 :: 		RD0_bit = 1;
	BSF        RD0_bit+0, BitPos(RD0_bit+0)
;Deber1Embebidos.c,3 :: 		Delay_ms(80);
	MOVLW      208
	MOVWF      R12+0
	MOVLW      201
	MOVWF      R13+0
L_beep_ms10:
	DECFSZ     R13+0, 1
	GOTO       L_beep_ms10
	DECFSZ     R12+0, 1
	GOTO       L_beep_ms10
	NOP
	NOP
;Deber1Embebidos.c,4 :: 		RD0_bit = 0;
	BCF        RD0_bit+0, BitPos(RD0_bit+0)
;Deber1Embebidos.c,5 :: 		Delay_ms(40);
	MOVLW      104
	MOVWF      R12+0
	MOVLW      228
	MOVWF      R13+0
L_beep_ms11:
	DECFSZ     R13+0, 1
	GOTO       L_beep_ms11
	DECFSZ     R12+0, 1
	GOTO       L_beep_ms11
	NOP
;Deber1Embebidos.c,6 :: 		}
L_end_beep_ms1:
	RETURN
; end of _beep_ms1

_beep_ms2:

;Deber1Embebidos.c,8 :: 		void beep_ms2() {
;Deber1Embebidos.c,9 :: 		RD0_bit = 1;
	BSF        RD0_bit+0, BitPos(RD0_bit+0)
;Deber1Embebidos.c,10 :: 		Delay_ms(120);
	MOVLW      2
	MOVWF      R11+0
	MOVLW      56
	MOVWF      R12+0
	MOVLW      173
	MOVWF      R13+0
L_beep_ms22:
	DECFSZ     R13+0, 1
	GOTO       L_beep_ms22
	DECFSZ     R12+0, 1
	GOTO       L_beep_ms22
	DECFSZ     R11+0, 1
	GOTO       L_beep_ms22
;Deber1Embebidos.c,11 :: 		RD0_bit = 0;
	BCF        RD0_bit+0, BitPos(RD0_bit+0)
;Deber1Embebidos.c,12 :: 		Delay_ms(60);
	MOVLW      156
	MOVWF      R12+0
	MOVLW      215
	MOVWF      R13+0
L_beep_ms23:
	DECFSZ     R13+0, 1
	GOTO       L_beep_ms23
	DECFSZ     R12+0, 1
	GOTO       L_beep_ms23
;Deber1Embebidos.c,13 :: 		}
L_end_beep_ms2:
	RETURN
; end of _beep_ms2

_waka_waka_sound:

;Deber1Embebidos.c,16 :: 		void waka_waka_sound() {
;Deber1Embebidos.c,17 :: 		beep_ms1(); // tono 1
	CALL       _beep_ms1+0
;Deber1Embebidos.c,18 :: 		beep_ms1(); // tono 2
	CALL       _beep_ms1+0
;Deber1Embebidos.c,19 :: 		}
L_end_waka_waka_sound:
	RETURN
; end of _waka_waka_sound

_game_over_sound:

;Deber1Embebidos.c,22 :: 		void game_over_sound() {
;Deber1Embebidos.c,23 :: 		beep_ms2();
	CALL       _beep_ms2+0
;Deber1Embebidos.c,24 :: 		beep_ms1();
	CALL       _beep_ms1+0
;Deber1Embebidos.c,25 :: 		Delay_ms(150);
	MOVLW      2
	MOVWF      R11+0
	MOVLW      134
	MOVWF      R12+0
	MOVLW      153
	MOVWF      R13+0
L_game_over_sound4:
	DECFSZ     R13+0, 1
	GOTO       L_game_over_sound4
	DECFSZ     R12+0, 1
	GOTO       L_game_over_sound4
	DECFSZ     R11+0, 1
	GOTO       L_game_over_sound4
;Deber1Embebidos.c,26 :: 		beep_ms1();
	CALL       _beep_ms1+0
;Deber1Embebidos.c,27 :: 		}
L_end_game_over_sound:
	RETURN
; end of _game_over_sound

_main:

;Deber1Embebidos.c,29 :: 		void main() {
;Deber1Embebidos.c,30 :: 		TRISD0_bit = 0; // RD0 como salida (buzzer)
	BCF        TRISD0_bit+0, BitPos(TRISD0_bit+0)
;Deber1Embebidos.c,31 :: 		TRISD1_bit = 1; // RD1 como entrada (comida)
	BSF        TRISD1_bit+0, BitPos(TRISD1_bit+0)
;Deber1Embebidos.c,32 :: 		TRISD2_bit = 1; // RD2 como entrada (game over)
	BSF        TRISD2_bit+0, BitPos(TRISD2_bit+0)
;Deber1Embebidos.c,34 :: 		RD0_bit = 0;
	BCF        RD0_bit+0, BitPos(RD0_bit+0)
;Deber1Embebidos.c,36 :: 		while (1) {
L_main5:
;Deber1Embebidos.c,37 :: 		if (RD1_bit == 1) {
	BTFSS      RD1_bit+0, BitPos(RD1_bit+0)
	GOTO       L_main7
;Deber1Embebidos.c,38 :: 		waka_waka_sound();
	CALL       _waka_waka_sound+0
;Deber1Embebidos.c,39 :: 		while (RD1_bit == 1); // Esperar liberación
L_main8:
	BTFSS      RD1_bit+0, BitPos(RD1_bit+0)
	GOTO       L_main9
	GOTO       L_main8
L_main9:
;Deber1Embebidos.c,40 :: 		}
L_main7:
;Deber1Embebidos.c,42 :: 		if (RD2_bit == 1) {
	BTFSS      RD2_bit+0, BitPos(RD2_bit+0)
	GOTO       L_main10
;Deber1Embebidos.c,43 :: 		game_over_sound();
	CALL       _game_over_sound+0
;Deber1Embebidos.c,44 :: 		while (RD2_bit == 1); // Esperar liberación
L_main11:
	BTFSS      RD2_bit+0, BitPos(RD2_bit+0)
	GOTO       L_main12
	GOTO       L_main11
L_main12:
;Deber1Embebidos.c,45 :: 		}
L_main10:
;Deber1Embebidos.c,46 :: 		}
	GOTO       L_main5
;Deber1Embebidos.c,47 :: 		}
L_end_main:
	GOTO       $+0
; end of _main
