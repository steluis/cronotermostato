/*
 * Cronotermostato con display Nextion NX4024T032 (3,2" 240x400)
 * Stefano Luise 02.03.2020
 */

#include "Nextion.h"
#include <EEPROM.h>
#include <DS3231.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <SoftwareSerial.h>
//SoftwareSerial Messaggi(10,11);  //TX,RX

#define MANUALE 0
#define AUTOMATICO 1
#define VA 2   //Uscita per comando elettrovalvola

DS3231 Clock;
Adafruit_BME280 bme; // Libreria BME280. Legge temperatura, umidità, pressione atmosferica

//float temp=13.0;  // Temperatura da raggiungere. Può essere impostata manualmente o programmata
unsigned char temp=130;  // Temperatura da raggiungere. Può essere impostata manualmente o programmata
char stato=MANUALE;
unsigned char giorno_prog=1; //Giorno della settimana nella pagina di programmazione
unsigned char orario_prog=0; //Ora 00-23 nella pagina di programmazione
unsigned char temp_prog=0;   //Temperatura programmata per il dato giorno e la data ora
unsigned char prec_hour=99;
bool variata_temp=0;         //Variabile che dice se la temperatura programmata di un dato giorno e data ora è stata modificata
bool aggiorna_pressione=0;   
int press_mis[6];            //Memorizza le pressioni misurate ogni 60 minuti

// Declare your Nextion objects - Example (page id = 0, component id = 1, component name = "b0") 
NexText t2_11 = NexText(2, 21, "t11");  //Testo t11 nella pagina 2. Impostato come non visibile serve per memorizzare data, ora, giorno, mese, anno
NexButton b0 = NexButton(0, 2, "b0");   //Pulsante b0, per passare da Automatico a Manuale
NexButton b2 = NexButton(0, 13, "b2");  //Pulsante b2, incrementa la temperatura se in manuale
NexButton b3 = NexButton(0, 14, "b3");  //Pulsante b3, secrementa la temperatura se in manuale
NexButton b4 = NexButton(0, 18, "b4");  //Pulsante b4, fittizio che in pagina 0 è settato non visibile e pure rimpicciolito in alto a sx. Ne viene simulata la pressione ogni 5 secondi
                                        //dal timer tm0. ENtra nella funzione apposita definita più sotto per fare gli aggiornamenti di pagina 0
NexButton b11 = NexButton(1, 4, "b1");  //Pulsante incrementa giorno pagina 1
NexButton b12 = NexButton(1, 5, "b2");  //Pulsante decrementa giorno pagina 1
NexButton b13 = NexButton(1, 7, "b3");  //Pulsante incrementa ora programmazione in pagina 1
NexButton b14 = NexButton(1, 8, "b4");  //Pulsante decrementa ora programmazione in pagina 1
NexButton b15 = NexButton(1, 10, "b5"); // Pulsante temperatura su pagina 1: programmazione
NexButton b16 = NexButton(1, 11, "b6"); // Pulsante temperatura giu pagina 1: programmazione
NexButton b17 = NexButton(1, 12, "b7"); // Pulsante Copia Precedente in pagina 1

NexButton b21 = NexButton(2, 28, "b11"); // Pulsante IMPOSTA pagina 2. CHiama la funzione che setta data a ora nel DS3211

// Declare pages:
// Sending data to the display to nonexistent objects on the current page creates an error code sent by the display.
// Any error sent by the display creates lag on the arduino loop because arduino tries to read it, thinking it's a touch event.
// So to avoid this, I am only going to send data depending on the page the display is on.
// That's the reason I want the arduino to know which page is loaded on the display.
// To let arduino know what page is currently loaded, we are creating a touch event for each page.
// On the nextion project, each page most send a simulated "Touch Press Event" in the "Preinitialize Event" section so
// we can register that a new page was loaded.
NexPage page0 = NexPage(0, 0, "page0");  // Page added as a touch event
NexPage page1 = NexPage(1, 0, "page1");  // Page added as a touch event

// Register a button object to the touch event list.  Registrare qui tutti gli oggetti che triggerano un evento
NexTouch *nex_listen_list[] = {
  &b3,
  &b2,
  &b0,
  &b4,
  &b11,
  &b12,
  &b13,
  &b14,
  &b15,
  &b16,
  &b17,
  &b21,
  &page0,  // Page added as a touch event
  &page1,  // Page added as a touch event
  NULL
};


