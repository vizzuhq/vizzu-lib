# Vizzu JS Library Reference

This is the API reference documentation of the Vizzu JS library. 
The intention of this documentation is to provide information about 
every details of the API for user with understanding of the basics of the 
Vizzu Library.

For a getting started introduction to the library please visit our 
[Tutorial](https://lib.vizzuhq.com/0.3.0).

## Library Overview

The main entry point of the library is the [Vizzu](classes/default.html) class, 
and it's most important method controlling the basic functionality of the 
library is the [animate()](classes/default.html#animate) method:

* [Vizzu](classes/default.html) class
	* [constructor()](classes/default.html#constructor)
	* [animate](classes/default.html#animate) ( [AnimTarget](interfaces/AnimTarget.html) , [Anim.Options](interfaces/Anim.Options.html) )

The animate() method's main parameter is the [AnimTarget](interfaces/AnimTarget.html)
interface which contains the definition of the chart, the underlying data and the chart's styling:

* [AnimTarget](interfaces/AnimTarget.html)
	* [Data.Set](modules/Data.html#Set)
	* [Config.Chart](interfaces/Config.Chart.html)
		* [Config.Channels](interfaces/Config.Channels.html)
	* [Styles.Chart](interfaces/Styles.Chart.html)
		* [Styles.Plot](interfaces/Styles.Plot.html)
			* [Styles.Marker](interfaces/Styles.Marker.html)
			* [Styles.Axis](interfaces/Styles.Axis.html)
		* [Styles.Legend](interfaces/Styles.Legend.html)
		* [Styles.Title](interfaces/Styles.Title.html)
		* [Styles.Tooltip](interfaces/Styles.Tooltip.html)
		

## Details:

You can find all interface declarations and types under these namespaces. 

### Namespaces:

* [Data](modules/Data.html)
* [Config](modules/Config.html)
* [Style](modules/Styles.html)
* [Anim](modules/Anim.html)
* [Events](modules/Event.html)
