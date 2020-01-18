EESchema Schematic File Version 4
LIBS:Luciebox-cache
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 4
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
$Comp
L Connector_Generic:Conn_01x08 P6
U 1 1 56D70129
P 9600 1900
F 0 "P6" H 9600 2350 50  0000 C CNN
F 1 "Power" V 9700 1900 50  0000 C CNN
F 2 "Socket_Arduino_Uno:Socket_Strip_Arduino_1x08" V 9750 1900 20  0000 C CNN
F 3 "" H 9600 1900 50  0000 C CNN
	1    9600 1900
	1    0    0    -1  
$EndComp
Text Label 8650 1800 0    60   ~ 0
Reset
$Comp
L power:+3.3V #PWR02
U 1 1 56D70538
P 9150 1450
F 0 "#PWR02" H 9150 1300 50  0001 C CNN
F 1 "+3.3V" V 9150 1700 50  0000 C CNN
F 2 "" H 9150 1450 50  0000 C CNN
F 3 "" H 9150 1450 50  0000 C CNN
	1    9150 1450
	1    0    0    -1  
$EndComp
$Comp
L power:VCC #PWR01
U 1 1 56D707BB
P 9050 1350
F 0 "#PWR01" H 9050 1200 50  0001 C CNN
F 1 "VCC" V 9050 1550 50  0000 C CNN
F 2 "" H 9050 1350 50  0000 C CNN
F 3 "" H 9050 1350 50  0000 C CNN
	1    9050 1350
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR05
U 1 1 56D70CC2
P 9300 3150
F 0 "#PWR05" H 9300 2900 50  0001 C CNN
F 1 "GND" H 9300 3000 50  0000 C CNN
F 2 "" H 9300 3150 50  0000 C CNN
F 3 "" H 9300 3150 50  0000 C CNN
	1    9300 3150
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR06
U 1 1 56D70CFF
P 10300 3150
F 0 "#PWR06" H 10300 2900 50  0001 C CNN
F 1 "GND" H 10300 3000 50  0000 C CNN
F 2 "" H 10300 3150 50  0000 C CNN
F 3 "" H 10300 3150 50  0000 C CNN
	1    10300 3150
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x06 P8
U 1 1 56D70DD8
P 9600 2700
F 0 "P8" H 9600 2300 50  0000 C CNN
F 1 "Analog" V 9700 2700 50  0000 C CNN
F 2 "Socket_Arduino_Uno:Socket_Strip_Arduino_1x06" V 9750 2750 20  0000 C CNN
F 3 "" H 9600 2700 50  0000 C CNN
	1    9600 2700
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x01 P1
U 1 1 56D71177
P 10800 650
F 0 "P1" V 10900 650 50  0000 C CNN
F 1 "CONN_01X01" V 10900 650 50  0001 C CNN
F 2 "Socket_Arduino_Uno:Arduino_1pin" H 10721 724 20  0000 C CNN
F 3 "" H 10800 650 50  0000 C CNN
	1    10800 650 
	0    -1   -1   0   
$EndComp
$Comp
L Connector_Generic:Conn_01x01 P2
U 1 1 56D71274
P 10900 650
F 0 "P2" V 11000 650 50  0000 C CNN
F 1 "CONN_01X01" V 11000 650 50  0001 C CNN
F 2 "Socket_Arduino_Uno:Arduino_1pin" H 10900 650 20  0001 C CNN
F 3 "" H 10900 650 50  0000 C CNN
	1    10900 650 
	0    -1   -1   0   
$EndComp
$Comp
L Connector_Generic:Conn_01x01 P3
U 1 1 56D712A8
P 11000 650
F 0 "P3" V 11100 650 50  0000 C CNN
F 1 "CONN_01X01" V 11100 650 50  0001 C CNN
F 2 "Socket_Arduino_Uno:Arduino_1pin" V 11000 650 20  0001 C CNN
F 3 "" H 11000 650 50  0000 C CNN
	1    11000 650 
	0    -1   -1   0   
$EndComp
$Comp
L Connector_Generic:Conn_01x01 P4
U 1 1 56D712DB
P 11100 650
F 0 "P4" V 11200 650 50  0000 C CNN
F 1 "CONN_01X01" V 11200 650 50  0001 C CNN
F 2 "Socket_Arduino_Uno:Arduino_1pin" H 11024 572 20  0000 C CNN
F 3 "" H 11100 650 50  0000 C CNN
	1    11100 650 
	0    -1   -1   0   
$EndComp
NoConn ~ 10800 850 
NoConn ~ 10900 850 
NoConn ~ 11000 850 
NoConn ~ 11100 850 
$Comp
L Connector_Generic:Conn_01x08 P7
U 1 1 56D7164F
P 10000 2600
F 0 "P7" H 10000 2100 50  0000 C CNN
F 1 "Digital" V 10100 2600 50  0000 C CNN
F 2 "Socket_Arduino_Uno:Socket_Strip_Arduino_1x08" V 10150 2550 20  0000 C CNN
F 3 "" H 10000 2600 50  0000 C CNN
	1    10000 2600
	-1   0    0    -1  
$EndComp
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
$Comp
L Connector_Generic:Conn_01x10 P5
U 1 1 56D721E0
P 10000 1600
F 0 "P5" H 10000 2150 50  0000 C CNN
F 1 "Digital" V 10100 1600 50  0000 C CNN
F 2 "Socket_Arduino_Uno:Socket_Strip_Arduino_1x10" V 10150 1600 20  0000 C CNN
F 3 "" H 10000 1600 50  0000 C CNN
	1    10000 1600
	-1   0    0    -1  