/* Funzione chiamata alla pressione del b1 pagina 2 */
/*                IMPOSTA                           */
void b21PopCallback(void *ptr) {
char testo[17]={0};
char giorno[3];
char mese[3];
char anno[3];
char ore[3];
char minuti[3];

int dow,y;
static int t[] = { 0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4 };
t2_11.getText(testo,17);  // Preleva la stringa t11 della pagina 2, composta da: giorno/mese/anno/ore/minuti. 
                          //Tale strimga viene composta dal Nextion nella porzione di codice di pagina 2 nel timer tm0
char* token = strtok(testo, "/"); 

// Tramite strtok estrae le porzioni di stringa delimitate da / e le assegna alle rispettive stringhe
strcpy(giorno,token);
token = strtok(NULL, "/"); 
strcpy(mese,token);
token = strtok(NULL, "/"); 
strcpy(anno,token);
token = strtok(NULL, "/"); 
strcpy(ore,token);
token = strtok(NULL, "/"); 
strcpy(minuti,token);

Clock.setMinute(atoi(minuti));
Clock.setHour(atoi(ore));
Clock.setDate(atoi(giorno));
Clock.setMonth(atoi(mese));
Clock.setYear(atoi(anno));
y=atoi(anno);
y -= atoi(mese) < 3;
dow=(y+y/4-y/100+y/400+t[atoi(mese)-1]+atoi(giorno))%7; // Formula per calcolare il giorno della settimana a partire da anno, mese, giorno. NB: Domenica=0
if (dow==0) dow=7; //La formula sopra mi dà 0 per domenica mentre io ho considerto i giorni della settimana che vanno da 1(lunedì) a 7(domenica). Da qui questa riga.
Clock.setDoW(dow);
}
/** FINE IMPOSTA **/


/* Funzione chiamata alla pressione del tasto b7 pagina 1 */
/*                COPIA PRECEDENTE                        */
void b17PopCallback(void *ptr) {
unsigned char da,i,k;
unsigned char da_precedente;
da=24*(giorno_prog-1);
if (giorno_prog==1) 
 da_precedente=144;
else
 da_precedente=24*(giorno_prog-2);
k=0;
for(i=da;i<=da+23;i++)
{
  EEPROM.write(i,EEPROM.read(da_precedente+k));
  k++;
}

 Serial.print(F("page 1"));
 Serial.write(0xff);
 Serial.write(0xff);
 Serial.write(0xff);
}
/** FINE COPIA PRECEDENTE **/


/* Funzione chiamata alla pressione del tasto b1 pagina 1 */
/*         INCREMENTA GIORNO PROGRAMMAZIONE               */
void b11PopCallback(void *ptr) {
 giorno_prog++;
 if (giorno_prog>7) giorno_prog=1;
 Serial.print(F("page 1"));
 Serial.write(0xff);
 Serial.write(0xff);
 Serial.write(0xff);
}
/** FINE INCREMENTA GIORNO PROGRAMMAZIONE **/

 /* Funzione chiamata alla pressione del tasto b2 pagina 1 */
/*         DECREMENTA GIORNO PROGRAMMAZIONE               */
void b12PopCallback(void *ptr) {
 giorno_prog--;
 if (giorno_prog<1) giorno_prog=7;
 Serial.print(F("page 1"));
 Serial.write(0xff);
 Serial.write(0xff);
 Serial.write(0xff);
}
/** FINE DECREMENTA GIORNO PROGRAMMAZIONE **/

/* Funzione chiamata alla pressione del tasto b3 pagina 1 */
/*  INCREMENTA ORARIO GIORNALIERO PROGRAMMAZIONE          */
void b13PopCallback(void *ptr) {
char buf[4]={0};
char stampa[13]={0};
char temperatura_letta[5]={0};
unsigned char n;
float gradi_show;

char cerchio[21]={0};
unsigned char y,k,w;
signed int x;

if (variata_temp==1)
{
 n=24*(giorno_prog-1)+orario_prog;
 EEPROM.write(n,temp_prog);  
}
variata_temp=0;

orario_prog+=1;
if (orario_prog>23) orario_prog=0;
itoa(orario_prog,buf,10);

strcat(stampa,"t2.txt=\"");

if (orario_prog<=9)
 strcat (stampa,"0");
strcat (stampa,buf);
strcat (stampa,":00");

strcat(stampa,"\"");

 Serial.print(stampa);
 Serial.write(0xff);
 Serial.write(0xff);
 Serial.write(0xff);
 memset(stampa,0,sizeof(stampa));

// Colora la barra di blu
 n=24*(giorno_prog-1)+orario_prog;
 temp_prog=EEPROM.read(n);

 gradi_show=(EEPROM.read(n))/10.0;
 dtostrf(gradi_show,4,1,temperatura_letta); 
 char* p  = strchr(temperatura_letta, '.');
 *p = ',';
 strcat(stampa,"t3.txt=\"");
 strcat(stampa,temperatura_letta);
 strcat(stampa,"\"");
 Serial.print(stampa);
 Serial.write(0xff);
 Serial.write(0xff);
 Serial.write(0xff);
 memset(stampa,0,sizeof(stampa));
 
 
 x=(orario_prog+1)*15-4;
 itoa(x,buf,10);
 strcat (cerchio,"fill ");
 strcat (cerchio,buf);
 strcat (cerchio,",");
 x=220-(2*(EEPROM.read(n)-130))/3;
 itoa(x,buf,10);
 strcat (cerchio,buf);
 strcat (cerchio,",8,");
 x=1+(2*(EEPROM.read(n)-130))/3;
 itoa(x,buf,10);
 strcat (cerchio,buf);
 strcat(cerchio,",31");
 Serial.print(cerchio);
 Serial.write(0xff);
 Serial.write(0xff);
 Serial.write(0xff);
 memset(cerchio,0,sizeof(cerchio));
// Colora la precedente di arancio
if (orario_prog>0)
{
 x=(orario_prog)*15-4;
 itoa(x,buf,10);
 strcat (cerchio,"fill ");
 strcat (cerchio,buf);
 strcat (cerchio,",");
 x=220-(2*(EEPROM.read(n-1)-130))/3;
 itoa(x,buf,10);
 strcat (cerchio,buf);
 strcat (cerchio,",8,");
 x=1+(2*(EEPROM.read(n-1)-130))/3;
 itoa(x,buf,10);
 strcat (cerchio,buf);
 strcat(cerchio,",64193");
 Serial.print(cerchio);
 Serial.write(0xff);
 Serial.write(0xff);
 Serial.write(0xff);
 memset(cerchio,0,sizeof(cerchio));
}
}
/** FINE INCREMENTA ORARIO GIORNALIERO PROGRAMMAZIONE **/

