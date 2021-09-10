export const tutorial = doc()
.h2('Adding new dimensions to channels - group/stack')
.p(`
When you add dimensions to the channels, there is a simple rule to keep in mind 
in the current beta phase of Vizzu. The following example also shows how to 
group and stack elements of a bar chart.
`)
.code('Adding categories - group/stack', chart => 
	chart.animate({
		config: {
			channels: {
				y: { set: ['Popularity'] },
				x: { set: ['Genres'] }
			}
		}
	})
)
.h3('4.1')  
.p(`
When you add a dimension (Type) to a channel (x-axis) with only dimensions on 
it, Vizzu will fade between the states, as shown here. Thus, you get a grouped 
bar chart, but the connection between the original state and this one will be 
unclear for the viewer because of the fading.
`)
.code('The wrong way means no animation', chart => 
	chart.animate({
		config: {
			channels: {
				x: { set: ['Genres', 'Types'] },
				color: { attach: ['Types'] }
			}
		}
	})
)
.h3('4.2')
.p(`
Let’s get back to the original state by removing the extra dimension - Vizzu 
will also simply fade between these states.
`)
.code('Let\'s get back to where we were', chart => 
	chart.animate({
		config: {
			channels: {
				x: { detach: ['Types'] },
				color: { set: null }
			}
		}
	})
)
.h3('4.3')
.p(`
The right way is to add the new dimension to the same channel where the measure 
is: the y-axis. However, since doing only this would result in multiple column 
chart elements with the same color stacked on top of each other, we also add the 
same dimension (Type) to the color channel.
`)
.code('The right way: add to the same channel where the measure is', chart => 
	chart.animate({
		config: {
			channels: {
				y: { attach: ['Types'] },
				color: { attach: ['Types'] }
			}
		}
	})
)
.h3('4.4')
.p(`
By detaching this newly added dimension from the y-axis and attaching it to the 
x-axis, you get to the same grouped bar chart as in the first step but in a way 
that is easy to follow for the viewer.
`)
.code('...then you can add it to another channel = group elements...', chart => 
	chart.animate({
		config: {
			channels: {
				y: { detach: ['Types'] },
				x: { attach: ['Types'] }
			}
		}
	})
)
.h3('4.5') 
.p(`
To stack a grouped chart, you just have to do the same thing the other way 
around: detach the dimension from the x-axis and attach it to the y-axis.
`)
.code('...doing it the other way is how you stack your chart', chart => 
	chart.animate({
		config: {
			channels: {
				x: { detach: ['Types'] },
				y: { attach: ['Types'] }
			}
		}
	})
)