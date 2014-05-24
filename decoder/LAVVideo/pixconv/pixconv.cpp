/*
 *      Copyright (C) 2010-2014 Hendrik Leppkes
 *      http://www.1f0.de
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "stdafx.h"
#include "pixconv_internal.h"

// 8x8 Bayes ordered dithering table, scaled to the 0-255 range for 16->8 conversion
// stored as 16-bit unsigned for optimized SIMD access
DECLARE_ALIGNED(16, const uint16_t, dither_8x8_256)[8][8] = {
  {   0, 192,  48, 240,  12, 204,  60, 252 },
  { 128,  64, 176, 112, 140,  76, 188, 124 },
  {  32, 224,  16, 208,  44, 236,  28, 220 },
  { 160,  96, 144,  80, 172, 108, 156,  92 },
  {   8, 200,  56, 248,   4, 196,  52, 244 },
  { 136,  72, 184, 120, 132,  68, 180, 116 },
  {  40, 232,  24, 216,  36, 228,  20, 212 },
  { 168, 104, 152,  88, 164, 100, 148,  84 }
};

DECLARE_CONV_FUNC_IMPL(plane_copy)
{
  LAVOutPixFmtDesc desc = lav_pixfmt_desc[outputFormat];

  int plane, line;

  const int widthBytes = width * desc.codedbytes;
  const int planes = max(desc.planes, 1);

  for (plane = 0; plane < planes; plane++) {
    const int planeWidth = widthBytes / desc.planeWidth[plane];
    const int planeHeight = height / desc.planeHeight[plane];
    const uint8_t *srcBuf = src[plane];
    uint8_t *dstBuf = dst[plane];
    for (line = 0; line < planeHeight; ++line) {
      memcpy(dstBuf, srcBuf, planeWidth);
      srcBuf += srcStride[plane];
      dstBuf += dstStride[plane];
    }
  }

  return S_OK;
}
