EESchema Schematic File Version 4
LIBS:Basic-Connector-Board-cache
EELAYER 26 0
EELAYER END
$Descr User 9843 5512
encoding utf-8
Sheet 1 1
Title "ViamSonus-2.0 Example Connector Board"
Date "2019-11-23"
Rev "1"
Comp "Manuvr"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text Label 5700 900  0    39   ~ 0
Out-L-0
Text Label 5700 2000 0    39   ~ 0
Out-L-1
Text Label 7250 900  0    39   ~ 0
Out-L-2
Text Label 7250 2000 0    39   ~ 0
Out-L-3
Text Label 5700 1100 0    39   ~ 0
Out-R-0
Text Label 5700 2200 0    39   ~ 0
Out-R-1
Text Label 7250 1100 0    39   ~ 0
Out-R-2
Text Label 7250 2200 0    39   ~ 0
Out-R-3
Text Label 8800 1150 0    39   ~ 0
~RESET
Text Label 8800 1050 0    39   ~ 0
SCL
Text Label 8800 950  0    39   ~ 0
SDA
Text Label 8000 950  0    39   ~ 0
Vcc
Text Label 8000 1050 0    39   ~ 0
GND
Text Label 8800 1450 0    39   ~ 0
ANAGND
Text Label 8800 1350 0    39   ~ 0
ANAVcc
Text Label 1850 3600 0    39   ~ 0
Biased-In-R-2
Text Label 3700 1300 0    39   ~ 0
Biased-In-R-3
Text Label 3700 2450 0    39   ~ 0
Biased-In-R-4
Text Label 3700 3600 0    39   ~ 0
Biased-In-R-5
Text Label 8800 1250 0    39   ~ 0
VccDiv2
$Comp
L Connector:Conn_01x14_Male J11
U 1 1 5F7D73BF
P 7800 1550
F 0 "J11" H 7900 2300 50  0000 C CNN
F 1 "Conn_01x14_Male" V 7750 1550 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x14_P2.54mm_Vertical" H 7800 1550 50  0001 C CNN
F 3 "~" H 7800 1550 50  0001 C CNN
	1    7800 1550
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x14_Male J12
U 1 1 5F7D7655
P 8600 1550
F 0 "J12" H 8700 2300 50  0000 C CNN
F 1 "Conn_01x14_Male" V 8550 1550 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x14_P2.54mm_Vertical" H 8600 1550 50  0001 C CNN
F 3 "~" H 8600 1550 50  0001 C CNN
	1    8600 1550
	1    0    0    -1  
$EndComp
Text Label 8000 1150 0    39   ~ 0
Biased-In-L-0
Text Label 8000 1350 0    39   ~ 0
Biased-In-L-1
Text Label 8000 1550 0    39   ~ 0
Biased-In-L-2
Text Label 8000 1750 0    39   ~ 0
Biased-In-L-3
Text Label 8000 1950 0    39   ~ 0
Biased-In-L-4
Text Label 8000 2150 0    39   ~ 0
Biased-In-L-5
Text Label 8000 1250 0    39   ~ 0
Biased-In-R-0
Text Label 8000 1450 0    39   ~ 0
Biased-In-R-1
Text Label 8000 1650 0    39   ~ 0
Biased-In-R-2
Text Label 8000 1850 0    39   ~ 0
Biased-In-R-3
Text Label 8000 2050 0    39   ~ 0
Biased-In-R-4
Text Label 8000 2250 0    39   ~ 0
Biased-In-R-5
$Comp
L Connector:AudioJack3_Ground_Switch J5
U 1 1 5DFB666B
P 900 3600
F 0 "J5" H 850 3850 50  0000 C CNN
F 1 "AudioJack3_Ground_Switch" V 650 3600 50  0000 C CNN
F 2 "" H 900 3600 50  0001 C CNN
F 3 "~" H 900 3600 50  0001 C CNN
	1    900  3600
	1    0    0    1   
$EndComp
Text Label 8800 1550 0    39   ~ 0
Out-L-0
Text Label 8800 1750 0    39   ~ 0
Out-L-1
Text Label 8800 1950 0    39   ~ 0
Out-L-2
Text Label 8800 2150 0    39   ~ 0
Out-L-3
Text Label 8800 1650 0    39   ~ 0
Out-R-0
Text Label 8800 1850 0    39   ~ 0
Out-R-1
Text Label 8800 2050 0    39   ~ 0
Out-R-2
Text Label 8800 2250 0    39   ~ 0
Out-R-3
$Comp
L Device:R R6
U 1 1 5E13A1F1
P 1650 3800
F 0 "R6" V 1700 3900 50  0000 L CNN
F 1 "20k" V 1650 3750 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 1580 3800 50  0001 C CNN
F 3 "~" H 1650 3800 50  0001 C CNN
F 4 "RNCP0603FTD20K0CT-ND" V 1650 3800 50  0001 C CNN "Digikey_Number"
F 5 "RNCP0603FTD20K0" V 1650 3800 50  0001 C CNN "Manu_Number"
F 6 "Stackpole Electronics Inc" V 1650 3800 50  0001 C CNN "Manu_Name"
	1    1650 3800
	-1   0    0    1   
$EndComp
Text Label 1500 4000 2    39   ~ 0
ANAGND
Text Label 1600 3000 2    39   ~ 0
ANAVcc
$Comp
L Device:R R5
U 1 1 5E13A214
P 1650 3200
F 0 "R5" V 1700 3300 50  0000 L CNN
F 1 "20k" V 1650 3150 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 1580 3200 50  0001 C CNN
F 3 "~" H 1650 3200 50  0001 C CNN
F 4 "RNCP0603FTD20K0CT-ND" V 1650 3200 50  0001 C CNN "Digikey_Number"
F 5 "RNCP0603FTD20K0" V 1650 3200 50  0001 C CNN "Manu_Number"
F 6 "Stackpole Electronics Inc" V 1650 3200 50  0001 C CNN "Manu_Name"
	1    1650 3200
	1    0    0    -1  
$EndComp
$Comp
L Device:R R12
U 1 1 5E13A21D
P 1750 3800
F 0 "R12" V 1800 3900 50  0000 L CNN
F 1 "20k" V 1750 3750 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 1680 3800 50  0001 C CNN
F 3 "~" H 1750 3800 50  0001 C CNN
F 4 "RNCP0603FTD20K0CT-ND" V 1750 3800 50  0001 C CNN "Digikey_Number"
F 5 "RNCP0603FTD20K0" V 1750 3800 50  0001 C CNN "Manu_Number"
F 6 "Stackpole Electronics Inc" V 1750 3800 50  0001 C CNN "Manu_Name"
	1    1750 3800
	-1   0    0    1   
$EndComp
$Comp
L Device:R R11
U 1 1 5E13A226
P 1750 3200
F 0 "R11" V 1800 3300 50  0000 L CNN
F 1 "20k" V 1750 3150 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 1680 3200 50  0001 C CNN
F 3 "~" H 1750 3200 50  0001 C CNN
F 4 "RNCP0603FTD20K0CT-ND" V 1750 3200 50  0001 C CNN "Digikey_Number"
F 5 "RNCP0603FTD20K0" V 1750 3200 50  0001 C CNN "Manu_Number"
F 6 "Stackpole Electronics Inc" V 1750 3200 50  0001 C CNN "Manu_Name"
	1    1750 3200
	1    0    0    -1  
