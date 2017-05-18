#include <18f4620+enc28j60.h>

//intenal regiter 
//byte porta = 0xF80 
//#byte portb = 0xF81 
//#byte portc = 0xF82 
#byte portd = 0xF83 
#byte porte = 0xF84 
//#byte trisa = 0xF92                                               
//#byte trisb = 0xF93                     
//#byte trisc = 0xF94                                                       
//#byte trisd = 0xF95 
//#byte trise = 0xF96 

/** Configuración para el uso del stack tcip **/
#define STACK_USE_ICMP        1  //Módulo de respuesta ICMP (ping)
#define STACK_USE_ARP         1  //Para solucionar direccionamiento MAC de las IP
#define STACK_USE_TCP         1  //Para enviar paquetes TCP 
#define STACK_USE_HTTP        1  //Uso de las funciones http del stack.
#define STACK_USE_CCS_PICENS  1  //CCS PICENS (Embedded Ethernet) 18F4620 + ENC28J60
#define STACK_USE_MCPENC      1  //Uso del enc28j60 por el stack (stacktsk.c)
#define STACK_USE_MAC         1  //Uso de la tarjeta de red

#define HTTP_SOCKET 80         //Nº de puerto asociado al socket.

/********** Definición del patillaje de conexión al enc28j60 ******************/
/* Existen varias posibilidades:

Opción 1. No definir nada, en cuyo caso se implementará una comunicación SPI por 
software y se tomarán la definición de patillas establecida en enc28j60.c

SO  PIN_D7 ---- ENC >>>> PIC
SI  PIN_D6 ---- PIC >>>> ENC
CLK PIN_D5
CS  PIN_D4
RST PIN_D3
INT PIN_D2
WOL PIN_D1

Opción 2. Definir todas las patillas de la comunicación SPI, en cuyo caso se 
implementará una comunicación SPI por software con la definición de patillas 
elegida. Por ejemplo...

#define PIN_ENC_MAC_SO  PIN_C4   //Entrada serie de datos
#define PIN_ENC_MAC_SI  PIN_C5   //Salida serie de datos                               
#define PIN_ENC_MAC_CLK PIN_B4   //Señal de reloj
#define PIN_ENC_MAC_CS  PIN_B5   //Chip select
#define PIN_ENC_MAC_RST PIN_B6   //Reset                                                                                  
#define PIN_ENC_MAC_INT PIN_B7   //Interrupción

Opción 3. El que aquí se ha utilizado, que consiste en habilitar el uso de SPI 
por hardware del PIC y definir las patillas ajenas al hardware del módulo SPI 
(CS, INT y RST). En este caso es imprescindible definir también la patilla SO 
para que el stack (dentro de enc28j60.c) no habilite SPI por software. Da igual 
que patilla SO se defina, la que se deberá cablear será la SO real del PIC    */

#define ENC_MAC_USE_SPI 1           //Uso del SPI por hardware
              
#define PIN_ENC_MAC_SO  PIN_B4      //Entrada serie de datos
//#define PIN_ENC_MAC_SI  PIN_C7    //Salida serie de datos (no necesario definir)
//#define PIN_ENC_MAC_CLK PIN_B1    //Señal de reloj  (no necesario definir)
#define PIN_ENC_MAC_CS  PIN_B6      //Chip select
#define PIN_ENC_MAC_RST PIN_B7      //Reset              
#define PIN_ENC_MAC_INT PIN_B5      //Interrupción
                 
/******************************************************************************/
                                                                          
//#define use_portd_lcd TRUE       //Uso del puerto d para control del lcd
//#include <LCD420PIC18F_RyP.c>    //Carga librería del lcd de 4x20 para familia 18F

#include "tcpip/stacktsk.c"      //Carga el stack TCP/IP de Microchip 

