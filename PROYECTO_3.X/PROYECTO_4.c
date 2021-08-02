/*
 * Archivo:   PROYECTO_3.c
 * Autores:   Carlos Felipe Alonso Caro
 *            Viviana Lemus Ruiz
 *            Christian Danilo Reyes Parra
 *
 * Creado el 29 de marzo del 2019,      01:11 PM
 * Terminado el 5 de abril del 2019,    02:35 AM
 */
#define  FOSC         (7370000L)
#define  FCY          (FOSC/2)

#define  BAUDRATE     9600
#define  BREG         ((FCY)/BAUDRATE/16)-1

unsigned char DATO;
unsigned char contador_int = 0, contador2_int = 0;
unsigned char seg_int, segd_int, min_int, mind_int, hor_int, hord_int; //variables de interrupcion
unsigned char dia_int, diad_int, mes_int, mesd_int, yr_int, yrd_int; //variabes de interrupcion fecha
unsigned char dato_int;
unsigned char data = 0;
unsigned char VEL;
float factor;

// FOSC
//#pragma config FOSFPR = FRC             // Oscillator (Internal Fast RC (No change to Primary Osc Mode bits))
#pragma config FCKSMEN = CSW_FSCM_OFF   // Clock Switching and Monitor (Sw Disabled, Mon Disabled)

// FWDT
#pragma config FWPSB = WDTPSB_16        // WDT Prescaler B (1:16)
#pragma config FWPSA = WDTPSA_512       // WDT Prescaler A (1:512)
#pragma config WDT = WDT_OFF            // Watchdog Timer (Disabled)

// FBORPOR
#pragma config FPWRT = PWRT_64          // POR Timer Value (64ms)
#pragma config BODENV = BORV20          // Brown Out Voltage (Reserved)
#pragma config BOREN = PBOR_ON          // PBOR Enable (Enabled)
#pragma config MCLRE = MCLR_EN          // Master Clear Enable (Enabled)

// FGS
#pragma config GWRP = GWRP_OFF          // General Code Segment Write Protect (Disabled)
#pragma config GCP = CODE_PROT_OFF      // General Segment Code Protection (Disabled)

// FICD
#pragma config ICS = ICS_PGD            // Comm Channel Select (Use PGC/EMUC and PGD/EMUD)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.


#include    <xc.h>
#include    <stdio.h>
#include    <string.h>
#include    <libpic30.h>

#define     memoria         0b01010111
#define     reloj           0b01101000
#define     sensor_cruce    PORTEbits.RE4
#define     sensor_s1       PORTBbits.RB4
#define     sensor_s0       PORTBbits.RB5
#define     puls_pwm        PORTBbits.RB6
#define     pololu_izq      PORTBbits.RB2
#define     pololu_der      PORTBbits.RB1
#define     pololu_adel      PORTBbits.RB0

void I2CCONFIG(void); //configuracion inicial de i2c
void TMR1_CONFIG(void); //configuracion inicial de interrupcion de timer
void Config_RTC(void); //configuracion inicial del reloj
void UARTTXCHAR(unsigned char data); //Declaración de función para transmitir por la EUSART
char UARTRXCHAR(void); //Declaración de función para recibir por la EUSART
void IOCONFIG(void); //configuracion de entradas/salidas
void UCONFIG(void); //configuracion de la uart
void config_vel(unsigned char *); //configuracion velocidad     
void config_factor(unsigned char *); //configuracion factor de mult
void config_ruta(unsigned char [299], unsigned char *); //guardar ruta
void most_dat(unsigned char [299], unsigned char *); //mostrar datos
void config_PWM(unsigned char, float); //configurar pwm
void devuelvase(void);
void GenerateStartCondition(void);
void GenerateRestartCondition(void);
void GenerateStopCondition(void);
unsigned char TxByte(unsigned char); //habilitar transimitir BYTE
unsigned char RxByte(void); //habilitar recibir BYTE
void GenerateNACK(void);
void GenerateACK(void);
void lcd_init(void); //inicializar lc
void lcd_especial(unsigned char);
void lcd_esc(unsigned char data); //escribir el dato en lcd
void lcd_clear(void); //borrar pantalla de lcd
void lcd_set_display(unsigned char); //poner cursor en direccion deseada de lcd
void Write_Data(unsigned char, unsigned int, unsigned char);
void Write_Data_Seq(unsigned char slave, unsigned int add, unsigned char datos[300], unsigned char num_dat, unsigned int comienza);
unsigned char Read_Data(unsigned char, unsigned int);
void Read_Data_Seq(unsigned char slave, unsigned int add, unsigned char datos[300], unsigned char num_dat);
unsigned char Write_RTC(unsigned char add, unsigned char datos);
unsigned char Read_RTC(unsigned char);
void leer_hora(void);
void seg_ruta(unsigned int comienzo);
void gir_der(void);
void gir_izq(void);
void gir_atras(void);
void seg_algoritmo(void);
void config_VEL(void);
void config_fac_mult(void);

