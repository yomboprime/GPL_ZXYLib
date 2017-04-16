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

#include "../../src/EspDrv/EspDrv.h"
#include "../../src/EspDrv/IPAddress.h"

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

#define BUFFER_SIZE 1023
uint8_t theBuffer[ BUFFER_SIZE + 1 ];

uint8_t currentTime[ 6 ];

int i;

// Function prototypes

void setPalette();
void paintHourSeconds( uint8_t *buffer );
void paintUpperDigits( uint8_t *buffer );
void paintAlarmFlags( uint8_t flags );
bool timeIsGreater( uint8_t *timeBuffer1, uint8_t *timeBuffer2 );
void connectWiFi( uint8_t *ssid, uint8_t *passw );
void disconnectWiFi();
int getTimeAsText( uint8_t *buffer );
bool parseTime( uint8_t *buffer, uint8_t *time );
uint8_t ascii2smallDigit( uint8_t a );


void main(void) {

    // Wifi name:
    char *ssid = "apoint";
    // Wifi password:
    char*ssidpassw = "apoint123";

    int error;


    setPalette( palette );
    ula_plus_mode();

    srand( time(NULL) );
    textUtils_32ColumnsMode();
    zx_border( INK_BLACK );

    for ( i = 0; i < 6; i++ ) {
        currentTime[ i ] = 0;
    }
    paintHourSeconds( currentTime );

    connectWiFi( ssid, ssidpassw );

    while ( 1 ) {
        
        error = getTimeAsText( theBuffer );
        if ( error > 0 ) {
            textUtils_print( "ERROR: " );
            textUtils_print_l( error );
        }
        else {

            if ( parseTime( theBuffer, currentTime ) ) {
                paintHourSeconds( currentTime );
                paintUpperDigits( currentTime + 6 );
            }

        }

    }

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

    flags &= 0x07;

    paintGraphicBlockPosition( 10, 9, 4, 2, graphicsIcons + ( flags << 6 ) );

}

bool timeIsGreater( uint8_t *timeBuffer1, uint8_t *timeBuffer2 ) {

    // Returns true iff time1 > time2

    int16_t i;
    uint8_t v1, v2;

    for ( i = 0; i < 6; i++ ) {

        v1 = *timeBuffer1++;
        v2 = *timeBuffer2++;

        if ( v1 > v2 ) {
            return true;
        }
        else if ( v1 < v2 ) {
            return false;
        }

        // If v1==v2, the loop continues

    }

    return false;

}

void connectWiFi( uint8_t *ssid, uint8_t *passw ) {

    uint8_t connected = false;
    uint8_t flagDisplayCon;

    if ( EspDrv_wifiDriverInit() == false ) {

        textUtils_cls();
        textUtils_println( "Couldn't communicate with the WiFi module. Press a key to exit." );
        waitKey();

        exit(1);

    }

    flagDisplayCon = 0;
    while ( connected == false ) {

        connected = EspDrv_wifiConnect( ssid, passw );

        for ( i = 0; i < 6; i++ ) {
            //theBuffer[ i ] = ( ( i & 1 ) ^ flagDisplayCon ) != 0 ? ( i < 4 ? DIGIT_BIG_SPACE : DIGIT_SPACE ) : DIGIT_8;
            theBuffer[ i ] = ( ( i ^ flagDisplayCon ) & 1 ) != 0 ? DIGIT_8 : DIGIT_1;
        }

        paintHourSeconds( theBuffer );

        flagDisplayCon = flagDisplayCon == 0 ? 1 : 0;
    }

}

void disconnectWiFi() {

    EspDrv_disconnect();

}

int getTimeAsText( uint8_t *buffer ) {

    bool connClosed;
    uint8_t c = 0;
    uint8_t *pbuf;
    uint16_t numBytesReceived;
    uint8_t parseState;

    connClosed = false;

    EspDrv_stopClient( 1 );
    if ( EspDrv_startClient( "www.google.com", 80, 1, TCP_MODE ) == false ) {
        return 1;
    }


    connClosed = false;

    pbuf = "GET /";
    if ( EspDrv_sendData( 1, pbuf, strlen( pbuf ), true) == false ) {
        return 2;
    }
    else {
        parseState = 0;
        numBytesReceived = 0;
        pbuf = "Date: ";

        while ( connClosed == false ) {
            if ( EspDrv_availData( 1 ) > 0 ) {

                if ( EspDrv_getData( 1, &c, false, &connClosed ) == false ) {
                    return 3;
                }

                switch ( parseState ) {
                    case 0:
                    case 1:
                    case 2:
                    case 3:
                    case 4:
                    case 5:
                        if ( c == pbuf[ parseState ] ) {
                            parseState++;
                        }
                        else {
                            parseState = 0;
                        }
                        break;
                    case 6:
                        if ( c == 13 ) {
                            parseState++;
                        }
                        else if ( numBytesReceived < BUFFER_SIZE ) {
                            buffer[ numBytesReceived++ ] = c;
                        }
                        break;
                    default:
                        // Just read rest of bytes, do nothing
                        break;
                }
            }
        }

        buffer[ numBytesReceived ] = 0;

    }

    return 0;
}

bool parseTime( uint8_t *buffer, uint8_t *time ) {

    // Input: "Mon, 01 Jan 2000 00:00:00 GMT"

    // time: HHmmsswwDD
    // ww = day of week (letters)
    // DD = day of month (numbers)

    uint8_t w0;
    uint8_t w1;
    uint8_t d0;
    uint8_t d1;

    w0 = *buffer++;
    w1 = *buffer++;
    buffer += 3;

    d0 = *buffer++;
    d1 = *buffer++;
    buffer += 10;

    *time++ = *buffer++ - '0';
    *time++ = *buffer++ - '0';
    buffer++;
    *time++ = *buffer++ - '0';
    *time++ = *buffer++ - '0';
    buffer++;
    *time++ = *buffer++ - '0';
    *time++ = *buffer++ - '0';

    *time++ = ascii2smallDigit( w0 );
    *time++ = ascii2smallDigit( w1 );
    *time++ = ascii2smallDigit( d0 );
    *time++ = ascii2smallDigit( d1 );

    return true;

}

uint8_t ascii2smallDigit( uint8_t a ) {

    if ( a >= '0' && a <= '9' ) {
        return a - '0';
    }

    switch ( a ) {
        case 'F':
        case 'f':
            return DIGIT_F;
        case 'R':
        case 'r':
            return DIGIT_R;
        case 'M':
        case 'm':
            return DIGIT_M;
        case 'O':
        case 'o':
            return DIGIT_O;
        case 'A':
        case 'a':
            return DIGIT_A;
        case 'U':
        case 'u':
            return DIGIT_U;
        case 'T':
        case 't':
            return DIGIT_T;
        case 'H':
        case 'h':
            return DIGIT_H;
        case 'W':
        case 'w':
            return DIGIT_W;
        case 'E':
        case 'e':
            return DIGIT_E;
        case 'S':
        case 's':
            return DIGIT_S;
        case ' ':
            return DIGIT_SPACE;

        default:
            return 0;
    }

}