/*********************  PAGINA WEB A MOSTRAR **********************************/
/* Página principal INDEX (/) */                                                       
const char  HTML_INDEX_PAGE[]="
<!DOCTYPE html>
<html lang=\"es\">
<meta charset=\"UTF-8\">
<meta name=\"MobileOptimized\" content=\"width\" />
<meta name=\"HandheldFriendly\" content=\"true\" />
<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">
<title>Embedded Server Altec</title> 
<div style=\"text-align: center;\">
<div style=\"box-sizing: border-box; display: inline-block; width: auto; max-width: 480px; background-color: #FFFFFF; border: 2px solid #0361A8; border-radius: 5px; box-shadow: 0px 0px 8px #0361A8; margin: 50px auto auto;\">
<div style=\"background: #0361A8; border-radius: 5px 5px 0px 0px; padding: 15px;\"><span style=\"font-family: verdana,arial; color: #D4D4D4; font-size: 1.00em; font-weight:bold;\">Embed Server Altec</span></div>         
<div style=\"background: ; padding: 15px\" id=\"ap_style\">                                                                                                                                                                                      
<style type=\"text/css\" scoped>                          
#ap_style td { text-align:left; font-family: verdana,arial; color: #064073; font-size: 1.00em; }
#ap_style input { border: 1px solid #CCCCCC; border-radius: 5px; color: #666666; display: inline-block; font-size: 1.00em;  padding: 5px; }
#ap_style input[type=\"text\"], input[type=\"password\"] { width: 100%; }                                                                                                                       
#ap_style input[type=\"button\"],
#ap_style input[type=\"reset\"],
#ap_style input[type=\"submit\"] { height: auto; width: auto; cursor: pointer; box-shadow: 0px 0px 5px #0361A8; float: right; text-align:right; margin-top: 10px; margin-left:7px;}
#ap_style table.center { margin-left:auto; margin-right:auto; }
#ap_style .error { font-family: verdana,arial; color: #D41313; font-size: 1.00em; }
</style>
<p><img src=\"data:image/gif;base64,/9j/4AAQSkZJRgABAQEAYABgAAD/4QAuRXhpZgAATU0AKgAAAAgAAlEAAAQAAAABAAAAAFEBAAMA
AAABAAEAAAAAAAD/2wBDAAIBAQIBAQICAgICAgICAwUDAwMDAwYEBAMFBwYHBwcGBwcICQsJCAgK
CAcHCg0KCgsMDAwMBwkODw0MDgsMDAz/2wBDAQICAgMDAwYDAwYMCAcIDAwMDAwMDAwMDAwMDAwM
DAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAz/wAARCAB5ARsDASIAAhEBAxEB/8QA
HwAAAQUBAQEBAQEAAAAAAAAAAAECAwQFBgcICQoL/8QAtRAAAgEDAwIEAwUFBAQAAAF9AQIDAAQR
BRIhMUEGE1FhByJxFDKBkaEII0KxwRVS0fAkM2JyggkKFhcYGRolJicoKSo0NTY3ODk6Q0RFRkdI
SUpTVFVWV1hZWmNkZWZnaGlqc3R1dnd4eXqDhIWGh4iJipKTlJWWl5iZmqKjpKWmp6ipqrKztLW2
t7i5usLDxMXGx8jJytLT1NXW19jZ2uHi4+Tl5ufo6erx8vP09fb3+Pn6/8QAHwEAAwEBAQEBAQEB
AQAAAAAAAAECAwQFBgcICQoL/8QAtREAAgECBAQDBAcFBAQAAQJ3AAECAxEEBSExBhJBUQdhcRMi
MoEIFEKRobHBCSMzUvAVYnLRChYkNOEl8RcYGRomJygpKjU2Nzg5OkNERUZHSElKU1RVVldYWVpj
ZGVmZ2hpanN0dXZ3eHl6goOEhYaHiImKkpOUlZaXmJmaoqOkpaanqKmqsrO0tba3uLm6wsPExcbH
yMnK0tPU1dbX2Nna4uPk5ebn6Onq8vP09fb3+Pn6/9oADAMBAAIRAxEAPwD9/KKKKACiiigAoooo
AKKA2aN1ABRRmjNABRRmgHNABRQTRmgAooooAKKTdS55oAKKKKACignFGaACik3ilBzQAUUZozQA
UUUUAFFFFABRRRQAUUUUAFFFFABRRRQAUUUUAFFBOKaJAaAAkUL1rj/jn8bvDf7Ofwr1rxn4u1KH
S/D+hwGe5nc/Meyoi9Xd2KqqjlmYAV8+f8EpP+Cik3/BQ/wj8RNYvLGDSH0DxIbfT7CMlpLfTZII
zbmVs/NKzpOWIwM8DgVtHD1HSdZL3U7XMXWgqnsnuz61ByKQsBQOK+Gf+CzP/BULxX/wTri8A2/g
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                    
bd4D8FsPDqldJB0W2xpYLF8QfJ+6G47vkxzz1rtqKrml3J9nHscvc/BXwhd6zrGoy+FfDcmoeIbf
7Jqty+mwmbUoeB5U77d0qYAG1yRwOKl8O/CPwt4R07SLPSfDPh7S7Tw+7yaZBaafFDHprOrI5hVV
AjLKzKSuMhiD1NdHRRzS7hyx7HO2Hwn8M6XBbw23h3QbeGztpbK3SKwiVYLeUhpYUAUBY3IBZRwx
AJyaq6N8C/BnhzTobPT/AAj4XsbW3MDRQ2+lwRRxmCR5YSqqgAMckjumB8rOxGCTXWUUc0u4+Vdj
Bh+GPh221FLyPQdFjvI7yTUknWyjWRbqRPLknDbciVk+UvncV4JIrJP7Ofw/bTNQsj4F8HGz1a7W
/vrf+xrfy7y4UllmkXZh5ASSHYFgSea7Sijml3Fyx7HK6t8D/BviC102DUPCfhm+h0e6a90+OfS4
JEsbhn3vLEGU+XIzksWXBLHOc0zxF8B/BPi/QP7K1bwf4V1TTPtrakLO70mCe3F0zFmn2MhXzSzM
S+NxLE5ya62ijml3DlXY5/VPhZ4b1tb4Xnh/Q7z+04Ira8E1jFJ9siiJaKOTKncqMSVByFJJAzVT
UvgZ4M1jxo3iS88I+GbvxE0ZhbVJtLge9KFDHsMxQvtKErjONpI6cV1dFHPLuHJHscfF8IrPwR8M
tQ8P/D+20HwHLcRyGzlsNHi+y2c7jHnG3XYjkcHBIzjk1mfs0/s6aX+zN8LIfDem3NxqVxPdT6nq
up3QUXOsahcOZLi7l2jG+R2JwOFUKo4UV6HRR7SVrXD2cb3OP0T9n/wL4cttag0/wX4SsIPEalNV
jttIt4k1RTnInCoBLnc3389T6mrOn/BfwjpGv6nq1p4V8N2uqa1B9m1C8h02FJ76LAXZK4XdIuAB
hiRgCunoo5pdw5I9jh739mf4d6lZ6Lb3HgLwXcW/hssdIil0S2dNK3Nub7OpTEWTydmMnnrW3D8M
/D1tcxzRaDosc0d9JqaOtlGGS7kBWS4B2/611Zgz/eYMQTzW7RRzS7hyx7HJ3fwL8G31tpsM3hPw
xNDorzSafHJpcDLYtNkytENmIy5J3FcbsnOc1JoPwV8H+FfF03iDS/CvhvTteubdLSbUrXTIYbyW           
FFVEiaVVDlFVEUKTgBQBjArqKKOZ2tcFTitUjmYvg54Uh8fyeLE8M+Hk8UTJ5cmsLp0I1B12hNpn
2+YRtAXGcYAGMVnap+zZ8PdcisI73wH4Lu49JiFvYpNottItnFuLbIwU+RdzMdq4GST3NdvRRzS7                             
hyR7Gfo3hfT/AA694+n6fY2MmpXDXd41vAsRupiFUySbQN7lVUFjyQoHQVoAYooqSgooooAKKKKA                                                                           
CiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAK                                     
KKKACiiigAooooAKKKKACiiigAooooAKKKKAP//Z\"</p>
<form method=\"post\" action=\"/login\">     
<input type=\"hidden\" name=\"action\" value=\"login\">                        
<input type=\"hidden\" name=\"hide\" value=\"\">                                              
<table class='center'>                                                                                                                                                                   
<tr><td>Usuario:</td><td><input type=\"text\" name=\"user\"></td></tr>
<tr><td>Clave:</td><td><input type=\"password\" name=\"pass\"></td></tr> 
<tr><td>&nbsp;</td><td><input type=\"submit\" value=\"Confirmar\"></td></tr>                                                                                                            
<tr><td colspan=2>&nbsp;</td></tr>                                                                    
</table>                                                                                                       
</form> 
</div></div></div>                                                                             
";                                                                         
  
