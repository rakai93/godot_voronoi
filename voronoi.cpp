#include <utility>

#define JC_VORONOI_IMPLEMENTATION
#include "voronoi.h"

#define NEW_REF(CLASS, ...) Variant(Ref<CLASS>(new CLASS(__VA_ARGS__)).get_ref_ptr())

template<typename T, typename... As>
Variant new_ref_variant(As&&... args) {
	return Variant(Ref<T>(new T(std::forward<As>(args)...)).get_ref_ptr());
}

Variant VoronoiEdge::sites() const {
	Vector<Variant> result;
	result.push_back(new_ref_variant<VoronoiSite>(_edge->sites[0]));
	result.push_back(new_ref_variant<VoronoiSite>(_edge->sites[1]));
	return result;
}

Vector2 VoronoiEdge::start() const {
	return Vector2(_edge->pos[0].x, _edge->pos[0].y);
}

Vector2 VoronoiEdge::end() const {
	return Vector2(_edge->pos[1].x, _edge->pos[1].y);
}

void VoronoiEdge::_bind_methods() {
	ObjectTypeDB::bind_method(_MD("sites"), &VoronoiEdge::sites);
	ObjectTypeDB::bind_method(_MD("start"), &VoronoiEdge::start);
	ObjectTypeDB::bind_method(_MD("end"), &VoronoiEdge::end);
}

int VoronoiSite::index() const {
	return _site->index;
}

Vector2 VoronoiSite::center() const {
	return Vector2(_site->p.x, _site->p.y);
}

Variant VoronoiSite::edges() const {
	Vector<Variant> result;
	const jcv_graphedge* graphedge = _site->edges;
	while (graphedge) {
		result.push_back(new_ref_variant<VoronoiEdge>(graphedge->edge));
		graphedge = graphedge->next;
	}
	return result;
}

void VoronoiSite::_bind_methods() {
	ObjectTypeDB::bind_method(_MD("index"), &VoronoiSite::index);
	ObjectTypeDB::bind_method(_MD("center"), &VoronoiSite::center);
	ObjectTypeDB::bind_method(_MD("edges"), &VoronoiSite::edges);
}

VoronoiDiagram::VoronoiDiagram()
	: _diagram(new jcv_diagram) {
	memset(_diagram, 0, sizeof(jcv_diagram));
}

VoronoiDiagram::~VoronoiDiagram() {
	jcv_diagram_free(_diagram);
}

Variant VoronoiDiagram::edges() const {
	Vector<Variant> result;
	const jcv_edge* edge = jcv_diagram_get_edges(_diagram);
	while (edge) {
		result.push_back(new_ref_variant<VoronoiEdge>(edge));
		edge = edge->next;
	}
	return result;
}

Variant VoronoiDiagram::sites() const {
	Vector<Variant> result;
	const jcv_site* sites = jcv_diagram_get_sites(_diagram);
	for (int i = 0; i < _diagram->numsites; i++) {
		result.push_back(new_ref_variant<VoronoiSite>(&sites[i]));
	}
	return result;
}

void VoronoiDiagram::_bind_methods() {
	ObjectTypeDB::bind_method(_MD("edges"), &VoronoiDiagram::edges);
	ObjectTypeDB::bind_method(_MD("sites"), &VoronoiDiagram::sites);
}

void Voronoi::set_points(Vector<Vector2> points) {
	assert(points.size());

	std::vector<jcv_point> new_points;
	new_points.reserve(points.size());

	// translate Godot Vector2 points into jcv_points
	for (int i = 0; i < points.size(); i++)
		new_points.push_back({ points[i].x, points[i].y });

	_points.swap(new_points);
}

void Voronoi::set_boundaries(Rect2 boundaries) {
	_boundaries.reset(new jcv_rect {
		jcv_point { boundaries.pos.x, boundaries.pos.y },
		jcv_point { boundaries.pos.x + boundaries.size.x, boundaries.pos.y + boundaries.size.y }
	});
}

void* useralloc(void* ctx, size_t size) {
	return memalloc(size);
}

void userfree(void* ctx, void* ptr) {
	return memfree(ptr);
}

Ref<VoronoiDiagram> Voronoi::generate_diagram() const {
	Ref<VoronoiDiagram> result { new VoronoiDiagram() };
	jcv_diagram_generate_useralloc(_points.size(), _points.data(), _boundaries.get(), NULL, &useralloc, &userfree, result->_diagram);
	return result;
}

void Voronoi::_bind_methods() {
    ObjectTypeDB::bind_method(_MD("set_points", "points"), &Voronoi::set_points);
	ObjectTypeDB::bind_method(_MD("set_boundaries", "boundaries"), &Voronoi::set_boundaries);
	ObjectTypeDB::bind_method(_MD("generate_diagram"), &Voronoi::generate_diagram);
}
