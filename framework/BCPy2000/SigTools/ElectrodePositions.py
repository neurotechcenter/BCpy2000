# -*- coding: utf-8 -*-
# 
#   $Id: ElectrodePositions.py 3445 2011-08-08 15:46:38Z jhill $
#   
#   This file is part of the BCPy2000 framework, a Python framework for
#   implementing modules that run on top of the BCI2000 <http://bci2000.org/>
#   platform, for the purpose of realtime biosignal processing.
# 
#   Copyright (C) 2007-11  Jeremy Hill, Thomas Schreiner,
#                          Christian Puzicha, Jason Farquhar
#   
#   bcpy2000@bci2000.org
#   
#   The BCPy2000 framework is free software: you can redistribute it
#   and/or modify it under the terms of the GNU General Public License
#   as published by the Free Software Foundation, either version 3 of
#   the License, or (at your option) any later version.
#
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
schemes = {
	'nose':           'NAS',
	'ears':           'A1 A2',
	'eyes_bipolar':   'EH1 EV1 EH2 EV2',
	'eyes_monopolar': 'EL1 EO1 EU1 EI1 EL2 EO2 EU2 EI2',
	'extended1020':   'Fp1 Fpz Fp2 AF7 AF3 AFz AF4 AF8 F9 F7 F5 F3 F1 Fz F2 F4 F6 F8 F10 FT9 FT7 FC5 FC3 FC1 FCz FC2 FC4 FC6 FT8 FT10 T9 T7 C5 C3 C1 Cz C2 C4 C6 T8 T10 TP9 TP7 CP5 CP3 CP1 CPz CP2 CP4 CP6 TP8 TP10 P9 P7 P5 P3 P1 Pz P2 P4 P6 P8 P10 PO9 PO7 PO3 POz PO4 PO8 PO10 O9 O1 Oz O2 O10 Iz',
	'biosemi64':      'A01 A02 A03 A04 A05 A06 A07 A08 A09 A10 A11 A12 A13 A14 A15 A16 A17 A18 A19 A20 A21 A22 A23 A24 A25 A26 A27 A28 A29 A30 A31 A32 B01 B02 B03 B04 B05 B06 B07 B08 B09 B10 B11 B12 B13 B14 B15 B16 B17 B18 B19 B20 B21 B22 B23 B24 B25 B26 B27 B28 B29 B30 B31 B32',
	'berlin118':      'Fp1 AFp1 Fpz AFp2 Fp2 AF7 AF3 AF4 AF8 FAF5 FAF1 FAF2 FAF6 F7 F5 F3 F1 Fz F2 F4 F6 F8 FFC7 FFC5 FFC3 FFC1 FFC2 FFC4 FFC6 FFC8 FT9 FT7 FC5 FC3 FC1 FCz FC2 FC4 FC6 FT8 FT10 CFC7 CFC5 CFC3 CFC1 CFC2 CFC4 CFC6 CFC8 T7 C5 C3 C1 Cz C2 C4 C6 T8 CCP7 CCP5 CCP3 CCP1 CCP2 CCP4 CCP6 CCP8 TP9 TP7 CP5 CP3 CP1 CPz CP2 CP4 CP6 TP8 TP10 PCP7 PCP5 PCP3 PCP1 PCP2 PCP4 PCP6 PCP8 P9 P7 P5 P3 P1 Pz P2 P4 P6 P8 P10 PPO7 PPO5 PPO1 PPO2 PPO6 PPO8 PO7 PO3 PO1 POz PO2 PO4 PO8 OPO1 OPO2 O1 Oz O2 OI1 OI2 I1 I2',
}
	
