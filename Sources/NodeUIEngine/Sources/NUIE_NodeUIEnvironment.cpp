#include "NUIE_NodeUIEnvironment.hpp"

namespace NUIE
{

NodeUIDrawingEnvironment::NodeUIDrawingEnvironment ()
{

}

NodeUIDrawingEnvironment::~NodeUIDrawingEnvironment ()
{

}

NodeUICalculationEnvironment::NodeUICalculationEnvironment ()
{

}

NodeUICalculationEnvironment::~NodeUICalculationEnvironment ()
{

}

NodeUIInteractionEnvironment::NodeUIInteractionEnvironment ()
{

}

NodeUIInteractionEnvironment::~NodeUIInteractionEnvironment ()
{

}

NodeUIEnvironment::NodeUIEnvironment ()
{

}

NodeUIEnvironment::~NodeUIEnvironment ()
{

}

NodeUIDrawingEnvironmentDecorator::NodeUIDrawingEnvironmentDecorator (NodeUIDrawingEnvironment& decorated) :
	NodeUIDrawingEnvironment (),
	decorated (decorated)
{

}

NodeUIDrawingEnvironmentDecorator::~NodeUIDrawingEnvironmentDecorator ()
{

}

DrawingContext& NodeUIDrawingEnvironmentDecorator::GetDrawingContext ()
{
	return decorated.GetDrawingContext ();
}

SkinParams& NodeUIDrawingEnvironmentDecorator::GetSkinParams ()
{
	return decorated.GetSkinParams ();
}

NodeUIDrawingEnvironmentContextDecorator::NodeUIDrawingEnvironmentContextDecorator (NodeUIDrawingEnvironment& decorated, DrawingContext& decoratedDrawingContext) :
	NodeUIDrawingEnvironmentDecorator (decorated),
	decoratedDrawingContext (decoratedDrawingContext)
{

}

NodeUIDrawingEnvironmentContextDecorator::~NodeUIDrawingEnvironmentContextDecorator ()
{

}

DrawingContext& NodeUIDrawingEnvironmentContextDecorator::GetDrawingContext ()
{
	return decoratedDrawingContext;
}

}
