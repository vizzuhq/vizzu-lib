## Align & range

Vizzu offers different options to align your chart elements and to set the range 
of values shown on the axes. Alignment can be used to create charts like a 
stream chart where the elements are vertically centered . A good example for 
using range is when you fix the y-axis so that it would not adapt to the data 
being shown.

Centered alignment. The effect of this parameter depends on the orientation of 
the chart. For example, on a column chart, elements will be vertically centered, 
whereas on a bar chart, horizontally.

```javascript { "title": "Align: center" }
chart.animate({
	config: {
		align:'center'
	}
})
```

Stretched alignment. This way the elements will proportionally fill the entire 
plot area, effectively showing proportions in stacked charts. This is why the 
scale will also switch from values to percentages when used. 

```javascript { "title": "Align: stretch = % view" }
chart.animate({
	config: {
		align:'stretch'
	}
})
```

Getting back to the default alignment

```javascript { "title": "Align: none - default" }
chart.animate({
	config: {
		align:'none'
	}
})
```

You can set the range of an axis by setting the minimum and maximum values of 
it. Both parameters are optional so that you can set only one of those, and you 
either set specific values or a relative value by adding the % sign. In this 
example, we set the range of the y-axis in a way that the max value is 150% of 
the biggest element’s value.

```javascript { "title": "Axis range set proportionally to shown values" }
chart.animate({
	config: {
		channels: {
			y: { range: { max: '150%' } }
		}
	}
})
```

You can also set the range for an axis with a dimension on it. You can even use 
this feature to filter certain elements, just like in the following example.

```javascript { "title": "Axis range set explicitly on an axis with discrete series" }
chart.animate({
	config: {
		channels: {
			x: { 
				range: { 
					min: -2, 
					max: 3 
				} 
			}
		}
	}
})
```

Ranges have certain defaults depending on the chart's configuration, based on common data viz guidelines because we wanted to make it easy for you to create sleek charts. For example, in the cartesian coordinate system, the range will be automatically set to the max:110% for an axis with a measure on it. Polar coordinates work differently, as you can see for yourself in the [Orientation, split & polar](#chapter-0.9) chapter. 

Whenever you want to set your ranges back to the default value, just set them to ‘auto’.

```javascript { "title": "Axis range set explicitly on an axis with discrete series" }
chart.animate({
	config: {
		channels: {
			x: { 
				range: { 
					min: 'auto',
					max: 'auto' 
				}
			},
			y: { 
				range: { 
					max: 'auto' 
				} 
			}
		}
	}
})
```
