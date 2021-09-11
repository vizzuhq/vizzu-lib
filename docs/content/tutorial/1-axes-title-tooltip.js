import tutorial from './tutorial-document.js';
import data from './tutorial-data.js';

tutorial.section('Axes, title, tooltip')
.p(`
To build a chart with Vizzu, you have to add data series to the channels.
`)
.p(`
The first step is to create a simple column chart, adding one of the dimensions 
(Genre) to the x-axis and the measure (Popularity) to the y-axis using the set 
property.
`)
.code(null, chart => 
	chart.animate({ 
		data: data,
		config: {
			channels: {
				y: { set: ['Popularity'] },
				x: { set: ['Genres'] }
			}
		}
	})
)
.h()
.p(`
In the next step, the chart is rearranged by putting both series on the y-axis 
using once again the set property, resulting in a single column chart. Vizzu 
automatically animates between the initial state and this one.
`)
.code(null, chart => 
	chart.animate({
		config: {
		channels: {
				x: { set: null },
				y: { set: ['Genres','Popularity'] }
			}
		}
	})
) 
.h()
.p(`
Instead of set, you can use attach and detach to add or remove series to/from 
the channels. Using these commands makes it easier to build your animated charts 
step-by-step, however you have to keep in mind what you had on which channel in 
the previous step.
`)
.code(null, chart => 
	chart.animate({
		config: {
			channels: {
				y: { detach: ['Popularity'] },
				x: { attach: ['Popularity'] }
			}
		}
	})
)
.h()
.p(`
Setting the chart title with the title command.
`)
.code(null, chart => 
	chart.animate({ title:'My first chart' })
)
.h()
.p(`
Switching on the tooltips that appear on the chart elements when the user hovers 
over them with their mouse by adding the (tooltip, true) parameters to the 
chart.feature method. Please note that this time we call the chart.feature method 
instead of the chart.animate method used in the previous steps.
`)
.code(null, chart => {
	chart.feature('tooltip',true);
	return chart;
}) 
