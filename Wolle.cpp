/*
 * Copyrights (c):
 *     2001 - 2008 , Werner Freytag.
 *     2009, Haiku
 * Distributed under the terms of the MIT License.
 *
 * Original Author:
 *              Werner Freytag <freytag@gmx.de>
 */

#include "Wolle.h"

#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <StringView.h>

#include "Prefs.h"
#include "SettingsView.h"

extern "C" _EXPORT BScreenSaver *instantiate_screen_saver(BMessage *msg, image_id id)
{
	return new Wolle(msg, id);
}

inline float rnd( float min, float max) {
	return (float)rand() / RAND_MAX * (max - min) + min;
}

Wolle::Wolle( BMessage *archive, image_id image )
	: 	BScreenSaver( archive, image )
{
	srand( system_time() );
}

void Wolle::StartConfig( BView *view )
{
	prefs.Load();

	BRect rect(0.0, 0.0, 240.0, 20.0 );
	
	rect.OffsetTo( 10, 10 );
	BStringView	*stringview;
	stringview = new BStringView( rect, B_EMPTY_STRING, "Wolle 2.0" );
	stringview->SetFont( be_bold_font );
	stringview->SetFontSize( 20.0 );
	view->AddChild( stringview );

	rect.OffsetTo( 10, 25 );
	stringview = new BStringView( rect, B_EMPTY_STRING, "Copyright ©2001 by Werner Freytag");
	stringview->SetHighColor( 100, 100, 100 );
	view->AddChild( stringview );

	SettingsView *settings_view = new SettingsView( BRect( 10.0, 70.0, 240.0, 160.0 ));
	view->AddChild( settings_view );

	rect.OffsetTo( 10, 210 );
	stringview = new BStringView( rect, B_EMPTY_STRING, "E-Mail: freytag@gmx.de");
	view->AddChild( stringview );

	rect.OffsetTo( 10, 225 );
	stringview = new BStringView( rect, B_EMPTY_STRING, "Homepage: http://www.pecora.de/");
	view->AddChild( stringview );
	
	rect.right = 80;

}

status_t Wolle::StartSaver( BView *view, bool preview )
{
	
	prefs.Load();
	
	fRadius = view->Bounds().Height() / 15;

	fSizeFactor = rnd( 0.75, 1.5 );
	fArc = rnd( 0, M_PI * 2 );
	fMoveX = rnd( view->Bounds().Width() / 100, view->Bounds().Width() / 60 ) * (rnd(0,1)>0.5 ? -1 : 1) * fSizeFactor;
	fMoveY = rnd( view->Bounds().Width() / 100, view->Bounds().Width() / 60 ) * (rnd(0,1)>0.5 ? -1 : 1) * fSizeFactor;
	fRotation = rnd( 0.2, 1 ) * (fRotation> 0 ? -1 : 1);
	fGrow = rnd( 0.98, 1.02 );
	
	fPosX = rnd( 0, view->Bounds().Width() - 2 * fRadius * fSizeFactor );
	fPosY = rnd( 0, view->Bounds().Height() - 2 * fRadius * fSizeFactor );

	SetTickSize(10000);
	return B_OK;
}

