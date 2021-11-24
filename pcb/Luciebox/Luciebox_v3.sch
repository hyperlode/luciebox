EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 2
Title ""
Date "lun. 30 mars 2015"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text Label 8950 1450 1    60   ~ 0
Vin
Text Label 9350 1450 1    60   ~ 0
IOREF
Text Label 8900 2500 0    60   ~ 0
A0
Text Label 8900 2600 0    60   ~ 0
A1
Text Label 8900 2700 0    60   ~ 0
A2
Text Label 8900 2800 0    60   ~ 0
A3
Text Label 8900 2900 0    60   ~ 0
A4(SDA)
Text Label 8900 3000 0    60   ~ 0
A5(SCL)
Text Label 10550 3000 0    60   ~ 0
0(Rx)
Text Label 10550 2800 0    60   ~ 0
2
Text Label 10550 2900 0    60   ~ 0
1(Tx)
Text Label 10550 2700 0    60   ~ 0
3(PWM)
Text Label 10550 2600 0    60   ~ 0
4
Text Label 10550 2500 0    60   ~ 0
5(PWM)
Text Label 10550 2400 0    60   ~ 0
6(PWM)
Text Label 10550 2300 0    60   ~ 0
7
Text Label 10550 2100 0    60   ~ 0
8
Text Label 10550 2000 0    60   ~ 0
9(PWM)
Text Label 10550 1900 0    60   ~ 0
10(PWM-SS)
Text Label 10550 1700 0    60   ~ 0
12(MISO)
Text Label 10550 1600 0    60   ~ 0
13(SCK)
Text Label 10550 1400 0    60   ~ 0
AREF
NoConn ~ 9400 1600
Text Label 10550 1300 0    60   ~ 0
A4(SDA)
Text Label 10550 1200 0    60   ~ 0
A5(SCL)
Text Notes 10850 1000 0    60   ~ 0
Holes
Text Notes 8550 750  0    60   ~ 0
Shield for Arduino that uses\nthe same pin disposition\nlike "Uno" board Rev 3.
Text Label 8650 1800 0    60   ~ 0
Reset
$Comp
L Luciebox_components:+3.3V-power #PWR020
U 1 1 56D70538
P 9150 1450
F 0 "#PWR020" H 9150 1300 50  0001 C CNN
F 1 "+3.3V" V 9150 1700 50  0000 C CNN
F 2 "" H 9150 1450 50  0000 C CNN
F 3 "" H 9150 1450 50  0000 C CNN
	1    9150 1450
	1    0    0    -1  
$EndComp
$Comp
L Luciebox_components:VCC-power #PWR019
U 1 1 56D707BB
P 9050 1350
F 0 "#PWR019" H 9050 1200 50  0001 C CNN
F 1 "VCC" V 9050 1550 50  0000 C CNN
F 2 "" H 9050 1350 50  0000 C CNN
F 3 "" H 9050 1350 50  0000 C CNN
	1    9050 1350
	1    0    0    -1  
$EndComp
$Comp
L Luciebox_components:GND-power #PWR021
U 1 1 56D70CC2
P 9300 3150
F 0 "#PWR021" H 9300 2900 50  0001 C CNN
F 1 "GND" H 9300 3000 50  0000 C CNN
F 2 "" H 9300 3150 50  0000 C CNN
F 3 "" H 9300 3150 50  0000 C CNN
	1    9300 3150
	1    0    0    -1  
$EndComp
$Comp
L Luciebox_components:GND-power #PWR023
U 1 1 56D70CFF
P 10300 3150
F 0 "#PWR023" H 10300 2900 50  0001 C CNN
F 1 "GND" H 10300 3000 50  0000 C CNN
F 2 "" H 10300 3150 50  0000 C CNN
F 3 "" H 10300 3150 50  0000 C CNN
	1    10300 3150
	1    0    0    -1  
$EndComp
NoConn ~ 10800 850 
NoConn ~ 10900 850 
NoConn ~ 11000 850 
NoConn ~ 11100 850 
Wire Notes Line
	8525 825  9925 825 
Wire Notes Line
	9925 825  9925 475 
Wire Wire Line
	9350 1450 9350 1700
Wire Wire Line
	9350 1700 9400 1700
Wire Wire Line
	9400 1900 9150 1900
Wire Wire Line
	9400 2000 9050 2000
Wire Wire Line
	9400 2300 8950 2300
Wire Wire Line
	9400 2100 9300 2100
Wire Wire Line
	9400 2200 9300 2200
Connection ~ 9300 2200
Wire Wire Line
	8950 2300 8950 1450
Wire Wire Line
	9050 2000 9050 1350
Wire Wire Line
	9150 1900 9150 1450
Wire Wire Line
	9400 2500 8900 2500
Wire Wire Line
	9400 2600 8900 2600
Wire Wire Line
	9400 2700 8900 2700
Wire Wire Line
	9400 2800 8900 2800
Wire Wire Line
	9400 2900 8900 2900
Wire Wire Line
	9400 3000 8900 3000
Wire Wire Line
	10200 2100 10550 2100
Wire Wire Line
	10200 2000 10550 2000
Wire Wire Line
	10200 1900 10550 1900
Wire Wire Line
	10200 1700 10550 1700
Wire Wire Line
	10200 1600 10550 1600
Wire Wire Line
	10200 1400 10550 1400
Wire Wire Line
	10200 1300 10550 1300
Wire Wire Line
	10200 1200 10550 1200
Wire Wire Line
	10200 3000 10550 3000
Wire Wire Line
	10200 2900 10550 2900
Wire Wire Line
	10200 2800 10550 2800
Wire Wire Line
	10200 2700 10550 2700
Wire Wire Line
	10200 2600 10550 2600
Wire Wire Line
	10200 2500 10550 2500
Wire Wire Line
	10200 2400 10550 2400
Wire Wire Line
	10200 2300 10550 2300
Wire Wire Line
	10200 1500 10300 1500
Wire Wire Line
	10300 1500 10300 3150
Wire Wire Line
	9300 2100 9300 2200
Wire Wire Line
	9300 2200 9300 3150
Wire Notes Line
	8500 500  8500 3450
Wire Notes Line
	8500 3450 11200 3450
Wire Wire Line
	9400 1800 8650 1800
Text Notes 9700 1600 0    60   ~ 0
1
Wire Notes Line
	11200 1000 10700 1000
Wire Notes Line
	10700 1000 10700 500 
Wire Wire Line
	7000 1600 7000 2300
Wire Wire Line
	6050 1500 7250 1500
Wire Wire Line
	7250 1500 7250 2300
Wire Wire Line
	6050 1400 7500 1400
Wire Wire Line
	7500 1400 7500 2300
Text Label 10550 1800 0    60   ~ 0
11(PWM-MOSI)
Wire Wire Line
	10200 1800 10550 1800
Text Label 7500 2500 3    50   ~ 0
11(PWM-MOSI)
Text Label 7250 2500 3    50   ~ 0
10(PWM-SS)
Text Label 7000 2500 3    50   ~ 0
9(PWM)
Wire Wire Line
	3850 1000 3800 1000
Wire Wire Line
	3850 1100 3750 1100
Text Label 3350 1400 2    50   ~ 0
12(MISO)
Text Label 3350 1500 2    50   ~ 0
13(SCK)
Text Label 3350 1700 2    50   ~ 0
0(Rx)
Wire Wire Line
	3350 1000 3750 1000
Connection ~ 3800 1000
Wire Wire Line
	3350 1100 3650 1100
Connection ~ 3750 1100
Wire Wire Line
	3350 1200 3700 1200
Wire Wire Line
	3350 1300 3650 1300
Wire Wire Line
	3350 1400 3600 1400
Wire Wire Line
	3350 1500 3550 1500
Wire Wire Line
	3350 1600 3500 1600
Connection ~ 3700 1200
Wire Wire Line
	3700 1200 3850 1200
Connection ~ 3650 1300
Wire Wire Line
	3650 1300 3850 1300
Connection ~ 3600 1400
Wire Wire Line
	3600 1400 3850 1400
Connection ~ 3550 1500
Wire Wire Line
	3550 1500 3850 1500
Connection ~ 3500 1600
Wire Wire Line
	3500 1600 3850 1600
Text Label 6750 2500 3    50   ~ 0
5(PWM)
Wire Wire Line
	6750 1700 6750 2300
Wire Wire Line
	6050 1700 6750 1700
Text Label 6500 2500 3    50   ~ 0
6(PWM)
Wire Wire Line
	1050 4900 1100 4900
Wire Wire Line
	1650 4900 1700 4900
Wire Wire Line
	1350 4900 1400 4900
Wire Wire Line
	1950 4900 2000 4900
Wire Wire Line
	2250 4900 2300 4900
Wire Wire Line
	2850 4900 2900 4900
Wire Wire Line
	2550 4900 2600 4900
Wire Wire Line
	3150 4900 3200 4900
Wire Wire Line
	3450 4900 3500 4900
Wire Wire Line
	4050 4900 4100 4900
