import time
import machine
import utime
from machine import Pin

Pin_reset = Pin(35,Pin.IN)
Pin_Trigger = Pin(18,Pin.OUT)
Pin_Echo = Pin(34,Pin.IN, machine.Pin.PULL_DOWN)
Pin_Motor1_1 = Pin(0)
Pin_Motor1_2 = Pin(0)
def Ultraschallsensor():
    #Das Trigger signal wird hier für 10 Mikrosekunden auf HIGH gesetzt dadurch
    #startet der Ultraschallsensor mit dem messen
    Pin_Trigger.value(1)
    print(10)
    utime.sleep_us(10)
    Pin_Trigger.value(0)
    lowHIGH = 0
    highLOW = 0
    echo = 0
    cm = 0

    while 0 == lowHIGH or highLOW == 0:

        VoherigesSignalEcho = echo
        echo = Pin_Echo.value()
        if 0 == lowHIGH and 0 == VoherigesSignalEcho and 1 == echo:
            lowHIGH = 1
            startTime = utime.ticks_us()
            print("start Time= "+str(startTime))
        if 1 == lowHIGH and 1 == VoherigesSignalEcho and 0 == echo:
            highLOW = 1
            stopTime = utime.ticks_us()
            print("stop Time = " + str(stopTime))
    difference = stopTime-startTime
    rangeCm = difference / 58
    print("Reichweite = "+str(rangeCm))
    """
    while not Pin_Echo.value():
        pass
    while Pin_Echo.value():
        utime.sleep_us(58)
        cm = cm+1
    rangeCm = cm
    print(rangeCm)
    return rangeCm
    """
def main():
    time.sleep(1)
    print("Start Ultraschallsensor")
    Ultraschallsensor()

while Pin_reset.value() == 1:
    print("Start main")
    main()