const char  HTML_LECTURAS_LOGIN[]=" 
<!DOCTYPE html>
<html lang=\"es\">
<meta charset=\"UTF-8\">
<meta name=\"MobileOptimized\" content=\"width\" />
<meta name=\"HandheldFriendly\" content=\"true\" />
<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">
<HTML><BODY BGCOLOR=#FFFFFF TEXT=#000000>                                    
<P>                                                                                                               
<P><A HREF=\"/lecturas\"><BR><center><H2>LOGIN</H2></CENTER></A>                                                                      
</BODY></HTML>
";                                
                                            
/* Página secundaria (\lecturas). Accesible desde la página principal */                                                                                                       
const char  HTML_LECTURAS_PAGE[]="
<!DOCTYPE html>
<html lang=\"es\">
<meta charset=\"UTF-8\">
<meta name=\"MobileOptimized\" content=\"width\" />
<meta name=\"HandheldFriendly\" content=\"true\" />
<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">
   <title>Altec S.E.</title>                                                                                                             
<body BGCOLOR=#71C7CE STYLE=\"background-color: #71C7CE;\"></body>
<meta name=\"GENERATOR\" content=\"Altec S.E.(http://www.altec.com.ar)\">
<meta http-equiv=\"refresh\"content=\"30;url=//www.google.com.ar\">
<title>Power distribution unit</title>                              
<div style=\"text-align: center;\">
<div style=\"box-sizing: border-box; display: inline-block; width: auto; max-width: auto; background-color: #FFFFFF; border: 2px solid #0361A8; border-radius: 5px; box-shadow: 0px 0px 8px #0361A8; margin: auto auto auto;\">
<div style=\"background: #0361A8; border-radius: 5px 5px 0px 0px; padding: 15px;\"><span style=\"font-family: verdana,arial; color: #D4D4D4; font-size: 1.00em; font-weight:bold;\">Remote Embed Server Altec</span></div>         
<div style=\"background: ; padding: 15px\" id=\"ap_style\">                                                                                                                                                                                      
<style type=\"text/css\" scoped>                                                                                                                                    
#ap_style td { text-align:left; font-family: verdana,arial; color: #064073; font-size: 1.00em; }
#ap_style input { border: 1px solid #CCCCCC; border-radius: 5px; color: #666666; display: inline-block; font-size: 1.00em;  padding: 5px; }
#ap_style table.center { margin-left:auto; margin-right:auto; }
#ap_style .error { font-family: verdana,arial; color: #D41313; font-size: 1.00em; }
</style>
</head> 

