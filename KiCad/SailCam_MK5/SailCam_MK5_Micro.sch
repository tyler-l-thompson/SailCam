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
L dk_RF-Transceiver-Modules:2491 M?
U 1 1 6121FD37
P 6150 2600
AR Path="/6121FD37" Ref="M?"  Part="1" 
AR Path="/61217E38/6121FD37" Ref="M1"  Part="1" 
F 0 "M1" H 6050 3193 60  0000 C CNN
F 1 "2491" H 6050 3087 60  0000 C CNN
F 2 "digikey-footprints:WIFI_Module_ESP8266_2491" H 6350 2800 60  0001 L CNN
F 3 "https://media.digikey.com/pdf/Data%20Sheets/Adafruit%20PDFs/2491_Web.pdf" H 6350 2900 60  0001 L CNN
F 4 "1528-1438-ND" H 6350 3000 60  0001 L CNN "Digi-Key_PN"
F 5 "2491" H 6350 3100 60  0001 L CNN "MPN"
F 6 "RF/IF and RFID" H 6350 3200 60  0001 L CNN "Category"
F 7 "RF Transceiver Modules" H 6350 3300 60  0001 L CNN "Family"
F 8 "https://media.digikey.com/pdf/Data%20Sheets/Adafruit%20PDFs/2491_Web.pdf" H 6350 3400 60  0001 L CNN "DK_Datasheet_Link"
F 9 "/product-detail/en/adafruit-industries-llc/2491/1528-1438-ND/5761206" H 6350 3500 60  0001 L CNN "DK_Detail_Page"
F 10 "ESP8266 SMT MODULE" H 6050 2981 60  0000 C CNN "Description"
F 11 "Adafruit Industries LLC" H 6350 3700 60  0001 L CNN "Manufacturer"
F 12 "Active" H 6350 3800 60  0001 L CNN "Status"
	1    6150 2600
	1    0    0    -1  
$EndComp
Text Label 5650 3300 2    50   ~ 0
GND
Text Label 6450 2400 0    50   ~ 0
Vcc
Text Label 5650 3000 2    50   ~ 0
SCL
Text Label 5650 2900 2    50   ~ 0
SDA
Text Label 6450 3100 0    50   ~ 0
MOSI
Text Label 6450 3000 0    50   ~ 0
SCK
Text Label 6450 2800 0    50   ~ 0
Cam_CS
Text Label 5650 2800 2    50   ~ 0
SD_CS
Text Label 5650 2500 2    50   ~ 0
ADC0
Text Label 6450 2600 0    50   ~ 0
Rx0
$Comp
L Device:R_Small_US R?
U 1 1 6121FD4A
P 4950 2450
AR Path="/6121FD4A" Ref="R?"  Part="1" 
AR Path="/61217E38/6121FD4A" Ref="R18"  Part="1" 
F 0 "R18" H 5018 2541 50  0000 L CNN
F 1 "10 KΩ" H 5018 2450 50  0000 L CNN
F 2 "footprints:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder_FullSilk" H 4950 2450 50  0001 C CNN
F 3 "~" H 4950 2450 50  0001 C CNN
F 4 "5%" H 5018 2359 50  0000 L CNN "Tolerance"
	1    4950 2450
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small_US R?
U 1 1 6121FD51
P 5400 1450
AR Path="/6121FD51" Ref="R?"  Part="1" 
AR Path="/61217E38/6121FD51" Ref="R19"  Part="1" 
F 0 "R19" H 5468 1541 50  0000 L CNN
F 1 "10 KΩ" H 5468 1450 50  0000 L CNN
F 2 "footprints:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder_FullSilk" H 5400 1450 50  0001 C CNN
F 3 "~" H 5400 1450 50  0001 C CNN
F 4 "5%" H 5468 1359 50  0000 L CNN "Tolerance"
	1    5400 1450
	1    0    0    -1  
$EndComp
Wire Wire Line
	4950 2600 4950 2550
Wire Wire Line
	5650 2600 4950 2600