schematic2D = {

	'fp1' :   (  -0.26  ,  +0.78  ),
	'fpz' :   (   0.00  ,  +0.82  ),
	'fp2' :   (  +0.26  ,  +0.78  ),
	'fp1p':   (  -0.12  ,  +0.71  ),
	'fp2p':   (  +0.12  ,  +0.71  ),
	'f7a' :   (  -0.49  ,  +0.66  ),
	'af7' :   (  -0.49  ,  +0.66  ),
	'f5a' :   (  -0.40  ,  +0.55  ),
	'af5' :   (  -0.40  ,  +0.55  ),
	'f3a' :   (  -0.24  ,  +0.58  ),
	'af3' :   (  -0.24  ,  +0.58  ),
	'fza' :   (   0.00  ,  +0.61  ),
	'afz' :   (   0.00  ,  +0.61  ),
	'f1a' :   (  -0.10  ,  +0.52  ),
	'af1' :   (  -0.10  ,  +0.52  ),
	'f2a' :   (  +0.10  ,  +0.52  ),
	'af2' :   (  +0.10  ,  +0.52  ),
	'f4a' :   (  +0.24  ,  +0.58  ),
	'af4' :   (  +0.24  ,  +0.58  ),
	'f6a' :   (  +0.40  ,  +0.55  ),
	'f8a' :   (  +0.49  ,  +0.66  ),
	'af8' :   (  +0.49  ,  +0.66  ),
	'f9'  :   (  -0.81  ,  +0.58  ),
	'f7'  :   (  -0.67  ,  +0.48  ),
	'f5'  :   (  -0.51  ,  +0.44  ),
	'f3'  :   (  -0.34  ,  +0.41  ),
	'f1'  :   (  -0.16  ,  +0.40  ),
	'fz'  :   (   0.00  ,  +0.40  ),
	'f2'  :   (  +0.16  ,  +0.40  ),
	'f4'  :   (  +0.34  ,  +0.41  ),
	'f6'  :   (  +0.51  ,  +0.44  ),
	'f8'  :   (  +0.67  ,  +0.48  ),
	'f7p' :   (  -0.64  ,  +0.36  ),
	'f5p' :   (  -0.45  ,  +0.32  ),
	'f3p' :   (  -0.26  ,  +0.30  ),
	'f1p' :   (  -0.09  ,  +0.29  ),
	'f2p' :   (  +0.09  ,  +0.29  ),
	'f4p' :   (  +0.26  ,  +0.30  ),
	'f6p' :   (  +0.45  ,  +0.32  ),
	'f8p' :   (  +0.64  ,  +0.36  ),
	'f10' :   (  +0.81  ,  +0.58  ),
	'ft9' :   (  -0.95  ,  +0.32  ),
	'ft7' :   (  -0.78  ,  +0.26  ),
	'fc5' :   (  -0.59  ,  +0.23  ),
	'fc3' :   (  -0.39  ,  +0.22  ),
	'fc1' :   (  -0.19  ,  +0.20  ),
	'fcz' :   (   0.00  ,  +0.20  ),
	'fc2' :   (  +0.19  ,  +0.20  ),
	'fc4' :   (  +0.39  ,  +0.22  ),
	'fc6' :   (  +0.59  ,  +0.23  ),
	'ft8' :   (  +0.78  ,  +0.26  ),
	'ft10':   (  +0.95  ,  +0.32  ),
	'c7a' :   (  -0.71  ,  +0.13  ),
	'c5a' :   (  -0.51  ,  +0.12  ),
	'c3a' :   (  -0.30  ,  +0.11  ),
	'c1a' :   (  -0.10  ,  +0.10  ),
	'c2a' :   (  +0.10  ,  +0.10  ),
	'c4a' :   (  +0.30  ,  +0.11  ),
	'c6a' :   (  +0.51  ,  +0.12  ),
	'c8a' :   (  +0.71  ,  +0.13  ),
	't9'  :   (  -1.00  ,  +0.00  ),
	't7'  :   (  -0.82  ,  +0.00  ),
	'c5'  :   (  -0.62  ,  +0.00  ),
	'c3'  :   (  -0.42  ,  +0.00  ),
	'c1'  :   (  -0.21  ,  +0.00  ),
	'cz'  :   (   0.00  ,  +0.00  ),
	'c2'  :   (  +0.21  ,  +0.00  ),
	'c4'  :   (  +0.42  ,  +0.00  ),
	'c6'  :   (  +0.62  ,  +0.00  ),
	't8'  :   (  +0.82  ,  +0.00  ),
	't10' :   (  +1.00  ,  +0.00  ),
	'c7p' :   (  -0.71  ,  -0.11  ),
	'c5p' :   (  -0.50  ,  -0.10  ),
	'c3p' :   (  -0.30  ,  -0.09  ),
	'c1p' :   (  -0.10  ,  -0.09  ),
	'c2p' :   (  +0.10  ,  -0.09  ),
	'c4p' :   (  +0.30  ,  -0.09  ),
	'c6p' :   (  +0.50  ,  -0.10  ),
	'c8p' :   (  +0.71  ,  -0.11  ),
	'tp9' :   (  -0.95  ,  -0.32  ),
	'tp7' :   (  -0.78  ,  -0.26  ),
	'cp5' :   (  -0.59  ,  -0.23  ),
	'cp3' :   (  -0.39  ,  -0.22  ),
	'cp1' :   (  -0.19  ,  -0.20  ),
	'cpz' :   (   0.00  ,  -0.20  ),
	'cp2' :   (  +0.19  ,  -0.20  ),
	'cp4' :   (  +0.39  ,  -0.22  ),
	'cp6' :   (  +0.59  ,  -0.23  ),
	'tp8' :   (  +0.78  ,  -0.26  ),
	'tp10':   (  +0.95  ,  -0.32  ),
	'p7a' :   (  -0.65  ,  -0.34  ),
	'p5a' :   (  -0.45  ,  -0.32  ),
	'p3a' :   (  -0.27  ,  -0.29  ),
	'p1a' :   (  -0.09  ,  -0.29  ),
	'p2a' :   (  +0.09  ,  -0.29  ),
	'p4a' :   (  +0.27  ,  -0.29  ),
	'p6a' :   (  +0.45  ,  -0.32  ),
	'p8a' :   (  +0.65  ,  -0.34  ),
	'p9'  :   (  -0.81  ,  -0.58  ),
	'p7'  :   (  -0.67  ,  -0.48  ),
	'p5'  :   (  -0.51  ,  -0.44  ),
	'p3'  :   (  -0.34  ,  -0.41  ),
	'p1'  :   (  -0.16  ,  -0.40  ),
	'pz'  :   (   0.00  ,  -0.40  ),
	'p2'  :   (  +0.16  ,  -0.40  ),
	'p4'  :   (  +0.34  ,  -0.41  ),
	'p6'  :   (  +0.51  ,  -0.44  ),
	'p8'  :   (  +0.67  ,  -0.48  ),
	'p10' :   (  +0.81  ,  -0.58  ),
	'pzp' :   (   0.00  ,  -0.62  ),
	'o9a' :   (  -0.59  ,  -0.85  ),
	'po9' :   (  -0.59  ,  -0.81  ),
	'p9p' :   (  -0.61  ,  -0.72  ),
	'p7p' :   (  -0.49  ,  -0.66  ),
	'po7' :   (  -0.49  ,  -0.66  ),
	'p5p' :   (  -0.40  ,  -0.53  ),
	'p3p' :   (  -0.24  ,  -0.58  ),
	'po3' :   (  -0.24  ,  -0.58  ),
	'p1p' :   (  -0.10  ,  -0.52  ), 			  
	'pzp' :   (   0.00  ,  -0.61  ),
	'poz' :   (   0.00  ,  -0.61  ),
	'p2p' :   (  +0.10  ,  -0.52  ),
	'p4p' :   (  +0.24  ,  -0.58  ),
	'po4' :   (  +0.24  ,  -0.58  ),
	'p6p' :   (  +0.40  ,  -0.53  ),
	'p8p' :   (  +0.49  ,  -0.66  ),
	'po8' :   (  +0.49  ,  -0.66  ),
	'p10p':   (  +0.61  ,  -0.72  ),
	'o10a':   (  +0.59  ,  -0.85  ),
	'po10':   (  +0.59  ,  -0.81  ),
	'o1a' :   (  -0.12  ,  -0.72  ),
	'o2a' :   (  +0.12  ,  -0.72  ),
	'o9'  :   (  -0.33  ,  -0.95  ),
	'o1'  :   (  -0.26  ,  -0.78  ),
	'oz'  :   (   0.00  ,  -0.82  ),
	'o2'  :   (  +0.26  ,  -0.78  ),
	'o10' :   (  +0.33  ,  -0.95  ),
	'o1p' :   (  -0.14  ,  -0.90  ),
	'o2p' :   (  +0.14  ,  -0.90  ),
	'iz'  :   (   0.00  ,  -1.00  ),

	'el1' :   (  -0.33  ,  +1.15  ),
	'eo1' :   (  -0.55  ,  +1.05  ),
	'ei1' :   (  -0.11  ,  +1.05  ),
	'eu1' :   (  -0.33  ,  +1.01  ),
	'el2' :   (  +0.33  ,  +1.15  ),
	'eo2' :   (  +0.55  ,  +1.05  ),
	'ei2' :   (  +0.11  ,  +1.05  ),
	'eu2' :   (  +0.33  ,  +1.01  ),

	'eh1' :   (  -0.43  ,  +1.05  ),
	'ev1' :   (  -0.24  ,  +1.05  ),
	'eh2' :   (  +0.43  ,  +1.05  ),
	'ev2' :   (  +0.24  ,  +1.05  ),

	'a1'  :   (  -1.20  ,  +0.00  ),
	'a2'  :   (  +1.20  ,  +0.00  ),

	'laud'  :   (  +1.30  ,  +0.90  ),
	'audl'  :   (  +1.30  ,  +0.90  ),
	'raud'  :   (  +1.30  ,  +0.75  ),
	'audr'  :   (  +1.30  ,  +0.75  ),
	'vmrk'  :   (  +1.30  ,  +0.60  ),
}