void Wolle::Draw( BView *view, int32 frame )
{

	if ( frame == 0 ) {
		view->SetDrawingMode( B_OP_ALPHA );
		view->SetPenSize(2);
	}
	
	bool	out_of_range;
	
	do {
		
		out_of_range	= false;
		
		// Backup old values
		float oldSizeFactor		= fSizeFactor;
		float oldArc			= fArc;
		float oldPosX			= fPosX;
		float oldPosY			= fPosY;
		
		// Calculate new values
		fSizeFactor *= fGrow;
		fArc	+= fRotation;
		fPosX	+= fMoveX;
		fPosY	+= fMoveY;
			
		BPoint	points[ 4 ];
		BRect	maxBounds(view->Bounds().InsetByCopy(-view->Bounds().Width()/20, -view->Bounds().Width()/20));
		
		for (int i = 0; i < 4; ++i) {
	
			float	arc = M_PI * 2 / 4 * i + fArc;
			points[i].x = (1 + sin(arc) ) * fRadius * fSizeFactor + fPosX;
			points[i].y = (1 + cos(arc) ) * fRadius * fSizeFactor + fPosY;
			
			if (!maxBounds.Contains(points[i])) {
				
				if (points[i].x < maxBounds.left || points[i].x > maxBounds.right) {
					fMoveX = rnd( view->Bounds().Width() / 100, +view->Bounds().Width() / 60 ) * (fMoveX > 0 ? -1 : 1) * fSizeFactor;
					fMoveY = rnd( 0.5, 3) * fMoveY;
				}
				else {
					fMoveY = rnd( view->Bounds().Height() / 100, +view->Bounds().Height() / 60 ) * (fMoveY > 0 ? -1 : 1) * fSizeFactor;
					fMoveX = rnd( 0.5, 3) * fMoveX;
				}
				
				fRotation = rnd( 0.2, 1 ) * (fRotation> 0 ? -1 : 1);

				fGrow = fSizeFactor > 1.5 ? rnd( 0.98, 1.0 ) :
						fSizeFactor < 0.75 ? rnd( 1.0, 1.02) :
						rnd( 0.98, 1.02);

				fSizeFactor		= oldSizeFactor;
				fArc			= oldArc;
				fPosX			= oldPosX;
				fPosY			= oldPosY;
	
				out_of_range = true; break;
			}
		}
	
		if (!out_of_range) {

			BPoint	betweenPoints[ 4 ];
			
			float	arc = oldArc;
	
			betweenPoints[0].x = (1 + sin(arc) ) * fRadius * oldSizeFactor + oldPosX;
			betweenPoints[0].y = (1 + cos(arc) ) * fRadius * oldSizeFactor + oldPosY;
			
			betweenPoints[3] = points[3];

			arc = M_PI * 2 / 4 * 1 + oldArc;
			betweenPoints[1].x = 2 * betweenPoints[0].x - ((1 + sin(arc) ) * fRadius * oldSizeFactor + oldPosX);
			betweenPoints[1].y = 2 * betweenPoints[0].y - ((1 + cos(arc) ) * fRadius * oldSizeFactor + oldPosY);

			arc = M_PI * 2 / 4 * 2 + fArc;
			betweenPoints[2].x = 2 * betweenPoints[3].x - ((1 + sin(arc) ) * fRadius * fSizeFactor + fPosX);
			betweenPoints[2].y = 2 * betweenPoints[3].y - ((1 + cos(arc) ) * fRadius * fSizeFactor + fPosY);
			
			view->SetHighColor( 0, 0, 0, 100 );
			BPoint	shadowPoints[ 4 ];
			for (int j=0; j<4; ++j) shadowPoints[j] = BPoint(betweenPoints[j].x+1, betweenPoints[j].y+1);
			view->StrokeBezier( shadowPoints );
			for (int j=0; j<4; ++j) shadowPoints[j] = BPoint(points[j].x+1, points[j].y+1);
			view->StrokeBezier( shadowPoints );
			
			int r1 = prefs.fColor[(frame / 128) % 4].red;
			int r2 = prefs.fColor[(frame / 128 + 1) % 4].red;
			int g1 = prefs.fColor[(frame / 128) % 4].green;
			int g2 = prefs.fColor[(frame / 128 + 1) % 4].green;
			int b1 = prefs.fColor[(frame / 128) % 4].blue;
			int b2 = prefs.fColor[(frame / 128 + 1) % 4].blue;

			rgb_color color = {
				r1 + (int)((float)(r2 - r1) / 128 * (frame % 128)),
				g1 + (int)((float)(g2 - g1) / 128 * (frame % 128)),
				b1 + (int)((float)(b2 - b1) / 128 * (frame % 128)),
				100 };
				
			view->SetHighColor( color );
			view->StrokeBezier( betweenPoints );
			view->StrokeBezier( points );
			
		}			
	
	} while (out_of_range);
}
