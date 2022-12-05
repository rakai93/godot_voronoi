Voronoi Godot integration for Godot 4.x
=======================================

This fork upgrades the orginal [library](https://github.com/rakai93/godot_voronoi) to support Godot 4.x.
It compiles against the 3.5 and master branches of [godot](https://github.com/godotengine/godot). 

Integration of [JCash's Voronoi implementation](https://github.com/JCash/voronoi) as a Godot V3.1.1 module.

Install
-------

Download the contents of this git repository into Godot's module folder and re-build Godot.
The module folder needs to be named `voronoi`, otherwise Godot won't compile properly.

`git clone https://github.com/rakai93/godot_voronoi.git voronoi --recurse-submodules`

Example usage
-----------------

```gdscript
#VoronoiTest.gd
extends Node2D

var diagram

func _ready():
	# Create voronoi generator	
	var points = PackedVector2Array([Vector2(0,0)])
	for i in 100:
		randomize()
		points.append(Vector2(randi_range(0, 1000), randi_range(0, 1000)))
	
	var generator = Voronoi.new()
	generator.set_points(points)
	# optional: set boundaries for diagram, otherwise boundaries are computed based on points
	#generator.set_boundaries(rect2_bounds)
	# optional: relax points N times, resulting in more equal sites
	generator.relax_points(2)

	# Generate diagram
	diagram = generator.generate_diagram()

func _draw():
	# Iterate over sites
	for site in diagram.sites():
		randomize()
		var center = site.center()
		#print("site: ", site, "center: ", center)
		draw_circle(center, 5,  Color(randf(), randf(), randf()))

	# Iterate over edges
	for edge in diagram.edges():
		draw_line(edge.start(), edge.end(), Color.BLACK, 2.0)

```
