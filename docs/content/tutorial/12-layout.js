import tutorial from './tutorial-document.js';

tutorial.section('Chart layout')
.p(`
Vizzu has three separate parts of the chart layout: the plot area that contains 
the chart, the title containing the chart title on the top, and the legend on 
the left. Vizzu automatically hides the legend when it’s unnecessary to show it. 
When the title is not in use, Vizzu hides that part automatically as well. Each 
of these parts have their own paddings on all four sides that adjusts to the 
chart size by default, but can also be set with the appropriate settings in the 
style object. All size parameters can be set in pixel, percentage and em.
`)
.code('Layout', chart =>
	chart.animate({
		config: {
			channels: {
				y: { set: ['Popularity', 'Types'] },
				x: { attach: ['Genres'] },
				color: { set: ['Types'] },
				label: { attach: ['Popularity'] }
			}
		}
	})
)
.h()
.p(`
We add different background colors to the parts of the layout to show how they 
are aligned.
`)
.code('Plot, title and legend background', chart =>
	chart.animate({
		style: {
			title: { backgroundColor: '#A0A0A0' },
			plot: { backgroundColor: '#C0C0C0' },
			legend: { backgroundColor: '#808080' },
		}
	})
)
.h()
.p(`
Setting the width of the legend.
`)
.code('Legend width', chart =>  
	chart.animate({
		style: {
			legend: {
				width: 50
			}
		}
	})
)
.h() 
.p(`
Setting the legend width back to its default value.
`)
.code('Legend width', chart => chart.animate({
	style: {
		legend: {
			width: null
		}
	}
})
)
.h()
.p(`
Changing the title paddings. By default, the title is horizontally centered 
above the plot area - separated from the legend. In this example, we set the 
title’s left padding, resulting in the text moving to the right.
`)
.code('Title padding', chart =>
	chart.animate({
		style: {
			title: {
				paddingTop: 20,
				paddingBottom: 20,
				paddingLeft: 200
			}
		}
	})
)
.h()
.p(`
Setting the title paddings back to their default values.
`)
.code('Title padding', chart =>
	chart.animate({
		style: {
			title: {
				paddingTop: null,
				paddingBottom: null,
				paddingLeft: null
			}
		}
	})
)
.h()
.p(`
Changing the paddings of the plot area to position the plot. The texts on the 
axes are drawn on the padding of the plot and not the plot itself.
`)
.code('Plot padding', chart =>  
	chart.animate({
		style: {
			plot: {
				paddingLeft: 100,
				paddingRight: 100
			}
		}
	})
)
.h()
.p(`
Setting the plot paddings back to their default values.
`)
.code('Plot padding', chart =>
	chart.animate({
		style: {
			plot: {
				paddingLeft: null,
				paddingRight: null
			}
		}
	})
)
  