EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 3 3
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
L Transistor_FET:IRF540N Q?
U 1 1 60855919
P 6900 2150
AR Path="/60855919" Ref="Q?"  Part="1" 
AR Path="/6082BF78/60855919" Ref="Q?"  Part="1" 
F 0 "Q?" H 7106 2196 50  0000 L CNN
F 1 "IRF2204" H 7106 2105 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-220-3_Vertical" H 7150 2075 50  0001 L CIN
F 3 "http://www.irf.com/product-info/datasheets/data/irf2204.pdf" H 6900 2150 50  0001 L CNN
	1    6900 2150
	1    0    0    -1  
$EndComp
$Comp
L Transistor_FET:IRF540N Q?
U 1 1 6085591F
P 6900 3800
AR Path="/6085591F" Ref="Q?"  Part="1" 
AR Path="/6082BF78/6085591F" Ref="Q?"  Part="1" 
F 0 "Q?" H 7106 3846 50  0000 L CNN
F 1 "IRF2204" H 7106 3755 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-220-3_Vertical" H 7150 3725 50  0001 L CIN
F 3 "http://www.irf.com/product-info/datasheets/data/irf2204.pdf" H 6900 3800 50  0001 L CNN
	1    6900 3800
	1    0    0    -1  
$EndComp
Text GLabel 2350 2550 0    50   Input ~ 0
PWM_DCDC
Wire Wire Line
	2850 2550 2900 2550
$Comp
L Device:R R?
U 1 1 60855927
P 2900 2100
AR Path="/60855927" Ref="R?"  Part="1" 
AR Path="/6082BF78/60855927" Ref="R?"  Part="1" 
F 0 "R?" V 2693 2100 50  0000 C CNN
F 1 "100k" V 2784 2100 50  0000 C CNN
F 2 "" V 2830 2100 50  0001 C CNN
F 3 "~" H 2900 2100 50  0001 C CNN
	1    2900 2100
	-1   0    0    1   
$EndComp
Connection ~ 2900 2550
Wire Wire Line
	2900 2550 2950 2550
$Comp
L Device:R R?
U 1 1 6085592F
P 2700 2550
AR Path="/6085592F" Ref="R?"  Part="1" 
AR Path="/6082BF78/6085592F" Ref="R?"  Part="1" 
F 0 "R?" V 2493 2550 50  0000 C CNN
F 1 "1k" V 2584 2550 50  0000 C CNN
F 2 "" V 2630 2550 50  0001 C CNN
F 3 "~" H 2700 2550 50  0001 C CNN
	1    2700 2550
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR?
U 1 1 60855935
P 3250 2850
AR Path="/60855935" Ref="#PWR?"  Part="1" 
AR Path="/6082BF78/60855935" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 3250 2600 50  0001 C CNN
F 1 "GND" H 3255 2677 50  0000 C CNN
F 2 "" H 3250 2850 50  0001 C CNN
F 3 "" H 3250 2850 50  0001 C CNN
	1    3250 2850
	1    0    0    -1  
$EndComp
Wire Wire Line
	3250 2750 3250 2850
Wire Wire Line
	5150 1500 5150 1700
$Comp
L Device:R R?
U 1 1 6085593D
P 3250 1700
AR Path="/6085593D" Ref="R?"  Part="1" 
AR Path="/6082BF78/6085593D" Ref="R?"  Part="1" 
F 0 "R?" V 3043 1700 50  0000 C CNN
F 1 "3k" V 3134 1700 50  0000 C CNN
F 2 "" V 3180 1700 50  0001 C CNN
F 3 "~" H 3250 1700 50  0001 C CNN
	1    3250 1700
	-1   0    0    1   
$EndComp
Wire Wire Line
	3250 1500 3250 1550
$Comp
L Device:D D?
U 1 1 60855944
P 3100 1500
AR Path="/60855944" Ref="D?"  Part="1" 
AR Path="/6082BF78/60855944" Ref="D?"  Part="1" 
F 0 "D?" H 3100 1284 50  0000 C CNN
F 1 "D" H 3100 1375 50  0000 C CNN
F 2 "" H 3100 1500 50  0001 C CNN
F 3 "~" H 3100 1500 50  0001 C CNN
	1    3100 1500
	-1   0    0    1   
