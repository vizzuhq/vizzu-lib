import Chart from '../../../../chart.mjs'

// Deterministic stand-in for Math.random() (mulberry32),
// so the rendered reference hash is reproducible.
let seed = 1
const random = () => {
	seed = (seed + 0x6d2b79f5) | 0
	let t = Math.imul(seed ^ (seed >>> 15), 1 | seed)
	t = (t + Math.imul(t ^ (t >>> 7), 61 | t)) ^ t
	return ((t ^ (t >>> 14)) >>> 0) / 4294967296
}

const alpha = Array.from(Array(26)).map((e, i) => i + 65)
const alphabet = alpha.map((x) => String.fromCharCode(x).repeat(1 + random() * 20))

const data = {
	series: [
		{ name: 'Foo', values: alphabet },
		{ name: 'Bar', values: alpha }
	]
}

const config = {
	x: 'Foo',
	y: 'Bar'
}

const style = (label) => ({
	plot: {
		xAxis: {
			label
		}
	}
})

const testCases = [
	{
		testName: 'static',
		testSteps: [
			Chart.animate({
				data,
				config
			})
		]
	},
	{
		testName: 'static_30deg',
		testSteps: [
			Chart.animate({
				data,
				config,
				style: style({ angle: '30deg' })
			})
		]
	},
	{
		testName: 'static_30deg_center',
		testSteps: [
			Chart.animate({
				data,
				config,
				style: style({ angle: '30deg', textAlign: 'center' })
			})
		]
	},
	{
		testName: 'static_30deg_left',
		testSteps: [
			Chart.animate({
				data,
				config,
				style: style({ angle: '30deg', textAlign: 'left' })
			})
		]
	},
	{
		testName: 'static_30deg_right',
		testSteps: [
			Chart.animate({
				data,
				config,
				style: style({ angle: '30deg', textAlign: 'right' })
			})
		]
	},
	{
		testName: 'static_min30deg',
		testSteps: [
			Chart.animate({
				data,
				config,
				style: style({ angle: '-30deg' })
			})
		]
	},
	{
		testName: 'static_min30deg_center',
		testSteps: [
			Chart.animate({
				data,
				config,
				style: style({ angle: '-30deg', textAlign: 'center' })
			})
		]
	},
	{
		testName: 'static_min30deg_left',
		testSteps: [
			Chart.animate({
				data,
				config,
				style: style({ angle: '-30deg', textAlign: 'left' })
			})
		]
	},
	{
		testName: 'static_min30deg_right',
		testSteps: [
			Chart.animate({
				data,
				config,
				style: style({ angle: '-30deg', textAlign: 'right' })
			})
		]
	}
]

export default testCases