$EndComp
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
L Device:R_Small R3
U 1 1 5D49734E
P 7000 2400
F 0 "R3" H 7059 2446 50  0000 L CNN
F 1 "100" H 7059 2355 50  0000 L CNN
F 2 "Luciebox:1206" H 7000 2400 50  0001 C CNN
F 3 "~" H 7000 2400 50  0001 C CNN
	1    7000 2400
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small R4
U 1 1 5D497707
P 7250 2400
F 0 "R4" H 7309 2446 50  0000 L CNN
F 1 "100" H 7309 2355 50  0000 L CNN
F 2 "Luciebox:1206" H 7250 2400 50  0001 C CNN
F 3 "~" H 7250 2400 50  0001 C CNN
	1    7250 2400
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small R5
U 1 1 5D49796F
P 7500 2400
F 0 "R5" H 7559 2446 50  0000 L CNN
F 1 "100" H 7559 2355 50  0000 L CNN
F 2 "Luciebox:1206" H 7500 2400 50  0001 C CNN
F 3 "~" H 7500 2400 50  0001 C CNN
	1    7500 2400
	1    0    0    -1  
$EndComp
Wire Wire Line
	6050 1600 7000 1600
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
Text Label 3350 1000 2    50   ~ 0
7
Wire Wire Line
	3850 1000 3800 1000
Wire Wire Line
	3850 1100 3750 1100
Text Label 3350 1100 2    50   ~ 0
2
Text Label 3350 1200 2    50   ~ 0
4
Text Label 3350 1300 2    50   ~ 0
8
Text Label 3350 1400 2    50   ~ 0
12(MISO)
Text Label 3350 1500 2    50   ~ 0
13(SCK)
Text Label 3350 1600 2    50   ~ 0
3(PWM)
Text Label 3350 1700 2    50   ~ 0
0(Rx)
$Comp
L Transistor_Array:ULN2003A U2
U 1 1 5D4C411F
P 4250 2500
F 0 "U2" H 4250 3167 50  0000 C CNN
F 1 "ULN2003A" H 4250 3076 50  0000 C CNN
F 2 "Luciebox:DIP-16_W7.62mm" H 4300 1950 50  0001 L CNN
F 3 "http://www.ti.com/lit/ds/symlink/uln2003a.pdf" H 4350 2300 50  0001 C CNN
	1    4250 2500
	1    0    0    -1  
$EndComp
Wire Wire Line
	3800 2300 3850 2300
Wire Wire Line
	3750 2400 3750 1100
Wire Wire Line
	3350 1000 3800 1000
Connection ~ 3800 1000
Wire Wire Line
	3350 1100 3750 1100
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
	3800 1000 3800 2300
Wire Wire Line
	3350 1600 3500 1600
Wire Wire Line
	3350 1700 3850 1700
Wire Wire Line
	3700 2500 3700 1200
Connection ~ 3700 1200
Wire Wire Line
	3700 1200 3850 1200
Wire Wire Line
	3650 1300 3650 2600
Connection ~ 3650 1300
Wire Wire Line
	3650 1300 3850 1300
Wire Wire Line
	3600 2700 3600 1400
Connection ~ 3600 1400
Wire Wire Line
	3600 1400 3850 1400
Wire Wire Line
	3550 1500 3550 2800
Connection ~ 3550 1500
Wire Wire Line
	3550 1500 3850 1500
Wire Wire Line
	3500 2900 3500 1600
Connection ~ 3500 1600
Wire Wire Line
	3500 1600 3850 1600
Text Label 6750 2500 3    50   ~ 0
5(PWM)
Wire Wire Line
	6750 1700 6750 2300
Wire Wire Line
	6050 1700 6750 1700
$Comp
L Device:R_Small R2
U 1 1 5D496C09
P 6750 2400
F 0 "R2" H 6809 2446 50  0000 L CNN
F 1 "100" H 6809 2355 50  0000 L CNN
F 2 "Luciebox:1206" H 6750 2400 50  0001 C CNN
F 3 "~" H 6750 2400 50  0001 C CNN
	1    6750 2400
	1    0    0    -1  
$EndComp
Text Label 6500 2500 3    50   ~ 0
6(PWM)
$Comp
L Device:R_Small R1
U 1 1 5D4E6F2F
P 6500 2400
F 0 "R1" H 6559 2446 50  0000 L CNN
F 1 "100" H 6559 2355 50  0000 L CNN
F 2 "Luciebox:1206" H 6500 2400 50  0001 C CNN
F 3 "~" H 6500 2400 50  0001 C CNN
	1    6500 2400
	1    0    0    -1  
$EndComp
$Comp
L Transistor_BJT:BC558 Q1
U 1 1 5D50231A
P 6100 2300
F 0 "Q1" H 6291 2254 50  0000 L CNN
F 1 "BC558" H 6291 2345 50  0000 L CNN
F 2 "Luciebox:TO-92-3" H 6300 2225 50  0001 L CIN
F 3 "http://www.fairchildsemi.com/ds/BC/BC557.pdf" H 6100 2300 50  0001 L CNN
	1    6100 2300
	-1   0    0    1   
$EndComp
Wire Wire Line
	6300 2300 6500 2300