<body>                                                                                                  

<h1 align=\"center\"><font color=\"#000000\"><font color=\"#000000\">(PDU) Power distribution unit </font></font></h1>
                                                    
                  <table align=\"center\" bgcolor=\"#00FF00\" border=\"2\" cellpadding=\"6\" cellspacing=\"1\" width=auto>
                  
                  <tbody>                                        
                           
                  <tr>                                         
                  <th><p align=\"center\">Lecturas Pic ( 1 = ON ) ( 0 = OFF)</p></th>                  
                  <th><p align=\"center\">ON/OFF Relay Correspondiente (PRECAUCION)</p></th>
                  <th><p align=\"center\">REBOOT Relay </p></th>
                  
                  </tr>
                                                          
                  <tr>                                                                                     
                  <td><b>Corriente: </b>%0</td>
                  <FORM METHOD=POST><td>Pulse  ON/OFF = Interruptor Fuente 1 <input type=\"submit\" name=\"boton1\" value=\"Relay 1\"></td></FORM>
                  <FORM METHOD=POST><td>Reboot = Fuente 1 <input type=\"submit\" name=\"reboot1\" value=\"Reboot Relay 1\"></td></FORM>
                  </tr>                                                          
                                                                                                                                                                     
                  <tr>
                  <td><b>Estado: </b>%1</td>                                                                                      
                  <FORM METHOD=POST><td>Pulse  ON/OFF = Interruptor Fuente 2 <input type=\"submit\" name=\"boton2\" value=\"Relay 2\"></td></FORM>
                  <FORM METHOD=POST><td>Reboot = Fuente 2 <input type=\"submit\" name=\"reboot2\" value=\"Reboot Relay 2\"></td></FORM>
                  </tr>
                                                       
                  <tr>
                  <td><b>Rele 1=</b>%2</td>                                                                                     
                  <FORM METHOD=POST><td>Pulse  ON/OFF = Interruptor Fuente 3 <input type=\"submit\" name=\"boton3\" value=\"Relay 3\"></td></FORM>
                  <FORM METHOD=POST><td>Reboot = Fuente 3 <input type=\"submit\" name=\"reboot3\" value=\"Reboot Relay 3\"></td></FORM>
                  </tr>

                  <tr>
                  <td><b>Rele 2=</b>%3</td>
                  <FORM METHOD=POST><td>Pulse  ON/OFF = Interruptor Fuente 4 <input type=\"submit\" name=\"boton4\" value=\"Relay 4\"></td></FORM>
                  <FORM METHOD=POST><td>Reboot = Fuente 4 <input type=\"submit\" name=\"reboot4\" value=\"Reboot Relay 4\"></td></FORM>
                  </tr>
                                                                                                       
                  <tr>
                  <td><b>Relay 3=</b>%4</td>
                  </tr>                                 
                                                                    
                  <tr>
                  <td><b>Relay 4=</b>%5</td>                                                                                                                                                                          
                  </tr>           
                </table>
     </table>                                                                         
   </tbody>
   
