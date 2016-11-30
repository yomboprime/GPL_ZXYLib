/*
    This file is part of GPL_ZXYLib

    GPL_ZXYLib is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <input.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <spectrum.h>
#include "../../src/textUtils.h"
#include "../../src/graphics.h"
#include "watchGraphicsDef.h"
#include "media/palettes.h"

//#include "../src/EspDrv/EspDrv.h"
//#include "../src/EspDrv/IPAddress.h"

#define DIGIT_0 0
#define DIGIT_1 1
#define DIGIT_2 2
#define DIGIT_3 3
#define DIGIT_4 4
#define DIGIT_5 5
#define DIGIT_6 6
#define DIGIT_7 7
#define DIGIT_8 8
#define DIGIT_9 9
#define DIGIT_F 10
#define DIGIT_R 11
#define DIGIT_M 12
#define DIGIT_O 13
#define DIGIT_A 14
#define DIGIT_U 15
#define DIGIT_T 16
#define DIGIT_H 17
#define DIGIT_W 18
#define DIGIT_E 19
#define DIGIT_S 5
#define DIGIT_SPACE 20
#define DIGIT_BIG_SPACE 10

#define getKey() in_Inkey()

void waitKey() {
    while (getKey());
    while (!getKey());
}

#define BUFFER_SIZE 128
uint8_t theBuffer[ BUFFER_SIZE ];


// Function prototypes

void setPalette();
void paintHourSeconds( uint8_t *buffer );
void paintUpperDigits( uint8_t *buffer );
void paintAlarmFlags( uint8_t flags );



#if 0

void mainServer(void) {

    // Time service
    char* host = "india.colorado.edu";
    uint16_t serverPort = 13;

    // Wifi name:
    char *ssid = "apoint";
    // Wifi password:
    char*ssidpassw = "apoint123";

    uint8_t connected = false;
    IPAddress localIP[ 4 ];

    bool connClosed;
    uint8_t returnCode;

    uint8_t c = 0;
    uint8_t *pbuf;

    uint8_t key;

    textUtils_64ColumnsMode();
    textUtils_cls();

    textUtils_println( "This example is a 'TCP terminal'. Press a key to start." );

    waitKey();

    textUtils_println( "Initing WiFi chip..." );

    if ( EspDrv_wifiDriverInit() == true ) {

        textUtils_println( "WiFi module inited OK." );

    } else {

        textUtils_println( "Couldn't communicate with the WiFi module. Press key to exit." );

        waitKey();

        exit(1);

    }

    textUtils_print( "Trying to connect to net " );
    textUtils_print( ssid );
    textUtils_println( "..." );
    while ( connected == false ) {
        connected = EspDrv_wifiConnect( ssid, ssidpassw );
        textUtils_print( "..." );
    }
    textUtils_println( "Connected to WiFi." );

    EspDrv_getIpAddress( localIP );
    printf( "IP Address: %d,%d,%d,%d\n", localIP[ 0 ], localIP[ 1 ], localIP[ 2 ], localIP[ 3 ] );

    textUtils_print( "Press a key to continue...\n" );

    waitKey();

    while (1) {

    connClosed = false;
    textUtils_println( "Trying to connect to server..." );

    EspDrv_stopClient( 1 );
    if ( EspDrv_startClient( host, serverPort, 1, TCP_MODE ) == false ) {

        textUtils_println("Can't connect to server.");

        return;

    }

    textUtils_println( "\nConnected to server. Type to send chars, received chars will be shown." );

    connClosed = false;
    key = 0;
    while ( connClosed == false && key != 12 ) {

        // Reads data
        if ( EspDrv_availData( 1 ) > 0 ) {

            if ( EspDrv_getData( 1, &c, false, &connClosed ) == false ) {
                textUtils_println( "ERROR reading socket***" );
            }
            else if ( c >= 32 && c < 127 ) {
                fputc_cons( c );
            }
            else if ( c == '\n' || c == '\r' ) {
                fputc_cons( c );
            }

        }


        // Send data
        key = getKey();
        if ( key > 0 && key != 12 ) {
            while ( getKey() > 0 ) {
                // Wait key release
            }

            if ( key == '\n' ) {
                buffer[ 0 ] = '\n';
                buffer[ 1 ] = '\r';
                buffer[ 2 ] = 0;
            }
            else {
                buffer[ 0 ] = key;
                buffer[ 1 ] = 0;
            }

            if ( EspDrv_sendData( 1, buffer, strlen( buffer ), false ) == false ) {
                textUtils_println( "Error sending data." );
                return;
            }

            textUtils_print( buffer );

        }

    }
    
    delay( 1200 );

    }

    textUtils_println( "\nDisconnecting WiFi..." );

    EspDrv_disconnect();

    textUtils_println( "End. Press a key." );
    waitKey();

}

#endif

void main(void) {

    int i;

    setPalette( palette );
    ula_plus_mode();

    srand( time(NULL) );
    textUtils_32ColumnsMode();
    zx_border( INK_BLACK );

    for ( i = 0; i < 6; i++ ) {
        theBuffer[ i ] = i;
    }
    paintHourSeconds( theBuffer );

    /*
    theBuffer[ 0 ] = DIGIT_T;
    theBuffer[ 1 ] = DIGIT_U;
    theBuffer[ 2 ] = DIGIT_2;
    theBuffer[ 3 ] = DIGIT_9;
    */
    theBuffer[ 0 ] = DIGIT_S;
    theBuffer[ 1 ] = DIGIT_A;
    theBuffer[ 2 ] = DIGIT_F;
    theBuffer[ 3 ] = DIGIT_R;
    paintUpperDigits( theBuffer );

    waitKey();

    ula_normal_mode();

}

