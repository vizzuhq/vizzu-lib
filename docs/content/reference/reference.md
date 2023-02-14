# Vizzu JS Library Reference

This is the API reference document of the Vizzu JS library. It provides information
about every detail of the API. This works best for users who already have a basic
understanding of the Vizzu Library and its logic.

In case you're just getting started with Vizzu, we strongly recommend visiting our 
[Tutorial](../tutorial) first.

## Library Overview

The main entry point of the library is the [Vizzu](classes/vizzu.Vizzu/) class, 
and its most important component is the [animate()](classes/vizzu.Vizzu/#animate) method:

- [Vizzu](classes/vizzu.Vizzu/) class
  - [constructor()](classes/vizzu.Vizzu/#constructor)
  - [animate](classes/vizzu.Vizzu/#animate) ( [Anim.Target](interfaces/vizzu.Anim.Target/) , [Anim.Options](interfaces/vizzu.Anim.Options/) )
    : [Anim.Control](interfaces/vizzu.Anim.Control/)

The animate() method's main parameter is the [Anim.Target](interfaces/vizzu.Anim.Target/)
interface, which contains the configuration of the chart, the underlying data, and the chart's style settings:

- [Anim.Target](interfaces/vizzu.Anim.Target/)
  - [Data.Set](modules/vizzu.Data/#Set)
  - [Config.Chart](interfaces/vizzu.Config.Chart/)
    - [Config.Channels](interfaces/vizzu.Config.Channels/)
  - [Styles.Chart](interfaces/vizzu.Styles.Chart/)
    - [Styles.Plot](interfaces/vizzu.Styles.Plot/)
      - [Styles.Marker](interfaces/vizzu.Styles.Marker/)
      - [Styles.Axis](interfaces/vizzu.Styles.Axis/)
    - [Styles.Legend](interfaces/vizzu.Styles.Legend/)
    - [Styles.Tooltip](interfaces/vizzu.Styles.Tooltip/)


## Details

You can find all interface declarations and types under these namespaces. 

- [Data](modules/vizzu.Data/) - Data structure and operations
- [Config](modules/vizzu.Config/) - Configuration settings of your charts
- [Style](modules/vizzu.Styles/) - Style settings
- [Anim](modules/vizzu.Anim/) - Animation settings
- [Events](modules/vizzu.Event/) - Event handling
- [Presets](modules/presets/) - Preset chart configurations
