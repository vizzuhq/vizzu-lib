## Geometry

In Vizzu you can set the geometric elements used to represent your data by the 
geometry property within the config object. 

Switching the geometry to area.

```javascript { "title": "Geometry: area" }
chart.animate({
	config: {
		geometry: 'area'
	}
})
```

Drawing a line chart.

```javascript { "title": "Geometry: line" }
chart.animate({
	config: {
		geometry: 'line'
	}
})
```

Switching the geometry to circle. This setting is the most useful when used 
together with the size channel, as shown in the next chapter of the tutorial.

```javascript { "title": "Geometry: circle" }
chart.animate({
	config: {
		geometry: 'circle'
	}
})
```

Rectangle geometry is the default setting in Vizzu, used for most common charts 
like bar and column charts.

```javascript { "title": "Geometry: rectangle - default" }
chart.animate({
	config: {
		geometry: 'rectangle'
	}
})
```
