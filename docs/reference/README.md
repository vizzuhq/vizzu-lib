# Vizzu JS Library Reference

This is the API reference document of the **Vizzu JS library**. It provides
information about every detail of the API. This works best for users who already
have a basic understanding of the Vizzu library and its logic.

In case you're just getting started with Vizzu, we recommend visiting our
[Tutorial](../tutorial/index.md) first.

## Library Overview

The main entry point of the library is the [Vizzu](./vizzu/classes/vizzu.md)
class, and its most important component is the
[animate](./vizzu/classes/vizzu.md#animate) method:

[`Vizzu`](./vizzu/classes/vizzu.md)`.`[`animate`](./vizzu/classes/vizzu.md#animate)`(`[`Anim.AnimTarget`](./types/anim/type-aliases/AnimTarget.md)`,`[`Anim.ControlOptions`](./types/anim/interfaces/ControlOptions.md)`):`[`Anim.Completing`](./animcompleting/README.md)

The **animate** method's main parameter is the **AnimTarget** interface, which
is an array of keyframes containing the configuration of the chart, the
underlying data, the chart's style settings, and the animation options:

Keyframe:

- [Anim.Target](./types/anim/interfaces/Target.md)
    - [Data.Set](./types/data/type-aliases/Set.md)
    - [Config.Chart](./types/config/interfaces/Chart.md)
        - [Config.Channels](./types/config/interfaces/Channel.md)
    - [Styles.Chart](./types/styles/interfaces/Chart.md)
        - [Styles.Plot](./types/styles/interfaces/Plot.md)
            - [Styles.Marker](./types/styles/interfaces/Marker.md)
            - [Styles.Axis](./types/styles/interfaces/Axis.md)
        - [Styles.Legend](./types/styles/interfaces/Legend.md)
        - [Styles.Tooltip](./types/styles/interfaces/Tooltip.md)
- [Anim.Options](./types/anim/interfaces/Options.md)

You can find all interface declarations and types under these modules:

- [vizzu](./vizzu/README.md) - Main Vizzu class
- [data](./types/data/README.md) - Data structure and operations
- [config](./types/config/README.md) - Configuration settings of your charts
- [styles](./types/styles/README.md) - Style settings
- [anim](./types/anim/README.md) - Animation settings
- [events](./events/README.md) - Event handling
- [plugins](./plugins/README.md) - Plugin infrastructure
- [presets](./types/presets/README.md) - Preset charts
