EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Luciebox_components:button_laz16-11 S1
U 1 1 5FCA721B
P 850 1100
F 0 "S1" V 1350 700 50  0000 R CNN
F 1 "button_laz16-11" V 1250 950 50  0000 R CNN
F 2 "Luciebox:button_LAZ16-11" H 800 1350 50  0001 C CNN
F 3 "" H 800 1350 50  0001 C CNN
	1    850  1100
	0    -1   -1   0   
$EndComp
$Comp
L Luciebox_components:DAC_R-2R R1
U 1 1 5FCA9008
P 5450 1700
F 0 "R1" H 6178 2246 50  0000 L CNN
F 1 "DAC_R-2R" H 6200 2100 50  0000 L CNN
F 2 "Luciebox:SIP-10" H 5450 1700 50  0001 C CNN
F 3 "https://www.bourns.com/docs/Product-Datasheets/R2R.pdf" H 5450 1700 50  0001 C CNN
	1    5450 1700
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR03
U 1 1 5FCABE63
P 4550 1700
F 0 "#PWR03" H 4550 1450 50  0001 C CNN
F 1 "GND" H 4555 1527 50  0000 C CNN
F 2 "" H 4550 1700 50  0001 C CNN
F 3 "" H 4550 1700 50  0001 C CNN
	1    4550 1700
	1    0    0    -1  
$EndComp
$Comp
L Luciebox_components:button_laz16-11 S2
U 1 1 5FCAEC77
P 1700 1100
F 0 "S2" V 2200 700 50  0000 R CNN
F 1 "button_laz16-11" V 2100 950 50  0000 R CNN
F 2 "Luciebox:button_LAZ16-11" H 1650 1350 50  0001 C CNN
F 3 "" H 1650 1350 50  0001 C CNN
	1    1700 1100
	0    -1   -1   0   
$EndComp
$Comp
L Luciebox_components:button_laz16-11 S3
U 1 1 5FCAF323
P 2550 1100
F 0 "S3" V 3050 700 50  0000 R CNN
F 1 "button_laz16-11" V 2950 950 50  0000 R CNN
F 2 "Luciebox:button_LAZ16-11" H 2500 1350 50  0001 C CNN
F 3 "" H 2500 1350 50  0001 C CNN
	1    2550 1100
	0    -1   -1   0   
$EndComp
$Comp
L Luciebox_components:button_laz16-11 S4
U 1 1 5FCAF8CD
P 3400 1100
F 0 "S4" V 3900 700 50  0000 R CNN
F 1 "button_laz16-11" V 3800 950 50  0000 R CNN
F 2 "Luciebox:button_LAZ16-11" H 3350 1350 50  0001 C CNN
F 3 "" H 3350 1350 50  0001 C CNN
	1    3400 1100
	0    -1   -1   0   
$EndComp
Wire Wire Line
	1000 1300 1000 1600
Wire Wire Line
	1000 1600 1100 1600
Wire Wire Line
	1850 1600 1850 1300
Wire Wire Line
	1850 1600 2600 1600
Wire Wire Line
	2700 1600 2700 1300
Connection ~ 1850 1600
Wire Wire Line
	2700 1600 3550 1600
Wire Wire Line
	3550 1600 3550 1300
Connection ~ 2700 1600
Wire Wire Line
	1000 1600 700  1600
Connection ~ 1000 1600
$Comp
L power:VCC #PWR01
U 1 1 5FCB135E
P 700 1600
F 0 "#PWR01" H 700 1450 50  0001 C CNN
F 1 "VCC" H 717 1773 50  0000 C CNN
F 2 "" H 700 1600 50  0001 C CNN
F 3 "" H 700 1600 50  0001 C CNN
	1    700  1600
	1    0    0    -1  
$EndComp
Wire Wire Line
	1150 1300 1150 1750
Wire Wire Line
	1150 1750 2000 1750
Wire Wire Line
	2000 1750 2000 1300
Wire Wire Line
	2000 1750 2850 1750
Wire Wire Line
	2850 1750 2850 1300
Connection ~ 2000 1750
Wire Wire Line
	2850 1750 3700 1750
Wire Wire Line
	3700 1750 3700 1300
