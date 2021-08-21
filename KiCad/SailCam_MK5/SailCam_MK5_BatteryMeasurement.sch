EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 2 7
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
L ADA4806-1ARJZ-R7:ADA4806-1ARJZ-R7 U?
U 1 1 6118AE4B
P 5100 3700
AR Path="/6118AE4B" Ref="U?"  Part="1" 
AR Path="/611821E5/6118AE4B" Ref="U?"  Part="1" 
AR Path="/61199C7C/6118AE4B" Ref="U1"  Part="1" 
F 0 "U1" H 5800 4177 60  0000 C CNN
F 1 "ADA4806-1ARJZ-R7" H 5800 4071 60  0000 C CNN
F 2 "footprints:ADA4806-1ARJZ-R7" H 6100 3940 60  0001 C CNN
F 3 "https://www.analog.com/media/en/technical-documentation/data-sheets/ADA4806-1.pdf" H 6100 3981 60  0001 C CNN
F 4 "Op Amp" H 5800 3973 50  0000 C CNN "Description"
	1    5100 3700
	1    0    0    -1  
$EndComp
Wire Wire Line
	6500 4000 6700 4000
Wire Wire Line
	6700 4000 6700 3100
Wire Wire Line
	6700 3100 5100 3100
Wire Wire Line
	5100 3100 5100 3700
$Comp
L Device:R_Small_US R?
U 1 1 6118AE59
P 4650 4200
AR Path="/6118AE59" Ref="R?"  Part="1" 
AR Path="/611821E5/6118AE59" Ref="R?"  Part="1" 
AR Path="/61199C7C/6118AE59" Ref="R2"  Part="1" 
F 0 "R2" H 4700 4100 50  0000 L CNN
F 1 "100 KΩ" H 4718 4200 50  0000 L CNN
F 2 "footprints:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder_FullSilk" H 4650 4200 50  0001 C CNN
F 3 "~" H 4650 4200 50  0001 C CNN
F 4 "1%" H 4700 4300 50  0000 L CNN "Tolerance"
	1    4650 4200
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR?
U 1 1 6118AE5F
P 4650 4300
AR Path="/6118AE5F" Ref="#PWR?"  Part="1" 
AR Path="/611821E5/6118AE5F" Ref="#PWR?"  Part="1" 
AR Path="/61199C7C/6118AE5F" Ref="#PWR0101"  Part="1" 
F 0 "#PWR0101" H 4650 4050 50  0001 C CNN
F 1 "GND" H 4655 4127 50  0000 C CNN
F 2 "" H 4650 4300 50  0001 C CNN
F 3 "" H 4650 4300 50  0001 C CNN
	1    4650 4300
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small_US R?
U 1 1 6118AE66
P 4650 3800
AR Path="/6118AE66" Ref="R?"  Part="1" 
AR Path="/611821E5/6118AE66" Ref="R?"  Part="1" 
AR Path="/61199C7C/6118AE66" Ref="R1"  Part="1" 
F 0 "R1" H 4500 3950 50  0000 L CNN
F 1 "320 KΩ" H 4350 3850 50  0000 L CNN
F 2 "footprints:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder_FullSilk" H 4650 3800 50  0001 C CNN
F 3 "~" H 4650 3800 50  0001 C CNN
F 4 "1%" H 4500 3750 50  0000 L CNN "Tolerance"
	1    4650 3800
	1    0    0    -1  
$EndComp
Wire Wire Line
	5100 4000 4650 4000
Wire Wire Line
	4650 4000 4650 3900
Wire Wire Line
	4650 4000 4650 4100