/* Funzione chiamata alla pressione del tasto b4 pagina 1 */
/*  DECREMENTA ORARIO GIORNALIERO PROGRAMMAZIONE          */
void b14PopCallback(void *ptr) {
char buf[4]={0};
char stampa[13]={0};
char temperatura_letta[5]={0};
unsigned char n;
float gradi_show;

char cerchio[21]={0};
unsigned char y,k,w;
signed int x;

if (variata_temp==1)
{
 n=24*(giorno_prog-1)+orario_prog;
 EEPROM.write(n,temp_prog);  
}
variata_temp=0;

orario_prog-=1;
if (orario_prog==255) orario_prog=23;  // Verifica se scende sotto 0;
itoa(orario_prog,buf,10);

strcat(stampa,"t2.txt=\"");

if (orario_prog<=9)
 strcat (stampa,"0");
 strcat (stampa,buf);
 strcat (stampa,":00");

 strcat(stampa,"\"");

//t1_2.setText(stampa);
 Serial.print(stampa);
 Serial.write(0xff);
 Serial.write(0xff);
 Serial.write(0xff);
 memset(stampa,0,sizeof(stampa));

// Colora la barra di blu
 n=24*(giorno_prog-1)+orario_prog;
 temp_prog=EEPROM.read(n);

 gradi_show=(EEPROM.read(n))/10.0;
 dtostrf(gradi_show,4,1,temperatura_letta); 
 char* p  = strchr(temperatura_letta, '.');
 *p = ',';
// t1_3.setText(temperatura_letta);
 strcat(stampa,"t3.txt=\"");
 strcat(stampa,temperatura_letta);
 strcat(stampa,"\"");
 Serial.print(stampa);
 Serial.write(0xff);
 Serial.write(0xff);
 Serial.write(0xff);
 memset(stampa,0,sizeof(stampa));

 x=(orario_prog+1)*15-4;
 itoa(x,buf,10);
 strcat (cerchio,"fill ");
 strcat (cerchio,buf);
 strcat (cerchio,",");
 x=220-(2*(EEPROM.read(n)-130))/3;
 itoa(x,buf,10);
 strcat (cerchio,buf);
 strcat (cerchio,",8,");
 x=1+(2*(EEPROM.read(n)-130))/3;
 itoa(x,buf,10);
 strcat (cerchio,buf);
 strcat(cerchio,",31");
 Serial.print(cerchio);
 Serial.write(0xff);
 Serial.write(0xff);
 Serial.write(0xff);
 memset(cerchio,0,sizeof(cerchio));
// Colora la precedente di arancio
if (orario_prog>=0)
{
 x=(orario_prog+2)*15-4;
 itoa(x,buf,10);
 strcat (cerchio,"fill ");
 strcat (cerchio,buf);
 strcat (cerchio,",");
 x=220-(2*(EEPROM.read(n+1)-130))/3;
 itoa(x,buf,10);
 strcat (cerchio,buf);
 strcat (cerchio,",8,");
 x=1+(2*(EEPROM.read(n+1)-130))/3;
 itoa(x,buf,10);
 strcat (cerchio,buf);
 strcat(cerchio,",64193");
 Serial.print(cerchio);
 Serial.write(0xff);
 Serial.write(0xff);
 Serial.write(0xff);
 memset(cerchio,0,sizeof(cerchio));
}
}
/** FINE DECREMENTA ORARIO GIORNALIERO PROGRAMMAZIONE **/


