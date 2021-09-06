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
	}

	set(obj)
	{
		if (obj === null || obj === undefined) return;

		if (obj.series !== undefined)
		{
			if (obj.series === null || !Array.isArray(obj.series))
				throw new Error('data series field is not an array');

			for (const series of obj.series) this.setSeries(series);
		}

		if (obj.records !== undefined)
		{
			if (obj.records === null || !Array.isArray(obj.records))
				throw new Error('data records field is not an array');

			for (const record of obj.records) this.addRecord(record);
		}

		if (obj.filter !== undefined)
		{
			this.setFilter(obj.filter);
		}
	}

	addRecord(record)
	{
		if (record === null || !Array.isArray(record))
			throw new Error('data record is not an array');

		let ptrs = new Uint32Array(record.length);
		for (let i = 0; i < record.length; i++)
		{
			let ptr = this.chart.toCString(String(record[i]).toString());
			ptrs[i] = ptr;
		}

		let ptrArrayLen = record.length * 4;

		let ptrArr = this.chart.module._malloc(ptrArrayLen);
		var ptrHeap = new Uint8Array(
			this.chart.module.HEAPU8.buffer, ptrArr, ptrArrayLen);
		ptrHeap.set(new Uint8Array(ptrs.buffer));

		try
		{
			this.chart.call(this.chart.module._data_addRecord)
				(ptrArr, record.length);
		}
		finally
		{
			for (let ptr of ptrs) this.chart.module._free(ptr);
			this.chart.module._free(ptrArr);
		}
	}

	setSeries(series)
	{
		if (series.name === undefined)
			throw new Error('missing series name');

		if (series.values === undefined)
			throw new Error('missing series values');

		if(series.type === 'categories')
			this.addCategories(series.name, series.values);

		else if (series.type === 'values')
			this.addValues(series.name, series.values);

		else throw new Error('invalid series type: ' + series.type);
	}

	addCategories(name, categories)
	{
		if (typeof name !== 'string' && ! (name instanceof String))
			throw new Error('first parameter should be string');

		if ( !(categories instanceof Array))
			throw new Error('second parameter should be an array');

		let ptrs = new Uint32Array(categories.length);
		for (let i = 0; i < categories.length; i++)
		{
			if (typeof categories[i] !== 'string'
				&& ! (categories[i] instanceof String))
				throw new Error('array element should be string');

			let ptr = this.chart.toCString(categories[i]);
			ptrs[i] = ptr;
		}

		let ptrArrayLen = categories.length * 4;

		let ptrArr = this.chart.module._malloc(ptrArrayLen);
		var ptrHeap = new Uint8Array(
			this.chart.module.HEAPU8.buffer, ptrArr, ptrArrayLen);
		ptrHeap.set(new Uint8Array(ptrs.buffer));

		let cname = this.chart.toCString(name);

		try
		{
			this.chart.call(this.chart.module._data_addCategories)
				(cname, ptrArr, categories.length);
		}
		finally
		{
			this.chart.module._free(cname);
			for (let ptr of ptrs) this.chart.module._free(ptr);
			this.chart.module._free(ptrArr);
		}
	}

	addValues(name, values)
	{
		if (typeof name !== 'string' && ! (name instanceof String))
			throw new Error('first parameter should be string');

		if ( !(values instanceof Array))
			throw new Error('second parameter should be an array');

		let vals = new Float64Array(values);
		let valArrayLen = values.length * 8;

		let valArr = this.chart.module._malloc(valArrayLen);
		var valHeap = new Uint8Array(
			this.chart.module.HEAPU8.buffer, valArr, valArrayLen);

		valHeap.set(new Uint8Array(vals.buffer));

		let cname = this.chart.toCString(name);

		try
		{
			this.chart.call(this.chart.module._data_addValues)
				(cname, valArr, values.length);
		}
		finally
		{
			this.chart.module._free(cname);
			this.chart.module._free(valArr);
		}
	}

	setFilter(filter)
	{
		if (typeof filter === 'function')
		{
			let callback = ptr => filter(new DataRecord(this.chart, ptr));
			let callbackPtr = this.chart.module.addFunction(callback, 'ii');
			this.chart.call(this.chart.module._chart_setFilter)(callbackPtr);
		}
		else if (filter === null)
		{
			this.chart.call(this.chart.module._chart_setFilter)(0);
		}
		else throw new Error('data filter is not a function or null');
	}
}
