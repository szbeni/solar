EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 2 3
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text Notes 1200 1050 0    50   ~ 0
Power Stage for Solar (at the moment this is realized on the modified DC/DC converter)
Wire Notes Line
	1200 4250 1200 950 
Wire Notes Line
	10350 4250 1200 4250
Wire Notes Line
	10350 950  10350 4250
Wire Notes Line
	1200 950  10350 950 
Connection ~ 4100 3800
Connection ~ 9200 2300
Wire Wire Line
	9200 2300 9200 2500
Wire Wire Line
	9200 2900 9200 3350
Wire Wire Line
	9200 3350 9550 3350
$Comp
L power:+12V #PWR?
U 1 1 607BCE84
P 9550 3350
AR Path="/607BCE84" Ref="#PWR?"  Part="1" 
AR Path="/607A4A71/607BCE84" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 9550 3200 50  0001 C CNN
F 1 "+12V" H 9565 3523 50  0000 C CNN
F 2 "" H 9550 3350 50  0001 C CNN
F 3 "" H 9550 3350 50  0001 C CNN
	1    9550 3350
	1    0    0    -1  
$EndComp
Wire Wire Line
	7850 3050 7900 3050
Connection ~ 7850 3050
Wire Wire Line
	7850 3100 7850 3050
Connection ~ 8200 3500
Wire Wire Line
	8200 3700 8200 3500
Wire Wire Line
	8200 3250 8200 3500
Wire Wire Line
	7850 3500 8200 3500
Wire Wire Line
	7850 3400 7850 3500
$Comp
L Device:R R?
U 1 1 607BCE92
P 7850 3250
AR Path="/607BCE92" Ref="R?"  Part="1" 
AR Path="/607A4A71/607BCE92" Ref="R?"  Part="1" 
F 0 "R?" V 7643 3250 50  0000 C CNN
F 1 "100k" V 7734 3250 50  0000 C CNN
F 2 "" V 7780 3250 50  0001 C CNN
F 3 "~" H 7850 3250 50  0001 C CNN
	1    7850 3250
	-1   0    0    1   
$EndComp
Text GLabel 8800 2000 0    50   Input ~ 0
DCDC_VOUT
Wire Wire Line
	9200 2000 8800 2000
Wire Wire Line
	9200 2300 9200 2000
Connection ~ 8200 2700
Wire Wire Line
	8200 2700 8200 2850
Wire Wire Line
	8200 2700 8900 2700
Wire Wire Line
	8200 2600 8200 2700
$Comp
L Device:R R?
U 1 1 607BCE9F
P 8200 2450
AR Path="/607BCE9F" Ref="R?"  Part="1" 
AR Path="/607A4A71/607BCE9F" Ref="R?"  Part="1" 
F 0 "R?" V 7993 2450 50  0000 C CNN
F 1 "1k" V 8084 2450 50  0000 C CNN
F 2 "" V 8130 2450 50  0001 C CNN
F 3 "~" H 8200 2450 50  0001 C CNN
	1    8200 2450
	-1   0    0    1   
$EndComp
Wire Wire Line
	9200 2300 8200 2300
$Comp
L Transistor_FET:IRF9540N Q?
U 1 1 607BCEA6
P 9100 2700
AR Path="/607BCEA6" Ref="Q?"  Part="1" 
AR Path="/607A4A71/607BCEA6" Ref="Q?"  Part="1" 
F 0 "Q?" H 9304 2654 50  0000 L CNN
F 1 "ipp120p04p4l-03" H 9304 2745 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-220-3_Vertical" H 9300 2625 50  0001 L CIN
F 3 "https://www.infineon.com/dgdl/Infineon-IPP_B_I120P04P4L_03-DataSheet-v01_02-EN.pdf?fileId=db3a30432f69f146012f783b3b5a2e3f" H 9100 2700 50  0001 L CNN
	1    9100 2700
	1    0    0    1   
$EndComp
Wire Wire Line
	7500 3050 7300 3050
Wire Wire Line
	7800 3050 7850 3050
