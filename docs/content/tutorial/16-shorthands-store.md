## Shorthands & Store

> To assist you with the development we added various shorthands that will make 
> your code more compact. And we also added the store function, which enables you
> to save a chart state into a variable that you can later call without setting up 
> that state once again.

### When only the config property is used

> If you don't use the data and style properties in the first parameter of the animate method, you can use the function without expressing the config property.
>
> ```javascript
> chart.animate({
> 	align:'stretch'
> })
> ```

### Store function

> Let's save this state by calling the store function. 
> 
> ```javascript
> let snapshot = chart.store();
> ```

### When just one series is used

> If you set/attach/detach just one series on a channel, you don't have to put that into an array.
> 
> ```javascript
> chart.animate({
> 	channels: {
> 		x: { attach: 'Types' },
> 		y: { detach: 'Types' }
> 	},
> 	align: 'none'
> })
> ```

### When you use set and no other channel options

> If you use set on a channel and no other options like range, you don't have to use the channel object. If you only set one series on the channel you don't even have to put that into an object.
> 
> ```javascript
> chart.animate({
> 	channels: {
> 		y: [ 'Types', 'Popularity' ],
> 		x: 'Genres'
> 	}
> })
> ```

### Shorthand for styles

> Instead of creating nested objects, you can use the styles like this.
> 
> ```javascript
> chart.animate({
> 	style: { 
> 		'plot.axis.marker.label.fontSize': '150%',
> 		'title.backgroundColor': '#A0A0A0'  
> 	}
> })
> ```

### Restoring a previously stored state

> This is how you can get back to a state that you previously stored.
> 
> ```javascript
> chart.animate(snapshot)
> ```