Wire Wire Line
	3750 4900 3800 4900
Wire Wire Line
	4350 4900 4400 4900
Wire Wire Line
	850  4900 800  4900
$Comp
L Luciebox_components:GND-power #PWR01
U 1 1 5D7F94C7
P 700 4900
F 0 "#PWR01" H 700 4650 50  0001 C CNN
F 1 "GND" H 705 4727 50  0000 C CNN
F 2 "" H 700 4900 50  0001 C CNN
F 3 "" H 700 4900 50  0001 C CNN
	1    700  4900
	1    0    0    -1  
$EndComp
$Comp
L Luciebox_components:VCC_minimalCurrent #PWR010
U 1 1 5D800E0F
P 4800 4900
F 0 "#PWR010" H 4800 4750 50  0001 C CNN
F 1 "VCC_minimalCurrent" H 4817 5073 50  0000 C CNN
F 2 "" H 4800 4900 50  0001 C CNN
F 3 "" H 4800 4900 50  0001 C CNN
	1    4800 4900
	1    0    0    -1  
$EndComp
Wire Wire Line
	2100 4550 2100 4600
Wire Wire Line
	2100 4600 1100 4600
Wire Wire Line
	1100 4600 1100 4900
Connection ~ 1100 4900
Wire Wire Line
	1100 4900 1150 4900
Wire Wire Line
	2200 4550 2200 4650
Wire Wire Line
	2200 4650 1400 4650
Wire Wire Line
	1400 4650 1400 4900
Connection ~ 1400 4900
Wire Wire Line
	1400 4900 1450 4900
Wire Wire Line
	1700 4900 1700 4700
Wire Wire Line
	1700 4700 2300 4700
Wire Wire Line
	2300 4700 2300 4550
Connection ~ 1700 4900
Wire Wire Line
	1700 4900 1750 4900
Wire Wire Line
	2400 4550 2400 4750
Wire Wire Line
	2400 4750 2000 4750
Wire Wire Line
	2000 4750 2000 4900
Connection ~ 2000 4900
Wire Wire Line
	2000 4900 2050 4900
Wire Wire Line
	2300 4900 2300 4800
Wire Wire Line
	2300 4800 2500 4800
Wire Wire Line
	2500 4800 2500 4550
Connection ~ 2300 4900
Wire Wire Line
	2300 4900 2350 4900
Wire Wire Line
	2600 4550 2600 4900
Connection ~ 2600 4900
Wire Wire Line
	2600 4900 2650 4900
Wire Wire Line
	2700 4550 2700 4800
Wire Wire Line
	2700 4800 2900 4800
Wire Wire Line
	2900 4800 2900 4900
Connection ~ 2900 4900
Wire Wire Line
	2900 4900 2950 4900
Wire Wire Line
	2800 4550 2800 4750
Wire Wire Line
	2800 4750 3200 4750
Wire Wire Line
	3200 4750 3200 4900
Connection ~ 3200 4900
Wire Wire Line
	3200 4900 3250 4900
Wire Wire Line
	2900 4550 2900 4700
Wire Wire Line
	2900 4700 3500 4700
Wire Wire Line
	3500 4700 3500 4900
Connection ~ 3500 4900
Wire Wire Line
	3500 4900 3550 4900
Wire Wire Line
	3000 4550 3000 4650
Wire Wire Line
	3000 4650 3800 4650
Wire Wire Line
	3800 4650 3800 4900
Connection ~ 3800 4900
Wire Wire Line
	3800 4900 3850 4900
Wire Wire Line
	3100 4550 3100 4600
Wire Wire Line
	3100 4600 4100 4600
Wire Wire Line
	4100 4600 4100 4900
Connection ~ 4100 4900
Wire Wire Line
	4100 4900 4150 4900
Wire Wire Line
	4400 4900 4400 4550
Wire Wire Line
	4400 4550 3200 4550
Connection ~ 4400 4900
Wire Wire Line
	4400 4900 4600 4900
Text Label 2000 4000 2    50   ~ 0
A0
$Comp
L Luciebox_components:GND-power #PWR03
U 1 1 5D90E4DE
P 1100 6200
F 0 "#PWR03" H 1100 5950 50  0001 C CNN
F 1 "GND" H 1105 6027 50  0000 C CNN
F 2 "" H 1100 6200 50  0001 C CNN
F 3 "" H 1100 6200 50  0001 C CNN
	1    1100 6200
	1    0    0    -1  
$EndComp
Wire Wire Line
	1300 6850 1150 6850
Wire Wire Line
	1500 6850 2000 6850
$Comp
L Luciebox_components:GND-power #PWR05
U 1 1 5D974320
P 2000 6850
F 0 "#PWR05" H 2000 6600 50  0001 C CNN
F 1 "GND" H 2005 6677 50  0000 C CNN
F 2 "" H 2000 6850 50  0001 C CNN
F 3 "" H 2000 6850 50  0001 C CNN
	1    2000 6850
	1    0    0    -1  
$EndComp
Text Label 1150 6850 2    50   ~ 0
A5(SCL)
$Comp
L Luciebox_components:GND-power #PWR017
U 1 1 5D988298
P 7900 6450
F 0 "#PWR017" H 7900 6200 50  0001 C CNN
F 1 "GND" H 7905 6277 50  0000 C CNN
F 2 "" H 7900 6450 50  0001 C CNN
F 3 "" H 7900 6450 50  0001 C CNN
	1    7900 6450
	1    0    0    -1  
$EndComp
Text Label 8500 3750 0    50   ~ 0
8
Text Label 8500 3850 0    50   ~ 0
9(PWM)
Text Label 8500 3950 0    50   ~ 0
10(PWM-SS)
Text Label 8500 4050 0    50   ~ 0
11(PWM-MOSI)
Text Label 8500 4150 0    50   ~ 0
12(MISO)
Text Label 8500 4250 0    50   ~ 0
13(SCK)
Text Label 8500 4650 0    50   ~ 0
A0
Text Label 8500 4750 0    50   ~ 0
A1
Text Label 8500 4850 0    50   ~ 0
A2
Text Label 8500 4950 0    50   ~ 0
A3
Text Label 8500 5050 0    50   ~ 0
A4(SDA)
Text Label 8500 5150 0    50   ~ 0
A5(SCL)
Text Label 8500 5450 0    50   ~ 0
0(Rx)
Text Label 8500 5550 0    50   ~ 0
1(Tx)
Text Label 8500 5650 0    50   ~ 0
2
Text Label 8500 5750 0    50   ~ 0
3(PWM)
Text Label 8500 5850 0    50   ~ 0
4
Text Label 8500 5950 0    50   ~ 0
5(PWM)
Text Label 8500 6050 0    50   ~ 0
6(PWM)
Text Label 8500 6150 0    50   ~ 0
7
$Comp
L Luciebox_components:VCC-power #PWR016
U 1 1 5D991DF2
P 7900 3450
F 0 "#PWR016" H 7900 3300 50  0001 C CNN
F 1 "VCC" H 7917 3623 50  0000 C CNN
F 2 "" H 7900 3450 50  0001 C CNN
F 3 "" H 7900 3450 50  0001 C CNN
	1    7900 3450
	1    0    0    -1  
$EndComp
$Comp
L Luciebox_components:VCC-power #PWR018
U 1 1 5D992AD7
P 8000 3450
F 0 "#PWR018" H 8000 3300 50  0001 C CNN
F 1 "VCC" H 8017 3623 50  0000 C CNN
F 2 "" H 8000 3450 50  0001 C CNN
F 3 "" H 8000 3450 50  0001 C CNN
	1    8000 3450
	1    0    0    -1  
$EndComp
$Comp
L Luciebox_components:VCC-power #PWR015
U 1 1 5D993297
P 7300 3750
F 0 "#PWR015" H 7300 3600 50  0001 C CNN
F 1 "VCC" H 7317 3923 50  0000 C CNN
F 2 "" H 7300 3750 50  0001 C CNN
F 3 "" H 7300 3750 50  0001 C CNN
	1    7300 3750
	1    0    0    -1  
$EndComp
Wire Wire Line
	8500 4350 9300 4350
Connection ~ 9300 4350
Wire Wire Line
	9300 4350 9750 4350
Wire Wire Line
	9750 4650 9300 4650
Wire Wire Line
	9300 4650 9300 4550
Wire Wire Line
	9300 4550 8800 4550
Wire Wire Line
	8800 4550 8800 4450
Wire Wire Line
	8800 4450 8500 4450
Connection ~ 9300 4550
Wire Wire Line
	9750 4350 10150 4350
Connection ~ 9750 4350
Wire Wire Line
	10150 4700 9750 4700
Wire Wire Line
	9750 4700 9750 4650
Connection ~ 9750 4650
Wire Wire Line
	10350 4350 10500 4350
Wire Wire Line
	10500 4350 10500 4500
Wire Wire Line
	10500 4700 10350 4700
