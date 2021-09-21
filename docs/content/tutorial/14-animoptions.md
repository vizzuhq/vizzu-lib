## Animation options

In Vizzu you can set the timing and duration of the animation. You can do this 
either for the whole animation, or for animation groups such as the elements 
moving along the x-axis or the y-axis, appearing or disappearing or when the 
coordinate system is changed.

Let’s see first a simple example when a stacked column chart is grouped using 
the default animation options.

```javascript { "title": "Default options - step 1" }
chart.animate({
	config: {
		channels: {
			y: { detach: ['Types'] },
			x: { attach: ['Types'] }
		}
	}
})
```

We stack the columns, still with the default options.

```javascript { "title": "Default options - step 2" }
chart.animate({
	config: {
		channels: {
			x: { detach: ['Types'] },
			y: { attach: ['Types'] }
		}
	}
})
```

Now we change the animation settings for the elements moving along the y-axis 
and also the change in styles, more specifically when the labels on the markers 
move from the center of the chart elements to the top of them.

```javascript { "title": "Custom animation settings for specific groups" }
chart.animate({
	config: {
		channels: {
			y: { detach: ['Types'] },
			x: { attach: ['Types'] }
		}
	}
}, 
{
	y: { 
		duration: 2, 
		delay: 2 
	},
	style: { 
		duration: 2, 
		delay: 4 
	}
})
```

This is an example of changing the settings for the whole animation at once.

```javascript { "title": "Custom options for the whole animation" }
chart.animate({
	config: {
		channels: {
			x: { detach: ['Types'] },
			y: { attach: ['Types'] }
		}
	}
}, 
{ 
	duration: 1, 
	easing: 'linear'
})
```

When the two settings are combined, Vizzu will use the general animation 
options and spread the unique settings for specific groups proportionally. 
This is why you can see the same animation as two steps before but happening 
much quicker since the duration of the whole animation is set to 1 second. 

```javascript { "title": "Custom settings for both" }
chart.animate(
{
	config: {
		channels: {
			x: { attach: ['Types'] },
			y: { detach: ['Types'] }
		}
	}
},
{
	duration: 1, 
	easing: 'linear',
	y: { 
		duration: 2, 
		delay: 2 
	},
	style: { 
		duration: 2, 
		delay: 4 
	}
})
```

In case you only want to change the duration of the animation, you can use 
the following, simplified format to do that. 

```javascript { "title": "Shorthand for duration" }
chart.animate(
{
	config: {
		channels: {
			x: { detach: ['Types'] },
			y: { attach: ['Types'] }
		}
	}
}, '500ms')
```
