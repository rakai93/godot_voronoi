Voronoi Godot integration
=========================

Integration of [JCash's Voronoi implementation](https://github.com/JCash/voronoi) as a Godot V3.1.1 module.

Install
-------

Download the contents of this git repository into Godot's module folder and re-build Godot.
The module folder needs to be named `voronoi`, otherwise Godot won't compile properly.

`git clone https://github.com/rakai93/godot_voronoi.git voronoi`

Example usage
-----------------

```gdscript
# Create voronoi generator
var generator = Voronoi.new()
generator.set_points(list_of_vector2)
# optional: set boundaries for diagram, otherwise boundaries are computed based on points
generator.set_boundaries(rect2_bounds)
# optional: relax points N times, resulting in more equal sites
generator.relax_points(2)

# Generate diagram
var diagram = generator.generate_diagram()

# Iterate over sites
for site in diagram.sites():
	draw_circle(site.center())

# Iterate over edges
for edge in diagram.edges():
	draw_line(edge.start(), edge.end())
```
