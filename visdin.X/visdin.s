;*******************************************************************************
;                                                                              *
;    Filename:      Primer_Lab.s                                              *
;    Date:          06/02/2019                                             *
;    File Version:  v1.0                                                     *
;    Authors:       Carlos Felipe Alonso Caro                              *
;     Christian Danilo Reyes Parra
;     Viviana Lemus Ruiz
;    Company:       Escuela Colombiana de Ingenieria                       *
;    Description:   es leidy viviana           *
;                                                                              *
;*******************************************************************************
 
;*******************************************************************************
; Processor Inclusion
;*******************************************************************************
 
.equ __30F4011, 1         ;select the 33FJ128MC802 as processor
;.equ __24FJ64GB002, 1      ;select the 24FJ64GB002 as processor
 
; include the processor definitions
 
.include "xc.inc"       ;generic definition.
;.include "p33FJ128GP802.inc" ;Esta dentro del xc.inc
;.include "p33FJ128MC802.inc"   ;specific definition.
;.include "p24FJ64GB002.inc"    ;specific definition.            
 
;*******************************************************************************
; Configuration Word Setup
;*******************************************************************************
 
;*******************************************************************************
;Global Declarations:
;*******************************************************************************
 
.global _main           ;The label for the first line of code. If the
                ;assembler encounters "_main", it invokes the
                ;start-up code that initializes data sections
 
;*******************************************************************************
;Constants stored in Program space & progrm memory
;*******************************************************************************
.equ    K1, 2666        ;pre-compiler constants
.equ    K2, 993
;.equ    LED, RA0        ;generic use of port rapid prototyping
;.equ    LED_PORT, LATA      ;generic use of port rapid prototyping
;.ifdef __30F4011
;    .equ Fosc, (8000000)        ;Clock for PIC24        
;.else
;    .equ Fosc, (7370000)        ;Clock for dspic33      
;.endif
     
.equ FCY, (Fosc/2)
.equ Ftmr, (1000)               ;constant for 1khz timer interrupt
.equ PS, 1
.equ T_Period , ((FCY/PS/Ftmr)-1)        
;*******************************************************************************
;Code Section in Program Memory
;*******************************************************************************
.data
DATO0: .word 0x0000      ;initializes DISPLAY DATA  
DATO1: .word 0x0000
DATO2: .word 0x0000
DATO3: .word 0x0000
  
.bss 
DISP0: .space 2
DISP1: .space 2
DISP2: .space 2
DISP3: .space 2
CONT:  .space 2; Variable cont sin inicializar con 2 bytes de disponibilidad

.text               ;Start of Code section

