/*
 * Copyrights (c):
 *     2001 - 2008 , Werner Freytag.
 *     2009, Haiku
 * Distributed under the terms of the MIT License.
 *
 * Original Author:
 *              Werner Freytag <freytag@gmx.de>
 */

#ifndef _SETTINGS_VIEW_H
#define _SETTINGS_VIEW_H

#include <Box.h>
#include <Messenger.h>
#include <Looper.h>

class Panel;

class SettingsView : public BBox {

public:

		SettingsView( BRect bounds );
		~SettingsView();
		
void	AttachedToWindow();
void	MessageReceived( BMessage *msg );

void	ShowFilePanel();

private:

Panel	*fPanel;
};

#endif
