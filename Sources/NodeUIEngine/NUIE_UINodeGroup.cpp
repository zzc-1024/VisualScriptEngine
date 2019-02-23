#include "NUIE_UINodeGroup.hpp"
#include "NUIE_SkinParams.hpp"

#include <algorithm>

namespace NUIE
{

NE::DynamicSerializationInfo UINodeGroup::serializationInfo (NE::ObjectId ("{FCC7498F-03C5-49C8-B4C0-C88D768F18CD}"), NE::ObjectVersion (1), UINodeGroup::CreateSerializableInstance);

GroupDrawingImage::GroupDrawingImage ()
{

}

GroupDrawingImage::~GroupDrawingImage ()
{

}

void GroupDrawingImage::Reset ()
{
	Clear ();
	rect = Rect ();
}

const Rect& GroupDrawingImage::GetRect () const
{
	return rect;
}

void GroupDrawingImage::SetRect (const Rect& newRect)
{
	rect = newRect;
}

NodeRectGetter::NodeRectGetter ()
{

}

NodeRectGetter::~NodeRectGetter ()
{

}

UINodeGroup::UINodeGroup () :
	UINodeGroup (L"")
{

}

UINodeGroup::UINodeGroup (const std::wstring& name) :
	NE::NodeGroup (),
	name (name)
{

}

UINodeGroup::~UINodeGroup ()
{

}

const std::wstring& UINodeGroup::GetName () const
{
	return name;
}

void UINodeGroup::SetName (const std::wstring& newName)
{
	name = newName;
	InvalidateGroupDrawing ();
}

Rect UINodeGroup::GetRect (NodeUIDrawingEnvironment& env, const NodeRectGetter& rectGetter, const NE::NodeCollection& nodes) const
{
	return GetDrawingImage (env, rectGetter, nodes).GetRect ();
}

void UINodeGroup::Draw (NodeUIDrawingEnvironment& env, const NodeRectGetter& rectGetter, const NE::NodeCollection& nodes) const
{
	DrawingContext& drawingContext = env.GetDrawingContext ();
	GetDrawingImage (env, rectGetter, nodes).Draw (drawingContext);
}

void UINodeGroup::InvalidateGroupDrawing () const
{
	drawingImage.Reset ();
}

NE::Stream::Status UINodeGroup::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	NE::NodeGroup::Read (inputStream);
	inputStream.Read (name);
	return inputStream.GetStatus ();
}

NE::Stream::Status UINodeGroup::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	NE::NodeGroup::Write (outputStream);
	outputStream.Write (name);
	return outputStream.GetStatus ();
}

const GroupDrawingImage& UINodeGroup::GetDrawingImage (NodeUIDrawingEnvironment& env, const NodeRectGetter& rectGetter, const NE::NodeCollection& nodes) const
{
	if (drawingImage.IsEmpty ()) {
		UpdateDrawingImage (env, rectGetter, nodes);
	}
	return drawingImage;
}

void UINodeGroup::UpdateDrawingImage (NodeUIDrawingEnvironment& env, const NodeRectGetter& rectGetter, const NE::NodeCollection& nodes) const
{
	const SkinParams& skinParams = env.GetSkinParams ();
	DrawingContext& drawingContext = env.GetDrawingContext ();

	BoundingRectCalculator boundingRectCalculator;
	nodes.Enumerate ([&] (const NE::NodeId& nodeId) {
		Rect nodeRect = rectGetter.GetNodeRect (nodeId);
		boundingRectCalculator.AddRect (nodeRect);
		return true;
	});

	double groupPadding = skinParams.GetGroupPadding ();

	Size textSize = drawingContext.MeasureText (skinParams.GetGroupNameFont (), name);
	Rect boundingRect = boundingRectCalculator.GetRect ();
	double maxWidth = std::max (textSize.GetWidth (), boundingRect.GetWidth ());
	Rect fullRect = Rect::FromPositionAndSize (
		boundingRect.GetTopLeft () - Point (groupPadding, 2.0 * groupPadding + textSize.GetHeight ()),
		Size (maxWidth, boundingRect.GetHeight ()) + Size (2.0 * groupPadding, 3.0 * groupPadding + textSize.GetHeight ())
	);

	Rect textRect = Rect::FromPositionAndSize (
		Point (fullRect.GetX () + groupPadding, fullRect.GetY () + groupPadding),
		Size (maxWidth, textSize.GetHeight ())
	);

	drawingImage.SetRect (fullRect);
	drawingImage.AddItem (DrawingItemPtr (new DrawingFillRect (fullRect, skinParams.GetGroupBackgroundColor ())));
	drawingImage.AddItem (DrawingItemPtr (new DrawingText (textRect, skinParams.GetGroupNameFont (), name, HorizontalAnchor::Left, VerticalAnchor::Center, skinParams.GetGroupNameColor ())));
}

}
