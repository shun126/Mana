# Symbol Table
* id='typedef' type='struct { 0x00000008 bytes aligment 0x00000004 }' name='vec2' address=0x00000000
  * id='instance variable' type='float' name='y' address=0x00000004
  * id='instance variable' type='float' name='x' address=0x00000000
* id='typedef' type='struct { 0x0000000c bytes aligment 0x00000004 }' name='vec3' address=0x00000000
  * id='instance variable' type='float' name='z' address=0x00000008
  * id='instance variable' type='float' name='y' address=0x00000004
  * id='instance variable' type='float' name='x' address=0x00000000
* id='typedef' type='struct { 0x00000010 bytes aligment 0x00000004 }' name='vec4' address=0x00000000
  * id='instance variable' type='float' name='w' address=0x0000000c
  * id='instance variable' type='float' name='z' address=0x00000008
  * id='instance variable' type='float' name='y' address=0x00000004
  * id='instance variable' type='float' name='x' address=0x00000000
* id='typedef' type='struct { 0x0000000c bytes aligment 0x00000004 }' name='rotator' address=0x00000000
  * id='instance variable' type='float' name='roll' address=0x00000008
  * id='instance variable' type='float' name='yaw' address=0x00000004
  * id='instance variable' type='float' name='pitch' address=0x00000000
* id='typedef' type='struct { 0x00000010 bytes aligment 0x00000004 }' name='color' address=0x00000000
  * id='instance variable' type='float' name='a' address=0x0000000c
  * id='instance variable' type='float' name='b' address=0x00000008
  * id='instance variable' type='float' name='g' address=0x00000004
  * id='instance variable' type='float' name='r' address=0x00000000
* id='constant integer' type='int' name='Digit1' address=0x00000000
* id='constant integer' type='int' name='Digit2' address=0x00000000
* id='constant real' type='float' name='Real1' address=0x00000000
* id='constant real' type='float' name='Real2' address=0x00000000
* id='constant string' type='ref\<char\>' name='Hoge' address=0x00000000
* id='typedef' type='struct { 0x00000020 bytes aligment 0x00000008 }' name='TestStruct1' address=0x00000000
  * id='instance variable' type='actor { 0x00000008 bytes aligment 0x00000008 }' name='actorValue' address=0x00000018
  * id='instance variable' type='float' name='realValue' address=0x00000010
  * id='instance variable' type='ref\<char\>' name='stringValue' address=0x00000008
  * id='instance variable' type='int' name='digitValue' address=0x00000000
* id='typedef' type='struct { 0x00000004 bytes aligment 0x00000004 }' name='TestStruct2' address=0x00000000
  * id='instance variable' type='float' name='realValue' address=0x00000000
* id='global variable' type='float' name='realValue' address=0x00000410
* id='global variable' type='int' name='digitValue' address=0x00000414
* id='global variable' type='struct { 0x00000020 bytes aligment 0x00000008 }' name='structValue' address=0x00000008
  * id='instance variable' type='actor { 0x00000008 bytes aligment 0x00000008 }' name='actorValue' address=0x00000018
  * id='instance variable' type='float' name='realValue' address=0x00000010
  * id='instance variable' type='ref\<char\>' name='stringValue' address=0x00000008
  * id='instance variable' type='int' name='digitValue' address=0x00000000
* id='static variable' type='int' name='staticDigitValue' address=0x00000000
* id='static variable' type='float' name='staticRealValue' address=0x00000004
* id='native function' type='int' name='nativeFunction' address=0x00000000
  * id='local variable' type='int' name='a0' address=0x00000010
  * id='local variable' type='float' name='a1' address=0x0000000c
* id='function' type='int' name='callTest' address=0x00000000
  * id='local variable' type='char' name='a' address=0x00000013
  * id='local variable' type='short' name='b' address=0x00000012
  * id='local variable' type='int' name='c' address=0x00000010
  * id='local variable' type='float' name='d' address=0x0000000c
* id='function' type='void' name='assignmentTest' address=0x0000008a
  * id='local variable' type='float' name='value' address=0x0000000c
* id='function' type='void' name='expressionTest' address=0x0000014d
* id='function' type='void' name='statementTest' address=0x0000027f
* id='function' type='float' name='functionCallingTest' address=0x000003aa
* id='function' type='int' name='test_sort' address=0x000003d4
  * id='local variable' type='array[short, a]' name='array' address=0x00000020
  * id='local variable' type='int' name='size' address=0x0000000c
