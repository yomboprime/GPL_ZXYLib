# @author yomboprime
#
#
#
#


# Configure here your path to Node js:
node = node


# All the source files:
srcFilesExampleSDWrite = src/textUtils.c src/esxdos.c examples/exampleSDWrite.c

srcFilesExampleSDRead = src/textUtils.c src/esxdos.c examples/exampleSDRead.c

srcFilesExampleSDList = src/textUtils.c src/esxdos.c examples/exampleSDList.c

srcFilesExampleSDStat = src/textUtils.c src/esxdos.c examples/exampleSDStat.c

srcFilesExampleSDSeek = src/textUtils.c src/esxdos.c examples/exampleSDSeek.c

srcFilesExampleFTPList = src/EspDrv/RingBuffer.c src/EspDrv/IPAddress.c src/EspDrv/EspDrv.c src/zxuno/zxuno.c src/esxdos.c src/textUtils.c src/zxuno/ftp.c src/zxuno/uart.c examples/exampleFTPList.c

srcFilesExampleFTPDownload = src/EspDrv/RingBuffer.c src/EspDrv/IPAddress.c src/EspDrv/EspDrv.c src/zxuno/zxuno.c src/textUtils.c src/esxdos.c src/zxuno/ftp.c src/zxuno/uart.c examples/exampleFTPDownload.c

srcFilesExampleHTTP = src/EspDrv/RingBuffer.c src/EspDrv/IPAddress.c src/EspDrv/EspDrv.c src/zxuno/zxuno.c src/textUtils.c src/zxuno/uart.c examples/exampleHTTP.c

srcFilesExampleUARTTerminal = src/textUtils.c src/esxdos.c src/zxuno/uart.c src/zxuno/zxuno.c examples/exampleUARTTerminal.c

srcFilesExampleTCPTerminal = src/EspDrv/RingBuffer.c src/EspDrv/IPAddress.c src/EspDrv/EspDrv.c src/textUtils.c src/zxuno/uart.c src/zxuno/zxuno.c examples/exampleTCPTerminal.c

srcFilesExampleWatch =  src/EspDrv/RingBuffer.c src/EspDrv/IPAddress.c src/EspDrv/EspDrv.c src/graphics.c src/textUtils.c src/zxuno/uart.c src/zxuno/zxuno.c examples/zxunowatch/zxunowatch.c

# All the targets:
all: generateBASICLoader createExample1 createExample2 createExample3 createExample4 createExample5 createExample6 createExample7 createExample8 createExample9 createExample10 createExample11
#all: generateBASICLoader createExample11


# Targets:

generateBASICLoader:
	./bas2tap/bas2tap -q -a10 -sGPL_ZXYLib ./cargadorBASIC/cargador.bas


#------------------------------------------------------------------------------
createExample1: compile1 createTAP1 concatenateTAPs1 generateWavLeches1

compile1:
	zcc +zx -o f1.bin -lndos $(srcFilesExampleSDWrite) > ultimolog.txt

createTAP1:
	$(node) ./bin2tap-js/bin2tap.js ../f1.bin > ultimolog.txt

concatenateTAPs1:
	cat ./cargadorBASIC/cargador.tap f1.tap > ExampleSDWrite.tap

generateWav1:
	tape2wav ./ExampleWriteSDTest.tap ./ExampleSDWrite.wav > ultimolog.txt

generateWavLeches1:
	./CgLeches ExampleSDWrite.tap ExampleSDWrite.wav 3 > ultimolog.txt

#------------------------------------------------------------------------------
createExample2: compile2 createTAP2 concatenateTAPs2 generateWavLeches2

compile2:
	zcc +zx -o f2.bin -lndos $(srcFilesExampleSDRead) > ultimolog.txt

createTAP2:
	$(node) ./bin2tap-js/bin2tap.js ../f2.bin > ultimolog.txt

concatenateTAPs2:
	cat ./cargadorBASIC/cargador.tap f2.tap > ExampleSDRead.tap

generateWav2:
	tape2wav ./ExampleSDRead.tap ./ExampleSDRead.wav > ultimolog.txt

generateWavLeches2:
	./CgLeches ExampleSDRead.tap ExampleSDRead.wav 3 > ultimolog.txt

#------------------------------------------------------------------------------
createExample3: compile3 createTAP3 concatenateTAPs3 generateWavLeches3

compile3:
	zcc +zx -o f3.bin -lndos $(srcFilesExampleSDList) > ultimolog.txt

createTAP3:
	$(node) ./bin2tap-js/bin2tap.js ../f3.bin > ultimolog.txt

concatenateTAPs3:
	cat ./cargadorBASIC/cargador.tap f3.tap > ExampleSDList.tap

generateWav3:
	tape2wav ./ExampleSDList.tap ./ExampleSDList.wav > ultimolog.txt

generateWavLeches3:
	./CgLeches ExampleSDList.tap ExampleSDList.wav 3 > ultimolog.txt

#------------------------------------------------------------------------------
createExample4: compile4 createTAP4 concatenateTAPs4 generateWavLeches4

compile4:
	zcc +zx -o f4.bin -lndos $(srcFilesExampleSDStat) > ultimolog.txt

createTAP4:
	$(node) ./bin2tap-js/bin2tap.js ../f4.bin > ultimolog.txt

concatenateTAPs4:
	cat ./cargadorBASIC/cargador.tap f4.tap > ExampleSDStat.tap

generateWav4:
	tape2wav ./ExampleSDStat.tap ./ExampleSDStat.wav > ultimolog.txt

generateWavLeches4:
	./CgLeches ExampleSDStat.tap ExampleSDStat.wav 3 > ultimolog.txt

