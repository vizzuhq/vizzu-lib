import { data } from '../../../../test_data/chart_types_eu.mjs'
import VideoCapture from '../../../../utils/vizzu-videocapture.mjs'
import PPTXGen from '../../../../utils/vizzu-pptxgen.mjs'

const testSteps = [
	async (chart) => {
		window.PptxGenJS = (
			await import('https://cdn.jsdelivr.net/npm/pptxgenjs@3.12.0/+esm')
		).default
		return chart
	},

	async (chart) => {
		chart.feature(new VideoCapture(/* PPTXGen.videoCaptureOptions // fails on test.cjs */))
		chart.feature(new PPTXGen())

		await chart.animate({
			data,
			config: {
				x: 'Year',
				y: ['Value 2 (+)', 'Joy factors'],
				color: 'Joy factors',
				title: 'Slide 1'
			}
		})

		await chart.animate({
			data,
			config: {
				x: 'Year',
				y: 'Joy factors',
				color: 'Value 2 (+)',
				title: 'Slide 2'
			}
		})

		await chart.feature.pptxGen.save('Sample-Vizzu-Presentation.pptx')

		return chart
	}
]

export default testSteps
