# @author yomboprime
#
#
#
#


# Configure here your path to Node js:
node = /media/datos/soft/nodejs/node-v4.4.5-linux-x64/bin/node


# All the source files:
srcFilesExampleSDWrite = src/textUtils.c src/esxdos.c examples/exampleSDWrite.c

srcFilesExampleSDRead = src/textUtils.c src/esxdos.c examples/exampleSDRead.c

srcFilesExampleSDList = src/textUtils.c src/esxdos.c examples/exampleSDList.c

srcFilesExampleSDStat = src/textUtils.c src/esxdos.c examples/exampleSDStat.c

srcFilesExampleSDSeek = src/textUtils.c src/esxdos.c examples/exampleSDSeek.c

srcFilesExampleFTPList = src/EspDrv/RingBuffer.c src/EspDrv/IPAddress.c src/EspDrv/EspDrv.c src/zxuno/zxuno.c src/esxdos.c src/textUtils.c src/zxuno/ftp.c src/zxuno/uart.c examples/exampleFTPList.c

srcFilesExampleFTPDownload = src/EspDrv/RingBuffer.c src/EspDrv/IPAddress.c src/EspDrv/EspDrv.c src/zxuno/zxuno.c src/textUtils.c src/esxdos.c src/zxuno/ftp.c src/zxuno/uart.c examples/exampleFTPDownload.c

srcFilesExampleHTTP = src/EspDrv/RingBuffer.c src/EspDrv/IPAddress.c src/EspDrv/EspDrv.c src/zxuno/zxuno.c src/textUtils.c src/zxuno/uart.c examples/exampleHTTP.c

# All the targets:
all: generateBASICLoader createExample1 createExample2 createExample3 createExample4 createExample5 createExample6 createExample7 createExample8


# Targets:

generateBASICLoader:
	./bas2tap/bas2tap -q -a10 -sftpUno ./cargadorBASIC/cargador.bas


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

clean:
	$(RM) *.bin *.i *.op* *.o *.reloc *~ zcc_opt.def *.wav ./cargadorBASIC/cargador.tap