int main(void) {
    unsigned char trac = 0, contoo = 0, error = 0, VEL0 = 0, VEL1 = 25, VEL2 = 50, VEL3 = 75, contuo = 0, DATOSS[64];
    unsigned char direccion[299];

    contoo = 0;
    IOCONFIG();
    UCONFIG();
    I2CCONFIG();
    TMR1_CONFIG();
    lcd_init();
    __delay_ms(10);
    lcd_set_display(0x0040);
    strncpy(DATOSS, "CONFIGURACION", 64);
    while (DATOSS[contoo] != 0) {
        lcd_esc(DATOSS[contoo]);
        __delay_ms(10);
        contoo++;
    }
    contoo = 0;
    //Config_RTC(); //configurar fecha actual
chup:
    error = 0;
    printf("\n\n\nBienvenido a Pits. Presione:\n\t"
            "0.Para seguir ruta o resolver por algoritmo. \n\t"
            "1.Para configurar velocidades. \n\t"
            "2.Para configurar el factor de multiplicacion.\n\t"
            "3.Para configurar la ruta a seguir. \n\t"
            "4.Para ver la configuracion actual. \n\t"
            "5.Para salir de pits\n\n\t");
    __delay_ms(50);
    while (error == 0) {
        DATO = UARTRXCHAR();
        switch (DATO) {
            case '0':
                IFS0bits.T1IF = 0; //poner bandera de interrupcion en 0
                IEC0bits.T1IE = 1; //habilita interrupciones de timer
hoo:
                printf("Presione:\n\t"
                        "A.Para seguir ruta. \n\t"
                        "B.Para resolver por laberinto\n\t");
                __delay_ms(10);
                while (U1STAbits.URXDA == 0);
                DATO = UARTRXCHAR();
                if (DATO == 'A') {
                    Write_Data(0b01010111, 0x0000, DATO);
                    trac = 0;
                    error = 1;
                    break;
                } else if (DATO == 'B') {
                    Write_Data(0b01010111, 0x0000, DATO);
                    trac = 1;
                    error = 1;
                } else if (DATO != 'A' && DATO != 'B') {
                    printf("\n\tIngrese dato correcto\n\t");
                    goto hoo;
                }
                break;
            case '1':
                config_vel(&error);
                break;
            case '2':
                config_factor(&error);
                break;
            case '3':
                config_ruta(direccion, &error);
                DATO = '4';
                break;
            case '4':
                most_dat(direccion, &error);
                break;
            case '5':
                __delay_ms(1500);
                goto pwm;
                break;
            default:
                printf("RECUERDA: pasaras MPEI\n\n");
                break;
        }
    }
    IEC0bits.T1IE = 1;
    IFS0bits.T1IF = 0;
    __delay_ms(10);
    //IEC0bits.T1IE=1;
    goto chup;
pwm: ///A PARTIR DE ACA Y HASTA QUE TERMINE EL MAIN, NO LEER    
    //    
    IEC0bits.T1IE = 0;
    IFS0bits.T1IF = 0;
    T1CONbits.TON = 0;
    lcd_set_display(0x0040);
    lcd_esc('P');
    lcd_esc('I');
    lcd_esc('S');
    lcd_esc('T');
    lcd_esc('A');
    lcd_esc(' ');
    lcd_esc(' ');
    lcd_esc(' ');
    lcd_esc(' ');
    lcd_esc(' ');
    lcd_esc(' ');
    lcd_esc(' ');
    lcd_esc(' ');
    config_VEL();
    config_fac_mult();
    while (puls_pwm == 0) leer_hora();
    config_PWM(VEL, 1);
    __delay_ms(1500);
    while (1) {
        if (Read_Data(memoria, 0x0000) == 'A') {
            seg_ruta(0x0260);
            //            seg_ruta(0x0280);
            //            seg_ruta(0x02A0);
            //            seg_ruta(0x02C0);
            //            seg_ruta(0x02E0);
            //            seg_ruta(0x0300);
            //            seg_ruta(0x0320);
            //            seg_ruta(0x0340);
            //            seg_ruta(0x0360);
            //            seg_ruta(0x0380);
            while (1) {
                PTCONbits.PTEN = 0;
                printf("\n\t\tacabe.");
            }
        } //


        if (sensor_cruce == 1) {
            if (pololu_izq==1 && pololu_adel==1 && pololu_der==1) {
                __delay_ms(200);
                gir_atras();
            } 
            else if (pololu_izq==1 && pololu_adel==1 && pololu_der==0) {
                __delay_ms(200);
                gir_der();
            } 
            else if (pololu_izq==1 && pololu_adel==0 && pololu_der==1) {
                __delay_ms(200);
                PTCONbits.PTEN = 0;
                PWMCON1bits.PEN1H = 1;
                PWMCON1bits.PEN2H = 1;
                PWMCON1bits.PEN1L = 0;
                PWMCON1bits.PEN2L = 0;
                PTCONbits.PTEN = 1;
                lcd_set_display(0x40);
                lcd_esc('D');
                lcd_esc('E');
                lcd_esc('L');
                lcd_esc('A');
                lcd_esc('N');
                lcd_esc('T');
                lcd_esc('E');
                while (sensor_cruce == 0) {
                    seg_algoritmo();
                    leer_hora();
                }
                config_PWM(VEL, 1);
                __delay_ms(400);
            } 
            else if (pololu_izq==1 && pololu_adel==0 && pololu_der==0) {
                __delay_ms(200);
                PTCONbits.PTEN = 0;
                PWMCON1bits.PEN1H = 1;
                PWMCON1bits.PEN2H = 1;
                PWMCON1bits.PEN1L = 0;
                PWMCON1bits.PEN2L = 0;
                PTCONbits.PTEN = 1;
                lcd_set_display(0x40);
                lcd_esc('D');
                lcd_esc('E');
                lcd_esc('L');
                lcd_esc('A');
                lcd_esc('N');
                lcd_esc('T');
                lcd_esc('E');
                while (sensor_cruce == 0) {
                    seg_algoritmo();
                    leer_hora();
                }
                config_PWM(VEL, 1);
                __delay_ms(400);
            } 
            else if (pololu_izq==0 && pololu_adel==1 && pololu_der==1) {
                __delay_ms(200);
                gir_izq();
            } 
            else if (pololu_izq==0 && pololu_adel==1 && pololu_der==0) {
                __delay_ms(200);
                gir_izq();
            } 
            else if (pololu_izq==0 && pololu_adel==0 && pololu_der==1) {
                __delay_ms(200);
                gir_izq();
            } 
            else if (pololu_izq==0 && pololu_adel==0 && pololu_der==0) {
                __delay_ms(200);
                gir_izq();
            }
        } 
        else {
            lcd_set_display(0x40);
            lcd_esc('A');
            lcd_esc('L');
            lcd_esc('G');
            lcd_esc(' ');
            lcd_esc(' ');
            lcd_esc(' ');
            lcd_esc(' ');
            lcd_esc(' ');
            seg_algoritmo();
            leer_hora();
        }

    } //ACA TERMINA EL WHILE

    return 0;
} //ACA TERMINA EL MAIN

