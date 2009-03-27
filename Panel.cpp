/*
 * Copyrights (c):
 *     2001 - 2008 , Werner Freytag.
 *     2009, Haiku
 * Distributed under the terms of the MIT License.
 *
 * Original Author:
 *              Werner Freytag <freytag@gmx.de>
 */


/* This is necessary because the messages are always sent to a looper -
 * the screensaver application, which doesn't know, what to do!
 */

#include "Panel.h"

#include <Handler.h>
#include <Looper.h>
#include <Message.h>

Panel::Panel( BHandler *handler )
:	BFilePanel(B_OPEN_PANEL, 0, 0, 0, false, new BMessage('Colp')),
	fHandler( handler )
{
}
			
void Panel::SendMessage(const BMessenger *messenger, BMessage *message) {
	if (message->what == 'Colp') {
		fHandler->Looper()->PostMessage( message, fHandler );
	}
	else
		BFilePanel::SendMessage( messenger, message );
}
