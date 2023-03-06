import { data } from '../../../../test_data/chart_types_eu.mjs';

const testSteps = [
	chart => {

		class Scroller {
			constructor(min, max) {
				this.min = min;
				this.max = max;
				this.window = max - min;
				this.pos = null;
			}

			drag(pos) {
				this.pos = pos;
			}

			release() {
				this.pos = null;
			}

			track(pos) {
				if (this.pos)
				{
					let delta = (this.pos - pos) * this.window;
					this.min += delta;
					this.max += delta;
					this.pos = pos;
				}
			}
		};

		let scroller = new Scroller(0.5, 5.5);

		class Throttle {
			constructor() {
				this.finished = true;
				this.next = null;
			}
			call(func) {
				if (!this.finished) {
					this.next = func;
					return;
				}
				else {
					this.finished = false;
					func().then(() => {
						this.finished = true;
						if (this.next !== null) {
							let f = this.next;
							this.next = null;
							this.call(f);
						}
					})
				}
			}
		}

		let throttle = new Throttle();

		chart.on("click", (event) => { event.preventDefault(); });

		chart.on('mousedown', event => {
			scroller.drag(event.data.coords.x);
			event.preventDefault();
		});

		chart.on('mouseup', event => {
			scroller.release();
			event.preventDefault();
		});

		chart.on('mousemove', event => {
			scroller.track(event.data.coords.x);
			throttle.call(() => 
				chart.animate(
					{ x: { range: { 
						min: scroller.min, 
						max: scroller.max 
					} } },
					{ duration: '50ms', easing: 'linear' })
			);
			event.preventDefault();
		});

		return chart.animate({
			data: data,
			config: {
				x: {
					set: 'Year',
					range: {
						min: scroller.min,
						max: scroller.max
					}
				},
				y: 'Value 5 (+/-)',
				title: 'Mouse Scroll',
				geometry: 'line'
			}
		}, 0)
	}
];

export default testSteps;