void I2CCONFIG(void) {
    I2CBRG = 15; //para trabajar a 100khz
    I2CCONbits.I2CSIDL = 0; //sigue funcionando aun en idle mode
    I2CCONbits.A10M = 0; //configurar adress en 7 bits
    I2CCONbits.IPMIEN = 0; //debe estar en 0 si es maestro
    I2CCONbits.I2CEN = 1; //Habilitar i2c
    __delay_ms(10);
}

void TMR1_CONFIG(void) {
    IPC0 = 0x4000; //prioridad timer
    T1CON = 0x0000;
    T1CONbits.TCKPS = 1; //prescaler en 1
    PR1 = 0xFFFF; //hasta donde cuenta tmr1
    TMR1 = 0x0000; //inicializando en 0
    IFS0bits.T1IF = 0; //poniendo bandera en 0
    IEC0bits.T1IE = 1; //habilitar interrupcion de tmr1
    T1CONbits.TON = 1; //encender tmr1
}

void Config_RTC(void) {
    Write_RTC(0x0000, 0x00); //segundos 
    Write_RTC(0x0001, 0x11); //minutos
    Write_RTC(0x0002, 0x22); //horas
    Write_RTC(0x0003, 0x01); //dia semana
    Write_RTC(0x0004, 0x20); //dia calendario
    Write_RTC(0x0005, 0x05); //mes calendario
    Write_RTC(0x0006, 0x19); //año calendario
    Write_RTC(0x000E, 0x00); //activar el oscilador con bateria
}

void config_vel(unsigned char *error) {
    unsigned char dato, devuelv;
    printf("\n\nLa ultima velocidad modificada, sera la seleccionada");
config_v:
    devuelv = 0;
    printf("\n\n\tPulse 1,2,3 o 4 para cambiar las velocidades y enter para salir de este menu.");
    dato = UARTRXCHAR();
    if (dato != '1' && dato != '2' && dato != '3' && dato != '4' && dato != 13) {
        printf("\n\ningrese dato correcto");
        devuelv = 1;
    }
    if (devuelv == 1) goto config_v;
    if (dato == 13) {
        *error = 1;
        return;
    }

    if (dato == '1') { //cambiar velocidad 1
        printf("\n\n\t\tVelocidad 1:");
        dato = UARTRXCHAR();
        Write_Data(memoria, 0x0001, dato);
        printf("%c", dato);
        dato = UARTRXCHAR();
        Write_Data(memoria, 0x0002, dato);
        printf("%c", dato);
        dato = UARTRXCHAR();
        Write_Data(memoria, 0x0003, dato);
        printf("%c", dato);
        __delay_ms(1000);
        dato = 0;
        Write_Data(memoria, 0x000E, '1');
    }

    if (dato == '2') { //cambiar velocidad 1
        printf("\n\n\t\tVelocidad 2:");
        dato = UARTRXCHAR();
        Write_Data(memoria, 0x0004, dato);
        printf("%c", dato);
        dato = UARTRXCHAR();
        Write_Data(memoria, 0x0005, dato);
        printf("%c", dato);
        dato = UARTRXCHAR();
        Write_Data(memoria, 0x0006, dato);
        printf("%c", dato);
        __delay_ms(1000);
        dato = 0;
        Write_Data(memoria, 0x000E, '2');
    }

    if (dato == '3') { //cambiar velocidad 1
        printf("\n\n\t\tVelocidad 3:");
        dato = UARTRXCHAR();
        Write_Data(memoria, 0x0007, dato);
        printf("%c", dato);
        dato = UARTRXCHAR();
        Write_Data(memoria, 0x0008, dato);
        printf("%c", dato);
        dato = UARTRXCHAR();
        Write_Data(memoria, 0x0009, dato);
        printf("%c", dato);
        __delay_ms(1000);
        dato = 0;
        Write_Data(memoria, 0x000E, '3');
    }

    if (dato == '4') { //cambiar velocidad 1
        printf("\n\n\t\tVelocidad 4:");
        dato = UARTRXCHAR();
        Write_Data(memoria, 0x000A, dato);
        printf("%c", dato);
        dato = UARTRXCHAR();
        Write_Data(memoria, 0x000B, dato);
        printf("%c", dato);
        dato = UARTRXCHAR();
        Write_Data(memoria, 0x000C, dato);
        printf("%c", dato);
        __delay_ms(1000);
        dato = 0;
        Write_Data(memoria, 0x000E, '4');
    }
    goto config_v;
}//main

