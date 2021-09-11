import tutorial from './tutorial-document.js';

tutorial.section('Aggregate/drill-down')
.p(`
These features basically mean that you add or remove an additional dimension 
to/from an axis or another channel. As you can see below, there are some 
important things to keep in mind when you use them.
`)
.code('Aggregate, drill-down', chart => 
	chart.animate({
		config: {
			channels: {
				y: { attach: ['Popularity', 'Types'] },
				x: { set: ['Genres'] },
				color: { attach: ['Types'] },
				label: { set: ['Popularity'] }
			}
		}
	})
)
.h()
.p(`
Let’s stack together the elements by putting the Genres dimension from the 
x-axis to the y-axis. At the end of this phase, there are chart elements with 
the same color stacked on top of each other, which is something you would want 
to avoid. 
`)
.code('Stack', chart =>
	chart.animate({
		config: {
			channels: {
				y: { attach: ['Genres'] },
				x: { set: null }
			}
		}
	})
)
.h() 
.p(`
By taking the Genres off of the y-axis, only one chart element remains for every 
color, and Vizzu automatically calculates and shows the aggregate value of the 
elements. 
`)
.p(`
Note: Instead of taking the unwanted dimension down from the chart, Genres could 
have been added to the lightness channel to differentiate the chart elements.
`)
.code('Aggregate elements', chart =>
	chart.animate({
		config: {
			channels: {
				y: { detach: ['Genres'] }
			}
		}
	})
)
.h() 
.p(`
To drill-down, the same category is put back on the y-axis, which results in a 
state that we suggest you to only use temporarily when in transition.
`)
.code('Drill-down', chart =>
	chart.animate({
		config: {
			channels: {
				y: { attach: ['Genres'] }
			}
		}
	})
)
.h() 
.p(`
We group the elements by putting once again the Genres dimension on the x-axis.
`)
.code('Group', chart =>
	chart.animate({
		config: {
			channels: {
				y: { detach: ['Genres'] },
				x: { set: ['Genres'] }
			}
		}
	})
)