$EndComp
Text Label 3700 3400 0    39   ~ 0
Biased-In-L-5
Text Label 3700 2250 0    39   ~ 0
Biased-In-L-4
Text Label 3700 1100 0    39   ~ 0
Biased-In-L-3
Text Label 1850 3400 0    39   ~ 0
Biased-In-L-2
Text Label 1850 2250 0    39   ~ 0
Biased-In-L-1
Text Label 1850 1100 0    39   ~ 0
Biased-In-L-0
Text Label 1850 2450 0    39   ~ 0
Biased-In-R-1
Text Label 1850 1300 0    39   ~ 0
Biased-In-R-0
Wire Wire Line
	1100 3600 1300 3600
Wire Wire Line
	1100 3400 1300 3400
Wire Wire Line
	1600 3600 1750 3600
Wire Wire Line
	1600 3400 1650 3400
Wire Wire Line
	1650 3350 1650 3400
Connection ~ 1650 3400
Wire Wire Line
	1650 3400 1850 3400
Wire Wire Line
	1650 3400 1650 3650
Wire Wire Line
	1750 3650 1750 3600
Connection ~ 1750 3600
Wire Wire Line
	1750 3600 1850 3600
Wire Wire Line
	1750 3350 1750 3600
Wire Wire Line
	1600 3000 1650 3000
Wire Wire Line
	1750 3000 1750 3050
Wire Wire Line
	1650 3050 1650 3000
Connection ~ 1650 3000
Wire Wire Line
	1650 3000 1750 3000
Wire Wire Line
	1750 3950 1750 4000
Wire Wire Line
	1750 4000 1650 4000
Wire Wire Line
	1150 4000 1150 3700
Wire Wire Line
	1150 3700 1100 3700
$Comp
L Connector:AudioJack3_Ground_Switch J4
U 1 1 5E2010F1
P 900 2450
F 0 "J4" H 850 2700 50  0000 C CNN
F 1 "AudioJack3_Ground_Switch" V 650 2450 50  0000 C CNN
F 2 "" H 900 2450 50  0001 C CNN
F 3 "~" H 900 2450 50  0001 C CNN
	1    900  2450
	1    0    0    1   
$EndComp
$Comp
L Device:R R4
U 1 1 5E201105
P 1650 2650
F 0 "R4" V 1700 2750 50  0000 L CNN
F 1 "20k" V 1650 2600 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 1580 2650 50  0001 C CNN
F 3 "~" H 1650 2650 50  0001 C CNN
F 4 "RNCP0603FTD20K0CT-ND" V 1650 2650 50  0001 C CNN "Digikey_Number"
F 5 "RNCP0603FTD20K0" V 1650 2650 50  0001 C CNN "Manu_Number"
F 6 "Stackpole Electronics Inc" V 1650 2650 50  0001 C CNN "Manu_Name"
	1    1650 2650
	-1   0    0    1   
$EndComp
Text Label 1500 2850 2    39   ~ 0
ANAGND
Text Label 1600 1850 2    39   ~ 0
ANAVcc
$Comp
L Device:R R3
U 1 1 5E20111B
P 1650 2050
F 0 "R3" V 1700 2150 50  0000 L CNN
F 1 "20k" V 1650 2000 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 1580 2050 50  0001 C CNN
F 3 "~" H 1650 2050 50  0001 C CNN
F 4 "RNCP0603FTD20K0CT-ND" V 1650 2050 50  0001 C CNN "Digikey_Number"
F 5 "RNCP0603FTD20K0" V 1650 2050 50  0001 C CNN "Manu_Number"
F 6 "Stackpole Electronics Inc" V 1650 2050 50  0001 C CNN "Manu_Name"
	1    1650 2050
	1    0    0    -1  
$EndComp
$Comp
L Device:R R10
U 1 1 5E201125
P 1750 2650
F 0 "R10" V 1800 2750 50  0000 L CNN
F 1 "20k" V 1750 2600 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 1680 2650 50  0001 C CNN
F 3 "~" H 1750 2650 50  0001 C CNN
F 4 "RNCP0603FTD20K0CT-ND" V 1750 2650 50  0001 C CNN "Digikey_Number"
F 5 "RNCP0603FTD20K0" V 1750 2650 50  0001 C CNN "Manu_Number"
F 6 "Stackpole Electronics Inc" V 1750 2650 50  0001 C CNN "Manu_Name"
	1    1750 2650
	-1   0    0    1   
$EndComp
$Comp
L Device:R R9
U 1 1 5E20112F
P 1750 2050
F 0 "R9" V 1800 2150 50  0000 L CNN
F 1 "20k" V 1750 2000 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 1680 2050 50  0001 C CNN
F 3 "~" H 1750 2050 50  0001 C CNN
F 4 "RNCP0603FTD20K0CT-ND" V 1750 2050 50  0001 C CNN "Digikey_Number"
F 5 "RNCP0603FTD20K0" V 1750 2050 50  0001 C CNN "Manu_Number"
F 6 "Stackpole Electronics Inc" V 1750 2050 50  0001 C CNN "Manu_Name"
	1    1750 2050
	1    0    0    -1  
$EndComp
Wire Wire Line
	1100 2450 1300 2450
Wire Wire Line
	1100 2250 1300 2250
Wire Wire Line
	1600 2450 1750 2450
Wire Wire Line
	1600 2250 1650 2250
Wire Wire Line
	1650 2200 1650 2250
Connection ~ 1650 2250
Wire Wire Line
	1650 2250 1850 2250
Wire Wire Line
	1650 2250 1650 2500
Wire Wire Line
	1750 2500 1750 2450
Connection ~ 1750 2450
Wire Wire Line
	1750 2450 1850 2450
Wire Wire Line
	1750 2200 1750 2450
Wire Wire Line
	1600 1850 1650 1850
Wire Wire Line
	1750 1850 1750 1900
Wire Wire Line
	1650 1900 1650 1850
Connection ~ 1650 1850
Wire Wire Line
	1650 1850 1750 1850
Wire Wire Line
	1750 2800 1750 2850
Wire Wire Line
	1750 2850 1650 2850
Wire Wire Line
	1150 2850 1150 2550
Wire Wire Line
	1150 2550 1100 2550
$Comp
L Connector:AudioJack3_Ground_Switch J3
U 1 1 5E203AE3
P 900 1300
F 0 "J3" H 850 1550 50  0000 C CNN
F 1 "AudioJack3_Ground_Switch" V 650 1300 50  0000 C CNN
F 2 "" H 900 1300 50  0001 C CNN
F 3 "~" H 900 1300 50  0001 C CNN
	1    900  1300
	1    0    0    1   
$EndComp
$Comp
L Device:R R2
U 1 1 5E203AF7
P 1650 1500
F 0 "R2" V 1700 1600 50  0000 L CNN
F 1 "20k" V 1650 1450 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 1580 1500 50  0001 C CNN
F 3 "~" H 1650 1500 50  0001 C CNN
F 4 "RNCP0603FTD20K0CT-ND" V 1650 1500 50  0001 C CNN "Digikey_Number"
F 5 "RNCP0603FTD20K0" V 1650 1500 50  0001 C CNN "Manu_Number"
F 6 "Stackpole Electronics Inc" V 1650 1500 50  0001 C CNN "Manu_Name"
	1    1650 1500
	-1   0    0    1   
