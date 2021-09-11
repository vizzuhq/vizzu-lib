import tutorial from './tutorial-document.js';

tutorial.section('Color palette & fonts')
.p(`
This and the next chapter provide a quick intro to the styling of the charts using the style object. The style settings are similar to CSS. The font sizes automatically adjust to the chart size to help readability, but they can also be set separately or for specific groups.
`)
.code('Style settings', chart => 
	chart.animate({
		config: {
			channels: {
				y: { set: ['Popularity', 'Types'] },
				x: { attach: ['Genres'] },
				color: { attach: ['Types'] },
				label: { set: ['Popularity'] }
			}
		}
	})
)
.h() 
.p(`
The color palette is changed to the colors we add here. The order of the dimension’s items in the data set determine which color belongs to which item as the colors are added one-by-one. 
`)
.code('Color palette', chart =>
	chart.animate({
		style: {
			plot: {
				marker: {
					colorPalette: '#9355e8FF #123456FF #BDAF10FF'
				}
			}
		}
	})
)
.h() 
.p(`
Changing the title font size will only affect the title; all other font sizes remain the same.
`)
.code('Title font size', chart =>
	chart.animate({
		style: {
			title: {
				fontSize: 50
			}
		}
	})
)
.h()
.p(`
This is how to set the font size back to its default value
`)
.code('Title font size - back to default', chart =>
	chart.animate({
		style: {
			title: {
				fontSize: null
			}
		}
	})
)
.h() 
.p(`
In case you change the font size of the whole chart with the top-level fontSize parameter then every font on the chart will grow/shrink proportionally. The size refers to the font size of the axis labels by default.
`)
.code('Setting all font sizes in one step', chart =>
	chart.animate({
		style: {
			fontSize: 20
		}
	})
)
.h() 
.p(`
You can reset styles to default on any levels by setting them to null. 
`)
.code('Setting all style settings back to default', chart =>
	chart.animate({
		style: null
	})
)
