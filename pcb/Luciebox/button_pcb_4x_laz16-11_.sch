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
	4750 1700 4750 1850
Wire Wire Line
	4850 2600 4850 2550
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
	2450 1300 2450 2700
Wire Wire Line
	1500 2700 1500 2950
Wire Wire Line
	1600 2950 1600 2750
Wire Wire Line
	3300 2750 3300 1300
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
L Luciebox_components:R_Small R2
U 1 1 5FD2D965
P 5900 1250
F 0 "R2" H 5959 1296 50  0000 L CNN
F 1 "R_Small" H 5959 1205 50  0000 L CNN
F 2 "Luciebox:1206_0805" H 5900 1250 50  0001 C CNN
F 3 "~" H 5900 1250 50  0001 C CNN
	1    5900 1250
	1    0    0    -1  
$EndComp
$Comp
L Luciebox_components:R_Small R3
U 1 1 5FD36770
P 6100 1050
F 0 "R3" V 5904 1050 50  0000 C CNN
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
L Luciebox_components:R_Small R4
U 1 1 5FD403BE
P 6300 1250
F 0 "R4" H 6359 1296 50  0000 L CNN
F 1 "R_Small" H 6359 1205 50  0000 L CNN
F 2 "Luciebox:1206_0805" H 6300 1250 50  0001 C CNN
F 3 "~" H 6300 1250 50  0001 C CNN
	1    6300 1250
	1    0    0    -1  
$EndComp
$Comp
L Luciebox_components:R_Small R5
U 1 1 5FD403C8
P 6500 1050
F 0 "R5" V 6304 1050 50  0000 C CNN
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
L Luciebox_components:R_Small R6
U 1 1 5FD434BF
P 6700 1250
F 0 "R6" H 6759 1296 50  0000 L CNN
F 1 "R_Small" H 6759 1205 50  0000 L CNN
F 2 "Luciebox:1206_0805" H 6700 1250 50  0001 C CNN
F 3 "~" H 6700 1250 50  0001 C CNN
	1    6700 1250
	1    0    0    -1  
$EndComp
$Comp
L Luciebox_components:R_Small R7
U 1 1 5FD434C9
P 6900 1050
F 0 "R7" V 6704 1050 50  0000 C CNN
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
L Luciebox_components:R_Small R8
U 1 1 5FD434D7
P 7100 1250
F 0 "R8" H 7159 1296 50  0000 L CNN
F 1 "R_Small" H 7159 1205 50  0000 L CNN
F 2 "Luciebox:1206_0805" H 7100 1250 50  0001 C CNN
F 3 "~" H 7100 1250 50  0001 C CNN
	1    7100 1250
	1    0    0    -1  
$EndComp
$Comp
L Luciebox_components:R_Small R9
U 1 1 5FD434E1
P 7300 1050
F 0 "R9" V 7104 1050 50  0000 C CNN
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
L Luciebox_components:R_Small R10
U 1 1 5FD4CA04
P 7500 1250
F 0 "R10" H 7559 1296 50  0000 L CNN
F 1 "R_Small" H 7559 1205 50  0000 L CNN
F 2 "Luciebox:1206_0805" H 7500 1250 50  0001 C CNN
F 3 "~" H 7500 1250 50  0001 C CNN
	1    7500 1250
	1    0    0    -1  
$EndComp
$Comp
L Luciebox_components:R_Small R11
U 1 1 5FD4CA0E
P 7700 1050
F 0 "R11" V 7504 1050 50  0000 C CNN
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
L Luciebox_components:R_Small R12
U 1 1 5FD4CA1C
P 7900 1250
F 0 "R12" H 7959 1296 50  0000 L CNN
F 1 "R_Small" H 7959 1205 50  0000 L CNN
F 2 "Luciebox:1206_0805" H 7900 1250 50  0001 C CNN
F 3 "~" H 7900 1250 50  0001 C CNN
	1    7900 1250
	1    0    0    -1  
$EndComp
$Comp
L Luciebox_components:R_Small R13
U 1 1 5FD4CA26
P 8100 1050
F 0 "R13" V 7904 1050 50  0000 C CNN
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
L Luciebox_components:R_Small R14
U 1 1 5FD4CA35
P 8300 1250
F 0 "R14" H 8359 1296 50  0000 L CNN
F 1 "R_Small" H 8359 1205 50  0000 L CNN
F 2 "Luciebox:1206_0805" H 8300 1250 50  0001 C CNN
F 3 "~" H 8300 1250 50  0001 C CNN
	1    8300 1250
	1    0    0    -1  
