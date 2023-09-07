import { data } from '../../../../test_data/chart_types_eu.mjs';

const testSteps = [
	chart => {

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

		class Window {
			constructor(chart, xmin, xmax, ymin, ymax) {
				this.chart = chart;

				this.chart.on('pointerdown', event => { this.down(event.detail); });
				this.chart.on('pointerup', event => { this.up(event.detail); });
				this.chart.on('pointermove', event => { this.move(event.detail); });
				this.chart.on('draw-complete', event => { this.render(event.renderingContext); });

				this.x = { min: xmin, max: xmax };
				this.y = { min: ymin, max: ymax };
				this.xInit = { ...this.x };
				this.yInit = { ...this.y };
				this.throttle = new Throttle();

				this.reset();
			}

			zoomOut() {
				this.x = { ...this.xInit };
				this.y = { ...this.yInit };
				this.update();
				this.reset();
			}

			reset() {
				this.data0 = null;
				this.data1 = null;
			}

			isActive() { return this.data1 !== null; }
			
			down(data) { this.data0 = data; }
			move(data) { if (this.data0 !== null) this.data1 = data; }
			
			up() {
				if (!this.isActive()) this.zoomOut();
				else this.applyZoom();
				this.reset();
			}

			posAt(range, factor) {
				return range.min + factor * (range.max - range.min);
			}

			applyZoomOnAxis(range, p0, p1)
			{
				let r = { ... range };
				range.min = this.posAt(r, Math.min(p0, p1));
				range.max = this.posAt(r, Math.max(p0, p1));
			}

			applyZoom() {
				let convert = this.chart.getConverter("plot-area", "canvas", "relative");
				let coords0 = convert(this.data0.position);
				let coords1 = convert(this.data1.position);
				this.applyZoomOnAxis(this.x, coords0.x, coords1.x);
				this.applyZoomOnAxis(this.y, coords0.y, coords1.y);
				this.update();
			}

			update() {
				this.throttle.call(() => this.chart.animate(
					{ x: { range: this.x }, y: { range: this.y } },
					{ duration: '200ms', easing: 'linear' })
				);
			}

			render(ctx) {
				if (this.isActive())
				{
					ctx.lineWidth = 1;
					ctx.strokeStyle = "#888888";
					ctx.beginPath();
					ctx.rect(this.data0.position.x, this.data0.position.y, 
						this.data1.position.x - this.data0.position.x, 
						this.data1.position.y - this.data0.position.y);
					ctx.stroke();
				}	
			}
		};

		let win = new Window(chart, 0.5, 20.5, -500, 500);

		return chart.animate({
			data: data,
			config: {
				x: { set: 'Year', range: win.x },
				y: { set: 'Value 5 (+/-)', range: win.y },
				title: 'Window Zoom',
				geometry: 'line'
			}
		}, 0)
	},
	chart => 
	{
		chart.module._vizzu_pointerDown(0, 100, 100);
		chart.module._vizzu_pointerMove(0, 300, 250);
		chart.module._vizzu_pointerUp(0, 300, 250);
		return chart.anim;
	}
];

export default testSteps;