/* Funzione chiamata alla pressione del tasto b5 pagina 1 */
/*         INCREMENTA TEMPERATURA PROGRAMMAZIONE          */
void b15PopCallback(void *ptr) {
float gradi_show;
char temperatura_letta[5]={0};
signed int x;
char buf[4]={0};
char cerchio[21]={0};

 temp_prog+=1;
 if (temp_prog>250) temp_prog=250;
 variata_temp=1;
 gradi_show=(temp_prog)/10.0;
 dtostrf(gradi_show,4,1,temperatura_letta); 
 char* p  = strchr(temperatura_letta, '.');
 *p = ',';
// t1_3.setText(temperatura_letta);
 strcat(cerchio,"t3.txt=\"");
 strcat(cerchio,temperatura_letta);
 strcat(cerchio,"\"");
 Serial.print(cerchio);
 Serial.write(0xff);
 Serial.write(0xff);
 Serial.write(0xff);
 memset(cerchio,0,sizeof(cerchio));

 x=(orario_prog+1)*15-4;
 itoa(x,buf,10);
 strcat (cerchio,"fill ");
 strcat (cerchio,buf);
 strcat (cerchio,",");
 x=220-(2*(temp_prog-130))/3;
 itoa(x,buf,10);
 strcat (cerchio,buf);
 strcat (cerchio,",8,1,31");
 Serial.print(cerchio);
 Serial.write(0xff);
 Serial.write(0xff);
 Serial.write(0xff);
 memset(cerchio,0,sizeof(cerchio));
}
/** FINE INCREMENTA TEMPERATURA PROGRAMMAZIONE **/


/* Funzione chiamata alla pressione del tasto b6 pagina 1 */
/*         DECREMENTA TEMPERATURA PROGRAMMAZIONE          */
void b16PopCallback(void *ptr) {
float gradi_show;
char temperatura_letta[5]={0};
signed int x;
char buf[4]={0};
char cerchio[21]={0};


 temp_prog-=1;
 if (temp_prog<130) temp_prog=130;
 variata_temp=1;
 gradi_show=(temp_prog)/10.0;
 dtostrf(gradi_show,4,1,temperatura_letta); 
 char* p  = strchr(temperatura_letta, '.');
 *p = ',';
// t1_3.setText(temperatura_letta);
 strcat(cerchio,"t3.txt=\"");
 strcat(cerchio,temperatura_letta);
 strcat(cerchio,"\"");
 Serial.print(cerchio);
 Serial.write(0xff);
 Serial.write(0xff);
 Serial.write(0xff);
 memset(cerchio,0,sizeof(cerchio));

 x=(orario_prog+1)*15-4;
 itoa(x,buf,10);
 strcat (cerchio,"fill ");
 strcat (cerchio,buf);
 strcat (cerchio,",");
 x=220-(2*(temp_prog+1-130))/3;
 itoa(x,buf,10);
 strcat (cerchio,buf);
 if ((x==147)||(x==167)||(x==187)||(x==207))
  strcat (cerchio,",8,1,0"); //Se sopra la righe nere che indicano i gradi fa la riga nera
 else
  strcat (cerchio,",8,1,65535"); // Alrimenti bianca
 
 Serial.print(cerchio);
 Serial.write(0xff);
 Serial.write(0xff);
 Serial.write(0xff);
 memset(cerchio,0,sizeof(cerchio));
}
/** FINE DECREMENTA TEMPERATURA PROGRAMMAZIONE **/

/* Funzione chiamata alla pressione del tast0 b2=FrecciaSu */
/*          INCREMENTA TEMPERATURA MANUALMENTE             */
void b2PopCallback(void *ptr) {
//  tempactual.setText("99");
char buffer[7]={0};
char stringa[14]={0};
//float temp_vis;
if (stato == MANUALE)
{
 temp += 1;
 if (temp > 300) temp=300;
 //strcpy(buffer, "44");
 //tem_vis=temp/10.0;
 dtostrf(temp/10.0, 4, 1, buffer);
 char* p  = strchr(buffer, '.');
 *p = ',';
 //strcat(buffer,"°");
 strcat(stringa,"t7.txt=\"");
 strcat(stringa,buffer);
 strcat(stringa,"\"");
 Serial.print(stringa);
 Serial.write(0xff);
 Serial.write(0xff);
 Serial.write(0xff);

// t7.setText(buffer);
}
}

