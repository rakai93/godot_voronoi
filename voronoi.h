#ifndef VORONOI_H
#define VORONOI_H

#include <cstdint>
#include <map>
#include <type_traits>
#include <vector>

#include <core/math/vector2.h>
#include <core/math/rect2.h>
#include <core/object.h>
#include <core/reference.h>
#include <core/variant.h>
#include <core/vector.h>

#include "lib/src/jc_voronoi.h"

namespace voronoi_detail {

template<typename T>
struct GodotAllocator {
	using value_type = T;

	constexpr GodotAllocator() noexcept {}

    template<typename U>
	GodotAllocator(const GodotAllocator<U>&) noexcept {}

    template<typename U>
	bool operator==(const GodotAllocator<U>&) const noexcept {
        return true;
    }

	template<typename U>
	bool operator!=(const GodotAllocator<U>&) const noexcept {
        return false;
    }

	inline T* allocate(size_t n) const {
		return reinterpret_cast<T*>(memalloc(sizeof(T) * n));
	}

	inline void deallocate(T* ptr, size_t) const noexcept {
		memfree(ptr);
	}
};

template<typename K, typename V>
using map = std::map<K, V, std::less<K>, GodotAllocator<std::pair<const K, V>>>;

template<typename T>
using vector = std::vector<T, GodotAllocator<T>>;

}  // namespace voronoi_detail

class VoronoiEdge;
class VoronoiSite;
class VoronoiDiagram;

class VoronoiEdge : public Object {
	GDCLASS(VoronoiEdge, Object)

public:
	const jcv_edge* _edge;
	const VoronoiDiagram* _diagram;

	VoronoiEdge() = default;
	inline VoronoiEdge(const jcv_edge* edge, const VoronoiDiagram* diagram)
		: _edge(edge), _diagram(diagram) {
	}

	~VoronoiEdge() = default;

	Vector<Variant> sites() const;
	Vector2 start() const;
	Vector2 end() const;

protected:
	static void _bind_methods();
};

class VoronoiSite : public Object {
	GDCLASS(VoronoiSite, Object)

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
	Vector<Variant> edges() const;
	Vector<Variant> neighbors() const;

protected:
	static void _bind_methods();
};

class VoronoiDiagram : public Reference {
	GDCLASS(VoronoiDiagram, Reference)

public:
	jcv_diagram _diagram;

	voronoi_detail::vector<Variant> _edges;
	voronoi_detail::vector<Variant> _sites;

	voronoi_detail::map<std::uintptr_t, VoronoiEdge*> _edges_by_address;
	voronoi_detail::map<int, VoronoiSite*> _sites_by_index;

	VoronoiDiagram();
	~VoronoiDiagram();

	void build_objects();

	Vector<Variant> edges() const;
	Vector<Variant> sites() const;

protected:
	static void _bind_methods();
};

class Voronoi : public Reference {
	GDCLASS(Voronoi, Reference)

	jcv_rect _boundaries;
	bool _has_boundaries;
	voronoi_detail::vector<jcv_point> _points;

public:
	Voronoi() = default;
    ~Voronoi() = default;

	void set_points(Vector<Vector2> points);
	void set_boundaries(Rect2 boundaries);
	void relax_points(int iterations);
	Ref<VoronoiDiagram> generate_diagram() const;

protected:
    static void _bind_methods();

};

#endif // VORONOI_H
