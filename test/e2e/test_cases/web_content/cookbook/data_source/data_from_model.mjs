const testSteps = [
	async (chart) => {
		function Rand(a) {
			return function () {
				let t = (a += 0x6d2b79f5)
				t = Math.imul(t ^ (t >>> 15), t | 1)
				t ^= t + Math.imul(t ^ (t >>> 7), t | 61)
				return ((t ^ (t >>> 14)) >>> 0) / 4294967296
			}
		}

		const rand = Rand(32)

		class Vector {
			constructor(x, y) {
				this.x = x
				this.y = y
			}

			static Random() {
				return new Vector(rand(), rand())
			}

			mul(multiplier) {
				return new Vector(
					this.x * (multiplier?.x || multiplier),
					this.y * (multiplier?.y || multiplier)
				)
			}

			add(other) {
				return new Vector(this.x + other.x, this.y + other.y)
			}
		}

		class BouncingBall {
			static lastIndex = 0

			constructor(massToSize) {
				this.index = BouncingBall.lastIndex++
				this.position = Vector.Random()
				this.speed = Vector.Random().mul(new Vector(3, 5))
				this.mass = rand()
				this.radius = Math.sqrt(this.mass) * massToSize
			}

			update(timeStep) {
				const g = 9.81
				const friction = 0.5
				const acceleration = this.speed.mul(-friction * this.mass).add(new Vector(0, -g))
				this.speed = this.speed.add(acceleration.mul(timeStep))
				this.position = this.position.add(this.speed.mul(timeStep))
				this.collision('y', (v) => v)
				this.collision('x', (v) => v)
				this.collision('x', (v) => 1 - v)
			}

			collision(coordinate, conversion) {
				const collisionDumping = 0.6
				const side = conversion(this.position[coordinate]) - this.radius
				if (side < 0) {
					this.position[coordinate] = conversion(-side + this.radius)
					this.speed[coordinate] *= -collisionDumping
				}
			}
		}

		class Model {
			constructor(massToSize, ballCount = 150) {
				this.time = 0
				this.balls = []
				for (let i = 0; i < ballCount; i++) this.balls.push(new BouncingBall(massToSize))
			}

			update(timeStep) {
				for (const ball of this.balls) ball.update(timeStep)
				this.time += timeStep
			}
		}

		function getDataFromModel(model) {
			return {
				series: [
					{
						name: 'index',
						type: 'dimension',
						values: model.balls.map((ball) => `${ball.index}`)
					},
					{
						name: 'x',
						type: 'measure',
						values: model.balls.map((ball) => ball.position.x)
					},
					{
						name: 'y',
						type: 'measure',
						values: model.balls.map((ball) => ball.position.y)
					},
					{
						name: 'size',
						type: 'measure',
						values: model.balls.map((ball) => ball.mass)
					}
				]
			}
		}

		function getChartConfig() {
			return {
				title: 'Bouncing balls realtime simulation',
				x: { set: 'x', range: { min: 0, max: 1 } },
				y: { set: 'y', range: { min: 0, max: 1 } },
				color: 'index',
				size: 'size',
				geometry: 'circle',
				legend: null
			}
		}

		function update(model, chart) {
			const timeStep = 0.01
			model.update(timeStep)
			if (model.time < 1)
				return chart
					.animate(
						{
							data: getDataFromModel(model),
							config: getChartConfig()
						},
						timeStep
					)
					.then((chart) => update(model, chart))
		}

		const circleMaxRadius = chart.getComputedStyle().plot.marker.circleMaxRadius
		const model = new Model(circleMaxRadius)

		return update(model, chart)
	}
]

export default testSteps