$EndComp
Text Label 1500 1700 2    39   ~ 0
ANAGND
Text Label 1600 700  2    39   ~ 0
ANAVcc
$Comp
L Device:R R1
U 1 1 5E203B0D
P 1650 900
F 0 "R1" V 1700 1000 50  0000 L CNN
F 1 "20k" V 1650 850 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 1580 900 50  0001 C CNN
F 3 "~" H 1650 900 50  0001 C CNN
F 4 "RNCP0603FTD20K0CT-ND" V 1650 900 50  0001 C CNN "Digikey_Number"
F 5 "RNCP0603FTD20K0" V 1650 900 50  0001 C CNN "Manu_Number"
F 6 "Stackpole Electronics Inc" V 1650 900 50  0001 C CNN "Manu_Name"
	1    1650 900 
	1    0    0    -1  
$EndComp
$Comp
L Device:R R8
U 1 1 5E203B17
P 1750 1500
F 0 "R8" V 1800 1600 50  0000 L CNN
F 1 "20k" V 1750 1450 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 1680 1500 50  0001 C CNN
F 3 "~" H 1750 1500 50  0001 C CNN
F 4 "RNCP0603FTD20K0CT-ND" V 1750 1500 50  0001 C CNN "Digikey_Number"
F 5 "RNCP0603FTD20K0" V 1750 1500 50  0001 C CNN "Manu_Number"
F 6 "Stackpole Electronics Inc" V 1750 1500 50  0001 C CNN "Manu_Name"
	1    1750 1500
	-1   0    0    1   
$EndComp
$Comp
L Device:R R7
U 1 1 5E203B21
P 1750 900
F 0 "R7" V 1800 1000 50  0000 L CNN
F 1 "20k" V 1750 850 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 1680 900 50  0001 C CNN
F 3 "~" H 1750 900 50  0001 C CNN
F 4 "RNCP0603FTD20K0CT-ND" V 1750 900 50  0001 C CNN "Digikey_Number"
F 5 "RNCP0603FTD20K0" V 1750 900 50  0001 C CNN "Manu_Number"
F 6 "Stackpole Electronics Inc" V 1750 900 50  0001 C CNN "Manu_Name"
	1    1750 900 
	1    0    0    -1  
$EndComp
Wire Wire Line
	1100 1300 1300 1300
Wire Wire Line
	1100 1100 1300 1100
Wire Wire Line
	1600 1300 1750 1300
Wire Wire Line
	1600 1100 1650 1100
Wire Wire Line
	1650 1050 1650 1100
Connection ~ 1650 1100
Wire Wire Line
	1650 1100 1850 1100
Wire Wire Line
	1650 1100 1650 1350
Wire Wire Line
	1750 1350 1750 1300
Connection ~ 1750 1300
Wire Wire Line
	1750 1300 1850 1300
Wire Wire Line
	1750 1050 1750 1300
Wire Wire Line
	1600 700  1650 700 
Wire Wire Line
	1750 700  1750 750 
Wire Wire Line
	1650 750  1650 700 
Connection ~ 1650 700 
Wire Wire Line
	1650 700  1750 700 
Wire Wire Line
	1750 1650 1750 1700
Wire Wire Line
	1750 1700 1650 1700
Wire Wire Line
	1150 1700 1150 1400
Wire Wire Line
	1150 1400 1100 1400
$Comp
L Connector:AudioJack3_Ground_Switch J10
U 1 1 5E2075CF
P 2750 3600
F 0 "J10" H 2700 3850 50  0000 C CNN
F 1 "AudioJack3_Ground_Switch" V 2500 3600 50  0000 C CNN
F 2 "" H 2750 3600 50  0001 C CNN
F 3 "~" H 2750 3600 50  0001 C CNN
	1    2750 3600
	1    0    0    1   
$EndComp
$Comp
L Device:CP1 C20
U 1 1 5E2075D9
P 3300 3600
F 0 "C20" V 3350 3500 39  0000 C CNN
F 1 "3.3uF" V 3350 3750 39  0000 C CNN
F 2 "Capacitor_SMD:CP_Elec_4x5.4" H 3300 3600 50  0001 C CNN
F 3 "~" H 3300 3600 50  0001 C CNN
F 4 "493-17493-1-ND" V 3300 3600 50  0001 C CNN "Digikey_Number"
F 5 "UUQ1H3R3MCL1GB" V 3300 3600 50  0001 C CNN "Manu_Number"
F 6 "Nichicon" V 3300 3600 50  0001 C CNN "Manu_Name"
	1    3300 3600
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R18
U 1 1 5E2075E3
P 3500 3800
F 0 "R18" V 3550 3900 50  0000 L CNN
F 1 "20k" V 3500 3750 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 3430 3800 50  0001 C CNN
F 3 "~" H 3500 3800 50  0001 C CNN
F 4 "RNCP0603FTD20K0CT-ND" V 3500 3800 50  0001 C CNN "Digikey_Number"
F 5 "RNCP0603FTD20K0" V 3500 3800 50  0001 C CNN "Manu_Number"
F 6 "Stackpole Electronics Inc" V 3500 3800 50  0001 C CNN "Manu_Name"
	1    3500 3800
	-1   0    0    1   
$EndComp
Text Label 3350 4000 2    39   ~ 0
ANAGND
Text Label 3450 3000 2    39   ~ 0
ANAVcc
$Comp
L Device:R R17
U 1 1 5E2075F9
P 3500 3200
F 0 "R17" V 3550 3300 50  0000 L CNN
F 1 "20k" V 3500 3150 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 3430 3200 50  0001 C CNN
F 3 "~" H 3500 3200 50  0001 C CNN
F 4 "RNCP0603FTD20K0CT-ND" V 3500 3200 50  0001 C CNN "Digikey_Number"
F 5 "RNCP0603FTD20K0" V 3500 3200 50  0001 C CNN "Manu_Number"
F 6 "Stackpole Electronics Inc" V 3500 3200 50  0001 C CNN "Manu_Name"
	1    3500 3200
	1    0    0    -1  
$EndComp
$Comp
L Device:R R24
U 1 1 5E207603
P 3600 3800
F 0 "R24" V 3650 3900 50  0000 L CNN
F 1 "20k" V 3600 3750 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 3530 3800 50  0001 C CNN
F 3 "~" H 3600 3800 50  0001 C CNN
F 4 "RNCP0603FTD20K0CT-ND" V 3600 3800 50  0001 C CNN "Digikey_Number"
F 5 "RNCP0603FTD20K0" V 3600 3800 50  0001 C CNN "Manu_Number"
F 6 "Stackpole Electronics Inc" V 3600 3800 50  0001 C CNN "Manu_Name"
	1    3600 3800
	-1   0    0    1   
$EndComp
$Comp
L Device:R R23
U 1 1 5E20760D
P 3600 3200
F 0 "R23" V 3650 3300 50  0000 L CNN
F 1 "20k" V 3600 3150 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 3530 3200 50  0001 C CNN
F 3 "~" H 3600 3200 50  0001 C CNN
F 4 "RNCP0603FTD20K0CT-ND" V 3600 3200 50  0001 C CNN "Digikey_Number"
F 5 "RNCP0603FTD20K0" V 3600 3200 50  0001 C CNN "Manu_Number"
F 6 "Stackpole Electronics Inc" V 3600 3200 50  0001 C CNN "Manu_Name"
	1    3600 3200
	1    0    0    -1  
