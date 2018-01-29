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

#include <math_2d.h>
#include <reference.h>
#include <variant.h>
#include <vector.h>

#include "lib/jc_voronoi.h"

class VoronoiEdge;
class VoronoiSite;

class VoronoiEdge : public Reference {
	OBJ_TYPE(VoronoiEdge, Reference)

public:
	const jcv_edge* _edge;

	VoronoiEdge() = default;
	inline VoronoiEdge(const jcv_edge* edge)
		: _edge(edge) {
	}

	~VoronoiEdge() = default;

	Variant sites() const;
	Vector2 start() const;
	Vector2 end() const;

protected:
	static void _bind_methods();
};

class VoronoiSite : public Reference {
	OBJ_TYPE(VoronoiSite, Reference)

public:
	const jcv_site* _site;

	VoronoiSite() = default;
	inline VoronoiSite(const jcv_site* site)
		: _site(site) {
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

	VoronoiDiagram();
	~VoronoiDiagram();

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
