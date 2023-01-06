## Group/stack

The following example shows how to group, regroup and stack elements of a bar chart.

To get a grouped chart, you need to add a new dimension to the channel where you
would like the data to be grouped: the x-axis. However, since doing only this 
would result in several columns with the same color next to each other, 
we also add the same dimension to the color channel.

```javascript { "title": "Creating a grouped chart" }
chart.animate({
	config: {
		channels: {
			x: { attach: ['Genres'] },
			color: { attach: ['Genres'] }
		}
	}
})
```

To change the grouping of the chart, you just need to put replace the recently
added dimension to the chart.

```javascript { "title": "Regrouping the chart" }
chart.animate({
	config: {
		channels: {
			x: { detach: ['Genres'], attach: ['Kinds'] },
			color: { detach: ['Genres'], attach: ['Kinds'] }
		}
	}
})
```

When the partitioning of the underlying data to markers on the chart is changing
due to setting different dimensions for the chart, there are multiple ways to 
transition between the starting and end state.

You can aggregate the markers to the common base of the two states, as shown 
above with the default settings, but you can also drill down:

```javascript { "title": "Regrouping the chart by drilling down" }
chart.animate({
	config: {
		channels: {
			x: { detach: ['Kinds'], attach: ['Genres'] },
			color: { detach: ['Kinds'], attach: ['Genres'] }
		}
	}
}, { regroupStrategy: 'drilldown' })
```

or simply fade between them:

```javascript { "title": "Regrouping the chart with fading" }
chart.animate({
	config: {
		channels: {
			x: { detach: ['Genres'], attach: ['Kinds'] },
			color: { detach: ['Genres'], attach: ['Kinds'] }
		}
	}
}, { regroupStrategy: 'fade' })
```

To get a stacked chart, you just have to move the added dimension to the same channel 
where the measure is: the y-axis.

```javascript { "title": "...stacking your chart" }
chart.animate({
	config: {
		channels: {
			x: { detach: ['Kinds'] },
			y: { attach: ['Kinds'] }
		}
	}
})
```
