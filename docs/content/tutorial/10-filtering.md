## Filtering & adding new records

Filtering enables you to zoom in or out within a chart, allowing the viewer to 
focus on certain selected elements, or get more context.
You can also add new records to the data on the chart which makes it easy to work
with real-time sources.

We add two items from the Genres dimension - using the || operator - to 
the filter, so the chart elements that belong to the other two items will vanish 
from the chart.

```javascript { "title": "Filter by one dimension" }
chart.animate({
	data: {
		filter: record => 
			record["Genres"] == 'Pop' 
			|| record["Genres"] == 'Metal',
	}
})
```

Now we add a cross-filter that includes items from both the Genres and the Types dimensions. This way we override the filter from the previous state. If we weren't update the filter, Vizzu would use it in subsequent states.

```javascript { "title": "Filter by two dimensions" }
chart.animate({
	data: {
		filter: record => 
			(record["Genres"] == 'Pop' || record["Genres"] == 'Metal') 
			&& record["Types"] == 'Smooth'
	}
})
```

Switching the filter off to get back to the original view.

```javascript { "title": "Filter off" }
chart.animate({
	data: {
		filter: null,
	}
})
```

Here we add another record to the data set and update the chart accordingly.

```javascript { "title": "Adding new records" }
chart.animate({
	data: {
		records: [
		  ['Soul', 'Hard', 91],
		  ['Soul', 'Smooth', 57],
		  ['Soul', 'Experimental', 115],
	        ] 
	}
})
```

Note: combining this option with the [store function](#chapter-0.17) makes it easy to 
update previously configured states with fresh data since this function saves the config 
and style parameters of the chart into a variable but not the data.