$Comp
L Luciebox_components:GND-power #PWR024
U 1 1 5DA1D313
P 10700 4500
F 0 "#PWR024" H 10700 4250 50  0001 C CNN
F 1 "GND-power" H 10700 4350 50  0000 C CNN
F 2 "" H 10700 4500 50  0000 C CNN
F 3 "" H 10700 4500 50  0000 C CNN
	1    10700 4500
	1    0    0    -1  
$EndComp
Wire Wire Line
	10700 4500 10500 4500
Connection ~ 10500 4500
Wire Wire Line
	10500 4500 10500 4700
Wire Wire Line
	7900 3450 7050 3450
Wire Wire Line
	7050 3450 7050 3400
Connection ~ 7900 3450
$Comp
L Luciebox_components:GND-power #PWR014
U 1 1 5DA38582
P 6700 3400
F 0 "#PWR014" H 6700 3150 50  0001 C CNN
F 1 "GND" H 6705 3227 50  0000 C CNN
F 2 "" H 6700 3400 50  0001 C CNN
F 3 "" H 6700 3400 50  0001 C CNN
	1    6700 3400
	1    0    0    -1  
$EndComp
Wire Wire Line
	6700 3400 6850 3400
Wire Wire Line
	9400 5250 9250 5250
$Comp
L Luciebox_components:VCC-power #PWR022
U 1 1 5DA5FE62
P 9900 5250
F 0 "#PWR022" H 9900 5100 50  0001 C CNN
F 1 "VCC" H 9917 5423 50  0000 C CNN
F 2 "" H 9900 5250 50  0001 C CNN
F 3 "" H 9900 5250 50  0001 C CNN
	1    9900 5250
	1    0    0    -1  
$EndComp
Wire Wire Line
	9600 5250 9900 5250
Wire Wire Line
	10700 5250 9900 5250
Connection ~ 9900 5250
Wire Wire Line
	10500 4700 10500 5350
Wire Wire Line
	10500 5350 10700 5350
Connection ~ 10500 4700
Text Label 10700 5450 2    50   ~ 0
1(Tx)
Text Label 10700 5550 2    50   ~ 0
0(Rx)
Wire Wire Line
	10700 5650 9250 5650
Wire Wire Line
	9250 5650 9250 5250
Connection ~ 9250 5250
Wire Wire Line
	9250 5250 8500 5250
Wire Wire Line
	3100 6700 3100 6900
Connection ~ 3100 6900
Text Label 3100 6500 0    50   ~ 0
A4(SDA)
$Comp
L Luciebox_components:GND-power #PWR09
U 1 1 5D70EC69
P 3950 6400
F 0 "#PWR09" H 3950 6150 50  0001 C CNN
F 1 "GND" H 3955 6227 50  0000 C CNN
F 2 "" H 3950 6400 50  0001 C CNN
F 3 "" H 3950 6400 50  0001 C CNN
	1    3950 6400
	1    0    0    -1  
$EndComp
Wire Wire Line
	3800 7100 3100 7100
Wire Wire Line
	3950 6400 3800 6400
Wire Wire Line
	3800 6400 3800 6650
Wire Wire Line
	3800 7100 3800 7550
Connection ~ 3800 7100
Wire Wire Line
	4900 6650 3800 6650
Wire Wire Line
	3800 6650 3800 6700
Wire Wire Line
	5700 6950 5350 6950
Wire Wire Line
	4900 6950 4900 6650
Wire Wire Line
	5700 7050 5250 7050
Wire Wire Line
	4900 7550 3800 7550
Wire Wire Line
	5700 7250 5150 7250
$Comp
L Luciebox_components:VCC-power #PWR011
U 1 1 5D7FF3DB
P 5000 7150
F 0 "#PWR011" H 5000 7000 50  0001 C CNN
F 1 "VCC" H 5018 7323 50  0000 C CNN
F 2 "" H 5000 7150 50  0001 C CNN
F 3 "" H 5000 7150 50  0001 C CNN
	1    5000 7150
	-1   0    0    1   
$EndComp
Text Notes 6050 7450 0    50   ~ 0
BAT+
Text Notes 6050 7350 0    50   ~ 0
BAT-
Text Notes 6050 7150 0    50   ~ 0
SWITCH_BATTERY_POWER
Text Notes 6050 7250 0    50   ~ 0
SWITCH_BATTERY_POWER
Text Notes 6050 6950 0    50   ~ 0
GND-EXTERNAL
Text Notes 6050 7050 0    50   ~ 0
GND-SWITCHED
Text Notes 3050 6350 0    50   ~ 0
Touch ON and Auto power OFF 
Wire Wire Line
	3100 6900 3550 6900
Wire Wire Line
	5400 4000 5400 4100
Wire Wire Line
	5400 4100 5400 4200
Connection ~ 5400 4100
Wire Wire Line
	5400 4200 5400 4300
Connection ~ 5400 4200
Wire Wire Line
	5400 4300 5400 4400
Connection ~ 5400 4300
Wire Wire Line
	5400 4400 5400 4500
Connection ~ 5400 4400
Wire Wire Line
	5400 4500 5400 4600
Connection ~ 5400 4500
Wire Wire Line
	5400 4600 5400 4700
Connection ~ 5400 4600
Wire Wire Line
	6450 4700 6450 4600
Wire Wire Line
	6450 4600 6500 4600
Wire Wire Line
	6500 4600 6500 4500
Wire Wire Line
	6500 4500 6450 4500
Connection ~ 6450 4600
Wire Wire Line
	6450 4500 6450 4400
Connection ~ 6450 4500
Wire Wire Line
	6450 4400 6450 4300
Connection ~ 6450 4400
Wire Wire Line
	6450 4300 6450 4200
Connection ~ 6450 4300
Wire Wire Line
	6450 4200 6450 4100
Connection ~ 6450 4200
Wire Wire Line
	6450 4100 6450 4000
Connection ~ 6450 4100
Wire Wire Line
	5550 4000 5550 4100
Wire Wire Line
	5550 4100 5550 4200
Connection ~ 5550 4100
Wire Wire Line
	5550 4200 5550 4300
Connection ~ 5550 4200
Wire Wire Line
	5700 4000 5700 4100
Wire Wire Line
	5700 4100 5700 4200
Connection ~ 5700 4100
Wire Wire Line
	5700 4200 5700 4300
Connection ~ 5700 4200
Wire Wire Line
	5850 4000 5850 4100
Wire Wire Line
	5850 4100 5850 4200
Connection ~ 5850 4100
Wire Wire Line
	5850 4200 5850 4300
Connection ~ 5850 4200
Wire Wire Line
	6000 4300 6000 4200
Wire Wire Line
	6000 4200 6000 4100
Connection ~ 6000 4200
Wire Wire Line
	6000 4100 6000 4000
Connection ~ 6000 4100
Wire Wire Line
	6150 4000 6150 4100
Wire Wire Line
	6150 4100 6150 4200
Connection ~ 6150 4100
Wire Wire Line
	6150 4200 6150 4300
Connection ~ 6150 4200
Wire Wire Line
	6300 4300 6300 4200
Wire Wire Line
	6300 4200 6300 4100
Connection ~ 6300 4200
Wire Wire Line
	6300 4100 6300 4000
Connection ~ 6300 4100
Wire Wire Line
	5550 4400 5550 4500
Wire Wire Line
	5550 4600 5550 4700
Wire Wire Line
	5700 4700 5700 4600
Wire Wire Line
	5700 4500 5700 4400
Wire Wire Line
	5850 4400 5850 4500
Wire Wire Line
	5850 4600 5850 4700
Wire Wire Line
	6000 4700 6000 4600
Wire Wire Line
	6000 4500 6000 4400
Wire Wire Line
	6150 4400 6150 4500
Wire Wire Line
	6150 4600 6150 4700
Wire Wire Line
	6300 4700 6300 4600
Wire Wire Line
	6300 4500 6300 4400
$Comp
L Luciebox_components:GND-power #PWR013
U 1 1 5DCBF971
P 6450 4700
F 0 "#PWR013" H 6450 4450 50  0001 C CNN
F 1 "GND" H 6455 4527 50  0000 C CNN
F 2 "" H 6450 4700 50  0001 C CNN
F 3 "" H 6450 4700 50  0001 C CNN
	1    6450 4700
	1    0    0    -1  
$EndComp
$Comp
L Luciebox_components:VCC-power #PWR012
U 1 1 5DCC8327
P 5400 4000
F 0 "#PWR012" H 5400 3850 50  0001 C CNN
F 1 "VCC" H 5417 4173 50  0000 C CNN
F 2 "" H 5400 4000 50  0001 C CNN
F 3 "" H 5400 4000 50  0001 C CNN
	1    5400 4000
	1    0    0    -1  
$EndComp
Connection ~ 5400 4000
Connection ~ 6450 4700
$Comp
L Luciebox_components:VCC-power #PWR04
U 1 1 5E19FB8B
P 1550 2400
F 0 "#PWR04" H 1550 2250 50  0001 C CNN
F 1 "VCC" H 1567 2573 50  0000 C CNN
F 2 "" H 1550 2400 50  0001 C CNN
F 3 "" H 1550 2400 50  0001 C CNN
	1    1550 2400
	0    1    1    0   