void config_ruta(unsigned char vec[299], unsigned char *error) {
    unsigned int cont = 0, conto = 0;
    unsigned char datt;
    printf("\n\n\t");
    DATO = 32;
    while (conto <= 299) {
        vec[conto] = 0;
        conto++;
    }
    conto = 0;
    while (*error == 0) {
        UARTTXCHAR(datt);
juju:
        datt = UARTRXCHAR();
        //UARTTXCHAR(DATO);
        if (datt == 'L' || datt == 'l') vec[cont] = 'L';
        else if (datt == 'R' || datt == 'r') vec[cont] = 'R';
        else if (datt == 'F' || datt == 'f') vec[cont] = 'F';
        else if (datt == 'B' || datt == 'b') vec[cont] = 'B';
        else if (datt == 'S' || datt == 's') {
            IEC0bits.T1IE = 0;
            vec[cont] = 'S';
            __delay_ms(5);
            UARTTXCHAR(datt);
            *error = 1;
            IEC0bits.T1IE = 1;
        } else if (datt == '-') vec[cont] = '-';
        else if (datt == '_') vec[cont] = '_';
        else if (datt == 13) *error = 1;
        else if (datt != 'L' && datt != 'l' && datt != 'R' && datt != 'r' && datt != 'F' && datt != 'f' && datt != 'B' && datt != 'b' && datt != 32) {
            goto juju;
            ////            *error=1;
            ////            for (conto=0;conto<=100;conto ++)
            ////            {
            ////                vec[conto]='0';
            ////            }
        }
        __delay_ms(50);
        U1STAbits.TRMT = 0;
        cont = cont + 1;
        if (cont == 300) *error = 1;


    }
    IEC0bits.T1IE = 0;
    __delay_ms(10);
    Write_Data_Seq(memoria, 0x0260, vec, 30, 0);
    __delay_ms(10);
    Write_Data_Seq(memoria, 0x0280, vec, 30, 30);
    __delay_ms(10);
    Write_Data_Seq(memoria, 0x02A0, vec, 30, 60);
    __delay_ms(10);
    Write_Data_Seq(memoria, 0x02C0, vec, 30, 90);
    __delay_ms(10);
    Write_Data_Seq(memoria, 0x02E0, vec, 30, 120);
    __delay_ms(10);
    Write_Data_Seq(memoria, 0x0300, vec, 30, 150);
    __delay_ms(10);
    Write_Data_Seq(memoria, 0x0320, vec, 30, 180);
    __delay_ms(10);
    Write_Data_Seq(memoria, 0x0340, vec, 30, 210);
    __delay_ms(10);
    Write_Data_Seq(memoria, 0x0360, vec, 30, 240);
    __delay_ms(10);
    Write_Data_Seq(memoria, 0x0380, vec, 30, 270);
    __delay_ms(100);
    IEC0bits.T1IE = 0;
    __delay_ms(10);
    IFS0bits.T1IF = 0;
    DATO = Read_Data(memoria, 0x0000);
    IEC0bits.T1IE = 1;
}

void most_dat(unsigned char vec[299], unsigned char *error) {
    int contoo = 0;
devuev:
    IEC0bits.T1IE = 0;
    printf("\n\n\tVelocidad elegida: Velocidad %c", Read_Data(memoria, 0x000E));
    printf("\n\n\tVelocidad 1: %c%c%c\n\n\tVelocidad 2: %c%c%c\n\n\tVelocidad 3: %c%c%c\n\n\tVelocidad 4: %c%c%c", Read_Data(memoria, 0x0001), Read_Data(memoria, 0x0002), Read_Data(memoria, 0x0003), Read_Data(memoria, 0x0004), Read_Data(memoria, 0x0005), Read_Data(memoria, 0x0006), Read_Data(memoria, 0x0007), Read_Data(memoria, 0x0008), Read_Data(memoria, 0x0009), Read_Data(memoria, 0x000A), Read_Data(memoria, 0x000B), Read_Data(memoria, 0x000C));
    printf("\n\n\tRuta elegida\n\n:");
    contoo = 0;
    while (Read_Data(memoria, 0x0260 + contoo) != 0 && contoo < 30) {
        printf("%c", Read_Data(memoria, 0x0260 + contoo));
        __delay_ms(10);
        contoo++;
    }
    contoo = 0;
    while (Read_Data(memoria, 0x0280 + contoo) != 0 && contoo < 30) {
        printf("%c", Read_Data(memoria, 0x0280 + contoo));
        __delay_ms(10);
        contoo++;
    }
    contoo = 0;
    while (Read_Data(memoria, 0x02A0 + contoo) != 0 && contoo < 30) {
        printf("%c", Read_Data(memoria, 0x02A0 + contoo));
        __delay_ms(10);
        contoo++;
    }
    contoo = 0;
    while (Read_Data(memoria, 0x02C0 + contoo) != 0 && contoo < 30) {
        printf("%c", Read_Data(memoria, 0x02C0 + contoo));
        __delay_ms(10);
        contoo++;
    }
    contoo = 0;
    while (Read_Data(memoria, 0x02E0 + contoo) != 0 && contoo < 30) {
        printf("%c", Read_Data(memoria, 0x02E0 + contoo));
        __delay_ms(10);
        contoo++;
    }
    contoo = 0;
    while (Read_Data(memoria, 0x0300 + contoo) != 0 && contoo < 30) {
        printf("%c", Read_Data(memoria, 0x0300 + contoo));
        __delay_ms(10);
        contoo++;
    }
    contoo = 0;
    while (Read_Data(memoria, 0x0320 + contoo) != 0 && contoo < 30) {
        printf("%c", Read_Data(memoria, 0x0320 + contoo));
        __delay_ms(10);
        contoo++;
    }
    contoo = 0;
    while (Read_Data(memoria, 0x0340 + contoo) != 0 && contoo < 30) {
        printf("%c", Read_Data(memoria, 0x0340 + contoo));
        __delay_ms(10);
        contoo++;
    }
    contoo = 0;
    while (Read_Data(memoria, 0x0360 + contoo) != 0 && contoo < 30) {
        printf("%c", Read_Data(memoria, 0x0360 + contoo));
        __delay_ms(10);
        contoo++;
    }
    contoo = 0;
    while (Read_Data(memoria, 0x0380 + contoo) != 0 && contoo < 30) {
        printf("%c", Read_Data(memoria, 0x0380 + contoo));
        __delay_ms(10);
        contoo++;
    }
    contoo = 0;
    printf("\n\tfactor:");
    if ((Read_Data(memoria, 0x000D)) == 'A') printf(" 1.0");
    else printf(" 0.%c", Read_Data(memoria, 0x000D));
    if ((Read_Data(memoria, 0x0000)) == 'A') printf("\n\n\tEl laberinto se resolvera por ruta");
    if ((Read_Data(memoria, 0x0000)) == 'B') printf("\n\n\tEl laberinto se resolvera por algoritmo");
    if (U1STAbits.URXDA == 0) {
        IEC0bits.T1IE = 1;
        DATO = UARTRXCHAR();
    }
    if (DATO == 13) {
        *error = 1;
    } else goto devuev;
}//fin most_dat

