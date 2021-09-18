## Sorting

Vizzu provides multiple options to sort data. By default, the data is sorted by 
the order it is added to Vizzu. This is why we suggest to add temporal data such 
as dates in chronological order - from oldest to newest.

You can also sort the elements by value, which will provide you with an 
ascending order.

```javascript { "title": "Switch to ascending order..." }
chart.animate({
	config: {
		sort: 'byValue'
	}
})
```

If you want descending order instead, you have to set the reverse parameter to 
true. When used without setting the sorting to byValue, the elements will be in 
the opposite order than they are in the data set added to Vizzu.

```javascript { "title": "...or descending order." }
chart.animate({
	config: {
		reverse: true
	}
})
```

This is how to switch back to the default sorting

```javascript { "title": "Let's get back to where we were" }
chart.animate({
	config: {
		sort: 'none',
		reverse: false
	}
})
```

When you have more than one dimension on a channel, their order determines how 
the elements are grouped. 

```javascript { "title": "With two discretes on one axis... " }
chart.animate({
	config: {
		channels: {
			y: { detach: ['Types'] },
			x: { set: ['Genres', 'Types'] }
		}
	}
})
```

When switching the order of dimensions on the x-axis Vizzu will rearrange the 
elements according to this new logic.

```javascript { "title": "...grouping is determined by their order. " }
chart.animate({
	config: {
		channels: {
			x: { set: ['Types', 'Genres'] },
		}
	}
})
```