Connection ~ 2850 1750
Wire Wire Line
	1150 1750 1000 1750
Connection ~ 1150 1750
$Comp
L power:GND #PWR02
U 1 1 5FCB31CF
P 700 1750
F 0 "#PWR02" H 700 1500 50  0001 C CNN
F 1 "GND" H 705 1577 50  0000 C CNN
F 2 "" H 700 1750 50  0001 C CNN
F 3 "" H 700 1750 50  0001 C CNN
	1    700  1750
	1    0    0    -1  
$EndComp
Wire Wire Line
	1450 1300 1450 1900
Wire Wire Line
	1450 1900 2300 1900
Wire Wire Line
	2300 1900 2300 1300
Wire Wire Line
	2300 1900 2500 1900
Wire Wire Line
	3150 1900 3150 1300
Connection ~ 2300 1900
Wire Wire Line
	3150 1900 4000 1900
Wire Wire Line
	4000 1900 4000 1300
Connection ~ 3150 1900
Wire Wire Line
	3850 1300 3850 2050
Wire Wire Line
	3850 2050 5050 2050
Wire Wire Line
	5050 2050 5050 1700
Wire Wire Line
	5150 1700 5150 2200
Wire Wire Line
	5150 2200 3000 2200
Wire Wire Line
	3000 2200 3000 1300
Wire Wire Line
	2150 1300 2150 2350
Wire Wire Line
	2150 2350 5250 2350
Wire Wire Line
	5250 2350 5250 1700
Wire Wire Line
	5350 1700 5350 2500
Wire Wire Line
	5350 2500 1300 2500
Wire Wire Line
	1300 1300 1300 2500
$Comp
L Luciebox_components:terminal_8x1 T2
U 1 1 5FCC8382
P 4650 2650
F 0 "T2" V 4721 3478 50  0000 L CNN
F 1 "terminal_8x1" V 4812 3478 50  0000 L CNN
F 2 "Luciebox:Terminal_8x1_100mil" H 4450 2550 50  0001 C CNN
F 3 "" H 4600 3750 50  0001 C CNN
	1    4650 2650
	0    1    1    0   
$EndComp
$Comp
L Luciebox_components:terminal_8x1 T1
U 1 1 5FCC91EB
P 1000 3000
F 0 "T1" V 1071 3828 50  0000 L CNN
F 1 "terminal_8x1" V 1162 3828 50  0000 L CNN
F 2 "Luciebox:Terminal_8x1_100mil" H 800 2900 50  0001 C CNN
F 3 "" H 950 4100 50  0001 C CNN
	1    1000 3000
	0    1    1    0   
$EndComp
Wire Wire Line
	4650 2600 4650 1800
Wire Wire Line
	4750 1700 4750 1850
Wire Wire Line
	4850 2600 4850 1900
Wire Wire Line
	4950 1700 4950 1950
Wire Wire Line
	5050 2600 5050 2050
Connection ~ 5050 2050
Wire Wire Line
	5150 2200 5150 2600
Connection ~ 5150 2200
Wire Wire Line
	5250 2350 5250 2600
Connection ~ 5250 2350
Wire Wire Line
	5350 2500 5350 2600
Connection ~ 5350 2500
Wire Wire Line
	1000 2950 1000 1750
Connection ~ 1000 1750
Wire Wire Line
	1000 1750 700  1750
Wire Wire Line
	1100 1600 1100 2950
Connection ~ 1100 1600
Wire Wire Line
	1100 1600 1850 1600
Text Label 5450 1700 3    50   ~ 0
ANALOG_1
Text Label 1200 2950 1    50   ~ 0
ANALOG_1
Wire Wire Line
	1450 1900 1450 2600
Wire Wire Line
	1450 2600 1300 2600
Wire Wire Line
	1300 2600 1300 2950
Connection ~ 1450 1900
Wire Wire Line
	1400 2950 1400 2650
Wire Wire Line
	1400 2650 1600 2650
Wire Wire Line
	1600 2650 1600 1300
Wire Wire Line
	2450 1300 2450 2700
Wire Wire Line
	2450 2700 1500 2700
Wire Wire Line
	1500 2700 1500 2950
Wire Wire Line
	1600 2950 1600 2750