$EndComp
Wire Wire Line
	2950 3600 3150 3600
Wire Wire Line
	2950 3400 3150 3400
Wire Wire Line
	3450 3600 3600 3600
Wire Wire Line
	3450 3400 3500 3400
Wire Wire Line
	3500 3350 3500 3400
Connection ~ 3500 3400
Wire Wire Line
	3500 3400 3700 3400
Wire Wire Line
	3500 3400 3500 3650
Wire Wire Line
	3600 3650 3600 3600
Connection ~ 3600 3600
Wire Wire Line
	3600 3600 3700 3600
Wire Wire Line
	3600 3350 3600 3600
Wire Wire Line
	3450 3000 3500 3000
Wire Wire Line
	3600 3000 3600 3050
Wire Wire Line
	3500 3050 3500 3000
Connection ~ 3500 3000
Wire Wire Line
	3500 3000 3600 3000
Wire Wire Line
	3600 3950 3600 4000
Wire Wire Line
	3600 4000 3500 4000
Wire Wire Line
	3000 4000 3000 3700
Wire Wire Line
	3000 3700 2950 3700
$Comp
L Connector:AudioJack3_Ground_Switch J9
U 1 1 5E20C3E5
P 2750 2450
F 0 "J9" H 2700 2700 50  0000 C CNN
F 1 "AudioJack3_Ground_Switch" V 2500 2450 50  0000 C CNN
F 2 "" H 2750 2450 50  0001 C CNN
F 3 "~" H 2750 2450 50  0001 C CNN
	1    2750 2450
	1    0    0    1   
$EndComp
$Comp
L Device:R R16
U 1 1 5E20C3F9
P 3500 2650
F 0 "R16" V 3550 2750 50  0000 L CNN
F 1 "20k" V 3500 2600 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 3430 2650 50  0001 C CNN
F 3 "~" H 3500 2650 50  0001 C CNN
F 4 "RNCP0603FTD20K0CT-ND" V 3500 2650 50  0001 C CNN "Digikey_Number"
F 5 "RNCP0603FTD20K0" V 3500 2650 50  0001 C CNN "Manu_Number"
F 6 "Stackpole Electronics Inc" V 3500 2650 50  0001 C CNN "Manu_Name"
	1    3500 2650
	-1   0    0    1   
$EndComp
Text Label 3350 2850 2    39   ~ 0
ANAGND
Text Label 3450 1850 2    39   ~ 0
ANAVcc
$Comp
L Device:R R15
U 1 1 5E20C40F
P 3500 2050
F 0 "R15" V 3550 2150 50  0000 L CNN
F 1 "20k" V 3500 2000 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 3430 2050 50  0001 C CNN
F 3 "~" H 3500 2050 50  0001 C CNN
F 4 "RNCP0603FTD20K0CT-ND" V 3500 2050 50  0001 C CNN "Digikey_Number"
F 5 "RNCP0603FTD20K0" V 3500 2050 50  0001 C CNN "Manu_Number"
F 6 "Stackpole Electronics Inc" V 3500 2050 50  0001 C CNN "Manu_Name"
	1    3500 2050
	1    0    0    -1  
$EndComp
$Comp
L Device:R R22
U 1 1 5E20C419
P 3600 2650
F 0 "R22" V 3650 2750 50  0000 L CNN
F 1 "20k" V 3600 2600 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 3530 2650 50  0001 C CNN
F 3 "~" H 3600 2650 50  0001 C CNN
F 4 "RNCP0603FTD20K0CT-ND" V 3600 2650 50  0001 C CNN "Digikey_Number"
F 5 "RNCP0603FTD20K0" V 3600 2650 50  0001 C CNN "Manu_Number"
F 6 "Stackpole Electronics Inc" V 3600 2650 50  0001 C CNN "Manu_Name"
	1    3600 2650
	-1   0    0    1   
$EndComp
$Comp
L Device:R R21
U 1 1 5E20C423
P 3600 2050
F 0 "R21" V 3650 2150 50  0000 L CNN
F 1 "20k" V 3600 2000 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 3530 2050 50  0001 C CNN
F 3 "~" H 3600 2050 50  0001 C CNN
F 4 "RNCP0603FTD20K0CT-ND" V 3600 2050 50  0001 C CNN "Digikey_Number"
F 5 "RNCP0603FTD20K0" V 3600 2050 50  0001 C CNN "Manu_Number"
F 6 "Stackpole Electronics Inc" V 3600 2050 50  0001 C CNN "Manu_Name"
	1    3600 2050
	1    0    0    -1  
$EndComp
Wire Wire Line
	2950 2450 3150 2450
Wire Wire Line
	2950 2250 3150 2250
Wire Wire Line
	3450 2450 3600 2450
Wire Wire Line
	3450 2250 3500 2250
Wire Wire Line
	3500 2200 3500 2250
Connection ~ 3500 2250
Wire Wire Line
	3500 2250 3700 2250
Wire Wire Line
	3500 2250 3500 2500
Wire Wire Line
	3600 2500 3600 2450
Connection ~ 3600 2450
Wire Wire Line
	3600 2450 3700 2450
Wire Wire Line
	3600 2200 3600 2450
Wire Wire Line
	3450 1850 3500 1850
Wire Wire Line
	3600 1850 3600 1900
Wire Wire Line
	3500 1900 3500 1850
Connection ~ 3500 1850
Wire Wire Line
	3500 1850 3600 1850
Wire Wire Line
	3600 2800 3600 2850
Wire Wire Line
	3600 2850 3500 2850
Wire Wire Line
	3000 2850 3000 2550
Wire Wire Line
	3000 2550 2950 2550
$Comp
L Connector:AudioJack3_Ground_Switch J8
U 1 1 5E212601
P 2750 1300
F 0 "J8" H 2700 1550 50  0000 C CNN
F 1 "AudioJack3_Ground_Switch" V 2500 1300 50  0000 C CNN
F 2 "" H 2750 1300 50  0001 C CNN
F 3 "~" H 2750 1300 50  0001 C CNN
	1    2750 1300
	1    0    0    1   
$EndComp
$Comp
L Device:R R14
U 1 1 5E212615
P 3500 1500
F 0 "R14" V 3550 1600 50  0000 L CNN
F 1 "20k" V 3500 1450 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 3430 1500 50  0001 C CNN
F 3 "~" H 3500 1500 50  0001 C CNN
F 4 "RNCP0603FTD20K0CT-ND" V 3500 1500 50  0001 C CNN "Digikey_Number"
F 5 "RNCP0603FTD20K0" V 3500 1500 50  0001 C CNN "Manu_Number"
F 6 "Stackpole Electronics Inc" V 3500 1500 50  0001 C CNN "Manu_Name"
	1    3500 1500
	-1   0    0    1   
$EndComp
Text Label 3350 1700 2    39   ~ 0
ANAGND
Text Label 3450 700  2    39   ~ 0
ANAVcc
$Comp
L Device:R R13
U 1 1 5E21262B
P 3500 900
F 0 "R13" V 3550 1000 50  0000 L CNN
F 1 "20k" V 3500 850 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 3430 900 50  0001 C CNN
F 3 "~" H 3500 900 50  0001 C CNN
F 4 "RNCP0603FTD20K0CT-ND" V 3500 900 50  0001 C CNN "Digikey_Number"
F 5 "RNCP0603FTD20K0" V 3500 900 50  0001 C CNN "Manu_Number"
F 6 "Stackpole Electronics Inc" V 3500 900 50  0001 C CNN "Manu_Name"
	1    3500 900 
	1    0    0    -1  
