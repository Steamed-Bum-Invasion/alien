#include "MapCompartmentImpl.h"

MapCompartmentImpl::MapCompartmentImpl(QObject * parent)
	: MapCompartment(parent)
{
}

void MapCompartmentImpl::init(SpaceMetric * metric, IntRect mapRect)
{
	_rect = mapRect;
}

IntVector2D MapCompartmentImpl::getSize() const
{
	return{ _rect.p2.x - _rect.p1.x, _rect.p2.y - _rect.p1.y };
}

void MapCompartmentImpl::registerNeighborContext(RelativeLocation location, UnitContext * context)
{
	_contextsByLocations[location] = context;
}

std::vector<UnitContext*> MapCompartmentImpl::getNeighborContexts() const
{
	std::vector<UnitContext*> result;
	for (auto const& contextByLocation : _contextsByLocations) {
		result.push_back(contextByLocation.second);
	}
	return result;
}

UnitContext* MapCompartmentImpl::convertAbsToRelPosition(IntVector2D& pos) const
{
	return nullptr;
}
