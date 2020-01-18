EESchema Schematic File Version 4
LIBS:Luciebox-cache
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 3 4
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
L Device:R_Small R27
U 1 1 5D7526CF
P 3650 2200
AR Path="/5D74964C/5D7526CF" Ref="R27"  Part="1" 
AR Path="/5D761BCA/5D7526CF" Ref="R36"  Part="1" 
AR Path="/5D9307E8/5D7526CF" Ref="R45"  Part="1" 
F 0 "R45" H 3709 2246 50  0000 L CNN
F 1 "R_Small" H 3709 2155 50  0000 L CNN
F 2 "Luciebox:1206" H 3650 2200 50  0001 C CNN
F 3 "~" H 3650 2200 50  0001 C CNN
	1    3650 2200
	-1   0    0    1   
$EndComp
$Comp
L Device:R_Small R26
U 1 1 5D7526DB
P 3500 2200
AR Path="/5D74964C/5D7526DB" Ref="R26"  Part="1" 
AR Path="/5D761BCA/5D7526DB" Ref="R35"  Part="1" 
AR Path="/5D9307E8/5D7526DB" Ref="R44"  Part="1" 
F 0 "R44" H 3559 2246 50  0000 L CNN
F 1 "R_Small" H 3559 2155 50  0000 L CNN
F 2 "Luciebox:1206" H 3500 2200 50  0001 C CNN
F 3 "~" H 3500 2200 50  0001 C CNN
	1    3500 2200
	-1   0    0    1   
$EndComp
$Comp
L Device:R_Small R25
U 1 1 5D7526E1
P 3350 2200
AR Path="/5D74964C/5D7526E1" Ref="R25"  Part="1" 
AR Path="/5D761BCA/5D7526E1" Ref="R34"  Part="1" 
AR Path="/5D9307E8/5D7526E1" Ref="R43"  Part="1" 
F 0 "R43" H 3409 2246 50  0000 L CNN
F 1 "R_Small" H 3409 2155 50  0000 L CNN
F 2 "Luciebox:1206" H 3350 2200 50  0001 C CNN
F 3 "~" H 3350 2200 50  0001 C CNN
	1    3350 2200
	-1   0    0    1   
$EndComp
$Comp
L Device:R_Small R24
U 1 1 5D7526E7
P 3200 2200
AR Path="/5D74964C/5D7526E7" Ref="R24"  Part="1" 
AR Path="/5D761BCA/5D7526E7" Ref="R33"  Part="1" 
AR Path="/5D9307E8/5D7526E7" Ref="R42"  Part="1" 
F 0 "R42" H 3259 2246 50  0000 L CNN
F 1 "R_Small" H 3259 2155 50  0000 L CNN
F 2 "Luciebox:1206" H 3200 2200 50  0001 C CNN
F 3 "~" H 3200 2200 50  0001 C CNN
	1    3200 2200
	-1   0    0    1   
$EndComp
$Comp
L Device:R_Small R23
U 1 1 5D7526ED
P 3050 2200
AR Path="/5D74964C/5D7526ED" Ref="R23"  Part="1" 
AR Path="/5D761BCA/5D7526ED" Ref="R32"  Part="1" 
AR Path="/5D9307E8/5D7526ED" Ref="R41"  Part="1" 
F 0 "R32" H 3109 2246 50  0000 L CNN
F 1 "R_Small" H 3109 2155 50  0000 L CNN
F 2 "Luciebox:1206" H 3050 2200 50  0001 C CNN
F 3 "~" H 3050 2200 50  0001 C CNN
	1    3050 2200
	-1   0    0    1   
$EndComp
$Comp
L Device:R_Small R22
U 1 1 5D7526F3
P 2900 2200
AR Path="/5D74964C/5D7526F3" Ref="R22"  Part="1" 
AR Path="/5D761BCA/5D7526F3" Ref="R31"  Part="1" 
AR Path="/5D9307E8/5D7526F3" Ref="R40"  Part="1" 
F 0 "R31" H 2959 2246 50  0000 L CNN
F 1 "R_Small" H 2959 2155 50  0000 L CNN
F 2 "Luciebox:1206" H 2900 2200 50  0001 C CNN
F 3 "~" H 2900 2200 50  0001 C CNN
	1    2900 2200
	-1   0    0    1   
