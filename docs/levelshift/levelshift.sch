EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A 11000 8500
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
L power:+3.3V #PWR08
U 1 1 5F7E764C
P 7300 4350
F 0 "#PWR08" H 7300 4200 50  0001 C CNN
F 1 "+3.3V" V 7315 4478 50  0000 L CNN
F 2 "" H 7300 4350 50  0001 C CNN
F 3 "" H 7300 4350 50  0001 C CNN
	1    7300 4350
	0    -1   -1   0   
$EndComp
$Comp
L power:+5V #PWR07
U 1 1 5F7E79AC
P 7300 3500
F 0 "#PWR07" H 7300 3350 50  0001 C CNN
F 1 "+5V" V 7315 3628 50  0000 L CNN
F 2 "" H 7300 3500 50  0001 C CNN
F 3 "" H 7300 3500 50  0001 C CNN
	1    7300 3500
	0    -1   -1   0   
$EndComp
$Comp
L Transistor_FET:2N7000 Q4
U 1 1 5F7EC15C
P 7750 3950
F 0 "Q4" H 7954 3996 50  0000 L CNN
F 1 "2N7000" H 7954 3905 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-92_Inline" H 7950 3875 50  0001 L CIN
F 3 "https://www.fairchildsemi.com/datasheets/2N/2N7000.pdf" H 7750 3950 50  0001 L CNN
	1    7750 3950
	1    0    0    -1  
$EndComp
$Comp
L Device:R R8
U 1 1 5F7EEBBC
P 7550 4350
F 0 "R8" V 7757 4350 50  0000 C CNN
F 1 "10k" V 7666 4350 50  0000 C CNN
F 2 "" V 7480 4350 50  0001 C CNN
F 3 "~" H 7550 4350 50  0001 C CNN
	1    7550 4350
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R9
U 1 1 5F7EFE60
P 7850 4550
F 0 "R9" H 7920 4596 50  0000 L CNN
F 1 "330" H 7920 4505 50  0000 L CNN
F 2 "" V 7780 4550 50  0001 C CNN
F 3 "~" H 7850 4550 50  0001 C CNN
	1    7850 4550
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x01 J7
U 1 1 5F82D48F
P 7850 3050
F 0 "J7" V 7814 2962 50  0000 R CNN
F 1 "PIC_Data" V 7723 2962 50  0000 R CNN
F 2 "" H 7850 3050 50  0001 C CNN
F 3 "~" H 7850 3050 50  0001 C CNN
	1    7850 3050
	0    -1   -1   0   
$EndComp
$Comp
L Connector_Generic:Conn_01x01 J8
U 1 1 5F8379D9
P 7850 5000
F 0 "J8" V 7722 4912 50  0000 R CNN
F 1 "GPIO_Data" V 7813 4912 50  0000 R CNN
F 2 "" H 7850 5000 50  0001 C CNN
F 3 "~" H 7850 5000 50  0001 C CNN
	1    7850 5000
	0    -1   1    0   
$EndComp
$Comp
L Device:R R7
U 1 1 5F7EF528
P 7550 3500
F 0 "R7" V 7757 3500 50  0000 C CNN
F 1 "10k" V 7666 3500 50  0000 C CNN
F 2 "" V 7480 3500 50  0001 C CNN
F 3 "~" H 7550 3500 50  0001 C CNN
	1    7550 3500
	0    -1   -1   0   
$EndComp
Wire Wire Line
	7850 3250 7850 3500
Wire Wire Line
	7700 3500 7850 3500
Connection ~ 7850 3500
Wire Wire Line
	7850 3500 7850 3750
Wire Wire Line
	7300 3500 7400 3500
Wire Wire Line
	7300 4350 7350 4350
Wire Wire Line
	7850 4150 7850 4350
Wire Wire Line
	7700 4350 7850 4350
Connection ~ 7850 4350
Wire Wire Line
	7850 4350 7850 4400
Wire Wire Line
	7850 4800 7850 4700
Wire Wire Line
	7550 3950 7350 3950
Wire Wire Line
	7350 3950 7350 4350
Connection ~ 7350 4350
Wire Wire Line
	7350 4350 7400 4350
Text Notes 4450 5300 0    50   ~ 0
Unidirectional (inverted logic)
Wire Wire Line
	3500 3500 3600 3500
Wire Wire Line
	4000 3500 4000 3750
Connection ~ 4000 3500
Wire Wire Line
	3900 3500 4000 3500
Wire Wire Line
	4600 3500 4700 3500
Wire Wire Line
	5100 3500 5100 3750
Connection ~ 5100 3500
Wire Wire Line
	5000 3500 5100 3500
Wire Wire Line
	5850 3500 5950 3500
Wire Wire Line
	6350 3500 6350 3750
Connection ~ 6350 3500
Wire Wire Line
	6250 3500 6350 3500
Wire Wire Line
	6350 3250 6350 3500
Wire Wire Line
	5100 3250 5100 3500
