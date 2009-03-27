/*
 * Copyrights (c):
 *     2001 - 2008 , Werner Freytag.
 *     2009, Haiku
 * Distributed under the terms of the MIT License.
 *
 * Original Author:
 *              Werner Freytag <freytag@gmx.de>
 */

#include "SettingsView.h"

#include <Alert.h>
#include <Button.h>
#include <FilePanel.h>
#include <iostream>
#include <Roster.h>
#include <stdio.h>
#include <String.h>
#include <string.h>
#include <StringView.h>

#include "ColorContainer.h"
#include "Panel.h"
#include "Prefs.h"

SettingsView::SettingsView( BRect bounds )
:	BBox( bounds )
{
	fPanel = new Panel( this );
}

SettingsView::~SettingsView() {
	delete fPanel;
}

void SettingsView::AttachedToWindow()
{

	BStringView *stringview;
	
	for (int i=0; i<4; ++i) {

		char string[8];
		sprintf(string, "Color %d", i+1);

		stringview = new BStringView( BRect( 0.0, 0.0, 45.0, 10.0).OffsetToCopy(10 + i * 55, 8 ), B_EMPTY_STRING, string);
		stringview->SetFontSize( 10.0 );
		AddChild( stringview );
	
		ColorContainer *container = new ColorContainer(
			BRect( 0.0, 0.0, 45.0, 30.0).OffsetToCopy( 10 + i * 55, 20 ), "", "", new BMessage('Col0'+i) );
		container->SetColor( prefs.fColor[i] );
		AddChild( container );
		container->SetTarget( (BLooper *)this );
		
	};

	stringview = new BStringView(
		BRect( 0.0, 0.0, 70.0, 20.0 ).OffsetToCopy(10.0, 58.0), B_EMPTY_STRING, "Color picker:");
	AddChild( stringview );
	
	BRect buttonrect = BRect( 0.0, 0.0, 120.0, 20.0 ).OffsetToCopy(80.0, 60.0);
	BButton *button = new BButton( buttonrect, "colorpickerbutton", prefs.fColorPickerPath.Leaf(), new BMessage('ColP'));
	AddChild( button );
	button->SetTarget( (BLooper *)this );
	button->ResizeTo( buttonrect.Width(), buttonrect.Height() );
	
	buttonrect = BRect( 0.0, 0.0, 20.0, 20.0 ).OffsetToCopy(200.0, 60.0);
	button = new BButton( buttonrect, "...", "...", new BMessage('ChnC'));
	AddChild( button );
	button->SetTarget( (BLooper *)this );
	button->ResizeTo( buttonrect.Width(), buttonrect.Height() );

}

void SettingsView::MessageReceived( BMessage *msg ) {

	msg->PrintToStream();
	
	switch (msg->what) {
		case 'Col0':
		case 'Col1':
		case 'Col2':
		case 'Col3':
		{

			rgb_color *color;
			ssize_t numBytes;
			if (msg->FindData("RGBColor", B_RGB_COLOR_TYPE, (const void **)&color, &numBytes)!=B_OK) return;
			prefs.fColor[ msg->what - 'Col0' ] = *color;
			prefs.Save();
			
		} break;
			
		case 'ChnC': {
			
			ShowFilePanel();
						
		} break;
		
		case 'ColP': {
			
			if (!prefs.fColorPickerPath.Path() || strlen(prefs.fColorPickerPath.Path())==0) {
				if (! (new BAlert("Warning",
					"You must have a color picker application to use this feature.\n\n"
					"I suggest 'Colors!' (of course ;-) ) or 'kColor' by Kelvin Sherlock. Both can be downloaded at BeBits.",
					"I have a color picker...", "I will come back later!"
				))->Go() )
				
				ShowFilePanel();
			}

			entry_ref	ref;
			get_ref_for_path( prefs.fColorPickerPath.Path(), &ref );
			be_roster->Launch( &ref, 0, 0 );
	
		} break;
		
		case 'Colp': {

			entry_ref	ref;
			if (msg->FindRef("refs", &ref)!=B_OK) return;
			
			// traverse
			BEntry	entry( &ref, true);
			entry.GetRef( &ref );
			
			// is it an app?
			entry_ref	app;
			be_roster->FindApp(&ref, &app);
			if (app != ref) return;
			
			prefs.fColorPickerPath.SetTo( &ref );
			
			BButton	*button = (BButton *)FindView("colorpickerbutton");
			if (!button) return;
			
			button->SetLabel( prefs.fColorPickerPath.Leaf() );
	
		} break;
		
		default:
			BView::MessageReceived( msg );
	}
}

void SettingsView::ShowFilePanel() {

	BPath	path;
	prefs.fColorPickerPath.GetParent(&path);
	
	if (path.Path()) fPanel->SetPanelDirectory(path.Path());
	fPanel->Show();

}