Wire Wire Line
	1600 2750 3300 2750
Wire Wire Line
	3300 2750 3300 1300
Wire Wire Line
	1700 2950 1700 2800
Wire Wire Line
	1700 2800 4150 2800
Wire Wire Line
	4150 2800 4150 1300
$Comp
L Luciebox_components:R_Small connect_for_vcc_as_common_anode1
U 1 1 5FD29B56
P 2650 3150
F 0 "connect_for_vcc_as_common_anode1" H 2709 3196 50  0000 L CNN
F 1 "R_Small" H 2709 3105 50  0000 L CNN
F 2 "Luciebox:1206_0805" H 2650 3150 50  0001 C CNN
F 3 "~" H 2650 3150 50  0001 C CNN
	1    2650 3150
	1    0    0    -1  
$EndComp
Wire Wire Line
	2600 1600 2600 3050
Wire Wire Line
	2600 3050 2650 3050
Connection ~ 2600 1600
Wire Wire Line
	2600 1600 2700 1600
Wire Wire Line
	2650 3250 2500 3250
Wire Wire Line
	2500 3250 2500 1900
Connection ~ 2500 1900
Wire Wire Line
	2500 1900 3150 1900
$Comp
L Luciebox_components:R_Small R?
U 1 1 5FD2D965
P 5900 1250
F 0 "R?" H 5959 1296 50  0000 L CNN
F 1 "R_Small" H 5959 1205 50  0000 L CNN
F 2 "Luciebox:1206_0805" H 5900 1250 50  0001 C CNN
F 3 "~" H 5900 1250 50  0001 C CNN
	1    5900 1250
	1    0    0    -1  
$EndComp
$Comp
L Luciebox_components:R_Small R?
U 1 1 5FD36770
P 6100 1050
F 0 "R?" V 5904 1050 50  0000 C CNN
F 1 "R_Small" V 5995 1050 50  0000 C CNN
F 2 "Luciebox:1206_0805" H 6100 1050 50  0001 C CNN
F 3 "~" H 6100 1050 50  0001 C CNN
	1    6100 1050
	0    1    1    0   
$EndComp
Wire Wire Line
	6000 1050 5900 1050
Wire Wire Line
	5900 1050 5900 1150
Wire Wire Line
	6200 1050 6300 1050
Wire Wire Line
	5900 1350 5900 1450
$Comp
L Luciebox_components:R_Small R?
U 1 1 5FD403BE
P 6300 1250
F 0 "R?" H 6359 1296 50  0000 L CNN
F 1 "R_Small" H 6359 1205 50  0000 L CNN
F 2 "Luciebox:1206_0805" H 6300 1250 50  0001 C CNN
F 3 "~" H 6300 1250 50  0001 C CNN
	1    6300 1250
	1    0    0    -1  
$EndComp
$Comp
L Luciebox_components:R_Small R?
U 1 1 5FD403C8
P 6500 1050
F 0 "R?" V 6304 1050 50  0000 C CNN
F 1 "R_Small" V 6395 1050 50  0000 C CNN
F 2 "Luciebox:1206_0805" H 6500 1050 50  0001 C CNN
F 3 "~" H 6500 1050 50  0001 C CNN
	1    6500 1050
	0    1    1    0   
$EndComp
Wire Wire Line
	6400 1050 6300 1050
Wire Wire Line
	6300 1050 6300 1150
Wire Wire Line
	6600 1050 6700 1050
Connection ~ 6300 1050
$Comp
L Luciebox_components:R_Small R?
U 1 1 5FD434BF
P 6700 1250
F 0 "R?" H 6759 1296 50  0000 L CNN
F 1 "R_Small" H 6759 1205 50  0000 L CNN
F 2 "Luciebox:1206_0805" H 6700 1250 50  0001 C CNN
F 3 "~" H 6700 1250 50  0001 C CNN
	1    6700 1250
	1    0    0    -1  
$EndComp
$Comp
L Luciebox_components:R_Small R?
U 1 1 5FD434C9
P 6900 1050
F 0 "R?" V 6704 1050 50  0000 C CNN
F 1 "R_Small" V 6795 1050 50  0000 C CNN
F 2 "Luciebox:1206_0805" H 6900 1050 50  0001 C CNN
F 3 "~" H 6900 1050 50  0001 C CNN
	1    6900 1050
	0    1    1    0   