<p align=\"center\"><font size=\"2\"><font size=\"2\"><font size=\"2\"><font size=\"2\"><font size=\"2\">Laboratorio de I+D. Power distribution unit </font><a href=\"http://www.altec.com.ar\" target=\"_blank\"><font size=\"4\"> Altec S.E.</font></a><font size=\"4\">&nbsp; </font></font></font></font></font></p>
<FORM>
   <INPUT TYPE=\"submit\" onClick=\"history.go(0)\" VALUE=\"Refresh\">
</FORM>
<P><A HREF=\"http://www.google.com.ar\">SALIR</A>
</body>
";                                                    

/* Elección de MAC. No puede haber 2 dispositivos con misma MAC en una misma red
   Microchip Vendor ID  MAC: 00.04.A3.xx.xx.xx.  */
void MACAddrInit(void) {
   MY_MAC_BYTE1=0;
   MY_MAC_BYTE2=0x08;
   MY_MAC_BYTE3=0xdc;
   MY_MAC_BYTE4=0x18;
   MY_MAC_BYTE5=0x7c;
   MY_MAC_BYTE6=0x06;
}

void IPAddrInit(void) {
   //Elección de la dirección IP. 
   MY_IP_BYTE1=10;
   MY_IP_BYTE2=2;
   MY_IP_BYTE3=10;
   MY_IP_BYTE4=200;

   //Elección de la dirección de puerta de enlace. 
   MY_GATE_BYTE1=10;
   MY_GATE_BYTE2=2;
   MY_GATE_BYTE3=10;
   MY_GATE_BYTE4=1;

   //Elección de la máscara de red.Si no se indica nada se tomará 255.255.255.0
   MY_MASK_BYTE1=255;
   MY_MASK_BYTE2=255;
   MY_MASK_BYTE3=255;         
   MY_MASK_BYTE4=0;                                   
}                                      
                                             
 ///// Variables Globales ///////////                                            
 static char valid_user[8];
 static char valid_pass[8];    
 static char memouser[]="charlie";                                                              
 static char memopass[]="1234";
 
 int1 elige_reboot1=0;
 int1 elige_reboot2=0;
 int1 elige_reboot3=0;
 int1 elige_reboot4=0;
 int32 reboot1=0;
 int32 reboot2=0;
 int32 reboot3=0;
 int32 reboot4=0;
 ///////////////////////////////////
 /***************************  FUNCIÓN EXEC CGI   ******************************/
