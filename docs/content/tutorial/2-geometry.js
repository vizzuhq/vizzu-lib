export const tutorial = doc()
.h2('Geometry')
.p(`
In Vizzu you can set the geometric elements used to represent your data by the 
geometry property within the config object. 
`)
.code('Changing geometry', chart => 
	chart.animate({
		config: {
			channels: {
				y: { set: ['Popularity'] },
				x: { set: ['Genres'] }
			}
		}
	})
)
.h3('2.1') 
.p(`
Switching the geometry to area.
`)
.code('Geometry: area', chart => 
	chart.animate({
		config: {
			geometry: 'area'
		}
	})
)
.h3('2.2') 
.p(`
Drawing a line chart.
`)
.code('Geometry: line', chart => 
	chart.animate({
		config: {
			geometry: 'line'
		}
	})
)
.h3('2.3') 
.p(`
Switching the geometry to circle. This setting is the most useful when used 
together with the size channel, as shown in the next chapter of the tutorial.
`)
.code('Geometry: circle', chart => 
	chart.animate({
		config: {
			geometry: 'circle'
		}
	})
)
.h3('2.4') 
.p(`
Rectangle geometry is the default setting in Vizzu, used for most common charts 
like bar and column charts.
`)
.code('Geometry: rectangle - default', chart => 
	chart.animate({
		config: {
			geometry: 'rectangle'
		}
	})
)
