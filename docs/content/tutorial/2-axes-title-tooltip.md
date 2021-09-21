## Axes, title, tooltip

To build a chart with Vizzu, you have to add data series to the channels.

The first step is to create a simple column chart, adding one of the dimensions 
(Genre) to the x-axis and the measure (Popularity) to the y-axis using the set 
property.

```javascript
chart.animate({
	config: {
		channels: {
			y: { set: ['Popularity'] },
			x: { set: ['Genres'] }
		}
	}
})
```

In the next step, the chart is rearranged by putting both series on the y-axis 
using once again the set property, resulting in a single column chart. Vizzu 
automatically animates between the initial state and this one.

```javascript
chart.animate({
	config: {
		channels: {
				x: { set: null },
				y: { set: ['Genres','Popularity'] }
		}
	}
})
```

Instead of set, you can use attach and detach to add or remove series to/from the channels. 

```javascript
chart.animate({
	config: {
		channels: {
			y: { detach: ['Popularity'] },
			x: { attach: ['Popularity'] }
		}
	}
})
```
Using attach & detach makes it easier to build your animated charts step-by-step, however you either have to keep in mind what you had on which channel in 
the previous step or add the following code to access the actual configuration of the chart.

```javascript { "run": false }
console.log(chart.config); 
```


Setting the chart title with the title command.

```javascript
chart.animate({ title:'My first chart' })
```

Switching on the tooltips that appear on the chart elements when the user hovers 
over them with their mouse by adding the (tooltip, true) parameters to the 
chart.feature method. 

```javascript { "title" : "My first chart" }
chart.feature('tooltip',true)
```
