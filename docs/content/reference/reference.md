# Vizzu JS Library Reference

This is the API reference document of the Vizzu JS library. It provides information
about every detail of the API. This works best for users who already have a basic
understanding of the Vizzu Library and its logic.

In case you're just getting started with Vizzu, we strongly recommend visiting our 
[Tutorial](https://lib.vizzuhq.com/0.3.0) first.

## Library Overview

The main entry point of the library is the [Vizzu](classes/default.html) class, 
and its most important component is the [animate()](classes/default.html#animate) method:

* [Vizzu](classes/default.html) class
	* [constructor()](classes/default.html#constructor)
	* [animate](classes/default.html#animate) ( [AnimTarget](interfaces/AnimTarget.html) , [Anim.Options](interfaces/Anim.Options.html) )

The animate() method's main parameter is the [AnimTarget](interfaces/AnimTarget.html)
interface, which contains the configuration of the chart, the underlying data, and the chart's style settings:

* [AnimTarget](interfaces/AnimTarget.html)
	* [Data.Set](modules/Data.html#Set)
	* [Config.Chart](interfaces/Config.Chart.html)
		* [Config.Channels](interfaces/Config.Channels.html)
	* [Styles.Chart](interfaces/Styles.Chart.html)
		* [Styles.Plot](interfaces/Styles.Plot.html)
			* [Styles.Marker](interfaces/Styles.Marker.html)
			* [Styles.Axis](interfaces/Styles.Axis.html)
		* [Styles.Legend](interfaces/Styles.Legend.html)
		* [Styles.Tooltip](interfaces/Styles.Tooltip.html)
		

## Details:

You can find all interface declarations and types under these namespaces. 

* [Data](modules/Data.html) - Data structure and operations
* [Config](modules/Config.html) - Configuration settings of your charts
* [Style](modules/Styles.html) - Style settings
* [Anim](modules/Anim.html) - Animation settings
* [Events](modules/Event.html) - Event handling