$EndComp
Connection ~ 3250 1500
$Comp
L power:+12V #PWR?
U 1 1 6085594B
P 2450 1250
AR Path="/6085594B" Ref="#PWR?"  Part="1" 
AR Path="/6082BF78/6085594B" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 2450 1100 50  0001 C CNN
F 1 "+12V" H 2465 1423 50  0000 C CNN
F 2 "" H 2450 1250 50  0001 C CNN
F 3 "" H 2450 1250 50  0001 C CNN
	1    2450 1250
	1    0    0    -1  
$EndComp
Wire Wire Line
	2450 1250 2450 1500
Text GLabel 6500 1650 0    50   Input ~ 0
V_SOLAR
$Comp
L Device:CP C?
U 1 1 60855953
P 5150 1850
AR Path="/60855953" Ref="C?"  Part="1" 
AR Path="/6082BF78/60855953" Ref="C?"  Part="1" 
F 0 "C?" H 5268 1896 50  0000 L CNN
F 1 "10uF" H 5268 1805 50  0000 L CNN
F 2 "" H 5188 1700 50  0001 C CNN
F 3 "~" H 5150 1850 50  0001 C CNN
	1    5150 1850
	1    0    0    -1  
$EndComp
Wire Wire Line
	5150 2000 5150 2850
Wire Wire Line
	6500 1650 7000 1650
Wire Wire Line
	7000 1650 7000 1950
$Comp
L power:GND #PWR?
U 1 1 6085595C
P 7000 4100
AR Path="/6085595C" Ref="#PWR?"  Part="1" 
AR Path="/6082BF78/6085595C" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 7000 3850 50  0001 C CNN
F 1 "GND" H 7005 3927 50  0000 C CNN
F 2 "" H 7000 4100 50  0001 C CNN
F 3 "" H 7000 4100 50  0001 C CNN
	1    7000 4100
	1    0    0    -1  
$EndComp
Text GLabel 2350 3800 0    50   Input ~ 0
PWM_DCDC_N
$Comp
L Device:R R?
U 1 1 60855963
P 2900 3500
AR Path="/60855963" Ref="R?"  Part="1" 
AR Path="/6082BF78/60855963" Ref="R?"  Part="1" 
F 0 "R?" V 2693 3500 50  0000 C CNN
F 1 "10k" V 2784 3500 50  0000 C CNN
F 2 "" V 2830 3500 50  0001 C CNN
F 3 "~" H 2900 3500 50  0001 C CNN
	1    2900 3500
	-1   0    0    1   
$EndComp
$Comp
L Device:R R?
U 1 1 60855969
P 2700 3800
AR Path="/60855969" Ref="R?"  Part="1" 
AR Path="/6082BF78/60855969" Ref="R?"  Part="1" 
F 0 "R?" V 2493 3800 50  0000 C CNN
F 1 "1k" V 2584 3800 50  0000 C CNN
F 2 "" V 2630 3800 50  0001 C CNN
F 3 "~" H 2700 3800 50  0001 C CNN
	1    2700 3800
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR?
U 1 1 6085596F
P 3250 4350
AR Path="/6085596F" Ref="#PWR?"  Part="1" 
AR Path="/6082BF78/6085596F" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 3250 4100 50  0001 C CNN
F 1 "GND" H 3255 4177 50  0000 C CNN
F 2 "" H 3250 4350 50  0001 C CNN
F 3 "" H 3250 4350 50  0001 C CNN
	1    3250 4350
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 60855975
P 3250 3400
AR Path="/60855975" Ref="R?"  Part="1" 
AR Path="/6082BF78/60855975" Ref="R?"  Part="1" 
F 0 "R?" V 3043 3400 50  0000 C CNN
F 1 "10k" V 3134 3400 50  0000 C CNN
F 2 "" V 3180 3400 50  0001 C CNN
F 3 "~" H 3250 3400 50  0001 C CNN
	1    3250 3400
	-1   0    0    1   
