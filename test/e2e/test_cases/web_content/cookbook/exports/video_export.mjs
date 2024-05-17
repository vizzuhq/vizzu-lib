import { data } from '../../../../test_data/chart_types_eu.mjs'
import VideoCapture from '../../../../utils/vizzu-videocapture.mjs'

const testSteps = [
	(chart) => {
		chart.feature(new VideoCapture())

		const anim = chart.animate({
			data,
			config: {
				x: 'Year',
				y: ['Value 2 (+)', 'Joy factors'],
				color: 'Joy factors',
				title: 'Video Export'
			}
		})

		anim.activated.then(() => {
			chart.feature.videoCapture.start()
		})

		return anim.then(async (chart) => {
			const output = await chart.feature.videoCapture.stop()
			window.open(output.getObjectURL())
		})
	}
]

export default testSteps
