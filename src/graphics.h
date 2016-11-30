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

#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "../src/integerTypes.h"

extern void paintGraphic8x8BlockPosition( uint16_t blockPosX, uint16_t blockPosY, uint8_t *graphic );
extern void paintGraphicBlockPosition( uint16_t blockPosX, uint16_t blockPosY, uint16_t numBlocksX, uint16_t numBlocksY, uint8_t *graphic );

extern void paintGraphic8x8PixelPosition( uint16_t pixelPosX, uint16_t pixelPosY, uint16_t numLines, uint8_t *graphic );
extern void paintGraphicPixelPosition( uint16_t pixelPosX, uint16_t pixelPosY, uint16_t numBlocksX, uint16_t numPixelsY, uint8_t *graphic );

#endif /* GRAPHICS_H */

