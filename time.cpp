#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <wiringPi.h>

#define PIN_CLK 21
#define PIN_DAT 22
#define PIN_CLR 26

#define PIN_STB1 23
#define PIN_STB2 27
#define PIN_STB3 24
#define PIN_STB4 28
#define PIN_STB5 29
#define PIN_STB6 25

#define SEG_1 PIN_STB1
#define SEG_2 PIN_STB2
#define SEG_3 PIN_STB3
#define SEG_4 PIN_STB4
#define SEG_5 PIN_STB5
#define SEG_6 PIN_STB6

void off7Seg(unsigned char port, unsigned char dp)
{
	unsigned char seg_data[8] = {
		dp, 0, 0, 0, 0, 0, 0, 0
	};
	
	
	for(int i = 0; i < 8; i++){
		digitalWrite(PIN_CLK, 0);
		digitalWrite(PIN_DAT, seg_data[i]);
		digitalWrite(PIN_CLK, 1);
	}
	digitalWrite(port, 0);
	usleep(100);
	digitalWrite(port, 1);
}
void set7Seg(unsigned char port, unsigned char num, unsigned char dp)
{
	unsigned char seg_data[10][8] = {
		{dp, 0, 1, 1, 1, 1, 1, 1},
		{dp, 0, 0, 0, 0, 1, 1, 0},
		{dp, 1, 0, 1, 1, 0, 1, 1},
		{dp, 1, 0, 0, 1, 1, 1, 1},
		{dp, 1, 1, 0, 0, 1, 1, 0},
		{dp, 1, 1, 0, 1, 1, 0, 1},
		{dp, 1, 1, 1, 1, 1, 0, 1},
		{dp, 0, 1, 0, 0, 1, 1, 1},
		{dp, 1, 1, 1, 1, 1, 1, 1},
		{dp, 1, 1, 0, 1, 1, 1, 1}
	};
	
	
	for(int i = 0; i < 8; i++){
		digitalWrite(PIN_CLK, 0);
		digitalWrite(PIN_DAT, seg_data[num][i]);
		digitalWrite(PIN_CLK, 1);
	}
	digitalWrite(port, 0);
	usleep(100);
	digitalWrite(port, 1);
}

int main()
{
	if(wiringPiSetup() == -1){
		printf("GPIO Setup Error\n");
		return 1;
	}

	pinMode(PIN_CLR, OUTPUT);
	pinMode(PIN_CLK, OUTPUT);
	pinMode(PIN_DAT, OUTPUT);
	
	pinMode(PIN_STB1, OUTPUT);
	pinMode(PIN_STB2, OUTPUT);
	pinMode(PIN_STB3, OUTPUT);
	pinMode(PIN_STB4, OUTPUT);
	pinMode(PIN_STB5, OUTPUT);
	pinMode(PIN_STB6, OUTPUT);
	
	digitalWrite(PIN_CLR, 1);
	digitalWrite(PIN_STB1, 1);
	digitalWrite(PIN_STB2, 1);
	digitalWrite(PIN_STB3, 1);
	digitalWrite(PIN_STB4, 1);
	digitalWrite(PIN_STB5, 1);
	digitalWrite(PIN_STB6, 1);
	
	time_t timer;
	struct timeval myTime;
	struct tm *local;
	unsigned char dps[6] = {
		0, 0, 0, 0, 0, 0
	};
	while(1){
		gettimeofday(&myTime, NULL);
		local = localtime(&myTime.tv_sec);

		unsigned char hour_lower = local->tm_hour % 10;
		unsigned char hour_higher = local->tm_hour / 10;
		unsigned char min_lower = local->tm_min % 10;
		unsigned char min_higher = local->tm_min / 10;
		unsigned char sec_lower = local->tm_sec % 10;
		unsigned char sec_higher = local->tm_sec / 10;

		if(myTime.tv_usec < 166 * 1000){
			dps[0] = 0;
			dps[1] = 0;
			dps[2] = 0;
			dps[3] = 0;
			dps[4] = 0;
			dps[5] = 1;
		}else if(myTime.tv_usec > 166 * 5 * 1000){
			dps[0] = 1;
			dps[1] = 0;
			dps[2] = 0;
			dps[3] = 0;
			dps[4] = 0;
			dps[5] = 0;
		}else if(myTime.tv_usec > 166 * 4 * 1000){
			dps[0] = 0;
			dps[1] = 1;
			dps[2] = 0;
			dps[3] = 0;
			dps[4] = 0;
			dps[5] = 0;
		}else if(myTime.tv_usec > 166 * 3 * 1000){
			dps[0] = 0;
			dps[1] = 0;
			dps[2] = 1;
			dps[3] = 0;
			dps[4] = 0;
			dps[5] = 0;
		}else if(myTime.tv_usec > 166 * 2 * 1000){
			dps[0] = 0;
			dps[1] = 0;
			dps[2] = 0;
			dps[3] = 1;
			dps[4] = 0;
			dps[5] = 0;
		}else if(myTime.tv_usec > 166 * 1 * 1000){
			dps[0] = 0;
			dps[1] = 0;
			dps[2] = 0;
			dps[3] = 0;
			dps[4] = 1;
			dps[5] = 0;
		}
		
		set7Seg(SEG_1, hour_higher, dps[0]);
		set7Seg(SEG_2, hour_lower, dps[1]);
		set7Seg(SEG_3, min_higher, dps[2]);
		set7Seg(SEG_4, min_lower, dps[3]);
		set7Seg(SEG_5, sec_higher, dps[4]);
		set7Seg(SEG_6, sec_lower, dps[5]);
		
		usleep(100);
	}
	

	return 0;
}
