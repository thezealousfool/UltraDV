//---------------------------------------------------------------------
//
//	File:	TSorterListItem.cpp
//
//	Author:	Gene Z. Ragan
//
//	Date:	05.20.98
//
//	Desc:	A SorterList is single vertical display of an element.  It has
//			a toolbar at the top that can be clicked, resized and moved.
//			These actions send a message to all other Sorters in the
//			SorterContainer.
//			
//
//	Copyright ©1998 mediapede Software
//
//---------------------------------------------------------------------

// Includes
#include "BuildApp.h"

#include <app/Application.h>
#include <support/Debug.h>

#include "AppConstants.h"
#include "AppUtils.h"
#include "ResourceManager.h"

#include "TSorterContainer.h"

#include "TSorterListItem.h"

//---------------------------------------------------------------------
//	Constructor
//---------------------------------------------------------------------
//
//

TSorterListItem::TSorterListItem(uint32 level, bool expanded, BRect bounds, const char *theString, SorterType theType, entry_ref& fileRef) : 
				 BListItem(level, expanded)
{
	m_Cue		= NULL;
	m_Type 		= theType;
	m_EntryRef	= fileRef;
	
	// Make a copy of the data string
	m_DataString = (char *) malloc(strlen(theString)+1);       
	strcpy(m_DataString, theString);
		
	// Perform default initialization
	Init();
}


//---------------------------------------------------------------------
//	Destructor
//---------------------------------------------------------------------
//
//

TSorterListItem::~TSorterListItem()
{
	// Free m_DataString
	free(m_DataString);
}


//---------------------------------------------------------------------
//	Init
//---------------------------------------------------------------------
//
//	Perform default initialization tasks

void TSorterListItem::Init()
{
	// Set DataType based on entry_ref
	BFile theFile; 
	if ( theFile.SetTo(&m_EntryRef, B_READ_WRITE) == B_OK )
	{		
		// Create node
		BNodeInfo nodeInfo(&theFile);
		if (nodeInfo.InitCheck() == B_NO_ERROR)
		{
			if (IsAudio(nodeInfo))
				m_DataType = kAudioType;
			else if (IsImage(nodeInfo))
				m_DataType = kPictureType;
			else if (IsText(nodeInfo))
				m_DataType = kTextType;
			else if (IsVideo(nodeInfo))
				m_DataType = kVideoType;
		}
		else
		{
			m_DataType = kUnknownType;	
		}	
		
		theFile.Unset();
	}
}


//---------------------------------------------------------------------
//	Init
//---------------------------------------------------------------------
//
//	Perform default initialization tasks

void	TSorterListItem::Update(BView *owner, const BFont *font)
{
	// 	This is a cheap hack to get the height of the listItem set 
	//	properly.  We aren't actually using a font this large.
	BFont theFont;
	owner->GetFont(&theFont);
	theFont.SetSize(kSorterFontSize + 6);	

	BListItem::Update(owner, &theFont);
}

#pragma mark -
#pragma mark === Drawing Routines ===

//---------------------------------------------------------------------
//	Draw
//---------------------------------------------------------------------
//
//

void TSorterListItem::DrawItem(BView *owner, BRect itemRect, bool drawEverything)
{
	owner->PushState();
	
	owner->SetHighColor(owner->ViewColor());
	owner->FillRect(itemRect);
	
	// Draw our data string
	owner->SetHighColor(kBlack);
	BPoint textPt(itemRect.left+kSorterTextOffset, itemRect.bottom - 2);
	owner->DrawString(m_DataString, textPt);
		
	// Is it selected
	if ( IsSelected() && m_Type == kNameSorter)
	{
		itemRect.top = itemRect.bottom - (kSorterFontSize+2);
		itemRect.left += (kSorterTextOffset-2);
		float width = owner->StringWidth(m_DataString);
		itemRect.right = itemRect.left + width + 2;
		owner->InvertRect(itemRect);
	}
	
	owner->PopState();
}
