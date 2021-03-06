EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "Cronotermostato"
Date "2020-03-08"
Rev "1.0"
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L MCU_Microchip_ATmega:ATmega328-PU U2
U 1 1 5E656AC5
P 5000 3300
F 0 "U2" H 4356 3346 50  0000 R CNN
F 1 "ATmega328-PU" H 4356 3255 50  0000 R CNN
F 2 "Package_DIP:DIP-28_W7.62mm" H 5000 3300 50  0001 C CIN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/ATmega328_P%20AVR%20MCU%20with%20picoPower%20Technology%20Data%20Sheet%2040001984A.pdf" H 5000 3300 50  0001 C CNN
	1    5000 3300
	1    0    0    -1  
$EndComp
$Comp
L Device:Crystal Y1
U 1 1 5E65903F
P 7550 2750
F 0 "Y1" V 7504 2881 50  0000 L CNN
F 1 "Crystal" V 7595 2881 50  0000 L CNN
F 2 "Crystal:Crystal_HC49-4H_Vertical" H 7550 2750 50  0001 C CNN
F 3 "~" H 7550 2750 50  0001 C CNN
	1    7550 2750
	0    1    1    0   
$EndComp
$Comp
L Device:C_Small C3
U 1 1 5E65A195
P 7550 2300
F 0 "C3" H 7642 2346 50  0000 L CNN
F 1 "22pf" H 7642 2255 50  0000 L CNN
F 2 "Capacitor_THT:C_Disc_D3.0mm_W1.6mm_P2.50mm" H 7550 2300 50  0001 C CNN
F 3 "~" H 7550 2300 50  0001 C CNN
	1    7550 2300
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C4
U 1 1 5E65A9A0
P 7550 3250
F 0 "C4" H 7642 3296 50  0000 L CNN
F 1 "22pf" H 7642 3205 50  0000 L CNN
F 2 "Capacitor_THT:C_Disc_D3.0mm_W1.6mm_P2.50mm" H 7550 3250 50  0001 C CNN
F 3 "~" H 7550 3250 50  0001 C CNN
	1    7550 3250
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0101
U 1 1 5E65AEB4
P 8200 2200
F 0 "#PWR0101" H 8200 1950 50  0001 C CNN
F 1 "GND" H 8205 2027 50  0000 C CNN
F 2 "" H 8200 2200 50  0001 C CNN
F 3 "" H 8200 2200 50  0001 C CNN
	1    8200 2200
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0102
U 1 1 5E65B5F8
P 7550 3600
F 0 "#PWR0102" H 7550 3350 50  0001 C CNN
F 1 "GND" H 7555 3427 50  0000 C CNN
F 2 "" H 7550 3600 50  0001 C CNN
F 3 "" H 7550 3600 50  0001 C CNN
	1    7550 3600
	1    0    0    -1  
$EndComp
Wire Wire Line
	7550 2200 8200 2200
Wire Wire Line
	7550 2900 7550 2950
Wire Wire Line
	7550 3350 7550 3600
Wire Wire Line
	5950 2950 5950 2800
Wire Wire Line
	5950 2800 5600 2800
Connection ~ 7550 2950
Wire Wire Line
	7550 2950 7550 3150
Wire Wire Line
	7550 2600 7550 2550
Wire Wire Line
	5950 2550 5950 2700
Wire Wire Line
	5950 2700 5600 2700
Connection ~ 7550 2550
Wire Wire Line
	7550 2550 7550 2400
$Comp
L power:GND #PWR0103
U 1 1 5E65C780
P 5000 4950
F 0 "#PWR0103" H 5000 4700 50  0001 C CNN
F 1 "GND" H 5005 4777 50  0000 C CNN
F 2 "" H 5000 4950 50  0001 C CNN
F 3 "" H 5000 4950 50  0001 C CNN
	1    5000 4950
	1    0    0    -1  
$EndComp
Wire Wire Line
	5000 4950 5000 4800
$Comp
L power:+5V #PWR0104
U 1 1 5E65CDAD
P 5000 1600
F 0 "#PWR0104" H 5000 1450 50  0001 C CNN
F 1 "+5V" H 5015 1773 50  0000 C CNN
F 2 "" H 5000 1600 50  0001 C CNN
F 3 "" H 5000 1600 50  0001 C CNN
	1    5000 1600
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0105
U 1 1 5E65D649
P 7100 4000
F 0 "#PWR0105" H 7100 3850 50  0001 C CNN
F 1 "+5V" H 7115 4173 50  0000 C CNN
F 2 "" H 7100 4000 50  0001 C CNN
F 3 "" H 7100 4000 50  0001 C CNN
	1    7100 4000
	1    0    0    -1  
