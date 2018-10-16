# PFE-drivers
Embedded Linux Drivers development



### 16/10/2018 RdV N°3

#### Bilan de Période 1

- Compilation croisée (Muyao) OK
- Première rencontre avec les drivers OK



#### Travail à venir

- Mettre en place un driver simple (print ou LED)
- Etudier et programmer un driver pour une périphérique (SPI, I2C...)





### 09/10/2018 RdV N°2

- Signaler des problèmes concernant U-boot



### 02/10/2018 RdV N°1

#### Points à aborder:

- Les attendues du projet
- Planification provisoire
- Choix de la carte - Beaglebone Black
- Manière de suivi 
  - soutenances intermédiaires entre des périodes
  - fréquence de suivi
- Travail en cours 
  - Muyao - Uboot+Linux sur la carte BBB
  - Amal



### 30/09/2018 Let's start from here!

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

