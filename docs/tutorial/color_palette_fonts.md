---
data_url: ../../assets/data/music_data.js
---

# Color palette & fonts

This and the next chapter provide a quick intro to the styling of the charts.
You can either use the `style` property like in the following examples or use
`CSS`. By using `CSS`, it's easier to set the same style for multiple charts on
one page or re-use style settings. `CSS` parameter usage is disabled by default
and can be enabled by `chart.feature('cssProperties', true)`.

The font sizes automatically adjust to the chart size to help readability, and
can also be set separately or for specific groups.

The color palette is changed to the colors we add here. The order of the
dimension’s items in the data set determine which color belongs to which item as
the colors are added one-by-one. If you want to use the same setting via `CSS`,
you should add
`--vizzu-plot-marker-colorPalette: #9355e8FF #123456FF #BDAF10FF;`.

<div id="tutorial_01"></div>

// {% include-markdown "tutorial/assets/setup/setup_c.md" %}

```javascript
// {% include "tutorial/color_palette_fonts/01_b.js" %}
```

The actual style settings of the chart can be accessed via the `style` property.

```javascript
// {% include "tutorial/color_palette_fonts/01_c.js" %}
```

Changing the title font size will only affect the title; all other font sizes
remain the same. `CSS` version: `--vizzu-title-fontSize: 50;`.

<div id="tutorial_02"></div>

```javascript
// {% include "tutorial/color_palette_fonts/02_b.js" %}
```

This is how to set the font size back to its default value.

<div id="tutorial_03"></div>

```javascript
// {% include "tutorial/color_palette_fonts/03_b.js" %}
```

In case you change the font size of the whole chart with the top-level
`fontSize` parameter then every font on the chart will grow/shrink
proportionally. The size refers to the font size of the axis labels by default.

<div id="tutorial_04"></div>

```javascript
// {% include "tutorial/color_palette_fonts/04_b.js" %}
```

You can reset styles to default on any levels by setting them to `null`.

<div id="tutorial_05"></div>

```javascript
// {% include "tutorial/color_palette_fonts/05_b.js" %}
```

For information on all available style parameters see the [Style](./style.md)
chapter.

<script src="../tutorial.js"></script>
