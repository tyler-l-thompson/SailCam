EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 4 7
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
L SailCam_MK5-rescue:XC6220B331MR-G-XC6220B331MR-G U?
U 1 1 611C1252
P 6150 3100
AR Path="/611C1252" Ref="U?"  Part="1" 
AR Path="/611BA1F2/611C1252" Ref="U5"  Part="1" 
F 0 "U5" H 6950 3577 60  0000 C CNN
F 1 "XC6220B331MR-G" H 6950 3471 60  0000 C CNN
F 2 "footprints:XC6220B331MR-G" H 6950 3340 60  0001 C CNN
F 3 "https://media.digikey.com/pdf/Data%20Sheets/Torex/XC6220.pdf" H 6150 3100 60  0001 C CNN
F 4 "3.3v DC Regulator" H 6950 3373 50  0000 C CNN "Description"
	1    6150 3100
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C?
U 1 1 611C1259
P 8050 3200
AR Path="/611C1259" Ref="C?"  Part="1" 
AR Path="/611BA1F2/611C1259" Ref="C4"  Part="1" 
F 0 "C4" H 8142 3246 50  0000 L CNN
F 1 "10 uF" H 8142 3155 50  0000 L CNN
F 2 "footprints:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder_FullSilk" H 8050 3200 50  0001 C CNN
F 3 "~" H 8050 3200 50  0001 C CNN
	1    8050 3200
	1    0    0    -1  
$EndComp
$Comp
L power:VCC #PWR?
U 1 1 611C125F
P 8050 3100
AR Path="/611C125F" Ref="#PWR?"  Part="1" 
AR Path="/611BA1F2/611C125F" Ref="#PWR0109"  Part="1" 
F 0 "#PWR0109" H 8050 2950 50  0001 C CNN
F 1 "VCC" H 8067 3273 50  0000 C CNN
F 2 "" H 8050 3100 50  0001 C CNN
F 3 "" H 8050 3100 50  0001 C CNN
	1    8050 3100
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 611C1265
P 8050 3300
AR Path="/611C1265" Ref="#PWR?"  Part="1" 
AR Path="/611BA1F2/611C1265" Ref="#PWR0110"  Part="1" 
F 0 "#PWR0110" H 8050 3050 50  0001 C CNN
F 1 "GND" H 8055 3127 50  0000 C CNN
F 2 "" H 8050 3300 50  0001 C CNN
F 3 "" H 8050 3300 50  0001 C CNN
	1    8050 3300
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C?
U 1 1 611C126B
P 5600 4200
AR Path="/611C126B" Ref="C?"  Part="1" 
AR Path="/611BA1F2/611C126B" Ref="C2"  Part="1" 
F 0 "C2" H 5692 4246 50  0000 L CNN
F 1 "1 uF" H 5692 4155 50  0000 L CNN
F 2 "footprints:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder_FullSilk" H 5600 4200 50  0001 C CNN
F 3 "~" H 5600 4200 50  0001 C CNN
	1    5600 4200
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 611C1271
P 5600 4300
AR Path="/611C1271" Ref="#PWR?"  Part="1" 
AR Path="/611BA1F2/611C1271" Ref="#PWR0111"  Part="1" 
F 0 "#PWR0111" H 5600 4050 50  0001 C CNN
F 1 "GND" H 5605 4127 50  0000 C CNN
F 2 "" H 5600 4300 50  0001 C CNN
F 3 "" H 5600 4300 50  0001 C CNN
	1    5600 4300
	1    0    0    -1  
$EndComp
Wire Wire Line
	6150 3400 6150 3100
Wire Wire Line
	7750 3100 8050 3100