$EndComp
$Comp
L Device:R R20
U 1 1 5E212635
P 3600 1500
F 0 "R20" V 3650 1600 50  0000 L CNN
F 1 "20k" V 3600 1450 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 3530 1500 50  0001 C CNN
F 3 "~" H 3600 1500 50  0001 C CNN
F 4 "RNCP0603FTD20K0CT-ND" V 3600 1500 50  0001 C CNN "Digikey_Number"
F 5 "RNCP0603FTD20K0" V 3600 1500 50  0001 C CNN "Manu_Number"
F 6 "Stackpole Electronics Inc" V 3600 1500 50  0001 C CNN "Manu_Name"
	1    3600 1500
	-1   0    0    1   
$EndComp
$Comp
L Device:R R19
U 1 1 5E21263F
P 3600 900
F 0 "R19" V 3650 1000 50  0000 L CNN
F 1 "20k" V 3600 850 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 3530 900 50  0001 C CNN
F 3 "~" H 3600 900 50  0001 C CNN
F 4 "RNCP0603FTD20K0CT-ND" V 3600 900 50  0001 C CNN "Digikey_Number"
F 5 "RNCP0603FTD20K0" V 3600 900 50  0001 C CNN "Manu_Number"
F 6 "Stackpole Electronics Inc" V 3600 900 50  0001 C CNN "Manu_Name"
	1    3600 900 
	1    0    0    -1  
$EndComp
Wire Wire Line
	2950 1300 3150 1300
Wire Wire Line
	2950 1100 3150 1100
Wire Wire Line
	3450 1300 3600 1300
Wire Wire Line
	3450 1100 3500 1100
Wire Wire Line
	3500 1050 3500 1100
Connection ~ 3500 1100
Wire Wire Line
	3500 1100 3700 1100
Wire Wire Line
	3500 1100 3500 1350
Wire Wire Line
	3600 1350 3600 1300
Connection ~ 3600 1300
Wire Wire Line
	3600 1300 3700 1300
Wire Wire Line
	3600 1050 3600 1300
Wire Wire Line
	3450 700  3500 700 
Wire Wire Line
	3600 700  3600 750 
Wire Wire Line
	3500 750  3500 700 
Connection ~ 3500 700 
Wire Wire Line
	3500 700  3600 700 
Wire Wire Line
	3600 1650 3600 1700
Wire Wire Line
	3600 1700 3500 1700
Wire Wire Line
	3000 1700 3000 1400
Wire Wire Line
	3000 1400 2950 1400
$Comp
L Connector:AudioJack3_Ground_Switch J7
U 1 1 5E2CB01D
P 6500 2200
F 0 "J7" H 6450 2450 50  0000 C CNN
F 1 "AudioJack3_Ground_Switch" V 6250 2200 50  0000 C CNN
F 2 "" H 6500 2200 50  0001 C CNN
F 3 "~" H 6500 2200 50  0001 C CNN
	1    6500 2200
	1    0    0    1   
$EndComp
Wire Wire Line
	6700 2000 6900 2000
Wire Wire Line
	6900 2200 6700 2200
Text Label 6800 2350 0    39   ~ 0
ANAGND
Wire Wire Line
	6800 2350 6750 2350
Wire Wire Line
	6750 2350 6750 2300
Wire Wire Line
	6750 2300 6700 2300
Wire Wire Line
	7200 2000 7250 2000
Wire Wire Line
	7250 2200 7200 2200
$Comp
L Connector:AudioJack3_Ground_Switch J6
U 1 1 5E301699
P 6500 1100
F 0 "J6" H 6450 1350 50  0000 C CNN
F 1 "AudioJack3_Ground_Switch" V 6250 1100 50  0000 C CNN
F 2 "" H 6500 1100 50  0001 C CNN
F 3 "~" H 6500 1100 50  0001 C CNN
	1    6500 1100
	1    0    0    1   
$EndComp
Wire Wire Line
	6700 900  6900 900 
Wire Wire Line
	6900 1100 6700 1100
Text Label 6800 1250 0    39   ~ 0
ANAGND
Wire Wire Line
	6800 1250 6750 1250
Wire Wire Line
	6750 1250 6750 1200
Wire Wire Line
	6750 1200 6700 1200
Wire Wire Line
	7200 900  7250 900 
Wire Wire Line
	7250 1100 7200 1100
$Comp
L Device:CP1 C3
U 1 1 5E308FAF
P 5500 2000
F 0 "C3" V 5550 1900 39  0000 C CNN
F 1 "100uF" V 5550 2150 39  0000 C CNN
F 2 "Capacitor_SMD:CP_Elec_6.3x5.4_Nichicon" H 5500 2000 50  0001 C CNN
F 3 "~" H 5500 2000 50  0001 C CNN
F 4 "493-16090-1-ND" V 5500 2000 50  0001 C CNN "Digikey_Number"
F 5 "UUQ0J101MCL1GB" V 5500 2000 50  0001 C CNN "Manu_Number"
F 6 "Nichicon" V 5500 2000 50  0001 C CNN "Manu_Name"
	1    5500 2000
	0    1    1    0   
$EndComp
$Comp
L Connector:AudioJack3_Ground_Switch J2
U 1 1 5E308FC0
P 4950 2200
F 0 "J2" H 4900 2450 50  0000 C CNN
F 1 "AudioJack3_Ground_Switch" V 4700 2200 50  0000 C CNN
F 2 "" H 4950 2200 50  0001 C CNN
F 3 "~" H 4950 2200 50  0001 C CNN
	1    4950 2200
	1    0    0    1   
$EndComp
Wire Wire Line
	5150 2000 5350 2000
Wire Wire Line
	5350 2200 5150 2200
Text Label 5250 2350 0    39   ~ 0
ANAGND
Wire Wire Line
	5250 2350 5200 2350
Wire Wire Line
	5200 2350 5200 2300
Wire Wire Line
	5200 2300 5150 2300
Wire Wire Line
	5650 2000 5700 2000
Wire Wire Line
	5700 2200 5650 2200
$Comp
L Connector:AudioJack3_Ground_Switch J1
U 1 1 5E311727
P 4950 1100
F 0 "J1" H 4900 1350 50  0000 C CNN
F 1 "AudioJack3_Ground_Switch" V 4700 1100 50  0000 C CNN
F 2 "" H 4950 1100 50  0001 C CNN
F 3 "~" H 4950 1100 50  0001 C CNN
	1    4950 1100
	1    0    0    1   
$EndComp
Wire Wire Line
	5150 900  5350 900 
Wire Wire Line
	5350 1100 5150 1100
Text Label 5250 1250 0    39   ~ 0
ANAGND
Wire Wire Line
	5250 1250 5200 1250
Wire Wire Line
	5200 1250 5200 1200
Wire Wire Line
	5200 1200 5150 1200
Wire Wire Line
	5650 900  5700 900 
Wire Wire Line
	5700 1100 5650 1100
