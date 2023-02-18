# Vizzu JS Library Reference

This is the API reference document of the `Vizzu` `JS` library. It provides
information about every detail of the API. This works best for users who already
have a basic understanding of the `Vizzu` library and its logic.

In case you're just getting started with `Vizzu`, we strongly recommend visiting
our [Tutorial](../tutorial/index.md) first.

## Library Overview

The main entry point of the library is the [Vizzu](./classes/vizzu.Vizzu.md)
class, and its most important component is the
[animate](./classes/vizzu.Vizzu.md#animate) method:

- [Vizzu](./classes/vizzu.Vizzu.md) class
  - [constructor()](./classes/vizzu.Vizzu.md#constructor)
  - [animate](./classes/vizzu.Vizzu.md#animate) (
    [Anim.Target](./interfaces/vizzu.Anim.Target.md) ,
    [Anim.Options](./interfaces/vizzu.Anim.Options.md) ) :
    [Anim.Control](./interfaces/vizzu.Anim.Control.md)

The `animate` method's main parameter is the
[Anim.Target](./interfaces/vizzu.Anim.Target.md) interface, which contains the
configuration of the chart, the underlying data, and the chart's style settings:

- [Anim.Target](./interfaces/vizzu.Anim.Target.md)
  - [Data.Set](./modules/vizzu.Data.md#Set)
  - [Config.Chart](./interfaces/vizzu.Config.Chart.md)
    - [Config.Channels](./interfaces/vizzu.Config.Channels.md)
  - [Styles.Chart](./interfaces/vizzu.Styles.Chart.md)
    - [Styles.Plot](./interfaces/vizzu.Styles.Plot.md)
      - [Styles.Marker](./interfaces/vizzu.Styles.Marker.md)
      - [Styles.Axis](./interfaces/vizzu.Styles.Axis.md)
    - [Styles.Legend](./interfaces/vizzu.Styles.Legend.md)
    - [Styles.Tooltip](./interfaces/vizzu.Styles.Tooltip.md)

## Details

You can find all interface declarations and types under these namespaces.

- [Data](./modules/vizzu.Data.md) - Data structure and operations
- [Config](./modules/vizzu.Config.md) - Configuration settings of your charts
- [Style](./modules/vizzu.Styles.md) - Style settings
- [Anim](./modules/vizzu.Anim.md) - Animation settings
- [Events](./modules/vizzu.Event.md) - Event handling
- [Presets](./modules/presets.md) - Preset chart configurations
