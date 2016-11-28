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

// Use this modes in setULAPlusMode()
#define ULAPLUS_MODE_NORMAL 0
#define ULAPLUS_MODE_PALETTE 1
#define ULAPLUS_MODE_GRAYSCALE 2
#define ULAPLUS_MODE_RADASTAN 3

extern void setULAPlusMode( uint8_t mode );
extern void setULAPlusPaletteRegister( uint8_t reg, uint8_t rrrggbb );


// These functions are for the normal ZX Spectrum graphics mode or The ulaplus paletted mode (not for Radastan  mode)

extern void paintGraphic8x8BlockPosition( uint16_t blockPosX, uint16_t blockPosY, uint8_t *graphic );
extern void paintGraphicBlockPosition( uint16_t blockPosX, uint16_t blockPosY, uint16_t numBlocksX, uint16_t numBlocksY, uint8_t *graphic );

extern void paintGraphic8x8PixelPosition( uint16_t pixelPosX, uint16_t pixelPosY, uint8_t *graphic );
extern void paintGraphicPixelPosition( uint16_t pixelPosX, uint16_t pixelPosY, uint16_t numBlocksX, uint16_t numBlocksY, uint8_t *graphic );

#endif /* GRAPHICS_H */