$Comp
L Connector_Generic:Conn_01x04 J13
U 1 1 5DDDD87E
P 7900 2900
F 0 "J13" H 7900 2600 50  0000 C CNN
F 1 "Conn_01x04" V 8000 2850 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Horizontal" H 7900 2900 50  0001 C CNN
F 3 "~" H 7900 2900 50  0001 C CNN
	1    7900 2900
	-1   0    0    1   
$EndComp
$Comp
L Connector_Generic:Conn_01x04 J14
U 1 1 5DDDDA2F
P 8700 2900
F 0 "J14" H 8700 2600 50  0000 C CNN
F 1 "Conn_01x04" V 8800 2850 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Horizontal" H 8700 2900 50  0001 C CNN
F 3 "~" H 8700 2900 50  0001 C CNN
	1    8700 2900
	-1   0    0    1   
$EndComp
Text Label 8100 2900 0    39   ~ 0
GND
Text Label 8100 2800 0    39   ~ 0
Vcc
Text Label 8100 2700 0    39   ~ 0
ANAVcc
Text Label 8100 3000 0    39   ~ 0
ANAGND
Text Label 8900 3000 0    39   ~ 0
~RESET
Text Label 8900 2700 0    39   ~ 0
VccDiv2
Text Label 8900 2800 0    39   ~ 0
SDA
Text Label 8900 2900 0    39   ~ 0
SCL
$Comp
L Device:CP1 C4
U 1 1 5DE02DB2
P 5500 2200
F 0 "C4" V 5550 2100 39  0000 C CNN
F 1 "100uF" V 5550 2350 39  0000 C CNN
F 2 "Capacitor_SMD:CP_Elec_6.3x5.4_Nichicon" H 5500 2200 50  0001 C CNN
F 3 "~" H 5500 2200 50  0001 C CNN
F 4 "493-16090-1-ND" V 5500 2200 50  0001 C CNN "Digikey_Number"
F 5 "UUQ0J101MCL1GB" V 5500 2200 50  0001 C CNN "Manu_Number"
F 6 "Nichicon" V 5500 2200 50  0001 C CNN "Manu_Name"
	1    5500 2200
	0    1    1    0   
$EndComp
$Comp
L Device:CP1 C2
U 1 1 5DE02E3C
P 5500 1100
F 0 "C2" V 5550 1000 39  0000 C CNN
F 1 "100uF" V 5550 1250 39  0000 C CNN
F 2 "Capacitor_SMD:CP_Elec_6.3x5.4_Nichicon" H 5500 1100 50  0001 C CNN
F 3 "~" H 5500 1100 50  0001 C CNN
F 4 "493-16090-1-ND" V 5500 1100 50  0001 C CNN "Digikey_Number"
F 5 "UUQ0J101MCL1GB" V 5500 1100 50  0001 C CNN "Manu_Number"
F 6 "Nichicon" V 5500 1100 50  0001 C CNN "Manu_Name"
	1    5500 1100
	0    1    1    0   
$EndComp
$Comp
L Device:CP1 C1
U 1 1 5DE02EE8
P 5500 900
F 0 "C1" V 5550 800 39  0000 C CNN
F 1 "100uF" V 5550 1050 39  0000 C CNN
F 2 "Capacitor_SMD:CP_Elec_6.3x5.4_Nichicon" H 5500 900 50  0001 C CNN
F 3 "~" H 5500 900 50  0001 C CNN
F 4 "493-16090-1-ND" V 5500 900 50  0001 C CNN "Digikey_Number"
F 5 "UUQ0J101MCL1GB" V 5500 900 50  0001 C CNN "Manu_Number"
F 6 "Nichicon" V 5500 900 50  0001 C CNN "Manu_Name"
	1    5500 900 
	0    1    1    0   
$EndComp
$Comp
L Device:CP1 C11
U 1 1 5DE02F72
P 7050 900
F 0 "C11" V 7100 800 39  0000 C CNN
F 1 "100uF" V 7100 1050 39  0000 C CNN
F 2 "Capacitor_SMD:CP_Elec_6.3x5.4_Nichicon" H 7050 900 50  0001 C CNN
F 3 "~" H 7050 900 50  0001 C CNN
F 4 "493-16090-1-ND" V 7050 900 50  0001 C CNN "Digikey_Number"
F 5 "UUQ0J101MCL1GB" V 7050 900 50  0001 C CNN "Manu_Number"
F 6 "Nichicon" V 7050 900 50  0001 C CNN "Manu_Name"
	1    7050 900 
	0    1    1    0   
$EndComp
$Comp
L Device:CP1 C12
U 1 1 5DE03038
P 7050 1100
F 0 "C12" V 7100 1000 39  0000 C CNN
F 1 "100uF" V 7100 1250 39  0000 C CNN
F 2 "Capacitor_SMD:CP_Elec_6.3x5.4_Nichicon" H 7050 1100 50  0001 C CNN
F 3 "~" H 7050 1100 50  0001 C CNN
F 4 "493-16090-1-ND" V 7050 1100 50  0001 C CNN "Digikey_Number"
F 5 "UUQ0J101MCL1GB" V 7050 1100 50  0001 C CNN "Manu_Number"
F 6 "Nichicon" V 7050 1100 50  0001 C CNN "Manu_Name"
	1    7050 1100
	0    1    1    0   
$EndComp
$Comp
L Device:CP1 C13
U 1 1 5DE030C2
P 7050 2000
F 0 "C13" V 7100 1900 39  0000 C CNN
F 1 "100uF" V 7100 2150 39  0000 C CNN
F 2 "Capacitor_SMD:CP_Elec_6.3x5.4_Nichicon" H 7050 2000 50  0001 C CNN
F 3 "~" H 7050 2000 50  0001 C CNN
F 4 "493-16090-1-ND" V 7050 2000 50  0001 C CNN "Digikey_Number"
F 5 "UUQ0J101MCL1GB" V 7050 2000 50  0001 C CNN "Manu_Number"
F 6 "Nichicon" V 7050 2000 50  0001 C CNN "Manu_Name"
	1    7050 2000
	0    1    1    0   
$EndComp
$Comp
L Device:CP1 C14
U 1 1 5DE03172
P 7050 2200
F 0 "C14" V 7100 2100 39  0000 C CNN
F 1 "100uF" V 7100 2350 39  0000 C CNN
F 2 "Capacitor_SMD:CP_Elec_6.3x5.4_Nichicon" H 7050 2200 50  0001 C CNN
F 3 "~" H 7050 2200 50  0001 C CNN
F 4 "493-16090-1-ND" V 7050 2200 50  0001 C CNN "Digikey_Number"
F 5 "UUQ0J101MCL1GB" V 7050 2200 50  0001 C CNN "Manu_Number"
F 6 "Nichicon" V 7050 2200 50  0001 C CNN "Manu_Name"
	1    7050 2200
	0    1    1    0   
