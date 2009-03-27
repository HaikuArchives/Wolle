/*
 * Copyrights (c):
 *     2001 - 2008 , Werner Freytag.
 *     2009, Haiku
 * Distributed under the terms of the MIT License.
 *
 * Original Author:
 *              Werner Freytag <freytag@gmx.de>
 */

#ifndef _COLOR_CONTAINER_H
#define _COLOR_CONTAINER_H

#include <Control.h>

#define MSG_MESSAGERUNNER	'-MsR'

class BMessageRunner;

class ColorContainer : public BControl {

public:
					ColorContainer( BRect frame,
						const char *name = NULL,
						const char *label = NULL,
						BMessage *message = NULL,
						uint32 resizeMask = B_FOLLOW_TOP|B_FOLLOW_LEFT,
						uint32 flags = B_WILL_DRAW );

virtual void		MessageReceived(BMessage *message);

virtual void		MouseDown(BPoint where);
virtual void		MouseUp(BPoint where);
virtual void		MouseMoved( BPoint where, uint32 code, const BMessage *a_message);

virtual void		Draw( BRect updateRect );

virtual void		SetColor( rgb_color color );
virtual void		SetColor( long int color );

virtual	status_t	Invoke(BMessage *msg = NULL);

rgb_color			GetColor();

protected:

virtual void		DragColor( BPoint where );
virtual void		FillMessage( BMessage *msg );

private:

bool				fMouseDown;
BMessageRunner		*fMessageRunner;
rgb_color			fColor;

};

#endif
