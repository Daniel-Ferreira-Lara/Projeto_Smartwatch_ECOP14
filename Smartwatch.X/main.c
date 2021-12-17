/*
 * File:   main.c
 * Author: Daniel ferreira Lara
 *
 * Created on 30 de Novembro de 2021, 11:14
 */
#include <proc/pic18f4520.h>
#include "adc.h"
#include "pwm.h"
#include "lcd.h"
#include "ds1307.h"
#include "keypad.h"
#include "bits.h"
#include "rgb.h"
#include "atraso.h"
#include "serial.h"
#include "timer.h"
#define L_CLR 0x01//Limpa linha LCD
#define L_OFF 0X08//Apaga o LCD
unsigned char  nome[16],matricula[16];//infos iniciais
unsigned char change=1,crono,tempos,som=0;
unsigned char musica=1,tela=0,freq=2;
unsigned int temperatura=01;//Valor de temperatura
static const int radio[] = {883,901,963,1005,1016,1074,1108,1185};//frequencias das radios
static const char semana[] = {'D','o','m','S','e','g','T','e','r','Q','u','a','Q','u','i','S','e','x','S','a','b'};//Dia da semanda 

void musicas(unsigned char n){
    switch(n){
        case 0:
            lcdString("Hey Jude - The Beetwes ");
            break;
        case 1:
            lcdString("Ana Julia - Los Hermanos");
            break;
        case 2:
            lcdString("Crazy - Aerosmith");
            break;
        case 3:
            lcdString("Andei so - Natiruts");
            break;
        case 4:
            lcdString("Infiel - Marilia Mendoca");
            break;
        case 5:
            lcdString("Epitafio - Titas");
            break;        
    }
}
void radios(unsigned char n){
    switch(n){
        case 0:
            lcdString("FM Cidade ");
            break;
        case 1:
            lcdString("Sucesso FM ");
            break;
        case 2:
            lcdString("Mais legal ");
            break;
        case 3:
            lcdString("Radio Pan ");
            break;
        case 4:
            lcdString("Radio Povo ");
            break;
        case 5:
            lcdString("Radional ");
            break;     
        case 6:
            lcdString("Politica FM");
            break;
        case 7:
            lcdString("Sertaneja ");
            break;
    }
}//Prepara os nomes das radios existentes
void LogoUnifei(){
    unsigned char i;
    char logo[48] = {
        0x01, 0x03, 0x03, 0x0E, 0x1C, 0x18, 0x08, 0x08, //0,0
        0x11, 0x1F, 0x00, 0x01, 0x1F, 0x12, 0x14, 0x1F, //0,1
        0x10, 0x18, 0x18, 0x0E, 0x07, 0x03, 0x02, 0x02, //0,2
        0x08, 0x18, 0x1C, 0x0E, 0x03, 0x03, 0x01, 0x00, //1,0
        0x12, 0x14, 0x1F, 0x08, 0x00, 0x1F, 0x11, 0x00, //1,1
        0x02, 0x03, 0x07, 0x0E, 0x18, 0x18, 0x10, 0x00 //1,2
    };
    lcdCommand(0x40);
    for(i=0;i<8;i++)
        lcdChar(logo[i]);
    lcdCommand(0x48);
    for(i=8;i<16;i++)
        lcdChar(logo[i]);
    lcdCommand(0x50);
    for(i=16;i<24;i++)
        lcdChar(logo[i]);
    lcdCommand(0x58);
    for(i=24;i<32;i++)
        lcdChar(logo[i]);
    lcdCommand(0x70);
    for(i=32;i<40;i++)
        lcdChar(logo[i]);
    lcdCommand(0x78);
    for(i=40;i<48;i++)
        lcdChar(logo[i]);
}//Configura logo da UNIFEI na memoria

void Limpa(){
    lcdPosition(0,0);
    lcdCommand(L_CLR);
    lcdPosition(1,0);
    lcdCommand(L_CLR);
}

