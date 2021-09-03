export default class Tooltip
{
	constructor(chart) {
		this.chart = chart;
		this.id = 0;
		this.lastMarkerId = null;
		this.lastMove = new Date();
		this.mouseMoveHandler = event => { this.mousemove(event); };
		this.mouseOnHandler = event => { this.mouseon(event); }
	}

	enable(enabled) 
	{
		if (enabled) {
			this.chart.on('mousemove', this.mouseMoveHandler);
			this.chart.on("mouseon", this.mouseOnHandler);	
		}
		else
		{
			this.chart.off('mousemove', this.mouseMoveHandler);
			this.chart.off("mouseon", this.mouseOnHandler);
			this.id++;
			setTimeout(() => { this.out(id); }, 200);
		}
	}

	mousemove() {
		this.lastMove = new Date();
	}

	mouseon(param) {
		this.id++;
		let id = this.id;
		if (param.data.marker === undefined)
		{
			setTimeout(() => { this.out(id); }, 200);
		}
		else
		{
			setTimeout(() => {
				if (this.id == id) {
					this.lastMarkerId = param.data.marker.id;
					//todo: check if animation is running
					this.chart.animate( { config : { tooltip: param.data.marker.id }}, 
						this.lastMarkerId !== undefined ? '100ms' : '250ms' );
				}
			}, 0);
		}
	}

	out(id) {
		if (this.id == id) {
			let ellapsed = new Date() - this.lastMove; 
			if (ellapsed > 200) {
				this.lastMarkerId = null;
				this.chart.animate( { config : { tooltip: null }}, '250ms' );	
			}
			else setTimeout(() => { this.out(id) }, 200 - ellapsed);
		}
	}
}