$Comp
L Connector:Screw_Terminal_01x07 J2
U 1 1 5D506721
P 4950 2600
F 0 "J2" H 5030 2642 50  0000 L CNN
F 1 "Screw_Terminal_01x07" H 5030 2551 50  0000 L CNN
F 2 "Luciebox:Terminal_7x1_100mil" H 4950 2600 50  0001 C CNN
F 3 "~" H 4950 2600 50  0001 C CNN
	1    4950 2600
	1    0    0    -1  
$EndComp
Wire Wire Line
	4650 2300 4750 2300
Wire Wire Line
	4750 2400 4650 2400
Wire Wire Line
	4650 2500 4750 2500
Wire Wire Line
	4750 2600 4650 2600
Wire Wire Line
	4650 2700 4750 2700
Wire Wire Line
	4750 2800 4650 2800
Wire Wire Line
	4650 2900 4750 2900
Wire Wire Line
	5700 2300 5700 2400
Wire Wire Line
	5650 2300 5700 2300
Wire Wire Line
	5650 2400 5700 2400
Wire Wire Line
	5700 2500 5650 2500
Wire Wire Line
	5700 2500 5700 2600
Wire Wire Line
	5700 2600 5650 2600
Wire Wire Line
	5650 2700 5700 2700
Wire Wire Line
	5700 2700 5700 2600
Connection ~ 5700 2600
Wire Wire Line
	5650 2800 5700 2800
Wire Wire Line
	5700 2800 5700 2700
Connection ~ 5700 2700
Wire Wire Line
	5650 2900 5700 2900
Wire Wire Line
	5700 2900 5700 2800
Connection ~ 5700 2800
Wire Wire Line
	5700 2500 5700 2400
Connection ~ 5700 2500
Connection ~ 5700 2400
Wire Wire Line
	6000 2500 5700 2500
$Comp
L Luciebox_components:Screw_Terminal_01x07_mirror J3
U 1 1 5D50A31C
P 5450 2600
F 0 "J3" H 5532 3117 50  0000 C CNN
F 1 "Screw_Terminal_01x07" H 5532 3026 50  0000 C CNN
F 2 "Luciebox:Terminal_7x1_100mil" H 5450 2600 50  0001 C CNN
F 3 "~" H 5450 2600 50  0001 C CNN
	1    5450 2600
	1    0    0    -1  
$EndComp
$Comp
L Luciebox_components:Screw_Terminal_01x07_mirror J1
U 1 1 5D554824
P 3200 2600
F 0 "J1" H 3282 3117 50  0000 C CNN
F 1 "Screw_Terminal_01x07" H 3282 3026 50  0000 C CNN
F 2 "Luciebox:Terminal_7x1_100mil" H 3200 2600 50  0001 C CNN
F 3 "~" H 3200 2600 50  0001 C CNN
	1    3200 2600
	1    0    0    -1  
$EndComp
Wire Wire Line
	3750 2400 3850 2400
Wire Wire Line
	3700 2500 3850 2500
Wire Wire Line
	3650 2600 3850 2600
Wire Wire Line
	3600 2700 3850 2700
Wire Wire Line
	3550 2800 3850 2800
Wire Wire Line
	3500 2900 3850 2900
Wire Wire Line
	3400 2900 3500 2900
Connection ~ 3500 2900
Wire Wire Line
	3400 2800 3550 2800
Connection ~ 3550 2800
Wire Wire Line
	3400 2700 3600 2700
Connection ~ 3600 2700
Wire Wire Line
	3650 2600 3400 2600
Connection ~ 3650 2600
Wire Wire Line
	3400 2500 3700 2500
Connection ~ 3700 2500
Wire Wire Line
	3400 2400 3750 2400
Connection ~ 3750 2400
Connection ~ 3800 2300
Wire Wire Line
	3400 2300 3800 2300
$Comp
L power:GND #PWR04
U 1 1 5D5B7F6D
P 4250 3100
F 0 "#PWR04" H 4250 2850 50  0001 C CNN
F 1 "GND" H 4255 2927 50  0000 C CNN
F 2 "" H 4250 3100 50  0001 C CNN
F 3 "" H 4250 3100 50  0001 C CNN
	1    4250 3100
	1    0    0    -1  
$EndComp
$Comp
L power:+12V #PWR03
U 1 1 5D5B8568
P 6000 2100
F 0 "#PWR03" H 6000 1950 50  0001 C CNN
F 1 "+12V" H 6015 2273 50  0000 C CNN
F 2 "" H 6000 2100 50  0001 C CNN
F 3 "" H 6000 2100 50  0001 C CNN
	1    6000 2100
	1    0    0    -1  
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
$Sheet
S 1250 2000 500  800 
U 5D74964C
F0 "R2-R_ladder_dac" 50
F1 "R2-R_ladder_dac.sch" 50
F2 "DAC_analog" O L 1250 2200 50 
$EndSheet
$Sheet
S 1250 3100 500  800 
U 5D761BCA
F0 "sheet5D761BCA" 50
F1 "R2-R_ladder_dac.sch" 50
F2 "DAC_analog" O L 1250 3300 50 
$EndSheet
Text Label 1250 2200 2    50   ~ 0
A1
Text Label 1250 3300 2    50   ~ 0
A2
$Comp
L Device:R_Small R7
U 1 1 5D798401
P 950 4900
F 0 "R7" H 1009 4946 50  0000 L CNN
F 1 "10K" H 1009 4855 50  0000 L CNN
F 2 "Luciebox:1206" H 950 4900 50  0001 C CNN
F 3 "~" H 950 4900 50  0001 C CNN
	1    950  4900
	0    1    1    0   
