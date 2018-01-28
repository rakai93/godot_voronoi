#include "register_types.h"
#include "voronoi.h"

void register_voronoi_types() {

	ObjectTypeDB::register_type<Voronoi>();
	ObjectTypeDB::register_type<VoronoiDiagram>();
	ObjectTypeDB::register_type<VoronoiSite>();
	ObjectTypeDB::register_type<VoronoiEdge>();

}

void unregister_voronoi_types() {

}

