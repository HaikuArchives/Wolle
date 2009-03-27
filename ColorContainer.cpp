/*
 * Copyrights (c):
 *     2001 - 2008 , Werner Freytag.
 *     2009, Haiku
 * Distributed under the terms of the MIT License.
 *
 * Original Author:
 *              Werner Freytag <freytag@gmx.de>
 */
 
#include "ColorContainer.h"

#include <Bitmap.h>
#include <Entry.h>
#include <iostream>
#include <MessageRunner.h>
#include <Resources.h>
#include <Roster.h>
#include <stdio.h>
#include <String.h>
#include <Window.h>

ColorContainer::ColorContainer( BRect frame, const char *name,
	const char *label, BMessage *message, uint32 resizeMask, uint32 flags)

:	BControl( frame, name, label, message, resizeMask, flags ),
	fMouseDown( false ),
	fMessageRunner( 0 )
{
}

void  ColorContainer::Draw( BRect updateRect ) {
		
	BRect rect(Bounds());

	SetHighColor( 0x98, 0x98, 0x98 );
	StrokeLine( rect.RightTop(), rect.LeftTop() );
	StrokeLine( rect.LeftBottom() );

	SetHighColor( 0xFF, 0xFF, 0xFF );
	StrokeLine( rect.RightBottom() );
	StrokeLine( rect.RightTop() );
		
	rect.InsetBy(1.0, 1.0);

	SetHighColor( fColor );
	FillRect( rect );

	SetDrawingMode( B_OP_ALPHA );
	SetHighColor( 0xFF, 0xFF, 0xFF, 100 );
	StrokeLine( rect.RightTop(), rect.LeftTop() );
	StrokeLine( rect.LeftBottom() );

	SetHighColor( 0x00, 0x00, 0x00, 100 );
	StrokeLine( rect.RightBottom() );
	StrokeLine( rect.RightTop() );
	SetDrawingMode( B_OP_COPY );

}

void  ColorContainer::MessageReceived( BMessage *message ) {

	if (message->what==MSG_MESSAGERUNNER) {

		BPoint	where;
		uint32	buttons;

		GetMouse( &where, &buttons );

		DragColor( where );

	}
	else {

		char *nameFound;
		type_code typeFound;
		
		if (message->GetInfo(B_RGB_COLOR_TYPE, 0, &nameFound, &typeFound)!=B_OK) {
			BView::MessageReceived( message );
			return;
		}
		
	   	rgb_color	*color;
		ssize_t		numBytes;
		message->FindData(nameFound, typeFound, (const void **)&color, &numBytes);
		SetColor( *color );

		Invoke();
	}
}

void  ColorContainer::MouseDown( BPoint where ) {

	fMouseDown = true;
	
	fMessageRunner = new BMessageRunner(this, new BMessage(MSG_MESSAGERUNNER), 300000, 1);
	
	SetMouseEventMask(B_POINTER_EVENTS, B_SUSPEND_VIEW_FOCUS|B_LOCK_WINDOW_FOCUS );

}
			
void  ColorContainer::MouseUp( BPoint where ) {

	delete fMessageRunner;
	fMessageRunner = 0;

	fMouseDown = false;
	BView::MouseUp( where );

}

void ColorContainer::MouseMoved( BPoint where, uint32 code, const BMessage *a_message) {

	if (fMouseDown) DragColor( where );

}

status_t ColorContainer::Invoke(BMessage *msg) {

	if (!msg) msg = Message();
	if (!msg) return B_ERROR;

	FillMessage( msg );

	return BControl::Invoke( msg );
}

void  ColorContainer::DragColor( BPoint where ) {

	BMessage msg(B_PASTE);
	FillMessage( &msg );

	BRect rect( 0.0, 0.0, 20.0, 20.0 );

	BBitmap *bmp = new BBitmap(rect, B_RGB32, true);
	bmp->Lock();
	
	BView *view = new BView( rect, "", B_FOLLOW_NONE, B_WILL_DRAW );
	bmp->AddChild(view);

	view->SetHighColor(B_TRANSPARENT_COLOR);
	view->FillRect( view->Bounds() );
	
	++rect.top; ++rect.left;
	view->SetHighColor(0, 0, 0, 100);
	view->FillRect( rect );
	rect.OffsetBy(-1.0, -1.0);

	view->SetHighColor(min_c(255, 1.2 * fColor.red + 40), min_c(255, 1.2 * fColor.green + 40), min_c(255, 1.2 * fColor.blue + 40));
	view->StrokeRect(rect);

	++rect.left; ++rect.top;
	view->SetHighColor(0.8 * fColor.red, 0.8 * fColor.green, 0.8 * fColor.blue);
	view->StrokeRect(rect);

	--rect.right; --rect.bottom;
	view->SetHighColor(fColor.red, fColor.green, fColor.blue);
	view->FillRect(rect);
	
	view->Flush();
	
	bmp->Unlock();

	DragMessage(&msg, bmp, B_OP_ALPHA, BPoint(14.0, 14.0) );

	MouseUp( where );

}

void ColorContainer::FillMessage( BMessage *msg ) {
	char hexstr[7];
	sprintf(hexstr, "#%.2X%.2X%.2X", fColor.red, fColor.green, fColor.blue);

	msg->RemoveName("text/plain");
	msg->AddData("text/plain", B_MIME_TYPE, &hexstr, sizeof(hexstr));

	msg->RemoveName("RGBColor");
	msg->AddData("RGBColor", B_RGB_COLOR_TYPE, &fColor, sizeof(fColor));
}

void ColorContainer::SetColor( long int c ) {

	rgb_color color;
	color.red	= (c>>16) & 255;
	color.green	= (c>>8) & 255;
	color.blue	= c & 255;
	SetColor( color );
}


void  ColorContainer::SetColor( rgb_color color ) {
	color.alpha	= 255;
	fColor = color;
	if (Window()) Draw(Bounds());
}

rgb_color ColorContainer::GetColor() {
	return fColor;
}