$EndComp
Wire Wire Line
	5000 1800 5000 1600
Wire Wire Line
	5000 1800 5100 1800
Connection ~ 5000 1800
$Comp
L Device:D D1
U 1 1 5E65ECD6
P 6600 4200
F 0 "D1" H 6700 4300 50  0000 C CNN
F 1 "1N4148" H 6600 4100 50  0000 C CNN
F 2 "Diode_SMD:D_SOD-123" H 6600 4200 50  0001 C CNN
F 3 "~" H 6600 4200 50  0001 C CNN
	1    6600 4200
	-1   0    0    1   
$EndComp
$Comp
L Device:R_Small_US R2
U 1 1 5E65F87E
P 6600 4500
F 0 "R2" V 6550 4600 50  0000 C CNN
F 1 "220" V 6700 4500 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric" H 6600 4500 50  0001 C CNN
F 3 "~" H 6600 4500 50  0001 C CNN
	1    6600 4500
	0    1    1    0   
$EndComp
Wire Wire Line
	6750 4200 6750 4500
Wire Wire Line
	6750 4500 6700 4500
Wire Wire Line
	6750 4200 7100 4200
Wire Wire Line
	7100 4200 7100 4000
Connection ~ 6750 4200
Wire Wire Line
	6500 4500 6300 4500
Wire Wire Line
	6300 4500 6300 4200
Wire Wire Line
	6300 4200 6450 4200
Wire Wire Line
	5600 3600 6000 3600
Wire Wire Line
	6000 3600 6000 4200
Wire Wire Line
	6000 4200 6300 4200
Connection ~ 6300 4200
$Comp
L Connector:Conn_01x02_Male J4
U 1 1 5E6617A8
P 7250 4850
F 0 "J4" H 7222 4732 50  0000 R CNN
F 1 "RST" H 7222 4823 50  0000 R CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 7250 4850 50  0001 C CNN
F 3 "~" H 7250 4850 50  0001 C CNN
	1    7250 4850
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR0106
U 1 1 5E663684
P 7050 5050
F 0 "#PWR0106" H 7050 4800 50  0001 C CNN
F 1 "GND" H 7055 4877 50  0000 C CNN
F 2 "" H 7050 5050 50  0001 C CNN
F 3 "" H 7050 5050 50  0001 C CNN
	1    7050 5050
	1    0    0    -1  
$EndComp
Wire Wire Line
	7050 5050 7050 4850
Wire Wire Line
	7050 4750 6000 4750
Wire Wire Line
	6000 4750 6000 4200
Connection ~ 6000 4200
$Comp
L Regulator_Linear:AMS1117 U1
U 1 1 5E6649A6
P 2400 2000
F 0 "U1" H 2400 2242 50  0000 C CNN
F 1 "AMS1117" H 2400 2151 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-223-3_TabPin2" H 2400 2200 50  0001 C CNN
F 3 "http://www.advanced-monolithic.com/pdf/ds1117.pdf" H 2500 1750 50  0001 C CNN
	1    2400 2000
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0107
U 1 1 5E666DE2
P 8950 5200
F 0 "#PWR0107" H 8950 5050 50  0001 C CNN
F 1 "+5V" H 8965 5373 50  0000 C CNN
F 2 "" H 8950 5200 50  0001 C CNN
F 3 "" H 8950 5200 50  0001 C CNN
	1    8950 5200
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0108
U 1 1 5E667AFC
P 8950 5750
F 0 "#PWR0108" H 8950 5500 50  0001 C CNN
F 1 "GND" H 8955 5577 50  0000 C CNN
F 2 "" H 8950 5750 50  0001 C CNN
F 3 "" H 8950 5750 50  0001 C CNN
	1    8950 5750
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0109
U 1 1 5E668A9C
P 1850 1900
F 0 "#PWR0109" H 1850 1750 50  0001 C CNN
F 1 "+5V" H 1865 2073 50  0000 C CNN
F 2 "" H 1850 1900 50  0001 C CNN
F 3 "" H 1850 1900 50  0001 C CNN
	1    1850 1900
	1    0    0    -1  
