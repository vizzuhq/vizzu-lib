## Events

You can register handlers for various events.
There are mouse events (click, mouseon), animation events (begin, update, complete),
and rendering events that are called before rendering the chart elements.
Handlers can be registered/unregistered with the on(), off() method pair.

We are registering a handler for the click event which will 
show an alert block with information about the clicked marker.

```javascript { "pure": true, "run": false }
function clickHandler(event) 
{
	alert(JSON.stringify(event.data)); 
}
```

```javascript { "title": "Click event added to markers" }
chart.on('click', clickHandler);
```

Unregistering the previously registered handler.

```javascript { "title": "Click event handler removed" }
chart.off('click', clickHandler);
```

Here we override the axis label color for 'Jazz' to red and all others to gray.

```javascript { "pure": true, "run": false }
function labelDrawHandler(event) 
{
	event.renderingContext.fillStyle = 
		(event.data.text === 'Jazz') ? 'red' : 'gray';
}
```

```javascript { "title": "Changing the canvas context before label draw" }
chart.on('plot-axis-label-draw', labelDrawHandler);
```

Unregistering the previously registered handler.

```javascript { "title": "Restore default behavior" }
chart.off('plot-axis-label-draw', labelDrawHandler);
```

The default behaviour of all events can be blocked by calling the event's
preventDefault() method. Here we block the drawing of the Vizzu Logo in the bottom right corner of the chart.

```javascript { "pure": true, "run": false }
function logoDrawHandler(event) 
{
	event.preventDefault();
}
```

```javascript { "title": "Prevent default behavior" }
chart.on('logo-draw', logoDrawHandler);
```

Unregistering the previously registered handler.

```javascript { "title": "Restore default behavior" }
chart.off('logo-draw', logoDrawHandler);
```
