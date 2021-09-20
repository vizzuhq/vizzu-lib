## Orientation, split & polar

Now that you are familiar with the basic logic and operators of Vizzu,
let's dive in with some more advanced features that you can use to create
animated data stories and show the data from different perspectives.

Switching orientation means that you put a measure from one axis to the other to 
see the data from a different perspective. This is once again a state you should 
only use temporarily.

```javascript { "title": "Switch the orienation = arrange by other axis" }
chart.animate({
	config: {
		channels: {
			y: { detach: ['Popularity'] },
			x: { attach: ['Popularity'] }
		}
	}
})
```

By turning the split parameter on, you can see stacked elements side-by-side, 
which enables the comparison of the components.

```javascript { "title": "Split stacked values = show side-by-side" }
chart.animate({
	config: {
		split: true
	}
})
```

Merging the components by turning the split parameter off

```javascript { "title": "Merge" }
chart.animate({
	config: {
		split: false
	}
})
```

We aggregate the data by removing the Genres dimension from the x-axis

```javascript { "title": "Aggregate" }
chart.animate({
	config: {
		channels: {
			x: { detach: ['Genres'] }
		}
	}
})
```

Switching from cartesian coordinates to polar. When doing so, it is worth 
setting the axes’ ranges so that the viewers can easily compare the values being 
shown. If you want to get back to the default, cartesian coordinates, just set 
the coordSystem parameter to ‘cartesian’.

```javascript { "title": "Polar coordinates" }
chart.animate({
	config: {
		channels: {
			x: { range: { max: '133%' } },
			y: { range: { min: '-100%' } }
		},
		coordSystem: 'polar'
	}
})
```
