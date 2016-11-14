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

void waitKey() {
    while (getKey());
    while (!getKey());
}

#define BUFFER_SIZE 512
uint8_t buffer[ BUFFER_SIZE ];

void main(void) {


    // Put your url here:
    char* host = "google.es";

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

    textUtils_64ColumnsMode();
    textUtils_cls();

    textUtils_println( "This example access the contents of a HTTP URL. Press a key to start..." );

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



    connClosed = false;
    textUtils_println( "Trying to connect to server..." );

    EspDrv_stopClient( 1 );
    if ( EspDrv_startClient( host, 80, 1, TCP_MODE ) == false ) {

        textUtils_println("Can't connect to server.");

        return;

    }

    connClosed = false;

    textUtils_print( "\nConnected.\nSending cmd GET..." );
    sprintf( buffer, "GET / HTTP/1.1" );
    if ( EspDrv_sendData( 1, buffer, strlen( buffer ), true) == false ) {
        textUtils_println( "Error sending GET." );
        return false;
    }
    else {

        pbuf = "Host: yombo.org";
        if ( EspDrv_sendData( 1, pbuf, strlen( pbuf ), true ) == false ) {
            textUtils_println( "Error sending request. 1" );
            return;
        }
        pbuf = "User-Agent: ZXSpectrum";
        if ( EspDrv_sendData( 1, pbuf, strlen( pbuf ), true ) == false ) {
            textUtils_println( "Error sending request. 2" );
            return;
        }
        pbuf = "Connection: close\n\r";
        if ( EspDrv_sendData( 1, pbuf, strlen( pbuf ), true ) == false ) {
            textUtils_println( "Error sending request. 3" );
            return;
        }

        textUtils_println( "Waiting server response...\n" );

        while ( connClosed == false ) {
            if ( EspDrv_availData( 1 ) > 0 ) {

                if ( EspDrv_getData( 1, &c, false, &connClosed ) == false ) {
                    textUtils_println( "ERROR reading socket***" );
                    return;
                }
                else if ( c >= 32 && c < 127 ) {
                    fputc_cons( c );
                }
            }
        }

    }

    textUtils_println( "Disconnecting..." );

    EspDrv_disconnect();

    textUtils_println( "End. Press a key." );
    waitKey();

}