$EndComp
Wire Wire Line
	6800 1050 6700 1050
Wire Wire Line
	6700 1050 6700 1150
Wire Wire Line
	7000 1050 7100 1050
$Comp
L Luciebox_components:R_Small R?
U 1 1 5FD434D7
P 7100 1250
F 0 "R?" H 7159 1296 50  0000 L CNN
F 1 "R_Small" H 7159 1205 50  0000 L CNN
F 2 "Luciebox:1206_0805" H 7100 1250 50  0001 C CNN
F 3 "~" H 7100 1250 50  0001 C CNN
	1    7100 1250
	1    0    0    -1  
$EndComp
$Comp
L Luciebox_components:R_Small R?
U 1 1 5FD434E1
P 7300 1050
F 0 "R?" V 7104 1050 50  0000 C CNN
F 1 "R_Small" V 7195 1050 50  0000 C CNN
F 2 "Luciebox:1206_0805" H 7300 1050 50  0001 C CNN
F 3 "~" H 7300 1050 50  0001 C CNN
	1    7300 1050
	0    1    1    0   
$EndComp
Wire Wire Line
	7200 1050 7100 1050
Wire Wire Line
	7100 1050 7100 1150
Wire Wire Line
	7400 1050 7500 1050
Connection ~ 7100 1050
Connection ~ 6700 1050
$Comp
L Luciebox_components:R_Small R?
U 1 1 5FD4CA04
P 7500 1250
F 0 "R?" H 7559 1296 50  0000 L CNN
F 1 "R_Small" H 7559 1205 50  0000 L CNN
F 2 "Luciebox:1206_0805" H 7500 1250 50  0001 C CNN
F 3 "~" H 7500 1250 50  0001 C CNN
	1    7500 1250
	1    0    0    -1  
$EndComp
$Comp
L Luciebox_components:R_Small R?
U 1 1 5FD4CA0E
P 7700 1050
F 0 "R?" V 7504 1050 50  0000 C CNN
F 1 "R_Small" V 7595 1050 50  0000 C CNN
F 2 "Luciebox:1206_0805" H 7700 1050 50  0001 C CNN
F 3 "~" H 7700 1050 50  0001 C CNN
	1    7700 1050
	0    1    1    0   
$EndComp
Wire Wire Line
	7600 1050 7500 1050
Wire Wire Line
	7500 1050 7500 1150
Wire Wire Line
	7800 1050 7900 1050
$Comp
L Luciebox_components:R_Small R?
U 1 1 5FD4CA1C
P 7900 1250
F 0 "R?" H 7959 1296 50  0000 L CNN
F 1 "R_Small" H 7959 1205 50  0000 L CNN
F 2 "Luciebox:1206_0805" H 7900 1250 50  0001 C CNN
F 3 "~" H 7900 1250 50  0001 C CNN
	1    7900 1250
	1    0    0    -1  
$EndComp
$Comp
L Luciebox_components:R_Small R?
U 1 1 5FD4CA26
P 8100 1050
F 0 "R?" V 7904 1050 50  0000 C CNN
F 1 "R_Small" V 7995 1050 50  0000 C CNN
F 2 "Luciebox:1206_0805" H 8100 1050 50  0001 C CNN
F 3 "~" H 8100 1050 50  0001 C CNN
	1    8100 1050
	0    1    1    0   
$EndComp
Wire Wire Line
	8000 1050 7900 1050
Wire Wire Line
	7900 1050 7900 1150
Wire Wire Line
	8200 1050 8300 1050
Connection ~ 7900 1050
$Comp
L Luciebox_components:R_Small R?
U 1 1 5FD4CA35
P 8300 1250
F 0 "R?" H 8359 1296 50  0000 L CNN
F 1 "R_Small" H 8359 1205 50  0000 L CNN
F 2 "Luciebox:1206_0805" H 8300 1250 50  0001 C CNN
F 3 "~" H 8300 1250 50  0001 C CNN
	1    8300 1250
	1    0    0    -1  