$Comp
L Transistor_BJT:PN2222A Q?
U 1 1 607BCEAE
P 8100 3050
AR Path="/607BCEAE" Ref="Q?"  Part="1" 
AR Path="/607A4A71/607BCEAE" Ref="Q?"  Part="1" 
F 0 "Q?" H 8290 3096 50  0000 L CNN
F 1 "MMBT2222A" H 8290 3005 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 8300 2975 50  0001 L CIN
F 3 "https://www.diodes.com/assets/Datasheets/ds30041.pdf" H 8100 3050 50  0001 L CNN
	1    8100 3050
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 607BCEB4
P 8200 3700
AR Path="/607BCEB4" Ref="#PWR?"  Part="1" 
AR Path="/607A4A71/607BCEB4" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 8200 3450 50  0001 C CNN
F 1 "GND" H 8205 3527 50  0000 C CNN
F 2 "" H 8200 3700 50  0001 C CNN
F 3 "" H 8200 3700 50  0001 C CNN
	1    8200 3700
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 607BCEBA
P 7650 3050
AR Path="/607BCEBA" Ref="R?"  Part="1" 
AR Path="/607A4A71/607BCEBA" Ref="R?"  Part="1" 
F 0 "R?" V 7443 3050 50  0000 C CNN
F 1 "470" V 7534 3050 50  0000 C CNN
F 2 "" V 7580 3050 50  0001 C CNN
F 3 "~" H 7650 3050 50  0001 C CNN
	1    7650 3050
	0    1    1    0   
$EndComp
Wire Wire Line
	2150 2900 2350 2900
Text GLabel 2150 2900 0    50   Input ~ 0
PWM_DCDC_SD
Wire Wire Line
	2150 2800 2350 2800
Text GLabel 2150 2800 0    50   Input ~ 0
PWM_DCDC
Wire Wire Line
	2650 3300 2650 3450
Connection ~ 2650 3300
Wire Wire Line
	2650 3250 2650 3300
$Comp
L Driver_FET:IR2104 U?
U 1 1 607BCEC7
P 2650 2800
AR Path="/607BCEC7" Ref="U?"  Part="1" 
AR Path="/607A4A71/607BCEC7" Ref="U?"  Part="1" 
F 0 "U?" H 2650 3481 50  0000 C CNN
F 1 "IR2104" H 2450 3250 50  0000 C CNN
F 2 "" H 2650 2800 50  0001 C CIN
F 3 "https://www.infineon.com/dgdl/ir2104.pdf?fileId=5546d462533600a4015355c7c1c31671" H 2650 2800 50  0001 C CNN
	1    2650 2800
	1    0    0    -1  
$EndComp
Wire Wire Line
	3750 3100 3800 3100
Wire Wire Line
	3450 3100 2950 3100
$Comp
L Device:R R?
U 1 1 607BCECF
P 3600 3100
AR Path="/607BCECF" Ref="R?"  Part="1" 
AR Path="/607A4A71/607BCECF" Ref="R?"  Part="1" 
F 0 "R?" V 3700 3100 50  0000 C CNN
F 1 "22" V 3800 3100 50  0000 C CNN
F 2 "" V 3530 3100 50  0001 C CNN
F 3 "~" H 3600 3100 50  0001 C CNN
	1    3600 3100
	0    1    1    0   
$EndComp
Wire Wire Line
	3450 2600 3450 1900
Wire Wire Line
	2950 2600 3450 2600
Wire Wire Line
	3750 1900 3800 1900
$Comp
L Device:R R?
U 1 1 607BCED8
P 3600 1900
AR Path="/607BCED8" Ref="R?"  Part="1" 
AR Path="/607A4A71/607BCED8" Ref="R?"  Part="1" 
F 0 "R?" V 3393 1900 50  0000 C CNN
F 1 "22" V 3484 1900 50  0000 C CNN
F 2 "" V 3530 1900 50  0001 C CNN
F 3 "~" H 3600 1900 50  0001 C CNN
	1    3600 1900
	0    1    1    0   
