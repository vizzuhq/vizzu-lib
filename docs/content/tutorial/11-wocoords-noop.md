## Without coordinates & noop channel

Certain chart types have neither measures nor dimensions on the axes such as 
treemaps and bubble charts. This is a case when the noop channel comes in handy 
for grouping and stacking elements in these kinds of charts

To get to a treemap, we have to detach all dimensions and the measure from the 
axes and put two of them on the size channel, whereas the other dimension is 
still on the color channel. Since the same dimensions are used in both cases 
Vizzu will be able to animate between these states.

```javascript { "title": "Treemap" }
chart.animate({
	config: {
		channels: {
			y: { set: null },
			x: { set: null },
			size: { attach: ['Genres', 'Popularity'] }
		}
	}
})
```

Getting from a treemap to a bubble chart is simply by changing the geometry to 
circle. This bubble chart is stacked by the Type dimension that is on the size 
channel - this is why the bubbles are in separate, small groups.

```javascript { "title": "Bubble chart - stacked" }
chart.animate({
	config: {
		geometry: 'circle'
	}
})
```

In order to show all bubbles as one group, we use the noop (no operations) channel for the Genres dimension. The noop channel enables us to have a dimension on the chart, that doesn’t affect any parameter of the elements, only their count.

```javascript { "title": "Bubble chart - grouped - using the noop channel" }
chart.animate({
	config: {
		channels: {
			size: { detach: ['Genres'] },
			noop: { set: ['Genres'] }
		}
	}
})
```