_main:
    ;conf
    mov     #0xFF80,W0  ;configurar port B <6:0> salida
    mov     W0,TRISB 
    mov     #0xFFFF,W0  ;configurar port E <2:0> entrada
    mov     W0,TRISE
    mov     #0xFFF0,W0   ;configura PORT D <3:0> salida
    mov     W0,TRISD   
    ;conf
    
 config_TMR1:
    bset    IPC0, #T1IP2        ;1. Set interrupt priority, T1IP as 7 maximum priority
    bset    IPC0, #T1IP1
    bset    IPC0, #T1IP0
    mov     #T_Period,W0        ;2. Configure peripheral on OFF state
    mov     W0, PR1             ;Set compare register to desired value
    clr     TMR1                ;Clear timer counter
    mov     #0x0000,W0          ;Configute T1 as timer with FCY, T1 ON
    mov     W0,T1CON   
    bclr    IFS0, #T1IF         ;3. Clear timer interrupt flag
    bset    IEC0, #T1IE         ;4. Enable timer interrupt
    bset    T1CON, #TON         ;5. Turn on Timer
    return   
 inicio:  
    pregunta:
    mov #0x0000,W0   ;inicializar w0 en 0 por precaucion
    mov PORTE,W0    ;leer entradaE y dejarla en w0
    mov #0x0000,W1    ;asignar 0 a w1
    cpseq W0,W1    ;compara entradaE con 0
    bra segc       ;si no es 0, ir a programa que compara con 1
    bra mensaje0   ;si es 0, configurar mensaje0
    
    segc:
    mov #0x0001,W1     ;asigna 1 a w1
    cpseq W0,W1     ;compara entradaE con 1
    bra terc     ;si no es 1, ir a programa que compara con 2
    bra mensaje1 ; si es 1, configurar mensaje 1
    
    terc:
    mov #0x0002,W1     ;asigna 2 a w1
    cpseq W0,W1     ;compara entradaE con 2
    bra cuarc     ;si no es 2, ir a programa que compara con 3
    bra mensaje2 ; si es 2, configurar mensaje 2
    
    cuarc:
    mov #0x0003,W1     ;asigna 3 a w1
    cpseq W0,W1     ;compara entradaE con 3
    bra quinc     ;si no es 3, ir a programa que compara con 4
    bra mensaje3 ; si es 3, configurar mensaje 3
    
    quinc:
    mov #0x0004,W1     ;asigna 4 a w1
    cpseq W0,W1     ;compara entradaE con 4
    bra sexc     ;si no es 4, ir a programa que compara con 5
    bra mensaje4 ; si es 4, configurar mensaje 4
    
    sexc:
    mov #0x0005,W1     ;asigna 5 a w1
    cpseq W0,W1     ;compara entradaE con 5
    bra sietc     ;si no es 5, ir a programa que compara con 6
    bra mensaje5 ; si es 5, configurar mensaje 5
    
    sietc:
    mov #0x0006,W1     ;asigna 6 a w1
    cpseq W0,W1     ;compara entradaE con 6
    bra mensaje7     ;si no es 6, configurar mensaje 7
    bra mensaje6 ; si es 6, configurar mensaje 6
    
    mensaje0:
    mov #0,w0
    mov #16,w0
    mov w0,DATO3
    mov #17,w0
    mov w0,DATO2
    mov #18,w0
    mov w0,DATO1
    mov #10,w0
    mov w0,DATO0
    bra habilitador
    
    mensaje1:
    mov #0,w0
    mov #14,w0
    mov w0,DATO3
    mov #19,w0
    mov w0,DATO2
    mov #17,w0
    mov w0,DATO1
    mov #19,w0
    mov w0,DATO0
    bra habilitador
    
    mensaje2:
    mov #0,w0
    mov #15,w0
    mov w0,DATO3
    mov #15,w0
    mov w0,DATO2
    mov #15,w0
    mov w0,DATO1
    mov #15,w0
    mov w0,DATO0
    bra habilitador
    
    mensaje3:
    mov #0,w0
    mov #0,w0
    mov w0,DATO3
    mov #1,w0
    mov w0,DATO2
    mov #2,w0
    mov w0,DATO1
    mov #3,w0
    mov w0,DATO0
    bra habilitador
    
    mensaje4:
    mov #0,w0
    mov #4,w0
    mov w0,DATO3
    mov #5,w0
    mov w0,DATO2
    mov #6,w0
    mov w0,DATO1
    mov #7,w0
    mov w0,DATO0
    bra habilitador
    
    mensaje5:
    mov #0,w0
    mov #8,w0
    mov w0,DATO3
    mov #9,w0
    mov w0,DATO2
    mov #10,w0
    mov w0,DATO1
    mov #11,w0
    mov w0,DATO0
    bra habilitador
    
    mensaje6:
    mov #0,w0
    mov #12,w0
    mov w0,DATO3
    mov #13,w0
    mov w0,DATO2
    mov #14,w0
    mov w0,DATO1
    mov #15,w0
    mov w0,DATO0
    bra habilitador
    
    mensaje7:
    mov #0,w0
    mov #12,w0
    mov w0,DATO3
    mov #16,w0
    mov w0,DATO2
    mov #10,w0
    mov w0,DATO1
    mov #17,w0
    mov w0,DATO0
    bra habilitador
    
    
    habilitador:
    mov #0x000F,W0
    bset LATD,#0   ;poner el bit 0 de puertoD en 1
    bset LATD,#1   ;poner el bit 1de puertoD en 1
    bset LATD,#2   ;poner el bit 2 de puertoD en 1
    bset LATD,#3   ;poner el bit 3 de puertoD en 1
 
    ;mostrardisplay0 
    most0:
    mov DATO0,W0
    mov #0x0000,W1
    cpseq W0,W1
    bra datocerouno     ;verificar codigo 1 del dato0
    rcall deco0         ;convierte de codigo a segmento el 0
    mov W4,DATO0
    bra vdin0
    datocerouno:
    mov DATO0,W0
    mov #0x0001,W1
    cpseq W0,W1     ;compara con 1, si es se va a deco, si no, salta a sig. comprobacion
    bra datocerodos
    rcall deco1
    mov W4,DATO0
    bra vdin0
    datocerodos:
    mov DATO0,W0
    mov #0x0002,W1
    cpseq W0,W1
    bra datocerotres
    rcall deco2
    mov W4,DATO0
    bra vdin0
    datocerotres:
    mov DATO0,W0
    mov #0x0003,W1
    cpseq W0,W1
    bra datocerocuatro
    rcall deco3
    mov W4,DATO0
    bra vdin0
    datocerocuatro:
    mov DATO0,W0
    mov #0x0004,W1
    cpseq W0,W1
    bra datocerocinco
    rcall deco4
    mov W4,DATO0
    bra vdin0
    datocerocinco:
    mov DATO0,W0
    mov #0x0005,W1
    cpseq W0,W1
    bra datoceroseis
    rcall deco5
    mov W4,DATO0
    bra vdin0 
    datoceroseis:
    mov DATO0,W0
    mov #0x0006,W1
    cpseq W0,W1
    bra datocerosiete
    rcall deco6
    mov W4,DATO0
    bra vdin0    
    datocerosiete:
    mov DATO0,W0
    mov #0x0007,W1
    cpseq W0,W1
    bra datoceroocho
    rcall deco7
    mov W4,DATO0
    bra vdin0    
    datoceroocho:
    mov DATO0,W0
    mov #0x0008,W1
    cpseq W0,W1
    bra datoceronueve
    rcall deco8
    mov W4,DATO0
    bra vdin0   
    datoceronueve:
    mov DATO0,W0
    mov #0x0009,W1
    cpseq W0,W1
    bra datocerodiez
    rcall deco9
    mov W4,DATO0
    bra vdin0    
    datocerodiez:
    mov DATO0,W0
    mov #0x000A,W1
    cpseq W0,W1
    bra datoceroonce
    rcall deco10
    mov W4,DATO0
    bra vdin0    
    datoceroonce:
    mov DATO0,W0
    mov #0x000B,W1
    cpseq W0,W1
    bra datocerodoce
    rcall deco11
    mov W4,DATO0
    bra vdin0    
    datocerodoce:
    mov DATO0,W0
    mov #0x000C,W1
    cpseq W0,W1
    bra datocerotrece
    rcall deco12
    mov W4,DATO0
    bra vdin0    
    datocerotrece:
    mov DATO0,W0
    mov #0x000D,W1
    cpseq W0,W1
    bra datocerocatorce
    rcall deco13
    mov W4,DATO0
    bra vdin0    
    datocerocatorce:
    mov DATO0,W0
    mov #0x000E,W1
    cpseq W0,W1
    bra datoceroquince
    rcall deco14
    mov W4,DATO0
    bra vdin0    
    datoceroquince:
    mov DATO0,W0
    mov #0x000F,W1
    cpseq W0,W1
    bra datocerodieciseis
    rcall deco15
    mov W4,DATO0
    bra vdin0    
    datocerodieciseis:
    mov DATO0,W0
    mov #0x0010,W1
    cpseq W0,W1
    bra datocerodiecisiete
    rcall deco16
    mov W4,DATO0
    bra vdin0    
    datocerodiecisiete:
    mov DATO0,W0
    mov #0x0011,W1
    cpseq W0,W1
    bra datocerodieciocho
    rcall deco17
    mov W4,DATO0
    bra vdin0    
    datocerodieciocho:
    mov DATO0,W0
    mov #0x0012,W1
    cpseq W0,W1
    bra datocerodiecinueve
    rcall deco18
    mov W4,DATO0
    bra vdin0    
    datocerodiecinueve:
    mov DATO0,W0
    mov #0x0013,W1
    cpseq W0,W1
    bra vdin0
    rcall deco19
    mov W4,DATO0
    bra vdin0

    vdin0:
    bclr LATD,#0
    rcall delay_2.5ms
    bset LATD,#0
    bra most1

  ;mostrardisplay0  


  ;mostrardisplay1
    most1:
    mov DATO1,W0
    mov #0x0000,W1
    cpseq W0,W1
    bra datounouno     ;verificar codigo 1 del dato1
    rcall deco0         ;convierte de codigo a segmento el 0
    mov W4,DATO1
    bra vdin1
    datounouno:
    mov DATO1,W0
    mov #0x0001,W1
    cpseq W0,W1     ;compara con 1, si es se va a deco, si no, salta a sig. comprobacion
    bra datounodos
    rcall deco1
    mov W4,DATO1
    bra vdin1
    datounodos:
    mov DATO1,W0
    mov #0x0002,W1
    cpseq W0,W1
    bra datounotres
    rcall deco2
    mov W4,DATO1
    bra vdin1
    datounotres:
    mov DATO1,W0
    mov #0x0003,W1
    cpseq W0,W1
    bra datounocuatro
    rcall deco3
    mov W4,DATO1
    bra vdin1
    datounocuatro:
    mov DATO1,W0
    mov #0x0004,W1
    cpseq W0,W1
    bra datounocinco
    rcall deco4
    mov W4,DATO1
    bra vdin1
    datounocinco:
    mov DATO1,W0
    mov #0x0005,W1
    cpseq W0,W1
    bra datounoseis
    rcall deco5
    mov W4,DATO1
    bra vdin1 
    datounoseis:
    mov DATO1,W0
    mov #0x0006,W1
    cpseq W0,W1
    bra datounosiete
    rcall deco6
    mov W4,DATO1
    bra vdin1    
    datounosiete:
    mov DATO1,W0
    mov #0x0007,W1
    cpseq W0,W1
    bra datounoocho
    rcall deco7
    mov W4,DATO1
    bra vdin1    
    datounoocho:
    mov DATO1,W0
    mov #0x0008,W1
    cpseq W0,W1
    bra datounonueve
    rcall deco8
    mov W4,DATO1
    bra vdin1   
    datounonueve:
    mov DATO1,W0
    mov #0x0009,W1
    cpseq W0,W1
    bra datounodiez
    rcall deco9
    mov W4,DATO1
    bra vdin1    
    datounodiez:
    mov DATO1,W0
    mov #0x000A,W1
    cpseq W0,W1
    bra datounoonce
    rcall deco10
    mov W4,DATO1
    bra vdin1    
    datounoonce:
    mov DATO1,W0
    mov #0x000B,W1
    cpseq W0,W1
    bra datounodoce
    rcall deco11
    mov W4,DATO1
    bra vdin1    
    datounodoce:
    mov DATO1,W0
    mov #0x000C,W1
    cpseq W0,W1
    bra datounotrece
    rcall deco12
    mov W4,DATO1
    bra vdin1    
    datounotrece:
    mov DATO1,W0
    mov #0x000D,W1
    cpseq W0,W1
    bra datounocatorce
    rcall deco13
    mov W4,DATO1
    bra vdin1    
    datounocatorce:
    mov DATO1,W0
    mov #0x000E,W1
    cpseq W0,W1
    bra datounoquince
    rcall deco14
    mov W4,DATO1
    bra vdin1    
    datounoquince:
    mov DATO1,W0
    mov #0x000F,W1
    cpseq W0,W1
    bra datounodieciseis
    rcall deco15
    mov W4,DATO1
    bra vdin1    
    datounodieciseis:
    mov DATO1,W0
    mov #0x0010,W1
    cpseq W0,W1
    bra datounodiecisiete
    rcall deco16
    mov W4,DATO1
    bra vdin1    
    datounodiecisiete:
    mov DATO1,W0
    mov #0x0011,W1
    cpseq W0,W1
    bra datounodieciocho
    rcall deco17
    mov W4,DATO1
    bra vdin1    
    datounodieciocho:
    mov DATO1,W0
    mov #0x0012,W1
    cpseq W0,W1
    bra datounodiecinueve
    rcall deco18
    mov W4,DATO1
    bra vdin1    
    datounodiecinueve:
    mov DATO1,W0
    mov #0x0013,W1
    cpseq W0,W1
    bra vdin1
    rcall deco19
    mov W4,DATO1
    bra vdin1

    vdin1:
    bclr LATD,#1
    rcall delay_2.5ms
    bset LATD,#1
    bra most2

 ;mostrardisplay1
  
 ;mostrardisplay2
    most2:
    mov DATO2,W0
    mov #0x0000,W1
    cpseq W0,W1
    bra datodosuno     ;verificar codigo 1 del dato2
    rcall deco0         ;convierte de codigo a segmento el 0
    mov W4,DATO2
    bra vdin2
    datodosuno:
    mov DATO2,W0
    mov #0x0001,W1
    cpseq W0,W1     ; compara con 1, si es se va a deco, si no, salta a sig. comprobacion
    bra datodosdos
    rcall deco1
    mov W4,DATO2
    bra vdin2
    datodosdos:
    mov DATO2,W0
    mov #0x0002,W1
    cpseq W0,W1
    bra datodostres
    rcall deco2
    mov W4,DATO2
    bra vdin2
    datodostres:
    mov DATO2,W0
    mov #0x0003,W1
    cpseq W0,W1
    bra datodoscuatro
    rcall deco3
    mov W4,DATO2
    bra vdin2
    datodoscuatro:
    mov DATO2,W0
    mov #0x0004,W1
    cpseq W0,W1
    bra datodoscinco
    rcall deco4
    mov W4,DATO2
    bra vdin2
    datodoscinco:
    mov DATO2,W0
    mov #0x0005,W1
    cpseq W0,W1
    bra datodosseis
    rcall deco5
    mov W4,DATO2
    bra vdin2 
    datodosseis:
    mov DATO2,W0
    mov #0x0006,W1
    cpseq W0,W1
    bra datodossiete
    rcall deco6
    mov W4,DATO2
    bra vdin2    
    datodossiete:
    mov DATO2,W0
    mov #0x0007,W1
    cpseq W0,W1
    bra datodosocho
    rcall deco7
    mov W4,DATO2
    bra vdin2    
    datodosocho:
    mov DATO2,W0
    mov #0x0008,W1
    cpseq W0,W1
    bra datodosnueve
    rcall deco8
    mov W4,DATO2
    bra vdin2   
    datodosnueve:
    mov DATO2,W0
    mov #0x0009,W1
    cpseq W0,W1
    bra datodosdiez
    rcall deco9
    mov W4,DATO2
    bra vdin2    
    datodosdiez:
    mov DATO2,W0
    mov #0x000A,W1
    cpseq W0,W1
    bra datodosonce
    rcall deco10
    mov W4,DATO2
    bra vdin2    
    datodosonce:
    mov DATO2,W0
    mov #0x000B,W1
    cpseq W0,W1
    bra datodosdoce
    rcall deco11
    mov W4,DATO2
    bra vdin2    
    datodosdoce:
    mov DATO2,W0
    mov #0x000C,W1
    cpseq W0,W1
    bra datodostrece
    rcall deco12
    mov W4,DATO2
    bra vdin2    
    datodostrece:
    mov DATO2,W0
    mov #0x000D,W1
    cpseq W0,W1
    bra datodoscatorce
    rcall deco13
    mov W4,DATO2
    bra vdin2    
    datodoscatorce:
    mov DATO2,W0
    mov #0x000E,W1
    cpseq W0,W1
    bra datodosquince
    rcall deco14
    mov W4,DATO2
    bra vdin2    
    datodosquince:
    mov DATO2,W0
    mov #0x000F,W1
    cpseq W0,W1
    bra datodosdieciseis
    rcall deco15
    mov W4,DATO2
    bra vdin2    
    datodosdieciseis:
    mov DATO2,W0
    mov #0x0010,W1
    cpseq W0,W1
    bra datodosdiecisiete
    rcall deco16
    mov W4,DATO2
    bra vdin2    
    datodosdiecisiete:
    mov DATO2,W0
    mov #0x0011,W1
    cpseq W0,W1
    bra datodosdieciocho
    rcall deco17
    mov W4,DATO2
    bra vdin2    
    datodosdieciocho:
    mov DATO2,W0
    mov #0x0012,W1
    cpseq W0,W1
    bra datodosdiecinueve
    rcall deco18
    mov W4,DATO2
    bra vdin2    
    datodosdiecinueve:
    mov DATO2,W0
    mov #0x0013,W1
    cpseq W0,W1
    bra vdin2
    rcall deco19
    mov W4,DATO2
    bra vdin2
    
    
    vdin2:
    bclr LATD,#2
    rcall delay_2.5ms
    bset LATD,#2
    bra most3    
  ;mostrardisplay2
  
  
  
  ;mostrardisplay3
    most3:
    mov DATO3,W0
    mov #0x0000,W1
    cpseq W0,W1
    bra datotresuno     ;verificar codigo 1 del dato0
    rcall deco0         ;convierte de codigo a segmento el 0
    mov W4,DATO3
    bra vdin3
    datotresuno:
    mov DATO3,W0
    mov #0x0001,W1
    cpseq W0,W1      ;compara con 1, si es se va a deco, si no, salta a sig. comprobacion
    bra datotresdos
    rcall deco1
    mov W4,DATO3
    bra vdin3
    datotresdos:
    mov DATO3,W0
    mov #0x0002,W1
    cpseq W0,W1
    bra datotrestres
    rcall deco2
    mov W4,DATO3
    bra vdin3
    datotrestres:
    mov DATO3,W0
    mov #0x0003,W1
    cpseq W0,W1
    bra datotrescuatro
    rcall deco3
    mov W4,DATO3
    bra vdin3
    datotrescuatro:
    mov DATO3,W0
    mov #0x0004,W1
    cpseq W0,W1
    bra datotrescinco
    rcall deco4
    mov W4,DATO3
    bra vdin3
    datotrescinco:
    mov DATO3,W0
    mov #0x0005,W1
    cpseq W0,W1
    bra datotresseis
    rcall deco5
    mov W4,DATO3
    bra vdin3 
    datotresseis:
    mov DATO3,W0
    mov #0x0006,W1
    cpseq W0,W1
    bra datotressiete
    rcall deco6
    mov W4,DATO3
    bra vdin3    
    datotressiete:
    mov DATO3,W0
    mov #0x0007,W1
    cpseq W0,W1
    bra datotresocho
    rcall deco7
    mov W4,DATO3
    bra vdin3    
    datotresocho:
    mov DATO3,W0
    mov #0x0008,W1
    cpseq W0,W1
    bra datotresnueve
    rcall deco8
    mov W4,DATO3
    bra vdin3   
    datotresnueve:
    mov DATO3,W0
    mov #0x0009,W1
    cpseq W0,W1
    bra datotresdiez
    rcall deco9
    mov W4,DATO3
    bra vdin3    
    datotresdiez:
    mov DATO3,W0
    mov #0x000A,W1
    cpseq W0,W1
    bra datotresonce
    rcall deco10
    mov W4,DATO3
    bra vdin3    
    datotresonce:
    mov DATO3,W0
    mov #0x000B,W1
    cpseq W0,W1
    bra datotresdoce
    rcall deco11
    mov W4,DATO3
    bra vdin3    
    datotresdoce:
    mov DATO3,W0
    mov #0x000C,W1
    cpseq W0,W1
    bra datotrestrece
    rcall deco12
    mov W4,DATO3
    bra vdin3    
    datotrestrece:
    mov DATO3,W0
    mov #0x000D,W1
    cpseq W0,W1
    bra datotrescatorce
    rcall deco13
    mov W4,DATO3
    bra vdin3    
    datotrescatorce:
    mov DATO3,W0
    mov #0x000E,W1
    cpseq W0,W1
    bra datotresquince
    rcall deco14
    mov W4,DATO3
    bra vdin3    
    datotresquince:
    mov DATO3,W0
    mov #0x000F,W1
    cpseq W0,W1
    bra datotresdieciseis
    rcall deco15
    mov W4,DATO3
    bra vdin3    
    datotresdieciseis:
    mov DATO3,W0
    mov #0x0010,W1
    cpseq W0,W1
    bra datotresdiecisiete
    rcall deco16
    mov W4,DATO3
    bra vdin3    
    datotresdiecisiete:
    mov DATO3,W0
    mov #0x0011,W1
    cpseq W0,W1
    bra datotresdieciocho
    rcall deco17
    mov W4,DATO3
    bra vdin3    
    datotresdieciocho:
    mov DATO3,W0
    mov #0x0012,W1
    cpseq W0,W1
    bra datotresdiecinueve
    rcall deco18
    mov W4,DATO3
    bra vdin3    
    datotresdiecinueve:
    mov DATO3,W0
    mov #0x0013,W1
    cpseq W0,W1
    bra vdin3
    rcall deco19
    mov W4,DATO3
    bra vdin3   
    vdin3:
    bclr LATD,#3
    rcall delay_2.5ms
    bset LATD,#3
    bra most0

