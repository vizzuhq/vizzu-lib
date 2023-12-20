const normalRecord = { Dim0: 'a', Dim1: 'A', Meas: 1 }
const missingMeasureRecord = { Dim0: 'b', Dim1: 'B' }
const missingDimensionRecord = { Dim0: 'c', Meas: 3 }
const extraDimensionRecord = { Dim0: 'd', Dim1: 'D', Meas: 4, Dim2: 'X' }
const outOfOrderRecord1 = { Dim1: 'E', Dim0: 'e', Meas: 5 }
const outOfOrderRecord2 = { Dim0: 'f', Meas: 6, Dim1: 'F' }

const data = {
	series: [
		{
			name: 'Dim0',
			type: 'dimension'
		},
		{
			name: 'Dim1',
			type: 'dimension'
		},
		{
			name: 'Meas',
			type: 'measure'
		}
	],
	records: [
		normalRecord,
		missingMeasureRecord,
		missingDimensionRecord,
		extraDimensionRecord,
		outOfOrderRecord1,
		outOfOrderRecord2
	]
}

const addedMissingMeasureRecord = { Dim0: 'b', Dim1: 'B', Meas: 2 }
const addedOutOfOrderRecord1 = { Dim1: 'G', Meas: 7, Dim0: 'g' }

const testSteps = [
	(chart) => chart.animate({ data }),
	(chart) =>
		chart.animate({
			x: ['Dim0', 'Dim1'],
			y: 'Meas',
			label: ['Meas', 'Dim1']
		}),
	(chart) => chart.animate({ data: { records: [addedMissingMeasureRecord] } }),
	(chart) => chart.animate({ data: { records: [addedOutOfOrderRecord1] } })
]

export default testSteps
