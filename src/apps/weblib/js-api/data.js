class DataRecord
{
	constructor(chart, record) {
		this.chart = chart;
		this.record = record;

		return new Proxy(this, {
			get: (target, columnName) => {
				return target.getValue(columnName);
			}
		});
	}

	getValue(columnName)
	{
		let col = this.chart.toCString(columnName);
		let ptr;
		let value;

		try {
			ptr = this.chart.call(this.chart.module._record_getValue)
				(this.record, col, true);

			if (ptr) {
				value = this.chart.fromCString(ptr);
			}
			else {
				ptr = this.chart.call(this.chart.module._record_getValue)
					(this.record, col, false);
				value = this.chart.module.getValue(ptr, 'double');
			}
		} finally
		{
			this.chart.module._free(col);
		}
		return value;
	}
};

export default class Data
{
	constructor(chart)
	{
		this.chart = chart;
		this.filter = null;
	}

	setFilter(filter)
	{
		if (this.filter !== null)
		{
			this.chart.module.removeFunction(this.filter);
			this.filter = null;
		}

		if (typeof filter === 'function')
		{
			let callback = ptr => filter(new DataRecord(this.chart, ptr));
			this.filter = this.chart.module.addFunction(callback, 'ii');
			// remove function
			this.chart.call(this.chart.module._chart_setFilter)(this.filter);
		}
		else if (filter === null)
		{
			this.chart.call(this.chart.module._chart_setFilter)(0);
		}
		else throw new Error('data filter is not a function or null');
	}
}