; ;mostrardisplay3
     
    deco0:
    mov #0X007E,W4
    return
    deco1:
    mov #0X0030,W4
    return
    deco2:
    mov #0X006D,W4
    return    
    deco3:
    mov #0X0079,W4
    return    
    deco4:
    mov #0X0033,W4
    return    
    deco5:
    mov #0X005B,W4
    return   
    deco6:
    mov #0X005F,W4
    return   
    deco7:
    mov #0X0070,W4
    return    
    deco8:
    mov #0X007F,W4
    return
    deco9:
    mov #0X007B,W4
    return    
    deco10:
    mov #0X0077,W4
    return    
    deco11:
    mov #0X001F,W4
    return    
    deco12:
    mov #0X004E,W4
    return    
    deco13:
    mov #0X003D,W4
    return    
    deco14:
    mov #0X004F,W4
    return    
    deco15:
    mov #0X0047,W4
    return    
    deco16:
    mov #0X0037,W4
    return    
    deco17:
    mov #0X001D,W4
    return    
    deco18:
    mov #0X000E,W4
    return 
    deco19:
    mov #0x0005,W4
    return
    
    
    
    
    
    
; mostrar0:
;    mov     CONT,W0
;    rcall   Dec7Seg ;Llamado a subrutina entrega "0000" y decodifica "11111110"
;    mov     W0,LATB ;Guarda el valor decodificado a W0 y escribe esto en el puerto B
;    
;    rcall   Delay1s
;    
;    btsc    PORTA,#1 ;Bit Test Skip Clear ;Revisa el bit 1 del puerto A si es 0 no reset si es 1 si reset
;    bra     inicio
;    
;    btsc    PORTA,#0 
;    bra     mostrar
;    
;    cp0     CONT
;;0-0 Z=1 salta a inicio si no, 0-1,2,3,4,5,6,7,8,9,A,B;C;D;E Z=0 no salta y va a mostrar (CP0=CONT-0x00)
;    bra     Z,inicio
;;Si Z=1 es por que CONT = 0
;    dec     CONT
;;Decrementa
;    bra     mostrar
; Dec7Seg:
;    return
;    
; Delay1s:
;    return
     
   
    
    
    
    
    
    
    
    
    