$EndComp
$Comp
L Device:R_Small R8
U 1 1 5D79CF90
P 1250 4900
F 0 "R8" H 1309 4946 50  0000 L CNN
F 1 "10K" H 1309 4855 50  0000 L CNN
F 2 "Luciebox:1206" H 1250 4900 50  0001 C CNN
F 3 "~" H 1250 4900 50  0001 C CNN
	1    1250 4900
	0    1    1    0   
$EndComp
Wire Wire Line
	1050 4900 1100 4900
$Comp
L Device:R_Small R9
U 1 1 5D7B59F4
P 1550 4900
F 0 "R9" H 1609 4946 50  0000 L CNN
F 1 "10K" H 1609 4855 50  0000 L CNN
F 2 "Luciebox:1206" H 1550 4900 50  0001 C CNN
F 3 "~" H 1550 4900 50  0001 C CNN
	1    1550 4900
	0    1    1    0   
$EndComp
$Comp
L Device:R_Small R10
U 1 1 5D7B59FE
P 1850 4900
F 0 "R10" H 1909 4946 50  0000 L CNN
F 1 "10K" H 1909 4855 50  0000 L CNN
F 2 "Luciebox:1206" H 1850 4900 50  0001 C CNN
F 3 "~" H 1850 4900 50  0001 C CNN
	1    1850 4900
	0    1    1    0   
$EndComp
Wire Wire Line
	1650 4900 1700 4900
Wire Wire Line
	1350 4900 1400 4900
Wire Wire Line
	1950 4900 2000 4900
$Comp
L Device:R_Small R11
U 1 1 5D7CF8B9
P 2150 4900
F 0 "R11" H 2209 4946 50  0000 L CNN
F 1 "10K" H 2209 4855 50  0000 L CNN
F 2 "Luciebox:1206" H 2150 4900 50  0001 C CNN
F 3 "~" H 2150 4900 50  0001 C CNN
	1    2150 4900
	0    1    1    0   
$EndComp
$Comp
L Device:R_Small R12
U 1 1 5D7CF8C3
P 2450 4900
F 0 "R12" H 2509 4946 50  0000 L CNN
F 1 "10K" H 2509 4855 50  0000 L CNN
F 2 "Luciebox:1206" H 2450 4900 50  0001 C CNN
F 3 "~" H 2450 4900 50  0001 C CNN
	1    2450 4900
	0    1    1    0   
$EndComp
Wire Wire Line
	2250 4900 2300 4900
$Comp
L Device:R_Small R13
U 1 1 5D7CF8CE
P 2750 4900
F 0 "R13" H 2809 4946 50  0000 L CNN
F 1 "10K" H 2809 4855 50  0000 L CNN
F 2 "Luciebox:1206" H 2750 4900 50  0001 C CNN
F 3 "~" H 2750 4900 50  0001 C CNN
	1    2750 4900
	0    1    1    0   
$EndComp
$Comp
L Device:R_Small R14
U 1 1 5D7CF8D8
P 3050 4900
F 0 "R14" H 3109 4946 50  0000 L CNN
F 1 "10K" H 3109 4855 50  0000 L CNN
F 2 "Luciebox:1206" H 3050 4900 50  0001 C CNN
F 3 "~" H 3050 4900 50  0001 C CNN
	1    3050 4900
	0    1    1    0   
$EndComp
Wire Wire Line
	2850 4900 2900 4900
Wire Wire Line
	2550 4900 2600 4900
Wire Wire Line
	3150 4900 3200 4900
$Comp
L Device:R_Small R15
U 1 1 5D7D8CA9
P 3350 4900
F 0 "R15" H 3409 4946 50  0000 L CNN
F 1 "10K" H 3409 4855 50  0000 L CNN
F 2 "Luciebox:1206" H 3350 4900 50  0001 C CNN
F 3 "~" H 3350 4900 50  0001 C CNN
	1    3350 4900
	0    1    1    0   
$EndComp
$Comp
L Device:R_Small R16
U 1 1 5D7D8CB3
P 3650 4900
F 0 "R16" H 3709 4946 50  0000 L CNN
F 1 "10K" H 3709 4855 50  0000 L CNN
F 2 "Luciebox:1206" H 3650 4900 50  0001 C CNN
F 3 "~" H 3650 4900 50  0001 C CNN
	1    3650 4900
	0    1    1    0   
$EndComp
Wire Wire Line
	3450 4900 3500 4900
$Comp
L Device:R_Small R17
U 1 1 5D7D8CBE
P 3950 4900
F 0 "R17" H 4009 4946 50  0000 L CNN
F 1 "10K" H 4009 4855 50  0000 L CNN
F 2 "Luciebox:1206" H 3950 4900 50  0001 C CNN
F 3 "~" H 3950 4900 50  0001 C CNN
	1    3950 4900
	0    1    1    0   
$EndComp
$Comp
L Device:R_Small R18
U 1 1 5D7D8CC8
P 4250 4900
F 0 "R18" H 4309 4946 50  0000 L CNN
F 1 "10K" H 4309 4855 50  0000 L CNN
F 2 "Luciebox:1206" H 4250 4900 50  0001 C CNN
F 3 "~" H 4250 4900 50  0001 C CNN
	1    4250 4900
	0    1    1    0   
$EndComp
Wire Wire Line
	4050 4900 4100 4900
Wire Wire Line
	3750 4900 3800 4900
Wire Wire Line
	4350 4900 4400 4900
