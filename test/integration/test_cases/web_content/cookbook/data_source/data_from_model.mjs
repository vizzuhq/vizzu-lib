const testSteps = [
	async chart => 
	{
		class Vector {
			constructor(x, y) {
				this.x = x;
				this.y = y;
			}

			static Random() {
				return new Vector(Math.random(), Math.random());
			}

			mul(multiplier) {
				return new Vector(
					this.x * (multiplier?.x || multiplier), 
					this.y * (multiplier?.y || multiplier));
			}

			add(other) {
				return new Vector(this.x + other.x, this.y + other.y);
			}
		}

		class BouncingBall {
			static lastIndex = 0;

			constructor(massToSize) {
				this.index = BouncingBall.lastIndex++;
				this.position = Vector.Random();
				this.speed = (Vector.Random()).mul(new Vector(3, 5));
				this.mass = Math.random();
				this.radius = Math.sqrt(this.mass)*massToSize;
			}

			update(dt) {
				const g = 9.81;
				const friction = 0.5;
				let acceleration = this.speed.mul(-friction * this.mass).add(new Vector(0, -g));
				this.speed = this.speed.add(acceleration.mul(dt));
				this.position = this.position.add(this.speed.mul(dt));
				this.collision('y', v => v);
				this.collision('x', v => v);
				this.collision('x', v => 1 - v);
			}

			collision(coordinate, conversion) {
				const collisionDumping = 0.6;
				let side = conversion(this.position[coordinate]) - this.radius;
				if (side < 0) { 
					this.position[coordinate] = conversion(- side + this.radius);
					this.speed[coordinate] *= -collisionDumping;
				}
			}
		}

		class Model {
			constructor(massToSize, ballCount = 150) {
				this.t = 0;
				this.balls = [];
				for (let i = 0; i < ballCount; i++)
					this.balls.push(new BouncingBall(massToSize));
			}

			update(dt) 
			{
				for (let ball of this.balls) ball.update(dt);
				this.t += dt;
			}
		}

		let circleMaxRadius = chart.getComputedStyle().plot.marker.circleMaxRadius;
		let model = new Model(circleMaxRadius)

		function update(chart) 
		{
			const dt = 0.01;
			model.update(dt);
			if (model.t < 3)
				return chart.animate({ 
					data: {
						series: [
							{ name: 'index', type: 'dimension', values: model.balls.map(ball => `${ball.index}`) },
							{ name: 'x', type: 'measure', values: model.balls.map(ball => ball.position.x) },
							{ name: 'y', type: 'measure', values: model.balls.map(ball => ball.position.y) },
							{ name: 'size', type: 'measure', values: model.balls.map(ball => ball.mass) }
						]
					},
					config: {
						title: 'Bouncing balls realtime simulation',
						x: { set: 'x', range: { min: 0, max: 1} },
						y: { set: 'y', range: { min: 0, max: 1} },
						color: 'index',
						size: 'size',
						geometry: 'circle',
						legend: null
					},
					style: { 
						'plot.marker.fillOpacity': 1
					}
				}, dt)
				.then(chart => update(chart));
		}

		return update(chart);
	}
];

export default testSteps;