$EndComp
Wire Wire Line
	1800 2350 1800 2250
$Comp
L power:GND #PWR?
U 1 1 607BCEDF
P 1800 2350
AR Path="/607BCEDF" Ref="#PWR?"  Part="1" 
AR Path="/607A4A71/607BCEDF" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 1800 2100 50  0001 C CNN
F 1 "GND" H 1805 2177 50  0000 C CNN
F 2 "" H 1800 2350 50  0001 C CNN
F 3 "" H 1800 2350 50  0001 C CNN
	1    1800 2350
	1    0    0    -1  
$EndComp
Connection ~ 1800 1950
$Comp
L Device:C C?
U 1 1 607BCEE6
P 1800 2100
AR Path="/607BCEE6" Ref="C?"  Part="1" 
AR Path="/607A4A71/607BCEE6" Ref="C?"  Part="1" 
F 0 "C?" V 1548 2100 50  0000 C CNN
F 1 "100nF" V 1639 2100 50  0000 C CNN
F 2 "" H 1838 1950 50  0001 C CNN
F 3 "~" H 1800 2100 50  0001 C CNN
	1    1800 2100
	-1   0    0    1   
$EndComp
Wire Wire Line
	1800 1800 1800 1950
Connection ~ 2650 1950
Wire Wire Line
	1800 1950 2650 1950
$Comp
L power:+12V #PWR?
U 1 1 607BCEEF
P 1800 1800
AR Path="/607BCEEF" Ref="#PWR?"  Part="1" 
AR Path="/607A4A71/607BCEEF" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 1800 1650 50  0001 C CNN
F 1 "+12V" H 1815 1973 50  0000 C CNN
F 2 "" H 1800 1800 50  0001 C CNN
F 3 "" H 1800 1800 50  0001 C CNN
	1    1800 1800
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 607BCEF5
P 2650 3450
AR Path="/607BCEF5" Ref="#PWR?"  Part="1" 
AR Path="/607A4A71/607BCEF5" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 2650 3200 50  0001 C CNN
F 1 "GND" H 2655 3277 50  0000 C CNN
F 2 "" H 2650 3450 50  0001 C CNN
F 3 "" H 2650 3450 50  0001 C CNN
	1    2650 3450
	1    0    0    -1  
$EndComp
Wire Wire Line
	3250 2500 3250 1950
Connection ~ 3250 2500
Wire Wire Line
	2950 2500 3250 2500
Wire Wire Line
	3250 1950 3050 1950
Wire Wire Line
	3250 2650 3250 2500
Wire Wire Line
	3250 3000 3800 3000
Connection ~ 3250 3000
Wire Wire Line
	3250 2950 3250 3000
$Comp
L Device:CP C?
U 1 1 607BCF03
P 3250 2800
AR Path="/607BCF03" Ref="C?"  Part="1" 
AR Path="/607A4A71/607BCF03" Ref="C?"  Part="1" 
F 0 "C?" H 3368 2846 50  0000 L CNN
F 1 "10uF" H 3368 2755 50  0000 L CNN
F 2 "" H 3288 2650 50  0001 C CNN
F 3 "~" H 3250 2800 50  0001 C CNN
	1    3250 2800
	1    0    0    -1  
$EndComp
Wire Wire Line
	2650 1950 2750 1950
Wire Wire Line
	2650 1950 2650 2300
$Comp
L Device:D D?
U 1 1 607BCF0B
P 2900 1950
AR Path="/607BCF0B" Ref="D?"  Part="1" 
AR Path="/607A4A71/607BCF0B" Ref="D?"  Part="1" 
F 0 "D?" H 2900 1734 50  0000 C CNN
F 1 "D" H 2900 1825 50  0000 C CNN
F 2 "" H 2900 1950 50  0001 C CNN
F 3 "~" H 2900 1950 50  0001 C CNN
	1    2900 1950
	-1   0    0    1   
$EndComp
Wire Wire Line
	3800 2600 4100 2600
Wire Wire Line
	3800 3000 3800 2600