$Comp
L Device:R_Small_US R?
U 1 1 6121FD5C
P 4550 2550
AR Path="/6121FD5C" Ref="R?"  Part="1" 
AR Path="/61217E38/6121FD5C" Ref="R17"  Part="1" 
F 0 "R17" H 4618 2641 50  0000 L CNN
F 1 "10 KΩ" H 4618 2550 50  0000 L CNN
F 2 "footprints:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder_FullSilk" H 4550 2550 50  0001 C CNN
F 3 "~" H 4550 2550 50  0001 C CNN
F 4 "5%" H 4618 2459 50  0000 L CNN "Tolerance"
	1    4550 2550
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small_US R?
U 1 1 6121FD63
P 4200 2650
AR Path="/6121FD63" Ref="R?"  Part="1" 
AR Path="/61217E38/6121FD63" Ref="R16"  Part="1" 
F 0 "R16" H 4268 2741 50  0000 L CNN
F 1 "10 KΩ" H 4268 2650 50  0000 L CNN
F 2 "footprints:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder_FullSilk" H 4200 2650 50  0001 C CNN
F 3 "~" H 4200 2650 50  0001 C CNN
F 4 "5%" H 4268 2559 50  0000 L CNN "Tolerance"
	1    4200 2650
	1    0    0    -1  
$EndComp
Wire Wire Line
	5650 2700 4550 2700
Wire Wire Line
	4550 2700 4550 2650
Wire Wire Line
	5650 2800 4200 2800
Wire Wire Line
	4200 2800 4200 2750
$Comp
L Device:R_Small_US R?
U 1 1 6121FD6E
P 7050 3050
AR Path="/6121FD6E" Ref="R?"  Part="1" 
AR Path="/61217E38/6121FD6E" Ref="R22"  Part="1" 
F 0 "R22" H 7118 3141 50  0000 L CNN
F 1 "10 KΩ" H 7118 3050 50  0000 L CNN
F 2 "footprints:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder_FullSilk" H 7050 3050 50  0001 C CNN
F 3 "~" H 7050 3050 50  0001 C CNN
F 4 "5%" H 7118 2959 50  0000 L CNN "Tolerance"
	1    7050 3050
	1    0    0    -1  
$EndComp
Wire Wire Line
	7050 2950 7050 2900
Wire Wire Line
	7050 2900 6450 2900
$Comp
L power:VCC #PWR?
U 1 1 6121FD76
P 5400 1350
AR Path="/6121FD76" Ref="#PWR?"  Part="1" 
AR Path="/611821E5/6121FD76" Ref="#PWR?"  Part="1" 
AR Path="/61199C7C/6121FD76" Ref="#PWR?"  Part="1" 
AR Path="/61217E38/6121FD76" Ref="#PWR09"  Part="1" 
F 0 "#PWR09" H 5400 1200 50  0001 C CNN
F 1 "VCC" H 5417 1523 50  0000 C CNN
F 2 "" H 5400 1350 50  0001 C CNN
F 3 "" H 5400 1350 50  0001 C CNN
	1    5400 1350
	1    0    0    -1  
$EndComp
$Comp
L power:VCC #PWR?
U 1 1 6121FD7C
P 4950 2350
AR Path="/6121FD7C" Ref="#PWR?"  Part="1" 
AR Path="/611821E5/6121FD7C" Ref="#PWR?"  Part="1" 
AR Path="/61199C7C/6121FD7C" Ref="#PWR?"  Part="1" 
AR Path="/61217E38/6121FD7C" Ref="#PWR06"  Part="1" 
F 0 "#PWR06" H 4950 2200 50  0001 C CNN
F 1 "VCC" H 4967 2523 50  0000 C CNN
F 2 "" H 4950 2350 50  0001 C CNN
F 3 "" H 4950 2350 50  0001 C CNN
	1    4950 2350
	1    0    0    -1  