$EndComp
$Comp
L Luciebox_components:VCC_minimalCurrent #PWR02
U 1 1 5E1A5201
P 950 2350
F 0 "#PWR02" H 950 2200 50  0001 C CNN
F 1 "VCC_minimalCurrent" H 967 2523 50  0000 C CNN
F 2 "" H 950 2350 50  0001 C CNN
F 3 "" H 950 2350 50  0001 C CNN
	1    950  2350
	1    0    0    -1  
$EndComp
Wire Wire Line
	1350 2400 950  2400
Wire Wire Line
	950  2400 950  2350
Connection ~ 3800 6650
Wire Wire Line
	4400 6000 4200 6000
Wire Wire Line
	3700 6000 3900 6000
Wire Wire Line
	4800 5350 4800 5400
Wire Wire Line
	4800 5350 4700 5350
Wire Wire Line
	4700 5350 4700 5400
Connection ~ 4800 5350
Wire Wire Line
	4050 5400 4400 5400
Wire Wire Line
	4400 5400 4400 5600
Wire Wire Line
	4050 5500 4350 5500
Wire Wire Line
	4350 5500 4350 5700
Wire Wire Line
	4350 5700 4400 5700
Wire Wire Line
	4400 5800 4150 5800
Wire Wire Line
	4150 5800 4150 5600
Wire Wire Line
	4150 5600 4050 5600
Wire Wire Line
	3350 1700 3450 1700
Connection ~ 3450 1700
Wire Wire Line
	3450 1700 3850 1700
Text Label 1300 1200 0    50   ~ 0
A4(SDA)
Text Label 3350 1300 2    50   ~ 0
8
Text Label 3350 1200 2    50   ~ 0
4
Text Label 3350 1000 2    50   ~ 0
7
$Sheet
S 800  800  500  800 
U 5D9307E8
F0 "Tilt switches" 50
F1 "R2-R_ladder_dac_tilt_switches.sch" 50
F2 "DAC_analog_out" O R 1300 1200 50 
$EndSheet
Connection ~ 5000 3400
Wire Wire Line
	5000 3300 5000 3400
Wire Wire Line
	4950 3300 5000 3300
Connection ~ 5000 3500
Wire Wire Line
	5000 3400 5000 3500
Wire Wire Line
	4950 3400 5000 3400
Connection ~ 5000 3600
Wire Wire Line
	5000 3500 5000 3600
Wire Wire Line
	4950 3500 5000 3500
Connection ~ 5000 3700
Wire Wire Line
	5000 3600 5000 3700
Wire Wire Line
	4950 3600 5000 3600
Connection ~ 5000 3800
Wire Wire Line
	5000 3700 4950 3700
Wire Wire Line
	5000 3800 5000 3700
Connection ~ 5000 3900
Wire Wire Line
	5000 3800 5000 3900
Wire Wire Line
	4950 3800 5000 3800
Wire Wire Line
	4950 3900 5000 3900
Wire Wire Line
	4950 4000 5000 4000
Wire Wire Line
	5000 3900 5000 4000
Wire Wire Line
	5000 2300 5000 3050
Connection ~ 5000 3300
Wire Wire Line
	5000 2300 6500 2300
Text Label 2450 1950 0    50   ~ 0
A1
Text Label 2450 2950 0    50   ~ 0
A2
Connection ~ 5000 2300
$Comp
L Luciebox_components:GND-power #PWR07
U 1 1 5FFAF02F
P 3150 1750
F 0 "#PWR07" H 3150 1500 50  0001 C CNN
F 1 "GND" H 3155 1577 50  0000 C CNN
F 2 "" H 3150 1750 50  0001 C CNN
F 3 "" H 3150 1750 50  0001 C CNN
	1    3150 1750
	1    0    0    -1  
$EndComp
$Comp
L Luciebox_components:GND-power #PWR08
U 1 1 5FFAF53B
P 3150 2750
F 0 "#PWR08" H 3150 2500 50  0001 C CNN
F 1 "GND" H 3155 2577 50  0000 C CNN
F 2 "" H 3150 2750 50  0001 C CNN
F 3 "" H 3150 2750 50  0001 C CNN
	1    3150 2750
	1    0    0    -1  
$EndComp
$Comp
L Luciebox_components:VCC_minimalCurrent #PWR06
U 1 1 5FFE60B8
P 3050 3650
F 0 "#PWR06" H 3050 3500 50  0001 C CNN
F 1 "VCC_minimalCurrent" H 3067 3823 50  0000 C CNN
F 2 "" H 3050 3650 50  0001 C CNN
F 3 "" H 3050 3650 50  0001 C CNN
	1    3050 3650
	-1   0    0    1   
$EndComp
Text Label 1250 6000 2    50   ~ 0
2
Text Label 1250 6100 2    50   ~ 0
3(PWM)
Text Label 3350 1600 2    50   ~ 0
1(Tx)
Text Label 3350 1100 2    50   ~ 0
A3
Wire Wire Line
	2650 4000 2000 4000
Wire Wire Line
	2000 4000 2000 4550
Wire Wire Line
	1100 6200 1250 6200
Wire Wire Line
	1100 6200 1100 6000
Wire Wire Line
	1100 6000 700  6000
Wire Wire Line
	700  6000 700  5700
Connection ~ 1100 6200
Wire Wire Line
	1200 5700 1200 6000
Wire Wire Line
	1200 6000 1100 6000
Connection ~ 1100 6000
Wire Wire Line
	1250 6000 1250 5850
Wire Wire Line
	1250 5850 1400 5850
Wire Wire Line
	1400 5850 1400 5700
Wire Wire Line
	1250 5850 900  5850
Wire Wire Line
	900  5850 900  5700
Connection ~ 1250 5850
Wire Wire Line
	1250 6100 800  6100
Wire Wire Line
	800  6100 800  5750
Wire Wire Line
	1300 5700 1300 5750
Wire Wire Line
	1300 5750 800  5750
Connection ~ 800  5750
Wire Wire Line
	800  5750 800  5700
Wire Wire Line
	1300 3800 1300 4100
Wire Wire Line
	1350 3950 1500 3950
Wire Wire Line
	1500 3950 1500 3800
Wire Wire Line
	1350 3950 1000 3950
Wire Wire Line
	1000 3950 1000 3800
Connection ~ 1350 3950
Wire Wire Line
	900  4200 900  3850
Wire Wire Line
	1400 3800 1400 3850
Wire Wire Line
	1400 3850 900  3850
Connection ~ 900  3850
Wire Wire Line
	900  3850 900  3800
Wire Wire Line
	800  3800 800  4100
Connection ~ 800  4900
Wire Wire Line
	800  4900 700  4900
Wire Wire Line
	2000 4550 1350 4550
Wire Wire Line
	1350 3950 1350 4550
Wire Wire Line
	1300 4100 800  4100
Connection ~ 800  4100
Wire Wire Line
	800  4100 800  4900
Wire Wire Line
	900  4200 1750 4200
Wire Wire Line
	1750 4200 1750 3850
Wire Wire Line
	1750 3850 2450 3850
Wire Wire Line
	2450 3850 2450 3950
Wire Wire Line
	2450 3950 3100 3950
Wire Wire Line
	3100 3950 3100 4200
Wire Wire Line
	3100 4200 3800 4200
Wire Wire Line
	3800 4200 3800 4400
Wire Wire Line
	3800 4400 4600 4400
Wire Wire Line
	4600 4400 4600 4900
Connection ~ 4600 4900
Wire Wire Line
	4600 4900 4800 4900
$Comp
L Luciebox_components:terminal_3x1 T4
U 1 1 5FF96513
P 1500 3750
F 0 "T4" H 1592 3535 50  0000 C CNN
F 1 "terminal_3x1" H 1592 3626 50  0000 C CNN
F 2 "Luciebox:Terminal_3x1_100mil" H 1300 3650 50  0001 C CNN
F 3 "" H 1450 4150 50  0001 C CNN
	1    1500 3750
	0    -1   -1   0   
$EndComp
$Comp
L Luciebox_components:terminal_3x1 T2
U 1 1 5FF96509
P 1000 3750
F 0 "T2" H 1092 3535 50  0000 C CNN
F 1 "terminal_3x1" H 1092 3626 50  0000 C CNN
F 2 "Luciebox:Terminal_3x1_100mil" H 800 3650 50  0001 C CNN
F 3 "" H 950 4150 50  0001 C CNN
	1    1000 3750
	0    -1   -1   0   
$EndComp
$Comp
L Luciebox_components:terminal_3x1 T3
U 1 1 5FEAEE3B
P 1400 5650
F 0 "T3" H 1492 5435 50  0000 C CNN
F 1 "terminal_3x1" H 1492 5526 50  0000 C CNN
F 2 "Luciebox:Terminal_3x1_100mil" H 1200 5550 50  0001 C CNN
F 3 "" H 1350 6050 50  0001 C CNN
	1    1400 5650
	0    -1   -1   0   
