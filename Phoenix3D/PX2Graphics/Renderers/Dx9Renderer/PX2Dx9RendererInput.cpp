// PX2Dx9RendererInput.cpp

#include "PX2Dx9RendererInput.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
PdrRendererInput::PdrRendererInput() :
mDriver(0)
{
}
//----------------------------------------------------------------------------
PdrRendererInput::~PdrRendererInput()
{
}
//----------------------------------------------------------------------------
bool PdrRendererInput::Ternamate()
{
	if (mDriver)
		mDriver->Release();

	mDriver = 0;

	return true;
}
//----------------------------------------------------------------------------