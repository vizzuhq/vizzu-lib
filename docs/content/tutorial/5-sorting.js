import tutorial from './tutorial-document.js';

tutorial.section('Sorting')
.p(`
Vizzu provides multiple options to sort data. By default, the data is sorted by 
the order it is added to Vizzu. This is why we suggest to add temporal data such 
as dates in chronological order - from oldest to newest.
`)
.code('Sort options - default: as in the data', chart => 
	chart.animate({
		config: {
			channels: {
				y: { set: ['Popularity', 'Types'] },
				x: { set: ['Genres'] },
				color: { attach: ['Types'] },
				label: { set: ['Popularity'] }
			}
		}
	})
)
.h()  
.p(`
You can also sort the elements by value, which will provide you with an 
ascending order.
`)
.code('Switch to ascending order...', chart =>
	chart.animate({
		config: {
			sort: 'byValue'
		}
	})
)
.h() 
.p(`
If you want descending order instead, you have to set the reverse parameter to 
true. When used without setting the sorting to byValue, the elements will be in 
the opposite order than they are in the data set added to Vizzu.
`)
.code('...or descending order.', chart =>
	chart.animate({
		config: {
			reverse: true
		}
	})
)
.h() 
.p(`
This is how to switch back to the default sorting
`)
.code('Let\'s get back to where we were', chart =>
	chart.animate({
		config: {
			sort: 'none',
			reverse: false
		}
	})
)
.h() 
.p(`
When you have more than one dimension on a channel, their order determines how 
the elements are grouped. 
`)
.code('With two discretes on one axis...', chart => 
	chart.animate({
		config: {
			channels: {
				y: { detach: ['Types'] },
				x: { set: ['Genres', 'Types'] }
			}
		}
	})
)
.h() 
.p(`
When switching the order of dimensions on the x-axis Vizzu will rearrange the 
elements according to this new logic.
`)
.code('...grouping is determined by their order.', chart => 
	chart.animate({
		config: {
			channels: {
				x: { set: ['Types', 'Genres'] },
			},
			legend: 'color'
		}
	})
)
