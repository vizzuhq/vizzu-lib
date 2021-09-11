import tutorial from './tutorial-document.js';

tutorial.section('Animation settings')
.p(`
In Vizzu you can set the timing and duration of the animation. You can do this 
either for the whole animation, or for animation groups such as the elements 
moving along the x-axis or the y-axis, appearing or disappearing or when the 
coordinate system is changed.
`)
.code('Animation settings', chart => 
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
Let’s see first a simple example when a stacked column chart is grouped using 
the default animation settings.
`)
.code('Default settings', chart =>
	chart.animate({
		config: {
			channels: {
				y: { detach: ['Types'] },
				x: { attach: ['Types'] }
			}
		}
	})
)
.h() 
.p(`
We stack the columns, still with the default settings.
`)
.code('Default settings', chart =>
	chart.animate({
		config: {
			channels: {
				x: { detach: ['Types'] },
				y: { attach: ['Types'] }
			}
		}
	})
)
.h() 
.p(`
Now we change the animation settings, for the elements moving along the y-axis 
and also the change in styles, more specifically when the labels on the markers 
move from the center of the chart elements to the top of them.
`)
.code('Custom animation settings for specific groups', chart =>
	chart.animate({
		config: {
			channels: {
				y: { detach: ['Types'] },
				x: { attach: ['Types'] }
			}
		}
	}, 
	{
		y: { 
			duration: 2, 
			delay: 2 
		},
		style: { 
			duration: 2, 
			delay: 4 
		}
	})
)
.h() 
.p(`
This is an example of changing the settings for the whole animation at once.
`)
.code('Custom settings for the whole animation', chart =>
	chart.animate({
		config: {
			channels: {
				x: { detach: ['Types'] },
				y: { attach: ['Types'] }
			}
		}
	}, 
	{ 
		duration: 1, 
		easing: 'linear'
	})
)
.h() 
.p(`
When the two settings are combined, Vizzu will use the general animation 
settings and spread the unique settings for specific groups proportionally. 
This is why you can see the same animation as two steps before but happening 
much quicker since the duration of the whole animation is set to 1 second. 
`)
.code('Custom settings for both', chart =>
	chart.animate({
		config: {
			channels: {
				x: { attach: ['Types'] },
				y: { detach: ['Types'] }
			}
		}
	},
	{
		duration: 1, 
		easing: 'linear',
		y: { 
			duration: 2, 
			delay: 2 
		},
		style: { 
			duration: 2, 
			delay: 4 
		}
	})
)