$EndComp
Text Label 8950 3450 0    39   ~ 0
SCL
Text Label 8950 3350 0    39   ~ 0
SDA
Text Label 8650 3350 2    39   ~ 0
Vcc
$Comp
L Device:R R25
U 1 1 5DE62C76
P 8800 3350
F 0 "R25" V 8900 3300 50  0000 L CNN
F 1 "2k2" V 8800 3300 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 8730 3350 50  0001 C CNN
F 3 "~" H 8800 3350 50  0001 C CNN
F 4 "RNCP0603FTD20K0CT-ND" V 8800 3350 50  0001 C CNN "Digikey_Number"
F 5 "RNCP0603FTD20K0" V 8800 3350 50  0001 C CNN "Manu_Number"
F 6 "Stackpole Electronics Inc" V 8800 3350 50  0001 C CNN "Manu_Name"
	1    8800 3350
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R26
U 1 1 5DE6324E
P 8800 3450
F 0 "R26" V 8700 3400 50  0000 L CNN
F 1 "2k2" V 8800 3400 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 8730 3450 50  0001 C CNN
F 3 "~" H 8800 3450 50  0001 C CNN
F 4 "RNCP0603FTD20K0CT-ND" V 8800 3450 50  0001 C CNN "Digikey_Number"
F 5 "RNCP0603FTD20K0" V 8800 3450 50  0001 C CNN "Manu_Number"
F 6 "Stackpole Electronics Inc" V 8800 3450 50  0001 C CNN "Manu_Name"
	1    8800 3450
	0    -1   -1   0   
$EndComp
Text Label 8650 3450 2    39   ~ 0
Vcc
$Comp
L Device:CP1 C19
U 1 1 5DE6CCDB
P 3300 3400
F 0 "C19" V 3350 3300 39  0000 C CNN
F 1 "3.3uF" V 3350 3550 39  0000 C CNN
F 2 "Capacitor_SMD:CP_Elec_4x5.4" H 3300 3400 50  0001 C CNN
F 3 "~" H 3300 3400 50  0001 C CNN
F 4 "493-17493-1-ND" V 3300 3400 50  0001 C CNN "Digikey_Number"
F 5 "UUQ1H3R3MCL1GB" V 3300 3400 50  0001 C CNN "Manu_Number"
F 6 "Nichicon" V 3300 3400 50  0001 C CNN "Manu_Name"
	1    3300 3400
	0    -1   -1   0   
$EndComp
$Comp
L Device:CP1 C18
U 1 1 5DE6D297
P 3300 2450
F 0 "C18" V 3350 2350 39  0000 C CNN
F 1 "3.3uF" V 3350 2600 39  0000 C CNN
F 2 "Capacitor_SMD:CP_Elec_4x5.4" H 3300 2450 50  0001 C CNN
F 3 "~" H 3300 2450 50  0001 C CNN
F 4 "493-17493-1-ND" V 3300 2450 50  0001 C CNN "Digikey_Number"
F 5 "UUQ1H3R3MCL1GB" V 3300 2450 50  0001 C CNN "Manu_Number"
F 6 "Nichicon" V 3300 2450 50  0001 C CNN "Manu_Name"
	1    3300 2450
	0    -1   -1   0   
$EndComp
$Comp
L Device:CP1 C17
U 1 1 5DE6D2A0
P 3300 2250
F 0 "C17" V 3350 2150 39  0000 C CNN
F 1 "3.3uF" V 3350 2400 39  0000 C CNN
F 2 "Capacitor_SMD:CP_Elec_4x5.4" H 3300 2250 50  0001 C CNN
F 3 "~" H 3300 2250 50  0001 C CNN
F 4 "493-17493-1-ND" V 3300 2250 50  0001 C CNN "Digikey_Number"
F 5 "UUQ1H3R3MCL1GB" V 3300 2250 50  0001 C CNN "Manu_Number"
F 6 "Nichicon" V 3300 2250 50  0001 C CNN "Manu_Name"
	1    3300 2250
	0    -1   -1   0   
$EndComp
$Comp
L Device:CP1 C16
U 1 1 5DE71B5F
P 3300 1300
F 0 "C16" V 3350 1200 39  0000 C CNN
F 1 "3.3uF" V 3350 1450 39  0000 C CNN
F 2 "Capacitor_SMD:CP_Elec_4x5.4" H 3300 1300 50  0001 C CNN
F 3 "~" H 3300 1300 50  0001 C CNN
F 4 "493-17493-1-ND" V 3300 1300 50  0001 C CNN "Digikey_Number"
F 5 "UUQ1H3R3MCL1GB" V 3300 1300 50  0001 C CNN "Manu_Number"
F 6 "Nichicon" V 3300 1300 50  0001 C CNN "Manu_Name"
	1    3300 1300
	0    -1   -1   0   
$EndComp
$Comp
L Device:CP1 C15
U 1 1 5DE71B69
P 3300 1100
F 0 "C15" V 3350 1000 39  0000 C CNN
F 1 "3.3uF" V 3350 1250 39  0000 C CNN
F 2 "Capacitor_SMD:CP_Elec_4x5.4" H 3300 1100 50  0001 C CNN
F 3 "~" H 3300 1100 50  0001 C CNN
F 4 "493-17493-1-ND" V 3300 1100 50  0001 C CNN "Digikey_Number"
F 5 "UUQ1H3R3MCL1GB" V 3300 1100 50  0001 C CNN "Manu_Number"
F 6 "Nichicon" V 3300 1100 50  0001 C CNN "Manu_Name"
	1    3300 1100
	0    -1   -1   0   
$EndComp
$Comp
L Device:CP1 C10
U 1 1 5DE762CE
P 1450 3600
F 0 "C10" V 1500 3500 39  0000 C CNN
F 1 "3.3uF" V 1500 3750 39  0000 C CNN
F 2 "Capacitor_SMD:CP_Elec_4x5.4" H 1450 3600 50  0001 C CNN
F 3 "~" H 1450 3600 50  0001 C CNN
F 4 "493-17493-1-ND" V 1450 3600 50  0001 C CNN "Digikey_Number"
F 5 "UUQ1H3R3MCL1GB" V 1450 3600 50  0001 C CNN "Manu_Number"
F 6 "Nichicon" V 1450 3600 50  0001 C CNN "Manu_Name"
	1    1450 3600
	0    -1   -1   0   
$EndComp
$Comp
L Device:CP1 C9
U 1 1 5DE762D8
P 1450 3400
F 0 "C9" V 1500 3300 39  0000 C CNN
F 1 "3.3uF" V 1500 3550 39  0000 C CNN
F 2 "Capacitor_SMD:CP_Elec_4x5.4" H 1450 3400 50  0001 C CNN
F 3 "~" H 1450 3400 50  0001 C CNN
F 4 "493-17493-1-ND" V 1450 3400 50  0001 C CNN "Digikey_Number"
F 5 "UUQ1H3R3MCL1GB" V 1450 3400 50  0001 C CNN "Manu_Number"
F 6 "Nichicon" V 1450 3400 50  0001 C CNN "Manu_Name"
	1    1450 3400
	0    -1   -1   0   
$EndComp
$Comp
L Device:CP1 C8
U 1 1 5DE7A910
P 1450 2450
F 0 "C8" V 1500 2350 39  0000 C CNN
F 1 "3.3uF" V 1500 2600 39  0000 C CNN
F 2 "Capacitor_SMD:CP_Elec_4x5.4" H 1450 2450 50  0001 C CNN
F 3 "~" H 1450 2450 50  0001 C CNN
F 4 "493-17493-1-ND" V 1450 2450 50  0001 C CNN "Digikey_Number"
F 5 "UUQ1H3R3MCL1GB" V 1450 2450 50  0001 C CNN "Manu_Number"
F 6 "Nichicon" V 1450 2450 50  0001 C CNN "Manu_Name"
	1    1450 2450
	0    -1   -1   0   