$EndComp
$Comp
L power:VCC #PWR?
U 1 1 6121FD82
P 4550 2450
AR Path="/6121FD82" Ref="#PWR?"  Part="1" 
AR Path="/611821E5/6121FD82" Ref="#PWR?"  Part="1" 
AR Path="/61199C7C/6121FD82" Ref="#PWR?"  Part="1" 
AR Path="/61217E38/6121FD82" Ref="#PWR05"  Part="1" 
F 0 "#PWR05" H 4550 2300 50  0001 C CNN
F 1 "VCC" H 4567 2623 50  0000 C CNN
F 2 "" H 4550 2450 50  0001 C CNN
F 3 "" H 4550 2450 50  0001 C CNN
	1    4550 2450
	1    0    0    -1  
$EndComp
$Comp
L power:VCC #PWR?
U 1 1 6121FD88
P 4200 2550
AR Path="/6121FD88" Ref="#PWR?"  Part="1" 
AR Path="/611821E5/6121FD88" Ref="#PWR?"  Part="1" 
AR Path="/61199C7C/6121FD88" Ref="#PWR?"  Part="1" 
AR Path="/61217E38/6121FD88" Ref="#PWR04"  Part="1" 
F 0 "#PWR04" H 4200 2400 50  0001 C CNN
F 1 "VCC" H 4217 2723 50  0000 C CNN
F 2 "" H 4200 2550 50  0001 C CNN
F 3 "" H 4200 2550 50  0001 C CNN
	1    4200 2550
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 6121FD8E
P 7050 3150
AR Path="/6121FD8E" Ref="#PWR?"  Part="1" 
AR Path="/611BA1F2/6121FD8E" Ref="#PWR?"  Part="1" 
AR Path="/61217E38/6121FD8E" Ref="#PWR014"  Part="1" 
F 0 "#PWR014" H 7050 2900 50  0001 C CNN
F 1 "GND" H 7055 2977 50  0000 C CNN
F 2 "" H 7050 3150 50  0001 C CNN
F 3 "" H 7050 3150 50  0001 C CNN
	1    7050 3150
	1    0    0    -1  
$EndComp
$Comp
L SailCam_MK5-rescue:CH340G-CH340G_Interface_USB U?
U 1 1 6121FD94
P 6050 4600
AR Path="/6121FD94" Ref="U?"  Part="1" 
AR Path="/61217E38/6121FD94" Ref="U8"  Part="1" 
F 0 "U8" H 6050 5267 50  0000 C CNN
F 1 "CH340G" H 6050 5176 50  0000 C CNN
F 2 "Package_SO:SOIC-16_3.9x9.9mm_P1.27mm" H 6100 4050 50  0001 L CNN
F 3 "http://www.datasheet5.com/pdf-local-2195953" H 5700 5400 50  0001 C CNN
	1    6050 4600
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 61220EEA
P 4950 4950
AR Path="/61220EEA" Ref="#PWR?"  Part="1" 
AR Path="/611BA1F2/61220EEA" Ref="#PWR?"  Part="1" 
AR Path="/61217E38/61220EEA" Ref="#PWR07"  Part="1" 
F 0 "#PWR07" H 4950 4700 50  0001 C CNN
F 1 "GND" H 4955 4777 50  0000 C CNN
F 2 "" H 4950 4950 50  0001 C CNN
F 3 "" H 4950 4950 50  0001 C CNN
	1    4950 4950
	1    0    0    -1  
$EndComp
Wire Wire Line
	4850 4950 4950 4950
Connection ~ 4950 4950
$Comp
L power:GND #PWR?
U 1 1 61226E1E
P 5650 5000
AR Path="/61226E1E" Ref="#PWR?"  Part="1" 
AR Path="/611BA1F2/61226E1E" Ref="#PWR?"  Part="1" 
AR Path="/61217E38/61226E1E" Ref="#PWR012"  Part="1" 
F 0 "#PWR012" H 5650 4750 50  0001 C CNN
F 1 "GND" H 5655 4827 50  0000 C CNN
F 2 "" H 5650 5000 50  0001 C CNN
F 3 "" H 5650 5000 50  0001 C CNN
	1    5650 5000
	1    0    0    -1  
