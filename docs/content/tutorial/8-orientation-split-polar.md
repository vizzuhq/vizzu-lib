## Orientation, split & polar

> Now that you are familiar with the basic logic and operators of Vizzu,
> let's dive in with some more advanced features that you can use to create
> animated data stories and show the data from different perspectives.

### Switch the orienation = arrange by other axis

> Switching orientation means that you put a measure from one axis to the other to 
> see the data from a different perspective. This is once again a state you should 
> only use temporarily.
> 
> ```javascript
> chart.animate({
> 	config: {
> 		channels: {
> 			y: { detach: ['Popularity'] },
> 			x: { attach: ['Popularity'] }
> 		}
> 	}
> })
> ```

### Split stacked values = show side-by-side

> By turning the split parameter on, you can see stacked elements side-by-side, 
> which enables the comparison of the components.
> 
> ```javascript
> chart.animate({
> 	config: {
> 		split: true
> 	}
> })
> ```

### Merge

> Merging the components by turning the split parameter off
> 
> ```javascript
> chart.animate({
> 	config: {
> 		split: false
> 	}
> })
> ```

### Aggregate

> We aggregate the data by removing the Genres dimension from the x-axis
> 
> ```javascript
> chart.animate({
> 	config: {
> 		channels: {
> 			x: { detach: ['Genres'] }
> 		}
> 	}
> })
> ```

### Polar coordinates

> Switching from cartesian coordinates to polar. When doing so, it is worth 
> setting the axes’ ranges so that the viewers can easily compare the values being 
> shown. If you want to get back to the default, cartesian coordinates, just set 
> the coordSystem parameter to ‘cartesian’.
> 
> ```javascript
> chart.animate({
> 	config: {
> 		channels: {
> 			x: { range: { max: '133%' } },
> 			y: { range: { min: '-100%' } }
> 		},
> 		coordSystem: 'polar'
> 	}
> })
> ```