s = schematic2D
s['a01'] = s['fp1']
s['a02'] = s['f7a']
s['a03'] = s['f3a']
s['a04'] = s['f1']
s['a05'] = s['f3']
s['a06'] = s['f5']
s['a07'] = s['f7']
s['a08'] = s['ft7']
s['a09'] = s['fc5']
s['a10'] = s['fc3']
s['a11'] = s['fc1']
s['a12'] = s['c1']
s['a13'] = s['c3']
s['a14'] = s['c5']
s['a15'] = s['t7']
s['a16'] = s['tp7']
s['a17'] = s['cp5']
s['a18'] = s['cp3']
s['a19'] = s['cp1']
s['a20'] = s['p1']
s['a21'] = s['p3']
s['a22'] = s['p5']
s['a23'] = s['p7']
s['a24'] = s['p9']
s['a25'] = s['p7p']
s['a26'] = s['p3p']
s['a27'] = s['o1']
s['a28'] = s['iz']
s['a29'] = s['oz']
s['a30'] = s['pzp']
s['a31'] = s['pz']
s['a32'] = s['cpz']
s['b01'] = s['fpz']
s['b02'] = s['fp2']
s['b03'] = s['f8a']
s['b04'] = s['f4a']
s['b05'] = s['fza']
s['b06'] = s['fz']
s['b07'] = s['f2']
s['b08'] = s['f4']
s['b09'] = s['f6']
s['b10'] = s['f8']
s['b11'] = s['ft8']
s['b12'] = s['fc6']
s['b13'] = s['fc4']
s['b14'] = s['fc2']
s['b15'] = s['fcz']
s['b16'] = s['cz']
s['b17'] = s['c2']
s['b18'] = s['c4']
s['b19'] = s['c6']
s['b20'] = s['t8']
s['b21'] = s['tp8']
s['b22'] = s['cp6']
s['b23'] = s['cp4']
s['b24'] = s['cp2']
s['b25'] = s['p2']
s['b26'] = s['p4']
s['b27'] = s['p6']
s['b28'] = s['p8']
s['b29'] = s['p10']
s['b30'] = s['p8p']
s['b31'] = s['p4p']
s['b32'] = s['o2']
