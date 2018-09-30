# PFE-drivers
Embedded Linux Drivers development



### 2018/9/30 Let's start from here!

#### La planification provisoire

- Carte : BBB (BeagleBone Black)

- Périphériques : 

  - ___Char Devices___ : LEDs(__GPIO__), OLED (__I2C__), Device __SPI__, Ecran LCD (__Périphérique complex__),  
  - ___Block Devices___ : Carte SD
  - (___Network Devices___)

- Etapes :

  - Période 1 (2 Semaines) : **Découvert**
    - Cross-compilation du kernel Linux pour la carte ;		chargé(e) : Muyao
    - Première d'étude d'un programme pilote (driver) - LED (GPIO)  		chargé(e) : Amal
  - Période 2 (4 Semaines) : __Char Driver Début__
    - Développement du driver pour un Ecran OLED (I2C)  	chargé(e) : Amal
    - Développement du driver pour un device SPI	chargé(e) : Muyao
  - Période 3 (4 Semaines) : __Char Driver Avancé__
    - Développement du driver pour un Ecran LCD 	chargé(e) : Muyao et Amal
  - Période 4 (4 Semaine) : __Block Driver__
    - Développement du driver pour une carte SD	 	chargé(e) : Muyao et Amal
  - 3 Semaines non programées (Gestion de risques, Rédaction du rapport, Nework Device...)

  

