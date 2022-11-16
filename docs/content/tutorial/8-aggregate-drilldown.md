## Aggregate/drill-down

These features basically mean that you add or remove an additional dimension 
to/from an axis or another channel. 

Let’s aggregate together the elements by getting the Genres dimension off the 
x-axis. By taking it off of the chart, only one chart element remains for every 
color, and Vizzu automatically calculates and shows the aggregate value of the 
elements. 

```javascript { "title": "Stack" }
chart.animate({
	config: {
		channels: {
			x: { set: null }
		}
	}
})
```

To drill-down, the same dimension is put back on the x-axis.

```javascript { "title": "Drill-down" }
chart.animate({
	config: {
		channels: {
			x: { attach: ['Genres'] }
		}
	}
})
```
