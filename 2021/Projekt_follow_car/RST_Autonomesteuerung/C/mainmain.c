#include <ncurses.h>
#include <curses.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <wiringPi.h>

/*
// Hier die Quelle auf welcher Basis ich mein Programm geschrieben habe
Quelle:
https://www.rahner-edu.de/mikrocontroller/prop-sensoren-et-al/sensor-hc-sr04/
https://www.mikrocontroller.net/attachment/highlight/193200
*/
//Hier legen wir die augänge fest einmal den Trigger 
//der zum aktieviren des Ultraschallsensors ist
//-----------------------------
#define PIN_TRIGGER 4 // GPIO Pin 23
#define PIN_ECHO 5 // GPIO Pin 24
#define Pin_PWM 6 // GPIO Pin 40
#define PwmMax 255 // Wie weit wird maximal die reichweite gemessen
#define PwmMin 2 // Wie weit wird maximal der mindest abstand gemessen
//-----------------------------
//Hier ist die funktion mit der wir uns die microsekunden rausziehen
//-----------------------------
long getMicrotime()
{
  struct timeval currentTime;
  gettimeofday(&currentTime, NULL);
  
  return currentTime.tv_sec * (int)1e6 + currentTime.tv_usec;
}
//-----------------------------

float Reichweite()
{
  //Hier legen wir die ausgänge fest 
  //was ein input oder output ist und setzten die auf low
  //-----------------------------
  wiringPiSetup();
  pinMode(PIN_TRIGGER, OUTPUT);
  digitalWrite(PIN_TRIGGER, LOW);
  pinMode(PIN_ECHO, INPUT);
  //-----------------------------
  //Das --Trigger-- signal wird hier für 10 µs auf High gesetzt das 
  //sorgt dann für den impuls zum starten des Ultraschallsensors
  //-----------------------------
  digitalWrite(PIN_TRIGGER, HIGH);
  usleep(10);
  digitalWrite(PIN_TRIGGER, LOW);
  //-----------------------------
  //Hier initalisieren wir unsere variabeln in denen wir nacher
  //unsere daten zum messen der entfernung 
  //-----------------------------
  int echo, VorherigesSignalEcho, lowHigh, highLow;
  long startTime, stopTime, difference;
  float rangeCm;
  lowHigh = 0;
  highLow = 0;
  echo = 0;
  //-----------------------------------
  //Hier starten wir eine schleife in der wir
  //abfragen ob der sensor grade was zurück gegeben hat
  //das sind 2 werte einmal das anfangs signal und das end signal
  //-----------------------------------
  while(0 == lowHigh || highLow == 0)
  {
  //-----------------------------------
    //Hier lesen wir den echo eingang aus und setzen in davor auf VorherigesSignalEcho
    //das sorgt dafür das wir das signal was wir im durchlauf vor dem jetzigen
    //eingelesen haben das dass als VorherigesSignalEcho gespeichert wird
    //-----------------------------------
    VorherigesSignalEcho = echo;
    echo = digitalRead(PIN_ECHO);
    //-----------------------------------
    //Der Sensor setzt zuerst seinen echo Ausgang auf High wenn er das signal aussendet
    //sobald dann das signal reflektiert wird und das signal wieder beim
    //ultraschallsenor ankommt setzt er den pin auf Low
    //-----------------------------------
    if(0 == lowHigh && 0 == VorherigesSignalEcho && 1 == echo)
    {
      lowHigh = 1;
      startTime = getMicrotime();
    }
    //-----------------------------------
    // Wenn das Voherige Signal auf dem Echo HIGH war und jetzt Nicht mehr
    // So wie auch voher die Variable "lowHigh" auf HIGH gesetzt wurde in der voherigen
    // "if" abfrage
    //-----------------------------------
    if(1 == lowHigh && 1 == VorherigesSignalEcho && 0 == echo)
    {
      highLow = 1;
      stopTime = getMicrotime();
    }
    
  }
  difference = stopTime - startTime;
  rangeCm = difference / 58;
  printf("Start: %ld, stop: %ld, difference: %ld, range: %.2f cm\n", startTime, stopTime, difference, rangeCm);
  
  if (rangeCm >= PwmMax)
  {
    rangeCm = PwmMax;
  }
  if (rangeCm <= PwmMin)
  {
      rangeCm = PwmMin;
  }
  return rangeCm;
}


int main()
{

    initscr();                 /* Start curses mode     */
    //printw("Hello World !!!"); /* Print Hello World    */
    //refresh();
    /* Print it on to the real screen */
    // Hier Initen wir unsere variabeln
    int i;
    char taste;
    char buffer [50];
    int minSeite = 0;
    int maxSeite = 50;
    int min = 0;
    int max = 100;
    int vorne = 0;
    int hinten = 0;
    int rechts = 0;
    int links = 0;
    char vornechar;
    char hintenchar;
    char rechtschar;
    char linkschar;
    float OLdAbstand = 0;
    float Abstand = 0;
    // Die system Funktion schluckt nur Strings in c gibts von grund auf kein c deswegen bauen wir uns hier einen mit der funktion sprintf
    i =sprintf (buffer,"./main %i %i %i %i\n",vorne,hinten,rechts,links);
    // die While Schleife damit bestimmen wir wie stark wir das auto anschlagen wollen
    OLdAbstand = Reichweite();
    while (true)
    {
        Abstand = Reichweite();
        if (OLdAbstand > Abstand+10)
        {
            vorne = vorne-10;
            
        }
        else if (OLdAbstand < Abstand+10)
        {
            vorne = vorne+10;
        }
        taste = getch();
        switch (taste)
        {
            case 'a':
            {

                if (links > min)
                {
                    links = links-10;
                }
                else if (rechts < max)
                {
                    rechts = rechts+10;
                }
                break;
            }
            case 'd':
            {
                if (rechts > min)
                {
                    rechts = rechts-10;
                }
                else if (links < max)
                {
                    links = links+10;
                }

                break;
            }
            case 'f':
            {
                vorne = 0;
                hinten = 0;
                rechts = 0;
                links = 0;
                break;
            }

        }
        
        //printw("./main %i %i %i %i\n",vorne,hinten,rechts,links);
        /*
        strcpy(kon, "./main %i %i %i %i\n",vorne,hinten,rechts,links)
        


        i =sprintf (buffer,"./main %i %i %i %i\n",vorne,hinten,rechts,links);
        // von der Funktion system(buffer) gehts dann zum Grund steuerungs c programm
        system(buffer);
        */
    }
    endwin();
}