$EndComp
NoConn ~ 5650 4400
NoConn ~ 6450 4800
NoConn ~ 6450 4700
NoConn ~ 6450 4600
NoConn ~ 6450 4500
Text Label 5250 4350 0    50   ~ 0
Bat_Chg_Vcc
Wire Wire Line
	5450 4200 5650 4200
$Comp
L Device:C_Small C?
U 1 1 6122B8E0
P 5150 5700
AR Path="/6122B8E0" Ref="C?"  Part="1" 
AR Path="/61217E38/6122B8E0" Ref="C7"  Part="1" 
F 0 "C7" H 5242 5746 50  0000 L CNN
F 1 "0.1 uF" H 5242 5655 50  0000 L CNN
F 2 "footprints:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder_FullSilk" H 5150 5700 50  0001 C CNN
F 3 "~" H 5150 5700 50  0001 C CNN
	1    5150 5700
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 6122B8E6
P 5150 5800
AR Path="/6122B8E6" Ref="#PWR?"  Part="1" 
AR Path="/611BA1F2/6122B8E6" Ref="#PWR?"  Part="1" 
AR Path="/61217E38/6122B8E6" Ref="#PWR08"  Part="1" 
F 0 "#PWR08" H 5150 5550 50  0001 C CNN
F 1 "GND" H 5155 5627 50  0000 C CNN
F 2 "" H 5150 5800 50  0001 C CNN
F 3 "" H 5150 5800 50  0001 C CNN
	1    5150 5800
	1    0    0    -1  
$EndComp
Wire Wire Line
	5150 5600 5450 5600
$Comp
L Device:Crystal Y2
U 1 1 6122FDFD
P 4050 5400
F 0 "Y2" V 3959 5531 50  0000 L CNN
F 1 "ECS-120-20-4X" V 4050 5531 50  0000 L CNN
F 2 "footprints:XTAL_ECS-120-20-4X" H 4050 5400 50  0001 C CNN
F 3 "https://ecsxtal.com/store/pdf/hc-49usx.pdf" H 4050 5400 50  0001 C CNN
F 4 "12 MHz Crystal" V 4141 5531 50  0000 L CNN "Description"
	1    4050 5400
	0    1    1    0   
$EndComp
$Comp
L Device:C_Small C?
U 1 1 61235D34
P 3650 5550
AR Path="/61235D34" Ref="C?"  Part="1" 
AR Path="/61217E38/61235D34" Ref="C6"  Part="1" 
F 0 "C6" V 3850 5450 50  0000 L CNN
F 1 "22 pF" V 3750 5450 50  0000 L CNN
F 2 "footprints:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder_FullSilk" H 3650 5550 50  0001 C CNN
F 3 "~" H 3650 5550 50  0001 C CNN
	1    3650 5550
	0    1    1    0   
$EndComp
$Comp
L Device:C_Small C?
U 1 1 612375C6
P 3650 5250
AR Path="/612375C6" Ref="C?"  Part="1" 
AR Path="/61217E38/612375C6" Ref="C5"  Part="1" 
F 0 "C5" V 3450 5150 50  0000 L CNN
F 1 "22 pF" V 3550 5150 50  0000 L CNN
F 2 "footprints:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder_FullSilk" H 3650 5250 50  0001 C CNN
F 3 "~" H 3650 5250 50  0001 C CNN
	1    3650 5250
	0    1    1    0   
$EndComp
Wire Wire Line
	3750 5250 4050 5250
Wire Wire Line
	4050 5550 3750 5550
Wire Wire Line
	3400 5550 3550 5550
Wire Wire Line
	3400 5550 3400 5250
Wire Wire Line
	3400 5250 3550 5250
$Comp
L power:GND #PWR?
U 1 1 61239E4D
P 3400 5550
AR Path="/61239E4D" Ref="#PWR?"  Part="1" 
AR Path="/611BA1F2/61239E4D" Ref="#PWR?"  Part="1" 
AR Path="/61217E38/61239E4D" Ref="#PWR03"  Part="1" 
F 0 "#PWR03" H 3400 5300 50  0001 C CNN
F 1 "GND" H 3405 5377 50  0000 C CNN
F 2 "" H 3400 5550 50  0001 C CNN
F 3 "" H 3400 5550 50  0001 C CNN
	1    3400 5550
	1    0    0    -1  
