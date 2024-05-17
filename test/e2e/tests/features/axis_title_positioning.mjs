const data = {
	series: [
		{ name: 'Dim0', type: 'dimension', values: ['a', 'b'] },
		{ name: 'Meas0', type: 'measure', values: [-0.5, 1] },
		{ name: 'Meas1', type: 'measure', values: [-0.3, 1] }
	]
}

const testSteps = [
	(chart) => chart.animate({ data }),
	(chart) =>
		chart.animate({
			x: 'Meas0',
			y: 'Meas1',
			label: 'Dim0',
			geometry: 'circle'
		}),
	(chart) => chart.animate({ coordSystem: 'polar' })
]

function titleStyle(pos, vpos, vside, side) {
	return {
		vposition: vpos,
		position: pos,
		vside,
		side
	}
}

function style(pos, vpos, vside, side) {
	return {
		plot: {
			xAxis: { title: titleStyle(pos, vpos, vside, side) },
			yAxis: { title: titleStyle(pos, vpos, vside, side) }
		}
	}
}

for (const vside of ['positive', 'upon', 'negative'])
	for (const side of ['positive', 'upon', 'negative'])
		for (const coordSystem of ['cartesian', 'polar']) {
			testSteps.push((chart) =>
				chart.animate({
					config: {
						x: { title: `x:${side}-${vside}` },
						y: { title: `y:${side}-${vside}` },
						coordSystem
					},
					style: style('axis', 'end', vside, side)
				})
			)
		}

for (const pos of ['min-edge', 'axis', 'max-edge'])
	for (const vpos of ['begin', 'middle', 'end'])
		for (const coordSystem of ['cartesian', 'polar']) {
			testSteps.push((chart) =>
				chart.animate({
					config: {
						x: { title: `x:${pos}-${vpos}` },
						y: { title: `y:${pos}-${vpos}` },
						coordSystem
					},
					style: style(pos, vpos, 'upon', 'upon')
				})
			)
		}

export default testSteps