Connection ~ 8050 3100
$Comp
L SailCam_MK5-rescue:LTC4054ES5-4.2#TRMPBF-LTC4054ES5-4.2_TRMPBF U?
U 1 1 611C1280
P 4600 4400
AR Path="/611C1280" Ref="U?"  Part="1" 
AR Path="/611BA1F2/611C1280" Ref="U4"  Part="1" 
F 0 "U4" H 4600 5060 50  0000 C CNN
F 1 "LTC4054ES5-4.2#TRMPBF" H 4600 4969 50  0000 C CNN
F 2 "footprints:SOT95P280X100-5N" H 4600 4400 50  0001 L BNN
F 3 "https://www.analog.com/media/en/technical-documentation/data-sheets/405442xf.pdf" H 4600 4400 50  0001 L BNN
F 4 "Manufacturer Recommendations" H 4600 4400 50  0001 L BNN "STANDARD"
F 5 "LTC4054ES5-4.2#TRMPBF" H 4600 4400 50  0001 L BNN "MP"
F 6 "None" H 4600 4400 50  0001 L BNN "PRICE"
F 7 "SOT-23-5 Linear Technology" H 4600 4400 50  0001 L BNN "PACKAGE"
F 8 "Linear Li-Ion Battery Charger" H 4600 4878 50  0000 C CNN "DESCRIPTION"
F 9 "Linear Technology/Analog" H 4600 4400 50  0001 L BNN "MANUFACTURER"
	1    4600 4400
	1    0    0    -1  
$EndComp
Wire Wire Line
	5300 4100 5600 4100
Text Label 3900 4500 2    50   ~ 0
Vcc_Bat
$Comp
L Device:LED D?
U 1 1 611C1289
P 3650 3850
AR Path="/611C1289" Ref="D?"  Part="1" 
AR Path="/611BA1F2/611C1289" Ref="D1"  Part="1" 
F 0 "D1" V 3750 3950 50  0000 C CNN
F 1 "LED" V 3643 3975 50  0000 C CNN
F 2 "LED_THT:LED_D3.0mm" H 3650 3850 50  0001 C CNN
F 3 "~" H 3650 3850 50  0001 C CNN
	1    3650 3850
	0    -1   -1   0   
$EndComp
$Comp
L Device:R_Small_US R?
U 1 1 611C1290
P 3650 4200
AR Path="/611C1290" Ref="R?"  Part="1" 
AR Path="/611BA1F2/611C1290" Ref="R8"  Part="1" 
F 0 "R8" H 3700 4100 50  0000 L CNN
F 1 "1k Ω" H 3718 4200 50  0000 L CNN
F 2 "footprints:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder_FullSilk" H 3650 4200 50  0001 C CNN
F 3 "~" H 3650 4200 50  0001 C CNN
F 4 "5%" H 3700 4300 50  0000 L CNN "Tolerance"
	1    3650 4200
	-1   0    0    1   
$EndComp
Wire Wire Line
	3650 4000 3650 4100
Wire Wire Line
	3650 4300 3900 4300
$Comp
L power:VCC #PWR?
U 1 1 611C1298
P 3650 3700
AR Path="/611C1298" Ref="#PWR?"  Part="1" 
AR Path="/611BA1F2/611C1298" Ref="#PWR0112"  Part="1" 
F 0 "#PWR0112" H 3650 3550 50  0001 C CNN
F 1 "VCC" H 3667 3873 50  0000 C CNN
F 2 "" H 3650 3700 50  0001 C CNN
F 3 "" H 3650 3700 50  0001 C CNN
	1    3650 3700
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small_US R?
U 1 1 611C129F
P 3650 4700
AR Path="/611C129F" Ref="R?"  Part="1" 
AR Path="/611BA1F2/611C129F" Ref="R9"  Part="1" 
F 0 "R9" H 3700 4600 50  0000 L CNN
F 1 "3 KΩ" H 3718 4700 50  0000 L CNN
F 2 "footprints:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder_FullSilk" H 3650 4700 50  0001 C CNN
F 3 "~" H 3650 4700 50  0001 C CNN
F 4 "1%" H 3700 4800 50  0000 L CNN "Tolerance"
	1    3650 4700
	-1   0    0    1   
$EndComp
Wire Wire Line
	3900 4600 3650 4600
$Comp
L power:GND #PWR?
U 1 1 611C12A6
P 3650 4800
AR Path="/611C12A6" Ref="#PWR?"  Part="1" 
AR Path="/611BA1F2/611C12A6" Ref="#PWR0113"  Part="1" 
F 0 "#PWR0113" H 3650 4550 50  0001 C CNN
F 1 "GND" H 3655 4627 50  0000 C CNN
F 2 "" H 3650 4800 50  0001 C CNN
F 3 "" H 3650 4800 50  0001 C CNN
	1    3650 4800
	1    0    0    -1  