$EndComp
$Comp
L Luciebox_components:R_Small R15
U 1 1 5FD4CA3F
P 8500 1050
F 0 "R15" V 8304 1050 50  0000 C CNN
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
L Luciebox_components:R_Small R16
U 1 1 5FD4CA4D
P 8700 1250
F 0 "R16" H 8759 1296 50  0000 L CNN
F 1 "R_Small" H 8759 1205 50  0000 L CNN
F 2 "Luciebox:1206_0805" H 8700 1250 50  0001 C CNN
F 3 "~" H 8700 1250 50  0001 C CNN
	1    8700 1250
	1    0    0    -1  
$EndComp
$Comp
L Luciebox_components:R_Small R17
U 1 1 5FD4CA57
P 8900 1050
F 0 "R17" V 8704 1050 50  0000 C CNN
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
L Luciebox_components:R_Small R18
U 1 1 5FD57290
P 9200 1250
F 0 "R18" H 9259 1296 50  0000 L CNN
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
L power:GND #PWR04
U 1 1 5FD5CD2D
P 5900 1450
F 0 "#PWR04" H 5900 1200 50  0001 C CNN
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
	4750 1850 4750 2550
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
	4950 1950 4950 2550
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
$Comp
L Luciebox_components:button_laz16-11 S5
U 1 1 5FDB2BE6
P 900 4000
F 0 "S5" V 1400 3600 50  0000 R CNN
F 1 "button_laz16-11" V 1300 3850 50  0000 R CNN
F 2 "Luciebox:button_LAZ16-11" H 850 4250 50  0001 C CNN
F 3 "" H 850 4250 50  0001 C CNN
	1    900  4000
	0    -1   -1   0   
$EndComp
$Comp
L Luciebox_components:DAC_R-2R R19
U 1 1 5FDB2BF0
P 5500 4600
F 0 "R19" H 6228 5146 50  0000 L CNN
F 1 "DAC_R-2R" H 6250 5000 50  0000 L CNN
F 2 "Luciebox:SIP-10" H 5500 4600 50  0001 C CNN
F 3 "https://www.bourns.com/docs/Product-Datasheets/R2R.pdf" H 5500 4600 50  0001 C CNN
	1    5500 4600
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR07
U 1 1 5FDB2BFA
P 4600 4600
F 0 "#PWR07" H 4600 4350 50  0001 C CNN
F 1 "GND" H 4605 4427 50  0000 C CNN
F 2 "" H 4600 4600 50  0001 C CNN
F 3 "" H 4600 4600 50  0001 C CNN
	1    4600 4600
	1    0    0    -1  
$EndComp
$Comp
L Luciebox_components:button_laz16-11 S6
U 1 1 5FDB2C04
P 1750 4000
F 0 "S6" V 2250 3600 50  0000 R CNN
F 1 "button_laz16-11" V 2150 3850 50  0000 R CNN
F 2 "Luciebox:button_LAZ16-11" H 1700 4250 50  0001 C CNN
F 3 "" H 1700 4250 50  0001 C CNN
	1    1750 4000
	0    -1   -1   0   
$EndComp
$Comp
L Luciebox_components:button_laz16-11 S7
U 1 1 5FDB2C0E
P 2600 4000
F 0 "S7" V 3100 3600 50  0000 R CNN
F 1 "button_laz16-11" V 3000 3850 50  0000 R CNN
F 2 "Luciebox:button_LAZ16-11" H 2550 4250 50  0001 C CNN
F 3 "" H 2550 4250 50  0001 C CNN
	1    2600 4000
	0    -1   -1   0   
$EndComp
$Comp
L Luciebox_components:button_laz16-11 S8
U 1 1 5FDB2C18
P 3450 4000
F 0 "S8" V 3950 3600 50  0000 R CNN
F 1 "button_laz16-11" V 3850 3850 50  0000 R CNN
F 2 "Luciebox:button_LAZ16-11" H 3400 4250 50  0001 C CNN
F 3 "" H 3400 4250 50  0001 C CNN
	1    3450 4000
	0    -1   -1   0   
$EndComp
Wire Wire Line
	1050 4200 1050 4500