$EndComp
$Comp
L Luciebox_components:terminal_3x1 T1
U 1 1 5FEADAEE
P 900 5650
F 0 "T1" H 992 5435 50  0000 C CNN
F 1 "terminal_3x1" H 992 5526 50  0000 C CNN
F 2 "Luciebox:Terminal_3x1_100mil" H 700 5550 50  0001 C CNN
F 3 "" H 850 6050 50  0001 C CNN
	1    900  5650
	0    -1   -1   0   
$EndComp
$Comp
L Luciebox_components:rotary_encoder S1
U 1 1 5FDADA52
P 1300 5850
F 0 "S1" H 1828 5646 50  0000 L CNN
F 1 "rotary_encoder" H 1828 5555 50  0000 L CNN
F 2 "Luciebox:rotary_encoder" H 1300 5850 50  0001 C CNN
F 3 "https://www.ttelectronics.com/TTElectronics/media/ProductFiles/Encoders/Datasheets/EN16.pdf" H 1300 5850 50  0001 C CNN
	1    1300 5850
	1    0    0    -1  
$EndComp
$Comp
L Luciebox_components:selector_knob S2
U 1 1 5FD1F666
P 2000 4350
F 0 "S2" V 2121 3092 50  0000 R CNN
F 1 "selector_knob" V 2030 3092 50  0000 R CNN
F 2 "Luciebox:Rotary_Switch" H 2000 4350 50  0001 C CNN
F 3 "" H 2000 4350 50  0001 C CNN
	1    2000 4350
	0    -1   -1   0   
$EndComp
$Comp
L Luciebox_components:terminal_8x1 T6
U 1 1 5FE9CBBF
P 2400 2750
F 0 "T6" H 2517 2535 50  0000 C CNN
F 1 "terminal_8x1" H 2517 2626 50  0000 C CNN
F 2 "Luciebox:Terminal_8x1_100mil" H 2200 2650 50  0001 C CNN
F 3 "" H 2350 3850 50  0001 C CNN
	1    2400 2750
	-1   0    0    1   
$EndComp
$Comp
L Luciebox_components:terminal_8x1 T5
U 1 1 5FE9B62C
P 2400 1750
F 0 "T5" H 2517 1535 50  0000 C CNN
F 1 "terminal_8x1" H 2517 1626 50  0000 C CNN
F 2 "Luciebox:Terminal_8x1_100mil" H 2200 1650 50  0001 C CNN
F 3 "" H 2350 2850 50  0001 C CNN
	1    2400 1750
	-1   0    0    1   
$EndComp
$Comp
L Luciebox_components:terminal_8x1 T11
U 1 1 5E968FAE
P 4900 4000
F 0 "T11" H 5017 3785 50  0000 C CNN
F 1 "terminal_8x1" H 5017 3876 50  0000 C CNN
F 2 "Luciebox:Terminal_8x1_100mil" H 4700 3900 50  0001 C CNN
F 3 "" H 4850 5100 50  0001 C CNN
	1    4900 4000
	-1   0    0    -1  
$EndComp
$Comp
L Luciebox_components:terminal_8x1 T10
U 1 1 5E835E31
P 4250 4000
F 0 "T10" H 4367 3785 50  0000 C CNN
F 1 "terminal_8x1" H 4367 3876 50  0000 C CNN
F 2 "Luciebox:Terminal_8x1_100mil" H 4050 3900 50  0001 C CNN
F 3 "" H 4200 5100 50  0001 C CNN
	1    4250 4000
	1    0    0    -1  
$EndComp
$Comp
L Luciebox_components:terminal_3x1 T9
U 1 1 5E74AAC5
P 4000 5400
F 0 "T9" H 4092 5185 50  0000 C CNN
F 1 "terminal_3x1" H 4092 5276 50  0000 C CNN
F 2 "Luciebox:Terminal_3x1_100mil" H 3800 5300 50  0001 C CNN
F 3 "" H 3950 5800 50  0001 C CNN
	1    4000 5400
	-1   0    0    1   
$EndComp
$Comp
L Luciebox_components:D_Schottky D1
U 1 1 5E65D2EA
P 4050 6000
F 0 "D1" H 4050 6216 50  0000 C CNN
F 1 "D_Schottky" H 4050 6125 50  0000 C CNN
F 2 "Luciebox:smd_throughhole_2_pins" H 4050 6000 50  0001 C CNN
F 3 "~" H 4050 6000 50  0001 C CNN
	1    4050 6000
	1    0    0    -1  
$EndComp
$Comp
L Luciebox_components:R_Small-Device R_vcc_protected1
U 1 1 5E1772EF
P 1450 2400
F 0 "R_vcc_protected1" H 1509 2446 50  0000 L CNN
F 1 "10K" H 1509 2355 50  0000 L CNN
F 2 "Luciebox:1206_0805" H 1450 2400 50  0001 C CNN
F 3 "~" H 1450 2400 50  0001 C CNN
	1    1450 2400
	0    1    1    0   
$EndComp
$Comp
L Luciebox_components:8X8PADS U2
U 1 1 5DD3C18A
P 5700 4850
F 0 "U2" H 6150 6050 50  0000 C CNN
F 1 "8X8PADS" H 6100 5950 50  0000 C CNN
F 2 "Luciebox:64_PADS_100mil" H 5700 4850 50  0001 C CNN
F 3 "" H 5700 4850 50  0001 C CNN
	1    5700 4850
	1    0    0    -1  
$EndComp
$Comp
L Luciebox_components:MountingHole H5
U 1 1 5DC9D509
P 6350 5350
F 0 "H5" H 6450 5396 50  0000 L CNN
F 1 "MountingHole" H 6450 5305 50  0000 L CNN
F 2 "Luciebox:MountingHole_4.3mm_M4" H 6350 5350 50  0001 C CNN
F 3 "~" H 6350 5350 50  0001 C CNN
	1    6350 5350
	1    0    0    -1  
$EndComp
$Comp
L Luciebox_components:MountingHole H6
U 1 1 5DC9D485
P 6350 5550
F 0 "H6" H 6450 5596 50  0000 L CNN
F 1 "MountingHole" H 6450 5505 50  0000 L CNN
F 2 "Luciebox:MountingHole_4.3mm_M4" H 6350 5550 50  0001 C CNN
F 3 "~" H 6350 5550 50  0001 C CNN
	1    6350 5550
	1    0    0    -1  
$EndComp
$Comp
L Luciebox_components:MountingHole H7
U 1 1 5DC9D403
P 6350 5750
F 0 "H7" H 6450 5796 50  0000 L CNN
F 1 "MountingHole" H 6450 5705 50  0000 L CNN
F 2 "Luciebox:MountingHole_4.3mm_M4" H 6350 5750 50  0001 C CNN
F 3 "~" H 6350 5750 50  0001 C CNN
	1    6350 5750
	1    0    0    -1  
$EndComp
$Comp
L Luciebox_components:MountingHole H8
U 1 1 5DC9D36B
P 6400 5950
F 0 "H8" H 6500 5996 50  0000 L CNN
F 1 "MountingHole" H 6500 5905 50  0000 L CNN
F 2 "Luciebox:MountingHole_4.3mm_M4" H 6400 5950 50  0001 C CNN
F 3 "~" H 6400 5950 50  0001 C CNN
	1    6400 5950
	1    0    0    -1  
$EndComp
$Comp
L Luciebox_components:MountingHole H4
U 1 1 5DC9D2EF
P 5800 5950
F 0 "H4" H 5900 5996 50  0000 L CNN
F 1 "MountingHole" H 5900 5905 50  0000 L CNN
F 2 "Luciebox:MountingHole_4.3mm_M4" H 5800 5950 50  0001 C CNN
F 3 "~" H 5800 5950 50  0001 C CNN
	1    5800 5950
	1    0    0    -1  
$EndComp
$Comp
L Luciebox_components:MountingHole H3
U 1 1 5DC9D279
P 5750 5750
F 0 "H3" H 5850 5796 50  0000 L CNN
F 1 "MountingHole" H 5850 5705 50  0000 L CNN
F 2 "Luciebox:MountingHole_4.3mm_M4" H 5750 5750 50  0001 C CNN
F 3 "~" H 5750 5750 50  0001 C CNN
	1    5750 5750
	1    0    0    -1  
$EndComp
$Comp
L Luciebox_components:MountingHole H2
U 1 1 5DC9D1FF
P 5750 5600
F 0 "H2" H 5850 5646 50  0000 L CNN
F 1 "MountingHole" H 5850 5555 50  0000 L CNN
F 2 "Luciebox:MountingHole_4.3mm_M4" H 5750 5600 50  0001 C CNN
F 3 "~" H 5750 5600 50  0001 C CNN
	1    5750 5600
	1    0    0    -1  
