import { data_6 } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	async (chart) => {
		await import('https://cdn.jsdelivr.net/npm/tinycolor2@1.6.0/dist/tinycolor-min.js')
		const THREE = await import(
			'https://cdn.jsdelivr.net/npm/three@0.175.0/build/three.module.js'
		)

		const toCanvasRect = (rect) => {
			const coordSystem = chart.feature.coordSystem
			const pos = coordSystem.toCanvas({ x: rect.pos.x, y: rect.pos.y + rect.size.y })
			const pos2 = coordSystem.toCanvas({ x: rect.pos.x + rect.size.x, y: rect.pos.y })
			return { pos, size: { x: pos2.x - pos.x, y: pos2.y - pos.y } }
		}

		const renderer = new THREE.WebGLRenderer({
			antialias: true
		})
		renderer.shadowMap.enabled = true
		renderer.setClearColor(0xffffff, 1)
		let scene = new THREE.Scene()

		const width = chart.feature.htmlCanvas.element.width
		const height = chart.feature.htmlCanvas.element.height

		renderer.setSize(width, height)
		renderer.domElement.style = `
		position: absolute; 
		top: ${chart.feature.htmlCanvas.element.offsetTop}px; 
		left: ${chart.feature.htmlCanvas.element.offsetLeft}px;
		visibility: hidden;
	`
		document.body.appendChild(renderer.domElement)

		const camera = new THREE.PerspectiveCamera(40, width / height, 0.1, 1000)
		camera.position.set(0.8, 1.2, 1.5)
		camera.lookAt(new THREE.Vector3(0.5, 0.5, 0))

		const light0 = new THREE.AmbientLight(0x999999)
		const light = new THREE.PointLight(0xffffff, 5, 100)
		light.position.set(1.1, 1.1, 1.1)
		light.castShadow = true

		chart.on('draw-begin', (event) => {
			scene = new THREE.Scene()
			scene.add(light0)
			scene.add(light)

			const geometry = new THREE.BoxGeometry(1, 0.01, 0.3)
			geometry.translate(0.5, 0, -0.1)
			const material = new THREE.MeshLambertMaterial({ color: '#e0e0f0' })
			const cube = new THREE.Mesh(geometry, material)
			cube.receiveShadow = true
			scene.add(cube)
		})

		chart.on('plot-marker-draw', (event) => {
			const ctx = event.renderingContext
			const rect = toCanvasRect(event.detail.rect)
			const color = tinycolor(ctx.fillStyle)
			const opacity = color.getAlpha()
			color.setAlpha(1)
			const geometry = new THREE.BoxGeometry(rect.size.x / width, rect.size.y / height, 0.1)
			geometry.translate(
				(rect.pos.x + rect.size.x / 2) / width,
				1 - (rect.size.y / 2 + rect.pos.y) / height,
				-0.05
			)
			const material = new THREE.MeshPhongMaterial({
				color: color.toRgbString()
			})
			material.transparent = true
			material.opacity = opacity
			const cube = new THREE.Mesh(geometry, material)
			cube.castShadow = true
			scene.add(cube)
		})

		chart.on('draw-complete', (event) => {
			renderer.render(scene, camera)
			event.renderingContext.drawImage(renderer.domElement, 0, 0)
		})

		return chart.animate(
			{
				data: data_6,
				config: {
					x: 'Joy factors',
					y: 'Value 2 (+)',
					color: 'Joy factors',
					legend: null,
					title: null
				},
				style: {
					plot: {
						paddingTop: 0,
						paddingLeft: 0,
						paddingBottom: 0,
						paddingRight: 0
					}
				}
			},
			0
		)
	},

	(chart) =>
		chart.animate({
			x: 'Country',
			color: 'Country'
		})
]

export default testSteps
