---
data_url: ../../assets/data/music_data.js
---

# Animation control & keyframes

Using the result object provided by the chart's animate method you can play,
pause, stop, seek, speed up or reverse the animations.

In this step, we seek forward to `50%` of progress after the animation starts.

<div id="tutorial_01"></div>

// {% include-markdown "tutorial/assets/setup/setup_c.md" %}

```javascript
// {% include "tutorial/animation_control_keyframes/01.js" %}
```

You can also control the initial position and play state of the animation
through the animation options argument of the animate method.

<div id="tutorial_02"></div>

```javascript
// {% include "tutorial/animation_control_keyframes/02_b.js" %}
```

You may want to control multiple animations as a single one. For example you
might want to go back to the original state if any of the animations get
canceled, or want to seamlessly seek through all of them.

You can do this by boundling them together and passing them to a single
`animate` call. To do this, you need to create a `Keyframe` object from the
arguments of every single `animate` call and then puting them into an array of
keyframes.

<div id="tutorial_03"></div>

```javascript
// {% include "tutorial/animation_control_keyframes/03_b.js" %}
```

The initial state of the animation can be set, too, by using the second argument
of the `animate` method.

<script src="../assets/snippet.js" config="../animation_control_keyframes/config.js"></script>
