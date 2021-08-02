;*******************************************************************************
;                                                                              *
;    Filename:      Primer_Lab.s                                              *
;    Date:          06/02/2019                                             *
;    File Version:  v1.0                                                     *
;    Authors:       Carlos Felipe Alonso Caro                              *
;		    Christian Danilo Reyes Parra
;		    Viviana Lemus Ruiz
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
 .bss
	CONT:	.space 2; Variable cont sin inicializar con 2 bytes de disponibilidad
	
.equ    K1, 121         ;pre-compiler constants
.equ    K2, 993
.equ    LED, RA0        ;generic use of port rapid prototyping
.equ    LED_PORT, LATA      ;generic use of port rapid prototyping
;*******************************************************************************
;Code Section in Program Memory
;*******************************************************************************
 .bss
 
    DISP0:	.space 2
    DISP1:	.space 2
    DISP2:	.space 2
    DISP3:	.space 2
	
.text               ;Start of Code section
_main:
    ;conf
    mov	    #0xFF80,W0  ;configurar port B <6:0> salida
    mov	    W0,TRISB 
    mov	    #0xFFFF,WO  ;configurar port E <2:0> entrada 
    mov	    W0,TRISE
    mov	    #0xFFF0,W0   ;configura PORT D <3:0> salida
    mov	    W0,TRISD   
    ;conf
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
    mov w0,DISP3
    mov #17,w0
    mov w0,DISP2
    mov #18,w0
    mov w0,DISP1
    mov #10,w0
    mov w0,DISP0
    bra habilitador
    
    mensaje1:
    mov #0,w0
    mov #14,w0
    mov w0,DISP3
    mov #19,w0
    mov w0,DISP2
    mov #17,w0
    mov w0,DISP1
    mov #19,w0
    mov w0,DISP0
    bra habilitador
    
    mensaje2:
    mov #0,w0
    mov #15,w0
    mov w0,DISP3
    mov #15,w0
    mov w0,DISP2
    mov #15,w0
    mov w0,DISP1
    mov #15,w0
    mov w0,DISP0
    bra habilitador
    
    mensaje3:
    mov #0,w0
    mov #0,w0
    mov w0,DISP3
    mov #1,w0
    mov w0,DISP2
    mov #2,w0
    mov w0,DISP1
    mov #3,w0
    mov w0,DISP0
    bra habilitador
    
    mensaje4:
    mov #0,w0
    mov #4,w0
    mov w0,DISP3
    mov #5,w0
    mov w0,DISP2
    mov #6,w0
    mov w0,DISP1
    mov #7,w0
    mov w0,DISP0
    bra habilitador
    
    mensaje5:
    mov #0,w0
    mov #8,w0
    mov w0,DISP3
    mov #9,w0
    mov w0,DISP2
    mov #10,w0
    mov w0,DISP1
    mov #11,w0
    mov w0,DISP0
    bra habilitador
    
    mensaje6:
    mov #0,w0
    mov #12,w0
    mov w0,DISP3
    mov #13,w0
    mov w0,DISP2
    mov #14,w0
    mov w0,DISP1
    mov #15,w0
    mov w0,DISP0
    bra habilitador
    
    mensaje7:
    mov #0,w0
    mov #12,w0
    mov w0,DISP3
    mov #16,w0
    mov w0,DISP2
    mov #10,w0
    mov w0,DISP1
    mov #17,w0
    mov w0,DISP0
    bra habilitador
    
    
 habilitador:
    mov #0x000F,WREG
    bset LATD,#0   ;poner el bit 0 de puertoD en 1
    bset LATD,#1   ;poner el bit 1de puertoD en 1
    bset LATD,#2   ;poner el bit 2 de puertoD en 1
    bset LATD,#3   ;poner el bit 3 de puertoD en 1
 most0:
    
    
    
    
    
    

 mostrar:
    mov	    CONT,W0
    rcall   Dec7Seg ;Llamado a subrutina entrega "0000" y decodifica "11111110"
    mov	    W0,LATB ;Guarda el valor decodificado a W0 y escribe esto en el puerto B
    
    rcall   Delay1s
    
    btsc    PORTA,#1 ;Bit Test Skip Clear ;Revisa el bit 1 del puerto A si es 0 no reset si es 1 si reset
    bra	    inicio
    
    btsc    PORTA,#0 
    bra	    mostrar
    
    cp0	    CONT	;0-0 Z=1 salta a inicio si no, 0-1,2,3,4,5,6,7,8,9,A,B;C;D;E Z=0 no salta y va a mostrar (CP0=CONT-0x00)
    bra	    Z,inicio	;Si Z=1 es por que CONT = 0
    dec	    CONT	;Decrementa
    bra	    mostrar
 Dec7Seg:
    return
    
 Delay1s:
    return
    
   
;*******************************************************************************
;Subroutines
;*******************************************************************************
 
loop:
    CALL delay_100ms        ;delay 100ms
    BTG LED_PORT, #LED      ;complement bit RA0
    BRA loop
 
config_IO:
    MOV #0xFFFF,W0
    MOV W0, AD1PCFGL        ;configure pins A/D = 0 as analog (default) / 1 as digital
    MOV #0xFFFE,W0
    MOV W0,TRISA        ;configure digitl pins I/O = 0 as output / 1 as digital (default)
    MOV #0xFFFF,W0
    MOV W0,TRISB        ;configure digitl pins I/O = 0 as output / 1 as digital (default)
    RETURN
 
delay_100ms:
    MOV #K2,W14         ;number of delay intructions alpha2 = K2*(alpha1 + 5) + 3
wait_100ms:
    CALL delay_100us
    DEC W14,W14
    BRA NZ, wait_100ms      ;if W14=0 execution goes to return otherwise goes to wait_100ms
    RETURN
 
delay_100us:            ;number of delay intructions alpha1 = 3*K1 + 3
    MOV #K1,W13
wait_100us:
    DEC W13,W13
    BRA NZ, wait_100us      ;if W13=0 execution goes to return otherwise goes to wait_100us
    RETURN     
 
;***************************End of All Code Sections****************************
.end                               ;End of program code in this file
                   ;Extra blank new line for compiler