$EndComp
Connection ~ 4050 5250
Wire Wire Line
	4050 5550 5350 5550
Wire Wire Line
	5350 5550 5350 4850
Wire Wire Line
	5350 4850 5650 4850
Connection ~ 4050 5550
$Comp
L Device:Q_NPN_BEC Q1
U 1 1 6123D76E
P 7700 4650
F 0 "Q1" H 7890 4696 50  0000 L CNN
F 1 "SS8050-G" H 7890 4605 50  0000 L CNN
F 2 "footprints:SS8050-G" H 7900 4750 50  0001 C CNN
F 3 "https://www.digikey.com/en/htmldatasheets/production/1953165/0/0/1/ss8050-g.html" H 7700 4650 50  0001 C CNN
	1    7700 4650
	1    0    0    -1  
$EndComp
$Comp
L Device:Q_NPN_BEC Q2
U 1 1 6123EBF4
P 7700 5250
F 0 "Q2" H 7890 5204 50  0000 L CNN
F 1 "SS8050-G" H 7890 5295 50  0000 L CNN
F 2 "footprints:SS8050-G" H 7900 5350 50  0001 C CNN
F 3 "https://www.digikey.com/en/htmldatasheets/production/1953165/0/0/1/ss8050-g.html" H 7700 5250 50  0001 C CNN
	1    7700 5250
	1    0    0    1   
$EndComp
$Comp
L Device:R_Small_US R?
U 1 1 612479FA
P 7400 4650
AR Path="/612479FA" Ref="R?"  Part="1" 
AR Path="/61217E38/612479FA" Ref="R23"  Part="1" 
F 0 "R23" V 7700 4700 50  0000 L CNN
F 1 "10 KΩ" V 7600 4550 50  0000 L CNN
F 2 "footprints:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder_FullSilk" H 7400 4650 50  0001 C CNN
F 3 "~" H 7400 4650 50  0001 C CNN
F 4 "5%" V 7500 4650 50  0000 L CNN "Tolerance"
	1    7400 4650
	0    -1   -1   0   
$EndComp
$Comp
L Device:R_Small_US R?
U 1 1 6124AF8B
P 7400 5250
AR Path="/6124AF8B" Ref="R?"  Part="1" 
AR Path="/61217E38/6124AF8B" Ref="R24"  Part="1" 
F 0 "R24" V 7300 5250 50  0000 L CNN
F 1 "10 KΩ" V 7200 5100 50  0000 L CNN
F 2 "footprints:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder_FullSilk" H 7400 5250 50  0001 C CNN
F 3 "~" H 7400 5250 50  0001 C CNN
F 4 "5%" V 7100 5200 50  0000 L CNN "Tolerance"
	1    7400 5250
	0    -1   -1   0   
$EndComp
Wire Wire Line
	6450 4900 6850 4900
Wire Wire Line
	6850 4900 6850 4650
Wire Wire Line
	6850 4650 7300 4650
Wire Wire Line
	6450 5000 6850 5000
Wire Wire Line
	6850 5000 6850 5250
Wire Wire Line
	6850 5250 7200 5250
Wire Wire Line
	7200 5250 7200 4850
Wire Wire Line
	7200 4850 7800 4850
Connection ~ 7200 5250
Wire Wire Line
	7200 5250 7300 5250
Wire Wire Line
	7300 4650 7300 5050
Wire Wire Line
	7300 5050 7800 5050
Connection ~ 7300 4650
Text Label 6450 2900 0    50   ~ 0
OpAmpEN
Text Label 7950 5450 0    50   ~ 0
SD_CS
Wire Wire Line
	7950 5450 7800 5450
Wire Wire Line
	7800 4450 7950 4450
