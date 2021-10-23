import UnPivot from './unpivot.js';

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

		let copy;

		if (UnPivot.isPivot(obj)) {
			if (this.is1NF(obj)) throw new Error
				(  'inconsistent data form: '
				 + 'series/records and dimensions/measures are both set.');
			else copy = UnPivot.convert(obj);
		}
		else copy = obj;

		if (copy.series !== undefined)
		{
			if (copy.series === null || !Array.isArray(copy.series))
				throw new Error('data series field is not an array');

			for (const series of copy.series) this.setSeries(series);
		}

		if (copy.records !== undefined)
		{
			if (copy.records === null || !Array.isArray(copy.records))
				throw new Error('data records field is not an array');

			for (const record of copy.records) this.addRecord(record);
		}

		if (copy.filter !== undefined)
		{
			this.setFilter(copy.filter);
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

		let copy = { 
			name: series.name,
			values: series.values === undefined ? [] : series.values, 
			type: series.type === undefined 
				? this.detectType(series.values) 
				: series.type
		};

		if(copy.type === 'dimension')
			this.addDimension(copy.name, copy.values);

		else if (copy.type === 'measure')
			this.addMeasure(copy.name, copy.values);

		else throw new Error('invalid series type: ' + copy.type);
	}

	detectType(values)
	{
		if ( values instanceof Array && values.length >= 1 )
		{
			if (typeof values[0] === 'number') return 'measure';
			else if (typeof values[0] === 'string' 
				|| values[0] instanceof String)
				return 'dimension';
			else return undefined;
		}
		else return undefined;
	}

	addDimension(name, dimension)
	{
		if (typeof name !== 'string' && ! (name instanceof String))
			throw new Error('first parameter should be string');

		if ( !(dimension instanceof Array))
			throw new Error('second parameter should be an array');

		let ptrs = new Uint32Array(dimension.length);
		for (let i = 0; i < dimension.length; i++)
		{
			if (typeof dimension[i] !== 'string'
				&& ! (dimension[i] instanceof String))
				throw new Error('array element should be string');

			let ptr = this.chart.toCString(dimension[i]);
			ptrs[i] = ptr;
		}
		
		let ptrArrayLen = dimension.length * 4;

		let ptrArr = this.chart.module._malloc(ptrArrayLen);
		var ptrHeap = new Uint8Array(
			this.chart.module.HEAPU8.buffer, ptrArr, ptrArrayLen);
		ptrHeap.set(new Uint8Array(ptrs.buffer));

		let cname = this.chart.toCString(name);

		try
		{
			this.chart.call(this.chart.module._data_addDimension)
				(cname, ptrArr, dimension.length);
		}
		finally
		{
			this.chart.module._free(cname);
			for (let ptr of ptrs) this.chart.module._free(ptr);
			this.chart.module._free(ptrArr);
		}
	}

	addMeasure(name, values)
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
			this.chart.call(this.chart.module._data_addMeasure)
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

	is1NF(data) 
	{
		return data.series !== undefined || data.records !== undefined;
	}
}
