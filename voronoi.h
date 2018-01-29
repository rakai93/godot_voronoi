#ifndef VORONOI_H
#define VORONOI_H

// see https://github.com/JCash/voronoi/blob/master/src/main.c

#if defined(_MSC_VER)
#include <malloc.h>
#define alloca _alloca
#else
#include <alloca.h>
#endif

#include <memory>
#include <vector>

#include <dictionary.h>
#include <math_2d.h>
#include <object.h>
#include <reference.h>
#include <variant.h>
#include <vector.h>

#include "lib/jc_voronoi.h"

class VoronoiEdge;
class VoronoiSite;
class VoronoiDiagram;

class VoronoiEdge : public Object {
	OBJ_TYPE(VoronoiEdge, Object)

public:
	const jcv_edge* _edge;
	const VoronoiDiagram* _diagram;

	VoronoiEdge() = default;
	inline VoronoiEdge(const jcv_edge* edge, const VoronoiDiagram* diagram)
		: _edge(edge), _diagram(diagram) {
	}

	~VoronoiEdge() = default;

	Variant sites() const;
	Vector2 start() const;
	Vector2 end() const;

protected:
	static void _bind_methods();
};

class VoronoiSite : public Object {
	OBJ_TYPE(VoronoiSite, Object)

public:
	const jcv_site* _site;
	const VoronoiDiagram* _diagram;

	VoronoiSite() = default;
	inline VoronoiSite(const jcv_site* site, const VoronoiDiagram* diagram)
		: _site(site), _diagram(diagram) {
	}

	~VoronoiSite() = default;

	int index() const;
	Vector2 center() const;
	Variant edges() const;
	Variant neighbors() const;

protected:
	static void _bind_methods();
};

class VoronoiDiagram : public Reference {
	OBJ_TYPE(VoronoiDiagram, Reference)

public:
	jcv_diagram* _diagram;

	Variant _edges;
	Variant _sites;

	Dictionary _edges_by_address;
	Dictionary _sites_by_index;

	VoronoiDiagram();
	~VoronoiDiagram();

	void build_objects();

	Variant edges() const;
	Variant sites() const;

protected:
	static void _bind_methods();
};

class Voronoi : public Reference {
	OBJ_TYPE(Voronoi, Reference)

	std::unique_ptr<jcv_rect> _boundaries;
	std::vector<jcv_point> _points;

public:
	Voronoi() = default;
    ~Voronoi() = default;

	void set_points(Vector<Vector2> points);
	void set_boundaries(Rect2 boundaries);
	Ref<VoronoiDiagram> generate_diagram() const;

protected:
    static void _bind_methods();

};

#endif // VORONOI_H
