/*
* Sistema Operacional BrOS
* CodeD by CorN_Sk8
* Kernel em C
*/

#include "include\floppy.h"
#include "include\system.h"
#include "include\video.h"
#include "include\in_out.h"

int fdc_flag            = 0;
int ST0                 = 0;
int fdc_status[7]       = { 0 };
int fdc_track           = 0xFF;
int fdc_motor           = 0;
int fdc_motor_countdown = 0;
int fdc_device          = 0;
int *fdc_buffer;

static int fdc_wait_until_ready( void )
{
   int counter, status;

   for( counter = 0; counter < 10000; counter++ )
   {
      status = inportb( FDC_MSR );
      if( status & MSR_READY )
      {
         return( status );
      }
   }
   return( -1 );
}

static int fdc_getbyte()
{
   int msr;

   msr = fdc_wait_until_ready();
   if( msr < 0 )
   {
      return( -1 );
   }
   msr &= MSR_DIR | MSR_READY | MSR_BUSY | MSR_DMA;
   if( msr == (MSR_DIR | MSR_READY | MSR_BUSY) )
   {
      return( inportb(FDC_DATA) );
   }
   return( -1 );
}

static int fdc_sendbyte( int b )
{
   int msr;

   msr = fdc_wait_until_ready();
   if( msr < 0 )
   {
      return( -1 );
   }
   if( (msr & (MSR_READY | MSR_DIR | MSR_DMA)) == MSR_READY )
   {
      outportb( FDC_DATA, b );
      return( 0 );
   }
   return( -1 );
}

int fdc_wait(int sensei){
     int i;
     
     fdc_flag=1;
     while(fdc_flag) ;
     
     i = 0;
     while( (i < 7) && (inportb(FDC_MSR) & MSR_BUSY) ){
         fdc_status[ i++ ] = fdc_getbyte();
     }
     
     if( sensei ){
         fdc_sendbyte( CMD_SENSEI );
         ST0 = fdc_getbyte();
         fdc_track = fdc_getbyte();
     }
     return 1;   
}

void fdc_motor_on(){
     
     char devs[] = {0x1C, 0x2D, 0x4E, 0x8F};   
     
     if(!fdc_motor){
         outportb( FDC_DOR, devs[fdc_device] );
         timer_wait(100);
         fdc_motor = 1;                   
     } 
}

void fdc_motor_off(){
     
     if(fdc_motor){
           outportb( FDC_DOR, 0x0C );
          fdc_motor = 1;                   
     }
}

static void fdc_recalibrate()
{
   // Turn the motor on.
   fdc_motor_on();

   // Send recalibrate command.
   fdc_sendbyte( CMD_RECAL );
   fdc_sendbyte( 0 );

   // Wait until recalibrate command is finished.
   fdc_wait( 1 );

   // Turn the motor off.
   fdc_motor_off();
}

int __fdc_seek( int track ){

   // If already there return.
   if( fdc_track == track )
   {
      return( 0 );
   }

   // Turn the motor on.
   fdc_motor_on();

   // Send seek command.
   fdc_sendbyte( CMD_SEEK );
   fdc_sendbyte( 0 );
   fdc_sendbyte( track );

   // Wait until seek is finished.
   if( fdc_wait(1) == 0 )
   {
      // Timeout!
      fdc_motor_off();
      printf("ERRO");
      return 0;
   }

   // Let the head settle for 15msec.
   timer_wait(50);

   // Turn off the motor.
   fdc_motor_off();

   // Check if seek worked.
   if( (ST0 == 0x20) && (fdc_track == track) )
   {
      return( 0 );
   }
   return 0;
}

int reset_floppy(int device){
   
    fdc_device = device;       

    outportb(FDC_DOR, 0x00);
     
    outportb(FDC_CCR, 0x00);
    outportb(FDC_DOR, 0x0C);
     
    fdc_wait(1);
     
    fdc_sendbyte( CMD_SPECIFY );
    fdc_sendbyte( 0xdf );
    fdc_sendbyte( 0x02 ); 
   
    __fdc_seek( 1 );   
    fdc_recalibrate();
     
    return 1;
}

void fdc_handler(struct regs *r){
     fdc_flag = 0;
}

int init_floppy(int device){
    int flag;
    int ret;
    int v;
   
    // Instal IRQ
    irq_install_handler(6, fdc_handler);
     
    // reset floppy driver         
    flag = reset_floppy(device);
    if(flag == 0){
        printf("\nError on init floppy driver in stage of [Reset].\n");
        return 0;
    }
     
     fdc_sendbyte( CMD_VERSION );
     v = fdc_getbyte();

     switch ( v ) {
         case 0xFF: printf(" [ controller not found ]");
         ret = 0;
         break;

           case 0x80: printf(" [ NEC controller ]");
         ret = 1;           
         break;

           case 0x81: printf(" [ VMware controller ]");
         ret = 1;           
         break;

         case 0x90: printf(" [ enhanced controller ]");
         ret = 1;         
         break;

           default: printf(" [ unknown controller [%d] ]", v);
         ret = 1;           
         break;
    }   

    return 1;
         
}