$EndComp
Wire Wire Line
	1850 1900 1850 2000
Wire Wire Line
	1850 2000 2100 2000
$Comp
L Device:CP1_Small C1
U 1 1 5E6697D3
P 1600 2150
F 0 "C1" H 1691 2196 50  0000 L CNN
F 1 "10uf" H 1691 2105 50  0000 L CNN
F 2 "Capacitor_SMD:CP_Elec_4x5.3" H 1600 2150 50  0001 C CNN
F 3 "~" H 1600 2150 50  0001 C CNN
	1    1600 2150
	1    0    0    -1  
$EndComp
$Comp
L Device:CP1_Small C2
U 1 1 5E66A055
P 3000 2100
F 0 "C2" H 3091 2146 50  0000 L CNN
F 1 "10uf" H 3091 2055 50  0000 L CNN
F 2 "Capacitor_SMD:CP_Elec_4x4.5" H 3000 2100 50  0001 C CNN
F 3 "~" H 3000 2100 50  0001 C CNN
	1    3000 2100
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0110
U 1 1 5E66A8CE
P 2400 2450
F 0 "#PWR0110" H 2400 2200 50  0001 C CNN
F 1 "GND" H 2405 2277 50  0000 C CNN
F 2 "" H 2400 2450 50  0001 C CNN
F 3 "" H 2400 2450 50  0001 C CNN
	1    2400 2450
	1    0    0    -1  
$EndComp
Wire Wire Line
	2400 2300 2400 2400
Wire Wire Line
	1600 2250 1600 2400
Wire Wire Line
	1600 2400 2400 2400
Connection ~ 2400 2400
Wire Wire Line
	2400 2400 2400 2450
Wire Wire Line
	2400 2400 3000 2400
Wire Wire Line
	3000 2400 3000 2200
Wire Wire Line
	3000 2000 2700 2000
Wire Wire Line
	1600 2050 1600 2000
Wire Wire Line
	1600 2000 1850 2000
Connection ~ 1850 2000
$Comp
L power:+3.3V #PWR0111
U 1 1 5E66D86A
P 3000 1900
F 0 "#PWR0111" H 3000 1750 50  0001 C CNN
F 1 "+3.3V" H 3015 2073 50  0000 C CNN
F 2 "" H 3000 1900 50  0001 C CNN
F 3 "" H 3000 1900 50  0001 C CNN
	1    3000 1900
	1    0    0    -1  
$EndComp
Wire Wire Line
	3000 2000 3000 1900
Connection ~ 3000 2000
$Comp
L power:GND #PWR0112
U 1 1 5E66FEFB
P 3050 3150
F 0 "#PWR0112" H 3050 2900 50  0001 C CNN
F 1 "GND" H 3055 2977 50  0000 C CNN
F 2 "" H 3050 3150 50  0001 C CNN
F 3 "" H 3050 3150 50  0001 C CNN
	1    3050 3150
	1    0    0    -1  
$EndComp
Wire Wire Line
	2600 3300 2600 3150
$Comp
L Connector:Conn_01x04_Male J1
U 1 1 5E672D32
P 1900 3400
F 0 "J1" H 2008 3681 50  0000 C CNN
F 1 "Display" H 2008 3590 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Horizontal" H 1900 3400 50  0001 C CNN
F 3 "~" H 1900 3400 50  0001 C CNN
	1    1900 3400
	1    0    0    -1  
$EndComp
Wire Wire Line
	2100 3300 2600 3300
Wire Wire Line
	2100 3400 2800 3400
Wire Wire Line
	2100 3500 2800 3500
Wire Wire Line
	5600 3800 5850 3800
Wire Wire Line
	5600 3900 5850 3900
Text Label 5850 3800 0    50   ~ 0
RX
Text Label 5850 3900 0    50   ~ 0
TX
$Comp
L Connector:Conn_01x05_Male J2
U 1 1 5E67B3F9
P 1900 4800
F 0 "J2" H 2008 5181 50  0000 C CNN
F 1 "DS3231" H 2008 5090 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x05_P2.54mm_Vertical" H 1900 4800 50  0001 C CNN
F 3 "~" H 1900 4800 50  0001 C CNN
	1    1900 4800
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0113
U 1 1 5E67CD72
P 3150 5000
F 0 "#PWR0113" H 3150 4750 50  0001 C CNN
F 1 "GND" H 3155 4827 50  0000 C CNN
F 2 "" H 3150 5000 50  0001 C CNN
F 3 "" H 3150 5000 50  0001 C CNN
	1    3150 5000
	1    0    0    -1  