/* Funzione chiamata alla pressione del tast0 b3=FrecciaGiu */
/*          DECREMENTA TEMPERATURA MANUALMENTE             */
void b3PopCallback(void *ptr) {
char buffer[7]={0};
char stringa[14]={0};
//  tempactual.setText("99");
if (stato == MANUALE)
{
 temp -= 1;
 if (temp < 130) temp=130;
 dtostrf(temp/10.0, 4, 1, buffer);
 char* p  = strchr(buffer, '.');
 *p = ',';
// t7.setText(buffer);
 strcat(stringa,"t7.txt=\"");
 strcat(stringa,buffer);
 strcat(stringa,"\"");
 Serial.print(stringa);
 Serial.write(0xff);
 Serial.write(0xff);
 Serial.write(0xff);
 }
}
/* FINE DECREMENTA TEMPERATURA MANUALMENTE */

/* Funzione chiamata alla pressione del tast0 b0=Auto/Man */
/*            PASSAGGIO DA AUTOMATICO A MANUALE           */
void b0PopCallback(void *ptr) {
if (stato==MANUALE) {
  stato=AUTOMATICO;
  b0.setText("Man");
//  t6.setText("Automatico");
 Serial.print(F("t6.txt=\"Automatico\""));
 Serial.write(0xff);
 Serial.write(0xff);
 Serial.write(0xff);

}
else
{
  stato=MANUALE;
  b0.setText("Auto");
//  t6.setText("Manuale");
 Serial.print(F("t6.txt=\"Manuale\""));
 Serial.write(0xff);
 Serial.write(0xff);
 Serial.write(0xff);
}
}
/* FINE PASSAGGIO DA AUTOMATICO A MANUALE */

/* Funzione chiamata al caricamento della pagina 0 */
/*                 CARICA PAGINA 0                 */
void page0PushCallback(void *ptr)  
{
char buffer[7]={0};
char stringa[14]={0};
b4PushCallback(&temp);  // Aggiorna data, ora, temperatura
MostraMeteo();          //Aggiorna meteo
giorno_prog=1;
orario_prog=0;
dtostrf(temp/10.0, 4, 1, buffer);
char* p  = strchr(buffer, '.');
*p = ',';
 strcat(stringa,"t7.txt=\"");
 strcat(stringa,buffer);
 strcat(stringa,"\"");
 Serial.print(stringa);
 Serial.write(0xff);
 Serial.write(0xff);
 Serial.write(0xff);

//t7.setText(buffer);
if (stato==MANUALE) {
  b0.setText("Auto");
 Serial.print(F("t6.txt=\"Manuale\""));
 Serial.write(0xff);
 Serial.write(0xff);
 Serial.write(0xff);
}
if (stato==AUTOMATICO) {
  b0.setText("Man");
 Serial.print(F("t6.txt=\"Automatico\""));
 Serial.write(0xff);
 Serial.write(0xff);
 Serial.write(0xff);
}
} 
/* FINE CARICA PAGINA 0 */

/* Funzione chiamata al caricamento della pagina 1 */
/*               CARICA PAGINA 1                   */
void page1PushCallback(void *ptr)  
{
char cerchio[21]={0};
char buf[4]={0};
unsigned char n,y,k,w;
signed int x;
orario_prog=0;
w=1;
 Serial.print(F("line 10,207,380,207,0")); // Riga dei 15°
 Serial.write(0xff);
 Serial.write(0xff);
 Serial.write(0xff);
 Serial.print(F("line 10,187,380,187,0")); // Riga dei 18°
 Serial.write(0xff);
 Serial.write(0xff);
 Serial.write(0xff);
 Serial.print(F("line 10,167,380,167,0")); // Riga dei 21°
 Serial.write(0xff);
 Serial.write(0xff);
 Serial.write(0xff);
 Serial.print(F("line 10,147,380,147,0")); // Riga dei 24°
 Serial.write(0xff);
 Serial.write(0xff);
 Serial.write(0xff);

//va1.setValue(giorno_prog);
 itoa(giorno_prog,buf,10);
 strcat(cerchio,"va1.val=");
 strcat(cerchio,buf);
 Serial.print(cerchio);
 Serial.write(0xff);
 Serial.write(0xff);
 Serial.write(0xff);
 memset(cerchio,0,sizeof(cerchio));

for(n=24*(giorno_prog-1);n<=(24*(giorno_prog-1)+23);n++)
{
 x=w*15-4;
 itoa(x,buf,10);
 strcat (cerchio,"fill ");
 strcat (cerchio,buf);
 strcat (cerchio,",");
 x=220-(2*(EEPROM.read(n)-130))/3;
 itoa(x,buf,10);
 strcat (cerchio,buf);
 strcat (cerchio,",8,");
 x=1+(2*(EEPROM.read(n)-130))/3;
 itoa(x,buf,10);
 strcat (cerchio,buf);
 if (w==orario_prog+1)  // Se l'orario di programmazione è corrispondente alla barra che sta disegnando, allora la colora di BLU
  strcat(cerchio,",31");
 else
  strcat(cerchio,",64193");
 
 Serial.print(cerchio);
 Serial.write(0xff);
 Serial.write(0xff);
 Serial.write(0xff);
 memset(cerchio,0,sizeof(cerchio));
 w++;
} 
}
/* FINE CARICA PAGINA 1 */


