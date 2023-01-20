#include "pch.h"
#include "MousePoint.h"

namespace Shambles
{
	MousePoint* MousePoint::_MousePoint = nullptr;

	MousePoint* MousePoint::GetInstance()
	{
		if (_MousePoint == nullptr)
		{
			_MousePoint = new MousePoint();
		}
		return _MousePoint;
	}
}