export const tutorial = doc()
.h2('Switch orientation, split and using polar coordinates')
.code('Switch orientation, split, polar coordinates', chart => 
	chart.animate({
		data: data,
		config: {
			channels: {
				y: { attach: ['Popularity', 'Types'] },
				x: { set: ['Genres'] },
				color: { set: ['Types'] },
				label: { attach: ['Popularity'] }
			}
		}
	})
)
.h3('8.1')
.p(`
Switching orientation means that you put a measure from one axis to the other to 
see the data from a different perspective. This is once again a state you should 
only use temporarily.
`)
.code('Switch the orienation = arrange by other axis', chart =>
	chart.animate({
		config: {
			channels: {
				y: { detach: ['Popularity'] },
				x: { attach: ['Popularity'] }
			}
		}
	})
)
.h3('8.2')
.p(`
By turning the split parameter on, you can see stacked elements side-by-side, 
which enables the comparison of the components.
`)
.code('Split stacked values = show side-by-side', chart =>
	chart.animate({
		config: {
			split: true
		}
	})
)
.h3('8.3')
.p(`
Merging the components by turning the split parameter off
`)
.code('Merge', chart =>
	chart.animate({
		config: {
			split: false
		}
	})
)
.h3('8.4')
.p(`
We aggregate the data by removing the Genres dimension from the x-axis
`)
.code('Aggregate', chart =>
	chart.animate({
		config: {
			channels: {
				x: { detach: ['Genres'] }
			}
		}
	})
)
.h3('8.5')
.p(`
Switching from cartesian coordinates to polar. When doing so, it is worth 
setting the axes’ ranges so that the viewers can easily compare the values being 
shown. If you want to get back to the default, cartesian coordinates, just set 
the coordSystem parameter to ‘cartesian’.
`)
.code('Polar coordinates', chart =>
	chart.animate({
		config: {
			channels: {
				x: { range: { max: '133%' } },
				y: { range: { min: '-100%' } }
			},
			coordSystem: 'polar'
		}
	})
)