$EndComp
Wire Wire Line
	2800 4600 2100 4600
$Comp
L power:+3.3V #PWR0114
U 1 1 5E67DF11
P 2800 4600
F 0 "#PWR0114" H 2800 4450 50  0001 C CNN
F 1 "+3.3V" H 2815 4773 50  0000 C CNN
F 2 "" H 2800 4600 50  0001 C CNN
F 3 "" H 2800 4600 50  0001 C CNN
	1    2800 4600
	1    0    0    -1  
$EndComp
Wire Wire Line
	3150 5000 2100 5000
Wire Wire Line
	2100 4800 2350 4800
Text Label 2350 4800 0    50   ~ 0
SCL
Wire Wire Line
	5950 2550 7550 2550
Wire Wire Line
	5950 2950 7550 2950
$Comp
L Device:R_Small_US R1
U 1 1 5E68B2A0
P 6350 3300
F 0 "R1" V 6300 3400 50  0000 C CNN
F 1 "220" V 6450 3300 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric" H 6350 3300 50  0001 C CNN
F 3 "~" H 6350 3300 50  0001 C CNN
	1    6350 3300
	-1   0    0    1   
$EndComp
$Comp
L Device:R_Small_US R3
U 1 1 5E68BA8A
P 6700 3400
F 0 "R3" V 6650 3500 50  0000 C CNN
F 1 "220" V 6800 3400 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric" H 6700 3400 50  0001 C CNN
F 3 "~" H 6700 3400 50  0001 C CNN
	1    6700 3400
	-1   0    0    1   
$EndComp
$Comp
L power:+5V #PWR0115
U 1 1 5E68C0AE
P 6950 3200
F 0 "#PWR0115" H 6950 3050 50  0001 C CNN
F 1 "+5V" H 6965 3373 50  0000 C CNN
F 2 "" H 6950 3200 50  0001 C CNN
F 3 "" H 6950 3200 50  0001 C CNN
	1    6950 3200
	1    0    0    -1  
$EndComp
Wire Wire Line
	5600 3500 6100 3500
Wire Wire Line
	5600 3400 6000 3400
Wire Wire Line
	6700 3300 6700 3200
Wire Wire Line
	6700 3200 6350 3200
Wire Wire Line
	6700 3200 6950 3200
Connection ~ 6700 3200
Wire Wire Line
	6000 3400 6000 3100
Wire Wire Line
	6000 3100 6200 3100
Connection ~ 6000 3400
Wire Wire Line
	6000 3400 6350 3400
Text Label 6200 3100 0    50   ~ 0
SDA
Wire Wire Line
	6100 3500 6100 3700
Wire Wire Line
	6100 3700 6200 3700
Connection ~ 6100 3500
Wire Wire Line
	6100 3500 6700 3500
Text Label 6200 3700 0    50   ~ 0
SCL
$Comp
L Connector:Screw_Terminal_01x03 J5
U 1 1 5E69848F
P 9350 5500
F 0 "J5" H 9430 5542 50  0000 L CNN
F 1 "MainConn" H 9430 5451 50  0000 L CNN
F 2 "TerminalBlock_4Ucon:TerminalBlock_4Ucon_1x03_P3.50mm_Horizontal" H 9350 5500 50  0001 C CNN
F 3 "~" H 9350 5500 50  0001 C CNN
	1    9350 5500
	1    0    0    -1  
$EndComp
Wire Wire Line
	9150 5400 8950 5400
Wire Wire Line
	8950 5400 8950 5200
Wire Wire Line
	9150 5600 8950 5600
Wire Wire Line
	8950 5600 8950 5750
Wire Wire Line
	5600 4000 5850 4000
Text Label 5850 4000 0    50   ~ 0
VA
Wire Wire Line
	9150 5500 8950 5500
Text Label 8950 5500 0    50   ~ 0
VA
$Comp
L Device:CP1_Small C5
U 1 1 5E6A24D1
P 8600 5500
F 0 "C5" H 8691 5546 50  0000 L CNN
F 1 "100uf" H 8691 5455 50  0000 L CNN
F 2 "Capacitor_SMD:CP_Elec_6.3x9.9" H 8600 5500 50  0001 C CNN
F 3 "~" H 8600 5500 50  0001 C CNN
	1    8600 5500
	1    0    0    -1  
