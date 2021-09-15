## Animation settings

> In Vizzu you can set the timing and duration of the animation. You can do this 
> either for the whole animation, or for animation groups such as the elements 
> moving along the x-axis or the y-axis, appearing or disappearing or when the 
> coordinate system is changed.

### Default settings

> Let’s see first a simple example when a stacked column chart is grouped using 
> the default animation settings.
> 
> ```javascript
> chart.animate({
> 	config: {
> 		channels: {
> 			y: { detach: ['Types'] },
> 			x: { attach: ['Types'] }
> 		}
> 	}
> })
> ```

### Default settings

> We stack the columns, still with the default settings.
> 
> ```javascript
> chart.animate({
> 	config: {
> 		channels: {
> 			x: { detach: ['Types'] },
> 			y: { attach: ['Types'] }
> 		}
> 	}
> })
> ```

### Custom animation settings for specific groups

> Now we change the animation settings, for the elements moving along the y-axis 
> and also the change in styles, more specifically when the labels on the markers 
> move from the center of the chart elements to the top of them.
> 
> ```javascript
> chart.animate({
> 	config: {
> 		channels: {
> 			y: { detach: ['Types'] },
> 			x: { attach: ['Types'] }
> 		}
> 	}
> }, 
> {
> 	y: { 
> 		duration: 2, 
> 		delay: 2 
> 	},
> 	style: { 
> 		duration: 2, 
> 		delay: 4 
> 	}
> })
> ```

### Custom settings for the whole animation

> This is an example of changing the settings for the whole animation at once.
> 
> ```javascript
> chart.animate({
> 	config: {
> 		channels: {
> 			x: { detach: ['Types'] },
> 			y: { attach: ['Types'] }
> 		}
> 	}
> }, 
> { 
> 	duration: 1, 
> 	easing: 'linear'
> })
> ```

### Custom settings for both

> When the two settings are combined, Vizzu will use the general animation 
> settings and spread the unique settings for specific groups proportionally. 
> This is why you can see the same animation as two steps before but happening 
> much quicker since the duration of the whole animation is set to 1 second. 
> 
> ```javascript
> chart.animate(
> {
> 	config: {
> 		channels: {
> 			x: { attach: ['Types'] },
> 			y: { detach: ['Types'] }
> 		}
> 	}
> },
> {
> 	duration: 1, 
> 	easing: 'linear',
> 	y: { 
> 		duration: 2, 
> 		delay: 2 
> 	},
> 	style: { 
> 		duration: 2, 
> 		delay: 4 
> 	}
> })
> ```

### Shorthand for duration

> In case you only want to change the duration of the animation, you can use 
> the following, simplified format to do that. 
> 
> ```javascript
> chart.animate(
> {
> 	config: {
> 		channels: {
> 			x: { detach: ['Types'] },
> 			y: { attach: ['Types'] }
> 		}
> 	}
> }, '500ms')
> ```