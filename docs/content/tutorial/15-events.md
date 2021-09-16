## Events

You can register handlers for various events.
There are mouse events (click, mouseon), animation events (begin, update, complete),
and rendering events that are called before rendering the chart elements.
Handlers can be registered/unregistered with the on(), off() method pair.

We are registering a handler for the click event which will 
show an alert block with information about the clicked marker.

```javascript { "title": "Click event added to markers" }
clickHandler = event => { alert(JSON.stringify(event.data)); };

chart.on('click', clickHandler);
```

Unregistering the previously registered handler.

```javascript { "title": "Click event handler removed" }
chart.off('click', clickHandler);
```

Here we override the axis label color for 'Jazz' to red and all others to gray.

```javascript { "title": "Changing the canvas context before label draw" }
labelDrawHandler = event => {
	event.renderingContext.fillStyle = 
		(event.data.text === 'Jazz') ? 'red' : 'gray';
};

chart.on('plot-axis-label-draw', labelDrawHandler);
```

Unregistering the previously registered handler.

```javascript { "title": "Restore default behavior" }
chart.off('plot-axis-label-draw', labelDrawHandler);
```

The default behaviour of all events can be blocked by calling the event's
preventDefault() method. For example we block the drawing of the Vizzu Logo.

```javascript { "title": "Prevent default behavior" }
logoDrawHandler = event => { event.preventDefault() };

chart.on('logo-draw', logoDrawHandler);
```

Unregistering the previously registered handler.

```javascript { "title": "Restore default behavior" }
chart.off('logo-draw', logoDrawHandler);
```