$EndComp
$Comp
L Device:R_Small R21
U 1 1 5D7526F9
P 2750 2200
AR Path="/5D74964C/5D7526F9" Ref="R21"  Part="1" 
AR Path="/5D761BCA/5D7526F9" Ref="R30"  Part="1" 
AR Path="/5D9307E8/5D7526F9" Ref="R39"  Part="1" 
F 0 "R30" H 2809 2246 50  0000 L CNN
F 1 "R_Small" H 2809 2155 50  0000 L CNN
F 2 "Luciebox:1206" H 2750 2200 50  0001 C CNN
F 3 "~" H 2750 2200 50  0001 C CNN
	1    2750 2200
	-1   0    0    1   
$EndComp
Wire Wire Line
	3650 2100 3500 2100
Wire Wire Line
	3500 2100 3350 2100
Connection ~ 3500 2100
Wire Wire Line
	3350 2100 3200 2100
Connection ~ 3350 2100
Wire Wire Line
	3200 2100 3050 2100
Connection ~ 3200 2100
Wire Wire Line
	2900 2100 3050 2100
Connection ~ 3050 2100
Wire Wire Line
	2900 2100 2750 2100
Connection ~ 2900 2100
Connection ~ 2750 2100
$Comp
L power:GND #PWR020
U 1 1 5D75277B
P 3850 1900
AR Path="/5D74964C/5D75277B" Ref="#PWR020"  Part="1" 
AR Path="/5D761BCA/5D75277B" Ref="#PWR023"  Part="1" 
AR Path="/5D9307E8/5D75277B" Ref="#PWR026"  Part="1" 
F 0 "#PWR023" H 3850 1650 50  0001 C CNN
F 1 "GND" H 3855 1727 50  0000 C CNN
F 2 "" H 3850 1900 50  0001 C CNN
F 3 "" H 3850 1900 50  0001 C CNN
	1    3850 1900
	1    0    0    -1  
$EndComp
Wire Wire Line
	2750 2300 2750 2350
Wire Wire Line
	2750 2350 2650 2350
Wire Wire Line
	2900 2300 2900 2400
Wire Wire Line
	2900 2400 2750 2400
Wire Wire Line
	2850 2450 3050 2450
Wire Wire Line
	3050 2450 3050 2300
Wire Wire Line
	3200 2300 3200 2500
Wire Wire Line
	3200 2500 2950 2500
Wire Wire Line
	3050 2550 3350 2550
Wire Wire Line
	3350 2550 3350 2300
Wire Wire Line
	3500 2300 3500 2600
Wire Wire Line
	3500 2600 3150 2600
Wire Wire Line
	3250 2650 3650 2650
Wire Wire Line
	3650 2650 3650 2300
Wire Wire Line
	2750 2100 2600 2100
Wire Wire Line
	2600 2300 2550 2300
$Comp
L Device:R_Small R20
U 1 1 5D7527B8
P 2600 2200
AR Path="/5D74964C/5D7527B8" Ref="R20"  Part="1" 
AR Path="/5D761BCA/5D7527B8" Ref="R29"  Part="1" 
AR Path="/5D9307E8/5D7527B8" Ref="R38"  Part="1" 
F 0 "R29" H 2659 2246 50  0000 L CNN
F 1 "R_Small" H 2659 2155 50  0000 L CNN
F 2 "Luciebox:1206" H 2600 2200 50  0001 C CNN
F 3 "~" H 2600 2200 50  0001 C CNN
	1    2600 2200
	-1   0    0    1   
