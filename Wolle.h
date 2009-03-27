/*
 * Copyrights (c):
 *     2001 - 2008 , Werner Freytag.
 *     2009, Haiku
 * Distributed under the terms of the MIT License.
 *
 * Original Author:
 *              Werner Freytag <freytag@gmx.de>
 */

#ifndef _WOLLE_H
#define _WOLLE_H

#include <ScreenSaver.h>

#include <Path.h>


class ColorContainer;

class Wolle : public BScreenSaver {

public:

	Wolle( BMessage *archive, image_id image );
	
	void			StartConfig( BView *view );
	status_t		StartSaver( BView *view, bool preview );
	void			Draw( BView *view, int32 frame );

private:

	BPoint			fPoints[4];
	float			fRadius;
	float			fPosX, fPosY, fArc, fSizeFactor;
	float			fMoveX, fMoveY, fRotation, fGrow;

};

#endif