void IOCONFIG(void) {
    ADPCFG = 0XFFFF;
    TRISEbits.TRISE0 = 0;
    TRISEbits.TRISE1 = 0;
    TRISEbits.TRISE2 = 0;
    TRISEbits.TRISE3 = 0;
    TRISBbits.TRISB0 = 1;
    TRISBbits.TRISB1 = 1;
    TRISBbits.TRISB2 = 1;
    TRISBbits.TRISB3 = 1;
    TRISBbits.TRISB4 = 1;
    TRISBbits.TRISB5 = 1;
    TRISBbits.TRISB6 = 1;
    TRISBbits.TRISB7 = 1;
    TRISBbits.TRISB8 = 1;
    TRISF = 0XFFFF;
    TRISFbits.TRISF2 = 1;
    TRISFbits.TRISF3 = 1;
    TRISCbits.TRISC13 = 1;
    TRISCbits.TRISC14 = 1;
    //TRISDbits.TRISD3= 1;
}

void UCONFIG(void) {
    U1BRG = 11;
    IFS0bits.U1RXIF = 0;
    IFS0bits.U1TXIF = 0;

    U1MODE = 0;
    U1STA = 0;
    U1MODEbits.ALTIO = 1; //modo alternativo
    U1STAbits.URXISEL = 00;
    U1MODEbits.UARTEN = 1;
    U1STAbits.UTXEN = 1;
}

void UARTTXCHAR(unsigned char DATO) {
    while (!U1STAbits.TRMT); //ciclo mientras el buffer de transmisión esté lleno
    U1TXREG = DATO; //asignar "data" a TXREG para iniciar transmisión
}

char UARTRXCHAR(void) {
    while (!U1STAbits.URXDA); //ciclo mientras el buffer de recepción esté vacío
    return U1RXREG; //retornar de la función con el dato recibido
}

void config_PWM(unsigned char VEL, float fact) {
    PTCONbits.PTEN = 0; //desactiva el PWM para configurarlo
    PTCONbits.PTMOD = 0; //poner en modo sencillo
    PTMRbits.PTDIR = 0; //configurar conteo hacia arriba por si acaso
    PTCONbits.PTCKPS = 0; //preescaler en 1:1
    PWMCON2bits.UDIS = 0; //debe estar en 0 para modificar duty cicle
    PTPER = 32500; //hasta donde cuenta tmr1
    PDC1 = ((65000 / 100) * VEL); //*fact;                //cuanto se mantiene en 1 PWM1
    PDC2 = ((65000 / 100) * VEL); //*fact;                //cuanto se mantiene en 1 PWM2
    //    DTCON1bits.DTAPS0 = 0;
    //    DTCON1bits.DTAPS1 = 0;         //configuracion dead time
    //    DTCON1bits.DTA = 10;
    PWMCON1bits.PTMOD1 = 1;
    PWMCON1bits.PTMOD2 = 1;
    PWMCON1bits.PEN2H = 1;
    PWMCON1bits.PEN1H = 1;
    PWMCON1bits.PEN1L = 0;
    PWMCON1bits.PEN2L = 0;
    PTCONbits.PTEN = 1; //salir de configuracion y habilitar PWM
}

void devuelvase(void) {
    while (sensor_cruce != 1) {
        LATBbits.LATB8 = ~LATBbits.LATB8;
        __delay_ms(200);
    }
}

void config_factor(unsigned char *error) {
    unsigned char dato, devuelta;
config_fac:
    devuelta = 0;
    printf("\n\tIngrese el factor multiplicativo(0-A)");
    dato = UARTRXCHAR();
    if (dato != '0' && dato != '1' && dato != '2' && dato != '3' && dato != '4' && dato != '5' && dato != '6' && dato != '7' && dato != '8' && dato != '9' && dato != 'A') {
        printf("\n\tIngrese valor correcto");
        __delay_ms(100);
        devuelta = 1;
    }
    if (devuelta == 1) goto config_fac;
    UARTTXCHAR(dato);
    Write_Data(memoria, 0x000D, dato);
    *error = 1;
}

void GenerateStartCondition(void) {
    IFS0bits.MI2CIF = 0;
    I2CCONbits.SEN = 1;
    while (IFS0bits.MI2CIF == 0) {
    }
}

void GenerateRestartCondition(void) {
    IFS0bits.MI2CIF = 0;
    I2CCONbits.RSEN = 1;
    while (IFS0bits.MI2CIF == 0) {
    }
    //I2CCONbits.RSEN=0;
}

void GenerateStopCondition(void) {
    IFS0bits.MI2CIF = 0;
    I2CCONbits.PEN = 1;
    while (IFS0bits.MI2CIF == 0) {
    }
    //I2CCONbits.PEN=0;
}

unsigned char TxByte(unsigned char data) {
    IFS0bits.MI2CIF = 0;
    I2CTRN = data;
    while (IFS0bits.MI2CIF == 0);
    return I2CSTATbits.ACKSTAT;
}

unsigned char RxByte(void) {
    IFS0bits.MI2CIF = 0;
    I2CCONbits.RCEN = 1;
    while (IFS0bits.MI2CIF == 0);
    return I2CRCV;
}

void GenerateNACK(void) {
    IFS0bits.MI2CIF = 0;
    I2CCONbits.ACKDT = 1; //para generar NACK  
    I2CCONbits.ACKEN = 1;
    while (IFS0bits.MI2CIF == 0);
}

void GenerateACK(void) {
    IFS0bits.MI2CIF = 0;
    I2CCONbits.ACKDT = 0; //para generar ACK  
    I2CCONbits.ACKEN = 1;
    while (IFS0bits.MI2CIF == 0);
}

