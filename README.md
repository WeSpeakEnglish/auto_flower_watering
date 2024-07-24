# auto_flower_watering
Auto flower watering simple system, based on [Cytron Maker Moisture sensor](https://www.cytron.io/p-maker-soil-moisture-sensor) and ultracheap [WeAct STM32G031F8P6](https://github.com/WeActStudio/WeActStudio.STM32G0xxC0xxCoreBoard/) module. Middle power consumplion as for now cca 5mA. With LiIon accu. 1350mA stay cca. 10 days. (may be imroved by adding additional switch for the sensor, and sleep mode intro into MCU code), this was 1 day setup and not optimized now.
Checks humidity level every 2000 sec. by collectiong 8 samples (in interval 1 sec). Calculating middle value and deside to water or not (switching small waterpump via mosfet transistor for 2 seconds).
Fast DIY construction. 
The video of how it works is here: [VIDEO](https://youtube.com/shorts/iQrWdliZB8A?si=7inJVKsTHUqY_74Y)  
