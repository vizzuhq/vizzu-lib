---
data_url: ../../assets/data/music_data.js
---

# Without coordinates & noop channel

Certain chart types have neither measures nor dimensions on the axes such as
treemaps and bubble charts. This is a case when the `noop` channel comes in
handy for grouping and stacking elements in these kinds of charts.

To get to a treemap, we have to detach all dimensions and the measure from the
axes and put two of them on the `size` channel, whereas the other dimension is
still on the `color` channel.

<div id="tutorial_01"></div>

// {% include-markdown "tutorial/assets/setup/setup_c.md" %}

```javascript
// {% include "tutorial/without_coordinates_noop_channel/01.js" %}
```

Getting from a treemap to a bubble chart is simply by changing the geometry to
circle. This bubble chart is stacked by the `Kinds` dimension that is on the
`size` channel - this is why the bubbles are in separate, small groups.

<div id="tutorial_02"></div>

```javascript
// {% include "tutorial/without_coordinates_noop_channel/02_b.js" %}
```

In order to show all bubbles as one group, we use the `noop` (no operations)
channel for the `Genres` dimension. The `noop` channel enables us to have a
dimension on the chart, that doesn’t affect any parameter of the elements, only
their count.

<div id="tutorial_03"></div>

```javascript
// {% include "tutorial/without_coordinates_noop_channel/03_b.js" %}
```

<script src="../assets/snippet.js" config="../without_coordinates_noop_channel/config.js"></script>