$EndComp
$Comp
L Luciebox_components:R_Small-Device R10
U 1 1 5D6C3ACD
P 3100 7000
F 0 "R10" H 3159 7046 50  0000 L CNN
F 1 "100K" H 3159 6955 50  0000 L CNN
F 2 "Luciebox:1206_0805" H 3100 7000 50  0001 C CNN
F 3 "~" H 3100 7000 50  0001 C CNN
	1    3100 7000
	1    0    0    -1  
$EndComp
$Comp
L Luciebox_components:R_Small-Device R9
U 1 1 5D6C3437
P 3100 6600
F 0 "R9" H 3159 6646 50  0000 L CNN
F 1 "10K" H 3159 6555 50  0000 L CNN
F 2 "Luciebox:1206_0805" H 3100 6600 50  0001 C CNN
F 3 "~" H 3100 6600 50  0001 C CNN
	1    3100 6600
	1    0    0    -1  
$EndComp
$Comp
L Luciebox_components:N-Channel_Mosfet Q1
U 1 1 5D6BDEDC
P 3700 6900
F 0 "Q1" H 3906 6991 50  0000 L CNN
F 1 "N-Channel_Mosfet" H 3906 6900 50  0000 L CNN
F 2 "Luciebox:TO-220-3" H 3906 6809 50  0000 L CIN
F 3 "https://www.onsemi.com/pub/Collateral/FQP30N06L-D.pdf" V 3700 6900 50  0001 L CNN
	1    3700 6900
	1    0    0    -1  
$EndComp
$Comp
L Luciebox_components:R_Small-Device R_reset_pullup1
U 1 1 5DA45BE6
P 9500 5250
F 0 "R_reset_pullup1" H 9559 5296 50  0000 L CNN
F 1 "30000" H 9559 5205 50  0000 L CNN
F 2 "Luciebox:1206_0805" H 9500 5250 50  0001 C CNN
F 3 "~" H 9500 5250 50  0001 C CNN
	1    9500 5250
	0    1    1    0   
$EndComp
$Comp
L Luciebox_components:C_Small-Device C1
U 1 1 5DA2A1AC
P 6950 3400
F 0 "C1" V 6721 3400 50  0000 C CNN
F 1 "100nF" V 6812 3400 50  0000 C CNN
F 2 "Luciebox:0805" H 6950 3400 50  0001 C CNN
F 3 "~" H 6950 3400 50  0001 C CNN
	1    6950 3400
	0    1    1    0   
$EndComp
$Comp
L Luciebox_components:C_Small-Device C3
U 1 1 5D9EAF46
P 10250 4700
F 0 "C3" V 10021 4700 50  0000 C CNN
F 1 "20pF" V 10112 4700 50  0000 C CNN
F 2 "Luciebox:0805" H 10250 4700 50  0001 C CNN
F 3 "~" H 10250 4700 50  0001 C CNN
	1    10250 4700
	0    1    1    0   
$EndComp
$Comp
L Luciebox_components:C_Small-Device C2
U 1 1 5D9EA27E
P 10250 4350
F 0 "C2" V 10021 4350 50  0000 C CNN
F 1 "20pF" V 10112 4350 50  0000 C CNN
F 2 "Luciebox:0805" H 10250 4350 50  0001 C CNN
F 3 "~" H 10250 4350 50  0001 C CNN
	1    10250 4350
	0    1    1    0   
$EndComp
$Comp
L Luciebox_components:R_Small-Device R20
U 1 1 5D9BA081
P 9300 4450
F 0 "R20" H 9359 4496 50  0000 L CNN
F 1 "1M" H 9359 4405 50  0000 L CNN
F 2 "Luciebox:1206_0805" H 9300 4450 50  0001 C CNN
F 3 "~" H 9300 4450 50  0001 C CNN
	1    9300 4450
	1    0    0    -1  
$EndComp
$Comp
L Luciebox_components:Crystal-Device Y1
U 1 1 5D9962E4
P 9750 4500
F 0 "Y1" V 9659 4631 50  0000 L CNN
F 1 "16MHz" V 9750 4631 50  0000 L CNN
F 2 "Luciebox:CRYSTAL" V 9841 4631 50  0000 L CNN
F 3 "~" H 9750 4500 50  0001 C CNN
	1    9750 4500
	0    1    1    0   
$EndComp
$Comp
L Luciebox_components:Buzzer-Device BZ1
U 1 1 5D93C6F4
P 1400 6750
F 0 "BZ1" V 1451 6563 50  0000 R CNN
F 1 "Buzzer" V 1360 6563 50  0000 R CNN
F 2 "Luciebox:buzzer" V 1375 6850 50  0001 C CNN
F 3 "~" V 1375 6850 50  0001 C CNN
	1    1400 6750
	0    -1   -1   0   
$EndComp
$Comp
L Luciebox_components:R_Small-Device R14
U 1 1 5D7D8CC8
P 4250 4900
F 0 "R14" H 4309 4946 50  0000 L CNN
F 1 "10K" H 4309 4855 50  0000 L CNN
F 2 "Luciebox:1206_0805" H 4250 4900 50  0001 C CNN
F 3 "~" H 4250 4900 50  0001 C CNN
	1    4250 4900
	0    1    1    0   
$EndComp
$Comp
L Luciebox_components:R_Small-Device R13
U 1 1 5D7D8CBE
P 3950 4900
F 0 "R13" H 4009 4946 50  0000 L CNN
F 1 "10K" H 4009 4855 50  0000 L CNN
F 2 "Luciebox:1206_0805" H 3950 4900 50  0001 C CNN
F 3 "~" H 3950 4900 50  0001 C CNN
	1    3950 4900
	0    1    1    0   
$EndComp
$Comp
L Luciebox_components:R_Small-Device R12
U 1 1 5D7D8CB3
P 3650 4900
F 0 "R12" H 3709 4946 50  0000 L CNN
F 1 "10K" H 3709 4855 50  0000 L CNN
F 2 "Luciebox:1206_0805" H 3650 4900 50  0001 C CNN
F 3 "~" H 3650 4900 50  0001 C CNN
	1    3650 4900
	0    1    1    0   
$EndComp
$Comp
L Luciebox_components:R_Small-Device R11
U 1 1 5D7D8CA9
P 3350 4900
F 0 "R11" H 3409 4946 50  0000 L CNN
F 1 "10K" H 3409 4855 50  0000 L CNN
F 2 "Luciebox:1206_0805" H 3350 4900 50  0001 C CNN
F 3 "~" H 3350 4900 50  0001 C CNN
	1    3350 4900
	0    1    1    0   
$EndComp
$Comp
L Luciebox_components:R_Small-Device R8
U 1 1 5D7CF8D8
P 3050 4900
F 0 "R8" H 3109 4946 50  0000 L CNN
F 1 "10K" H 3109 4855 50  0000 L CNN
F 2 "Luciebox:1206_0805" H 3050 4900 50  0001 C CNN
F 3 "~" H 3050 4900 50  0001 C CNN
	1    3050 4900
	0    1    1    0   
$EndComp
$Comp
L Luciebox_components:R_Small-Device R7
U 1 1 5D7CF8CE
P 2750 4900
F 0 "R7" H 2809 4946 50  0000 L CNN
F 1 "10K" H 2809 4855 50  0000 L CNN
F 2 "Luciebox:1206_0805" H 2750 4900 50  0001 C CNN
F 3 "~" H 2750 4900 50  0001 C CNN
	1    2750 4900
	0    1    1    0   
$EndComp
$Comp
L Luciebox_components:R_Small-Device R6
U 1 1 5D7CF8C3
P 2450 4900
F 0 "R6" H 2509 4946 50  0000 L CNN
F 1 "10K" H 2509 4855 50  0000 L CNN
F 2 "Luciebox:1206_0805" H 2450 4900 50  0001 C CNN
F 3 "~" H 2450 4900 50  0001 C CNN
	1    2450 4900
	0    1    1    0   
$EndComp
$Comp
L Luciebox_components:R_Small-Device R5
U 1 1 5D7CF8B9
P 2150 4900
F 0 "R5" H 2209 4946 50  0000 L CNN
F 1 "10K" H 2209 4855 50  0000 L CNN
F 2 "Luciebox:1206_0805" H 2150 4900 50  0001 C CNN
F 3 "~" H 2150 4900 50  0001 C CNN
	1    2150 4900
	0    1    1    0   
$EndComp
$Comp
L Luciebox_components:R_Small-Device R4
U 1 1 5D7B59FE
P 1850 4900
F 0 "R4" H 1909 4946 50  0000 L CNN
F 1 "10K" H 1909 4855 50  0000 L CNN
F 2 "Luciebox:1206_0805" H 1850 4900 50  0001 C CNN
F 3 "~" H 1850 4900 50  0001 C CNN
	1    1850 4900
	0    1    1    0   
$EndComp
$Comp
L Luciebox_components:R_Small-Device R3
U 1 1 5D7B59F4
P 1550 4900
F 0 "R3" H 1609 4946 50  0000 L CNN
F 1 "10K" H 1609 4855 50  0000 L CNN
F 2 "Luciebox:1206_0805" H 1550 4900 50  0001 C CNN
F 3 "~" H 1550 4900 50  0001 C CNN
	1    1550 4900
	0    1    1    0   