void setPalette( uint8_t *palette ) {

    int i;

    // GGG RRR BB
    // 000 000 00

    // Colors with BRIGHT = 0

    for ( i = 0; i < 8; i++ ) {

        // Ink
        ulaplus_set( i, palette[ i ] );

        // Paper
        ulaplus_set( i + 8, palette[ i ] );

    }

    // Colors with BRIGHT = 1

    for ( i = 16; i < 24; i++ ) {

        // Ink
        ulaplus_set( i, palette[ i - 8 ] );

        // Paper
        ulaplus_set( i + 8, palette[ i - 8 ] );

    }

    // Colors with FLASH = 1

    for ( i = 32; i < 64; i++ ) {

        ulaplus_set( i, 0x00 );
    }

}

void paintHourSeconds( uint8_t *buffer ) {

    // Expects 6 bytes in the buffer "hhmmss", 0 to 9 are numeric characters, 10 is space

    // Hours
    paintGraphicBlockPosition( 11, 11, 2, 4, graphicsBigDigits + ( ( *buffer++ ) << 6 ) );
    paintGraphicBlockPosition( 13, 11, 2, 4, graphicsBigDigits + ( ( *buffer++ ) << 6 ) );

    // Minutes
    paintGraphicBlockPosition( 16, 11, 2, 4, graphicsBigDigits + ( ( *buffer++ ) << 6 ) );
    paintGraphicBlockPosition( 18, 11, 2, 4, graphicsBigDigits + ( ( *buffer++ ) << 6 ) );

    // Seconds
    paintGraphicPixelPosition( 161, 103, 1, 13, graphicsSmallDigits + ( ( *buffer++ ) << 4 ) );
    paintGraphicPixelPosition( 171, 103, 1, 13, graphicsSmallDigits + ( ( *buffer ) << 4 ) );

}

void paintUpperDigits( uint8_t *buffer ) {

    // Expects 4 bytes in the buffer, 0 to 9 are numeric digits, 10 to 19 are the letters FRMOAUTHWE in that order, 20 is space

    paintGraphicPixelPosition( 127, 75, 1, 13, graphicsSmallDigits + ( ( *buffer++ ) << 4 ) );
    paintGraphicPixelPosition( 137, 75, 1, 13, graphicsSmallDigits + ( ( *buffer++ ) << 4 ) );

    paintGraphicPixelPosition( 151, 75, 1, 13, graphicsSmallDigits + ( ( *buffer++ ) << 4 ) );
    paintGraphicPixelPosition( 161, 75, 1, 13, graphicsSmallDigits + ( ( *buffer ) << 4 ) );

}

void paintAlarmFlags( uint8_t flags ) {

}