Wire Wire Line
	1050 4500 1150 4500
Wire Wire Line
	1900 4500 1900 4200
Wire Wire Line
	1900 4500 2650 4500
Wire Wire Line
	2750 4500 2750 4200
Connection ~ 1900 4500
Wire Wire Line
	2750 4500 3600 4500
Wire Wire Line
	3600 4500 3600 4200
Connection ~ 2750 4500
Wire Wire Line
	1050 4500 750  4500
Connection ~ 1050 4500
$Comp
L power:VCC #PWR05
U 1 1 5FDB2C2D
P 750 4500
F 0 "#PWR05" H 750 4350 50  0001 C CNN
F 1 "VCC" H 767 4673 50  0000 C CNN
F 2 "" H 750 4500 50  0001 C CNN
F 3 "" H 750 4500 50  0001 C CNN
	1    750  4500
	1    0    0    -1  
$EndComp
Wire Wire Line
	1200 4200 1200 4650
Wire Wire Line
	1200 4650 2050 4650
Wire Wire Line
	2050 4650 2050 4200
Wire Wire Line
	2050 4650 2900 4650
Wire Wire Line
	2900 4650 2900 4200
Connection ~ 2050 4650
Wire Wire Line
	2900 4650 3750 4650
Wire Wire Line
	3750 4650 3750 4200
Connection ~ 2900 4650
Wire Wire Line
	1200 4650 1050 4650
Connection ~ 1200 4650
$Comp
L power:GND #PWR06
U 1 1 5FDB2C42
P 750 4650
F 0 "#PWR06" H 750 4400 50  0001 C CNN
F 1 "GND" H 755 4477 50  0000 C CNN
F 2 "" H 750 4650 50  0001 C CNN
F 3 "" H 750 4650 50  0001 C CNN
	1    750  4650
	1    0    0    -1  
$EndComp
Wire Wire Line
	1500 4200 1500 4800
Wire Wire Line
	1500 4800 2350 4800
Wire Wire Line
	2350 4800 2350 4200
Wire Wire Line
	2350 4800 2550 4800
Wire Wire Line
	3200 4800 3200 4200
Connection ~ 2350 4800
Wire Wire Line
	3200 4800 4050 4800
Wire Wire Line
	4050 4800 4050 4200
Connection ~ 3200 4800
Wire Wire Line
	3900 4200 3900 4950
Wire Wire Line
	3900 4950 5100 4950
Wire Wire Line
	5100 4950 5100 4600
Wire Wire Line
	5200 4600 5200 5100
Wire Wire Line
	5200 5100 3050 5100
Wire Wire Line
	3050 5100 3050 4200
Wire Wire Line
	2200 4200 2200 5250
Wire Wire Line
	2200 5250 5300 5250
Wire Wire Line
	5300 5250 5300 4600
Wire Wire Line
	5400 4600 5400 5400
Wire Wire Line
	1350 4200 1350 5400
$Comp
L Luciebox_components:terminal_8x1 T4
U 1 1 5FDB2C61
P 4700 5550
F 0 "T4" V 4771 6378 50  0000 L CNN
F 1 "terminal_8x1" V 4862 6378 50  0000 L CNN
F 2 "Luciebox:Terminal_8x1_100mil" H 4500 5450 50  0001 C CNN
F 3 "" H 4650 6650 50  0001 C CNN
	1    4700 5550
	0    1    1    0   
$EndComp
$Comp
L Luciebox_components:terminal_8x1 T3
U 1 1 5FDB2C6B
P 1050 5900
F 0 "T3" V 1121 6728 50  0000 L CNN
F 1 "terminal_8x1" V 1212 6728 50  0000 L CNN
F 2 "Luciebox:Terminal_8x1_100mil" H 850 5800 50  0001 C CNN
F 3 "" H 1000 7000 50  0001 C CNN
	1    1050 5900
	0    1    1    0   
$EndComp
Wire Wire Line
	5100 5500 5100 4950
Connection ~ 5100 4950
Connection ~ 5200 5100
Wire Wire Line
	5300 5250 5300 5500
Connection ~ 5300 5250
Wire Wire Line
	5400 5400 5400 5500
Connection ~ 5400 5400
Wire Wire Line
	1050 5850 1050 4650
Connection ~ 1050 4650
Wire Wire Line
	1050 4650 750  4650
Wire Wire Line
	1150 4500 1150 5850