$EndComp
$Comp
L SailCam_MK5-rescue:NCV7805BD2TG-NCV7805BD2TG U?
U 1 1 611C12B3
P 3800 3100
AR Path="/611C12B3" Ref="U?"  Part="1" 
AR Path="/611BA1F2/611C12B3" Ref="U3"  Part="1" 
F 0 "U3" H 4600 3577 60  0000 C CNN
F 1 "NCV7805BD2TG" H 4600 3471 60  0000 C CNN
F 2 "footprints:NCV7805BD2TG" H 4600 3340 60  0001 C CNN
F 3 "https://www.onsemi.com/pdf/datasheet/mc7800-d.pdf" H 3800 3100 60  0001 C CNN
F 4 "5v DC Regulator" H 4600 3373 50  0000 C CNN "Description"
	1    3800 3100
	1    0    0    -1  
$EndComp
Wire Wire Line
	5400 3100 5600 3100
Wire Wire Line
	5600 3100 5600 4100
$Comp
L Device:C_Small C?
U 1 1 611C12BC
P 3800 3200
AR Path="/611C12BC" Ref="C?"  Part="1" 
AR Path="/611BA1F2/611C12BC" Ref="C1"  Part="1" 
F 0 "C1" H 3892 3246 50  0000 L CNN
F 1 "0.33 uF" H 3892 3155 50  0000 L CNN
F 2 "footprints:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder_FullSilk" H 3800 3200 50  0001 C CNN
F 3 "~" H 3800 3200 50  0001 C CNN
	1    3800 3200
	1    0    0    -1  
$EndComp
Connection ~ 5600 4100
Text Label 3800 3100 2    50   ~ 0
CHG
Text Label 6150 2850 0    50   ~ 0
Vcc_5v
$Comp
L Device:C_Small C?
U 1 1 611C12CF
P 6150 3500
AR Path="/611C12CF" Ref="C?"  Part="1" 
AR Path="/611BA1F2/611C12CF" Ref="C3"  Part="1" 
F 0 "C3" H 6250 3450 50  0000 L CNN
F 1 "10 uF" H 6250 3550 50  0000 L CNN
F 2 "footprints:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder_FullSilk" H 6150 3500 50  0001 C CNN
F 3 "~" H 6150 3500 50  0001 C CNN
	1    6150 3500
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR?
U 1 1 611C12D5
P 6150 3600
AR Path="/611C12D5" Ref="#PWR?"  Part="1" 
AR Path="/611BA1F2/611C12D5" Ref="#PWR0115"  Part="1" 
F 0 "#PWR0115" H 6150 3350 50  0001 C CNN
F 1 "GND" H 6155 3427 50  0000 C CNN
F 2 "" H 6150 3600 50  0001 C CNN
F 3 "" H 6150 3600 50  0001 C CNN
	1    6150 3600
	1    0    0    -1  
$EndComp
Connection ~ 6150 3400
$Comp
L power:GND #PWR?
U 1 1 611C1FFB
P 6950 4000
AR Path="/611C1FFB" Ref="#PWR?"  Part="1" 
AR Path="/611BA1F2/611C1FFB" Ref="#PWR0116"  Part="1" 
F 0 "#PWR0116" H 6950 3750 50  0001 C CNN
F 1 "GND" H 6955 3827 50  0000 C CNN
F 2 "" H 6950 4000 50  0001 C CNN
F 3 "" H 6950 4000 50  0001 C CNN
	1    6950 4000
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 611C2589
P 5300 4800
AR Path="/611C2589" Ref="#PWR?"  Part="1" 
AR Path="/611BA1F2/611C2589" Ref="#PWR0117"  Part="1" 
F 0 "#PWR0117" H 5300 4550 50  0001 C CNN
F 1 "GND" H 5305 4627 50  0000 C CNN
F 2 "" H 5300 4800 50  0001 C CNN
F 3 "" H 5300 4800 50  0001 C CNN
	1    5300 4800
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 611C27EF
P 5400 3200
AR Path="/611C27EF" Ref="#PWR?"  Part="1" 
AR Path="/611BA1F2/611C27EF" Ref="#PWR0118"  Part="1" 
F 0 "#PWR0118" H 5400 2950 50  0001 C CNN
F 1 "GND" H 5405 3027 50  0000 C CNN
F 2 "" H 5400 3200 50  0001 C CNN
F 3 "" H 5400 3200 50  0001 C CNN
	1    5400 3200
	1    0    0    -1  
