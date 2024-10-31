---
data_url: ../../assets/data/music_data.js
---

# Animation options

In `Vizzu` you can set the timing and duration of the animation. You can do this
either for the whole animation, or for animation groups such as the elements
moving along the x-axis or the y-axis, appearing or disappearing or when the
coordinate system is changed.

Let’s see first a simple example when a stacked column chart is grouped using
the default animation options.

<div id="tutorial_01"></div>

// {% include-markdown "tutorial/assets/setup/setup_c.md" %}

```javascript
// {% include "tutorial/animation_options/01_b.js" %}
```

We stack the columns, still with the default options.

<div id="tutorial_02"></div>

```javascript
// {% include "tutorial/animation_options/02_b.js" %}
```

Now we change the animation settings for the elements moving along the y-axis
and also the change in styles, more specifically when the labels on the markers
move from the center of the chart elements to the top of them.

<div id="tutorial_03"></div>

```javascript
// {% include "tutorial/animation_options/03_b.js" %}
```

This is an example of changing the settings for the whole animation at once.

<div id="tutorial_04"></div>

```javascript
// {% include "tutorial/animation_options/04_b.js" %}
```

When the two settings are combined, `Vizzu` will use the general animation
options and spread the unique settings for specific groups proportionally. This
is why you can see the same animation as two steps before but happening much
quicker since the duration of the whole animation is set to 1 second.

<div id="tutorial_05"></div>

```javascript
// {% include "tutorial/animation_options/05_b.js" %}
```

The default unit for animation is seconds, but you can set other units. Besides
if you only want to change the duration of the animation, you can use the
following, simplified format to do that.

<div id="tutorial_06"></div>

```javascript
// {% include "tutorial/animation_options/06_b.js" %}
```

<script src="../tutorial.js"></script>
