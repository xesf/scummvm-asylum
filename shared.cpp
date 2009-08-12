/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * $URL$
 * $Id$
 *
 */

#include "asylum/shared.h"

#include "common/system.h"

DECLARE_SINGLETON(Asylum::SharedResources);

namespace Asylum {

static bool g_initialized = false;

SharedResources::SharedResources() {
	if (!g_initialized) {
		g_initialized = true;
	}
    memset(_gameFlags, 0, 1512);
}

SharedResources::~SharedResources() {
	g_initialized = false;
}

bool SharedResources::pointInPoly(PolyDefinitions *poly, int x, int y) {
	// Copied from backends/vkeybd/polygon.cpp
	int  yflag0;
	int  yflag1;
	bool inside_flag = false;
	unsigned int pt;

	Common::Point *vtx0 = &poly->points[poly->numPoints - 1];
	Common::Point *vtx1 = &poly->points[0];

	yflag0 = (vtx0->y >= y);
	for (pt = 0; pt < poly->numPoints; pt++, vtx1++) {
		yflag1 = (vtx1->y >= y);
		if (yflag0 != yflag1) {
			if (((vtx1->y - y) * (vtx0->x - vtx1->x) >=
				(vtx1->x - x) * (vtx0->y - vtx1->y)) == yflag1) {
				inside_flag = !inside_flag;
			}
		}
		yflag0 = yflag1;
		vtx0   = vtx1;
	}

	return inside_flag;
}

void SharedResources::setGameFlag(int flag) {
	_gameFlags[flag / 32] |= 1 << flag % -32;
}

void SharedResources::clearGameFlag(int flag) {
	_gameFlags[flag / 32] &= ~(1 << flag % -32);
}

void SharedResources::toggleGameFlag(int flag) {
	_gameFlags[flag / 32] ^= 1 << flag % -32;
}

bool SharedResources::isGameFlagSet(int flag) {
	return ((1 << flag % -32) & (unsigned int)_gameFlags[flag / 32]) >> flag % -32 != 0;
}

bool SharedResources::isGameFlagNotSet(int flag) {
	return ((1 << flag % -32) & (unsigned int)_gameFlags[flag / 32]) >> flag % -32 == 0;
}

} // end of namespace Asylum
