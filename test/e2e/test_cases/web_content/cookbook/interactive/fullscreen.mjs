import { data_6 } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) => {
		const btnStyle = document.createElement('style')
		btnStyle.innerHTML = `
			<style>
			.vizzu-button > svg {
				vertical-align: middle;
			}
			.vizzu-button > svg path {
				fill: #c6c6c6;
			}
			.vizzu-button:hover > svg path {
				fill: #494949;
			}
			</style>
		`
		chart.feature.htmlCanvas.element.parentElement.appendChild(btnStyle)

		const button = document.createElement('button')
		button.style.background = 'transparent'
		button.style.padding = 0
		button.style.margin = 0
		button.style.border = 'none'
		button.style.cursor = 'pointer'
		button.style.minWidth = '25px'
		button.style.height = '22px'
		button.style.position = 'absolute'
		button.style.top = '30px'
		button.style.left = chart.feature.htmlCanvas.element.width - 30 + 'px'

		button.classList.add('vizzu-button')
		button.innerHTML = `
		<svg version="1.1" xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" width="15" height="15" viewBox="0 0 15 15">
			<path id="fullscreen_pass" d="M9.000,12.000 L12.000,12.000 L12.000,9.000 L15.000,9.000 L15.000,15.000 L12.000,15.000 L9.000,15.000 L9.000,12.000 zM9.000,-0.000 L15.000,-0.000 L15.000,6.000 L12.000,6.000 L12.000,3.000 L9.000,3.000 L9.000,-0.000 zM3.000,9.000 L3.000,12.000 L6.000,12.000 L6.000,15.000 L-0.000,15.000 L-0.000,9.000 L3.000,9.000 zM-0.000,-0.000 L6.000,-0.000 L6.000,3.000 L3.000,3.000 L3.000,6.000 L-0.000,6.000 L-0.000,-0.000 z" fill="#A2A2A2" />
		</svg>
		`
		chart.feature.htmlCanvas.element.parentElement.appendChild(button)

		button.addEventListener('click', (event) => {
			if (document.fullscreenElement) document.exitFullscreen()
			else {
				const controlledElement = chart.feature.htmlCanvas.element
				const requestMethod =
					controlledElement.requestFullScreen ||
					controlledElement.webkitRequestFullScreen ||
					controlledElement.mozRequestFullScreen ||
					controlledElement.msRequestFullScreen

				try {
					if (requestMethod) requestMethod.call(controlledElement)
				} catch (error) {
					alert(
						`Error attempting to enable full-screen mode: ${error.message} (${error.name})`
					)
				}
			}
		})

		return chart.animate({
			data: data_6,
			config: {
				x: 'Joy factors',
				y: 'Value 2 (+)',
				color: 'Joy factors',
				label: 'Value 2 (+)'
			}
		})
	}
]

export default testSteps
