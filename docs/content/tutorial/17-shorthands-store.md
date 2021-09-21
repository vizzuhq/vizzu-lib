## Shorthands & Store

To assist you with the development we added various shorthands that will make 
your code more compact. And we also added the store function, which enables you
to save a chart state into a variable that you can reuse later instead of setting up 
that state once again.

If you don't use the data and style properties in the first parameter of the animate method, you can simplify your code by using only the object of the config property.

```javascript { "title": "When only the config property is used" }
chart.animate({
// config: {
	align:'stretch'
// }
})
```

Let's save this state by calling the store function. 

```javascript { "title": "Store function" }
snapshot = chart.store();
```

If you set/attach/detach just one series on a channel, you don't have to put that series into an array.

```javascript { "title": "When just one series is used" }
chart.animate({
	channels: {
		// x: { attach: [ 'Types' ] },
		x: { attach: 'Types' },
		y: { detach: 'Types' }
	},
	align: 'none'
})
```

If you use set on a channel and no other options like range, you don't have to express that channel as an object. If you only set one series on a channel you can simply write the series' name after the channel name.

```javascript { "title": "When you use set and no other channel options" }
chart.animate({
	channels: {
		// y: { set: [ 'Types', 'Popularity' ] },
		y: [ 'Types', 'Popularity' ],
		x: 'Genres'
	}
})
```

In any case, you can simply omit the the channel object, Vizzu will automatically recognize the channels by their names.

```javascript { "title": "You don't have to use the channel object" }
chart.animate({
// channels: {
	y: 'Types',
	x: ['Genres','Popularity']
// }
})
```

Instead of creating nested objects, you can set the styles like this.

```javascript { "title": "Shorthand for styles" }
chart.animate({
	style: { 
		// plot: { xAxis: { label: { fontSize: '150%' } } }
		'plot.xAxis.label.fontSize': '150%',
		'title.backgroundColor': '#A0A0A0'  
	}
})
```

This is how you can get back to a state that you previously stored.

```javascript { "title": "Restoring a previously stored state" }
chart.animate(snapshot)
```
