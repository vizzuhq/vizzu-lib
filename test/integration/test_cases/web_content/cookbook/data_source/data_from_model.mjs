const testSteps = [
	async chart => 
	{
		let recordCount = 100;
		let t = 0;
		let data;

		function init()
		{
			function Rand(a) {
				return function() {
				  var t = a += 0x6D2B79F5;
				  t = Math.imul(t ^ t >>> 15, t | 1);
				  t ^= t + Math.imul(t ^ t >>> 7, t | 61);
				  return ((t ^ t >>> 14) >>> 0) / 4294967296;
				}
			}
			
			let rand = Rand(32);
	
			data = {
				series: [
					{ name: 'index', type: 'dimension', values: [] },
					{ name: 'x', type: 'measure', values: [] },
					{ name: 'y', type: 'measure', values: [] },
					{ name: 'vx', type: 'measure', values: [] },
					{ name: 'vy', type: 'measure', values: [] },
					{ name: 'size', type: 'measure', values: [] }
				]
			}
	
			for (let i = 0; i < recordCount; i++) {
				data.series[0].values.push(`${i}`);
				data.series[1].values.push(rand());
				data.series[2].values.push(rand());
				data.series[3].values.push(rand()*3);
				data.series[4].values.push(rand()*5);
				data.series[5].values.push(rand());
			}
		}

		function update(chart) 
		{
			if (t > 300) return;
			let x = data.series[1].values;
			let y = data.series[2].values;
			let vx = data.series[3].values;
			let vy = data.series[4].values;
			let size = data.series[5].values;
			for (let i = 0; i < recordCount; i++) {
				const g = 9.81;
				const fric = 0.5;
				const dt = 0.02;
				let ax = - fric * vx[i] * size[i];
				let ay = -g - fric * vy[i] * size[i];
				vx[i] += ax * dt;
				vy[i] += ay * dt;
				x[i] += vx[i] * dt;
				y[i] += vy[i] * dt;
				if (y[i] < 0) { 
					y[i] = -y[i];
					vy[i] = -vy[i] * 0.6;
				}
				if (x[i] < 0) { 
					x[i] = -x[i];
					vx[i] = -vx[i] * 0.6;
				}
				if (x[i] > 1) { 
					x[i] = 2-x[i];
					vx[i] = -vx[i] * 0.6;
				}
				t += dt;
			}
			return chart.animate({ data }, '20ms').then(chart => update(chart));
		}

		init();

		return chart.animate({
			data: data,
			config: {
				title: 'Bouncing balls realtime simulation',
				x: { set: 'x', range: { min: 0, max: 1} },
				y: { set: 'y', range: { min: 0, max: 1} },
				color: 'index',
				size: 'size',
				geometry: 'circle',
				legend: null
			}
		}, 0)
		.then(chart => update(chart))
	}
];

export default testSteps;