Text Label 7950 4450 0    50   ~ 0
RST
Text Label 5650 2400 2    50   ~ 0
RST
Text Label 6450 2700 0    50   ~ 0
Tx0
Text Label 6800 4300 0    50   ~ 0
Tx0
$Comp
L Device:R_Small_US R?
U 1 1 6125BD6A
P 6550 4200
AR Path="/6125BD6A" Ref="R?"  Part="1" 
AR Path="/61217E38/6125BD6A" Ref="R20"  Part="1" 
F 0 "R20" V 6850 4250 50  0000 L CNN
F 1 "470 Ω" V 6750 4100 50  0000 L CNN
F 2 "footprints:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder_FullSilk" H 6550 4200 50  0001 C CNN
F 3 "~" H 6550 4200 50  0001 C CNN
F 4 "5%" V 6650 4200 50  0000 L CNN "Tolerance"
	1    6550 4200
	0    -1   -1   0   
$EndComp
$Comp
L Device:R_Small_US R?
U 1 1 6125D445
P 6700 4300
AR Path="/6125D445" Ref="R?"  Part="1" 
AR Path="/61217E38/6125D445" Ref="R21"  Part="1" 
F 0 "R21" V 6600 4300 50  0000 L CNN
F 1 "470 Ω" V 6500 4150 50  0000 L CNN
F 2 "footprints:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder_FullSilk" H 6700 4300 50  0001 C CNN
F 3 "~" H 6700 4300 50  0001 C CNN
F 4 "5%" V 6400 4250 50  0000 L CNN "Tolerance"
	1    6700 4300
	0    -1   -1   0   
$EndComp
Wire Wire Line
	6450 4300 6600 4300
Text Label 6650 4200 0    50   ~ 0
Rx0
Text HLabel 3300 3100 0    50   Input ~ 0
Vcc
Text HLabel 3300 3200 0    50   Input ~ 0
GND
Text HLabel 3300 3400 0    50   Input ~ 0
Rx
Text HLabel 3300 3500 0    50   Input ~ 0
Tx
Text HLabel 3300 3600 0    50   Input ~ 0
SDA
Text HLabel 3300 3700 0    50   Input ~ 0
SCL
Text HLabel 3300 3800 0    50   Input ~ 0
MISO
Text HLabel 3300 3900 0    50   Input ~ 0
MOSI
Text HLabel 3300 4100 0    50   Input ~ 0
SD_CS
Text HLabel 3300 4200 0    50   Input ~ 0
Cam_CS
$Comp
L power:GND #PWR?
U 1 1 6126410D
P 5400 1850
AR Path="/6126410D" Ref="#PWR?"  Part="1" 
AR Path="/611BA1F2/6126410D" Ref="#PWR?"  Part="1" 
AR Path="/61217E38/6126410D" Ref="#PWR010"  Part="1" 
F 0 "#PWR010" H 5400 1600 50  0001 C CNN
F 1 "GND" H 5405 1677 50  0000 C CNN
F 2 "" H 5400 1850 50  0001 C CNN
F 3 "" H 5400 1850 50  0001 C CNN
	1    5400 1850
	1    0    0    -1  
$EndComp
Wire Wire Line
	5650 2400 5250 2400
Wire Wire Line
	5250 2400 5250 1600
Wire Wire Line
	5250 1600 5400 1600
Wire Wire Line
	5400 1600 5400 1550
Wire Wire Line
	5400 1600 5400 1650
Connection ~ 5400 1600
$Comp
L Device:C_Small C?
U 1 1 61264107
P 5400 1750
AR Path="/61264107" Ref="C?"  Part="1" 
AR Path="/61217E38/61264107" Ref="C8"  Part="1" 
F 0 "C8" H 5492 1796 50  0000 L CNN
F 1 "0.1 uF" H 5492 1705 50  0000 L CNN
F 2 "footprints:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder_FullSilk" H 5400 1750 50  0001 C CNN
F 3 "~" H 5400 1750 50  0001 C CNN
	1    5400 1750
	1    0    0    -1  
