# Vizzu JS Library Reference

This is the API reference document of the **Vizzu JS library**. It provides
information about every detail of the API. This works best for users who already
have a basic understanding of the Vizzu library and its logic.

In case you're just getting started with Vizzu, we recommend visiting our
[Tutorial](../tutorial/index.md) first.

## Library Overview

The main entry point of the library is the [Vizzu](./classes/vizzu.Vizzu.md)
class, and its most important component is the
[animate](./classes/vizzu.Vizzu.md#animate) method:

[`Vizzu`](./classes/vizzu.Vizzu.md)`.`[`animate`](./classes/vizzu.Vizzu.md#animate)`(`[`Anim.AnimTarget`](./modules/types_anim.md#AnimTarget)`,`[`Anim.ControlOptions`](./interfaces/types_anim.ControlOptions.md)`):`[`Anim.Completing`](./modules/animcompleting.md)

The **animate** method's main parameter is the **AnimTarget** interface, which
is an array of keyframes containing the configuration of the chart, the
underlying data, the chart's style settings, and the animation options:

Keyframe:

- [Anim.Target](./interfaces/types_anim.Target.md)
    - [Data.Set](./modules/types_data.md#Set)
    - [Config.Chart](./interfaces/types_config.Chart.md)
        - [Config.Channels](./interfaces/types_config.Channel.md)
    - [Styles.Chart](./interfaces/types_styles.Chart.md)
        - [Styles.Plot](./interfaces/types_styles.Plot.md)
            - [Styles.Marker](./interfaces/types_styles.Marker.md)
            - [Styles.Axis](./interfaces/types_styles.Axis.md)
        - [Styles.Legend](./interfaces/types_styles.Legend.md)
        - [Styles.Tooltip](./interfaces/types_styles.Tooltip.md)
- [Anim.Options](interfaces/types_anim.Options.md)

You can find all interface declarations and types under these modules:

- [vizzu](./modules/vizzu.md) - Main Vizzu class
- [data](./modules/types_data.md) - Data structure and operations
- [config](./modules/types_config.md) - Configuration settings of your charts
- [styles](./modules/types_styles.md) - Style settings
- [anim](./modules/types_anim.md) - Animation settings
- [events](./modules/events.md) - Event handling
- [plugins](./modules/plugins.md) - Plugin infrastructure
- [presets](./modules/types_presets.md) - Preset charts
