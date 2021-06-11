const loremIpsum = `Sed ut perspiciatis, unde omnis iste natus error sit voluptatem accusantium doloremque laudantium, totam rem aperiam eaque ipsa, quae ab illo inventore veritatis et quasi architecto beatae vitae dicta sunt, explicabo. Nemo enim ipsam voluptatem, quia voluptas sit, aspernatur aut odit aut fugit, sed quia consequuntur magni dolores eos, qui ratione voluptatem sequi nesciunt, neque porro quisquam est, qui dolorem ipsum, quia dolor sit amet consectetur adipisci[ng] velit, sed quia non numquam [do] eius modi tempora inci[di]dunt, ut labore et dolore magnam aliquam quaerat voluptatem. Ut enim ad minima veniam, quis`;

const documentation =
[
	{
		id: 'descriptor-channels-y',
		title: 'Y axis',
		description: loremIpsum,
		enter: chart => chart.animate({
			descriptor: {
				channels: {
					y: { attach: ['Category 1'] }
				}
			}
		}),
		leave: chart => chart.animate({
			descriptor: {
				channels: {
					y: { detach: ['Category 1'] }
				}
			}
		})
	},
	{
		id: 'descriptor-channels-color-categorical',
		title: 'Color - Categorical',
		description: loremIpsum,
		enter: chart => chart.animate({
			descriptor: {
				channels: {
					color: { attach: ['Timeseries'] }
				},
				legend: 'color'
			}
		}),
		leave: chart => chart.animate({
			descriptor: {
				channels: {
					color: { detach: ['Timeseries'] }
				}
			}
		})
	},
	{
		id: 'descriptor-channels-color-values',
		title: 'Color - Values',
		description: loremIpsum,
		enter: chart => chart.animate({
			descriptor: {
				channels: {
					color: { attach: ['Values 1'] }
				},
				legend: 'color'
			}
		}),
		leave: chart => chart.animate({
			descriptor: {
				channels: {
					color: { detach: ['Values 1'] }
				}
			}
		})
	},
	{
		id: 'descriptor-channels-lightness',
		title: 'Lightness',
		description: loremIpsum,
		enter: chart => chart.animate({
			descriptor: {
				channels: {
					lightness: { attach: ['Values 1'] }
				},
				legend: 'lightness'
			}
		}),
		leave: chart => chart.animate({
			descriptor: {
				channels: {
					lightness: { detach: ['Values 1'] }
				}
			}
		})
	},
	{
		id: 'descriptor-channels-label',
		title: 'Label',
		description: loremIpsum,
		enter: chart => chart.animate({
			descriptor: {
				channels: {
					label: { attach: ['Values 1'] }
				}
			}
		}),
		leave: chart => chart.animate({
			descriptor: {
				channels: {
					label: { detach: ['Values 1'] }
				}
			}
		})
	}

];

export default documentation;
