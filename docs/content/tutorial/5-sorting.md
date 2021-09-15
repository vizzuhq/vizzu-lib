## Sorting

> Vizzu provides multiple options to sort data. By default, the data is sorted by 
> the order it is added to Vizzu. This is why we suggest to add temporal data such 
> as dates in chronological order - from oldest to newest.

### Switch to ascending order...

> You can also sort the elements by value, which will provide you with an 
> ascending order.
> 
> ```javascript
> chart.animate({
> 	config: {
> 		sort: 'byValue'
> 	}
> })
> ```

### ...or descending order.

> If you want descending order instead, you have to set the reverse parameter to 
> true. When used without setting the sorting to byValue, the elements will be in 
> the opposite order than they are in the data set added to Vizzu.
> 
> ```javascript
> chart.animate({
> 	config: {
> 		reverse: true
> 	}
> })
> ```

### Let's get back to where we were

> This is how to switch back to the default sorting
> 
> ```javascript
> chart.animate({
> 	config: {
> 		sort: 'none',
> 		reverse: false
> 	}
> })
> ```

### With two discretes on one axis... 

> When you have more than one dimension on a channel, their order determines how 
> the elements are grouped. 
> 
> ```javascript
> chart.animate({
> 	config: {
> 		channels: {
> 			y: { detach: ['Types'] },
> 			x: { set: ['Genres', 'Types'] }
> 		}
> 	}
> })
> ```

### ...grouping is determined by their order. 

> When switching the order of dimensions on the x-axis Vizzu will rearrange the 
> elements according to this new logic.
> 
> ```javascript
> chart.animate({
> 	config: {
> 		channels: {
> 			x: { set: ['Types', 'Genres'] },
> 		},
> 		legend: 'color'
> 	}
> })
> ```