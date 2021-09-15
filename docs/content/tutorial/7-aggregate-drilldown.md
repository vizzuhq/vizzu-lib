## Aggregate/drill-down

> These features basically mean that you add or remove an additional dimension 
> to/from an axis or another channel. As you can see below, there are some 
> important things to keep in mind when you use them.

### Stack

> Let’s stack together the elements by putting the Genres dimension from the 
> x-axis to the y-axis. At the end of this phase, there are chart elements with 
> the same color stacked on top of each other, which is something you would want 
> to avoid. 
> 
> ```javascript
> chart.animate({
> 	config: {
> 		channels: {
> 			y: { attach: ['Genres'] },
> 			x: { set: null }
> 		}
> 	}
> })
> ```

### Aggregate elements

> By taking the Genres off of the y-axis, only one chart element remains for every 
> color, and Vizzu automatically calculates and shows the aggregate value of the 
> elements. 
> 
> Note: Instead of taking the unwanted dimension down from the chart, Genres could 
> have been added to the lightness channel to differentiate the chart elements.
> 
> ```javascript
> chart.animate({
> 	config: {
> 		channels: {
> 			y: { detach: ['Genres'] }
> 		}
> 	}
> })
> ```

### Drill-down

> To drill-down, the same category is put back on the y-axis, which results in a 
> state that we suggest you to only use temporarily when in transition.
> 
> ```javascript
> chart.animate({
> 	config: {
> 		channels: {
> 			y: { attach: ['Genres'] }
> 		}
> 	}
> })
> ```

### Group

> We group the elements by putting once again the Genres dimension on the x-axis.
> 
> ```javascript
> chart.animate({
> 	config: {
> 		channels: {
> 			y: { detach: ['Genres'] },
> 			x: { set: ['Genres'] }
> 		}
> 	}
> })
> ```