Connection ~ 1150 4500
Wire Wire Line
	1150 4500 1900 4500
Wire Wire Line
	1500 4800 1500 5500
Wire Wire Line
	1500 5500 1350 5500
Wire Wire Line
	1350 5500 1350 5850
Connection ~ 1500 4800
Wire Wire Line
	1450 5850 1450 5550
Wire Wire Line
	2500 4200 2500 5600
Wire Wire Line
	1550 5600 1550 5850
Wire Wire Line
	1650 5850 1650 5650
Wire Wire Line
	3350 5650 3350 4200
Wire Wire Line
	1750 5850 1750 5700
Wire Wire Line
	4200 5700 4200 4200
$Comp
L Luciebox_components:R_Small connect_for_vcc_as_common_anode2
U 1 1 5FDB2C99
P 2700 6050
F 0 "connect_for_vcc_as_common_anode2" H 2759 6096 50  0000 L CNN
F 1 "R_Small" H 2759 6005 50  0000 L CNN
F 2 "Luciebox:1206_0805" H 2700 6050 50  0001 C CNN
F 3 "~" H 2700 6050 50  0001 C CNN
	1    2700 6050
	1    0    0    -1  
$EndComp
Wire Wire Line
	2650 4500 2650 5950
Wire Wire Line
	2650 5950 2700 5950
Connection ~ 2650 4500
Wire Wire Line
	2650 4500 2750 4500
Wire Wire Line
	2700 6150 2550 6150
Wire Wire Line
	2550 6150 2550 4800
Connection ~ 2550 4800
Wire Wire Line
	2550 4800 3200 4800
Wire Wire Line
	4800 4600 4800 5450
Text Label 5500 4600 3    50   ~ 0
ANALOG_2
Text Label 1250 5850 1    50   ~ 0
ANALOG_2
$Comp
L Luciebox_components:4digit_7segments_common_anode_CA56-12SRWA U1
U 1 1 5FDD9307
P 8450 4100
F 0 "U1" H 8450 4767 50  0000 C CNN
F 1 "4digit_7segments_common_anode_CA56-12SRWA" H 8450 4676 50  0000 C CNN
F 2 "Display_7Segment:CA56-12SRWA" H 8450 3500 50  0001 C CNN
F 3 "http://www.kingbrightusa.com/images/catalog/SPEC/CA56-12SRWA.pdf" H 8020 4130 50  0001 C CNN
	1    8450 4100
	1    0    0    -1  
$EndComp
$Comp
L Luciebox_components:terminal_8x1 T5
U 1 1 5FDDA753
P 9800 5100
F 0 "T5" V 9871 5928 50  0000 L CNN
F 1 "terminal_8x1" V 9962 5928 50  0000 L CNN
F 2 "Luciebox:Terminal_8x1_100mil" H 9600 5000 50  0001 C CNN
F 3 "" H 9750 6200 50  0001 C CNN
	1    9800 5100
	0    1    1    0   
$EndComp
$Comp
L Luciebox_components:terminal_8x1 T6
U 1 1 5FDDAE5B
P 6350 4850
F 0 "T6" V 6421 5678 50  0000 L CNN
F 1 "terminal_8x1" V 6512 5678 50  0000 L CNN
F 2 "Luciebox:Terminal_8x1_100mil" H 6150 4750 50  0001 C CNN
F 3 "" H 6300 5950 50  0001 C CNN
	1    6350 4850
	0    1    1    0   
$EndComp
$Comp
L Luciebox_components:MountingHole H1
U 1 1 5FF140D9
P 1100 6650
F 0 "H1" H 1200 6696 50  0000 L CNN
F 1 "MountingHole" H 1200 6605 50  0000 L CNN
F 2 "Luciebox:MountingHole_4.3mm_M4" H 1100 6650 50  0001 C CNN
F 3 "~" H 1100 6650 50  0001 C CNN
	1    1100 6650
	1    0    0    -1  
$EndComp
$Comp
L Luciebox_components:MountingHole H4
U 1 1 5FF146C0
P 1350 6650
F 0 "H4" H 1450 6696 50  0000 L CNN
F 1 "MountingHole" H 1450 6605 50  0000 L CNN
F 2 "Luciebox:MountingHole_4.3mm_M4" H 1350 6650 50  0001 C CNN
F 3 "~" H 1350 6650 50  0001 C CNN
	1    1350 6650
	1    0    0    -1  
