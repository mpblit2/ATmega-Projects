// ***************************gtvbh njm,********************************
// Project: Czytnik impulsów z enkodera
//
// Author: Janusz Tomaszewski
//
// Module description: 	Program czyta impulsy z enkodera w obu kierunkach
//				i wyœwietla sumê odczytanych impulsów na 
//				wyœwietlaczach 7-segmentowym
// *********************************************************************

#define F_CPU 8000000UL

#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#define LED PORTB //zdefiniowanie sta³ych
#include <setjmp.h>

jmp_buf bufor1, bufor2;

int i=0;

//Zmienne do przechowywania wartoœci dla wyœwietlaczy
int p1;
int p2;
int p3;
int p4;

//WskaŸniki do operowania na zmiennych wyœwietlacza
int *pp1;
int *pp2;
int *pp3;
int *pp4;


//Przerwanie INT0 wykrywanie zbocza narastaj¹cego
ISR(INT0_vect )
{
	if(!bit_is_clear(PIND, PD3))
	{
		//LED = 0xff; //zaœwiecenie LEDów
		i++;
	}
	else
	{
		//LED = 0x00; //zgaszenie LEDów
		i--;
	}
}
//Przerwanie INT1 wykrywanie zbocza opadaj¹cego
ISR(INT1_vect )
{
	if(!bit_is_clear(PIND, PD2))
	{
		LED = 0xff; //zaœwiecenie LEDów
		//i++;
	}	
	else
	{
			
		LED = 0x00; //zgaszenie LEDów
		//i--;
	}
}
	
	
void wyswietl(int* liczba);
void proces1(int* p);
void proces2(int* p);

int main(void)
{

int *pint;


pint = &i;

DDRC = 0xFF;					// wyœwietlacze 7-segmentowe
PORTC = 0xFF;
//DDRD = 0xFF;					// wybor wyswietlacza
//PORTD = 0XFF;

DDRB = 0xff; //konfiguracja portu jako wyjœcia
/* set PD2 and PD3 as input */
//DDRD &=~ (1 << PD2);	/* PD2 and PD3 as input */
//DDRD &=~ (1 << PD3);
DDRD = 0xf0;
PORTD = 0XFF;
PORTD |= (1 << PD3)|(1 << PD2); /* PD2 and PD3 pull-up enabled */
GICR |= (1<<INT0)|(1<<INT1);	/* enable INT0 and INT1 */
MCUCR |= (1<<ISC01)|(1<<ISC11)|(1<<ISC10); /* INT0 - falling edge, INT1 - reising edge */
/* enable interrupt's */
sei();
while(1)
{
//do nothing ;)
//_delay_ms(1);

if(abs(i)<10)
{
	p1 = abs(i);
	p2 = 0;
	p3 = 0;
	p4 = 0;
}else if(abs(i)<100)
{
	p1 = abs(i) - abs(i)/10*10;
	p2 = abs(i)/10;
	p3 = 0;
	p4 = 0;
}else if(abs(i) < 1000)
{
	p1 = (abs(i) - abs(i)/100*100) - (abs(i) - abs(i)/100*100)/10*10;
	p2 = (abs(i) - abs(i)/100*100)/10;
	p3 = abs(i)/100;
	p4 = 0;
}else if(abs(i) < 10000)
{
	p1 = (((abs(i) - abs(i)/1000*1000) - (abs(i) - abs(i)/1000*1000)/100*100) - ((abs(i) - abs(i)/1000*1000) - (abs(i) - abs(i)/1000*1000)/100*100)/10*10);
	p2 = ((abs(i) - abs(i)/1000*1000) - (abs(i) - abs(i)/1000*1000)/100*100)/10;
	p3 = (abs(i) - abs(i)/1000*1000)/100;
	p4 = abs(i)/1000;
}

pp1 = &p1;
			pp2 = &p2;
			pp3 = &p3;
			pp4 = &p4;

/*	if(setjmp(bufor1))
		{
			p1 = i;
			p2 = i/10;
			p3 = i/100;
			p4 = i/1000;

			pp1 = &p1;
			pp2 = &p2;
			pp3 = &p3;
			pp4 = &p4;
			
			proces1(pp1);
		}
		proces2(pp2);
*/


	wyswietl(pp1);
	

	PORTD &= ~_BV(PORTD4);
	_delay_ms(1);	
	PORTD |= _BV(PORTD4);
	
	wyswietl(pp2);
	PORTD &= ~_BV(PORTD5);
	_delay_ms(1);
	PORTD |= _BV(PORTD5);
	
	wyswietl(pp3);
	PORTD &= ~_BV(PORTD6);
	_delay_ms(1);
	PORTD |= _BV(PORTD6);
	wyswietl(pp4);
	PORTD &= ~_BV(PORTD7);
	_delay_ms(1);
	PORTD |= _BV(PORTD7);

/*
	
	
	if(*pint < 10)
	{
		wyswietl(pp1);
		PORTD &= ~_BV(PORTD4);
	}
	else if(*pint <100)
	{
		PORTD |= _BV(PORTD4);
		*pint = *pint/100;
		wyswietl(pint);
		PORTD &= ~_BV(PORTD5);	
	}
	else if(*pint < 1000)
	{
		PORTD |= _BV(PORTD5);
		*pint = *pint/1000;
		wyswietl(pint);
		PORTD &= ~_BV(PORTD6);
	}else if(*pint < 10000)
	{
		PORTD |= _BV(PORTD6);
		*pint = *pint/10000;
		wyswietl(pint);
		PORTD &= ~_BV(PORTD7);
	}
*/
}
return 0;
}

//Funkcja do wyœwietlania liczby na wyœwietlaczu 7-segmentowym
void wyswietl(int* liczba)
{
	switch(*liczba)
	{
		case 0: PORTC = 0X03;
				break;
				
		case 1: PORTC = 0X9F;
				break;
				
		case 2: PORTC = 0X25;
				break;
				
		case 3: PORTC = 0X0D;
				break;
				
		case 4: PORTC = 0X99;
				break;
				
		case 5: PORTC = 0X49;
				break;
		
		case 6: PORTC = 0X41;
				break;
		
		case 7: PORTC = 0X1F;
				break;
		
		case 8: PORTC = 0X01;
				break;
		
		case 9: PORTC = 0X09;
				break;
				
		default: PORTC = 0XFF;
	}
	
}

void proces1(int* p)
{
		//PORTD |= _BV(PORTD5);	
		wyswietl(p);
		PORTD &= ~_BV(PORTD4);
		_delay_ms(0.1);
		
}

void proces2(int* p)
{
	PORTD |= _BV(PORTD4);
	wyswietl(p);
	PORTD &= ~_BV(PORTD5);	
	_delay_ms(0.1);

	longjmp(bufor1, 1);
}