$EndComp
Text HLabel 2450 3700 1    50   Output ~ 0
DAC_analog
$Comp
L Luciebox_components:DAC_R-2R R28
U 1 1 5DABC300
P 2450 3700
AR Path="/5D74964C/5DABC300" Ref="R28"  Part="1" 
AR Path="/5D761BCA/5DABC300" Ref="R37"  Part="1" 
AR Path="/5D9307E8/5DABC300" Ref="R46"  Part="1" 
F 0 "R46" V 2746 4778 50  0000 L CNN
F 1 "DAC_R-2R" V 2837 4778 50  0000 L CNN
F 2 "Luciebox:SIP-10" H 2450 3700 50  0001 C CNN
F 3 "https://www.bourns.com/docs/Product-Datasheets/R2R.pdf" H 2450 3700 50  0001 C CNN
	1    2450 3700
	0    1    1    0   
$EndComp
Wire Wire Line
	2850 2450 2850 3100
Wire Wire Line
	3250 3100 3250 2650
Wire Wire Line
	3150 2600 3150 3100
Wire Wire Line
	3050 3100 3050 2550
Wire Wire Line
	2950 2500 2950 3100
Wire Wire Line
	2750 2400 2750 3100
Wire Wire Line
	2650 2350 2650 3100
Wire Wire Line
	2550 2300 2550 3100
$Comp
L Luciebox_components:terminal_8x1 T4
U 1 1 5D7526D5
P 3250 3050
AR Path="/5D74964C/5D7526D5" Ref="T4"  Part="1" 
AR Path="/5D761BCA/5D7526D5" Ref="T7"  Part="1" 
AR Path="/5D9307E8/5D7526D5" Ref="T10"  Part="1" 
F 0 "T7" V 3765 3650 50  0000 C CNN
F 1 "terminal_8x1" V 3674 3650 50  0000 C CNN
F 2 "Luciebox:Terminal_8x1_100mil" H 3250 3050 50  0001 C CNN
F 3 "" H 3250 3050 50  0001 C CNN
	1    3250 3050
	0    -1   -1   0   
$EndComp
Wire Wire Line
	2550 3100 2550 3700
Connection ~ 2550 3100
Wire Wire Line
	2650 3700 2650 3100
Connection ~ 2650 3100
Wire Wire Line
	2750 3100 2750 3700
Wire Wire Line
	2850 3100 2850 3700
Wire Wire Line
	2950 3100 2950 3700
Wire Wire Line
	3050 3100 3050 3700
Wire Wire Line
	3150 3100 3150 3700
Wire Wire Line
	3250 3100 3250 3700
$Comp
L power:GND #PWR021
U 1 1 5DACA23D
P 3600 3550
AR Path="/5D74964C/5DACA23D" Ref="#PWR021"  Part="1" 
AR Path="/5D761BCA/5DACA23D" Ref="#PWR024"  Part="1" 
AR Path="/5D9307E8/5DACA23D" Ref="#PWR027"  Part="1" 
F 0 "#PWR024" H 3600 3300 50  0001 C CNN
F 1 "GND" H 3605 3377 50  0000 C CNN
F 2 "" H 3600 3550 50  0001 C CNN
F 3 "" H 3600 3550 50  0001 C CNN
	1    3600 3550
	1    0    0    -1  
$EndComp
Wire Wire Line
	3600 3550 3350 3550
Wire Wire Line
	3350 3550 3350 3700
Connection ~ 3250 3100
Connection ~ 3150 3100
Connection ~ 3050 3100
Connection ~ 2950 3100
Connection ~ 2850 3100
Connection ~ 2750 3100
$Comp
L Luciebox_components:terminal_8x1 T3
U 1 1 5DAEA72B
P 3300 1700
AR Path="/5D74964C/5DAEA72B" Ref="T3"  Part="1" 
AR Path="/5D761BCA/5DAEA72B" Ref="T6"  Part="1" 
AR Path="/5D9307E8/5DAEA72B" Ref="T9"  Part="1" 
F 0 "T6" V 3815 2300 50  0000 C CNN
F 1 "terminal_8x1" V 3724 2300 50  0000 C CNN
F 2 "Luciebox:Terminal_8x1_100mil" H 3300 1700 50  0001 C CNN
F 3 "" H 3300 1700 50  0001 C CNN
	1    3300 1700
	0    -1   -1   0   