/* Funzione che visualizza il meteo in funzione dell'andamento della pressione  */
/*                             MOSTRA METEO                                     */
void MostraMeteo()
{
  int pressione=press_mis[5];
  unsigned char month,hour,minuti;
  bool h12,PM,gn;
  bool Century=false;

  hour=Clock.getHour(h12, PM);
  month=Clock.getMonth(Century);

   if ((month>=4)&&(month<=9))
  {
  if ((hour>=20)||(hour<6)) gn=0; // Da aprile a settembre consideriamo notte oltre le 20 e prima delle 6
  else gn=1;
  }
  else 
  {
   if ((hour>=18)||(hour<6)) gn=0; // Da ottobre a marzo consideriamo notte oltre le 18 e prima delle 6
   else gn=1;
  }

   if ((((press_mis[5]+press_mis[4]+press_mis[3])/3)-((press_mis[2]+press_mis[1]+press_mis[0])/3))>=3) //Pressione in salita
   {
//    Messaggi.println(F("Pressione in salita"));
    if (pressione>=1000){
    if (gn==1) Serial.print(F("p1.pic=1"));
    else Serial.print(F("p1.pic=5"));
    } //Sole
    if ((pressione>=990)&&(pressione<1000)){
    if (gn==1) Serial.print(F("p1.pic=2"));
    else Serial.print(F("p1.pic=6"));
    }  //Sole+nuvola
    if ((pressione>=980)&&(pressione<990)){
    if (gn==1) Serial.print(F("p1.pic=3"));
    else Serial.print(F("p1.pic=7"));
    }   //Nuvola
    if (pressione<=980)
    {                      
    if (gn==1) Serial.print(F("p1.pic=4"));
    else Serial.print(F("p1.pic=8"));
    } //Pioggia
   }
   else 
   if ((((press_mis[5]+press_mis[4]+press_mis[3])/3)-((press_mis[2]+press_mis[1]+press_mis[0])/3))<=-3) //Pressione in discesa
   {
//    Messaggi.println(F("Pressione in discesa"));
    if (pressione>=1018){
    if (gn==1) Serial.print(F("p1.pic=1"));
    else Serial.print(F("p1.pic=5"));
    } //Sole
    if ((pressione>=1004)&&(pressione<1018)){
    if (gn==1) Serial.print(F("p1.pic=2"));
    else Serial.print(F("p1.pic=6"));
    }  //Sole+nuvola
    if ((pressione>=990)&&(pressione<1004)){
    if (gn==1) Serial.print(F("p1.pic=3"));
    else Serial.print(F("p1.pic=7"));
    }   //Nuvola
    if (pressione<=990){
    if (gn==1) Serial.print(F("p1.pic=4"));
    else Serial.print(F("p1.pic=8"));
    }                      //Pioggia
   }
   else //Pressione costante
   {
//    Messaggi.println(F("Pressione costante"));
    if (pressione>=1008){
    if (gn==1) Serial.print(F("p1.pic=1"));
    else Serial.print(F("p1.pic=5"));
    } //Sole
    if ((pressione>=990)&&(pressione<1008)){
    if (gn==1) Serial.print(F("p1.pic=2"));
    else Serial.print(F("p1.pic=6"));
    }  //Sole+nuvola
    if ((pressione>=980)&&(pressione<990)){
    if (gn==1) Serial.print(F("p1.pic=3"));
    else Serial.print(F("p1.pic=7"));
    }   //Nuvola
    if (pressione<=990){
    if (gn==1) Serial.print(F("p1.pic=4"));
    else Serial.print(F("p1.pic=8"));
    }                      //Pioggia
   }
   Serial.write(0xff);
   Serial.write(0xff);
   Serial.write(0xff);
}

