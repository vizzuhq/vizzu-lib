import tutorial from './tutorial-document.js';

tutorial.section('Without coordinates & noop channel')
.p(`
Certain chart types have neither measures nor dimensions on the axes such as 
treemaps and bubble charts. This is a case when the noop channel comes in handy 
for grouping and stacking elements in these kinds of charts
`)
.code('Charts without coordinates = no data on the axes', chart =>
	chart.animate({
		config: {
			channels: {
				y: { attach: ['Popularity', 'Types'] },
				x: { attach: ['Genres'] },
				color: { set: ['Types'] },
				label: { set: ['Popularity'] }
			}
		}
	})
)
.h()
.p(`
To get to a treemap, we have to detach all dimensions and the measure from the 
axes and put two of them on the size channel, whereas the other dimension is 
still on the color channel. Since the same dimensions are used in both cases 
Vizzu will be able to animate between these states.
`)
.code('Treemap', chart =>
	chart.animate({
		config: {
			channels: {
				y: { set: null },
				x: { set: null },
				size: { attach: ['Genres', 'Popularity'] }
			}
		}
	})
)
.h()
.p(`
Getting from a treemap to a bubble chart is simply by changing the geometry to 
circle. This bubble chart is stacked by the Type dimension that is on the size 
channel - this is why the bubbles are in separate, small groups.
`)
.code('Bubble chart - stacked', chart =>
	chart.animate({
		config: {
			geometry: 'circle'
		}
	})
)
.h()
.p(`
In order to show all bubbles as one group, we use the noop (no operations) 
channel where we put the Type dimension from the size channel. The noop channel 
enables that even though we have a dimension on the chart, that dimension 
doesn’t change any parameter of the chart elements.
`)
.code('Bubble chart - grouped - using the noop channel', chart =>
	chart.animate({
		config: {
			channels: {
				size: { detach: ['Genres'] },
				noop: { set: ['Genres'] }
			}
		}
	})
)
