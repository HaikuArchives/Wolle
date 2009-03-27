/*
 * Copyrights (c):
 *     2001 - 2008 , Werner Freytag.
 *     2009, Haiku
 * Distributed under the terms of the MIT License.
 *
 * Original Author:
 *              Werner Freytag <freytag@gmx.de>
 */

#include "Prefs.h"

#include <Directory.h>
#include <Entry.h>
#include <File.h>
#include <FindDirectory.h>
#include <Path.h>

#include <iostream>

#define SET_COLOR(c, r, g, b) { c.red = r; c.green = g; c.blue = b; c.alpha = 255; }

Prefs prefs;

Prefs::Prefs()
{

	// FindPath
	find_directory(B_USER_SETTINGS_DIRECTORY, &fSavePath);
	fSavePath.SetTo( fSavePath.Path(), PREFS_FILENAME );
	
	// Einstellungen einlesen
	BFile file( fSavePath.Path(), B_READ_ONLY );
	Unflatten( &file );

	Load();
}

Prefs::~Prefs() {

	Save();
	
}

void Prefs::Load() {

	// vordefinierte Einstellungen	
	SET_COLOR(fColor[0], 0xFF, 0x8B, 0x4C );
	SET_COLOR(fColor[1], 0x54, 0xFF, 0x4B );
	SET_COLOR(fColor[2], 0x4C, 0x83, 0xFF );
	SET_COLOR(fColor[3], 0xFF, 0x4C, 0xF9 );

	fColorPickerPath = "/app/Colors!/Colors!";
	
	// gespeicherte Einstellungen einlesen
	for (int i=0; i<4; ++i) {
		rgb_color *color;
		ssize_t	size = sizeof(color);
		if (FindData("color", B_RGB_COLOR_TYPE, i, (const void **)&color, &size)==B_OK) {
			fColor[i] = *color;
		}
	}
	
	const char *pickerpath;
	if (FindString("pickerpath", &pickerpath)==B_OK) {
		fColorPickerPath = pickerpath;
	}
	
	// testen, ob Pfad ok
	entry_ref	ref;
	if (get_ref_for_path( fColorPickerPath.Path(), &ref )!=B_OK) fColorPickerPath = "";

}

void Prefs::Save() {

	// Einstellungen ablegen
	RemoveName("color");
	for (int i=0; i<4; ++i) {
		AddData("color", B_RGB_COLOR_TYPE, (const void **)&fColor[i], sizeof(fColor[i]));
		rgb_color color = fColor[i];
	}

	RemoveName("pickerpath");
	AddString("pickerpath", fColorPickerPath.Path());

	// ...und speichern
	BFile		file( fSavePath.Path(), B_WRITE_ONLY|B_CREATE_FILE );
	Flatten( &file );
}
