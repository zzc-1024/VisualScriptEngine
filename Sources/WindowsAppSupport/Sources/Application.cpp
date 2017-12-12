#include "Application.hpp"
#include <gdiplusbase.h>
#include <assert.h>

class GdiplusInitializer
{
public:
	GdiplusInitializer ()
	{
		Gdiplus::GdiplusStartupInput input;
		Gdiplus::GdiplusStartupOutput output;
		Gdiplus::Status status = Gdiplus::GdiplusStartup (&gdiPlusToken, &input, &output);
		assert (status == Gdiplus::Ok);
	}

	~GdiplusInitializer ()
	{
		Gdiplus::GdiplusShutdown (gdiPlusToken);
	}

private:
	ULONG_PTR	gdiPlusToken;
};

static GdiplusInitializer gdiplusInitializer;

Application::Application ()
{
}

Application::~Application ()
{
}