Wire Wire Line
	4000 3250 4000 3500
$Comp
L power:+5V #PWR01
U 1 1 5F8D48EC
P 3500 3500
F 0 "#PWR01" H 3500 3350 50  0001 C CNN
F 1 "+5V" V 3515 3628 50  0000 L CNN
F 2 "" H 3500 3500 50  0001 C CNN
F 3 "" H 3500 3500 50  0001 C CNN
	1    3500 3500
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4000 4550 4000 4650
Wire Wire Line
	4000 4150 4000 4250
Wire Wire Line
	3700 3950 3700 4800
$Comp
L Connector_Generic:Conn_01x01 J1
U 1 1 5F8D48E3
P 3700 5000
F 0 "J1" V 3572 5080 50  0000 L CNN
F 1 "GPIO_PGM" V 3663 5080 50  0000 L CNN
F 2 "" H 3700 5000 50  0001 C CNN
F 3 "~" H 3700 5000 50  0001 C CNN
	1    3700 5000
	0    1    1    0   
$EndComp
$Comp
L Connector_Generic:Conn_01x01 J2
U 1 1 5F8D48DD
P 4000 3050
F 0 "J2" V 3964 2962 50  0000 R CNN
F 1 "PIC_PGM" V 3873 2962 50  0000 R CNN
F 2 "" H 4000 3050 50  0001 C CNN
F 3 "~" H 4000 3050 50  0001 C CNN
	1    4000 3050
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR02
U 1 1 5F8D48D7
P 4000 4650
F 0 "#PWR02" H 4000 4400 50  0001 C CNN
F 1 "GND" H 4005 4477 50  0000 C CNN
F 2 "" H 4000 4650 50  0001 C CNN
F 3 "" H 4000 4650 50  0001 C CNN
	1    4000 4650
	1    0    0    -1  
$EndComp
$Comp
L Device:R R2
U 1 1 5F8D48D1
P 4000 4400
F 0 "R2" H 4070 4446 50  0000 L CNN
F 1 "330" H 4070 4355 50  0000 L CNN
F 2 "" V 3930 4400 50  0001 C CNN
F 3 "~" H 4000 4400 50  0001 C CNN
	1    4000 4400
	1    0    0    -1  
$EndComp
$Comp
L Device:R R1
U 1 1 5F8D48CB
P 3750 3500
F 0 "R1" V 3957 3500 50  0000 C CNN
F 1 "10k" V 3866 3500 50  0000 C CNN
F 2 "" V 3680 3500 50  0001 C CNN
F 3 "~" H 3750 3500 50  0001 C CNN
	1    3750 3500
	0    -1   -1   0   
$EndComp
$Comp
L Transistor_FET:2N7000 Q1
U 1 1 5F8D48C5
P 3900 3950
F 0 "Q1" H 4104 3996 50  0000 L CNN
F 1 "2N7000" H 4104 3905 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-92_Inline" H 4100 3875 50  0001 L CIN
F 3 "https://www.fairchildsemi.com/datasheets/2N/2N7000.pdf" H 3900 3950 50  0001 L CNN
	1    3900 3950
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR03
U 1 1 5F8D19A8
P 4600 3500
F 0 "#PWR03" H 4600 3350 50  0001 C CNN
F 1 "+5V" V 4615 3628 50  0000 L CNN
F 2 "" H 4600 3500 50  0001 C CNN
F 3 "" H 4600 3500 50  0001 C CNN
	1    4600 3500
	0    -1   -1   0   
$EndComp
Wire Wire Line
	5100 4550 5100 4650
Wire Wire Line
	5100 4150 5100 4250
Wire Wire Line
	4800 3950 4800 4800
$Comp
L Connector_Generic:Conn_01x01 J3
U 1 1 5F8D199F
P 4800 5000
F 0 "J3" V 4672 5080 50  0000 L CNN
F 1 "GPIO_Clock" V 4763 5080 50  0000 L CNN
F 2 "" H 4800 5000 50  0001 C CNN
F 3 "~" H 4800 5000 50  0001 C CNN
	1    4800 5000
	0    1    1    0   
$EndComp
$Comp
L Connector_Generic:Conn_01x01 J4
U 1 1 5F8D1999
P 5100 3050
F 0 "J4" V 5064 2962 50  0000 R CNN
F 1 "PIC_Clock" V 4973 2962 50  0000 R CNN
F 2 "" H 5100 3050 50  0001 C CNN
F 3 "~" H 5100 3050 50  0001 C CNN
	1    5100 3050
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR04
U 1 1 5F8D1993
P 5100 4650
F 0 "#PWR04" H 5100 4400 50  0001 C CNN
F 1 "GND" H 5105 4477 50  0000 C CNN
F 2 "" H 5100 4650 50  0001 C CNN
F 3 "" H 5100 4650 50  0001 C CNN
	1    5100 4650
	1    0    0    -1  