$EndComp
$Comp
L Luciebox_components:R_Small R?
U 1 1 5FD4CA3F
P 8500 1050
F 0 "R?" V 8304 1050 50  0000 C CNN
F 1 "R_Small" V 8395 1050 50  0000 C CNN
F 2 "Luciebox:1206_0805" H 8500 1050 50  0001 C CNN
F 3 "~" H 8500 1050 50  0001 C CNN
	1    8500 1050
	0    1    1    0   
$EndComp
Wire Wire Line
	8400 1050 8300 1050
Wire Wire Line
	8300 1050 8300 1150
Wire Wire Line
	8600 1050 8700 1050
$Comp
L Luciebox_components:R_Small R?
U 1 1 5FD4CA4D
P 8700 1250
F 0 "R?" H 8759 1296 50  0000 L CNN
F 1 "R_Small" H 8759 1205 50  0000 L CNN
F 2 "Luciebox:1206_0805" H 8700 1250 50  0001 C CNN
F 3 "~" H 8700 1250 50  0001 C CNN
	1    8700 1250
	1    0    0    -1  
$EndComp
$Comp
L Luciebox_components:R_Small R?
U 1 1 5FD4CA57
P 8900 1050
F 0 "R?" V 8704 1050 50  0000 C CNN
F 1 "R_Small" V 8795 1050 50  0000 C CNN
F 2 "Luciebox:1206_0805" H 8900 1050 50  0001 C CNN
F 3 "~" H 8900 1050 50  0001 C CNN
	1    8900 1050
	0    1    1    0   
$EndComp
Wire Wire Line
	8800 1050 8700 1050
Wire Wire Line
	8700 1050 8700 1150
Connection ~ 8700 1050
Connection ~ 8300 1050
Connection ~ 7500 1050
$Comp
L Luciebox_components:R_Small R?
U 1 1 5FD57290
P 9200 1250
F 0 "R?" H 9259 1296 50  0000 L CNN
F 1 "R_Small" H 9259 1205 50  0000 L CNN
F 2 "Luciebox:1206_0805" H 9200 1250 50  0001 C CNN
F 3 "~" H 9200 1250 50  0001 C CNN
	1    9200 1250
	1    0    0    -1  
$EndComp
Wire Wire Line
	9300 1050 9200 1050
Wire Wire Line
	9200 1050 9200 1150
Connection ~ 9200 1050
Wire Wire Line
	9000 1050 9200 1050
$Comp
L power:GND #PWR?
U 1 1 5FD5CD2D
P 5900 1450
F 0 "#PWR?" H 5900 1200 50  0001 C CNN
F 1 "GND" H 5905 1277 50  0000 C CNN
F 2 "" H 5900 1450 50  0001 C CNN
F 3 "" H 5900 1450 50  0001 C CNN
	1    5900 1450
	1    0    0    -1  
$EndComp
Wire Wire Line
	4650 1800 6300 1800
Wire Wire Line
	6300 1350 6300 1800
Connection ~ 4650 1800
Wire Wire Line
	4650 1800 4650 1700
Wire Wire Line
	4750 1850 6700 1850
Wire Wire Line
	6700 1350 6700 1850
Connection ~ 4750 1850
Wire Wire Line
	4750 1850 4750 2600
Wire Wire Line
	4850 1900 7100 1900
Wire Wire Line
	7100 1350 7100 1900
Connection ~ 4850 1900
Wire Wire Line
	4850 1900 4850 1700
Wire Wire Line
	4950 1950 7500 1950
Wire Wire Line
	7500 1350 7500 1950
Connection ~ 4950 1950
Wire Wire Line
	4950 1950 4950 2600
Wire Wire Line
	5050 2050 7900 2050
Wire Wire Line
	7900 1350 7900 2050
Wire Wire Line
	5150 2200 8300 2200
Wire Wire Line
	8300 1350 8300 2200
Wire Wire Line
	5250 2350 8700 2350
Wire Wire Line
	8700 1350 8700 2350
Wire Wire Line
	5350 2500 9200 2500
Wire Wire Line
	9200 1350 9200 2500
Text Label 9300 1050 0    50   ~ 0
ANALOG_1
$EndSCHEMATC
