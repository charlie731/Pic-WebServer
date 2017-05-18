/*
   A wrapper to Microchip's SMTP (STACK_USE_CCS_EMAIL_ALERTS) library 
   for sending an e-mail based on alert flags.
   
   See ccs_email_alert.h for documentation
*/
///////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996,2014 Custom Computer Services           ////
//// This source code may only be used by licensed users of the CCS C  ////
//// compiler.  This source code may only be distributed to other      ////
//// licensed users of the CCS C compiler.  No other use, reproduction ////
//// or distribution is permitted without written permission.          ////
//// Derivative programs created using this software in object code    ////
//// form are not restricted in any way.                               ////
////                                                                   ////
//// http://www.ccsinfo.com                                            ////
///////////////////////////////////////////////////////////////////////////

#ifndef __CCS_EMAIL_ALERT_C__
#define __CCS_EMAIL_ALERT_C__

#ifndef EMAIL_ALERT_CUSTOM_BODY
#define EMAIL_ALERT_CUSTOM_BODY()   (TRUE)
#endif

#ifndef debug_email
#define debug_email(a,b,c,d,e,f,g,h,ui,j,k,l,m,n,o)
#endif

typedef unsigned int32 email_alert_bitmap_t;
#define EmailAlertSetBit(var, bit)         bit_set(var, bit)
#define EmailAlertClearAllBits(var)        var=0
#define EmailAlertIsBitSet(var, bit)       bit_test(var, bit)
#define EmailAlertIsAnyBitSet(var)         (var != 0)
#define EmailAlertCopy(dst, src)           dst=src

typedef enum
{
   EMAIL_ALERT_SM_IDLE = 0,
   EMAIL_ALERT_SM_FLAGS = 1,
   EMAIL_ALERT_SM_CUSTOM = 2,
   EMAIL_ALERT_SM_CLOSE = 3
} email_alert_sm_t;

struct
{
   email_alert_sm_t sm;
   email_alert_bitmap_t pending;
   email_alert_bitmap_t pending2;
   unsigned int16 sentAttempts;
   unsigned int16 sentSuccess; 
   WORD lastError;
} g_EmailAlert;

void EmailAlertInit(void)
{
   memset(&g_EmailAlert, 0x00, sizeof(g_EmailAlert));
}

void EmailAlertSetFlag(unsigned int8 flag)
{
   if (g_EmailAlert.sm)
   {
      debug_email(debug_putc, "EmailAlertSetFlag(%u) PENDING2\r\n", flag);
      
      EmailAlertSetBit(g_EmailAlert.pending2, flag);
   }
   else
   {
      debug_email(debug_putc, "EmailAlertSetFlag(%u) PENDING\r\n", flag);
      
      EmailAlertSetBit(g_EmailAlert.pending, flag);
   }
}

int1 EmailAlertIsBusy(void)
{
   return
   (
      EmailAlertIsAnyBitSet(g_EmailAlert.pending) ||
      EmailAlertIsAnyBitSet(g_EmailAlert.pending2) ||
      g_EmailAlert.sm
   );
}

unsigned int16 EmailAlertGetAttempts(void)
{
   return(g_EmailAlert.sentAttempts);
}

unsigned int16 EmailAlertGetSuccess(void)
{
   return(g_EmailAlert.sentSuccess);
}

WORD EmailAlertGetLastError(void)
{
   return(g_EmailAlert.lastError);
}

void EmailAlertTask(void)
{
   static unsigned int8 idx;
   ROM char *p;
   unsigned int16 len;
   int1 set;
   
   if ((g_EmailAlert.sm == EMAIL_ALERT_SM_IDLE) && EmailAlertIsAnyBitSet(g_EmailAlert.pending))
   {
      if (SMTPBeginUsage())
      {
         EmailAlertSetup();
         
         SMTPSendMail();
         
         debug_email(debug_putc, "EmailAlertTask() START\r\n");
         
         g_EmailAlert.sm++;
         idx = 0;
         g_EmailAlert.sentAttempts++;
      }
   }
   
   if (g_EmailAlert.sm && (g_EmailAlert.sm != EMAIL_ALERT_SM_CLOSE) && !SMTPIsBusy())
   {
      debug_email(debug_putc, "EmailAlertTask() ABORT %X\r\n", g_EmailAlert.sm);
      
      g_EmailAlert.sm = EMAIL_ALERT_SM_CLOSE;
   }
   
   switch(g_EmailAlert.sm)
   {
      default:
         g_EmailAlert.sm = 0;
         break;
         
      case EMAIL_ALERT_SM_IDLE:
         break;

      case EMAIL_ALERT_SM_FLAGS:
         set = EmailAlertIsBitSet(g_EmailAlert.pending, idx);
         if (set)
         {
            p = g_EmailAlertStrings[idx];
            len = strlenpgm(p);
            if (SMTPIsPutReady() >= (len+2))
            {
               debug_email(debug_putc, "EmailAlertTask() Sent %u of %lu, len=%lu p=%LX\r\n", idx, (sizeof(g_EmailAlertStrings)/sizeof(ROM char*)), len, p);
               SMTPPutROMString(p);
               SMTPPutROMString((ROM char*)"\r\n");
               SMTPFlush();
               set = FALSE;
            }
         }
         if (!set)
         {
            if (++idx >= (sizeof(g_EmailAlertStrings)/sizeof(ROM char*)))
            {
               g_EmailAlert.sm++;
            }         
         }
         break;

      case EMAIL_ALERT_SM_CUSTOM:
         if (EMAIL_ALERT_CUSTOM_BODY())
         {
            g_EmailAlert.sm++;
            
            debug_email(debug_putc, "EmailAlertTask() FINISHING\r\n");
            
            SMTPPutDone();
         }
         break;

      case EMAIL_ALERT_SM_CLOSE:
         if (!SMTPIsBusy())
         {
            g_EmailAlert.lastError = SMTPEndUsage();
            if (!g_EmailAlert.lastError)
            {
               g_EmailAlert.sentSuccess++;
            }
            EmailAlertCopy(g_EmailAlert.pending, g_EmailAlert.pending2);
            EmailAlertClearAllBits(g_EmailAlert.pending2);
            g_EmailAlert.sm = 0;
            debug_email(debug_putc, "EmailAlertTask() DONE EC=%LX\r\n", g_EmailAlert.lastError);
         }
         break;
   }
}

#endif