void lcd_init(void) {
    //unsigned char error=1;
    lcd_especial(0x02);
    __delay_us(100);
    lcd_especial(0x28);
    __delay_us(100);
    lcd_especial(0x0c);
    __delay_us(100);
    lcd_especial(0x80);
    __delay_ms(2);
}

void lcd_especial(unsigned char data) {
    unsigned char data_l, data_u;
    data_l = (data << 4)&0xf0; //PARTE BAJA
    data_u = data & 0xf0; //PARTE ALTA
    GenerateStartCondition();
    TxByte(0b01001110);
    TxByte(data_u | 0x0C); //E=1, RS=0
    TxByte(data_u | 0x08); //E=0, RS=0
    TxByte(data_l | 0x0C); //E=1, RS=0
    TxByte(data_l | 0x08); //E=0, RS=0
    GenerateStopCondition();
    __delay_ms(2);
}

void lcd_esc(unsigned char data) {
    unsigned char data_l, data_u;
    data_l = (data << 4)&0xf0; //PARTE BAJA
    data_u = data & 0xf0; //PARTE ALTA
    GenerateStartCondition();
    TxByte(0b01001110);
    TxByte(data_u | 0x0D); //E=1, RS=0
    TxByte(data_u | 0x09); //E=0, RS=0
    TxByte(data_l | 0x0D); //E=1, RS=0
    TxByte(data_l | 0x09); //E=0, RS=0
    GenerateStopCondition();
    __delay_ms(2);
}

void lcd_clear(void) {
    GenerateStartCondition();
    TxByte(0x4E);
    TxByte(0x0C);
    TxByte(0x08);
    TxByte(0x1C);
    __delay_ms(2);
    TxByte(0x18);
    GenerateStopCondition();
    __delay_ms(2);
}

void lcd_set_display(unsigned char data) {
    unsigned char data_l, data_h;
    data_l = (data << 4);
    data_h = (data)&(0xF0);
    data_h = (data_h) + 128;
    GenerateStartCondition();
    TxByte(0x4E);

    TxByte((data_h) | (0xC));
    TxByte((data_h) | (0x8));

    TxByte((data_l) | (0xC));
    TxByte((data_l) | (0x8));
    GenerateStopCondition();

}

void Write_Data(unsigned char slave, unsigned int add, unsigned char datos) {
    unsigned char slv, add_l, add_h;
    add_h = (add >> 8);
    add_l = add;
    GenerateStartCondition();
    slv = slave << 1;
    TxByte(slv);
    TxByte(add_h);
    TxByte(add_l);
    TxByte(datos);
    GenerateStopCondition();
}

void Write_Data_Seq(unsigned char slave, unsigned int add, unsigned char datos[300], unsigned char num_dat, unsigned int comienza) {
    unsigned int cont = 0, cont2 = 0;
    cont2 = comienza;
    __delay_ms(5);
    while (cont < num_dat) {
        Write_Data(slave, (add + cont), datos[cont2]);
        __delay_ms(10);
        cont = cont + 1;
        cont2 = cont2 + 1;
    }
}

unsigned char Read_Data(unsigned char slave, unsigned int add) {
    unsigned char slv, add_l, add_h, datos;
    add_h = (add >> 8);
    add_l = add;
    GenerateStartCondition();
    slv = slave << 1;
    TxByte(slv);
    TxByte(add_h);
    TxByte(add_l);
    GenerateRestartCondition();
    slv = slv + 1;
    TxByte(slv);
    __delay_ms(10);
    datos = RxByte();
    GenerateNACK();
    GenerateStopCondition();
    return datos;
}

void Read_Data_Seq(unsigned char slave, unsigned int add, unsigned char datos[300], unsigned char num_dat) {
    unsigned char slv, add_l, add_h, cont = 0;
    add_h = (add >> 8);
    add_l = add;
    GenerateStartCondition();
    slv = slave << 1;
    TxByte(slv);
    TxByte(add_h);
    TxByte(add_l);
    GenerateRestartCondition();
    slv = slv + 1;
    TxByte(slv);
    __delay_ms(10);
    datos[cont] = RxByte();
    cont = cont + 1;
    __delay_ms(10);
    while (cont < num_dat) {
        GenerateACK();
        datos[cont] = RxByte();
        __delay_ms(10);
        cont = cont + 1;
    }
    __delay_ms(1);
    GenerateNACK();
    GenerateStopCondition();
}

unsigned char Write_RTC(unsigned char add, unsigned char datos) {
    GenerateStartCondition();
    TxByte(0b11010000);
    TxByte(add);
    TxByte(datos);
    GenerateStopCondition();
    return datos;
}

unsigned char Read_RTC(unsigned char add) {
    unsigned char datos;
    GenerateStartCondition();
    TxByte(0b11010000);
    TxByte(add);
    GenerateRestartCondition();
    TxByte(0b11010001);
    __delay_ms(3);
    datos = RxByte();
    GenerateNACK();
    GenerateStopCondition();
    return datos;
}