$EndComp
$Comp
L Luciebox_components:R_Small-Device R2
U 1 1 5D79CF90
P 1250 4900
F 0 "R2" H 1309 4946 50  0000 L CNN
F 1 "10K" H 1309 4855 50  0000 L CNN
F 2 "Luciebox:1206_0805" H 1250 4900 50  0001 C CNN
F 3 "~" H 1250 4900 50  0001 C CNN
	1    1250 4900
	0    1    1    0   
$EndComp
$Comp
L Luciebox_components:R_Small-Device R1
U 1 1 5D798401
P 950 4900
F 0 "R1" H 1009 4946 50  0000 L CNN
F 1 "10K" H 1009 4855 50  0000 L CNN
F 2 "Luciebox:1206_0805" H 950 4900 50  0001 C CNN
F 3 "~" H 950 4900 50  0001 C CNN
	1    950  4900
	0    1    1    0   
$EndComp
$Comp
L Luciebox_components:ATmega328-PU U3
U 1 1 5D492254
P 7900 4950
F 0 "U3" H 7256 4996 50  0000 R CNN
F 1 "ATmega328-PU" H 7256 4905 50  0000 R CNN
F 2 "Luciebox:DIP-28_W7.62mm" H 7900 4950 50  0001 C CIN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/ATmega328_P%20AVR%20MCU%20with%20picoPower%20Technology%20Data%20Sheet%2040001984A.pdf" H 7900 4950 50  0001 C CNN
	1    7900 4950
	1    0    0    -1  
$EndComp
$Comp
L Luciebox_components:R_Small-Device R15
U 1 1 5D4E6F2F
P 6500 2400
F 0 "R15" H 6559 2446 50  0000 L CNN
F 1 "100" H 6559 2355 50  0000 L CNN
F 2 "Luciebox:1206_0805" H 6500 2400 50  0001 C CNN
F 3 "~" H 6500 2400 50  0001 C CNN
	1    6500 2400
	1    0    0    -1  
$EndComp
$Comp
L Luciebox_components:R_Small-Device R16
U 1 1 5D496C09
P 6750 2400
F 0 "R16" H 6809 2446 50  0000 L CNN
F 1 "100" H 6809 2355 50  0000 L CNN
F 2 "Luciebox:1206_0805" H 6750 2400 50  0001 C CNN
F 3 "~" H 6750 2400 50  0001 C CNN
	1    6750 2400
	1    0    0    -1  
$EndComp
$Comp
L Luciebox_components:R_Small-Device R19
U 1 1 5D49796F
P 7500 2400
F 0 "R19" H 7559 2446 50  0000 L CNN
F 1 "100" H 7559 2355 50  0000 L CNN
F 2 "Luciebox:1206_0805" H 7500 2400 50  0001 C CNN
F 3 "~" H 7500 2400 50  0001 C CNN
	1    7500 2400
	1    0    0    -1  
$EndComp
$Comp
L Luciebox_components:R_Small-Device R18
U 1 1 5D497707
P 7250 2400
F 0 "R18" H 7309 2446 50  0000 L CNN
F 1 "100" H 7309 2355 50  0000 L CNN
F 2 "Luciebox:1206_0805" H 7250 2400 50  0001 C CNN
F 3 "~" H 7250 2400 50  0001 C CNN
	1    7250 2400
	1    0    0    -1  
$EndComp
$Comp
L Luciebox_components:R_Small-Device R17
U 1 1 5D49734E
P 7000 2400
F 0 "R17" H 7059 2446 50  0000 L CNN
F 1 "100" H 7059 2355 50  0000 L CNN
F 2 "Luciebox:1206_0805" H 7000 2400 50  0001 C CNN
F 3 "~" H 7000 2400 50  0001 C CNN
	1    7000 2400
	1    0    0    -1  
$EndComp
$Comp
L Luciebox_components:4digit_7segments_common_anode_CA56-12SRWA U1
U 1 1 5D48F293
P 4950 1300
F 0 "U1" H 4950 1967 50  0000 C CNN
F 1 "CA56-12SRWA" H 4950 1876 50  0000 C CNN
F 2 "Luciebox:CA56-12SRWA" H 4950 700 50  0001 C CNN
F 3 "http://www.kingbrightusa.com/images/catalog/SPEC/CA56-12SRWA.pdf" H 4520 1330 50  0001 C CNN
	1    4950 1300
	1    0    0    -1  
$EndComp
$Comp
L Luciebox_components:Conn_01x10-Connector_Generic P3
U 1 1 56D721E0
P 10000 1600
F 0 "P3" H 10000 2150 50  0000 C CNN
F 1 "Digital" V 10100 1600 50  0000 C CNN
F 2 "Socket_Arduino_Uno:Socket_Strip_Arduino_1x10" V 10150 1600 20  0000 C CNN
F 3 "" H 10000 1600 50  0000 C CNN
	1    10000 1600
	-1   0    0    -1  
$EndComp
$Comp
L Luciebox_components:Conn_01x08-Connector_Generic P4
U 1 1 56D7164F
P 10000 2600
F 0 "P4" H 10000 2100 50  0000 C CNN
F 1 "Digital" V 10100 2600 50  0000 C CNN
F 2 "Socket_Arduino_Uno:Socket_Strip_Arduino_1x08" V 10150 2550 20  0000 C CNN
F 3 "" H 10000 2600 50  0000 C CNN
	1    10000 2600
	-1   0    0    -1  
$EndComp
$Comp
L Luciebox_components:Conn_01x01-Connector_Generic P8
U 1 1 56D712DB
P 11100 650
F 0 "P8" V 11200 650 50  0000 C CNN
F 1 "CONN_01X01" V 11200 650 50  0001 C CNN
F 2 "Socket_Arduino_Uno:Arduino_1pin" H 11024 572 20  0000 C CNN
F 3 "" H 11100 650 50  0000 C CNN
	1    11100 650 
	0    -1   -1   0   
$EndComp
$Comp
L Luciebox_components:Conn_01x01-Connector_Generic P7
U 1 1 56D712A8
P 11000 650
F 0 "P7" V 11100 650 50  0000 C CNN
F 1 "CONN_01X01" V 11100 650 50  0001 C CNN
F 2 "Socket_Arduino_Uno:Arduino_1pin" V 11000 650 20  0001 C CNN
F 3 "" H 11000 650 50  0000 C CNN
	1    11000 650 
	0    -1   -1   0   
$EndComp
$Comp
L Luciebox_components:Conn_01x01-Connector_Generic P6
U 1 1 56D71274
P 10900 650
F 0 "P6" V 11000 650 50  0000 C CNN
F 1 "CONN_01X01" V 11000 650 50  0001 C CNN
F 2 "Socket_Arduino_Uno:Arduino_1pin" H 10900 650 20  0001 C CNN
F 3 "" H 10900 650 50  0000 C CNN
	1    10900 650 
	0    -1   -1   0   
$EndComp
$Comp
L Luciebox_components:Conn_01x01-Connector_Generic P5
U 1 1 56D71177
P 10800 650
F 0 "P5" V 10900 650 50  0000 C CNN
F 1 "CONN_01X01" V 10900 650 50  0001 C CNN
F 2 "Socket_Arduino_Uno:Arduino_1pin" H 10721 724 20  0000 C CNN
F 3 "" H 10800 650 50  0000 C CNN
	1    10800 650 
	0    -1   -1   0   
$EndComp
$Comp
L Luciebox_components:Conn_01x06-Connector_Generic P2
U 1 1 56D70DD8
P 9600 2700
F 0 "P2" H 9600 2300 50  0000 C CNN
F 1 "Analog" V 9700 2700 50  0000 C CNN
F 2 "Socket_Arduino_Uno:Socket_Strip_Arduino_1x06" V 9750 2750 20  0000 C CNN
F 3 "" H 9600 2700 50  0000 C CNN
	1    9600 2700
	1    0    0    -1  
$EndComp
$Comp
L Luciebox_components:Conn_01x08-Connector_Generic P1
U 1 1 56D70129
P 9600 1900
F 0 "P1" H 9600 2350 50  0000 C CNN
F 1 "Power" V 9700 1900 50  0000 C CNN
F 2 "Socket_Arduino_Uno:Socket_Strip_Arduino_1x08" V 9750 1900 20  0000 C CNN
F 3 "" H 9600 1900 50  0000 C CNN
	1    9600 1900
	1    0    0    -1  
$EndComp
$Comp
L Luciebox_components:MountingHole H1
U 1 1 5DC89D39
P 5750 5300
F 0 "H1" H 5850 5346 50  0000 L CNN
F 1 "MountingHole" H 5850 5255 50  0000 L CNN
F 2 "Luciebox:MountingHole_4.3mm_M4" H 5750 5300 50  0001 C CNN
F 3 "~" H 5750 5300 50  0001 C CNN
	1    5750 5300
	1    0    0    -1  
