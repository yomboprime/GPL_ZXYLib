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

#include "../../src/textUtils.h"
#include "../../src/EspDrv/EspDrv.h"

#define chatTextColor(a)	printf("\033[%um",a+30)
#define chatTextBackground(a)	printf("\033[%um",a+40)

// Send data buffer
#define BUFFER_SIZE ( 4096 )
uint8_t buffer[ BUFFER_SIZE ];

uint8_t *message;
uint8_t *tempMessage;

// Command buffer
uint8_t commandBuffer[ 20 ];

#define MESSAGE_TYPE_CONNECT 0x55
#define MESSAGE_TYPE_QUERY_NUM_ROOMS 0x01
#define MESSAGE_TYPE_QUERY_ROOM_INFO 0x02
#define MESSAGE_TYPE_JOIN_ROOM 0x03
#define MESSAGE_TYPE_QUIT 0x04
#define MESSAGE_TYPE_QUIT_ROOM 0x05
#define MESSAGE_TYPE_DATA 0x06
#define MESSAGE_TYPE_QUERY_DATA 0x07
#define MESSAGE_TYPE_KEEP_ALIVE 0x08

#define NICK_SIZE ( 15 )
uint8_t nick[ NICK_SIZE + 1 ];

char *ssid = "apoint";
char *ssidpassw = "apoint123";
char *host = "10.42.0.1";
uint16_t chatPort = 8080;

#define chatSocket 1

uint8_t c = 0;
bool connClosed = false;
uint8_t doExit = false;

int i, j, k, n;

bool readCharFromServer();

void main(void) {

    textUtils_64ColumnsMode();
    textUtils_cls();
    textUtils_printAt( 0, 23 );

    textUtils_println( "Welcome to ZX-Uno Chat application." );
    
    textUtils_println( "\n\nPlease enter your nickname (min 3 chars, max 15 chars):" );
    *buffer = MESSAGE_TYPE_DATA;
    while ( gets( buffer + 1 ) == NULL || strlen( buffer + 1 ) < 3 || strlen( buffer + 1 ) > NICK_SIZE ) {
        textUtils_println( "\n\nPlease enter your nickname (min 3 chars, max 16 chars):" );
    }

    //sprintf( buffer + 1, "yombo" );

    message = buffer + 1 + strlen( buffer + 1 );
    *message++ = ':';
    *message++ = ' ';

    textUtils_println( "Initing WiFi chip..." );

    // Init WiFi driver
    if ( EspDrv_wifiDriverInit() == true ) {

        textUtils_println( "WiFi module inited OK." );

    }
    else {

        textUtils_println( "Couldn't communicate with the WiFi module. Press key to exit." );

        waitKey();
        exit(1);

    }

    textUtils_print( "Connecting to WiFi network... " );

    // Connect to WiFi
    while ( EspDrv_wifiConnect( ssid, ssidpassw ) == false ) {
        textUtils_print( "..." );
    }
    textUtils_println( "Connected to WiFi." );
    

    // Connect to server
    EspDrv_stopClient( chatSocket );
    if ( EspDrv_startClient( host, chatPort, chatSocket, TCP_MODE ) == false ) {

        textUtils_print("Can't connect to server. Press key to exit." );

        waitKey();
        doExit = true;

    }
    else {
        textUtils_println( "Connected to the chat. Type EXIT to disconnect.\n" );
    }

    while ( doExit == false ) {
        
        // Check for key press to start entering text
        k = in_Inkey();
        if ( k != 0 && k != 13 ) {

            // The user enters the message
            if ( gets( message ) != NULL && ( ( *message ) != 0 ) ) {
                if ( strcmp( message, "EXIT" ) == 0 ) {
    
                    // "EXIT" keyword exits the program
                    doExit = true;

                }
                else {
                    // Erase portion of the screen where the message was
                    n = strlen( buffer );

                    i = n / 64;
                    textUtils_printAt( 0, 23 - i - 1 );
                    for ( j = 0; j <= i; j++ ) {
                        for ( k = 0; k < 64; k++ ) {
                            fputc_cons( ' ' );
                        }
                    }

                    // Send the message to the server. + 1 for ending null.
                    if ( EspDrv_sendData( chatSocket, buffer, n + 1, false ) == false ) {
                        textUtils_println( "Error sending message to server." );
                        doExit = true;
                    }

                }
                
            }
            // Wait for user to release last key
            while ( in_Inkey() != 0 );
        }
        else {

            // If no keypress, check for incoming messages
            commandBuffer[ 0 ] = MESSAGE_TYPE_QUERY_DATA;
            if ( EspDrv_sendData( chatSocket, commandBuffer, 1, false ) == false ) {
                textUtils_println( "Error sending query to server." );
                doExit = true;
                break;
            }
            connClosed = false;
            
            // Read message type
            if ( readCharFromServer() == false || c != MESSAGE_TYPE_QUERY_DATA ) {
                textUtils_print( "Incorrect answer from the server. received byte: " );
                textUtils_println_l( c );
                doExit = true;
                break;
            }
            
            // Read number of messages
            if ( readCharFromServer() == false ) {
                doExit = true;
                break;
            }
            n = c;

            // Receive n messages
            i = 0;
            tempMessage = message;
            while ( i < n && connClosed == false ) {
                
                // Read message type
                if ( readCharFromServer() == false || c != MESSAGE_TYPE_DATA ) {
                    textUtils_print( "Incorrect message type in message query from the server." );
                    doExit = true;
                    break;
                }

                while ( c != 0 ) {
                
                    // Read message characters
                    if ( readCharFromServer() == false ) {
                        doExit = true;
                        break;
                    }
                
                    *tempMessage++ = c;
                
                }
                
                // End of message. Print it and step to the next one
                textUtils_println( message );
                tempMessage = message;
                i++;
            }
        }
    }

    textUtils_println( "Disconnecting from WiFi..." );

    EspDrv_stopClient( chatSocket );
    EspDrv_disconnect();

    textUtils_println( "End. Press a key." );
    waitKey();

    return 0;
}

bool readCharFromServer() {

    while ( EspDrv_availData( chatSocket ) == 0 );

    if ( EspDrv_getData( chatSocket, &c, false, &connClosed ) == false ) {
        textUtils_println( "Timeout receiving data from server." );
        return false;
    }

    if ( connClosed == true ) {
        textUtils_println( "Server disconnected." );
        return false;
    }

    return true;

}
