---
data_url: ../../assets/data/music_data.js
---

# Align & range

`Vizzu` offers different options to align your chart elements and to set the
range of values shown on the axes. Alignment can be used to create charts like a
stream chart where the elements are vertically centered. A good example for
using range is when you fix the y-axis so that it would not adapt to the data
being shown.

Centered alignment. The effect of this parameter depends on the orientation of
the chart. For example, on a column chart, elements will be vertically centered,
whereas on a bar chart, horizontally.

!!! info
    In the first example, the y-axis labels are hidden because they don't
    properly represent the values shown on the column chart anymore, as the
    chart elements float off the x-axis.

<div id="tutorial_01"></div>

// {% include-markdown "tutorial/assets/setup/setup_c.md" %}

Change align and configures the y axis labels to disappear during the animation.

```javascript
// {% include "tutorial/align_range/01.js" %}
```

Stretched alignment. This way the elements will proportionally fill the entire
plot area, effectively showing proportions in stacked charts. This is why the
scale will also switch from values to percentages when used.

<div id="tutorial_02"></div>

```javascript
// {% include "tutorial/align_range/02_b.js" %}
```

Getting back to the default alignment.

<div id="tutorial_03"></div>

```javascript
// {% include "tutorial/align_range/03_b.js" %}
```

You can set the range of an axis by setting the minimum and maximum values of
it. Both parameters are optional so that you can set only one of those, and you
either set specific values or a relative value by adding the `%` sign. In this
example, we set the range of the y-axis in a way that the max value is `150%` of
the biggest element’s value.

<div id="tutorial_04"></div>

```javascript
// {% include "tutorial/align_range/04_b.js" %}
```

You can also set the range for an axis with a dimension on it. You can even use
this feature to filter certain elements, just like in the following example.

<div id="tutorial_05"></div>

```javascript
// {% include "tutorial/align_range/05_b.js" %}
```

Ranges have certain defaults depending on the chart's configuration, based on
common data viz guidelines because we wanted to make it easy for you to create
sleek charts. For example, in the cartesian coordinate system, the range will be
automatically set to the `max:110%` for an axis with a measure on it. Polar
coordinates work differently, as you can see for yourself in the
[Orientation, split & polar](./orientation_split_polar.md) chapter.

Whenever you want to set your ranges back to the default value, just set them to
`'auto'`.

<div id="tutorial_06"></div>

```javascript
// {% include "tutorial/align_range/06_b.js" %}
```

<script src="../assets/snippet.js"></script>
