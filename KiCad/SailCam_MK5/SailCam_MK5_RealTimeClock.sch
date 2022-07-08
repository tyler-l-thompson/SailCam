EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 3 7
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
L SailCam_MK5-rescue:DS1672S-33+T&R-ds1672 U?
U 1 1 611A5679
P 5200 3800
AR Path="/611A5679" Ref="U?"  Part="1" 
AR Path="/6119E11D/611A5679" Ref="U2"  Part="1" 
F 0 "U2" H 5875 4277 60  0000 C CNN
F 1 "DS1672S-33+T&R" H 5875 4171 60  0000 C CNN
F 2 "footprints:DS1672S-33&plus_T&R" H 6500 4040 60  0001 C CNN
F 3 "https://datasheets.maximintegrated.com/en/ds/DS1672.pdf" H 6500 4081 60  0001 C CNN
F 4 "Real Time Clock" H 5875 4073 50  0000 C CNN "Description"
	1    5200 3800
	1    0    0    -1  
$EndComp
$Comp
L SailCam_MK5-rescue:ABS25-32.768KHZ-6-1-T-ABS25-32.768KHZ-6-1-T Y?
U 1 1 611A5681
P 4900 3800
AR Path="/611A5681" Ref="Y?"  Part="1" 
AR Path="/6119E11D/611A5681" Ref="Y1"  Part="1" 
F 0 "Y1" H 4900 4037 50  0000 C CNN
F 1 "ABS25-32.768KHZ-6-1-T" H 4900 3946 50  0000 C CNN
F 2 "footprints:XTAL_ABS25-32.768KHZ-6-1-T" H 4900 3800 50  0001 L BNN
F 3 "https://abracon.com/Resonators/abs25.pdf" H 4900 3800 50  0001 L BNN
F 4 "ABRACON LLC" H 4900 3800 50  0001 L BNN "MANUFACTURER"
F 5 "19131" H 4900 3800 50  0001 L BNN "SANPEDA_PACKAGE_ID"
	1    4900 3800
	1    0    0    -1  
$EndComp
$Comp
L SailCam_MK5-rescue:BU2032SM-BT-GTR-BU2032SM-BT-GTR B?
U 1 1 611A568F
P 3950 3800
AR Path="/611A568F" Ref="B?"  Part="1" 
AR Path="/6119E11D/611A568F" Ref="B1"  Part="1" 
F 0 "B1" H 3950 4067 50  0000 C CNN
F 1 "BU2032SM-BT-GTR" H 3950 3976 50  0000 C CNN
F 2 "footprints:BAT_BU2032SM-BT-GTR" H 3950 3800 50  0001 L BNN
F 3 "https://media.digikey.com/pdf/Data%20Sheets/Memory%20Protection%20PDFs/Coin_Cell_Guide.pdf" H 3950 3800 50  0001 L BNN
F 4 "BU2032SM-HD-G" H 3950 3800 50  0001 L BNN "MP"
F 5 "5.20mm" H 3950 3800 50  0001 L BNN "MAXIMUM_PACKAGE_HEIGHT"
F 6 "None" H 3950 3800 50  0001 L BNN "PRICE"
F 7 "None" H 3950 3800 50  0001 L BNN "PACKAGE"
F 8 "MPD _Memory" H 3950 3800 50  0001 L BNN "MF"
F 9 "BATTERY HOLDER COIN 20MM SMD" H 3950 3800 50  0001 L BNN "DESCRIPTION"
F 10 "Unavailable" H 3950 3800 50  0001 L BNN "AVAILABILITY"
F 11 "F" H 3950 3800 50  0001 L BNN "PARTREV"
	1    3950 3800
	1    0    0    -1  
$EndComp
Text Label 5200 4100 2    50   ~ 0
GND
Text Label 4250 3800 0    50   ~ 0
GND
Text Label 6550 3800 0    50   ~ 0
Vcc
Wire Wire Line
	4800 3800 4800 3900
Wire Wire Line
	5200 4000 3650 4000
Wire Wire Line
	3650 4000 3650 3800
Text Label 6550 4100 0    50   ~ 0
SDA
Text Label 6550 4000 0    50   ~ 0
SCL
$Comp
L Device:R_Small_US R?
U 1 1 611A56A0
P 6850 3800
AR Path="/611A56A0" Ref="R?"  Part="1" 
AR Path="/6119E11D/611A56A0" Ref="R5"  Part="1" 
F 0 "R5" H 6918 3891 50  0000 L CNN
F 1 "1 KΩ" H 6918 3800 50  0000 L CNN
F 2 "footprints:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder_FullSilk" H 6850 3800 50  0001 C CNN
F 3 "~" H 6850 3800 50  0001 C CNN
F 4 "5%" H 6918 3709 50  0000 L CNN "Tolerance"
	1    6850 3800
	1    0    0    -1  
