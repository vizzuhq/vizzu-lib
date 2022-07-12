## Chart presets

In this tutorial, we have shown how you can create charts with Vizzu 
using a chart type-agnostic, uniform way, without being limited to a predefined
set of available charts. But sometimes, we know exactly the chart type we 
would like to use, and it is easier to start with that, skipping all the 
necessary configurations to build it from scratch. For this reason, Vizzu
provides preset configurations for many known chart types
(for all available presets, see the [preset galery](#chart-presets)).

Presets are available through methods of the 'presets' static property of the Vizzu class.
These methods are returning chart configuration objects which can be passed to
the 'animate' method. Let's create a Stacked Bubble chart. 

```javascript { "title": "Using a preset" }
chart.animate(Vizzu.presets.stackedBubble({
	size: 'Popularity',
	color: 'Kinds',
	stackedBy: 'Genres'
}));
```

Presets will override all the channels, so previously set series will be
removed from the chart. They will also explicitly set most of the chart 
configuration parameters since these will effectively result in the selected
chart type. Exceptions are the 'legend', 'title', 'reverse', and 'sort' properties
which can be set along with the preset. Let's sort the next preset chart by values.

```javascript { "title": "Setting sorting for a preset" }
chart.animate(Vizzu.presets.radialStackedBar({
	angle: 'Popularity',
	radius: 'Genres',
	stackedBy: 'Kinds',
	sort: 'byValue'
}));
```

Presets will affect only the chart configuration. If you need to set the styles
or the underlying data, you can use the more verbose syntax below, explicitly
pass the preset to the 'config' property of the animate method's parameter. 

```javascript { "title": "Setting style for a preset" }
chart.animate({
	config: Vizzu.presets.radialBar({
		angle: 'Popularity',
		radius: 'Genres',
	}),
	style: {
		'plot.xAxis.interlacing.color': '#ffffff00'
	}
});
```

As you can see, the preset won't override the previously configured sorting, 
and it won't affect the rest of the chart config parameters mentioned above, 
which can be set along with the preset.
