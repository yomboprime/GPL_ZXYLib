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
#include "../src/zxuno/ftp.h"
#include "../src/EspDrv/IPAddress.h"

#define getKey() in_Inkey()

void waitKey() {
    while (getKey());
    while (!getKey());
}

#define BUFFER_SIZE ( 1024 )
uint8_t buffer[ BUFFER_SIZE ];

void downloadProgressCallback( int32_t progress );

void main(void) {


    // Put your host or server ip here:
    char* host = "10.42.0.1";

    // Wifi name:
    char *ssid = "apoint";
    // Wifi password:
    char*ssidpassw = "apoint123";
    // Ftp login name:
    char* user = "anonymous";
    // Ftp password:
    char *ftpPassword = "test";


    // FTP path of file to download:
    char *ftpFilePath = "/bomber.tap";

    // SD path of file to write down:
    char *sdFilePath = "/bomber.tap";


    uint8_t connected = false;
    IPAddress localIP[ 4 ];

    uint16_t numEntries = 0;
    uint8_t result = 0;

    uint32_t fileSize;
    uint8_t fileOrDirectory;

    uint8_t fileEntryIndex;


    textUtils_64ColumnsMode();
    textUtils_cls();

    textUtils_println( "This example downloads a file from a FTP server to the sd card. Press a key to start..." );
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

    textUtils_println( "Press a key to continue..." );

    waitKey();

    textUtils_println( "Downloading file..." );

    FTP_setConnectionParameters( host, 21, 1, 2, user, ftpPassword );

    

    if ( FTP_downloadFile( ftpFilePath, sdFilePath, buffer, BUFFER_SIZE, downloadProgressCallback ) == FTP_NO_ERROR ) {

        textUtils_println( "File downloaded succesfully." );

    }
    else {

        textUtils_print( "Error while downloading file." );

    }

    textUtils_println( "Disconnecting..." );

    EspDrv_disconnect();

    textUtils_println( "End. Press a key." );
    waitKey();

}

void downloadProgressCallback( int32_t progress ) {

    textUtils_print_l( progress );
    textUtils_println( " bytes read. " );

}
