/*
 * Copyrights (c):
 *     2001 - 2008 , Werner Freytag.
 *     2009, Haiku
 * Distributed under the terms of the MIT License.
 *
 * Original Author:
 *              Werner Freytag <freytag@gmx.de>
 */

#ifndef _PREFS_H
#define _PREFS_H

#include <Path.h>
#include <GraphicsDefs.h>

#define	PREFS_FILENAME	"Wolle_settings"

class Prefs : public BMessage {

public:
					Prefs();
					~Prefs();

void				Load();
void				Save();

BPath				fColorPickerPath;
rgb_color			fColor[4];

private:

BPath				fSavePath;

};

extern Prefs prefs;
	
#endif