/* Con la función http_exec_cgi interconectamos las variables virtuales de la                
página web con las variables del programa del PIC. Se encarga de recibir 
los cambios producidos en la aplicación web y reflejarlos en el hardware del PIC. 
Ejecuta, por tanto, la acción elegida según el valor de la variable virtual recibida 
de la página web

key es la variable virtual que viene de la pagina web
val es el valor de una variable virtual de la página web                                                      
file es la dirección de la página web devuelta por http_get_page ()

*/

void http_exec_cgi(int32 file, char *key, char *val) {
   static char boton1_key[]="boton1";                                    
   static char boton2_key[]="boton2";
   static char boton3_key[]="boton3";
   static char boton4_key[]="boton4";
   static char reboot1_key[]="reboot1";                                    
   static char reboot2_key[]="reboot2";
   static char reboot3_key[]="reboot3";
   static char reboot4_key[]="reboot4";
   static char lcd_user[]="user";                                                              
   static char lcd_pass[]="pass";                                   

   //printf(lcd_putc,"\fCGI FILE=%LD", file);
   //printf(lcd_putc,"\nKEY=%S", key);
   //printf(lcd_putc,"\nVAL=%S", val);

   /* Se ejecutará al pulsar el botón "Botón 1" en la aplicación web */
   if (stricmp(key,boton1_key)==0)                                  
   {
      output_toggle(PIN_D0);                                                                    
   }
   /* Se ejecutará al pulsar el botón "reboot 1" en la aplicación web */
   if (stricmp(key,reboot1_key)==0)                                  
   {
      output_low(PIN_D0);
      elige_reboot1=1;
   }
   /* Se ejecutará al pulsar el botón "Botón 2" en la aplicación web */                            
   if (stricmp(key,boton2_key)==0)
   {
      output_toggle(PIN_D1);
   }
   /* Se ejecutará al pulsar el botón "reboot 2" en la aplicación web */
   if (stricmp(key,reboot2_key)==0)                                  
   {
      output_low(PIN_D1);
      elige_reboot2=1;
   }
   /* Se ejecutará al pulsar el botón "Botón 3" en la aplicación web */
   if (stricmp(key,boton3_key)==0)
   {
      output_toggle(PIN_D2);                     
   } 
   /* Se ejecutará al pulsar el botón "reboot 3" en la aplicación web */
   if (stricmp(key,reboot3_key)==0)                                  
   {
      output_low(PIN_D2);
      elige_reboot3=1;
   }
   /* Se ejecutará al pulsar el botón "Botón 4" en la aplicación web */                                
    if (stricmp(key,boton4_key)==0)
    {                                                                                         
      output_toggle(PIN_D3);                    
   } 
   /* Se ejecutará al pulsar el botón "reboot 4" en la aplicación web */ 
   if (stricmp(key,reboot4_key)==0)                                  
   {
      output_low(PIN_D3);
      elige_reboot4=1;
   }
   /* Se ejecutará al pulsar el botón "Enviar texto" en la aplicación web */  
   if (stricmp(key,lcd_user)==0)                                                          
   {
      //printf("\r\n%s",val);  //Muestra en el lcd el texto recibido 
      strcpy(valid_user,val);                                                                         
   } 
   
   /* Se ejecutará al pulsar el botón "Enviar texto" en la aplicación web */                
   if (stricmp(key,lcd_pass)==0)
   {                    
      //printf("\r\n%s",val);  //Muestra en el lcd el texto recibido                                      
      strcpy(valid_pass,val);                                                         
   }                                                                                
}       
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
int8 http_format_char(int32 file, char id, char *str, int8 max_ret) {           
   char new_str[20];
   int8 len=0;                                            
   int8 AD0;
   int8 RE0;
   int8 RD0;
   int8 RD1;
   int8 RD2;
   int8 RD3;
                     
   *str=0;

   switch(id) {
       case '0':
       set_adc_channel(0);
         delay_us(100);
         AD0=read_adc();
         sprintf(new_str,"0x%X",AD0);                       
         len=strlen(new_str);
      break;
       case '1':
         RE0=bit_test(porte,0);
         sprintf(new_str,"%d",RE0);                    
         len=strlen(new_str);
      break;
       case '2':                      
       RD0=bit_test(portd,0);
         //strcpy(user1,valid_user);   
         sprintf(new_str,"%d",RD0);
         len=strlen(new_str);
      break;
       case '3':                                                             
       RD1=bit_test(portd,1);
         //strcpy(pass1,valid_pass);                                   
         sprintf(new_str,"%d",RD1);
         len=strlen(new_str);
      break;  
       case '4':
       RD2=bit_test(portd,2);
         //strcpy(user1,valid_user);   
         sprintf(new_str,"%d",RD2);
         len=strlen(new_str);
      break;        
       case '5':                            
       RD3=bit_test(portd,3);
         //strcpy(user1,valid_user);                                                                                                     
         sprintf(new_str,"%d",RD3);
         len=strlen(new_str);
      break;
      default:
      len=0;
   }                          

   if (len)strncpy(str, new_str, max_ret);
   else  *str=0;
   
   return(len);
}

                                     
/***************************  FUNCIÓN GET PAGE   ******************************/
/* Esta función devuelve la posición de memoria donde se encuentra la página web                          
a mostrar. En este caso se trata de una web con 2 páginas. Una principal index(/) 
y una secundaria(/lecturas)                                                   */
                                                         
