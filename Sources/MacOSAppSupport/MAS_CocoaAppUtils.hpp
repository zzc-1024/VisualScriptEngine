#ifndef MAS_COCOAAPPUTILS_HPP
#define MAS_COCOAAPPUTILS_HPP

#include "NUIE_Drawing.hpp"
#include "NUIE_Geometry.hpp"
#include "NUIE_InputEventHandler.hpp"
#include "NUIE_MenuCommands.hpp"
#include "MAS_IncludeCocoaHeaders.hpp"

namespace MAS
{

NSString*					StdWStringToNSString (const std::wstring& str);
	
NUIE::ModifierKeys			GetModifierKeysFromEvent (const NSEvent* event);
NUIE::Point					GetViewPositionFromEvent (const NSView* view, const NSEvent* event);

NSPoint						CreatePoint (CGFloat height, const NUIE::Point& point);
NSRect						CreateRect (CGFloat viewHeight, const NUIE::Rect& rect);
NSColor*					CreateColor (const NUIE::Color& color);

NSPoint						CreateScreenPoint (const NSView* view, const NUIE::Point& point);

NSImage*					FlipImageVertically (const NSImage* image);
NSImage*					FlipImageHorizontally (const NSImage* image);
	
NUIE::MenuCommandPtr		SelectCommandFromContextMenu (const NSView* nsView, const NUIE::Point& position, const NUIE::MenuCommandStructure& commands);
	
}

#endif
