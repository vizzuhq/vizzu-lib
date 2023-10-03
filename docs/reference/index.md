# Vizzu JS Library Reference

This is the API reference document of the `Vizzu` `JS` library. It provides
information about every detail of the API. This works best for users who already
have a basic understanding of the `Vizzu` library and its logic.

In case you're just getting started with `Vizzu`, we strongly recommend visiting
our [Tutorial](../tutorial/index.md) first.

## Library Overview

The main entry point of the library is the [Vizzu](./classes/Vizzu.md) class,
and its most important component is the [animate](./classes/Vizzu.md#animate)
method:

- [Vizzu](./classes/Vizzu.md) class
  - [constructor()](./classes/Vizzu.md#constructor)
  - [animate](./classes/Vizzu.md#animate) (
    [Anim.Target](./interfaces/Anim.Target.md) ,
    [Anim.Options](./interfaces/Anim.Options.md) ) :
    [Anim.Completing](./interfaces/Anim.Completing.md)

The `animate` method's main parameter is the
[Anim.Target](./interfaces/Anim.Target.md) interface, which contains the
configuration of the chart, the underlying data, and the chart's style settings:

- [Anim.Target](./interfaces/Anim.Target.md)
  - [Data.Set](./modules/Data.md#Set)
  - [Config.Chart](./interfaces/Config.Chart.md)
    - [Config.Channels](./interfaces/Config.Channels.md)
  - [Styles.Chart](./interfaces/Styles.Chart.md)
    - [Styles.Plot](./interfaces/Styles.Plot.md)
      - [Styles.Marker](./interfaces/Styles.Marker.md)
      - [Styles.Axis](./interfaces/Styles.Axis.md)
    - [Styles.Legend](./interfaces/Styles.Legend.md)
    - [Styles.Tooltip](./interfaces/Styles.Tooltip.md)

## Details

You can find all interface declarations and types under these namespaces.

- [Data](./modules/Data.md) - Data structure and operations
- [Config](./modules/Config.md) - Configuration settings of your charts
- [Style](./modules/Styles.md) - Style settings
- [Anim](./modules/Anim.md) - Animation settings
- [Events](./modules/Event.md) - Event handling
- [Presets](./modules/Presets.md) - Preset chart configurations
