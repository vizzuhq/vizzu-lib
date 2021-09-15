## Events

> You can register handlers for various events.
> There are mouse events (click, mouseon), animation events (begin, update, complete),
> and rendering events that are called before rendering the chart elements.
> Handlers can be registered/unregistered with the on(), off() method pair.

### Click event added to markers

We are registering a handler for the click event which will 
show an alert block with information about the clicked marker.

```javascript
clickHandler = event => { alert(JSON.stringify(event.data)); };

chart.on('click', clickHandler);
```

### Click event handler removed

Unregistering the previously registered handler.

```javascript
chart.off('click', clickHandler);
```

### Changing the canvas context before label draw

Here we override the axis label color for 'Jazz' to red and all others to gray.

```javascript
labelDrawHandler = event => {
	event.renderingContext.fillStyle = 
		(event.data.text === 'Jazz') ? 'red' : 'gray';
};

chart.on('plot-axis-label-draw', labelDrawHandler);
```

### Restore default behavior

> Unregistering the previously registered handler.
> 
> ```javascript
> chart.off('plot-axis-label-draw', labelDrawHandler);
> ```

### Prevent default behavior

> The default behaviour of all events can be blocked by calling the event's
> preventDefault() method. For example we block the drawing of the Vizzu Logo.
> 
> ```javascript
> logoDrawHandler = event => { event.preventDefault() };
> 
> chart.on('logo-draw', logoDrawHandler);
> ```

### Restore default behavior

> Unregistering the previously registered handler.
> 
> ```javascript
> chart.off('logo-draw', logoDrawHandler);
> ```