Wire Wire Line
	2950 3000 3250 3000
Wire Wire Line
	3800 3100 3800 3550
Text GLabel 6550 2600 2    50   Input ~ 0
DCDC_VOUT
Connection ~ 5750 2600
Wire Wire Line
	5750 2600 6550 2600
Wire Wire Line
	5750 3250 5750 3800
Wire Wire Line
	4100 3800 4100 3850
Wire Wire Line
	4100 3800 5750 3800
Wire Wire Line
	4100 3750 4100 3800
Wire Wire Line
	5750 2600 5750 2950
Wire Wire Line
	5350 2600 5750 2600
$Comp
L Device:CP C?
U 1 1 607BCF1E
P 5750 3100
AR Path="/607BCF1E" Ref="C?"  Part="1" 
AR Path="/607A4A71/607BCF1E" Ref="C?"  Part="1" 
F 0 "C?" H 5868 3146 50  0000 L CNN
F 1 "470uF" H 5868 3055 50  0000 L CNN
F 2 "" H 5788 2950 50  0001 C CNN
F 3 "~" H 5750 3100 50  0001 C CNN
	1    5750 3100
	1    0    0    -1  
$EndComp
Wire Wire Line
	4100 2600 4850 2600
$Comp
L pspice:INDUCTOR L?
U 1 1 607BCF25
P 5100 2600
AR Path="/607BCF25" Ref="L?"  Part="1" 
AR Path="/607A4A71/607BCF25" Ref="L?"  Part="1" 
F 0 "L?" H 5100 2815 50  0000 C CNN
F 1 "22uH" H 5100 2724 50  0000 C CNN
F 2 "" H 5100 2600 50  0001 C CNN
F 3 "~" H 5100 2600 50  0001 C CNN
	1    5100 2600
	1    0    0    -1  
$EndComp
Wire Wire Line
	4100 2600 4100 3350
Wire Wire Line
	4100 2100 4100 2600
Connection ~ 4100 2600
$Comp
L power:GND #PWR?
U 1 1 607BCF2E
P 4100 3850
AR Path="/607BCF2E" Ref="#PWR?"  Part="1" 
AR Path="/607A4A71/607BCF2E" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 4100 3600 50  0001 C CNN
F 1 "GND" H 4105 3677 50  0000 C CNN
F 2 "" H 4100 3850 50  0001 C CNN
F 3 "" H 4100 3850 50  0001 C CNN
	1    4100 3850
	1    0    0    -1  
$EndComp
Wire Wire Line
	4100 1400 4100 1700
Wire Wire Line
	3600 1400 4100 1400
Text GLabel 3600 1400 0    50   Input ~ 0
V_SOLAR
$Comp
L Transistor_FET:IRF540N Q?
U 1 1 607BCF37
P 4000 3550
AR Path="/607BCF37" Ref="Q?"  Part="1" 
AR Path="/607A4A71/607BCF37" Ref="Q?"  Part="1" 
F 0 "Q?" H 4206 3596 50  0000 L CNN
F 1 "IRF2204" H 4206 3505 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-220-3_Vertical" H 4250 3475 50  0001 L CIN
F 3 "http://www.irf.com/product-info/datasheets/data/irf2204.pdf" H 4000 3550 50  0001 L CNN
	1    4000 3550
	1    0    0    -1  
$EndComp
$Comp
L Transistor_FET:IRF540N Q?
U 1 1 607BCF3D
P 4000 1900
AR Path="/607BCF3D" Ref="Q?"  Part="1" 
AR Path="/607A4A71/607BCF3D" Ref="Q?"  Part="1" 
F 0 "Q?" H 4206 1946 50  0000 L CNN
F 1 "IRF2204" H 4206 1855 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-220-3_Vertical" H 4250 1825 50  0001 L CIN
F 3 "http://www.irf.com/product-info/datasheets/data/irf2204.pdf" H 4000 1900 50  0001 L CNN
	1    4000 1900
	1    0    0    -1  
$EndComp
NoConn ~ 2300 3050
$EndSCHEMATC