Wire Wire Line
	850  4900 700  4900
$Comp
L power:GND #PWR012
U 1 1 5D7F94C7
P 700 4900
F 0 "#PWR012" H 700 4650 50  0001 C CNN
F 1 "GND" H 705 4727 50  0000 C CNN
F 2 "" H 700 4900 50  0001 C CNN
F 3 "" H 700 4900 50  0001 C CNN
	1    700  4900
	1    0    0    -1  
$EndComp
$Comp
L Luciebox_components:VCC_minimalCurrent #PWR013
U 1 1 5D800E0F
P 4800 4900
F 0 "#PWR013" H 4800 4750 50  0001 C CNN
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
	4400 4900 4800 4900
$Comp
L Luciebox_components:terminal_13x1 T1
U 1 1 5D88583A
P 3200 4500
F 0 "T1" V 3615 5125 50  0000 C CNN
F 1 "terminal_13x1" V 3524 5125 50  0000 C CNN
F 2 "Luciebox:Terminal_13x1_100mil" H 3000 4400 50  0001 C CNN
F 3 "" H 3100 5600 50  0001 C CNN
	1    3200 4500
	0    -1   -1   0   
$EndComp
Wire Wire Line
	750  4550 2000 4550
Text Label 750  4550 2    50   ~ 0
A0
$Comp
L Device:R_POT RV1
U 1 1 5D8DAB75
P 1500 5850
F 0 "RV1" V 1293 5850 50  0000 C CNN
F 1 "R_POT" V 1384 5850 50  0000 C CNN
F 2 "Luciebox:BIG_POTENTIO" H 1500 5850 50  0001 C CNN
F 3 "~" H 1500 5850 50  0001 C CNN
	1    1500 5850
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR015
U 1 1 5D90E4DE
P 1250 5850
F 0 "#PWR015" H 1250 5600 50  0001 C CNN
F 1 "GND" H 1255 5677 50  0000 C CNN
F 2 "" H 1250 5850 50  0001 C CNN
F 3 "" H 1250 5850 50  0001 C CNN
	1    1250 5850
	1    0    0    -1  
$EndComp
$Comp
L Luciebox_components:VCC_minimalCurrent #PWR016
U 1 1 5D90EA1A
P 2250 5850
F 0 "#PWR016" H 2250 5700 50  0001 C CNN
F 1 "VCC_minimalCurrent" H 2267 6023 50  0000 C CNN
F 2 "" H 2250 5850 50  0001 C CNN
F 3 "" H 2250 5850 50  0001 C CNN
	1    2250 5850
	1    0    0    -1  
$EndComp
Wire Wire Line
	1350 5850 1300 5850
Wire Wire Line
	1500 6000 1500 6150
Wire Wire Line
	1500 6250 1250 6250
Text Label 1250 6250 2    50   ~ 0
A3
$Sheet
S 1250 750  500  800 
U 5D9307E8
F0 "Tilt switches" 50
F1 "R2-R_ladder_dac_tilt_switches.sch" 50
F2 "DAC_analog" O L 1250 950 50 
$EndSheet
Text Label 1250 950  2    50   ~ 0
A4(SDA)
$Comp
L Device:Buzzer BZ1
U 1 1 5D93C6F4
P 1400 6750
F 0 "BZ1" V 1451 6563 50  0000 R CNN
F 1 "Buzzer" V 1360 6563 50  0000 R CNN
F 2 "Luciebox:buzzer" V 1375 6850 50  0001 C CNN
F 3 "~" V 1375 6850 50  0001 C CNN
	1    1400 6750
	0    -1   -1   0   
$EndComp
Wire Wire Line
	1300 6850 1150 6850
Wire Wire Line
	1500 6850 2000 6850
$Comp
L power:GND #PWR018
U 1 1 5D974320
P 2000 6850
F 0 "#PWR018" H 2000 6600 50  0001 C CNN
F 1 "GND" H 2005 6677 50  0000 C CNN
F 2 "" H 2000 6850 50  0001 C CNN
F 3 "" H 2000 6850 50  0001 C CNN
	1    2000 6850
	1    0    0    -1  
$EndComp
Text Label 1150 6850 2    50   ~ 0
A5(SCL)
$Comp
L power:GND #PWR017
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
L power:VCC #PWR08
U 1 1 5D991DF2
P 7900 3450
F 0 "#PWR08" H 7900 3300 50  0001 C CNN
F 1 "VCC" H 7917 3623 50  0000 C CNN
F 2 "" H 7900 3450 50  0001 C CNN
F 3 "" H 7900 3450 50  0001 C CNN
	1    7900 3450
	1    0    0    -1  
$EndComp
$Comp
L power:VCC #PWR09
U 1 1 5D992AD7
P 8000 3450
F 0 "#PWR09" H 8000 3300 50  0001 C CNN
F 1 "VCC" H 8017 3623 50  0000 C CNN
F 2 "" H 8000 3450 50  0001 C CNN
F 3 "" H 8000 3450 50  0001 C CNN
	1    8000 3450
	1    0    0    -1  
$EndComp
$Comp
L power:VCC #PWR010
U 1 1 5D993297
P 7300 3750
F 0 "#PWR010" H 7300 3600 50  0001 C CNN
F 1 "VCC" H 7317 3923 50  0000 C CNN
F 2 "" H 7300 3750 50  0001 C CNN
F 3 "" H 7300 3750 50  0001 C CNN
	1    7300 3750
	1    0    0    -1  
