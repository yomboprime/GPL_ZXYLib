/*
    This file is part of GPLZXYLib

    GPLZXYLib is free software: you can redistribute it and/or modify
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

// ***** Librerias *****

var fs = require( 'fs' );
var Canvas = require( 'canvas' );
var Image = Canvas.Image;


console.log( "png2ulapluspalette");
console.log( "Use: node main.js <relative_image_path>" );
console.log( "" );

var pathImage = __dirname + "/" + process.argv[ 2 ];

console.log( "Reading image in: " + pathImage );

fs.readFile( pathImage, function( err, buferImagen ) {

    if ( err ) {
        console.error( "There was an error reading the image in: " + pathImage );
        throw err;
    }

    console.log( "Processing image..." );

    var img = new Image;
    img.src = buferImagen;
    var tx = img.width;
    var ty = img.height;

    var theCanvas = new Canvas( tx, ty );
    ctx2d = theCanvas.getContext( '2d' );
    ctx2d.drawImage( img, 0, 0 );

    var imageData = ctx2d.getImageData( 0, 0, tx, ty );
    var pixelData = imageData.data;

    function getPixelPR( p ) {
        return pixelData[ p ];
    }

    function getPixelPG( p ) {
        return pixelData[ p + 1 ];
    }

    function getPixelPB( p ) {
        return pixelData[ p + 2 ];
    }

    function getPixelPA( p ) {
        return pixelData[ p + 3 ];
    }

    function getPixelP( x, y ) {
        return 4 * ( Math.round( x ) + Math.round( y ) * tx );
    }

    function setPixel( x, y, r, g, b, a ) {
        var p = getPixelP( x, y );
        pixelData[ p ] = r;
        pixelData[ p + 1 ] = g;
        pixelData[ p + 2 ] = b;
        pixelData[ p + 3 ] = a;
    }

    function convertColorRGBARadasRGB332( r, g, b ) {

        // r, g, b de 0 a 255

        var rr = r & 0xE0;
        var gr = g & 0xE0;
        var br = b & 0xC0;
        if ( br !== 0 ) {
            br |= 0x20;
        }

        return [ rr, gr, br ];
    }

    function colorIsInPalette( color, palette ) {

        // Devuelve indice en la paleta, de 0 a 15, o -1 si no se encuentra

        for ( var i = 0; i < palette.length; i++ ) {
            if ( color[ 0 ] === palette[ i ][ 0 ] &&
                 color[ 1 ] === palette[ i ][ 1 ] &&
                 color[ 2 ] === palette[ i ][ 2 ] ) {
				return i;
            }
        }

        return -1;
    }

	function colorsEquals( c1, c2 ) {
		if ( c1[ 0 ] === c2[ 0 ] &&
			 c1[ 1 ] === c2[ 1 ] &&
			 c1[ 2 ] === c2[ 2 ] ) {
			return true;
		}
		return false;
	}

	function iscolorBlack( c1 ) {
		if ( c1[ 0 ] === 0 &&
			 c1[ 1 ] === 0 &&
			 c1[ 2 ] === 0 ) {
			return true;
		}
		return false;
	}

    var palette = [];
	var previousColor = [ 1, 1, 1 ];
	for ( var j = 0; j < ty; j++ ) {
		for ( var i = 0; i < tx; i++ ) {

			var p = getPixelP( i, j );
			var r = getPixelPR( p );
			var g = getPixelPG( p );
			var b = getPixelPB( p );
			var color = convertColorRGBARadasRGB332( r, g, b );

			var index = colorIsInPalette( color, palette );

			if ( index === -1 || ! colorsEquals( color, previousColor ) ) {
				palette.push( color );
			}

			setPixel( i, j, color[ 0 ], color[ 1 ], color[ 2 ], 255 );

			previousColor[ 0 ] = color[ 0 ];
			previousColor[ 1 ] = color[ 1 ];
			previousColor[ 2 ] = color[ 2 ];
		}
	}

    ctx2d.putImageData( imageData, 0, 0 );

	console.log( "There are " + palette.length + " colors in the palette." );

//    console.log( "Saving image..." );
//    savePNGImage( theCanvas, pathImage );


	var path = null;
	var barIndex = pathImage.lastIndexOf( "/" );
	if ( barIndex >= 0 ) {
		path = pathImage.substring( 0, barIndex );
	}
	else {
		console.error( "Error in the image path." );
		process.exit( -1 );
	}
    var pathPalettes = path + "/palettes.h";


    var text = "\n";

    text += "\n\n#ifndef PALETTES_H\n";
    text += "#define PALETTES_H\n\n";

	text += "#include \"../../../src/integerTypes.h\"\n\n";

    text += "#define NUM_COLORS " + palette.length + "\n\n";

    text += "static uint8_t palette [] = {\n\n";

    text += "    // Pixels in format gggrrrbb\n\n"

	for ( var ic = 0; ic < palette.length; ic++ ) {
		var col = palette[ ic ];
		var rr = ( col[ 1 ] & 0xE0 ) >> 5;
		var gr = ( col[ 0 ] & 0xE0 ) >> 5;
		var br = ( col[ 2 ] & 0xC0 ) >> 6;

		var valorBytePaleta = ( rr << 5 ) | ( gr << 2 ) | br;

		var char2 = valorBytePaleta.toString( 16 );

		text += "    0x" + char2 + ( ic === palette.length - 1 ? "\n" : ",\n" );
	}

    text += "};\n\n";


    text += "\n";
    text += "\n";

    text += "#endif // PALETTES_H\n\n\n";

    console.log( "Saving file palettes.h ..." );

    saveTextFile( text, pathPalettes, function() {
        console.log( "File saved." );
    } );

	console.log( "Fin." );

} );


function savePNGImage( canvas, pathImagen ) {

    var out = fs.createWriteStream( pathImagen );
    var stream = canvas.createPNGStream();

    stream.on( 'data', function( chunk ) {
        out.write( chunk );
    } );

}

function saveTextFile( text, path, onEnd ) {

    fs.writeFile( path, text, function( err ) {

        if( err ) {
            console.error( "Error saving text file: " + err + " file: " + path );
            process.exit( -1 );
        }

        if ( onEnd ) {
            onEnd();
        }

    } );
}
