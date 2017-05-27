#include "object.h"

object::object(vec4 pPosition,ShapeID pID,Material pMatType,float pRefFact)
	:
	mPosition(pPosition),
	mID(pID),
	mMatType(pMatType),
	mRefFact(pRefFact)
{
}
