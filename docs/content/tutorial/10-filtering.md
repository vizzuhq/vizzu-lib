## Filtering

Filtering enables you to zoom in or out within a chart, allowing the viewer to 
focus on certain selected elements, or add new ones to provide more context. 

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

```javascript { "title": "Filter off " }
chart.animate({
	data: {
		filter: null,
	}
})
```