$EndComp
$Comp
L Luciebox_components:MountingHole H7
U 1 1 5FF14E02
P 1550 6650
F 0 "H7" H 1650 6696 50  0000 L CNN
F 1 "MountingHole" H 1650 6605 50  0000 L CNN
F 2 "Luciebox:MountingHole_4.3mm_M4" H 1550 6650 50  0001 C CNN
F 3 "~" H 1550 6650 50  0001 C CNN
	1    1550 6650
	1    0    0    -1  
$EndComp
$Comp
L Luciebox_components:MountingHole H10
U 1 1 5FF14FFD
P 1750 6650
F 0 "H10" H 1850 6696 50  0000 L CNN
F 1 "MountingHole" H 1850 6605 50  0000 L CNN
F 2 "Luciebox:MountingHole_4.3mm_M4" H 1750 6650 50  0001 C CNN
F 3 "~" H 1750 6650 50  0001 C CNN
	1    1750 6650
	1    0    0    -1  
$EndComp
$Comp
L Luciebox_components:MountingHole H2
U 1 1 5FF16676
P 1100 6900
F 0 "H2" H 1200 6946 50  0000 L CNN
F 1 "MountingHole" H 1200 6855 50  0000 L CNN
F 2 "Luciebox:MountingHole_4.3mm_M4" H 1100 6900 50  0001 C CNN
F 3 "~" H 1100 6900 50  0001 C CNN
	1    1100 6900
	1    0    0    -1  
$EndComp
$Comp
L Luciebox_components:MountingHole H5
U 1 1 5FF16680
P 1350 6900
F 0 "H5" H 1450 6946 50  0000 L CNN
F 1 "MountingHole" H 1450 6855 50  0000 L CNN
F 2 "Luciebox:MountingHole_4.3mm_M4" H 1350 6900 50  0001 C CNN
F 3 "~" H 1350 6900 50  0001 C CNN
	1    1350 6900
	1    0    0    -1  
$EndComp
$Comp
L Luciebox_components:MountingHole H8
U 1 1 5FF1668A
P 1550 6900
F 0 "H8" H 1650 6946 50  0000 L CNN
F 1 "MountingHole" H 1650 6855 50  0000 L CNN
F 2 "Luciebox:MountingHole_4.3mm_M4" H 1550 6900 50  0001 C CNN
F 3 "~" H 1550 6900 50  0001 C CNN
	1    1550 6900
	1    0    0    -1  
$EndComp
$Comp
L Luciebox_components:MountingHole H11
U 1 1 5FF16694
P 1750 6900
F 0 "H11" H 1850 6946 50  0000 L CNN
F 1 "MountingHole" H 1850 6855 50  0000 L CNN
F 2 "Luciebox:MountingHole_4.3mm_M4" H 1750 6900 50  0001 C CNN
F 3 "~" H 1750 6900 50  0001 C CNN
	1    1750 6900
	1    0    0    -1  
$EndComp
$Comp
L Luciebox_components:MountingHole H3
U 1 1 5FF21649
P 1100 7150
F 0 "H3" H 1200 7196 50  0000 L CNN
F 1 "MountingHole" H 1200 7105 50  0000 L CNN
F 2 "Luciebox:MountingHole_4.3mm_M4" H 1100 7150 50  0001 C CNN
F 3 "~" H 1100 7150 50  0001 C CNN
	1    1100 7150
	1    0    0    -1  
$EndComp
$Comp
L Luciebox_components:MountingHole H6
U 1 1 5FF21653
P 1350 7150
F 0 "H6" H 1450 7196 50  0000 L CNN
F 1 "MountingHole" H 1450 7105 50  0000 L CNN
F 2 "Luciebox:MountingHole_4.3mm_M4" H 1350 7150 50  0001 C CNN
F 3 "~" H 1350 7150 50  0001 C CNN
	1    1350 7150
	1    0    0    -1  
$EndComp
$Comp
L Luciebox_components:MountingHole H9
U 1 1 5FF2165D
P 1550 7150
F 0 "H9" H 1650 7196 50  0000 L CNN
F 1 "MountingHole" H 1650 7105 50  0000 L CNN
F 2 "Luciebox:MountingHole_4.3mm_M4" H 1550 7150 50  0001 C CNN
F 3 "~" H 1550 7150 50  0001 C CNN
	1    1550 7150
	1    0    0    -1  
