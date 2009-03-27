/*
 * Copyrights (c):
 *     2001 - 2008 , Werner Freytag.
 *     2009, Haiku
 * Distributed under the terms of the MIT License.
 *
 * Original Author:
 *              Werner Freytag <freytag@gmx.de>
 */

#ifndef _PANEL_H
#define _PANEL_H

#include <FilePanel.h>

class Panel : public BFilePanel {

public:

			Panel( BHandler *handler );
			
void		SendMessage(const BMessenger*, BMessage*);

private:

BHandler	*fHandler;

};

#endif