/* Funzione lettura ora e temperatura chiamata tramite timer ogni 5 secondi */
/*                        REFRESH OGNI 5 SECONDI                            */
void b4PushCallback(void *ptr)
{
// int month,dow,indice,minute,hour;
unsigned char month,dow,indice,minute,hour,j;
// int hour,date,year;
// int second;
 int pressione;
 float temperature;
 bool h12;
 bool PM;
 bool Century=false;
 char data_display[40]={0};
 char ore_str[5] = {0};
 char minuti_str[5] = {0};
 char orario[6] = {0};
// char temperatura_letta[5] = {0};
 char temperatura_prog[5] = {0};
 //char giornosett[11]={0};
 char giorno[3]={0};
// char mese[11]={0};
 char anno[3]={0};
 char anno_intero[5]={0};
 char umidita[3]={0};
 char comando_temperatura[15]={0};
 char stringa[14]={0};
 char temp_str[6]={0};
 bool gn; //Variabile che vale 1 se giorno 0 se notte
 char numero[4]={0};
 
// second=Clock.getSecond();
   minute=Clock.getMinute();
   hour=Clock.getHour(h12, PM);
// date=Clock.getDate();
 month=Clock.getMonth(Century);
// year=Clock.getYear();
 dow=Clock.getDoW();
 itoa(Clock.getHour(h12, PM),ore_str,10);
 itoa(Clock.getMinute(),minuti_str,10);
 itoa(Clock.getDate(),giorno,10);
 itoa(Clock.getYear(),anno,10);
 strcat(anno_intero,"20");
 strcat(anno_intero,anno);
 if ((month>=4)&&(month<=9))
 {
  if ((hour>=20)||(hour<6)) gn=0; // Da aprile a settembre consideriamo notte oltre le 20 e prima delle 6
  else gn=1;
 }
 else 
 {
  if ((hour>=18)||(hour<6)) gn=0; // Da ottobre a marzo consideriamo notte oltre le 18 e prima delle 6
  else gn=1;
 }
 
 strcat(orario,ore_str);
 strcat(orario,":");
 if (minute<=9)
  strcat(orario,"0");
 strcat(orario,minuti_str);
// t1.setText(orario);

 strcat(stringa,"t1.txt=\"");
 strcat(stringa,orario);
 strcat(stringa,"\"");
  Serial.print(stringa);
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);
 memset(stringa,0,sizeof(stringa));

// Temperatura ambiente
// temperature=Clock.getTemperature();
 temperature=bme.readTemperature();
 temperature=round(temperature*100)/100.0;
 dtostrf(temperature,4,1,stringa); 
 char* p  = strchr(stringa, '.');
 *p = ',';
  // Stampa la temperatura letta
  strcat(comando_temperatura,"t11.txt=\"");
  strcat(comando_temperatura,stringa);
  strcat(comando_temperatura,"\"");
  Serial.print(comando_temperatura);
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);
  memset(comando_temperatura,0,sizeof(comando_temperatura));
  memset(stringa,0,sizeof(stringa));
  
  MostraMeteo(); //Aggiorna l'icona meteo
  
// Rilievo dei minuti trascorsi dall'ultima misurazione della pressione e se >1 ora memorizzazione
/*  conta5+=1;
  itoa(conta5,numero,10); 
  strcat(stringa,"t0.txt=\"");
  strcat(stringa,numero);
  strcat(stringa,"\"");
  Serial.print(stringa);
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);
  memset(stringa,0,sizeof(stringa));*/

  
 
// t11.setText(temperatura_letta);

 dtostrf(temp/10.0, 4, 1, temperatura_prog);
 char* q  = strchr(temperatura_prog, '.');
 *q = ',';
 strcat(stringa,"t7.txt=\"");
 strcat(stringa,temperatura_prog);
 strcat(stringa,"\"");
 Serial.print(stringa);
 Serial.write(0xff);
 Serial.write(0xff);
 Serial.write(0xff);
 memset(stringa,0,sizeof(stringa));

// t7.setText(temperatura_prog);

// Se lo stato è AUTOMATICO preleva la temperatura da impostare dalla programmazione
if (stato==AUTOMATICO)
{
  indice=24*(dow-1)+hour;
  temp=EEPROM.read(indice);
}

if (temperature*10 < temp)
{
  // Accende il riscaldamento
  Serial.print(F("vis p0,1"));
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);
  digitalWrite(VA, LOW); //Mette elettrovalvola in ON
}
else
{
  // Spegne il riscaldamento
  Serial.print(F("vis p0,0"));
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);
  digitalWrite(VA, HIGH); //Mette elettrovalvola in OFF

}
// Imposta i valori delle variabili nascose n0 e n1. Poi nel programma del display in base a questi valori scrive il giorno della settimana e il mese
//n0.setValue(dow);
 itoa(dow,temp_str,10);
 strcat(stringa,"n0.val=");
 strcat(stringa,temp_str);
 Serial.print(stringa);
 Serial.write(0xff);
 Serial.write(0xff);
 Serial.write(0xff);
 memset(stringa,0,sizeof(stringa));

//n1.setValue(month);
 itoa(month,temp_str,10);
 strcat(stringa,"n1.val=");
 strcat(stringa,temp_str);
 Serial.print(stringa);
 Serial.write(0xff);
 Serial.write(0xff);
 Serial.write(0xff);
 memset(stringa,0,sizeof(stringa));

 pressione=bme.readPressure()/100;
 itoa(pressione,temp_str,10);
 strcat(stringa,"t13.txt=\"");
 strcat(stringa,temp_str);
 strcat(stringa," hPa\"");
 Serial.print(stringa);
 Serial.write(0xff);
 Serial.write(0xff);
 Serial.write(0xff);
 memset(stringa,0,sizeof(stringa));

