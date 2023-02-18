## Group/stack

The following example shows how to group, regroup and stack elements of a bar chart.

To get a stacked chart, you need to add a new dimension to the same channel 
where the measure is: the y-axis. However, since doing only this 
would result in multiple column chart elements with the same color stacked on
top of each other, we also add the same dimension to the color channel.

```javascript { "title": "Creating a stacked chart" }
chart.animate({
	config: {
		channels: {
			y: { attach: ['Kinds'] },
			color: { attach: ['Kinds'] }
		}
	}
})
```

By detaching this newly added dimension from the y-axis and attaching it to the 
x-axis, you get a grouped bar chart in a way that is easy to follow for the viewer.

```javascript { "title": "...then you can add it to another channel = group elements..." }
chart.animate({
	config: {
		channels: {
			y: { detach: ['Kinds'] },
			x: { attach: ['Kinds'] }
		}
	}
})
```

In order to change the category via which the elements are grouped, just
change the order of the dimension with another one on the same axis.

```javascript { "title": "Regrouping the chart" }
chart.animate({
	config: {
		channels: {
			x: { set: ['Kinds','Genres'] }
		}
	}
})
```

To stack a grouped chart, you just have to do the same thing the other way 
around: detach the dimension from the x-axis and attach it to the y-axis.

```javascript { "title": "...doing it the other way is how you stack your chart" }
chart.animate({
	config: {
		channels: {
			x: { detach: ['Kinds'] },
			y: { attach: ['Kinds'] }
		}
	}
})
```