void buzzer(unsigned int time){ //Aciona o buzzer quando finalizar o timer
    pwmSet(100);
    atraso_ms(time);
    pwmSet(0);
}

void volume(unsigned char valor){
    unsigned char i;
    for(i=0;i<16;i++)
        if(i<valor+1)                
            lcdChar(-1);
        else
            lcdChar(' ');
}

unsigned int LeSerial(unsigned int tempo){
    unsigned char tmp[2],Atual;
    tmp[0] = serial_rx(tempo);
    tmp[1] = serial_rx(100);
    
    Atual = (((tmp[0] - 48)%10)*10 + ((tmp[1] - 48)%10));
    if(tmp[0]!=0xa5&&tmp[1]!=0xa5)
        return (int)Atual;
    else
        return 0;
}//Le dois digitos da Serial

void LeInfo(unsigned char *palavra[]){
    unsigned char aux=0,i=0;
    lcdPosition(1,0);
    //Recebe a informacao
    while(aux!='.'){
        aux=serial_rx(5000);
        if(aux!=0xa5&&aux!='.'){
            *palavra[i]=aux;
            serial_tx(aux);
            lcdChar(aux);
            i++;
        }
    }
    *palavra[i]='\0';
}

void IMC(){
    unsigned char aux,imc;
    lcdPosition(0,0);
    lcdString("Digite o IMC: ");
    serial_tx_str("\nDigite o IMC: ");
    lcdPosition(1,3); 
    aux=LeSerial(10000);
    if(aux!=0)
        imc=aux;
    Limpa();
    lcdPosition(1,0);
    if(imc<15||imc>29)
        rgbColor(1),lcdString("Procure o medico");//Perigo, Red Zone
    else if(imc<18||imc>25)
        rgbColor(3),lcdString("Se alimente bem");//Cuidado, Yellow Zone
    else 
        rgbColor(2),lcdString("Ok, Saude otima");//Ok, Green Zone*/
    atraso_ms(3000);
}

void data(){
    unsigned char i;
    unsigned int TempAtual;
    //Logo Parte 1
    lcdPosition(0,0);
    lcdChar(0);
    lcdChar(1);
    lcdChar(2);
    lcdString(" ");
    //Dia da Semana
    for(i=getWeekDay()*3;i<getWeekDay()*3+3;i++)
        lcdChar(semana[i]);
    lcdChar(' ');
    //Data Completa
    lcdChar((getDays()/10%10)+48);
    lcdChar((getDays()%10)+48);
    lcdChar('/');
    lcdChar((getMonths()/10%10)+48);
    lcdChar((getMonths()%10)+48);
    lcdChar('/');
    lcdChar((getYears()/10%10)+48);
    lcdChar((getYears()%10)+48);
    lcdString("  ");
    //Logo Parte 2
    lcdPosition(1,0);
    lcdChar(3);
    lcdChar(6);
    lcdChar(7);
    //Temperatura atual
    TempAtual=temperatura/2+358;
    lcdChar(' ');
    lcdChar((TempAtual/100%10)+48);
    lcdChar((TempAtual/10%10)+48);
    lcdChar(',');
    lcdChar((TempAtual%10)+48);
    lcdString("C ");
    //Horario
    lcdChar((getHours()/10%10)+48);
    lcdChar((getHours()%10)+48);
    lcdChar(':');
    lcdChar((getMinutes()/10%10)+48);
    lcdChar((getMinutes()%10)+48);
    lcdString("   ");
}

void Cronometro(){
    unsigned char anterior;
    if(tempos!=anterior){
        lcdPosition(0,0);
        lcdString("   Cronometro   ");
        lcdPosition(1,0);
        lcdString("      ");
        lcdChar(((tempos/600)%6)+48);
        lcdChar(((tempos/60)%10)+48);
        lcdChar(':');
        lcdChar(((tempos/10)%6)+48);
        lcdChar(((tempos)%10)+48);
        lcdString("     ");
        anterior=tempos;
        change=1;
    }
}