;*******************************************************************************
;Subroutines
;*******************************************************************************
 
;loop:
;    CALL delay_100ms        ;delay 100ms
;    BTG LED_PORT, #LED      ;complement bit RA0
;    BRA loop
; 
;config_IO:
;    MOV #0xFFFF,W0
;    MOV W0, AD1PCFGL        ;configure pins A/D = 0 as analog (default) / 1 as digital
;    MOV #0xFFFE,W0
;    MOV W0,TRISA        ;configure digitl pins I/O = 0 as output / 1 as digital (default)
;    MOV #0xFFFF,W0
;    MOV W0,TRISB        ;configure digitl pins I/O = 0 as output / 1 as digital (default)
;    RETURN
; 
;delay_100ms:
;    MOV #K2,W14         ;number of delay intructions alpha2 = K2*(alpha1 + 5) + 3
;wait_100ms:
;    CALL delay_100us
;    DEC W14,W14
;    BRA NZ, wait_100ms      ;if W14=0 execution goes to return otherwise goes to wait_100ms
;    RETURN
; 
delay_2.5ms:            ;number of delay intructions alpha1 = 3*K1 + 3
    mov #K1,W13
wait_2.5ms:
    dec W13,W13
    bra NZ, wait_2.5ms      ;if W13=0 execution goes to return otherwise goes to wait_100us
    return     
 
