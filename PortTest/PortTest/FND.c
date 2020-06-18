/*
 * FND.c
 *
 * Created: 2020-06-15 오전 11:03:32
 *  Author: PKNU
 */ 
#include <avr/io.h>
#include <util/delay.h>

int Count_TR(unsigned char flag);

int main()
{
	unsigned char FND_DATA_TBL[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7c,0x07,0x7f,0x6F};
	int cnt=0;
	unsigned char Switch_flag=0;
	
	DDRA=0xFF;
	DDRD=0x00;
	DDRF=0xFF;
	
	while(1)
	{	
		Switch_flag = PIND;
		while(PIND !=0x00); // 스위치를 땔때까지 대기
		
		
		if(Switch_flag !=0) cnt += Count_TR(Switch_flag);
		
		if(cnt<0) 
			cnt=0;
		else if(cnt>99) 
			cnt = 99;
		
		PORTF = FND_DATA_TBL[cnt%10];
		PORTA = FND_DATA_TBL[cnt/10];
		_delay_ms(100);
	}
	
	return 0;
}

int Count_TR(unsigned char flag)
{
	int count=0;
	switch(flag){
		case 0x01:
			count=1;
			break;
		case 0x02:
			count=2;
			break;
		case 0x04:
			count=-1;
			break;
		case 0x08:
			count=-2;
			break;
	}
	return count;
}