$EndComp
$Comp
L Device:Crystal Y1
U 1 1 5D9962E4
P 9750 4500
F 0 "Y1" V 9659 4631 50  0000 L CNN
F 1 "16MHz" V 9750 4631 50  0000 L CNN
F 2 "Luciebox:CRYSTAL" V 9841 4631 50  0000 L CNN
F 3 "~" H 9750 4500 50  0001 C CNN
	1    9750 4500
	0    1    1    0   
$EndComp
Wire Wire Line
	8500 4350 9300 4350
$Comp
L Device:R_Small R6
U 1 1 5D9BA081
P 9300 4450
F 0 "R6" H 9359 4496 50  0000 L CNN
F 1 "1M" H 9359 4405 50  0000 L CNN
F 2 "Luciebox:1206" H 9300 4450 50  0001 C CNN
F 3 "~" H 9300 4450 50  0001 C CNN
	1    9300 4450
	1    0    0    -1  
$EndComp
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
$Comp
L Device:C_Small C2
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
L Device:C_Small C3
U 1 1 5D9EAF46
P 10250 4700
F 0 "C3" V 10021 4700 50  0000 C CNN
F 1 "20pF" V 10112 4700 50  0000 C CNN
F 2 "Luciebox:0805" H 10250 4700 50  0001 C CNN
F 3 "~" H 10250 4700 50  0001 C CNN
	1    10250 4700
	0    1    1    0   
$EndComp
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
L power:GND #PWR011
U 1 1 5DA1D313
P 10700 4500
F 0 "#PWR011" H 10700 4250 50  0001 C CNN
F 1 "GND" H 10700 4350 50  0000 C CNN
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
$Comp
L Device:C_Small C1
U 1 1 5DA2A1AC
P 6950 3400
F 0 "C1" V 6721 3400 50  0000 C CNN
F 1 "100nF" V 6812 3400 50  0000 C CNN
F 2 "Luciebox:0805" H 6950 3400 50  0001 C CNN
F 3 "~" H 6950 3400 50  0001 C CNN
	1    6950 3400
	0    1    1    0   
$EndComp
Wire Wire Line
	7900 3450 7050 3450
Wire Wire Line
	7050 3450 7050 3400
Connection ~ 7900 3450
$Comp
L power:GND #PWR07
U 1 1 5DA38582
P 6700 3400
F 0 "#PWR07" H 6700 3150 50  0001 C CNN
F 1 "GND" H 6705 3227 50  0000 C CNN
F 2 "" H 6700 3400 50  0001 C CNN
F 3 "" H 6700 3400 50  0001 C CNN
	1    6700 3400
	1    0    0    -1  
$EndComp
Wire Wire Line
	6700 3400 6850 3400
$Comp
L Device:R_Small R19
U 1 1 5DA45BE6
P 9500 5250
F 0 "R19" H 9559 5296 50  0000 L CNN
F 1 "100" H 9559 5205 50  0000 L CNN
F 2 "Luciebox:1206" H 9500 5250 50  0001 C CNN
F 3 "~" H 9500 5250 50  0001 C CNN
	1    9500 5250
	0    1    1    0   
$EndComp
Wire Wire Line
	9400 5250 9250 5250
$Comp
L power:VCC #PWR014
U 1 1 5DA5FE62
P 9900 5250
F 0 "#PWR014" H 9900 5100 50  0001 C CNN
F 1 "VCC" H 9917 5423 50  0000 C CNN
F 2 "" H 9900 5250 50  0001 C CNN
F 3 "" H 9900 5250 50  0001 C CNN
	1    9900 5250
	1    0    0    -1  
$EndComp
Wire Wire Line
	9600 5250 9900 5250
$Comp
L Connector:Screw_Terminal_01x07 J4
U 1 1 5DA6F0A0
P 10900 5550
F 0 "J4" H 10980 5592 50  0000 L CNN
F 1 "Screw_Terminal_01x07" H 10980 5501 50  0000 L CNN
F 2 "Luciebox:Terminal_7x1_100mil" H 10900 5550 50  0001 C CNN
F 3 "~" H 10900 5550 50  0001 C CNN
	1    10900 5550
	1    0    0    -1  
$EndComp
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
$Comp
L Luciebox_components:N-Channel_Mosfet Q2
U 1 1 5D6BDEDC
P 3900 6750
F 0 "Q2" H 4106 6841 50  0000 L CNN
F 1 "N-Channel_Mosfet" H 4106 6750 50  0000 L CNN
F 2 "Luciebox:TO-220-3" H 4106 6659 50  0000 L CIN
F 3 "https://www.onsemi.com/pub/Collateral/FQP30N06L-D.pdf" V 3900 6750 50  0001 L CNN
	1    3900 6750
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small R47
U 1 1 5D6C3437
P 3300 6450
F 0 "R47" H 3359 6496 50  0000 L CNN
F 1 "100K" H 3359 6405 50  0000 L CNN
F 2 "Luciebox:1206" H 3300 6450 50  0001 C CNN
F 3 "~" H 3300 6450 50  0001 C CNN
	1    3300 6450
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small R48
U 1 1 5D6C3ACD
P 3300 6850
F 0 "R48" H 3359 6896 50  0000 L CNN
F 1 "100K" H 3359 6805 50  0000 L CNN
F 2 "Luciebox:1206" H 3300 6850 50  0001 C CNN
F 3 "~" H 3300 6850 50  0001 C CNN
	1    3300 6850
	1    0    0    -1  
