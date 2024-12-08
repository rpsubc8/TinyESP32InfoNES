# Tiny ESP32 InfoNES
Port Jay Kumogata's SDL InfoNES x86 emulator to the ESP32 (TTGO VGA32 v1.x).<br>

Version under development (Not recommended for use).<br>
<ul>
 <li>VStudio v1.67.2 PlatformIO v3.3.3 Espressif32 v3.3.2</li>
 <li>VGA 320x240</li>
 <li>UART keyboard</li>
</ul>

Remote keyboard UART (115200 bauds):
<ul> 
 <li>A - SELECT</li>
 <li>S - START</li>
 <li>X - Button A</li>
 <li>Z - Button B</li>
 <li>8 - UP</li>
 <li>2 - DOWN</li>
 <li>4 - LEFT</li>
 <li>6 - RIGHT</li>
</ul>

Mapper:
<ul>
 <li>iNES Mapper: #0, #1, #2, #3, #4, #5, #6, #7, #8, #9, #10, #11, #13, #15, #16, #17, #18, #19, #21, #22,</li>
 <li>iNES Mapper: #23, #24, #25, #26, #32, #33, #34, #40, #41, #42, #43, #44, #45, #46, #47, #48, #49, #50, #51, #57,</li>
 <li>iNES Mapper: #58, #60, #61, #62, #64, #65, #66, #67, #68, #69, #70, #71, #72, #73, #74, #75, #76, #77, #78, #79,</li>
 <li>iNES Mapper: #80, #82, #83, #85, #86, #87, #88, #89, #90, #91, #92, #93, #94, #95, #96, #97, #99,#100,#101,#105,</li>
 <li>iNES Mapper: #107,#108,#109,#110,#112,#113,#114,#115,#116,#117,#118,#119,#120,#122,#133,#134,#135,#140,#151,#160,</li>
 <li>iNES Mapper: #180,#181,#182,#183,#185,#187,#188,#189,#191,#193,#194,#200,#201,#202,#222,#225,#226,#227,#228,#229,</li>
 <li>iNES Mapper: #230,#231,#232,#233,#234,#235,#236,#240,#241,#242,#243,#244,#245,#246,#248,#249,#251,#252,#255</li>
 <li>Nintendo MMC: MMC1(#1), MMC2(#9), MMC3(#4), MMC4(#10), MMC5(#5)</li>
</ul>

Mapper 5, 6, 19, 45, 85, 185, 188 and 235 require PSRAM.
