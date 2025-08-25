#include <xc.h>
#include <stdint.h>

// ========================== CONFIGURACIÓN GENERAL ========================
// Configuraciones que no dependen del modo de oscilador
#pragma config WDTEN = OFF      
#pragma config LVP = OFF        
#pragma config PBADEN = OFF     
#pragma config CP0 = OFF, CP1 = OFF, CP2 = OFF, CP3 = OFF  
#pragma config BOREN = OFF      
#pragma config FCMEN = OFF      
#pragma config IESO = OFF       

// ========================== MODO DE OSCILADOR ==========================
// 1 = INTOSC interno
// 2 = Cristal externo HS
// 3 = RC externo
#define MODE 1  

#if MODE == 1
    #pragma config FOSC = INTIO67   // Oscilador interno
    #define USE_PLL 0
#elif MODE == 2
    #pragma config FOSC = HSHP     // Cristal HS
    #define USE_PLL 0
#elif MODE == 3
    #pragma config FOSC = RC       // RC externo
    #define USE_PLL 0
#else
    #error "Modo de oscilador inválido"
#endif


// ========================== FRECUENCIA DEL OSCILADOR =====================
#if MODE == 1 || MODE == 2
    #if USE_PLL
        #define _XTAL_FREQ 64000000UL // 16 MHz * 4
    #else
        #define _XTAL_FREQ 16000000UL
    #endif
#else
    #define _XTAL_FREQ 16000000UL // Ajustar según resistencia + condensador
#endif

// ========================== FUNCIONES ==========================
void delay_ms(uint16_t ms) {
    while(ms--) {
        __delay_ms(1);
    }
}

void init_pins(void) {
    // RC0 salida blinker
    TRISCbits.TRISC0 = 0;
    LATCbits.LATC0 = 0;

    // RA6 salida CLKO solo si modo lo permite
    if(MODE == 1 || (MODE == 2 && USE_PLL)) {
        TRISAbits.TRISA6 = 0;
        LATAbits.LATA6 = 0;
    }
}

void init_oscillator(void) {
#if USE_PLL
    OSCCONbits.SPLLEN = 1;  // habilita PLL
#endif
}

// ========================== PROGRAMA PRINCIPAL ==========================
void main(void) {
    init_pins();
    init_oscillator();

    while(1) {
        // RC0 toggle ≈ 500 Hz
        LATCbits.LATC0 = 1;
        delay_ms(1);
        LATCbits.LATC0 = 0;
        delay_ms(1);
    }
}