$EndComp
$Comp
L Luciebox_components:MountingHole H12
U 1 1 5FF21667
P 1750 7150
F 0 "H12" H 1850 7196 50  0000 L CNN
F 1 "MountingHole" H 1850 7105 50  0000 L CNN
F 2 "Luciebox:MountingHole_4.3mm_M4" H 1750 7150 50  0001 C CNN
F 3 "~" H 1750 7150 50  0001 C CNN
	1    1750 7150
	1    0    0    -1  
$EndComp
Wire Wire Line
	4000 1900 4000 3000
Wire Wire Line
	4000 3000 4400 3000
Wire Wire Line
	4400 3000 4400 4800
Wire Wire Line
	4400 4800 4050 4800
Connection ~ 4000 1900
Connection ~ 4050 4800
Wire Wire Line
	7350 4200 6450 4200
Wire Wire Line
	6450 4200 6450 4800
Wire Wire Line
	7350 4100 6550 4100
Wire Wire Line
	6550 4100 6550 4800
Wire Wire Line
	7350 4500 6650 4500
Wire Wire Line
	6650 4500 6650 4800
Wire Wire Line
	7350 4000 6750 4000
Wire Wire Line
	6750 4000 6750 4800
Wire Wire Line
	7350 4400 6850 4400
Wire Wire Line
	6850 4400 6850 4800
Wire Wire Line
	9550 4500 9550 4750
Wire Wire Line
	9550 4750 6950 4750
Wire Wire Line
	6950 4750 6950 4800
Wire Wire Line
	9550 4200 9900 4200
Wire Wire Line
	9900 4200 9900 5050
Wire Wire Line
	10000 5050 10000 3350
Wire Wire Line
	10000 3350 7050 3350
Wire Wire Line
	7050 3350 7050 3800
Wire Wire Line
	7050 3800 7350 3800
Wire Wire Line
	10100 5050 10100 3200
Wire Wire Line
	10100 3200 6900 3200
Wire Wire Line
	6900 4300 7350 4300
Wire Wire Line
	6900 3200 6900 4300
Wire Wire Line
	10200 5050 10200 4300
Wire Wire Line
	10200 4300 9550 4300
Wire Wire Line
	10300 5050 10300 4400
Wire Wire Line
	10300 4400 9550 4400
Wire Wire Line
	10400 5050 10400 3050
Wire Wire Line
	10400 3050 6750 3050
Wire Wire Line
	6750 3050 6750 3900
Wire Wire Line
	6750 3900 7350 3900
Text Label 7050 3550 0    50   ~ 0
A
Text Label 7050 3200 0    50   ~ 0
F
Text Label 7200 3050 0    50   ~ 0
B
Text Label 7050 4000 0    50   ~ 0
C
Text Label 6650 4100 0    50   ~ 0
D
Text Label 6450 4200 2    50   ~ 0
E
Text Label 6850 4400 2    50   ~ 0
G
Text Label 7050 4500 0    50   ~ 0
DP
Text Label 9600 4200 0    50   ~ 0
Digit1
Text Label 9600 4300 0    50   ~ 0
Digit2
Text Label 9600 4400 0    50   ~ 0
Digit3
Text Label 9100 4750 0    50   ~ 0
Digit4
$Comp
L power:GND #PWR0101
U 1 1 5FEF5CB7
P 4450 5650
F 0 "#PWR0101" H 4450 5400 50  0001 C CNN
F 1 "GND" H 4455 5477 50  0000 C CNN
F 2 "" H 4450 5650 50  0001 C CNN
F 3 "" H 4450 5650 50  0001 C CNN
	1    4450 5650
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0102
U 1 1 5FEF6BAB
P 4400 2600
F 0 "#PWR0102" H 4400 2350 50  0001 C CNN
F 1 "GND" H 4405 2427 50  0000 C CNN
F 2 "" H 4400 2600 50  0001 C CNN
F 3 "" H 4400 2600 50  0001 C CNN
	1    4400 2600
	1    0    0    -1  
$EndComp
Wire Wire Line
	5200 5100 5200 5500
Wire Wire Line
	4900 4600 4900 5450
Wire Wire Line
	5000 4600 5000 5450
Wire Wire Line
	4450 5650 4450 5450
Wire Wire Line
	4450 5450 4700 5450
