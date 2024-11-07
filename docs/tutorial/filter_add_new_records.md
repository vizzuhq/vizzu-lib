---
data_url: ../../assets/data/music_data.js
---

# Filtering & adding new records

Filtering enables you to zoom in or out within a chart, allowing the viewer to
focus on certain selected elements, or get more context. You can also add new
records to the data on the chart which makes it easy to work with real-time
sources.

We add two items from the `Genres` dimension - using the `||` operator - to the
filter, so the chart elements that belong to the other two items will vanish
from the chart.

<div id="tutorial_01"></div>

// {% include-markdown "tutorial/assets/setup/setup_c.md" %}

```javascript
// {% include "tutorial/filter_add_new_records/01.js" %}
```

Now we add a cross-filter that includes items from both the `Genres` and the
`Kinds` dimensions. This way we override the filter from the previous state. If
we weren't update the filter, `Vizzu` would use it in subsequent states.

<div id="tutorial_02"></div>

```javascript
// {% include "tutorial/filter_add_new_records/02_b.js" %}
```

Switching the filter off to get back to the original view.

<div id="tutorial_03"></div>

```javascript
// {% include "tutorial/filter_add_new_records/03_b.js" %}
```

Here we add another record to the data set and update the chart accordingly.

<div id="tutorial_04"></div>

```javascript
// {% include "tutorial/filter_add_new_records/04_b.js" %}
```

!!! info
    Combining this option with the [store](./shorthands_store.md) function makes
    it easy to update previously configured states with fresh data since this
    function saves the config and style parameters of the chart into a variable
    but not the data.

<script src="../assets/snippet.js" config="../filter_add_new_records/config.js"></script>
