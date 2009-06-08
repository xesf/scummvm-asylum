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
 */

#include "asylum/resourcemanager.h"
#include "asylum/resources/palette.h"

namespace Asylum {

ResourceManager::ResourceManager() {

}

ResourceManager::~ResourceManager() {

}

int ResourceManager::addBundle(Common::String filename) {
	Common::File* file = new Common::File;
	Bundle* bun = new Bundle;

	bun->id = filename;

	// load the file
	if (!file || !file->open(filename)) {
		printf("Failed to load file %s", filename.c_str());
		return -1;
	}

	bun->size = file->size();

	// read the entry count
	file->read( &bun->numEntries, 4 );

	// create the resource item array and
	// set the item offset for each entry
	for (uint8 i = 0; i < bun->numEntries; i++) {
		Resource *res = new Resource;
		file->read(&res->offset, 4);
		bun->entries.push_back(*res);
	}

	// set the last entry's offset to the filesize
	bun->entries[bun->numEntries - 1].offset = bun->size - file->pos();

	// calculate each entry's size based on the offset
	// information
	for (uint8 j = 0; j < bun->numEntries; j++) {
		if (bun->entries[j].offset == 0) {
			bun->entries[j].size = 0;
			continue;
		}

		bun->entries[j].size = getNextValidOffset(bun, j+1) - bun->entries[j].offset;
	}

	// populate the data
	for (uint8 k = 0; k < bun->numEntries; k++) {
		if (bun->entries[k].size > 0) {
			bun->entries[k].data = (uint8*)malloc(bun->entries[k].size);
			file->seek(bun->entries[k].offset, SEEK_SET);
			file->read(bun->entries[k].data, bun->entries[k].size);
		}
	}

	file->close();
	file = NULL;

	return 0;
}

GraphicBundle ResourceManager::getGraphic(Common::String file, uint32 offset) {
	GraphicBundle *bun = new GraphicBundle(file, offset);

	return *bun;
}

PaletteResource ResourceManager::getPalette(Common::String file, uint32 offset) {
	Bundle *bun = new Bundle(file, offset);

	PaletteResource *pal = new PaletteResource(bun->getData());

	delete bun;

	return *pal;
}

uint32 ResourceManager::getNextValidOffset(Bundle *bun, uint8 pos) {
	for (uint8 i = pos; i < bun->numEntries; i++) {
		if (bun->entries[i].offset != 0) {
			return bun->entries[i].offset;
		}
	}

	return bun->size;
}

} // end of namespace Asylum