Wire Wire Line
	4700 4600 4700 5450
Wire Wire Line
	4700 5450 4700 5500
Connection ~ 4700 5450
Wire Wire Line
	4800 5450 4700 5450
Connection ~ 4800 5450
Wire Wire Line
	4800 5450 4800 5500
Wire Wire Line
	4800 5450 4900 5450
Connection ~ 4900 5450
Wire Wire Line
	4900 5450 4900 5500
Wire Wire Line
	4900 5450 5000 5450
Connection ~ 5000 5450
Wire Wire Line
	5000 5450 5000 5500
Wire Wire Line
	4650 2600 4650 2550
Wire Wire Line
	4400 2600 4400 2550
Wire Wire Line
	4400 2550 4650 2550
Connection ~ 4650 2550
Wire Wire Line
	4650 2550 4650 1800
Wire Wire Line
	4650 2550 4750 2550
Connection ~ 4750 2550
Wire Wire Line
	4750 2550 4750 2600
Wire Wire Line
	4750 2550 4850 2550
Connection ~ 4850 2550
Wire Wire Line
	4850 2550 4850 1900
Wire Wire Line
	4850 2550 4950 2550
Connection ~ 4950 2550
Wire Wire Line
	4950 2550 4950 2600
$Comp
L Luciebox_components:MountingHole H13
U 1 1 5FFBD226
P 1800 7400
F 0 "H13" H 1900 7446 50  0000 L CNN
F 1 "MountingHole" H 1900 7355 50  0000 L CNN
F 2 "Luciebox:MountingHole_4.3mm_M4" H 1800 7400 50  0001 C CNN
F 3 "~" H 1800 7400 50  0001 C CNN
	1    1800 7400
	1    0    0    -1  
$EndComp
$Comp
L Luciebox_components:MountingHole H14
U 1 1 5FFBD56F
P 2100 7400
F 0 "H14" H 2200 7446 50  0000 L CNN
F 1 "MountingHole" H 2200 7355 50  0000 L CNN
F 2 "Luciebox:MountingHole_4.3mm_M4" H 2100 7400 50  0001 C CNN
F 3 "~" H 2100 7400 50  0001 C CNN
	1    2100 7400
	1    0    0    -1  
$EndComp
$Comp
L Luciebox_components:MountingHole H15
U 1 1 5FFC62A6
P 2500 7400
F 0 "H15" H 2600 7446 50  0000 L CNN
F 1 "MountingHole" H 2600 7355 50  0000 L CNN
F 2 "Luciebox:MountingHole_4.3mm_M4" H 2500 7400 50  0001 C CNN
F 3 "~" H 2500 7400 50  0001 C CNN
	1    2500 7400
	1    0    0    -1  
$EndComp
$Comp
L Luciebox_components:MountingHole H16
U 1 1 5FFEBD74
P 1800 7650
F 0 "H16" H 1900 7696 50  0000 L CNN
F 1 "MountingHole" H 1900 7605 50  0000 L CNN
F 2 "Luciebox:MountingHole_4.3mm_M4" H 1800 7650 50  0001 C CNN
F 3 "~" H 1800 7650 50  0001 C CNN
	1    1800 7650
	1    0    0    -1  
$EndComp
$Comp
L Luciebox_components:MountingHole H17
U 1 1 5FFEC049
P 2050 7650
F 0 "H17" H 2150 7696 50  0000 L CNN
F 1 "MountingHole" H 2150 7605 50  0000 L CNN
F 2 "Luciebox:MountingHole_4.3mm_M4" H 2050 7650 50  0001 C CNN
F 3 "~" H 2050 7650 50  0001 C CNN
	1    2050 7650
	1    0    0    -1  
$EndComp
Wire Wire Line
	5400 5400 1350 5400
Wire Wire Line
	1650 5300 1650 4200
Wire Wire Line
	1950 5300 1950 5550
Wire Wire Line
	1950 5550 1450 5550
Wire Wire Line
	4000 5700 4200 5700
Wire Wire Line
	3700 5700 1750 5700
Wire Wire Line
	1650 5650 2800 5650
Wire Wire Line
	3100 5650 3350 5650
Wire Wire Line
	2500 5600 2350 5600
Wire Wire Line
	2050 5600 1550 5600
Wire Wire Line
	3950 2800 4150 2800
Wire Wire Line
	3650 2800 1700 2800
