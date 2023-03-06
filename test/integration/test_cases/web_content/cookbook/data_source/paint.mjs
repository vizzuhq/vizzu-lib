import { data } from '../../../../test_data/chart_types_eu.mjs';

const testSteps = [
  chart => 
  {
	let data = {
		series: [
		  { name: 'index', type: 'dimension' },
		  { name: 'x', type: 'measure' },
		  { name: 'y', type: 'measure' }
		]
	  };
	  
	  let index = 0;

	  chart.on('click', event => {
		chart.animate({
		  data: {
			records: [[ index++, event.data.coords.x, event.data.coords.y ]]
		  }
		},0.1)
	  });
	
	  return chart.animate(
		{
		  data,
		  config: {
			x: { set: 'x', range: { min: 0, max: 1 } },
			y: { set: 'y', range: { min: 0, max: 1 } },
			geometry: 'circle',
			noop: 'index',
			title: 'Click the chart to put in data points!'
		  }
	  })
  }
];

export default testSteps;