void leer_hora(void) {
    lcd_set_display(0x0000);

    hord_int = Read_RTC(0x0002);
    hord_int = (hord_int) >> 4;
    hord_int = hord_int + 48;
    __delay_ms(2);
    lcd_esc(hord_int);

    hor_int = Read_RTC(0x0002);
    hor_int = (hor_int)&(0x0F);
    hor_int = hor_int + 48;
    __delay_ms(2);
    lcd_esc(hor_int);

    lcd_esc(':');

    mind_int = Read_RTC(0x0001);
    mind_int = (mind_int) >> 4;
    mind_int = mind_int + 48;
    __delay_ms(2);
    lcd_esc(mind_int);

    min_int = Read_RTC(0x0001);
    min_int = (min_int)&(0x0F);
    min_int = min_int + 48;
    __delay_ms(2);
    lcd_esc(min_int);

    lcd_esc(':');

    segd_int = Read_RTC(0x0000);
    segd_int = (segd_int) >> 4;
    segd_int = segd_int + 48;
    __delay_ms(2);
    lcd_esc(segd_int);

    seg_int = Read_RTC(0x0000);
    seg_int = (seg_int)&(0x0F);
    seg_int = seg_int + 48;
    __delay_ms(2);
    lcd_esc(seg_int);

    // lcd_set_display(0x0B);
    //    if(Read_RTC(0x0003)==1) lcd_esc('L');
    //    if(Read_RTC(0x0003)==2) lcd_esc('m');
    //    if(Read_RTC(0x0003)==3) lcd_esc('M');
    //    if(Read_RTC(0x0003)==4) lcd_esc('J');
    //    if(Read_RTC(0x0003)==5) lcd_esc('V');
    //    if(Read_RTC(0x0003)==6) lcd_esc('S');
    //    if(Read_RTC(0x0003)==7) lcd_esc('D');
    //    lcd_esc('.');    

    //lcd_set_display(0x000F);

    diad_int = Read_RTC(0x0004);
    diad_int = (diad_int) >> 4;
    diad_int = diad_int + 48;
    __delay_ms(2);
    lcd_esc(diad_int);

    dia_int = Read_RTC(0x0004);
    dia_int = (dia_int)&(0x0F);
    dia_int = dia_int + 48;
    __delay_ms(2);
    lcd_esc(dia_int);

    lcd_esc('/');

    mesd_int = Read_RTC(0x0005);
    mesd_int = (mesd_int) >> 4;
    mesd_int = mesd_int + 48;
    __delay_ms(2);
    lcd_esc(mesd_int);

    mes_int = Read_RTC(0x0005);
    mes_int = (mes_int)&(0x0F);
    mes_int = mes_int + 48;
    __delay_ms(2);
    lcd_esc(mes_int);

    lcd_esc('/');

    yrd_int = Read_RTC(0x0006);
    yrd_int = (yrd_int) >> 4;
    yrd_int = yrd_int + 48;
    __delay_ms(2);
    lcd_esc(yrd_int);

    yr_int = Read_RTC(0x0006);
    yr_int = (yr_int)&(0x0F);
    yr_int = yr_int + 48;
    __delay_ms(2);
    lcd_esc(yr_int);

}

void lcd_desplazar_derecha(void) {
    GenerateStartCondition();
    TxByte(0b01001110);
    TxByte(0x10 | 0x0C); //E=1, RS=0
    TxByte(0x10 | 0x08); //E=0, RS=0
    TxByte(0x80 | 0x0C); //E=1, RS=0
    TxByte(0x80 | 0x08); //E=0, RS=0
    GenerateStopCondition();
    __delay_ms(10);
}

void __attribute__((interrupt, no_auto_psv)) _T1Interrupt(void) {
    //    if(contador2_int==6){
    //        lcd_desplazar_derecha();
    //        contador2_int=0;
    //    }
    //    if (contador_int==28){
    //        lcd_desplazar_derecha();
    leer_hora();
    //        contador_int  =0;
    //    }
    //    contador_int ++;
    //    contador2_int ++;
    //    __delay_ms(10);
    IFS0bits.T1IF = 0;
}

void seg_ruta(unsigned int comienzo) {
    unsigned int cont_pwm = 0;
    while (Read_Data(memoria, comienzo + cont_pwm) != 0 && cont_pwm < 30) {
        //IEC0bits.T1IE = 0;
        //IFS0bits.T1IF = 0;
        if (Read_Data(memoria, comienzo + cont_pwm) == 'L') {
            gir_izq();
        }
        if (Read_Data(memoria, comienzo + cont_pwm) == 'R') {
            gir_der();
        }
        if (Read_Data(memoria, comienzo + cont_pwm) == 'F') {
            PTCONbits.PTEN = 0;
            PWMCON1bits.PEN1H = 1;
            PWMCON1bits.PEN2H = 1;
            PWMCON1bits.PEN1L = 0;
            PWMCON1bits.PEN2L = 0;
            PTCONbits.PTEN = 1;
            __delay_ms(400);
            lcd_set_display(0x40);
            lcd_esc('D');
            lcd_esc('E');
            lcd_esc('L');
            lcd_esc('A');
            lcd_esc('N');
            lcd_esc('T');
            lcd_esc('E');
            while (sensor_cruce == 0) {
                seg_algoritmo();
                leer_hora();
            }
            config_PWM(VEL, 1);
            __delay_ms(400);
        }
        if (Read_Data(memoria, comienzo + cont_pwm) == 'B') {
            gir_atras();
        }
        if (Read_Data(memoria, comienzo + cont_pwm) == 'S') {
            PTCONbits.PTEN = 0;
            lcd_set_display(0x40);
            lcd_esc('S');
            lcd_esc('T');
            lcd_esc('O');
            lcd_esc('P');
            lcd_esc(' ');
            lcd_esc(' ');
            lcd_esc(' ');
            while (1) {
            }
        }
        cont_pwm++;
    }
    printf("asdas");
    __delay_ms(10);
}

void gir_der(void) {
    PTCONbits.PTEN = 0;
    PWMCON1bits.PEN1H = 0;
    PWMCON1bits.PEN2H = 1;
    PWMCON1bits.PEN1L = 1;
    PWMCON1bits.PEN2L = 0;
    PTCONbits.PTEN = 1;
    lcd_set_display(0x40);
    lcd_esc('D');
    lcd_esc('E');
    lcd_esc('R');
    lcd_esc(' ');
    lcd_esc(' ');
    lcd_esc(' ');
    lcd_esc(' ');
    while (sensor_cruce != 1) {
        leer_hora();
    }
    __delay_ms(400);
}