$EndComp
Wire Wire Line
	3250 3550 3250 3600
Wire Wire Line
	3250 3250 3250 3200
Wire Wire Line
	3250 3200 2900 3200
$Comp
L power:+12V #PWR?
U 1 1 6085597E
P 2450 3200
AR Path="/6085597E" Ref="#PWR?"  Part="1" 
AR Path="/6082BF78/6085597E" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 2450 3050 50  0001 C CNN
F 1 "+12V" H 2465 3373 50  0000 C CNN
F 2 "" H 2450 3200 50  0001 C CNN
F 3 "" H 2450 3200 50  0001 C CNN
	1    2450 3200
	1    0    0    -1  
$EndComp
Wire Wire Line
	3250 4000 3250 4350
Wire Wire Line
	5150 2850 7000 2850
Connection ~ 7000 2850
Wire Wire Line
	7000 2350 7000 2850
Wire Wire Line
	7000 2850 7000 3600
$Comp
L pspice:INDUCTOR L?
U 1 1 60855989
P 8000 2850
AR Path="/60855989" Ref="L?"  Part="1" 
AR Path="/6082BF78/60855989" Ref="L?"  Part="1" 
F 0 "L?" H 8000 3065 50  0000 C CNN
F 1 "22uH" H 8000 2974 50  0000 C CNN
F 2 "" H 8000 2850 50  0001 C CNN
F 3 "~" H 8000 2850 50  0001 C CNN
	1    8000 2850
	1    0    0    -1  
$EndComp
Wire Wire Line
	7000 2850 7750 2850
$Comp
L Device:CP C?
U 1 1 60855990
P 8650 3350
AR Path="/60855990" Ref="C?"  Part="1" 
AR Path="/6082BF78/60855990" Ref="C?"  Part="1" 
F 0 "C?" H 8768 3396 50  0000 L CNN
F 1 "470uF" H 8768 3305 50  0000 L CNN
F 2 "" H 8688 3200 50  0001 C CNN
F 3 "~" H 8650 3350 50  0001 C CNN
	1    8650 3350
	1    0    0    -1  
$EndComp
Wire Wire Line
	8250 2850 8650 2850
Wire Wire Line
	8650 2850 8650 3200
Wire Wire Line
	7000 4000 7000 4050
Wire Wire Line
	7000 4050 8650 4050
Connection ~ 7000 4050
Wire Wire Line
	7000 4050 7000 4100
Wire Wire Line
	8650 3500 8650 4050
Wire Wire Line
	8650 2850 9450 2850
Connection ~ 8650 2850
Text GLabel 9450 2850 2    50   Input ~ 0
DCDC_VOUT
$Comp
L Transistor_BJT:MMBT5551L Q?
U 1 1 608559A0
P 3150 2550
AR Path="/608559A0" Ref="Q?"  Part="1" 
AR Path="/6082BF78/608559A0" Ref="Q?"  Part="1" 
F 0 "Q?" H 3341 2596 50  0000 L CNN
F 1 "MMBT5551L" H 3341 2505 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 3350 2475 50  0001 L CIN
F 3 "www.onsemi.com/pub/Collateral/MMBT5550LT1-D.PDF" H 3150 2550 50  0001 L CNN
	1    3150 2550
	1    0    0    -1  
$EndComp
$Comp
L Transistor_BJT:MMBT5551L Q?
U 1 1 608559A6
P 3150 3800
AR Path="/608559A6" Ref="Q?"  Part="1" 
AR Path="/6082BF78/608559A6" Ref="Q?"  Part="1" 
F 0 "Q?" H 3341 3846 50  0000 L CNN
F 1 "MMBT5551L" H 3341 3755 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 3350 3725 50  0001 L CIN
F 3 "www.onsemi.com/pub/Collateral/MMBT5550LT1-D.PDF" H 3150 3800 50  0001 L CNN
	1    3150 3800
	1    0    0    -1  