Connection ~ 4650 4000
$Comp
L Device:R_Small_US R?
U 1 1 6118AE77
P 7300 4200
AR Path="/6118AE77" Ref="R?"  Part="1" 
AR Path="/611821E5/6118AE77" Ref="R?"  Part="1" 
AR Path="/61199C7C/6118AE77" Ref="R4"  Part="1" 
F 0 "R4" H 7350 4100 50  0000 L CNN
F 1 "100 KΩ" H 7368 4200 50  0000 L CNN
F 2 "footprints:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder_FullSilk" H 7300 4200 50  0001 C CNN
F 3 "~" H 7300 4200 50  0001 C CNN
F 4 "1%" H 7350 4300 50  0000 L CNN "Tolerance"
	1    7300 4200
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR?
U 1 1 6118AE7D
P 7300 4300
AR Path="/6118AE7D" Ref="#PWR?"  Part="1" 
AR Path="/611821E5/6118AE7D" Ref="#PWR?"  Part="1" 
AR Path="/61199C7C/6118AE7D" Ref="#PWR0102"  Part="1" 
F 0 "#PWR0102" H 7300 4050 50  0001 C CNN
F 1 "GND" H 7305 4127 50  0000 C CNN
F 2 "" H 7300 4300 50  0001 C CNN
F 3 "" H 7300 4300 50  0001 C CNN
	1    7300 4300
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small_US R?
U 1 1 6118AE84
P 7300 3800
AR Path="/6118AE84" Ref="R?"  Part="1" 
AR Path="/611821E5/6118AE84" Ref="R?"  Part="1" 
AR Path="/61199C7C/6118AE84" Ref="R3"  Part="1" 
F 0 "R3" H 7150 3950 50  0000 L CNN
F 1 "320 KΩ" H 7000 3850 50  0000 L CNN
F 2 "footprints:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder_FullSilk" H 7300 3800 50  0001 C CNN
F 3 "~" H 7300 3800 50  0001 C CNN
F 4 "1%" H 7150 3750 50  0000 L CNN "Tolerance"
	1    7300 3800
	1    0    0    -1  
$EndComp
Wire Wire Line
	7300 3900 7300 4000
Wire Wire Line
	6700 4000 7300 4000
Connection ~ 6700 4000
Connection ~ 7300 4000
Wire Wire Line
	7300 4000 7300 4100
$Comp
L power:VCC #PWR?
U 1 1 6118AE6C
P 7300 3700
AR Path="/6118AE6C" Ref="#PWR?"  Part="1" 
AR Path="/611821E5/6118AE6C" Ref="#PWR?"  Part="1" 
AR Path="/61199C7C/6118AE6C" Ref="#PWR0103"  Part="1" 
F 0 "#PWR0103" H 7300 3550 50  0001 C CNN
F 1 "VCC" H 7317 3873 50  0000 C CNN
F 2 "" H 7300 3700 50  0001 C CNN
F 3 "" H 7300 3700 50  0001 C CNN
	1    7300 3700
	1    0    0    -1  
$EndComp
Text Label 6500 3900 0    50   ~ 0
Vcc
Text Label 6500 3700 0    50   ~ 0
Vcc
Text Label 5100 3900 2    50   ~ 0
GND
Text HLabel 3250 3100 0    50   Input ~ 0
Vcc_Bat
Text Label 4650 3700 0    50   ~ 0
Vcc_Bat
Text HLabel 3250 3400 0    50   Input ~ 0
GND
Text HLabel 3250 3000 0    50   Input ~ 0
Vcc
Text HLabel 3250 3200 0    50   Input ~ 0
EN
Text Label 6500 3800 0    50   ~ 0
EN
Text Label 3250 3200 0    50   ~ 0
EN
Text Label 3250 3100 0    50   ~ 0
Vcc_Bat
$Comp
L power:GND #PWR?
U 1 1 61193403
P 3250 3400
AR Path="/61193403" Ref="#PWR?"  Part="1" 
AR Path="/611821E5/61193403" Ref="#PWR?"  Part="1" 
AR Path="/61199C7C/61193403" Ref="#PWR0104"  Part="1" 
F 0 "#PWR0104" H 3250 3150 50  0001 C CNN
F 1 "GND" H 3255 3227 50  0000 C CNN
F 2 "" H 3250 3400 50  0001 C CNN
F 3 "" H 3250 3400 50  0001 C CNN
	1    3250 3400
	1    0    0    -1  
$EndComp
$Comp
L power:VCC #PWR?
U 1 1 61193F38
P 3250 3000
AR Path="/61193F38" Ref="#PWR?"  Part="1" 
AR Path="/611821E5/61193F38" Ref="#PWR?"  Part="1" 
AR Path="/61199C7C/61193F38" Ref="#PWR0105"  Part="1" 
F 0 "#PWR0105" H 3250 2850 50  0001 C CNN
F 1 "VCC" H 3267 3173 50  0000 C CNN
F 2 "" H 3250 3000 50  0001 C CNN
F 3 "" H 3250 3000 50  0001 C CNN
	1    3250 3000
	1    0    0    -1  
$EndComp
Text Label 3250 3000 0    50   ~ 0
Vcc
Text Label 3250 3400 0    50   ~ 0
GND
Text Label 6500 4000 0    50   ~ 0
OUT
Text Label 3250 3300 0    50   ~ 0
OUT
Text HLabel 3250 3300 0    50   Input ~ 0
OUT
$EndSCHEMATC
