---
data_url: ../../assets/data/music_data.js
---

# Group/stack

The following example shows how to group, regroup and stack elements of a bar
chart.

To get a stacked chart, you need to add a new dimension to the same channel
where the measure is: the y-axis. However, since doing only this would result in
multiple column chart elements with the same color stacked on top of each other,
we also add the same dimension to the color channel.

<div id="tutorial_01"></div>

// {% include-markdown "tutorial/assets/setup/setup_b.md" %}

```javascript
// {% include "tutorial/group_stack/01.js" %}
```

By detaching this newly added dimension from the y-axis and attaching it to the
x-axis, you get a grouped bar chart in a way that is easy to follow for the
viewer.

<div id="tutorial_02"></div>

```javascript
// {% include "tutorial/group_stack/02_b.js" %}
```

In order to change the category via which the elements are grouped, just change
the order of the dimension with another one on the same axis.

<div id="tutorial_03"></div>

```javascript
// {% include "tutorial/group_stack/03_b.js" %}
```

To stack a grouped chart, you just have to do the same thing the other way
around: detach the dimension from the x-axis and attach it to the y-axis.

<div id="tutorial_04"></div>

```javascript
// {% include "tutorial/group_stack/04_b.js" %}
```

<script src="../assets/snippet.js" config="../group_stack/config.js"></script>
