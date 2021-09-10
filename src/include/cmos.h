// CMOS header module header

extern unsigned char second;
extern unsigned char minute;
extern unsigned char hour;
extern unsigned char day;
extern unsigned char month;
extern unsigned int year;
extern unsigned char century;
extern unsigned char last_second;
extern unsigned char last_minute;
extern unsigned char last_hour;
extern unsigned char last_day;
extern unsigned char last_month;
extern unsigned char last_year;
extern unsigned char last_century;
extern unsigned char registerB;

void read_rtc();

void getnowtime();