$EndComp
Wire Wire Line
	3300 6550 3300 6750
Connection ~ 3300 6750
Text Label 3300 6350 0    50   ~ 0
0(Rx)
$Comp
L power:GND #PWR0101
U 1 1 5D70EC69
P 4150 6250
F 0 "#PWR0101" H 4150 6000 50  0001 C CNN
F 1 "GND" H 4155 6077 50  0000 C CNN
F 2 "" H 4150 6250 50  0001 C CNN
F 3 "" H 4150 6250 50  0001 C CNN
	1    4150 6250
	1    0    0    -1  
$EndComp
Wire Wire Line
	4000 6950 3300 6950
Wire Wire Line
	4150 6250 4000 6250
Wire Wire Line
	4000 6250 4000 6500
Wire Wire Line
	4000 6950 4000 7400
Connection ~ 4000 6950
$Comp
L Connector:Screw_Terminal_01x07 J5
U 1 1 5D74D272
P 6100 7100
F 0 "J5" H 6050 7700 50  0000 L CNN
F 1 "Screw_Terminal_01x07" H 5750 7600 50  0000 L CNN
F 2 "Luciebox:Terminal_7x1_100mil" H 6100 7100 50  0001 C CNN
F 3 "~" H 6100 7100 50  0001 C CNN
	1    6100 7100
	1    0    0    -1  
$EndComp
Wire Wire Line
	5100 6500 4000 6500
Wire Wire Line
	4000 6500 4000 6550
Wire Wire Line
	5900 6800 5100 6800
Wire Wire Line
	5100 6800 5100 6500
Wire Wire Line
	5900 6900 5100 6900
Wire Wire Line
	5100 7400 4000 7400
Wire Wire Line
	5100 6900 5100 7000
Wire Wire Line
	5900 7000 5100 7000
Connection ~ 5100 7000
Wire Wire Line
	5100 7000 5100 7400
Wire Wire Line
	5900 7100 5350 7100
$Comp
L power:VCC #PWR0102
U 1 1 5D7FF3DB
P 5700 7300
F 0 "#PWR0102" H 5700 7150 50  0001 C CNN
F 1 "VCC" H 5718 7473 50  0000 C CNN
F 2 "" H 5700 7300 50  0001 C CNN
F 3 "" H 5700 7300 50  0001 C CNN
	1    5700 7300
	-1   0    0    1   
$EndComp
Text Notes 6200 7350 0    50   ~ 0
BAT+
Text Notes 6200 7250 0    50   ~ 0
BAT-
Text Notes 6200 7050 0    50   ~ 0
MASTER-ON
Text Notes 6200 7150 0    50   ~ 0
MASTER-ON
Text Notes 6200 6850 0    50   ~ 0
TOUCH-ON
Text Notes 6200 6950 0    50   ~ 0
TOUCH-ON
Wire Wire Line
	5350 7200 5350 7100
Wire Wire Line
	5350 7200 5900 7200
Wire Wire Line
	5900 7300 5700 7300
Text Notes 3750 6050 0    50   ~ 0
Touch ON and Auto power OFF 
$Comp
L Luciebox_components:terminal_3x1 T11
U 1 1 5DC4B415
P 1600 5450
F 0 "T11" V 1738 5372 50  0000 R CNN
F 1 "terminal_3x1" V 1647 5372 50  0000 R CNN
F 2 "Luciebox:Terminal_3x1_100mil" H 1400 5350 50  0001 C CNN
F 3 "" H 1550 5850 50  0001 C CNN
	1    1600 5450
	0    -1   -1   0   
$EndComp
Wire Wire Line
	1400 5500 1400 5650
Wire Wire Line
	1400 5650 1300 5650
Wire Wire Line
	1300 5650 1300 5850
Connection ~ 1300 5850
Wire Wire Line
	1300 5850 1250 5850
Wire Wire Line
	1600 5500 1750 5500
Wire Wire Line
	1750 5500 1750 5850
Wire Wire Line
	1500 5500 1500 5600
Connection ~ 1500 6150
Wire Wire Line
	1500 6150 1500 6250
Wire Wire Line
	1750 5850 1650 5850
Connection ~ 1750 5850
Wire Wire Line
	1100 5600 1100 6150
Wire Wire Line
	1500 5600 1100 5600
Wire Wire Line
	1500 6150 1100 6150
Wire Wire Line
	3300 6750 3750 6750
$Comp
L power:+12V #PWR0103
U 1 1 5DC88B13
P 5900 7400
F 0 "#PWR0103" H 5900 7250 50  0001 C CNN
F 1 "+12V" H 5915 7573 50  0000 C CNN
F 2 "" H 5900 7400 50  0001 C CNN
F 3 "" H 5900 7400 50  0001 C CNN
	1    5900 7400
	-1   0    0    1   
