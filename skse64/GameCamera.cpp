#include "GameCamera.h"

void LocalMapCamera::SetDefaultStateMinFrustumDimensions(float width, float height)
{
	defaultState->minFrustumWidth = width / 2.0;
	defaultState->minFrustumHeight = height / 2.0;
}

void LocalMapCamera::SetAreaBounds(NiPoint3 * maxBound, NiPoint3 * minBound)
{
	areaBoundsMin = *minBound;
	areaBoundsMax = *maxBound;
	areaBoundsMax.z += (*g_mapLocalHeight);
}

void LocalMapCamera::SetDefaultStateMaxBound(NiPoint3 * maxBound)
{
	defaultState->someBoundMax = *maxBound;
	defaultState->someBoundMax.z += (*g_mapLocalHeight);
}

void LocalMapCamera::SetDefaultStateBounds(float x, float y, float z)
{
	defaultState->someBoundMin.x = x - defaultState->someBoundMax.x;
	defaultState->someBoundMin.y = y - defaultState->someBoundMax.y;
	defaultState->someBoundMin.z = z - defaultState->someBoundMax.z;
}