$EndComp
Text HLabel 1400 1400 0    50   Input ~ 0
Vcc
Text HLabel 1400 1500 0    50   Input ~ 0
GND
$Comp
L power:GND #PWR?
U 1 1 611C3E21
P 1400 1500
AR Path="/611C3E21" Ref="#PWR?"  Part="1" 
AR Path="/611BA1F2/611C3E21" Ref="#PWR0119"  Part="1" 
F 0 "#PWR0119" H 1400 1250 50  0001 C CNN
F 1 "GND" H 1405 1327 50  0000 C CNN
F 2 "" H 1400 1500 50  0001 C CNN
F 3 "" H 1400 1500 50  0001 C CNN
	1    1400 1500
	1    0    0    -1  
$EndComp
$Comp
L power:VCC #PWR?
U 1 1 611C4DC9
P 1400 1400
AR Path="/611C4DC9" Ref="#PWR?"  Part="1" 
AR Path="/611BA1F2/611C4DC9" Ref="#PWR0120"  Part="1" 
F 0 "#PWR0120" H 1400 1250 50  0001 C CNN
F 1 "VCC" H 1417 1573 50  0000 C CNN
F 2 "" H 1400 1400 50  0001 C CNN
F 3 "" H 1400 1400 50  0001 C CNN
	1    1400 1400
	1    0    0    -1  
$EndComp
Text HLabel 2000 1400 0    50   Input ~ 0
Vcc_5v
Text HLabel 2000 1500 0    50   Input ~ 0
CHG
Text HLabel 2000 1600 0    50   Input ~ 0
Vcc_Bat
Text Label 2000 1400 0    50   ~ 0
Vcc_5v
Text Label 2000 1500 0    50   ~ 0
CHG
Text Label 2000 1600 0    50   ~ 0
Vcc_Bat
Text HLabel 2000 1300 0    50   Input ~ 0
Vcc_5v_S
Text Label 5750 2850 2    50   ~ 0
Vcc_5v_S
Text Label 2000 1300 0    50   ~ 0
Vcc_5v_S
$Comp
L Switch:SW_SPST S1
U 1 1 61287042
P 5950 2850
F 0 "S1" H 5950 3175 50  0000 C CNN
F 1 "SW_SPST" H 5950 3084 50  0000 C CNN
F 2 "digikey-footprints:Switch_Toggle_ATE1D-2M3-10-Z" H 5950 2850 50  0001 C CNN
F 3 "~" H 5950 2850 50  0001 C CNN
F 4 "Power On/Off Bypass" H 5950 2993 50  0000 C CNN "Description"
	1    5950 2850
	1    0    0    -1  
$EndComp
Wire Wire Line
	6150 2850 6150 3100
Connection ~ 6150 3100
Wire Wire Line
	3900 4500 3250 4500
Wire Wire Line
	3250 4500 3250 3450
Wire Wire Line
	3250 3450 5750 3450
Wire Wire Line
	5750 3450 5750 2850
Text HLabel 2000 1700 0    50   Input ~ 0
Bat_Chg_Vcc
Text Label 5600 4100 0    50   ~ 0
Bat_Chg_Vcc
Text Label 2000 1700 0    50   ~ 0
Bat_Chg_Vcc
$Comp
L power:GND #PWR?
U 1 1 6153E2D4
P 3800 3300
F 0 "#PWR?" H 3800 3050 50  0001 C CNN
F 1 "GND" H 3805 3127 50  0000 C CNN
F 2 "" H 3800 3300 50  0001 C CNN
F 3 "" H 3800 3300 50  0001 C CNN
	1    3800 3300
	1    0    0    -1  
$EndComp
$EndSCHEMATC
