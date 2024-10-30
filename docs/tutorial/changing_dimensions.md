---
data_url: ../../assets/data/music_data.js
---

# Changing dimensions

The simplest of dimension-changing operations are aggregate and drill-down.
These features basically mean that you either add or remove an additional
dimension to/from an axis or another channel.

Let’s aggregate together the elements by getting the `Genres` dimension off the
x-axis. By taking it off the chart, only one chart element remains for every
color, and `Vizzu` automatically calculates and shows the aggregate value of the
elements.

<div id="tutorial_01"></div>

// {% include-markdown "tutorial/assets/setup/setup_c.md" %}

```javascript
chart.animate({
    config: {
        channels: {
            x: {
                set: null
            }
        }
    }
})
```

When you simultaneously add and remove dimensions, the partitioning of the
underlying data to markers on the chart changes. There are multiple ways to
transition through these kinds of operations using.

By default, the markers are aggregated to the common base of the two states,
then drilled down to the target state, as shown below.

<div id="tutorial_02"></div>

```javascript
chart.animate({
    config: {
        channels: {
            y: {
                detach: ['Kinds']
            },
            x: {
                set: ['Genres']
            },
            color: {
                set: null
            },
        }
    }
})
```

You can change this setting and drill down to the union of the two states
instead, and then aggregate to the target state:

<div id="tutorial_03"></div>

```javascript
chart.animate({
    config: {
        channels: {
            x: {
                detach: ['Genres'],
                attach: ['Kinds']
            }
        }
    }
}, {
    regroupStrategy: 'drilldown'
})
```

There is also the option to fade the chart between the states:

<div id="tutorial_04"></div>

```javascript
chart.animate({
    config: {
        channels: {
            x: {
                detach: ['Kinds'],
                attach: ['Genres']
            }
        }
    }
}, {
    regroupStrategy: 'fade'
})
```

To simply drill down, the same dimension is put back on the y-axis.

<div id="tutorial_05"></div>

```javascript
chart.animate({
    config: {
        channels: {
            y: {
                attach: ['Kinds']
            },
            color: {
                set: ['Kinds']
            }
        }
    }
})
```

<script src="../changing_dimensions.js"></script>