$EndComp
$Comp
L Device:CP1 C7
U 1 1 5DE7A91A
P 1450 2250
F 0 "C7" V 1500 2150 39  0000 C CNN
F 1 "3.3uF" V 1500 2400 39  0000 C CNN
F 2 "Capacitor_SMD:CP_Elec_4x5.4" H 1450 2250 50  0001 C CNN
F 3 "~" H 1450 2250 50  0001 C CNN
F 4 "493-17493-1-ND" V 1450 2250 50  0001 C CNN "Digikey_Number"
F 5 "UUQ1H3R3MCL1GB" V 1450 2250 50  0001 C CNN "Manu_Number"
F 6 "Nichicon" V 1450 2250 50  0001 C CNN "Manu_Name"
	1    1450 2250
	0    -1   -1   0   
$EndComp
$Comp
L Device:CP1 C6
U 1 1 5DE7EF46
P 1450 1300
F 0 "C6" V 1500 1200 39  0000 C CNN
F 1 "3.3uF" V 1500 1450 39  0000 C CNN
F 2 "Capacitor_SMD:CP_Elec_4x5.4" H 1450 1300 50  0001 C CNN
F 3 "~" H 1450 1300 50  0001 C CNN
F 4 "493-17493-1-ND" V 1450 1300 50  0001 C CNN "Digikey_Number"
F 5 "UUQ1H3R3MCL1GB" V 1450 1300 50  0001 C CNN "Manu_Number"
F 6 "Nichicon" V 1450 1300 50  0001 C CNN "Manu_Name"
	1    1450 1300
	0    -1   -1   0   
$EndComp
$Comp
L Device:CP1 C5
U 1 1 5DE7EF50
P 1450 1100
F 0 "C5" V 1500 1000 39  0000 C CNN
F 1 "3.3uF" V 1500 1250 39  0000 C CNN
F 2 "Capacitor_SMD:CP_Elec_4x5.4" H 1450 1100 50  0001 C CNN
F 3 "~" H 1450 1100 50  0001 C CNN
F 4 "493-17493-1-ND" V 1450 1100 50  0001 C CNN "Digikey_Number"
F 5 "UUQ1H3R3MCL1GB" V 1450 1100 50  0001 C CNN "Manu_Number"
F 6 "Nichicon" V 1450 1100 50  0001 C CNN "Manu_Name"
	1    1450 1100
	0    -1   -1   0   
$EndComp
$Comp
L ViamSonus2.0-parts:LM1085 U1
U 1 1 5DE887A1
P 6650 3100
F 0 "U1" H 6650 3365 50  0000 C CNN
F 1 "LM1085" H 6650 3274 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:TO-263-3_TabPin2" H 6650 3100 50  0001 C CNN
F 3 "" H 6650 3100 50  0001 C CNN
F 4 "296-35391-1-ND" H 6650 3100 50  0001 C CNN "Digikey_Number"
F 5 "LM1085ISX-5.0/NOPB" H 6650 3100 50  0001 C CNN "Manu_Number"
F 6 "Texas Instruments" H 6650 3100 50  0001 C CNN "Manu_Name"
	1    6650 3100
	1    0    0    -1  
$EndComp
$Comp
L Connector:Barrel_Jack J15
U 1 1 5DE88EDC
P 5550 3300
F 0 "J15" H 5605 3625 50  0000 C CNN
F 1 "Barrel_Jack" H 5605 3534 50  0000 C CNN
F 2 "" H 5600 3260 50  0001 C CNN
F 3 "~" H 5600 3260 50  0001 C CNN
	1    5550 3300
	1    0    0    -1  
$EndComp
$Comp
L Device:CP1 C21
U 1 1 5DE895B1
P 6150 3300
F 0 "C21" V 6200 3200 39  0000 C CNN
F 1 "10uF" V 6300 3300 39  0000 C CNN
F 2 "Capacitor_SMD:CP_Elec_4x5.4" H 6150 3300 50  0001 C CNN
F 3 "~" H 6150 3300 50  0001 C CNN
F 4 "493-14558-1-ND" V 6150 3300 50  0001 C CNN "Digikey_Number"
F 5 "UUQ11C100MCL1GB" V 6150 3300 50  0001 C CNN "Manu_Number"
F 6 "Nichicon" V 6150 3300 50  0001 C CNN "Manu_Name"
	1    6150 3300
	1    0    0    -1  
$EndComp
$Comp
L Device:CP1 C22
U 1 1 5DE89C62
P 7100 3300
F 0 "C22" V 7150 3200 39  0000 C CNN
F 1 "10uF" V 7250 3300 39  0000 C CNN
F 2 "Capacitor_SMD:CP_Elec_4x5.4" H 7100 3300 50  0001 C CNN
F 3 "~" H 7100 3300 50  0001 C CNN
F 4 "493-14558-1-ND" V 7100 3300 50  0001 C CNN "Digikey_Number"
F 5 "UUQ11C100MCL1GB" V 7100 3300 50  0001 C CNN "Manu_Number"
F 6 "Nichicon" V 7100 3300 50  0001 C CNN "Manu_Name"
	1    7100 3300
	1    0    0    -1  
$EndComp
Wire Wire Line
	6150 3150 6150 3100
Wire Wire Line
	6150 3100 6300 3100
Wire Wire Line
	7000 3100 7100 3100
Wire Wire Line
	7100 3100 7100 3150
Wire Wire Line
	6650 3350 6650 3500
Wire Wire Line
	6650 3500 6150 3500
Wire Wire Line
	6150 3500 6150 3450
Wire Wire Line
	6650 3500 7100 3500
Wire Wire Line
	7100 3500 7100 3450
Connection ~ 6650 3500
Wire Wire Line
	5850 3200 5900 3200
Wire Wire Line
	5900 3200 5900 3100
Wire Wire Line
	5900 3100 6150 3100
Connection ~ 6150 3100
Wire Wire Line
	5850 3400 5900 3400
Wire Wire Line
	5900 3400 5900 3500
Wire Wire Line
	5900 3500 6150 3500
Connection ~ 6150 3500
Text Label 7200 3100 0    39   ~ 0
ANAVcc
Wire Wire Line
	7200 3100 7100 3100
Connection ~ 7100 3100
Text Label 7200 3500 0    39   ~ 0
ANAGND
Wire Wire Line
	7200 3500 7100 3500
Connection ~ 7100 3500
Wire Wire Line
	3500 3950 3500 4000
Connection ~ 3500 4000
Wire Wire Line
	3500 4000 3000 4000
Wire Wire Line
	3500 2800 3500 2850
Connection ~ 3500 2850
Wire Wire Line
	3500 2850 3000 2850
Wire Wire Line
	3500 1650 3500 1700
Connection ~ 3500 1700
Wire Wire Line
	3500 1700 3000 1700
Wire Wire Line
	1650 3950 1650 4000
Connection ~ 1650 4000
Wire Wire Line
	1650 4000 1150 4000
Wire Wire Line
	1650 2800 1650 2850
Connection ~ 1650 2850
Wire Wire Line
	1650 2850 1150 2850
Wire Wire Line
	1650 1650 1650 1700
Connection ~ 1650 1700
Wire Wire Line
	1650 1700 1150 1700
$EndSCHEMATC
