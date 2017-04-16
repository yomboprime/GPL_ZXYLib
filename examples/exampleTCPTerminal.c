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
#include <string.h>

#include "../src/textUtils.h"
#include "../src/EspDrv/EspDrv.h"
#include "../src/EspDrv/IPAddress.h"

#define getKey() in_Inkey()

#define BUFFER_SIZE 128
uint8_t buffer[ BUFFER_SIZE ];

void main(void) {


    // Put your url here:
    char* host = "google.es";
    uint16_t serverPort = 80;

    // ZX Spectrum BBS
    //char* host = "retrobbs.sinclair.homepc.it";
    //uint16_t serverPort = 23;

    // Time service
    //char* host = "india.colorado.edu";
    //uint16_t serverPort = 13;

    // Meteorology service
    //char* host = "rainmaker.wunderground.com";
    //uint16_t serverPort = 23;


    
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