//t15.setText(giorno);
 strcat(stringa,"t15.txt=\"");
 strcat(stringa,giorno);
 strcat(stringa,"\"");
 Serial.print(stringa);
 Serial.write(0xff);
 Serial.write(0xff);
 Serial.write(0xff);
 memset(stringa,0,sizeof(stringa));

 strcat(stringa,"t16.txt=\"");
 strcat(stringa,anno_intero);
 strcat(stringa,"\"");
 Serial.print(stringa);
 Serial.write(0xff);
 Serial.write(0xff);
 Serial.write(0xff);
 memset(stringa,0,sizeof(stringa));

//t16.setText(anno_intero);
itoa(bme.readHumidity(),umidita,10);
//itoa(30,umidita,10);
//t4.setText(umidita);
 strcat(stringa,"t4.txt=\"");
 strcat(stringa,umidita);
 strcat(stringa,"\"");
 Serial.print(stringa);
 Serial.write(0xff);
 Serial.write(0xff);
 Serial.write(0xff);
 memset(stringa,0,sizeof(stringa));
}
/*       FINE REFRESH OGNI 5 SECONDI      */


void setup(void) {    
  Wire.begin();
  Serial.begin(9600);
  int ciclo;
    
  // You might need to change NexConfig.h file in your ITEADLIB_Arduino_Nextion folder
  // Set the baudrate which is for debug and communicate with Nextion screen
  nexInit();
  bme.begin(0x76);
  // Register the pop event callback function of the components
  b4.attachPush(b4PushCallback, &b4);
  b3.attachPop(b3PopCallback, &b3);
  b2.attachPop(b2PopCallback, &b2);
  b0.attachPop(b0PopCallback, &b0);
  b11.attachPop(b11PopCallback, &b11);
  b12.attachPop(b12PopCallback, &b12);
  b13.attachPop(b13PopCallback, &b13);
  b14.attachPop(b14PopCallback, &b14);
  b15.attachPop(b15PopCallback, &b15);
  b16.attachPop(b16PopCallback, &b16);
  b17.attachPop(b17PopCallback, &b17);
  b21.attachPop(b21PopCallback, &b21);
  page0.attachPush(page0PushCallback);  // Page 0 press event
  page1.attachPush(page1PushCallback);  // Page 1 press event

//for(ciclo=0;ciclo<=167;ciclo++)
// EEPROM.write(ciclo,130);
//EEPROM.write(12,220);
//EEPROM.write(13,225);
//EEPROM.write(14,227);
//EEPROM.write(15,230);
//EEPROM.write(16,235);
//char nomebar[3]={0};
//char numerox[3]={0};

// Nasconde il pulsante b4
  Serial.print(F("vis b4,0"));
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);

  pinMode(VA, OUTPUT); 
  digitalWrite(VA, HIGH); //Mette elettrovalvola in OFF

//  Messaggi.begin(9600);

}

void loop(void) {   
  unsigned char minuti,j,hour=0;
  bool h12,PM;
  int pressione;
  
  hour=Clock.getHour(h12, PM);
  if (hour!=prec_hour)
  {
    prec_hour=hour;
    aggiorna_pressione=1;
  }

  if (aggiorna_pressione==1) {
    aggiorna_pressione=0;
    pressione=bme.readPressure()/100;
    for (j=1;j<=5;j++)
    {
      press_mis[j-1]=press_mis[j]; //Sposta indietro gli ultimi valori
    }
    press_mis[5]=pressione; //Mette in sesta posizione il valore letto

/*
  minuti=Clock.getMinute();
  Messaggi.print(F("Ore: "));
  Messaggi.print(hour);
  Messaggi.print(F(":"));
  Messaggi.println(minuti);
  Messaggi.print(F("Pressione0 = "));
  Messaggi.println(press_mis[0]);
  Messaggi.print(F("Pressione1 = "));
  Messaggi.println(press_mis[1]);
  Messaggi.print(F("Pressione2 = "));
  Messaggi.println(press_mis[2]);
  Messaggi.print(F("Pressione3 = "));
  Messaggi.println(press_mis[3]);
  Messaggi.print(F("Pressione4 = "));
  Messaggi.println(press_mis[4]);
  Messaggi.print(F("Pressione5 = "));
  Messaggi.println(press_mis[5]);
*/
//    MostraMeteo();
   }

  /*
   * When a pop or push event occured every time,
   * the corresponding component[right page id and component id] in touch event list will be asked.
   */
  nexLoop(nex_listen_list);
}
