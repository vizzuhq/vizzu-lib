---
data_url: ../../assets/data/music_data.js
---

# Chart presets

Throughout this tutorial, we have shown you how to create charts with `Vizzu`
using a chart type-agnostic, uniform way without being limited to a predefined
set of available charts. But sometimes, when you know the chart type you want to
use, it is easier to start with that and skip configuring it from scratch. For
this reason, `Vizzu` provides preset chart configurations for many known chart
types. See the [preset gallery](../examples/presets/index.md) for all available
presets.

Use the methods of the `presets` static property of the `Vizzu` class to build a
chart based on a preset. These methods return chart configuration objects that
can be passed to the `animate` method. For example, this is how to create a
stacked bubble chart using its preset.

<div id="tutorial_01"></div>

// {% include-markdown "tutorial/assets/setup/setup_a.md" %}

```javascript
// {% include "tutorial/chart_presets/01_b.js" %}
```

Presets will override all channels, removing all previously set series from the
chart. Using a preset will also explicitly set most chart configuration
parameters. Exceptions to this are the `legend`, `title`, `reverse`, and `sort`
properties that can be set while using a preset. Here's an example of a preset
where chart elements are sorted by value.

<div id="tutorial_02"></div>

```javascript
// {% include "tutorial/chart_presets/02_b.js" %}
```

As you will see, the preset doesn't override the previously configured sorting
and wouldn't affect the rest of the chart config parameters mentioned above
either.

Presets will affect chart configuration, but you might also want to set the
style or the underlying data. In this case, you can use the more verbose syntax
below, explicitly passing the preset to the 'config' property of the animate
method's parameter.

<div id="tutorial_03"></div>

```javascript
// {% include "tutorial/chart_presets/03_b.js" %}
```

!!! info
    You can also access presets via a `chart` promise, for example
    `Vizzu.presets.radialBar` is equivalent to
    `chart.constructor.presets.radialBar`.

<script src="../assets/snippet.js" config="../chart_presets/config.js"></script>
