EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 5 7
Title "SailCam"
Date "2021-07-31"
Rev "MK5"
Comp "Octana Laboratories"
Comment1 "Tyler Thompson"
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text Label 5600 3800 0    50   ~ 0
NEMA+
Text Label 5600 3900 0    50   ~ 0
NEMA-
Text Label 5600 4000 0    50   ~ 0
GND
Text Label 4400 3800 2    50   ~ 0
GND
Text Label 4400 3900 2    50   ~ 0
GND
Text Label 4400 3700 2    50   ~ 0
Rx
$Comp
L Device:R_Small_US R11
U 1 1 611B59E8
P 4050 3400
F 0 "R11" H 3900 3550 50  0000 L CNN
F 1 "10 KΩ" H 3750 3450 50  0000 L CNN
F 2 "footprints:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder_FullSilk" H 4050 3400 50  0001 C CNN
F 3 "~" H 4050 3400 50  0001 C CNN
F 4 "5%" H 3900 3350 50  0000 L CNN "Tolerance"
	1    4050 3400
	1    0    0    -1  
$EndComp
Wire Wire Line
	4050 3500 4050 3700
$Comp
L Device:R_Small_US R12
U 1 1 611B59F6
P 6050 3800
F 0 "R12" V 6350 3750 50  0000 L CNN
F 1 "10 Ω" V 6250 3700 50  0000 L CNN
F 2 "footprints:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder_FullSilk" H 6050 3800 50  0001 C CNN
F 3 "~" H 6050 3800 50  0001 C CNN
F 4 "5%" V 6150 3750 50  0000 L CNN "Tolerance"
	1    6050 3800
	0    -1   -1   0   
$EndComp
$Comp
L Device:R_Small_US R14
U 1 1 611B59FD
P 6350 3900
F 0 "R14" V 6650 3850 50  0000 L CNN
F 1 "10 Ω" V 6550 3800 50  0000 L CNN
F 2 "footprints:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder_FullSilk" H 6350 3900 50  0001 C CNN
F 3 "~" H 6350 3900 50  0001 C CNN
F 4 "5%" V 6450 3850 50  0000 L CNN "Tolerance"
	1    6350 3900
	0    -1   -1   0   
$EndComp
Wire Wire Line
	5600 3800 5950 3800
Wire Wire Line
	6250 3900 5600 3900
$Comp
L Device:R_Small_US R15
U 1 1 611B5A06
P 6600 3700
F 0 "R15" H 6668 3791 50  0000 L CNN
F 1 "330 Ω" H 6668 3700 50  0000 L CNN
F 2 "footprints:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder_FullSilk" H 6600 3700 50  0001 C CNN
F 3 "~" H 6600 3700 50  0001 C CNN
F 4 "5%" H 6668 3609 50  0000 L CNN "Tolerance"
	1    6600 3700
	1    0    0    -1  
$EndComp
Wire Wire Line
	6450 3900 6600 3900
Wire Wire Line
	6600 3900 6600 3800
Wire Wire Line
	6150 3800 6150 4000
$Comp
L SM712-TCT:SM712.TCT U7
U 1 1 611B5A15
P 7000 3900
F 0 "U7" V 7297 4029 60  0000 L CNN
F 1 "SM712.TCT" V 7403 4029 60  0000 L CNN
F 2 "footprints:SM712.TCT" H 6975 3140 60  0001 C CNN
F 3 "https://semtech.my.salesforce.com/sfc/p/#E0000000JelG/a/440000001MKW/f6pBKheIN2CeR0DxWdS3THuJ7nh4aKYltSzghsLP5iw" H 7000 3900 60  0001 C CNN
	1    7000 3900
	0    1    1    0   
$EndComp
Connection ~ 6600 3900
Wire Wire Line
	7000 3900 7000 3250
Wire Wire Line
	7000 3250 6150 3250
Wire Wire Line
	6150 3250 6150 3800