$EndComp
Wire Wire Line
	3450 1700 3450 2150
Wire Wire Line
	3550 1500 3550 3450
Wire Wire Line
	3650 1300 3650 2250
Wire Wire Line
	3800 1000 3800 2450
Wire Wire Line
	5000 2050 5000 2300
Wire Wire Line
	2450 2150 3450 2150
Wire Wire Line
	2450 1750 3150 1750
Wire Wire Line
	2450 1850 3050 1850
Wire Wire Line
	3050 1850 3050 2850
Wire Wire Line
	3650 2250 2450 2250
Wire Wire Line
	2450 2350 3600 2350
Wire Wire Line
	3600 1400 3600 2350
Wire Wire Line
	2450 2450 3800 2450
Wire Wire Line
	2450 2750 3150 2750
Wire Wire Line
	2450 2850 3050 2850
Connection ~ 3050 2850
Wire Wire Line
	2450 3050 5000 3050
Connection ~ 5000 3050
Wire Wire Line
	5000 3050 5000 3300
Wire Wire Line
	2450 2050 5000 2050
Wire Wire Line
	3500 1600 3500 3250
Wire Wire Line
	2450 3150 3750 3150
Wire Wire Line
	3750 1100 3750 3150
Wire Wire Line
	3050 2850 3050 3650
Wire Wire Line
	2450 3250 3500 3250
Wire Wire Line
	2450 3350 3700 3350
Wire Wire Line
	2450 3450 3550 3450
Text Label 4200 4000 2    50   ~ 0
A3
Text Label 4200 3900 2    50   ~ 0
1(Tx)
Text Label 4200 3800 2    50   ~ 0
4
Text Label 4200 3600 2    50   ~ 0
0(Rx)
Text Label 4200 3500 2    50   ~ 0
8
Text Label 4200 3400 2    50   ~ 0
12(MISO)
Text Label 4200 3300 2    50   ~ 0
7
Wire Wire Line
	3700 1200 3700 3350
Text Label 4200 3700 2    50   ~ 0
13(SCK)
$Comp
L Luciebox_components:terminal_3x1 T8
U 1 1 606256A8
P 2600 1200
F 0 "T8" H 2692 985 50  0000 C CNN
F 1 "terminal_3x1" H 2692 1076 50  0000 C CNN
F 2 "Luciebox:Terminal_3x1_100mil" H 2400 1100 50  0001 C CNN
F 3 "" H 2550 1600 50  0001 C CNN
	1    2600 1200
	-1   0    0    1   
$EndComp
$Comp
L Luciebox_components:terminal_3x1 T7
U 1 1 606246AE
P 2600 750
F 0 "T7" H 2692 535 50  0000 C CNN
F 1 "terminal_3x1" H 2692 626 50  0000 C CNN
F 2 "Luciebox:Terminal_3x1_100mil" H 2400 650 50  0001 C CNN
F 3 "" H 2550 1150 50  0001 C CNN
	1    2600 750 
	-1   0    0    1   
$EndComp
Wire Wire Line
	7500 600  2850 600 
Wire Wire Line
	2850 600  2850 750 
Wire Wire Line
	2850 750  2650 750 
Wire Wire Line
	7500 600  7500 1400
Connection ~ 7500 1400
Wire Wire Line
	3750 850  2650 850 
Wire Wire Line
	3550 1500 3550 950 
Wire Wire Line
	3550 950  2650 950 
Wire Wire Line
	7250 1500 7250 650 
Wire Wire Line
	7250 650  2900 650 
Wire Wire Line
	2900 650  2900 1200
Wire Wire Line
	2900 1200 2650 1200
Connection ~ 7250 1500
Wire Wire Line
	6050 1600 7000 1600
Wire Wire Line
	7000 1600 7000 700 
Wire Wire Line
	7000 700  2950 700 
Wire Wire Line
	2950 700  2950 1300
Wire Wire Line
	2950 1300 2650 1300
Connection ~ 7000 1600
Wire Wire Line
	3750 850  3750 1000
Connection ~ 3750 1000
Wire Wire Line
	3750 1000 3800 1000
Wire Wire Line
	3650 1100 3650 750 
Wire Wire Line
	3650 750  3000 750 
Wire Wire Line
	3000 750  3000 1400
Wire Wire Line
	3000 1400 2650 1400
Connection ~ 3650 1100
Wire Wire Line
	3650 1100 3750 1100
$Comp
L Luciebox_components:USB_B_Mini J1
U 1 1 608A892E
P 4700 5800
F 0 "J1" H 4470 5697 50  0000 R CNN
F 1 "USB_B_Mini" H 4470 5788 50  0000 R CNN
F 2 "Luciebox:usb_mini_Molex-0548190519" H 4850 5750 50  0001 C CNN
F 3 "~" H 4850 5750 50  0001 C CNN
	1    4700 5800
	-1   0    0    1   
$EndComp
$Comp
L Luciebox_components:terminal_8x1 T12
U 1 1 5FD5E541
P 5750 7650
F 0 "T12" H 5850 8550 50  0000 L CNN
F 1 "terminal_8x1" H 5750 8500 50  0000 L CNN
F 2 "Luciebox:Terminal_8x1_100mil" H 5550 7550 50  0001 C CNN
F 3 "" H 5700 8750 50  0001 C CNN
	1    5750 7650
	1    0    0    -1  
$EndComp
Text Notes 6050 7550 0    50   ~ 0
SWITCH_USB_POWER
Text Notes 6050 7650 0    50   ~ 0
SWITCH_USB_POWER
Wire Wire Line
	4900 7050 4900 7550
Wire Wire Line
	5000 7150 5100 7150
Wire Wire Line
	5700 7350 5350 7350
Wire Wire Line
	5350 7350 5350 6950
Connection ~ 5350 6950
Wire Wire Line
	5350 6950 5050 6950
Wire Wire Line
	5350 5350 5350 6950
Wire Wire Line
	4800 5350 5350 5350
$Comp
L Luciebox_components:R_Small-Device R_0_if_no_touch1
U 1 1 5FE57FC5
P 5150 6500
F 0 "R_0_if_no_touch1" V 5050 5850 50  0000 L CNN
F 1 "0" V 5209 6455 50  0000 L CNN
F 2 "Luciebox:1206_0805" H 5150 6500 50  0001 C CNN
F 3 "~" H 5150 6500 50  0001 C CNN
	1    5150 6500
	0    1    1    0   
$EndComp
Wire Wire Line
	5050 6500 5050 6950
Connection ~ 5050 6950
Wire Wire Line
	5050 6950 4900 6950
Wire Wire Line
	5250 6500 5250 7050
Connection ~ 5250 7050
Wire Wire Line
	5250 7050 4900 7050
Wire Wire Line
	5150 7250 5150 7450
Wire Wire Line
	5150 7450 5700 7450
Wire Wire Line
	3700 6150 5450 6150
Wire Wire Line
	5450 6150 5450 7550
Wire Wire Line
	5450 7550 5700 7550
Wire Wire Line
	3700 6000 3700 6150
Wire Wire Line
	5700 7650 5100 7650
Wire Wire Line
	5100 7650 5100 7550
Connection ~ 5100 7150
Wire Wire Line
	5100 7150 5700 7150
$Comp
L Luciebox_components:R_Small-Device R_0_if_no_usb_switch1
U 1 1 5FF3108A
P 5250 7550
F 0 "R_0_if_no_usb_switch1" V 5400 7200 50  0000 L CNN
F 1 "0" V 5309 7505 50  0000 L CNN
F 2 "Luciebox:1206_0805" H 5250 7550 50  0001 C CNN
F 3 "~" H 5250 7550 50  0001 C CNN
	1    5250 7550
	0    1    1    0   
$EndComp
Wire Wire Line
	5350 7550 5450 7550
Connection ~ 5450 7550
Wire Wire Line
	5150 7550 5100 7550
Connection ~ 5100 7550
Wire Wire Line
	5100 7550 5100 7150
$Comp
L Luciebox_components:terminal_8x1 T13
U 1 1 5FF871B6
P 10750 5950
F 0 "T13" H 10850 6850 50  0000 L CNN
F 1 "terminal_8x1" H 10750 6800 50  0000 L CNN
F 2 "Luciebox:Terminal_8x1_100mil" H 10550 5850 50  0001 C CNN
F 3 "" H 10700 7050 50  0001 C CNN
	1    10750 5950
	1    0    0    -1  
$EndComp
$Comp
L Connector:TestPoint_Probe TP1
U 1 1 5FFC5565
P 4250 2450
F 0 "TP1" H 4403 2551 50  0000 L CNN
F 1 "TestPoint_Probe" H 4403 2460 50  0000 L CNN
F 2 "" H 4450 2450 50  0001 C CNN
F 3 "~" H 4450 2450 50  0001 C CNN
	1    4250 2450
	1    0    0    -1  
$EndComp
$EndSCHEMATC