p_cont_oct:
    mov PORTE,W7
    btss W7,#1
    bra p_cont_dec
    rcall inc_dec_oct
    bra ritur
p_cont_dec:
    mov PORTE,W8
    btss W8,#2
    bra p_cont_hex
    rcall inc_dec_dec
    bra ritur
p_cont_hex:
    mov PORTE,W9
    btss W9,#3
    bra ritur
    rcall inc_dec_hex
    bra ritur

inc_dec_bin:
    mov PORTE,W10
    add #0b1,W10
    mov W10,LATE
    return
inc_dec_oct:
    mov PORTE,W11
    add #01,W11
    mov W11,LATE
    return 
inc_dec_dec:
    mov PORTE,W12
    add #1,W12
    mov W12,LATE
    return
 inc_dec_hex:
    mov PORTE,W13
    add #0x0001,W13
    mov W13,LATE
    return
__T1Interrupt:
    ;call    syn_proc
    mov PORTE,W6
    btss W6,#0
    bra p_cont_oct
    rcall inc_dec_bin
    ritur:
    bclr    IFS0, #T1IF        ;Clear the Timer1 Interrupt flag Status bit.
    retfie                     ;Return from Interrupt Service routine
         
__DefaultInterrupt:             ; services all other interrupts & traps
    nop
    inc     FaultCount          ; increment the fault counter
    reset                       ; and reset the device     
    
;***************************End of All Code Sections****************************
.end                               ;End of program code in this file
                   ;Extra blank new line for compiler