Connection ~ 6150 3800
$Comp
L power:GND #PWR0122
U 1 1 611B5A20
P 6150 4200
F 0 "#PWR0122" H 6150 3950 50  0001 C CNN
F 1 "GND" H 6155 4027 50  0000 C CNN
F 2 "" H 6150 4200 50  0001 C CNN
F 3 "" H 6150 4200 50  0001 C CNN
	1    6150 4200
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small_US R13
U 1 1 611B5A27
P 6150 4100
F 0 "R13" H 6200 4000 50  0000 L CNN
F 1 "330 Ω" H 6218 4100 50  0000 L CNN
F 2 "footprints:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder_FullSilk" H 6150 4100 50  0001 C CNN
F 3 "~" H 6150 4100 50  0001 C CNN
F 4 "5%" H 6200 4200 50  0000 L CNN "Tolerance"
	1    6150 4100
	-1   0    0    1   
$EndComp
$Comp
L MAX485CSA&plus_:MAX485CSA+ U6
U 1 1 611B5A2E
P 4400 3700
F 0 "U6" H 5000 4177 60  0000 C CNN
F 1 "MAX485CSA+" H 5000 4071 60  0000 C CNN
F 2 "footprints:MAX485CSA&plus_" H 5500 3940 60  0001 C CNN
F 3 "https://datasheets.maximintegrated.com/en/ds/MAX1487-MAX491.pdf" H 5000 4071 60  0001 C CNN
F 4 "RS-422/485 Transceiver" H 5000 3973 50  0000 C CNN "Description"
	1    4400 3700
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small_US R10
U 1 1 611B5A35
P 3650 3400
F 0 "R10" H 3500 3550 50  0000 L CNN
F 1 "10 KΩ" H 3350 3450 50  0000 L CNN
F 2 "footprints:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder_FullSilk" H 3650 3400 50  0001 C CNN
F 3 "~" H 3650 3400 50  0001 C CNN
F 4 "5%" H 3500 3350 50  0000 L CNN "Tolerance"
	1    3650 3400
	1    0    0    -1  
$EndComp
Wire Wire Line
	4050 3700 4400 3700
Wire Wire Line
	3650 3500 3650 4000
Wire Wire Line
	3650 4000 4400 4000
Text Label 4400 4000 2    50   ~ 0
Tx
$Comp
L power:GND #PWR0124
U 1 1 611B63B0
P 6800 4600
F 0 "#PWR0124" H 6800 4350 50  0001 C CNN
F 1 "GND" H 6805 4427 50  0000 C CNN
F 2 "" H 6800 4600 50  0001 C CNN
F 3 "" H 6800 4600 50  0001 C CNN
	1    6800 4600
	1    0    0    -1  
$EndComp
Text Label 5600 3700 0    50   ~ 0
Vcc_5v
Text Label 4500 2700 0    50   ~ 0
GND
$Comp
L power:GND #PWR0125
U 1 1 611D1570
P 4500 2700
F 0 "#PWR0125" H 4500 2450 50  0001 C CNN
F 1 "GND" H 4505 2527 50  0000 C CNN
F 2 "" H 4500 2700 50  0001 C CNN
F 3 "" H 4500 2700 50  0001 C CNN
	1    4500 2700
	1    0    0    -1  
$EndComp
Text HLabel 4500 2700 0    50   Input ~ 0
GND
Text HLabel 4500 2550 0    50   Input ~ 0
Vcc_5v
Text Label 4500 2550 0    50   ~ 0
Vcc_5v
Text Label 6600 3500 0    50   ~ 0
Vcc_5v
Wire Wire Line
	6600 3500 6600 3600
Text HLabel 5350 2500 0    50   Input ~ 0
Rx
Text HLabel 5350 2600 0    50   Input ~ 0
Tx
Text HLabel 5350 2700 0    50   Input ~ 0
NEMA+
Text HLabel 5350 2800 0    50   Input ~ 0
NEMA-
Text Label 5350 2500 0    50   ~ 0
Rx
Text Label 5350 2600 0    50   ~ 0
Tx
Text Label 5350 2800 0    50   ~ 0
NEMA-
Text Label 5350 2700 0    50   ~ 0
NEMA+
Text Label 4050 3300 0    50   ~ 0
Vcc_5v
Text Label 3650 3300 0    50   ~ 0
Vcc_5v
$EndSCHEMATC