* id='typedef' type='actor { 0x00000004 bytes aligment 0x00000004 }' name='TestActor1' address=0x00000000
  * id='member function' type='void' name='talk' address=0x000006c6
  * id='member function' type='void' name='main' address=0x0000067b
  * id='member function' type='void' name='init' address=0x00000656
  * id='instance variable' type='int' name='digitVariableInActor' address=0x00000000
* id='typedef' type='module { 0x00000000 bytes aligment 0x00000008 }' name='Battler' address=0x00000000
* id='typedef' type='actor { 0x00000000 bytes aligment 0x00000004 }' name='OtherTestActor' address=0x00000000
  * id='member function' type='void' name='talk' address=0x000006f1
  * id='member function' type='void' name='dead' address=0x000006e6
  * id='member function' type='void' name='init' address=0x00000707
* id='typedef' type='actor { 0x00000000 bytes aligment 0x00000004 }' name='Phantom' address=0x00000000
  * id='member function' type='void' name='dead' address=0x000006e6
  * id='member function' type='void' name='init' address=0x00000707

# Code
```
callTest
0x00000000:allocate 20 byte(s)
0x00000005:save ra
0x00000006:load effective address 0x00000013 (frame)
0x0000000b:store (char)
0x0000000c:load effective address 0x00000012 (frame)
0x00000011:store (short)
0x00000012:load effective address 0x00000010 (frame)
0x00000017:store (int)
0x00000018:load effective address 0x0000000c (frame)
0x0000001d:store (float)
0x0000001e:load effective address 0x00000013 (frame)
0x00000023:load (char)
0x00000024:push 5 (char)
0x00000026:< (integer)
0x00000027:beq 0x00000078
0x0000002c:load effective address 0x0000000c (frame)
0x00000031:load (float)
0x00000032:load effective address 0x00000010 (frame)
0x00000037:load (int)
0x00000038:load effective address 0x00000012 (frame)
0x0000003d:load (short)
0x0000003e:load effective address 0x00000013 (frame)
0x00000043:load (char)
0x00000044:load effective address 0x00000000 (data)
0x00000049:print argc=5
0x0000004e:load effective address 0x0000000c (frame)
0x00000053:load (float)
0x00000054:load effective address 0x00000010 (frame)
0x00000059:load (int)
0x0000005a:load effective address 0x00000012 (frame)
0x0000005f:load (short)
0x00000060:load effective address 0x00000013 (frame)
0x00000065:load (char)
0x00000066:push 1 (char)
0x00000068:add (integer)
0x00000069:bsr 0x00000000
0x0000006e:bra 0x00000088
0x00000073:bra 0x00000088
0x00000078:bsr 0x000003aa
0x0000007d:bsr 0x0000014d
0x00000082:push zero (integer)
0x00000083:load ra
0x00000084:release  20 byte(s)
0x00000089:return from function
assignmentTest
0x0000008a:allocate 124 byte(s)
0x0000008f:save ra
0x00000090:load effective address 0x0000000c (frame)
0x00000095:store (float)
0x00000096:load effective address 0x0000000d (data)
0x0000009b:print argc=1
0x000000a0:push 24 (char)
0x000000a2:load effective address 0x0000000d (frame)
0x000000a7:store (char)
0x000000a8:push 1.500000 (float)
0x000000ad:push 16 (size)
0x000000b2:push 1 (char)
0x000000b4:push 32 (size)
0x000000b9:mul (integer)
0x000000ba:load effective address 0x00000050 (frame)
0x000000bf:add (integer)
0x000000c0:add (integer)
0x000000c1:store (float)
0x000000c2:push zero (float)
0x000000c3:push 0 (size)
0x000000c8:load effective address 0x0000005c (frame)
0x000000cd:add (integer)
0x000000ce:store (float)
0x000000cf:push 1.000000 (float)
0x000000d4:push 4 (size)
0x000000d9:load effective address 0x0000005c (frame)
0x000000de:add (integer)
0x000000df:store (float)
0x000000e0:push 2.000000 (float)
0x000000e5:push 8 (size)
0x000000ea:load effective address 0x0000005c (frame)
0x000000ef:add (integer)
0x000000f0:store (float)
0x000000f1:push 1 (char)
0x000000f3:load effective address 0x0000005d (frame)
0x000000f8:store (char)
0x000000f9:push 65535 (integer)
0x000000fe:load effective address 0x00000060 (frame)
0x00000103:store (short)
0x00000104:push 16777215 (integer)
0x00000109:load effective address 0x00000064 (frame)
0x0000010e:store (int)
0x0000010f:push 4.000000 (float)
0x00000114:load effective address 0x00000068 (frame)
0x00000119:store (float)
0x0000011a:push 5.000000 (float)
0x0000011f:load effective address 0x00000068 (frame)
0x00000124:store (float)
0x00000125:load effective address 0x0000005d (frame)
0x0000012a:load (char)
0x0000012b:cast (float)
0x0000012c:load effective address 0x00000068 (frame)
0x00000131:store (float)
0x00000132:load effective address 0x0000007c (frame)
0x00000137:load (data) 20 byte(s)
0x0000013c:load effective address 0x0000007c (frame)
0x00000141:store (data) 20 byte(s)
0x00000146:load ra
0x00000147:release  124 byte(s)
0x0000014c:return from function
expressionTest
0x0000014d:allocate 16 byte(s)
0x00000152:save ra
0x00000153:load effective address 0x0000001d (data)
0x00000158:print argc=1
0x0000015d:load effective address 0x0000000c (frame)
0x00000162:load (float)
0x00000163:push 1.000000 (float)
0x00000168:add (float)
0x00000169:load effective address 0x0000000c (frame)
0x0000016e:store (float)
0x0000016f:load effective address 0x0000000c (frame)
0x00000174:load (float)
0x00000175:push 1.000000 (float)
0x0000017a:sub (float)
0x0000017b:load effective address 0x0000000c (frame)
0x00000180:store (float)
0x00000181:load effective address 0x0000000c (frame)
0x00000186:load (float)
0x00000187:push 1.000000 (float)
0x0000018c:mul (float)
0x0000018d:load effective address 0x0000000c (frame)
0x00000192:store (float)
0x00000193:load effective address 0x0000000c (frame)
0x00000198:load (float)
0x00000199:push 1.000000 (float)
0x0000019e:div (float)
0x0000019f:load effective address 0x0000000c (frame)
0x000001a4:store (float)
0x000001a5:load effective address 0x0000000c (frame)
0x000001aa:load (float)
0x000001ab:push 1.000000 (float)
0x000001b0:mod (float)
0x000001b1:load effective address 0x0000000c (frame)
0x000001b6:store (float)
0x000001b7:load effective address 0x00000010 (frame)
0x000001bc:load (int)
0x000001bd:push 1 (char)
0x000001bf:add (integer)
0x000001c0:load effective address 0x00000010 (frame)
0x000001c5:store (int)
0x000001c6:load effective address 0x00000010 (frame)
0x000001cb:load (int)
0x000001cc:push 1 (char)
0x000001ce:sub (integer)
0x000001cf:load effective address 0x00000010 (frame)
0x000001d4:store (int)
0x000001d5:load effective address 0x00000010 (frame)
0x000001da:load (int)
0x000001db:push 1 (char)
0x000001dd:mul (integer)
0x000001de:load effective address 0x00000010 (frame)
0x000001e3:store (int)
0x000001e4:load effective address 0x00000010 (frame)
0x000001e9:load (int)
0x000001ea:push 1 (char)
0x000001ec:div (integer)
0x000001ed:load effective address 0x00000010 (frame)
0x000001f2:store (int)
0x000001f3:load effective address 0x00000010 (frame)
0x000001f8:load (int)
0x000001f9:push 1 (char)
0x000001fb:mod (integer)
0x000001fc:load effective address 0x00000010 (frame)
0x00000201:store (int)
0x00000202:load effective address 0x00000010 (frame)
0x00000207:load (int)
0x00000208:push 1 (char)
0x0000020a:and
0x0000020b:load effective address 0x00000010 (frame)
0x00000210:store (int)
0x00000211:load effective address 0x00000010 (frame)
0x00000216:load (int)
0x00000217:push 1 (char)
0x00000219:or
0x0000021a:load effective address 0x00000010 (frame)
0x0000021f:store (int)
0x00000220:load effective address 0x00000010 (frame)
0x00000225:load (int)
0x00000226:push 1 (char)
0x00000228:eor
0x00000229:load effective address 0x00000010 (frame)
0x0000022e:store (int)
0x0000022f:load effective address 0x00000010 (frame)
0x00000234:load (int)
0x00000235:push 1 (char)
0x00000237:halt
0x00000238:load effective address 0x00000010 (frame)
0x0000023d:store (int)
0x0000023e:load effective address 0x00000010 (frame)
0x00000243:load (int)
0x00000244:push 1 (char)
0x00000246:or
0x00000247:load effective address 0x00000010 (frame)
0x0000024c:store (int)
0x0000024d:load effective address 0x00000010 (frame)
0x00000252:load (int)
0x00000253:~
0x00000254:load effective address 0x00000010 (frame)
0x00000259:store (int)
0x0000025a:load effective address 0x00000010 (frame)
0x0000025f:load (int)
0x00000260:push 1 (char)
0x00000262:shift left
0x00000263:load effective address 0x00000010 (frame)
0x00000268:store (int)
0x00000269:load effective address 0x00000010 (frame)
0x0000026e:load (int)
0x0000026f:push 1 (char)
0x00000271:shift right
0x00000272:load effective address 0x00000010 (frame)
0x00000277:store (int)
0x00000278:load ra
0x00000279:release  16 byte(s)
0x0000027e:return from function
statementTest
0x0000027f:allocate 20 byte(s)
0x00000284:save ra
0x00000285:load effective address 0x0000002d (data)
0x0000028a:print argc=1
0x0000028f:push 1 (char)
0x00000291:beq 0x0000029d
0x00000296:push zero (integer)
0x00000297:load effective address 0x00000010 (frame)
0x0000029c:store (int)
0x0000029d:push zero (integer)
0x0000029e:beq 0x000002af
0x000002a3:push zero (float)
0x000002a4:load effective address 0x00000010 (frame)
0x000002a9:store (float)
0x000002aa:bra 0x000002b6
0x000002af:push zero (integer)
0x000002b0:load effective address 0x0000000e (frame)
0x000002b5:store (short)
0x000002b6:push 4 (char)
0x000002b8:load effective address 0x0000000c (frame)
0x000002bd:store (int)
0x000002be:load effective address 0x0000000c (frame)
0x000002c3:load (int)
0x000002c4:beq 0x0000031d
0x000002c9:load effective address 0x0000000c (frame)
0x000002ce:load (int)
0x000002cf:bra 0x000002e8
0x000002d4:bra 0x00000308
0x000002d9:bra 0x00000308
0x000002de:bra 0x00000308
0x000002e3:bra 0x00000308
0x000002e8:duplicate
0x000002e9:push 3 (char)
0x000002eb:== (integer)
0x000002ec:bne 0x000002d4
0x000002f1:duplicate
0x000002f2:push 2 (char)
0x000002f4:== (integer)
0x000002f5:bne 0x000002d9
0x000002fa:duplicate
0x000002fb:push 1 (char)
0x000002fd:== (integer)
0x000002fe:bne 0x000002de
0x00000303:bra 0x000002d4
0x00000308:remove
0x00000309:load effective address 0x0000000c (frame)
0x0000030e:load (int)
0x0000030f:push 1 (char)
0x00000311:sub (integer)
0x00000312:load effective address 0x0000000c (frame)
0x00000317:store (int)
0x00000318:bra 0x000002be
0x0000031d:push 10 (char)
0x0000031f:load effective address 0x00000010 (frame)
0x00000324:store (int)
0x00000325:load effective address 0x00000010 (frame)
0x0000032a:load (int)
0x0000032b:beq 0x00000344
0x00000330:load effective address 0x00000010 (frame)
0x00000335:load (int)
0x00000336:push 1 (char)
0x00000338:sub (integer)
0x00000339:load effective address 0x00000010 (frame)
0x0000033e:store (int)
0x0000033f:bra 0x00000325
0x00000344:load effective address 0x00000010 (frame)
0x00000349:load (int)
0x0000034a:push 1 (char)
0x0000034c:add (integer)
0x0000034d:load effective address 0x00000010 (frame)
0x00000352:store (int)
0x00000353:load effective address 0x00000010 (frame)
0x00000358:load (int)
0x00000359:push 10 (char)
0x0000035b:< (integer)
0x0000035c:bne 0x00000344
0x00000361:push zero (integer)
0x00000362:load effective address 0x00000014 (frame)
0x00000367:store (int)
0x00000368:load effective address 0x00000014 (frame)
0x0000036d:load (int)
0x0000036e:push 10 (char)
0x00000370:< (integer)
0x00000371:beq 0x00000399
0x00000376:load effective address 0x00000010 (frame)
0x0000037b:load (int)
0x0000037c:push 1 (char)
0x0000037e:sub (integer)
0x0000037f:load effective address 0x00000010 (frame)
0x00000384:store (int)
0x00000385:load effective address 0x00000014 (frame)
0x0000038a:load (int)
0x0000038b:push 1 (char)
0x0000038d:add (integer)
0x0000038e:load effective address 0x00000014 (frame)
0x00000393:store (int)
0x00000394:bra 0x00000368
0x00000399:bra 0x000003a3
0x0000039e:bra 0x0000031d
0x000003a3:load ra
0x000003a4:release  20 byte(s)
0x000003a9:return from function
functionCallingTest
0x000003aa:allocate 12 byte(s)
0x000003af:save ra
0x000003b0:load effective address 0x0000003c (data)
0x000003b5:print argc=1
0x000003ba:load effective address 0x0000000c (frame)
0x000003bf:load (int)
0x000003c0:cast (float)
0x000003c1:bsr 0x0000008a
0x000003c6:load effective address 0x0000000c (frame)
0x000003cb:load (int)
0x000003cc:cast (float)
0x000003cd:load ra
0x000003ce:release  12 byte(s)
0x000003d3:return from function
test_sort
0x000003d4:allocate 48 byte(s)
0x000003d9:save ra
0x000003da:load effective address 0x00000020 (frame)
0x000003df:store (data) 20 byte(s)
0x000003e4:load effective address 0x0000000c (frame)
0x000003e9:store (int)
0x000003ea:load effective address 0x00000051 (data)
0x000003ef:print argc=1
0x000003f4:push zero (integer)
0x000003f5:load effective address 0x0000002c (frame)
0x000003fa:store (int)
0x000003fb:load effective address 0x0000002c (frame)
0x00000400:load (int)
0x00000401:load effective address 0x0000000c (frame)
0x00000406:load (int)
0x00000407:< (integer)
0x00000408:beq 0x00000464
0x0000040d:load effective address 0x0000000c (frame)
0x00000412:load (int)
0x00000413:load effective address 0x0000002c (frame)
0x00000418:load (int)
0x00000419:sub (integer)
0x0000041a:load effective address 0x0000002c (frame)
0x0000041f:load (int)
0x00000420:push 2 (size)
0x00000425:mul (integer)
0x00000426:load effective address 0x00000020 (frame)
0x0000042b:add (integer)
0x0000042c:store (short)
0x0000042d:load effective address 0x0000002c (frame)
0x00000432:load (int)
0x00000433:push 2 (size)
0x00000438:mul (integer)
0x00000439:load effective address 0x00000020 (frame)
0x0000043e:add (integer)
0x0000043f:load (short)
0x00000440:load effective address 0x0000002c (frame)
0x00000445:load (int)
0x00000446:load effective address 0x00000059 (data)
0x0000044b:print argc=3
0x00000450:load effective address 0x0000002c (frame)
0x00000455:load (int)
0x00000456:push 1 (char)
0x00000458:add (integer)
0x00000459:load effective address 0x0000002c (frame)
0x0000045e:store (int)
0x0000045f:bra 0x000003fb
0x00000464:load effective address 0x00000061 (data)
0x00000469:print argc=1
0x0000046e:push zero (integer)
0x0000046f:load effective address 0x00000024 (frame)
0x00000474:store (int)
0x00000475:push 1 (char)
0x00000477:load effective address 0x00000028 (frame)
0x0000047c:store (int)
0x0000047d:push zero (integer)
0x0000047e:load effective address 0x0000002c (frame)
0x00000483:store (int)
0x00000484:load effective address 0x0000002c (frame)
0x00000489:load (int)
0x0000048a:load effective address 0x0000000c (frame)
0x0000048f:load (int)
0x00000490:push 1 (char)
0x00000492:sub (integer)
0x00000493:< (integer)
0x00000494:beq 0x00000574
0x00000499:load effective address 0x0000002c (frame)
0x0000049e:load (int)
0x0000049f:push 2 (size)
0x000004a4:mul (integer)
0x000004a5:load effective address 0x00000020 (frame)
0x000004aa:add (integer)
0x000004ab:load (short)
0x000004ac:load effective address 0x0000002c (frame)
0x000004b1:load (int)
0x000004b2:push 1 (char)
0x000004b4:add (integer)
0x000004b5:push 2 (size)
0x000004ba:mul (integer)
0x000004bb:load effective address 0x00000020 (frame)
0x000004c0:add (integer)
0x000004c1:load (short)
0x000004c2:> (integer)
0x000004c3:beq 0x00000560
0x000004c8:load effective address 0x0000002c (frame)
0x000004cd:load (int)
0x000004ce:push 1 (char)
0x000004d0:add (integer)
0x000004d1:push 2 (size)
0x000004d6:mul (integer)
0x000004d7:load effective address 0x00000020 (frame)
0x000004dc:add (integer)
0x000004dd:load (short)
0x000004de:load effective address 0x0000002c (frame)
0x000004e3:load (int)
0x000004e4:push 2 (size)
0x000004e9:mul (integer)
0x000004ea:load effective address 0x00000020 (frame)
0x000004ef:add (integer)
0x000004f0:load (short)
0x000004f1:load effective address 0x00000068 (data)
0x000004f6:print argc=3
0x000004fb:load effective address 0x0000002c (frame)
0x00000500:load (int)
0x00000501:push 2 (size)
0x00000506:mul (integer)
0x00000507:load effective address 0x00000020 (frame)
0x0000050c:add (integer)
0x0000050d:load (short)
0x0000050e:load effective address 0x0000002e (frame)
0x00000513:store (short)
0x00000514:load effective address 0x0000002c (frame)
0x00000519:load (int)
0x0000051a:push 1 (char)
0x0000051c:add (integer)
0x0000051d:push 2 (size)
0x00000522:mul (integer)
0x00000523:load effective address 0x00000020 (frame)
0x00000528:add (integer)
0x00000529:load (short)
0x0000052a:load effective address 0x0000002c (frame)
0x0000052f:load (int)
0x00000530:push 2 (size)
0x00000535:mul (integer)
0x00000536:load effective address 0x00000020 (frame)
0x0000053b:add (integer)
0x0000053c:store (short)
0x0000053d:load effective address 0x0000002e (frame)
0x00000542:load (short)
0x00000543:load effective address 0x0000002c (frame)
0x00000548:load (int)
0x00000549:push 1 (char)
0x0000054b:add (integer)
0x0000054c:push 2 (size)
0x00000551:mul (integer)
0x00000552:load effective address 0x00000020 (frame)
0x00000557:add (integer)
0x00000558:store (short)
0x00000559:push zero (integer)
0x0000055a:load effective address 0x00000028 (frame)
0x0000055f:store (int)
0x00000560:load effective address 0x0000002c (frame)
0x00000565:load (int)
0x00000566:push 1 (char)
0x00000568:add (integer)
0x00000569:load effective address 0x0000002c (frame)
0x0000056e:store (int)
0x0000056f:bra 0x00000484
0x00000574:load effective address 0x00000024 (frame)
0x00000579:load (int)
0x0000057a:push 1 (char)
0x0000057c:add (integer)
0x0000057d:load effective address 0x00000024 (frame)
0x00000582:store (int)
0x00000583:push zero (integer)
0x00000584:load effective address 0x0000002c (frame)
0x00000589:store (int)
0x0000058a:load effective address 0x0000002c (frame)
0x0000058f:load (int)
0x00000590:load effective address 0x0000000c (frame)
0x00000595:load (int)
0x00000596:< (integer)
0x00000597:beq 0x000005d3
0x0000059c:load effective address 0x0000002c (frame)
0x000005a1:load (int)
0x000005a2:push 2 (size)
0x000005a7:mul (integer)
0x000005a8:load effective address 0x00000020 (frame)
0x000005ad:add (integer)
0x000005ae:load (short)
0x000005af:load effective address 0x0000002c (frame)
0x000005b4:load (int)
0x000005b5:load effective address 0x00000059 (data)
0x000005ba:print argc=3
0x000005bf:load effective address 0x0000002c (frame)
0x000005c4:load (int)
0x000005c5:push 1 (char)
0x000005c7:add (integer)
0x000005c8:load effective address 0x0000002c (frame)
0x000005cd:store (int)
0x000005ce:bra 0x0000058a
0x000005d3:load effective address 0x00000024 (frame)
0x000005d8:load (int)
0x000005d9:load effective address 0x00000073 (data)
0x000005de:print argc=2
0x000005e3:load effective address 0x00000028 (frame)
0x000005e8:load (int)
0x000005e9:!
0x000005ea:bne 0x00000475
0x000005ef:load effective address 0x0000007b (data)
0x000005f4:print argc=1
0x000005f9:push zero (integer)
0x000005fa:load effective address 0x0000002c (frame)
0x000005ff:store (int)
0x00000600:load effective address 0x0000002c (frame)
0x00000605:load (int)
0x00000606:load effective address 0x0000000c (frame)
0x0000060b:load (int)
0x0000060c:< (integer)
0x0000060d:beq 0x00000649
0x00000612:load effective address 0x0000002c (frame)
0x00000617:load (int)
0x00000618:push 2 (size)
0x0000061d:mul (integer)
0x0000061e:load effective address 0x00000020 (frame)
0x00000623:add (integer)
0x00000624:load (short)
0x00000625:load effective address 0x0000002c (frame)
0x0000062a:load (int)
0x0000062b:load effective address 0x00000059 (data)
0x00000630:print argc=3
0x00000635:load effective address 0x0000002c (frame)
0x0000063a:load (int)
0x0000063b:push 1 (char)
0x0000063d:add (integer)
0x0000063e:load effective address 0x0000002c (frame)
0x00000643:store (int)
0x00000644:bra 0x00000600
0x00000649:load effective address 0x00000024 (frame)
0x0000064e:load (int)
0x0000064f:load ra
0x00000650:release  48 byte(s)
0x00000655:return from function
TestActor1::init
0x00000656:allocate 8 byte(s)
0x0000065b:load effective address 0x00000082 (data)
0x00000660:print argc=1
0x00000665:push 4.000000 (float)
0x0000066a:push 3 (char)
0x0000066c:push 2 (char)
0x0000066e:push 1 (char)
0x00000670:bsr 0x00000000
0x00000675:release  8 byte(s)
0x0000067a:return from action
TestActor1::main
0x0000067b:allocate 28 byte(s)
0x00000680:load effective address 0x00000094 (data)
0x00000685:print argc=1
0x0000068a:push 10 (char)
0x0000068c:load effective address 0x0000001c (frame)
0x00000691:load (data) 20 byte(s)
0x00000696:bsr 0x000003d4
0x0000069b:push actor 'OtherTestActor'
0x000006a0:req 'talk'
0x000006a5:load effective address 0x000000a6 (data)
0x000006aa:print argc=1
0x000006af:push actor 'OtherTestActor'
0x000006b4:push zero (integer)
0x000006b5:join
0x000006b6:load effective address 0x000000bb (data)
0x000006bb:print argc=1
0x000006c0:release  28 byte(s)
0x000006c5:return from action
TestActor1::talk
0x000006c6:allocate 16 byte(s)
0x000006cb:load effective address 0x000000d0 (data)
0x000006d0:print argc=1
0x000006d5:release  16 byte(s)
0x000006da:return from action
0x000006db:allocate 8 byte(s)
0x000006e0:release  8 byte(s)
0x000006e5:return from function
OtherTestActor::dead
0x000006e6:allocate 8 byte(s)
0x000006eb:release  8 byte(s)
0x000006f0:return from function
OtherTestActor::talk
0x000006f1:allocate 20 byte(s)
0x000006f6:load effective address 0x000000da (data)
0x000006fb:print argc=1
0x00000700:yield
0x00000701:release  20 byte(s)
0x00000706:return from action
OtherTestActor::init
0x00000707:allocate 8 byte(s)
0x0000070c:release  8 byte(s)
0x00000711:return from action
```