void gir_izq(void) {

    PTCONbits.PTEN = 0;
    PWMCON1bits.PEN1H = 1;
    PWMCON1bits.PEN2H = 0;
    PWMCON1bits.PEN1L = 0;
    PWMCON1bits.PEN2L = 1;
    PTCONbits.PTEN = 1;
    lcd_set_display(0x40);
    lcd_esc('I');
    lcd_esc('Z');
    lcd_esc('Q');
    lcd_esc(' ');
    lcd_esc(' ');
    lcd_esc(' ');
    lcd_esc(' ');
    while (sensor_cruce == 0) {
        leer_hora();
    }
    __delay_ms(400);
}

void gir_atras(void) {
    PTCONbits.PTEN = 0;
    PWMCON1bits.PEN1H = 1;
    PWMCON1bits.PEN2H = 0;
    PWMCON1bits.PEN1L = 0;
    PWMCON1bits.PEN2L = 1;
    PTCONbits.PTEN = 1;
    lcd_set_display(0x40);
    lcd_esc('A');
    lcd_esc('T');
    lcd_esc('R');
    lcd_esc('A');
    lcd_esc('S');
    lcd_esc(' ');
    lcd_esc(' ');
    lcd_esc(' ');
    lcd_esc(' ');
    lcd_esc(' ');
    lcd_esc(' ');
    while (sensor_cruce != 1) {
        leer_hora();
    }
    __delay_ms(400);
    while (sensor_cruce != 1) {
        leer_hora();
    }
    __delay_ms(400);
}

void seg_algoritmo(void) {
    if (sensor_s1 == 1 && sensor_s0 == 0) {
        PTCONbits.PTEN = 0;
        PDC1 = ((65000 / 100) * VEL); //*fact;                //cuanto se mantiene en 1 PWM1
        PDC2 = ((65000 / 100) * VEL) * factor; //*fact;                //cuanto se mantiene en 1 PWM2
        PWMCON1bits.PEN1H = 1;
        PWMCON1bits.PEN2H = 1;
        PWMCON1bits.PEN1L = 0;
        PWMCON1bits.PEN2L = 0;
        PTCONbits.PTEN = 1;
    } else if (sensor_s1 == 0 && sensor_s0 == 1) {
        PTCONbits.PTEN = 0;
        PDC1 = ((65000 / 100) * VEL) * factor; //*fact;                //cuanto se mantiene en 1 PWM1
        PDC2 = ((65000 / 100) * VEL); //*fact;                //cuanto se mantiene en 1 PWM2
        PWMCON1bits.PEN1H = 1;
        PWMCON1bits.PEN2H = 1;
        PWMCON1bits.PEN1L = 0;
        PWMCON1bits.PEN2L = 0;
        PTCONbits.PTEN = 1;
    } else if (sensor_s1 == 1 && sensor_s0 == 1) {
        PTCONbits.PTEN = 0;
        PDC1 = ((65000 / 100) * VEL); //*fact;                //cuanto se mantiene en 1 PWM1
        PDC2 = ((65000 / 100) * VEL); //*fact;
        PWMCON1bits.PEN1H = 1;
        PWMCON1bits.PEN2H = 1;
        PWMCON1bits.PEN1L = 0;
        PWMCON1bits.PEN2L = 0;
        PTCONbits.PTEN = 1;
    } else {
        PTCONbits.PTEN = 0;
        PDC1 = ((65000 / 100) * VEL); //*fact;                //cuanto se mantiene en 1 PWM1
        PDC2 = ((65000 / 100) * VEL); //*fact;
        PWMCON1bits.PEN1H = 0;
        PWMCON1bits.PEN2H = 0;
        PWMCON1bits.PEN1L = 1;
        PWMCON1bits.PEN2L = 1;
        PTCONbits.PTEN = 1;
    }
}

void config_VEL(void) {
    VEL = 0;
    if (Read_Data(memoria, 0x000E) == '1') {
        if (Read_Data(memoria, 0x0001) == '1') VEL = 100;
        else {
            VEL = VEL + (((Read_Data(memoria, 0x0002)) - 48)*10);
            VEL = VEL + ((Read_Data(memoria, 0x0003)) - 48);
        }
    }
    if (Read_Data(memoria, 0x000E) == '2') {
        if (Read_Data(memoria, 0x0004) == '1') VEL = 100;
        else {
            VEL = VEL + (((Read_Data(memoria, 0x0005)) - 48)*10);
            VEL = VEL + ((Read_Data(memoria, 0x0006)) - 48);
        }
    }
    if (Read_Data(memoria, 0x000E) == '3') {
        if (Read_Data(memoria, 0x0007) == '1') VEL = 100;
        else {
            VEL = VEL + (((Read_Data(memoria, 0x0008)) - 48)*10);
            VEL = VEL + ((Read_Data(memoria, 0x0009)) - 48);
        }
    }
    if (Read_Data(memoria, 0x000E) == '4') {
        if (Read_Data(memoria, 0x000A) == '1') VEL = 100;
        else {
            VEL = VEL + (((Read_Data(memoria, 0x000B)) - 48)*10);
            VEL = VEL + ((Read_Data(memoria, 0x000C)) - 48);
        }
    }
}

void config_fac_mult(void) {
    factor = .0;
    if (Read_Data(memoria, 0x000D) == 'A') factor = 1.0;
    if (Read_Data(memoria, 0x000D) == '9') factor = 0.9;
    if (Read_Data(memoria, 0x000D) == '8') factor = 0.8;
    if (Read_Data(memoria, 0x000D) == '7') factor = 0.7;
    if (Read_Data(memoria, 0x000D) == '6') factor = 0.6;
    if (Read_Data(memoria, 0x000D) == '5') factor = 0.5;
    if (Read_Data(memoria, 0x000D) == '4') factor = 0.4;
    if (Read_Data(memoria, 0x000D) == '3') factor = 0.3;
    if (Read_Data(memoria, 0x000D) == '2') factor = 0.2;
    if (Read_Data(memoria, 0x000D) == '1') factor = 0.1;
    if (Read_Data(memoria, 0x000D) == '0') factor = 0.0;
}