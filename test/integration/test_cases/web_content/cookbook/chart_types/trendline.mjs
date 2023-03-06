import { data } from '../../../../test_data/chart_types_eu.mjs';

const testSteps = [
	chart => {
		class LinearRegression {
			constructor()
			{
				this.reset();
			}

			reset() {
				this.xSum = 0;
				this.ySum = 0;
				this.xxSum = 0;
				this.xySum = 0;
				this.count = 0;
				this.slope = null;
				this.intercept = null;
			}

			add(point) {
				this.xSum += point.x;
				this.ySum += point.y;
				this.xxSum += point.x * point.x;
				this.xySum += point.x * point.y;
				this.count++;
			}

			calc() {
				this.slope = (this.count * this.xySum - this.xSum * this.ySum) 
					/ (this.count * this.xxSum - this.xSum * this.xSum);
				this.intercept = (this.ySum / this.count) - (this.slope * this.xSum) 
					/ this.count;
			}

			at(x) { 
				if (this.slope === null) this.calc();
				return x * this.slope + this.intercept; 
			}
		}

		let regression = new LinearRegression();

		function rectCenter(rect) {
			return {
				x: rect.pos.x + rect.size.x / 2,
				y: rect.pos.y + rect.size.y / 2
			}
		}

		chart.on('plot-background-draw', event => {
			regression.reset();
		});

		chart.on('plot-marker-draw', event => {
			regression.add(rectCenter(event.data.rect));
		});

		chart.on('logo-draw', event => {
			let ctx = event.renderingContext;
			ctx.strokeStyle = '#a0b0f0';
			ctx.save();
			ctx.beginPath();
			ctx.rect(100,100,500,300);
			ctx.clip();
			ctx.beginPath();
			ctx.lineWidth = 2;
			ctx.moveTo(0, regression.at(0));
			ctx.lineTo(1000, regression.at(1000));
			ctx.stroke();
			ctx.restore();
		})

		return chart.animate({
			data: data,
			config:
			{
				channels:
				{
					x: 'Value 1 (+)',
					y: 'Value 3 (+)',
					noop: 'Year',
					label: 'Year'
				},
				title: 'Scatterplot with (-) Nums',
				geometry: 'circle',
				coordSystem: 'cartesian',
				orientation: 'horizontal'
			}
		})
	}
];

export default testSteps;