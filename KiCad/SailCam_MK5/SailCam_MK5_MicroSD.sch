EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 7 7
Title "SailCam"
Date "2021-07-31"
Rev "MK5"
Comp "Octana Laboratories"
Comment1 "Tyler Thompson"
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L DM3BT-DSF-PEJS:DM3BT-DSF-PEJS J?
U 1 1 611DCAF8
P 5050 2950
AR Path="/611DCAF8" Ref="J?"  Part="1" 
AR Path="/611DB121/611DCAF8" Ref="J8"  Part="1" 
F 0 "J8" H 5950 3427 60  0000 C CNN
F 1 "DM3BT-DSF-PEJS" H 5950 3321 60  0000 C CNN
F 2 "footprints:DM3BT-DSF-PEJS" H 5950 3190 60  0001 C CNN
F 3 "https://www.hirose.com/product/document?clcode=CL0609-0033-6-00&productname=DM3AT-SF-PEJ2M5&series=DM3&documenttype=Catalog&lang=en&documentid=D49662_en" H 5050 2950 60  0001 C CNN
F 4 "Micro SD Card Slot" H 5950 3223 50  0000 C CNN "Desription"
	1    5050 2950
	1    0    0    -1  
$EndComp
$Comp
L MC74HC365ADG-ND:MC74HC365ADG U?
U 1 1 611DCAFF
P 5150 4200
AR Path="/611DCAFF" Ref="U?"  Part="1" 
AR Path="/611DB121/611DCAFF" Ref="U9"  Part="1" 
F 0 "U9" H 5950 4677 60  0000 C CNN
F 1 "MC74HC365ADG" H 5950 4571 60  0000 C CNN
F 2 "footprints:MC74HC365ADG" H 6750 4440 60  0001 C CNN
F 3 "https://www.onsemi.com/pdf/datasheet/mc74hc365a-d.pdf" H 5150 4200 60  0001 C CNN
F 4 "3.3v Level Buffer" H 5950 4473 50  0000 C CNN "Description"
	1    5150 4200
	1    0    0    -1  
$EndComp
Text Label 5150 4200 2    50   ~ 0
GND
Text Label 5150 4900 2    50   ~ 0
GND
Text Label 6750 4300 0    50   ~ 0
GND
Text Label 6850 3350 0    50   ~ 0
GND
Text Label 6750 4200 0    50   ~ 0
Vcc
Text Label 5050 3250 2    50   ~ 0
Vcc
Text HLabel 5600 1550 0    50   Input ~ 0
Vcc
Text HLabel 5600 1650 0    50   Input ~ 0
GND
Text HLabel 5150 4600 0    50   Input ~ 0
MISO
Text HLabel 5150 4300 0    50   Input ~ 0
MOSI
Text HLabel 6750 4400 2    50   Input ~ 0
CS
Text HLabel 5150 4700 0    50   Input ~ 0
SCK
Text Label 5600 1550 0    50   ~ 0
Vcc
Text Label 5600 1650 0    50   ~ 0
GND
Text Label 5150 4400 2    50   ~ 0
SD_MISO
Text Label 5150 4500 2    50   ~ 0
SD_MOSI
Text Label 5150 4800 2    50   ~ 0
SD_SCK
Text Label 6750 4500 0    50   ~ 0
SD_CS
Text Label 5050 3350 2    50   ~ 0
SD_SCK
Text Label 5050 3050 2    50   ~ 0
SD_CS
Text Label 5050 3150 2    50   ~ 0
SD_MOSI
Text Label 6850 3250 0    50   ~ 0
SD_MISO
NoConn ~ 6850 3150
NoConn ~ 5050 2950
NoConn ~ 6850 3050
NoConn ~ 6850 2950
NoConn ~ 6750 4600
NoConn ~ 6750 4700
NoConn ~ 6750 4800
NoConn ~ 6750 4900
$Comp
L Device:C_Small C?
U 1 1 61294A83
P 7800 4250
AR Path="/61294A83" Ref="C?"  Part="1" 
AR Path="/611DB121/61294A83" Ref="C12"  Part="1" 
F 0 "C12" H 7892 4296 50  0000 L CNN
F 1 "0.1 uF" H 7892 4205 50  0000 L CNN
F 2 "footprints:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder_FullSilk" H 7800 4250 50  0001 C CNN
F 3 "~" H 7800 4250 50  0001 C CNN
	1    7800 4250
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 61296891
P 9650 5650
AR Path="/61296891" Ref="#PWR?"  Part="1" 
AR Path="/611BA1F2/61296891" Ref="#PWR?"  Part="1" 
AR Path="/611DB121/61296891" Ref="#PWR022"  Part="1" 
F 0 "#PWR022" H 9650 5400 50  0001 C CNN
F 1 "GND" H 9655 5477 50  0000 C CNN
F 2 "" H 9650 5650 50  0001 C CNN
F 3 "" H 9650 5650 50  0001 C CNN
	1    9650 5650
	1    0    0    -1  
$EndComp
$Comp
L power:VCC #PWR?
U 1 1 61296897
P 9650 5500
AR Path="/61296897" Ref="#PWR?"  Part="1" 
AR Path="/611821E5/61296897" Ref="#PWR?"  Part="1" 
AR Path="/61199C7C/61296897" Ref="#PWR?"  Part="1" 
AR Path="/611DB121/61296897" Ref="#PWR021"  Part="1" 
F 0 "#PWR021" H 9650 5350 50  0001 C CNN
F 1 "VCC" H 9667 5673 50  0000 C CNN
F 2 "" H 9650 5500 50  0001 C CNN
F 3 "" H 9650 5500 50  0001 C CNN
	1    9650 5500
	1    0    0    -1  
$EndComp
Text Label 9650 5500 2    50   ~ 0
Vcc
Text Label 9650 5650 2    50   ~ 0
GND
$Comp
L power:VCC #PWR?
U 1 1 61296E54
P 7800 4150
AR Path="/61296E54" Ref="#PWR?"  Part="1" 
AR Path="/611821E5/61296E54" Ref="#PWR?"  Part="1" 
AR Path="/61199C7C/61296E54" Ref="#PWR?"  Part="1" 
AR Path="/611DB121/61296E54" Ref="#PWR019"  Part="1" 
F 0 "#PWR019" H 7800 4000 50  0001 C CNN
F 1 "VCC" H 7817 4323 50  0000 C CNN
F 2 "" H 7800 4150 50  0001 C CNN
F 3 "" H 7800 4150 50  0001 C CNN
	1    7800 4150
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 61297559
P 7800 4350
AR Path="/61297559" Ref="#PWR?"  Part="1" 
AR Path="/611BA1F2/61297559" Ref="#PWR?"  Part="1" 
AR Path="/611DB121/61297559" Ref="#PWR020"  Part="1" 
F 0 "#PWR020" H 7800 4100 50  0001 C CNN
F 1 "GND" H 7805 4177 50  0000 C CNN
F 2 "" H 7800 4350 50  0001 C CNN
F 3 "" H 7800 4350 50  0001 C CNN
	1    7800 4350
	1    0    0    -1  
$EndComp
$EndSCHEMATC
