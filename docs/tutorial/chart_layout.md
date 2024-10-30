---
data_url: ../../assets/data/music_data.js
---

# Chart layout

`Vizzu` has three separate parts of the chart layout: the plot area that
contains the chart, the title containing the chart title on the top, and the
legend on the left. `Vizzu` automatically hides the legend when it’s unnecessary
to show it. When the title is not in use, `Vizzu` hides that part automatically
as well. Each of these parts have their own paddings on all four sides that
adjust to the chart size by default, but can also be set with the appropriate
settings in the `style` object. All size parameters can be set in pixel,
percentage and em.

We add different background colors to the parts of the layout to show how they
are aligned.

<div id="tutorial_01"></div>

// {% include-markdown "tutorial/assets/setup/setup_c.md" %}

```javascript
chart.animate({
    style: {
        backgroundColor: '#A0A0A0',
        plot: {
            backgroundColor: '#D2D2D2'
        },
        legend: {
            backgroundColor: '#808080'
        },
    }
})
```

Setting the width of the legend.

<div id="tutorial_02"></div>

```javascript
chart.animate({
    style: {
        legend: {
            width: 50
        }
    }
})
```

Setting the legend width back to its default value.

<div id="tutorial_03"></div>

```javascript
chart.animate({
    style: {
        legend: {
            width: null
        }
    }
})
```

Changing the title paddings. By default, the title is horizontally centered
above the chart. In this example, we set the title’s left padding, resulting in
the text moving to the right.

<div id="tutorial_04"></div>

```javascript
chart.animate({
    style: {
        title: {
            paddingTop: 20,
            paddingBottom: 20,
            paddingLeft: 200
        }
    }
})
```

Setting the title paddings back to their default values.

<div id="tutorial_05"></div>

```javascript
chart.animate({
    style: {
        title: {
            paddingTop: null,
            paddingBottom: null,
            paddingLeft: null
        }
    }
})
```

Changing the paddings of the plot area to position the plot. The texts on the
axes are drawn on the padding of the plot and not the plot itself.

<div id="tutorial_06"></div>

```javascript
chart.animate({
    style: {
        plot: {
            paddingLeft: 100,
            paddingRight: 100
        }
    }
})
```

Setting the plot paddings back to their default values.

<div id="tutorial_07"></div>

```javascript
chart.animate({
    style: {
        plot: {
            paddingLeft: null,
            paddingRight: null
        }
    }
})
```

<script src="../chart_layout.js"></script>