$EndComp
Text Label 3300 3100 0    50   ~ 0
Vcc
Text Label 3300 3200 0    50   ~ 0
GND
Text Label 3300 3500 0    50   ~ 0
Tx0
Text Label 3300 3400 0    50   ~ 0
Rx0
Text Label 3300 3600 0    50   ~ 0
SDA
Text Label 3300 3700 0    50   ~ 0
SCL
Text Label 3300 3800 0    50   ~ 0
MISO
Text Label 3300 3900 0    50   ~ 0
MOSI
Text Label 3300 4100 0    50   ~ 0
SD_CS
Text Label 3300 4200 0    50   ~ 0
Cam_CS
Text Label 5650 3100 2    50   ~ 0
MISO
Text HLabel 3300 4000 0    50   Input ~ 0
SCK
Text Label 3300 4000 0    50   ~ 0
SCK
$Comp
L power:GND #PWR?
U 1 1 6128025D
P 7050 2600
AR Path="/6128025D" Ref="#PWR?"  Part="1" 
AR Path="/611BA1F2/6128025D" Ref="#PWR?"  Part="1" 
AR Path="/61217E38/6128025D" Ref="#PWR013"  Part="1" 
F 0 "#PWR013" H 7050 2350 50  0001 C CNN
F 1 "GND" H 7055 2427 50  0000 C CNN
F 2 "" H 7050 2600 50  0001 C CNN
F 3 "" H 7050 2600 50  0001 C CNN
	1    7050 2600
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C?
U 1 1 61280263
P 7050 2500
AR Path="/61280263" Ref="C?"  Part="1" 
AR Path="/61217E38/61280263" Ref="C10"  Part="1" 
F 0 "C10" H 7142 2546 50  0000 L CNN
F 1 "0.1 uF" H 7142 2455 50  0000 L CNN
F 2 "footprints:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder_FullSilk" H 7050 2500 50  0001 C CNN
F 3 "~" H 7050 2500 50  0001 C CNN
	1    7050 2500
	1    0    0    -1  
$EndComp
Wire Wire Line
	6450 2400 7050 2400
Text HLabel 3300 4300 0    50   Input ~ 0
ADC0
Text Label 3300 4300 0    50   ~ 0
ADC0
Text HLabel 3300 4400 0    50   Input ~ 0
OpAmpEN
Text Label 5650 2700 2    50   ~ 0
GPIO2
Text Label 3300 4400 0    50   ~ 0
OpAmpEN
$Comp
L SailCam_MK5-rescue:AOD1N60-AOD1N60 Q3
U 1 1 612A588A
P 10050 4450
F 0 "Q3" H 10258 4541 50  0000 L CNN
F 1 "AOD1N60" H 10258 4450 50  0000 L CNN
F 2 "custom:TO228P1003X240-3N" H 10050 4450 50  0001 L BNN
F 3 "" H 10050 4450 50  0001 L BNN
F 4 "IPC 7351B" H 10050 4450 50  0001 L BNN "STANDARD"
F 5 "2.4 mm" H 10050 4450 50  0001 L BNN "MAXIMUM_PACKAGE_HEIGHT"
F 6 "Alpha & Omega" H 10050 4450 50  0001 L BNN "MANUFACTURER"
F 7 "U" H 10050 4450 50  0001 L BNN "PARTREV"
F 8 "IR Flash Control" H 10258 4359 50  0000 L CNN "Description"
	1    10050 4450
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small_US R?
U 1 1 612A9D0D
P 9800 4550
AR Path="/612A9D0D" Ref="R?"  Part="1" 
AR Path="/61217E38/612A9D0D" Ref="R25"  Part="1" 
F 0 "R25" V 10100 4600 50  0000 L CNN
F 1 "1 KΩ" V 10000 4500 50  0000 L CNN
F 2 "footprints:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder_FullSilk" H 9800 4550 50  0001 C CNN
F 3 "~" H 9800 4550 50  0001 C CNN
F 4 "5%" V 9900 4550 50  0000 L CNN "Tolerance"
	1    9800 4550
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR?
U 1 1 612AAE62
P 10150 4650
AR Path="/612AAE62" Ref="#PWR?"  Part="1" 
AR Path="/611BA1F2/612AAE62" Ref="#PWR?"  Part="1" 
AR Path="/61217E38/612AAE62" Ref="#PWR018"  Part="1" 
F 0 "#PWR018" H 10150 4400 50  0001 C CNN
F 1 "GND" H 10155 4477 50  0000 C CNN
F 2 "" H 10150 4650 50  0001 C CNN
F 3 "" H 10150 4650 50  0001 C CNN
	1    10150 4650
	1    0    0    -1  