void Telas(){
    unsigned char valor,anterior;
    valor = adcRead(1)/64;//de 0 a 1023 para de 0 a 16
    if(valor!=anterior) change=1;
    if(change){
        switch(tela){
            case 0: 
                data();
                break;
            case 1:
                lcdPosition(0,0);
                musicas(musica);
                lcdPosition(1,0);
                volume(valor);
                break;
            case 2:
                lcdPosition(0,0);
                radios(freq);
                lcdChar((radio[freq]/1000%10)+48);
                lcdChar((radio[freq]/100%10)+48);
                lcdChar((radio[freq]/10%10)+48);
                lcdChar(',');
                lcdChar((radio[freq]%10)+48);
                lcdString("   ");
                lcdPosition(1,0);
                volume(valor);
                break;
            case 3:
                Cronometro();
                break;
            default:
                tela=0;
                break;
        }
        change=0;
        anterior=valor;
    }   
}

void Temperatura(){
    unsigned char aux;
    aux=LeSerial(50);
    if(aux!=0)
        temperatura=aux;
}

void Bemvindo(){ 
    unsigned char i=0;
    lcdPosition(0,0);
    lcdString("Este eh o maior");
    lcdPosition(1,0);
    lcdString("Smartwhact visto");
    atraso_ms(3000);
    Limpa();
    
    lcdPosition(0,0);
    lcdString("Ele precisa ser");
    lcdPosition(1,0);
    lcdString(" Configurado...");
    atraso_ms(3000);
    Limpa();
    
    lcdPosition(0,0);
    lcdString("Diga seu nome:");
    serial_tx_str("\nDiga seu nome: ");
    LeInfo(nome);//Nome
    Limpa();
    
    lcdPosition(0,0);
    lcdString("Diga a matricula");
    serial_tx_str("\nDiga seu numero de matricula: ");
    LeInfo(matricula);//Matricula
    Limpa();
    
    IMC();//IMC
    atraso_ms(3000);
    Limpa();
    lcdPosition(0,0);
    lcdString("Seja bem vindo/a");
    lcdPosition(1,0);
    atraso_ms(5000);
}

void Finalizando(){
    buzzer(1000);
    Limpa();
    lcdPosition(0,0);
    lcdString("Algo deu errado!");
    lcdPosition(1,0);
    lcdString("Desligando...");
    atraso_ms(5000);
    lcdCommand(OFF);
}

void main(void) {
    unsigned char i=0,tecla,processos=0,contador;
    rgbInit();
    kpInit();
    adcInit();
    lcdInit();
    pwmInit();
    LogoUnifei();
    timerInit();
    serial_init();
    turnOn(4); 
    buzzer(500);
    Bemvindo();//Inicial
    for(;;){
        timerReset(50);
        switch(processos){
            case 0:
                Telas();
                processos=1;
                break;
            case 1:
                for(i=0;i<5;i++){
                    kpDebounce();
                    if (kpRead() != tecla){
                        change=1;
                        tecla = kpRead(); 
                            //cruz esquerda
                            if (bitTst(tecla, 0)&&freq<7)  freq++,tela=2;//cima
                            if (bitTst(tecla, 2)&&freq)  freq--,tela=2;//baixo
                            if (bitTst(tecla, 1)&&musica)  musica--,tela=1;//esquerda
                            if (bitTst(tecla, 3)&&musica<5)  musica++,tela=1;//direita
                            //centro
                            if (bitTst(tecla, 4))tela=0;//esquerda
                            //cruz direita
                            if (bitTst(tecla, 7)){if(tela==3)tempos=0,crono=1;tela=3;}//baixo
                            if (bitTst(tecla, 6))  tela=3,crono=!crono;//direita
                    }
                }
                processos=2;
                break;
            case 2:
                if(contador>20)tempos++,contador=0;
                processos=3;
                break;
            case 3:
                Temperatura();
                processos=0;
                break;
            default:
                processos=0;
                break;
        }
        if(crono) contador++;
        timerWait();
    }
    Finalizando();
}