$EndComp
Wire Wire Line
	6550 3900 6850 3900
$Comp
L power:VCC #PWR?
U 1 1 611A56A7
P 6850 3700
AR Path="/611A56A7" Ref="#PWR?"  Part="1" 
AR Path="/6119E11D/611A56A7" Ref="#PWR0106"  Part="1" 
F 0 "#PWR0106" H 6850 3550 50  0001 C CNN
F 1 "VCC" H 6867 3873 50  0000 C CNN
F 2 "" H 6850 3700 50  0001 C CNN
F 3 "" H 6850 3700 50  0001 C CNN
	1    6850 3700
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small_US R?
U 1 1 611A56AE
P 7200 3800
AR Path="/611A56AE" Ref="R?"  Part="1" 
AR Path="/6119E11D/611A56AE" Ref="R6"  Part="1" 
F 0 "R6" H 7268 3891 50  0000 L CNN
F 1 "1 KΩ" H 7268 3800 50  0000 L CNN
F 2 "footprints:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder_FullSilk" H 7200 3800 50  0001 C CNN
F 3 "~" H 7200 3800 50  0001 C CNN
F 4 "5%" H 7268 3709 50  0000 L CNN "Tolerance"
	1    7200 3800
	1    0    0    -1  
$EndComp
$Comp
L power:VCC #PWR?
U 1 1 611A56B4
P 7200 3700
AR Path="/611A56B4" Ref="#PWR?"  Part="1" 
AR Path="/6119E11D/611A56B4" Ref="#PWR0107"  Part="1" 
F 0 "#PWR0107" H 7200 3550 50  0001 C CNN
F 1 "VCC" H 7217 3873 50  0000 C CNN
F 2 "" H 7200 3700 50  0001 C CNN
F 3 "" H 7200 3700 50  0001 C CNN
	1    7200 3700
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small_US R?
U 1 1 611A56BB
P 7550 3800
AR Path="/611A56BB" Ref="R?"  Part="1" 
AR Path="/6119E11D/611A56BB" Ref="R7"  Part="1" 
F 0 "R7" H 7618 3891 50  0000 L CNN
F 1 "1 KΩ" H 7618 3800 50  0000 L CNN
F 2 "footprints:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder_FullSilk" H 7550 3800 50  0001 C CNN
F 3 "~" H 7550 3800 50  0001 C CNN
F 4 "5%" H 7618 3709 50  0000 L CNN "Tolerance"
	1    7550 3800
	1    0    0    -1  
$EndComp
$Comp
L power:VCC #PWR?
U 1 1 611A56C1
P 7550 3700
AR Path="/611A56C1" Ref="#PWR?"  Part="1" 
AR Path="/6119E11D/611A56C1" Ref="#PWR0108"  Part="1" 
F 0 "#PWR0108" H 7550 3550 50  0001 C CNN
F 1 "VCC" H 7567 3873 50  0000 C CNN
F 2 "" H 7550 3700 50  0001 C CNN
F 3 "" H 7550 3700 50  0001 C CNN
	1    7550 3700
	1    0    0    -1  
$EndComp
Wire Wire Line
	7200 3900 7200 4000
Wire Wire Line
	7200 4000 6550 4000
Wire Wire Line
	6550 4100 7550 4100
Wire Wire Line
	7550 4100 7550 3900
Text HLabel 4150 2700 0    50   Input ~ 0
SDA
Text HLabel 4150 2800 0    50   Input ~ 0
SCL
Text HLabel 4150 2900 0    50   Input ~ 0
Vcc
Text HLabel 4150 3000 0    50   Input ~ 0
GND
Text Label 4150 2700 0    50   ~ 0
SDA
Text Label 4150 2800 0    50   ~ 0
SCL
Text Label 4150 2900 0    50   ~ 0
Vcc
Text Label 4150 3000 0    50   ~ 0
GND
Wire Wire Line
	4800 3900 5050 3900
Wire Wire Line
	5050 3900 5050 3800
Wire Wire Line
	5050 3800 5200 3800
Wire Wire Line
	5000 3800 5000 3950
Wire Wire Line
	5000 3950 5200 3950
Wire Wire Line
	5200 3950 5200 3900
$EndSCHEMATC
