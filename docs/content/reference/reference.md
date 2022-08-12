# Vizzu JS Library Reference

This is the API reference document of the Vizzu JS library. It provides information
about every detail of the API. This works best for users who already have a basic
understanding of the Vizzu Library and its logic.

In case you're just getting started with Vizzu, we strongly recommend visiting our 
[Tutorial](https://lib.vizzuhq.com/0.5) first.

## Library Overview

The main entry point of the library is the [Vizzu](classes/vizzu.Vizzu.html) class, 
and its most important component is the [animate()](classes/vizzu.Vizzu.html#animate) method:

* [Vizzu](classes/vizzu.Vizzu.html) class
	* [constructor()](classes/vizzu.Vizzu.html#constructor)
	* [animate](classes/vizzu.Vizzu.html#animate) ( [AnimTarget](interfaces/vizzu.AnimTarget.html) , [Anim.Options](interfaces/vizzu.Anim.Options.html) )
	  : [Anim.Control](interfaces/vizzu.Anim.Control.html)

The animate() method's main parameter is the [AnimTarget](interfaces/vizzu.AnimTarget.html)
interface, which contains the configuration of the chart, the underlying data, and the chart's style settings:

* [AnimTarget](interfaces/vizzu.AnimTarget.html)
	* [Data.Set](modules/vizzu.Data.html#Set)
	* [Config.Chart](interfaces/vizzu.Config.Chart.html)
		* [Config.Channels](interfaces/vizzu.Config.Channels.html)
	* [Styles.Chart](interfaces/vizzu.Styles.Chart.html)
		* [Styles.Plot](interfaces/vizzu.Styles.Plot.html)
			* [Styles.Marker](interfaces/vizzu.Styles.Marker.html)
			* [Styles.Axis](interfaces/vizzu.Styles.Axis.html)
		* [Styles.Legend](interfaces/vizzu.Styles.Legend.html)
		* [Styles.Tooltip](interfaces/vizzu.Styles.Tooltip.html)
		

## Details:

You can find all interface declarations and types under these namespaces. 

* [Data](modules/vizzu.Data.html) - Data structure and operations
* [Config](modules/vizzu.Config.html) - Configuration settings of your charts
* [Style](modules/vizzu.Styles.html) - Style settings
* [Anim](modules/vizzu.Anim.html) - Animation settings
* [Events](modules/vizzu.Event.html) - Event handling
* [Presets](modules/presets.html) - Preset chart configurations