#------------------------------------------------------------------------------
createExample5: compile5 createTAP5 concatenateTAPs5 generateWavLeches5

compile5:
	zcc +zx -o f5.bin -lndos $(srcFilesExampleFTPList) > ultimolog.txt

createTAP5:
	$(node) ./bin2tap-js/bin2tap.js ../f5.bin > ultimolog.txt

concatenateTAPs5:
	cat ./cargadorBASIC/cargador.tap f5.tap > ExampleFTPList.tap

generateWav5:
	tape2wav ./ExampleFTPList.tap ./ExampleFTPList.wav > ultimolog.txt

generateWavLeches5:
	./CgLeches ExampleFTPList.tap ExampleFTPList.wav 3 > ultimolog.txt
#------------------------------------------------------------------------------
createExample6: compile6 createTAP6 concatenateTAPs6 generateWavLeches6

compile6:
	zcc +zx -o f6.bin -lndos $(srcFilesExampleFTPDownload) > ultimolog.txt

createTAP6:
	$(node) ./bin2tap-js/bin2tap.js ../f6.bin > ultimolog.txt

concatenateTAPs6:
	cat ./cargadorBASIC/cargador.tap f6.tap > ExampleFTPDownload.tap

generateWav6:
	tape2wav ./ExampleFTPDownload.tap ./ExampleFTPDownload.wav > ultimolog.txt

generateWavLeches6:
	./CgLeches ExampleFTPDownload.tap ExampleFTPDownload.wav 3 > ultimolog.txt
#------------------------------------------------------------------------------
createExample7: compile7 createTAP7 concatenateTAPs7 generateWavLeches7

compile7:
	zcc +zx -o f7.bin -lndos $(srcFilesExampleHTTP) > ultimolog.txt

createTAP7:
	$(node) ./bin2tap-js/bin2tap.js ../f7.bin > ultimolog.txt

concatenateTAPs7:
	cat ./cargadorBASIC/cargador.tap f7.tap > ExampleHTTP.tap

generateWav7:
	tape2wav ./ExampleHTTP.tap ./ExampleHTTP.wav > ultimolog.txt

generateWavLeches7:
	./CgLeches ExampleHTTP.tap ExampleHTTP.wav 3 > ultimolog.txt

#------------------------------------------------------------------------------
createExample8: compile8 createTAP8 concatenateTAPs8 generateWavLeches8

compile8:
	zcc +zx -o f8.bin -lndos $(srcFilesExampleSDSeek) > ultimolog.txt

createTAP8:
	$(node) ./bin2tap-js/bin2tap.js ../f8.bin > ultimolog.txt

concatenateTAPs8:
	cat ./cargadorBASIC/cargador.tap f8.tap > ExampleSDSeek.tap

generateWav8:
	tape2wav ./ExampleSDSeek.tap ./ExampleSDSeek.wav > ultimolog.txt

generateWavLeches8:
	./CgLeches ExampleSDSeek.tap ExampleSDSeek.wav 3 > ultimolog.txt

#------------------------------------------------------------------------------
createExample9: compile9 createTAP9 concatenateTAPs9 generateWavLeches9

compile9:
	zcc +zx -o f9.bin -lndos $(srcFilesExampleUARTTerminal) > ultimolog.txt

createTAP9:
	$(node) ./bin2tap-js/bin2tap.js ../f9.bin > ultimolog.txt

concatenateTAPs9:
	cat ./cargadorBASIC/cargador.tap f9.tap > UARTTERM.tap

generateWav9:
	tape2wav ./UARTTERM.tap ./UARTTERM.wav > ultimolog.txt

generateWavLeches9:
	./CgLeches UARTTERM.tap UARTTERM.wav 3 > ultimolog.txt


#------------------------------------------------------------------------------

createExample10: compile10 createTAP10 concatenateTAPs10 generateWavLeches10

compile10:
	zcc +zx -o f10.bin -lndos $(srcFilesExampleTCPTerminal) > ultimolog.txt

createTAP10:
	$(node) ./bin2tap-js/bin2tap.js ../f10.bin > ultimolog.txt

concatenateTAPs10:
	cat ./cargadorBASIC/cargador.tap f10.tap > TCPTERM.tap

generateWav10:
	tape2wav ./TCPTERM.tap ./TCPTERM.wav > ultimolog.txt

generateWavLeches10:
	./CgLeches TCPTERM.tap TCPTERM.wav 3 > ultimolog.txt


#------------------------------------------------------------------------------


createExample11: generateBASICLoader11 createPalette11 compile11 createTAP11 concatenateTAPs11 generateWavLeches11

generateBASICLoader11:
	./bas2tap/bas2tap -q -a10 -swatch ./examples/zxunowatch/loader/loader.bas

createPalette11:
	$(node) ./tools/png2ulapluspalette/main.js ../../examples/zxunowatch/media/palette.png > ultimolog.txt

compile11:
	zcc +zx -o f11.bin -lndos $(srcFilesExampleWatch) > ultimolog.txt

createTAP11:
	$(node) ./bin2tap-js/bin2tap.js ../f11.bin > ultimolog.txt

concatenateTAPs11:
	cat ./examples/zxunowatch/loader/loader.tap ./examples/zxunowatch/media/timex.tap f11.tap > TIMEZX.tap

generateWav11:
	tape2wav ./TIMEZX.tap ./TIMEZX.wav > ultimolog.txt

generateWavLeches11:
	./CgLeches TIMEZX.tap TIMEZX.wav 3 > ultimolog.txt


#------------------------------------------------------------------------------

clean:
	$(RM) *.bin *.i *.op* *.o *.reloc *~ zcc_opt.def *.wav ./cargadorBASIC/cargador.tap
