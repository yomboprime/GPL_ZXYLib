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

#define MAX_DISPLAY_DIR_ENTRIES ( 20 )
#define MAX_BYTES_FTP_FILENAME ( 20 )

#define FTP_FILE_ENTRY_SIZE ( MAX_BYTES_FTP_FILENAME + FTP_DIR_ENTRY_SIZE )
#define BUFFER_SIZE ( MAX_DISPLAY_DIR_ENTRIES * FTP_FILE_ENTRY_SIZE )
uint8_t buffer[ BUFFER_SIZE ];

void printDirectoryListing( uint8_t *buffer, uint16_t numEntries, uint8_t maxFileSize );



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


    uint8_t connected = false;
    IPAddress localIP[ 4 ];

    uint16_t numEntries = 0;
    uint16_t numTotalEntries = 0;
    uint8_t result = 0;

    uint32_t fileSize;
    uint8_t fileOrDirectory;

    uint8_t fileEntryIndex;


    textUtils_64ColumnsMode();
    textUtils_cls();

    textUtils_println( "This example reads a directory listing from a FTP server. Press a key to start..." );

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

    FTP_setConnectionParameters( host, 21, 1, 2, user, ftpPassword );

    result = FTP_listFiles( "/", buffer, 0, MAX_DISPLAY_DIR_ENTRIES, &numEntries, &numTotalEntries, MAX_BYTES_FTP_FILENAME );

    if ( result == FTP_NO_ERROR ) {
        textUtils_print( "Number of listed entries: " );
        textUtils_println_l( numEntries );
        printDirectoryListing( buffer, numEntries, MAX_BYTES_FTP_FILENAME );
    }
    else if ( result == FTP_NO_ERROR_MORE_FILES ) {
        textUtils_println( "There are more entries. Number of listed entries: " );
        textUtils_println_l( numEntries );
        printDirectoryListing( buffer, numEntries, MAX_BYTES_FTP_FILENAME );
    }
    else {
        textUtils_println( "Error while retrieving directory listing." );
    }

    textUtils_print( "Press a key to continue...\n" );

    waitKey();

    fileEntryIndex = 4;
    if ( fileEntryIndex > numEntries ) {
        fileEntryIndex = numEntries - 1;
    }
    textUtils_print( "Getting file entry number: " );
    textUtils_println_l( fileEntryIndex );
    result = FTP_getFileNameAndSize( "/", fileEntryIndex, buffer, BUFFER_SIZE, &fileSize, &fileOrDirectory );

    if ( result == FTP_NO_ERROR ) {
        textUtils_print( "File entry obtained.\nFile size: " );
        textUtils_println_l( fileSize );
        textUtils_print( "File or directory: " );
        textUtils_println( fileOrDirectory == '>' ? "Directory" : "File" );
        textUtils_print( "\nName: " );
        textUtils_println( buffer );
    }
    else {
        textUtils_println( "Error while retrieving file entry." );
    }

    textUtils_println( "Disconnecting..." );

    EspDrv_disconnect();

    textUtils_println( "End. Press a key." );
    waitKey();

}

void printDirectoryListing( uint8_t *buffer, uint16_t numEntries, uint8_t maxFileSize ) {

    uint16_t i;
    uint8_t j;

    bool flagMoreChars;
    uint8_t currentFileNameSize;

    uint32_t fileSize;

    textUtils_println( "\nDIRECTORY LISTING:" );

    for ( i = 0; i < numEntries; i++ ) {

        fputc_cons( *buffer++ );

        currentFileNameSize = maxFileSize;
        flagMoreChars = buffer[ MAX_BYTES_FTP_FILENAME ] == '.' ? true: false;
        if ( flagMoreChars == true ) {
            currentFileNameSize -= 2;
        }

        for ( j = 0; j < currentFileNameSize; j++ ) {

            fputc_cons( *buffer++ );

        }

        for ( ; j < MAX_BYTES_FTP_FILENAME; j++ ) {

            buffer++;

        }

        // Skip separation char
        buffer++;

        // Print separation
        if ( flagMoreChars == true ) {
            fputc_cons( '.' );
            fputc_cons( '.' );
            fputc_cons( '.' );
        }
        else {
            fputc_cons( ' ' );
        }

        // Extension
        for ( j = 0; j < 3; j++ ) {

            fputc_cons( *buffer++ );

        }

        // File size
        fileSize = *buffer++;
        fileSize += ( (uint32_t)( *buffer++ ) ) << 8;
        fileSize += ( (uint32_t)( *buffer++ ) ) << 16;
        fileSize += ( (uint32_t)( *buffer++ ) ) << 24;

        textUtils_print( "    " );
        textUtils_println_l( fileSize );

    }

    textUtils_println( "\nEND OF DIRECTORY LISTING." );


}