Wire Wire Line
	1600 2750 2750 2750
Wire Wire Line
	3050 2750 3300 2750
Wire Wire Line
	2450 2700 2300 2700
Wire Wire Line
	2000 2700 1500 2700
Wire Wire Line
	1700 2950 1700 2800
Wire Wire Line
	1600 2400 1600 1300
Wire Wire Line
	1900 2400 1900 2650
Wire Wire Line
	1900 2650 1400 2650
$Comp
L Luciebox_components:D_Schottky D2
U 1 1 5FD8A338
P 2150 2700
F 0 "D2" H 2150 2916 50  0000 C CNN
F 1 "D_Schottky" H 2150 2825 50  0000 C CNN
F 2 "Luciebox:smd_throughhole_2_pins" H 2150 2700 50  0001 C CNN
F 3 "~" H 2150 2700 50  0001 C CNN
	1    2150 2700
	1    0    0    -1  
$EndComp
$Comp
L Luciebox_components:D_Schottky D3
U 1 1 5FD8B35D
P 2900 2750
F 0 "D3" H 2900 2966 50  0000 C CNN
F 1 "D_Schottky" H 2900 2875 50  0000 C CNN
F 2 "Luciebox:smd_throughhole_2_pins" H 2900 2750 50  0001 C CNN
F 3 "~" H 2900 2750 50  0001 C CNN
	1    2900 2750
	1    0    0    -1  
$EndComp
$Comp
L Luciebox_components:D_Schottky D4
U 1 1 5FD8B78C
P 3800 2800
F 0 "D4" H 3800 3016 50  0000 C CNN
F 1 "D_Schottky" H 3800 2925 50  0000 C CNN
F 2 "Luciebox:smd_throughhole_2_pins" H 3800 2800 50  0001 C CNN
F 3 "~" H 3800 2800 50  0001 C CNN
	1    3800 2800
	1    0    0    -1  
$EndComp
$Comp
L Luciebox_components:D_Schottky D1
U 1 1 5FD8BA17
P 1750 2400
F 0 "D1" H 1750 2616 50  0000 C CNN
F 1 "D_Schottky" H 1750 2525 50  0000 C CNN
F 2 "Luciebox:smd_throughhole_2_pins" H 1750 2400 50  0001 C CNN
F 3 "~" H 1750 2400 50  0001 C CNN
	1    1750 2400
	-1   0    0    1   
$EndComp
$Comp
L Luciebox_components:D_Schottky D8
U 1 1 5FD8C5EE
P 3850 5700
F 0 "D8" H 3850 5916 50  0000 C CNN
F 1 "D_Schottky" H 3850 5825 50  0000 C CNN
F 2 "Luciebox:smd_throughhole_2_pins" H 3850 5700 50  0001 C CNN
F 3 "~" H 3850 5700 50  0001 C CNN
	1    3850 5700
	1    0    0    -1  
$EndComp
$Comp
L Luciebox_components:D_Schottky D7
U 1 1 5FD8CD51
P 2950 5650
F 0 "D7" H 2950 5866 50  0000 C CNN
F 1 "D_Schottky" H 2950 5775 50  0000 C CNN
F 2 "Luciebox:smd_throughhole_2_pins" H 2950 5650 50  0001 C CNN
F 3 "~" H 2950 5650 50  0001 C CNN
	1    2950 5650
	1    0    0    -1  
$EndComp
$Comp
L Luciebox_components:D_Schottky D6
U 1 1 5FD8D11C
P 2200 5600
F 0 "D6" H 2200 5816 50  0000 C CNN
F 1 "D_Schottky" H 2200 5725 50  0000 C CNN
F 2 "Luciebox:smd_throughhole_2_pins" H 2200 5600 50  0001 C CNN
F 3 "~" H 2200 5600 50  0001 C CNN
	1    2200 5600
	1    0    0    -1  
$EndComp
$Comp
L Luciebox_components:D_Schottky D5
U 1 1 5FD8D54B
P 1800 5300
F 0 "D5" H 1800 5516 50  0000 C CNN
F 1 "D_Schottky" H 1800 5425 50  0000 C CNN
F 2 "Luciebox:smd_throughhole_2_pins" H 1800 5300 50  0001 C CNN
F 3 "~" H 1800 5300 50  0001 C CNN
	1    1800 5300
	-1   0    0    1   
$EndComp
$EndSCHEMATC
