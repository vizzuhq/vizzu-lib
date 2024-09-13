---
data_url: ../../assets/data/music_data.js
---

# Shorthands & Store

To assist you with the development we added various shorthands that will make
your code more compact.

We also added store functions, which enable you to save either a chart state or
a whole animation into a variable that you can reuse later instead of setting up
that state or animation once again.

If you don't use the `data` and `style` properties in the first parameter of the
animate method, you can simplify your code by using only the object of the
`config` property.

<div id="tutorial_01"></div>

{% include-markdown "tutorial/assets/setup/setup_c.md" %}

```javascript
chart.animate({
    // config: {
    align: 'stretch'
    // }
})
```

Let's save this state by calling the `store` function.

<div id="tutorial_02"></div>

```javascript
var snapshot;

snapshot = chart.store();
```

If you set/attach/detach just one series on a channel, you don't have to put
that series into an array. Also, let's save this animation by calling the
`store` method of the animation `control` object.

<div id="tutorial_03"></div>

```javascript
var animation;

chart.animate({
    channels: {
        // x: { attach: [ 'Kinds' ] },
        x: {
            attach: 'Kinds'
        },
        // y: { detach: [ 'Kinds' ] },
        y: {
            detach: 'Kinds'
        }
    },
    align: 'none'
}).activated.then(control => {
    animation = control.store();
});
```

If you use set on a channel and no other options like range, you don't have to
express that channel as an object. If you only set one series on a channel you
can simply write the series' name after the channel name.

<div id="tutorial_04"></div>

```javascript
chart.animate({
    channels: {
        // y: { set: [ 'Kinds', 'Popularity' ] },
        y: ['Kinds', 'Popularity'],
        // x: { set: [ 'Genres' ] },
        x: 'Genres'
    }
})
```

In any case, you can simply omit the `channel` object, `Vizzu` will
automatically recognize the channels by their names.

<div id="tutorial_05"></div>

```javascript
chart.animate({
    // channels: {
    y: 'Kinds',
    x: ['Genres', 'Popularity']
    // }
})
```

If you have multiple keyframes, but with no animation options, you can omit the
`keyframe` object.

<div id="tutorial_06"></div>

```javascript
chart.animate([{
    // target: {
    y: ['Genres', 'Popularity']
    x: 'Kinds',
    // }
}, {
    // target: {
    y: 'Kinds',
    x: ['Genres', 'Popularity']
    // }
}])
```

Instead of creating nested objects, you can set the styles like this.

<div id="tutorial_07"></div>

```javascript
chart.animate({
    style: {
        // plot: { xAxis: { label: { fontSize: '150%' } } }
        'plot.xAxis.label.fontSize': '150%',
        'plot.backgroundColor': '#A0A0A0'
    }
})
```

Shorthands feature can be switched off if not needed:

```javascript
chart.feature('shorthands', false)
```

This is how you can reuse a previously stored animation.

<div id="tutorial_08"></div>

```javascript
chart.animate(animation)
```

You can also get back to a state that you previously stored.

<div id="tutorial_09"></div>

```javascript
chart.animate(snapshot)
```

<script src="../shorthands_store.js"></script>
