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
		let convert = chart.getConverter("plot-area", "canvas", "relative");
		let rel = convert(event.detail.position);
		chart.animate({
		  data: {
			records: [[ index++, rel.x, rel.y ]]
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
  },
  chart => 
  {
	chart.module._vizzu_pointerDown(0, 100, 100);
	chart.module._vizzu_pointerUp(0, 100, 100);
	return chart.anim;
  },
  chart => 
  {
	chart.module._vizzu_pointerDown(0, 300, 200);
	chart.module._vizzu_pointerUp(0, 300, 200);
	return chart.anim;
  }
];

export default testSteps;
