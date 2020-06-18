/*
 * exam
 *
 * Created: 2020-06-18 오전 10:17:46
 * Author : 김태관 
 * Atmega128A 시험
 */ 

#include <avr/io.h>
#include <util/delay.h>

void putch(unsigned char data) //송신 함수
{
	while((UCSR0A & 0x20)==0); //송신 대기
	UDR0=data;					
	UCSR0A |=0x20;
}

unsigned char getch() // 수신 함수
{
	unsigned char data;
	while((UCSR0A & 0x80)==0); // 수신 대기
	data=UDR0;
	return data;
}


int main(void)
{
	unsigned char title[]="\r\n\t ++++ Menu ++++ \n L : LED \t (동작 - 좌우로 약 500ms 간격으로 이동) \n C : CDS \t (동작 - CDS의 ADC 변환 값을 터미널에 출력)\n 0 ~ 7 : PIANO \t (동작 - 숫자키별로 음계발생)\n ";
	
	unsigned char title2[]="S : STEP MOTOR \t (동작 - 정방향 : f, 역방향 : r)\n push button : stop \t (동작 - kit의 푸쉬버튼을 누르면 동작을 멈춘다.(인터럽트))";
						
						
						
						
    unsigned char i =0;
	
	//UART 통신 설정.
	UCSR0A=0x00;
	UCSR0B=0x18;
	UCSR0C=0x06;
	
	UBRR0H=0x00;
	UBRR0L=0x03;
	
	while (title[i]!='\0') 
    {
		putch(title[i++]);
    }
	i=0;
	while (title2[i]!='\0')
	{
		putch(title[i++]);
	}
	return 0;
}