$EndComp
Connection ~ 8950 5400
Wire Wire Line
	8600 5600 8950 5600
Connection ~ 8950 5600
$Comp
L Connector:Conn_01x04_Female J3
U 1 1 5E6A6216
P 1900 5600
F 0 "J3" H 1850 5300 50  0000 C CNN
F 1 "BME280" H 1750 5800 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Vertical" H 1900 5600 50  0001 C CNN
F 3 "~" H 1900 5600 50  0001 C CNN
	1    1900 5600
	-1   0    0    1   
$EndComp
$Comp
L power:+5V #PWR0116
U 1 1 5E6A764D
P 2500 5300
F 0 "#PWR0116" H 2500 5150 50  0001 C CNN
F 1 "+5V" H 2515 5473 50  0000 C CNN
F 2 "" H 2500 5300 50  0001 C CNN
F 3 "" H 2500 5300 50  0001 C CNN
	1    2500 5300
	1    0    0    -1  
$EndComp
Wire Wire Line
	2100 5700 2500 5700
$Comp
L power:GND #PWR0117
U 1 1 5E6A9C28
P 2800 5600
F 0 "#PWR0117" H 2800 5350 50  0001 C CNN
F 1 "GND" H 2805 5427 50  0000 C CNN
F 2 "" H 2800 5600 50  0001 C CNN
F 3 "" H 2800 5600 50  0001 C CNN
	1    2800 5600
	1    0    0    -1  
$EndComp
Wire Wire Line
	2500 5300 2500 5700
Wire Wire Line
	2100 5600 2800 5600
Wire Wire Line
	2100 5500 3150 5500
Text Label 3150 5500 0    50   ~ 0
SCL
Wire Wire Line
	2100 5400 3150 5400
Text Label 3150 5400 0    50   ~ 0
SDA
$Comp
L Device:R_Small_US R4
U 1 1 5E6BEA53
P 8200 5400
F 0 "R4" V 8150 5500 50  0000 C CNN
F 1 "220" V 8300 5400 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric" H 8200 5400 50  0001 C CNN
F 3 "~" H 8200 5400 50  0001 C CNN
	1    8200 5400
	0    1    1    0   
$EndComp
Wire Wire Line
	8300 5400 8600 5400
Connection ~ 8600 5400
Wire Wire Line
	8600 5400 8950 5400
$Comp
L Device:LED D2
U 1 1 5E6C145E
P 7800 5400
F 0 "D2" H 7793 5616 50  0000 C CNN
F 1 "LED" H 7793 5525 50  0000 C CNN
F 2 "LED_THT:LED_D3.0mm" H 7800 5400 50  0001 C CNN
F 3 "~" H 7800 5400 50  0001 C CNN
	1    7800 5400
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0118
U 1 1 5E6C1BF3
P 7550 5750
F 0 "#PWR0118" H 7550 5500 50  0001 C CNN
F 1 "GND" H 7555 5577 50  0000 C CNN
F 2 "" H 7550 5750 50  0001 C CNN
F 3 "" H 7550 5750 50  0001 C CNN
	1    7550 5750
	1    0    0    -1  
$EndComp
Wire Wire Line
	8100 5400 7950 5400
Wire Wire Line
	7650 5400 7550 5400
Wire Wire Line
	7550 5400 7550 5750
$Comp
L power:+5V #PWR0119
U 1 1 5E6C8077
P 3200 3600
F 0 "#PWR0119" H 3200 3450 50  0001 C CNN
F 1 "+5V" H 3215 3773 50  0000 C CNN
F 2 "" H 3200 3600 50  0001 C CNN
F 3 "" H 3200 3600 50  0001 C CNN
	1    3200 3600
	1    0    0    -1  
$EndComp
Wire Wire Line
	2100 3600 3200 3600
Wire Wire Line
	2600 3150 3050 3150
Text Label 2800 3500 0    50   ~ 0
RX
Text Label 2800 3400 0    50   ~ 0
TX
Wire Wire Line
	2100 4700 2350 4700
Text Label 2350 4700 0    50   ~ 0
SDA
$EndSCHEMATC