int32 http_get_page(char *file_str) 
{                                                                                                                         
   int32 file_loc=0;
   static char index[]="/";                                                                                 
   static char login[]="/login";
   static char lecturas[]="/lecturas";                                                          
                                           
   //printf("\r\nRequest %s ",file_str);      //Muestra en lcd solicitud

   /* Busca la posición de memoria donde se encuentra la página solicitada */
   if (stricmp(file_str,index)==0)                 //Si es la principal...  
   {
      file_loc=label_address(HTML_INDEX_PAGE);     //...toma su posición en la memoria        
   }                                                                                                                                              
   else if (stricmp(file_str,login)==0)         //O si es la secundaria...                                  
   { 
          file_loc=label_address(HTML_LECTURAS_LOGIN);    //...toma su posición en la memoria                                                                                                            
   }    
   else if (stricmp(file_str,lecturas)==0)         //O si es la secundaria...
   {                                                                                                


       if ((stricmp(valid_user,memouser)==0) && (stricmp(valid_pass,memopass)==0))
    {
     file_loc=label_address(HTML_LECTURAS_PAGE);    //...toma su posición en la memoria
     //printf("\r\n(Entro Aqui Por clave y usuario)");       //...muestra en lcd mensaje 
    }    
                                                                                                             
   }                                                                                
   /* Muestra en lcd la página solicitada */
   /*  
   if (file_loc)
   {                                  //Si existe...
      printf("\r\n(FILE=%LU)",file_loc);    //...muestra en lcd pos. de memoria
   }
   else
   {                                           //Si no existe...
      printf("\r\n(File Not Found)");       //...muestra en lcd mensaje
   }
   */
   /* Devuelve la posición en la memoria donde se encuentra la página a mostrar */
                                                                                       
    return(file_loc);                                                                                                                                                                    
}  
                                                                                                            
                    
                                                                               