$EndComp
$Comp
L Device:R R4
U 1 1 5F8D198D
P 5100 4400
F 0 "R4" H 5170 4446 50  0000 L CNN
F 1 "330" H 5170 4355 50  0000 L CNN
F 2 "" V 5030 4400 50  0001 C CNN
F 3 "~" H 5100 4400 50  0001 C CNN
	1    5100 4400
	1    0    0    -1  
$EndComp
$Comp
L Device:R R3
U 1 1 5F8D1987
P 4850 3500
F 0 "R3" V 5057 3500 50  0000 C CNN
F 1 "10k" V 4966 3500 50  0000 C CNN
F 2 "" V 4780 3500 50  0001 C CNN
F 3 "~" H 4850 3500 50  0001 C CNN
	1    4850 3500
	0    -1   -1   0   
$EndComp
$Comp
L Transistor_FET:2N7000 Q2
U 1 1 5F8D1981
P 5000 3950
F 0 "Q2" H 5204 3996 50  0000 L CNN
F 1 "2N7000" H 5204 3905 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-92_Inline" H 5200 3875 50  0001 L CIN
F 3 "https://www.fairchildsemi.com/datasheets/2N/2N7000.pdf" H 5000 3950 50  0001 L CNN
	1    5000 3950
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR05
U 1 1 5F8B3576
P 5850 3500
F 0 "#PWR05" H 5850 3350 50  0001 C CNN
F 1 "+5V" V 5865 3628 50  0000 L CNN
F 2 "" H 5850 3500 50  0001 C CNN
F 3 "" H 5850 3500 50  0001 C CNN
	1    5850 3500
	0    -1   -1   0   
$EndComp
Wire Wire Line
	6350 4550 6350 4650
Wire Wire Line
	6350 4150 6350 4250
Wire Wire Line
	6050 3950 6050 4800
$Comp
L Connector_Generic:Conn_01x01 J5
U 1 1 5F88C2DB
P 6050 5000
F 0 "J5" V 5922 5080 50  0000 L CNN
F 1 "GPIO_MCLR" V 6013 5080 50  0000 L CNN
F 2 "" H 6050 5000 50  0001 C CNN
F 3 "~" H 6050 5000 50  0001 C CNN
	1    6050 5000
	0    1    1    0   
$EndComp
$Comp
L Connector_Generic:Conn_01x01 J6
U 1 1 5F82CE45
P 6350 3050
F 0 "J6" V 6314 2962 50  0000 R CNN
F 1 "PIC_MCLR" V 6223 2962 50  0000 R CNN
F 2 "" H 6350 3050 50  0001 C CNN
F 3 "~" H 6350 3050 50  0001 C CNN
	1    6350 3050
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR06
U 1 1 5F7F5A56
P 6350 4650
F 0 "#PWR06" H 6350 4400 50  0001 C CNN
F 1 "GND" H 6355 4477 50  0000 C CNN
F 2 "" H 6350 4650 50  0001 C CNN
F 3 "" H 6350 4650 50  0001 C CNN
	1    6350 4650
	1    0    0    -1  
$EndComp
$Comp
L Device:R R6
U 1 1 5F7F0F7C
P 6350 4400
F 0 "R6" H 6420 4446 50  0000 L CNN
F 1 "330" H 6420 4355 50  0000 L CNN
F 2 "" V 6280 4400 50  0001 C CNN
F 3 "~" H 6350 4400 50  0001 C CNN
	1    6350 4400
	1    0    0    -1  
$EndComp
$Comp
L Device:R R5
U 1 1 5F7EF072
P 6100 3500
F 0 "R5" V 6307 3500 50  0000 C CNN
F 1 "10k" V 6216 3500 50  0000 C CNN
F 2 "" V 6030 3500 50  0001 C CNN
F 3 "~" H 6100 3500 50  0001 C CNN
	1    6100 3500
	0    -1   -1   0   
$EndComp
$Comp
L Transistor_FET:2N7000 Q3
U 1 1 5F7EABC1
P 6250 3950
F 0 "Q3" H 6454 3996 50  0000 L CNN
F 1 "2N7000" H 6454 3905 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-92_Inline" H 6450 3875 50  0001 L CIN
F 3 "https://www.fairchildsemi.com/datasheets/2N/2N7000.pdf" H 6250 3950 50  0001 L CNN
	1    6250 3950
	1    0    0    -1  
$EndComp
Wire Notes Line
	3100 2900 3100 5350
Wire Notes Line
	3100 5350 6850 5350
Wire Notes Line
	6850 5350 6850 2900
Wire Notes Line
	6850 2900 3100 2900
Text Notes 7400 5300 0    50   ~ 0
Bidirectional
Wire Notes Line
	6900 2900 6900 5350
Wire Notes Line
	6900 5350 8400 5350
Wire Notes Line
	8400 2900 6900 2900
Wire Notes Line
	8400 5350 8400 2900
$EndSCHEMATC