$EndComp
Text Notes 6200 7450 0    50   ~ 0
12V
$Comp
L Mechanical:MountingHole H4
U 1 1 5DC89D39
P 5600 5450
F 0 "H4" H 5700 5496 50  0000 L CNN
F 1 "MountingHole" H 5700 5405 50  0000 L CNN
F 2 "MountingHole:MountingHole_4mm" H 5600 5450 50  0001 C CNN
F 3 "~" H 5600 5450 50  0001 C CNN
	1    5600 5450
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole H1
U 1 1 5DC9D1FF
P 5600 5650
F 0 "H1" H 5700 5696 50  0000 L CNN
F 1 "MountingHole" H 5700 5605 50  0000 L CNN
F 2 "MountingHole:MountingHole_4mm" H 5600 5650 50  0001 C CNN
F 3 "~" H 5600 5650 50  0001 C CNN
	1    5600 5650
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole H2
U 1 1 5DC9D279
P 5600 5800
F 0 "H2" H 5700 5846 50  0000 L CNN
F 1 "MountingHole" H 5700 5755 50  0000 L CNN
F 2 "MountingHole:MountingHole_4mm" H 5600 5800 50  0001 C CNN
F 3 "~" H 5600 5800 50  0001 C CNN
	1    5600 5800
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole H3
U 1 1 5DC9D2EF
P 5650 6000
F 0 "H3" H 5750 6046 50  0000 L CNN
F 1 "MountingHole" H 5750 5955 50  0000 L CNN
F 2 "MountingHole:MountingHole_4mm" H 5650 6000 50  0001 C CNN
F 3 "~" H 5650 6000 50  0001 C CNN
	1    5650 6000
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole H8
U 1 1 5DC9D36B
P 6250 6000
F 0 "H8" H 6350 6046 50  0000 L CNN
F 1 "MountingHole" H 6350 5955 50  0000 L CNN
F 2 "MountingHole:MountingHole_4mm" H 6250 6000 50  0001 C CNN
F 3 "~" H 6250 6000 50  0001 C CNN
	1    6250 6000
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole H7
U 1 1 5DC9D403
P 6200 5800
F 0 "H7" H 6300 5846 50  0000 L CNN
F 1 "MountingHole" H 6300 5755 50  0000 L CNN
F 2 "MountingHole:MountingHole_4mm" H 6200 5800 50  0001 C CNN
F 3 "~" H 6200 5800 50  0001 C CNN
	1    6200 5800
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole H6
U 1 1 5DC9D485
P 6200 5600
F 0 "H6" H 6300 5646 50  0000 L CNN
F 1 "MountingHole" H 6300 5555 50  0000 L CNN
F 2 "MountingHole:MountingHole_4mm" H 6200 5600 50  0001 C CNN
F 3 "~" H 6200 5600 50  0001 C CNN
	1    6200 5600
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole H5
U 1 1 5DC9D509
P 6200 5400
F 0 "H5" H 6300 5446 50  0000 L CNN
F 1 "MountingHole" H 6300 5355 50  0000 L CNN
F 2 "MountingHole:MountingHole_4mm" H 6200 5400 50  0001 C CNN
F 3 "~" H 6200 5400 50  0001 C CNN
	1    6200 5400
	1    0    0    -1  
$EndComp
$Comp
L Luciebox_components:8X8PADS U4
U 1 1 5DD3C18A
P 5700 4850
F 0 "U4" H 6150 6050 50  0000 C CNN
F 1 "8X8PADS" H 6100 5950 50  0000 C CNN
F 2 "Luciebox:64_PADS_100mil" H 5700 4850 50  0001 C CNN
F 3 "" H 5700 4850 50  0001 C CNN
	1    5700 4850
	1    0    0    -1  
$EndComp
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
L power:GND #PWR031
U 1 1 5DCBF971
P 6450 4700
F 0 "#PWR031" H 6450 4450 50  0001 C CNN
F 1 "GND" H 6455 4527 50  0000 C CNN
F 2 "" H 6450 4700 50  0001 C CNN
F 3 "" H 6450 4700 50  0001 C CNN
	1    6450 4700
	1    0    0    -1  
$EndComp
$Comp
L power:VCC #PWR030
U 1 1 5DCC8327
P 5400 4000
F 0 "#PWR030" H 5400 3850 50  0001 C CNN
F 1 "VCC" H 5417 4173 50  0000 C CNN
F 2 "" H 5400 4000 50  0001 C CNN
F 3 "" H 5400 4000 50  0001 C CNN
	1    5400 4000
	1    0    0    -1  
$EndComp
Connection ~ 5400 4000
Connection ~ 6450 4700
$Comp
L Device:R_Small R49
U 1 1 5E1772EF
P 3050 3500
F 0 "R49" H 3109 3546 50  0000 L CNN
F 1 "10K" H 3109 3455 50  0000 L CNN
F 2 "Luciebox:1206" H 3050 3500 50  0001 C CNN
F 3 "~" H 3050 3500 50  0001 C CNN
	1    3050 3500
	0    1    1    0   
$EndComp
$Comp
L power:VCC #PWR029
U 1 1 5E19FB8B
P 3150 3500
F 0 "#PWR029" H 3150 3350 50  0001 C CNN
F 1 "VCC" H 3167 3673 50  0000 C CNN
F 2 "" H 3150 3500 50  0001 C CNN
F 3 "" H 3150 3500 50  0001 C CNN
	1    3150 3500
	1    0    0    -1  
$EndComp
$Comp
L Luciebox_components:VCC_minimalCurrent #PWR028
U 1 1 5E1A5201
P 2550 3450
F 0 "#PWR028" H 2550 3300 50  0001 C CNN
F 1 "VCC_minimalCurrent" H 2567 3623 50  0000 C CNN
F 2 "" H 2550 3450 50  0001 C CNN
F 3 "" H 2550 3450 50  0001 C CNN
	1    2550 3450
	1    0    0    -1  
$EndComp
Wire Wire Line
	2950 3500 2550 3500
Wire Wire Line
	2550 3500 2550 3450
Wire Wire Line
	1750 5850 2250 5850
$EndSCHEMATC