$EndComp
Wire Wire Line
	3250 3600 6700 3600
Wire Wire Line
	6700 3600 6700 3800
Connection ~ 3250 3600
Wire Wire Line
	2350 2550 2550 2550
Wire Wire Line
	2900 2250 2900 2550
Wire Wire Line
	2900 1500 2900 1950
Wire Wire Line
	2450 1500 2900 1500
Wire Wire Line
	2900 1500 2950 1500
Connection ~ 2900 1500
Wire Wire Line
	3250 1850 3250 2150
$Comp
L Device:R R?
U 1 1 608559B6
P 3600 2150
AR Path="/608559B6" Ref="R?"  Part="1" 
AR Path="/6082BF78/608559B6" Ref="R?"  Part="1" 
F 0 "R?" V 3393 2150 50  0000 C CNN
F 1 "470R" V 3484 2150 50  0000 C CNN
F 2 "" V 3530 2150 50  0001 C CNN
F 3 "~" H 3600 2150 50  0001 C CNN
	1    3600 2150
	0    1    1    0   
$EndComp
Wire Wire Line
	3450 2150 3250 2150
Connection ~ 3250 2150
Wire Wire Line
	3250 2150 3250 2350
Wire Wire Line
	3750 2150 6700 2150
Wire Wire Line
	3250 1500 5150 1500
Wire Wire Line
	2850 3800 2900 3800
Wire Wire Line
	2900 3650 2900 3800
Connection ~ 2900 3800
Wire Wire Line
	2900 3800 2950 3800
Wire Wire Line
	2350 3800 2550 3800
Wire Wire Line
	2900 3350 2900 3200
Connection ~ 2900 3200
Wire Wire Line
	2900 3200 2450 3200
$Comp
L Amplifier_Operational:LM358 U?
U 1 1 608559C9
P 3550 5850
AR Path="/608559C9" Ref="U?"  Part="1" 
AR Path="/6082BF78/608559C9" Ref="U?"  Part="1" 
F 0 "U?" H 3550 6217 50  0000 C CNN
F 1 "LM358" H 3550 6126 50  0000 C CNN
F 2 "" H 3550 5850 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/lm2904-n.pdf" H 3550 5850 50  0001 C CNN
	1    3550 5850
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 608559CF
P 2250 5800
AR Path="/608559CF" Ref="R?"  Part="1" 
AR Path="/6082BF78/608559CF" Ref="R?"  Part="1" 
F 0 "R?" V 2043 5800 50  0000 C CNN
F 1 "0.007R" V 2134 5800 50  0000 C CNN
F 2 "" V 2180 5800 50  0001 C CNN
F 3 "~" H 2250 5800 50  0001 C CNN
	1    2250 5800
	-1   0    0    1   
$EndComp
$Comp
L Device:R R?
U 1 1 608559D5
P 2900 5950
AR Path="/608559D5" Ref="R?"  Part="1" 
AR Path="/6082BF78/608559D5" Ref="R?"  Part="1" 
F 0 "R?" V 2693 5950 50  0000 C CNN
F 1 "1k8" V 2784 5950 50  0000 C CNN
F 2 "" V 2830 5950 50  0001 C CNN
F 3 "~" H 2900 5950 50  0001 C CNN
	1    2900 5950
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R?
U 1 1 608559DB
P 2900 5650
AR Path="/608559DB" Ref="R?"  Part="1" 
AR Path="/6082BF78/608559DB" Ref="R?"  Part="1" 
F 0 "R?" V 2693 5650 50  0000 C CNN
F 1 "1k8" V 2784 5650 50  0000 C CNN
F 2 "" V 2830 5650 50  0001 C CNN
F 3 "~" H 2900 5650 50  0001 C CNN
	1    2900 5650
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R?
U 1 1 608559E1
P 3550 6300
AR Path="/608559E1" Ref="R?"  Part="1" 
AR Path="/6082BF78/608559E1" Ref="R?"  Part="1" 
F 0 "R?" V 3343 6300 50  0000 C CNN
F 1 "100k" V 3434 6300 50  0000 C CNN
F 2 "" V 3480 6300 50  0001 C CNN
F 3 "~" H 3550 6300 50  0001 C CNN
	1    3550 6300
	0    -1   -1   0   