$EndComp
Connection ~ 2600 2100
Wire Wire Line
	2700 1900 2750 1900
Wire Wire Line
	2750 1900 2750 2100
Wire Wire Line
	2800 2050 2900 2050
Wire Wire Line
	2900 2050 2900 2100
Wire Wire Line
	2900 2000 3050 2000
Wire Wire Line
	3050 2000 3050 2100
Wire Wire Line
	3000 1950 3200 1950
Wire Wire Line
	3200 1950 3200 2100
Wire Wire Line
	2600 1750 2600 2100
Wire Wire Line
	2700 1900 2700 1750
Wire Wire Line
	2800 1750 2800 2050
Wire Wire Line
	2900 1750 2900 2000
Wire Wire Line
	3000 1750 3000 1950
Wire Wire Line
	3100 1750 3100 1900
Wire Wire Line
	3100 1900 3350 1900
Wire Wire Line
	3350 1900 3350 2100
Wire Wire Line
	3200 1750 3200 1850
Wire Wire Line
	3200 1850 3500 1850
Wire Wire Line
	3500 1850 3500 2100
Wire Wire Line
	3300 1750 3300 1800
Wire Wire Line
	3300 1800 3650 1800
Wire Wire Line
	3650 1800 3650 2100
Connection ~ 3650 2100
Wire Wire Line
	3850 1900 3750 1900
Wire Wire Line
	3750 1900 3750 2100
Wire Wire Line
	3750 2100 3650 2100
$Comp
L Luciebox_components:terminal_8x1 T2
U 1 1 5DB0039D
P 3300 950
AR Path="/5D74964C/5DB0039D" Ref="T2"  Part="1" 
AR Path="/5D761BCA/5DB0039D" Ref="T5"  Part="1" 
AR Path="/5D9307E8/5DB0039D" Ref="T8"  Part="1" 
F 0 "T5" V 3815 1550 50  0000 C CNN
F 1 "terminal_8x1" V 3724 1550 50  0000 C CNN
F 2 "Luciebox:Terminal_8x1_100mil" H 3300 950 50  0001 C CNN
F 3 "" H 3300 950 50  0001 C CNN
	1    3300 950 
	0    -1   -1   0   
$EndComp
Wire Wire Line
	3300 1000 3200 1000
Connection ~ 2700 1000
Wire Wire Line
	2700 1000 2600 1000
Connection ~ 2800 1000
Wire Wire Line
	2800 1000 2700 1000
Connection ~ 2900 1000
Wire Wire Line
	2900 1000 2800 1000
Connection ~ 3000 1000
Wire Wire Line
	3000 1000 2900 1000
Connection ~ 3100 1000
Wire Wire Line
	3100 1000 3000 1000
Connection ~ 3200 1000
Wire Wire Line
	3200 1000 3100 1000
Connection ~ 3300 1000
$Comp
L Luciebox_components:VCC_minimalCurrent #PWR025
U 1 1 5DB05874
P 3750 1000
AR Path="/5D9307E8/5DB05874" Ref="#PWR025"  Part="1" 
AR Path="/5D74964C/5DB05874" Ref="#PWR019"  Part="1" 
AR Path="/5D761BCA/5DB05874" Ref="#PWR022"  Part="1" 
F 0 "#PWR022" H 3750 850 50  0001 C CNN
F 1 "VCC_minimalCurrent" H 3767 1173 50  0000 C CNN
F 2 "" H 3750 1000 50  0001 C CNN
F 3 "" H 3750 1000 50  0001 C CNN
	1    3750 1000
	1    0    0    -1  
$EndComp
Wire Wire Line
	3300 1000 3750 1000
$EndSCHEMATC