/************************** FUNCIÓN PRINCIPAL *********************************/
void main(void) {

   /* Habilitación y configuración del canal analógico 0 */
   setup_adc(ADC_CLOCK_INTERNAL);                                           
   setup_adc_ports(AN0);
   set_adc_channel(0);
   delay_ms(1);
                                           
   /*Reset de las salidas */
   output_high(PIN_D0);
   output_high(PIN_D1);
   output_high(PIN_D2);
   output_high(PIN_D3);            
    
   /* Inicialización del lcd */
   //lcd_init();
   //printf("\r\nAltec WEB SERVER");   //Mensaje de inicio en lcd 
   delay_ms(1000);
  
   /* Inicialización del Stack */
   MACAddrInit(); //Se asigna la dirección MAC elegida 
   IPAddrInit();  //Se asigna IP, mascara de red y puerta de enlace elegidos
   StackInit();   //Inicializa el stack                                                                  
   
   /* Muestra la IP elegida en lcd */
   //printf("\r\n IP: %u.%u.%u.%u", MY_IP_BYTE1, MY_IP_BYTE2, MY_IP_BYTE3, MY_IP_BYTE4);
   //printf("\r\n Puerto: %u", HTTP_SOCKET); 
   delay_ms(10);                                      
   
                                         
      //fprintf(terminal,"Pass: %s", valid_pass);
   
   while(TRUE)
   {                                                                       
       ///////////////////////////////////////////////////
       StackTask();
       ///////////////////////////////////////////////////
       
       
       //////////////////// REBOOT0 //////////////////////
       if (elige_reboot1)
       {
         reboot1++;
       }
       
       if(reboot1>=100000)
       {
         output_high(PIN_D0);
         elige_reboot1=0;
         reboot1=0;
       }
       //////////////////// FIN REBOOT0 //////////////////////
       
       
       //////////////////// REBOOT1 //////////////////////
        if (elige_reboot2)
       {
         reboot2++;
       }
       
       if(reboot2>=100000)
       {
         output_high(PIN_D1);
         elige_reboot2=0;
         reboot2=0;
       }
       //////////////////// FIN REBOOT1 //////////////////////
       
       
       //////////////////// REBOOT2 //////////////////////
        if (elige_reboot3)
       {
         reboot3++;
       }
       
       if(reboot3>=100000)
       {
         output_high(PIN_D2);
         elige_reboot3=0;
         reboot3=0;
       }
       //////////////////// FIN REBOOT2 //////////////////////
       
       
       //////////////////// REBOOT3 //////////////////////
        if (elige_reboot4)
       {
         reboot4++;
       }
       
       if(reboot4>=100000)
       {
         output_high(PIN_D3);
         elige_reboot4=0;
         reboot4=0;
       }
       //////////////////// FIN REBOOT3 //////////////////////
   }    
}                               
                           