$EndComp
Wire Wire Line
	2750 5650 2250 5650
Wire Wire Line
	2250 5950 2750 5950
Wire Wire Line
	3050 5950 3200 5950
Wire Wire Line
	3200 5950 3200 6300
Wire Wire Line
	3200 6300 3400 6300
Connection ~ 3200 5950
Wire Wire Line
	3200 5950 3250 5950
Wire Wire Line
	3700 6300 3850 6300
Wire Wire Line
	3850 5850 3850 6300
$Comp
L Device:R R?
U 1 1 608559F0
P 3500 5200
AR Path="/608559F0" Ref="R?"  Part="1" 
AR Path="/6082BF78/608559F0" Ref="R?"  Part="1" 
F 0 "R?" V 3293 5200 50  0000 C CNN
F 1 "100k" V 3384 5200 50  0000 C CNN
F 2 "" V 3430 5200 50  0001 C CNN
F 3 "~" H 3500 5200 50  0001 C CNN
	1    3500 5200
	0    -1   -1   0   
$EndComp
Wire Wire Line
	3200 5750 3200 5650
Wire Wire Line
	3200 5200 3350 5200
Wire Wire Line
	3200 5750 3250 5750
Wire Wire Line
	3050 5650 3200 5650
Connection ~ 3200 5650
Wire Wire Line
	3200 5650 3200 5200
Wire Wire Line
	3850 5200 3850 5350
Wire Wire Line
	3650 5200 3850 5200
$Comp
L power:GND #PWR?
U 1 1 608559FE
P 3850 5350
AR Path="/608559FE" Ref="#PWR?"  Part="1" 
AR Path="/6082BF78/608559FE" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 3850 5100 50  0001 C CNN
F 1 "GND" H 3855 5177 50  0000 C CNN
F 2 "" H 3850 5350 50  0001 C CNN
F 3 "" H 3850 5350 50  0001 C CNN
	1    3850 5350
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 60855A04
P 2250 6200
AR Path="/60855A04" Ref="#PWR?"  Part="1" 
AR Path="/6082BF78/60855A04" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 2250 5950 50  0001 C CNN
F 1 "GND" H 2255 6027 50  0000 C CNN
F 2 "" H 2250 6200 50  0001 C CNN
F 3 "" H 2250 6200 50  0001 C CNN
	1    2250 6200
	1    0    0    -1  
$EndComp
Wire Wire Line
	2250 6200 2250 5950
Connection ~ 2250 5950
Wire Wire Line
	2250 5650 2250 5350
Connection ~ 2250 5650
$Comp
L Amplifier_Operational:LM358 U?
U 2 1 60855A0E
P 4450 5950
AR Path="/60855A0E" Ref="U?"  Part="2" 
AR Path="/6082BF78/60855A0E" Ref="U?"  Part="2" 
F 0 "U?" H 4450 6317 50  0000 C CNN
F 1 "LM358" H 4450 6226 50  0000 C CNN
F 2 "" H 4450 5950 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/lm2904-n.pdf" H 4450 5950 50  0001 C CNN
	2    4450 5950
	1    0    0    -1  
$EndComp
Connection ~ 3850 5850
Wire Wire Line
	3850 5850 4150 5850
Wire Wire Line
	4150 6050 4100 6050
Wire Wire Line
	4100 6050 4100 6300
Wire Wire Line
	4100 6300 4750 6300
Wire Wire Line
	4750 6300 4750 5950
Wire Wire Line
	4750 5950 5000 5950
Connection ~ 4750 5950
Text GLabel 5000 5950 2    50   Input ~ 0
ADC_SOLAR_CURR
Text Notes 1450 800  0    50   ~ 0
Old design without Gate driver IC and Current measurement with a Shunt resistor instead of AC712
$EndSCHEMATC
