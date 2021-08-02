
    
    
 
    
    
    
    
    
    
    
    
    
    
    
    
    
    
_main:
    ;conf
    mov     #0xFF00,W0  ;configurar PORT B <7:0> salida
    mov     W0,TRISB 
    mov     #0xFFFF,W0  ;configurar PORT E entrada
    mov     W0,TRISE
    mov     #0xFFFF,W0   ;configura PORT F entrada
    mov     W0,TRISF
    mov     #0xFFFC,W0
    mov     W0,TRISD
    
    ;conf
    
    
    
    
    
    
    
    
    mov PORTF,W6
    mov LATD,#FFFF
visdin:
cpcon0:
    mov	    #0,W8
    cpseq   W6,W8
    bra	    cpcon1
    rcall   deco0
    bra     disp0
    
cpcon1:
    mov	    #1,W8
    cpseq   W6,W8
    bra	    cpcon2
    rcall   deco1
    mov W15,LATB
    bra disp0
    ;;el resto
    
deco0:
    mov.b #0X7E,W15
    return
    
disp0:
    mov LATD,#FFFE
    rcall delay2.5_ms
    mov LATD,#FFFF
    ;,resto de decos
disp    
    sum_res_w6:
    mov PORTE,W10
    btss W10,#0
    bra res
    add w6
    bra inc_dec
    rest:
    dec w6
    bra inc_dec
    
;  inc-dec
inc_dec:
    mov PORTE,W0
    btss W0,#0
    bra decr
    add #1,W6
    bra pausa
decr:
    dec #1,W6
    bra pausa
    
    
pausa:
    mov PORTE,W0
    btss W0,#1
    bra nuev
    bra ciclo_pausa
 
ciclo_pausa:
    mov PORTB,W1
    bset W1,#7
    mov W1,LATB
    mov PORTE,W0
    btss w0,#1
    bra ciclo_pausa
    bra nuevo
    
nuev:
    mov PORTE,W3
    btss W3,#2
    bra rein
    bra nuev_dat
nuev_dat:
    mov PORTF,W6
    bra rein
    
rein:
    mov PORTE,W4
    btss W4,#3
    bra visdin
    bra sub_rein

sub_rein:
    mov PORTE,W0
    btss W0,#0
    mov.b #0x00,W6
    mov.b #0xFF,W6
    bra visdin


;inc-dec


