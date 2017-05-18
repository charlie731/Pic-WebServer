#include <main.h>

/* TODO: Web server enabled. Server files are added to the /pages
directory. You can modify the contents of the dynamic display fields
at the end of the index.htm file. By default, headings are in the left
column and the dynamic display elements are in the right. The text on
the input buttons can be set by changing the 'value' strings in the
form section. Dynamic display elements can be added or removed from
index.htm and index.xml */
/* TODO: Server files must be built into an MPFS image prior to 
compiling and runing the web server. Running makempfsimg.bat in the
/mpfs directory will create an MPFS image of the files in /pages.
A new MPFS image has to be compiled and re-loaded if a file in
/pages needs to be changed. */

unsigned int8 http_format_char(char* file, char id, char *str, unsigned int8 max_ret)
{
  /**************************  FUNCIÓN FORMAT CHAR  *****************************/
/* Con  la función http_format_char  interconectamos las variables virtuales de 
la página web con las variables del programa del PIC. Se encarga de enviar los 
cambios producidos en la aplicación del PIC y reflejarlos en la aplicación web. 
Muestra,por tanto, las lectura obtenidas por el PIC y las representa en la 
aplicación de la página web               
                                         
%0 es la variable virtual para representar el valor de la lectura del canal 
analógico
%1 es la variable virtual para representar el valor de la lectura del bit 0 del 
puerto E.
*/
   char new_str[20];                                              
   int8 len=0;
   int8 AD0;
   
                                                  
    if (id == 0)
   {
         //TODO: Handle dyn0 field and save result to str
          set_adc_channel(0);                             
         delay_us(100);
         AD0=read_adc();
         sprintf(new_str,"0x%X",AD0);
         len=strlen(new_str);
         strncpy(str, new_str, max_ret);
   }           
}

void http_exec_cgi(char* file, char *key, char *val)         
{
/* Con la función http_exec_cgi interconectamos las variables virtuales de la 
página web con las variables del programa del PIC. Se encarga de recibir 
los cambios producidos en la aplicación web y reflejarlos en el hardware del PIC. 
Ejecuta, por tanto, la acción elegida según el valor de la variable virtual recibida 
de la página web

key es la variable virtual que viene de la pagina web
val es el valor de una variable virtual de la página web
file es la dirección de la página web devuelta por http_get_page ()
                                
*/                           
    if (strcmp(key, "button00") == 0)                               
   {
         //TODO: Handle button00
         output_toggle(PIN_D4);
   }                                                          
   if (strcmp(key, "button01") == 0)
   {                                              
         //TODO: Handle button01 
          output_toggle(PIN_D5);
   } 
   if (strcmp(key, "button02") == 0)                               
   {
         //TODO: Handle button00                   
         output_toggle(PIN_D6);                      
   }                                                         
   if (strcmp(key, "button03") == 0)
   {                                                        
         //TODO: Handle button01         
          output_toggle(PIN_D7);
   }

} 

#if HTTP_USE_AUTHENTICATION 
//This is a callback to the HTTP stack. 
//fileName is a file that has been requested over HTTP and is password 
//protected, user and pwd contains the authentication login the user had 
//attempted.  This function returns TRUE if the user/pwd is valid for this 
//fileName. 
int1 http_check_authentication(char *fileName, char *user, char *pwd) 
{ 
   static char goodUser[]="charlie"; 
   static char goodPwd[]="altec"; 
    
   return((stricmp(goodUser,user)==0) && (stricmp(goodPwd,pwd)==0)); 
} 
#endif 
                                             
void IPAddressInit(void)
{
   //MAC address of this unit
   MY_MAC_BYTE1=MY_DEFAULT_MAC_BYTE1;
   MY_MAC_BYTE2=MY_DEFAULT_MAC_BYTE2;
   MY_MAC_BYTE3=MY_DEFAULT_MAC_BYTE3;
   MY_MAC_BYTE4=MY_DEFAULT_MAC_BYTE4;
   MY_MAC_BYTE5=MY_DEFAULT_MAC_BYTE5;
   MY_MAC_BYTE6=MY_DEFAULT_MAC_BYTE6;

   //IP address of this unit
   MY_IP_BYTE1=MY_DEFAULT_IP_ADDR_BYTE1;
   MY_IP_BYTE2=MY_DEFAULT_IP_ADDR_BYTE2;
   MY_IP_BYTE3=MY_DEFAULT_IP_ADDR_BYTE3;
   MY_IP_BYTE4=MY_DEFAULT_IP_ADDR_BYTE4;

   //network gateway
   MY_GATE_BYTE1=MY_DEFAULT_GATE_BYTE1;
   MY_GATE_BYTE2=MY_DEFAULT_GATE_BYTE2;
   MY_GATE_BYTE3=MY_DEFAULT_GATE_BYTE3;
   MY_GATE_BYTE4=MY_DEFAULT_GATE_BYTE4;

   //subnet mask
   MY_MASK_BYTE1=MY_DEFAULT_MASK_BYTE1;
   MY_MASK_BYTE2=MY_DEFAULT_MASK_BYTE2;
   MY_MASK_BYTE3=MY_DEFAULT_MASK_BYTE3;
   MY_MASK_BYTE4=MY_DEFAULT_MASK_BYTE4;
}

void main()
{
   setup_adc_ports(AN0);
   setup_adc(ADC_CLOCK_INTERNAL|ADC_TAD_MUL_0);

   IPAddressInit();
   TickInit();
   enable_interrupts(GLOBAL);
   StackInit();


   while(TRUE)
   {

      // TCP/IP code
      StackTask();

      //StackPrintfChanges();

      StackApplications();



      //TODO: User Code
   }

}