$EndComp
Text Label 9700 4550 2    50   ~ 0
GPIO2
Text HLabel 3300 4500 0    50   Input ~ 0
IR_CTL
Text Label 3300 4500 0    50   ~ 0
IR_CTL
Text Label 10150 4050 2    50   ~ 0
IR_CTL
$Comp
L Device:C_Small C?
U 1 1 612B2E67
P 8950 4500
AR Path="/612B2E67" Ref="C?"  Part="1" 
AR Path="/61217E38/612B2E67" Ref="C11"  Part="1" 
F 0 "C11" H 9042 4546 50  0000 L CNN
F 1 "100 uF" H 9042 4455 50  0000 L CNN
F 2 "Capacitor_THT:CP_Radial_D5.0mm_P2.00mm" H 8950 4500 50  0001 C CNN
F 3 "~" H 8950 4500 50  0001 C CNN
	1    8950 4500
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 612B2E6D
P 8950 4600
AR Path="/612B2E6D" Ref="#PWR?"  Part="1" 
AR Path="/611BA1F2/612B2E6D" Ref="#PWR?"  Part="1" 
AR Path="/61217E38/612B2E6D" Ref="#PWR016"  Part="1" 
F 0 "#PWR016" H 8950 4350 50  0001 C CNN
F 1 "GND" H 8955 4427 50  0000 C CNN
F 2 "" H 8950 4600 50  0001 C CNN
F 3 "" H 8950 4600 50  0001 C CNN
	1    8950 4600
	1    0    0    -1  
$EndComp
Text Label 8950 4400 0    50   ~ 0
Vcc_5v
Text HLabel 3300 4600 0    50   Input ~ 0
Vcc_5v
Text Label 3300 4600 0    50   ~ 0
Vcc_5v
$Comp
L Device:R_Small_US R?
U 1 1 612BDB8C
P 10150 4150
AR Path="/612BDB8C" Ref="R?"  Part="1" 
AR Path="/61217E38/612BDB8C" Ref="R26"  Part="1" 
F 0 "R26" H 10218 4241 50  0000 L CNN
F 1 "10 KΩ" H 10218 4150 50  0000 L CNN
F 2 "Resistor_THT:R_Box_L8.4mm_W2.5mm_P5.08mm" H 10150 4150 50  0001 C CNN
F 3 "~" H 10150 4150 50  0001 C CNN
F 4 "5%" H 10218 4059 50  0000 L CNN "Tolerance"
	1    10150 4150
	1    0    0    -1  
$EndComp
Wire Wire Line
	9900 4550 9950 4550
Text HLabel 3300 4700 0    50   Input ~ 0
Bat_Chg_Vcc
Text Label 3300 4700 0    50   ~ 0
Bat_Chg_Vcc
$Comp
L SailCam:USB_B_Micro J7
U 1 1 614CF04D
P 4950 4550
F 0 "J7" H 5007 5017 50  0000 C CNN
F 1 "USB_B_Mini" H 5007 4926 50  0000 C CNN
F 2 "digikey-footprints:USB_Mini_B_Female_548190519" H 5100 4500 50  0001 C CNN
F 3 "~" H 5100 4500 50  0001 C CNN
	1    4950 4550
	1    0    0    -1  
$EndComp
Wire Wire Line
	5650 4750 5300 4750
Wire Wire Line
	5300 4750 5300 5250
Wire Wire Line
	4050 5250 5300 5250
NoConn ~ 5250 4750
Wire Wire Line
	5250 4650 5650 4650
Wire Wire Line
	5250 4550 5650 4550
Wire Wire Line
	5450 4200 5450 5600
Text Label 5650 4200 2    50   ~ 0
Vcc
Wire Wire Line
	5650 4300 5650 4200
Connection ~ 5650 4200
$EndSCHEMATC
