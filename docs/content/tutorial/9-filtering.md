## Filtering

Filtering enables you to zoom in or out within a chart, allowing the viewer to 
focus on certain selected elements, or add new ones to provide more context. 

We add two items from the Genres dimension - separated by the || operator - to 
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

Now we add a cross-filter that includes items from both the Genres and the Types 
dimensions. Check out the operators in the filter we used to achieve this 
result. Once again, we had to add the items to the filter that we added in the 
previous step since Vizzu will only include elements in a filter that are 
explicitly described in that step.

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
