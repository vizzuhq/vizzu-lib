---
data_url: ../../assets/data/music_data.js
---

# Events

You can register handlers for various events. There are pointer events (`click`,
`pointermove`), animation events (`begin`, `update`, `complete`), and rendering
events that are called before rendering the chart elements. Handlers can be
registered/unregistered with the `on`, `off` method pair.

We are registering a handler for the `click` event which will show an alert
block with information about the clicked chart element.

<div id="tutorial_01"></div>

// {% include-markdown "tutorial/assets/setup/setup_c.md" %}

```javascript
function clickHandler(event) {
    alert(JSON.stringify(event.target));
}

chart.on('click', clickHandler);
```

Unregistering the previously registered handler.

```javascript
chart.off('click', clickHandler);
```

Here we override the axis label color for `Jazz` to red and all others to gray.

<div id="tutorial_02"></div>

```javascript
function labelDrawHandler(event) {
    event.renderingContext.fillStyle =
        (event.target.value === 'Jazz') ? 'red' : 'gray';
}

chart.on('plot-axis-label-draw', labelDrawHandler);
```

Unregistering the previously registered handler.

```javascript
chart.off('plot-axis-label-draw', labelDrawHandler);
```

The default behaviour of all events can be blocked by calling the event's
`preventDefault` method. Here we block the drawing of the `Vizzu` logo in the
bottom right corner of the chart.

<div id="tutorial_03"></div>

```javascript
function logoDrawHandler(event) {
    event.preventDefault();
}

chart.on('logo-draw', logoDrawHandler);
```

Unregistering the previously registered handler.

```javascript
chart.off('logo-draw', logoDrawHandler);
```

You can also add a background image to the chart using the `preventDefault`
method.

<div id="tutorial_04"></div>

```javascript
const image = new Image();

function backgroundImageHandler(event) {
    event.renderingContext.drawImage(image, 0, 0,
        event.detail.rect.size.x, event.detail.rect.size.y);
    event.preventDefault();
}

image.src = 'data:image/gif;base64,R0lGODlhAwACAPIAAJLf6q/i7M/r8un0+PT6+/n8/QAAAAAAACH5BAQAAAAALAAAAAADAAIAAAMEWBMkkAA7';
image.onload = () => {
    chart.on('background-draw', backgroundImageHandler);
};
```

Unregistering the previously registered handler.

```javascript
chart.off('background-draw', backgroundImageHandler);
```

<script src="../events.js"></script>
