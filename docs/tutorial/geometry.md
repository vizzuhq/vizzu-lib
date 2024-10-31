---
data_url: ../../assets/data/music_data.js
---

# Geometry

In `Vizzu` you can set the geometric elements used to represent your data by the
geometry property within the config object. This is where the library shines -
beautifully animating between the geometries!

Switching the geometry to area.

<div id="tutorial_01"></div>

// {% include-markdown "tutorial/assets/setup/setup_b.md" %}

```javascript
// {% include "tutorial/geometry/01.js" %}
```

Drawing a line chart.

<div id="tutorial_02"></div>

```javascript
// {% include "tutorial/geometry/02_b.js" %}
```

Switching the geometry to circle. This setting is the most useful when used
together with the size channel, as shown in the next chapter of the tutorial.

<div id="tutorial_03"></div>

```javascript
// {% include "tutorial/geometry/03_b.js" %}
```

Rectangle geometry is the default setting in `Vizzu`, used for most common
charts like bar and column charts.

<div id="tutorial_04"></div>

```javascript
// {% include "tutorial/geometry/04_b.js" %}
```

<script src="../assets/snippet.js"></script>
