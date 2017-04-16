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
#include "../src/esxdos.h"

#define getKey() in_Inkey()

void main(void) {


    int16_t drive;

    int16_t fileHandle;

    int16_t errCode;

    uint8_t *filePath = "/hola.txt";

    ESXDOS_FSTAT_Struct stat;

    textUtils_64ColumnsMode();
    textUtils_cls();

    textUtils_println( "Starting SD stat test, press a key..." );

    waitKey();

    drive = ESXDOS_getDefaultDrive();

    iferror {
        textUtils_println( "SD card not inserted." );
        return;
    }

    textUtils_println( "SD card detected." );

    textUtils_print( "Drive: " );
    textUtils_println_l( drive );

    textUtils_print( "Opening file: " );
    textUtils_println( filePath );

    fileHandle = ESXDOS_fopen( filePath, ESXDOS_FILEMODE_READ, drive );
    iferror {
        textUtils_print( "Could not open file." );
        textUtils_print( "Error code= " );
        textUtils_println_l( fileHandle );
        return;
    }

    textUtils_print( "File handle: " );
    textUtils_println_l( fileHandle );

    errCode = ESXDOS_fstat( &stat, fileHandle );
    iferror {
        textUtils_print( "Error performing stat on the file. Error code: " );
        textUtils_println_l( errCode );
    }
    else {

        textUtils_println( "Stats read." );

        textUtils_print( "Drive: " );
        textUtils_println_l( stat.drive );

        textUtils_print( "Device: " );
        textUtils_println_l( stat.device );

        textUtils_print( "Attributes: " );
        textUtils_println_l( stat.attributes );

        textUtils_print( "Date: " );
        textUtils_println_l( stat.date );

        textUtils_print( "File Size: " );
        textUtils_println_l( stat.fileSize );

    }

    textUtils_print( "Get file size: " );
    textUtils_println_l( ESXDOS_fsize( fileHandle ) );

    textUtils_print( "Is directory: " );
    textUtils_println( ESXDOS_isDirectory( fileHandle ) == true ? "true" : "false" );

    ESXDOS_fclose( fileHandle );
    iferror {
        textUtils_println( "Error closing the file." );
        return;
    }
    textUtils_println( "File closed OK" );

    textUtils_println( "End. Press a key." );
    waitKey();


}
