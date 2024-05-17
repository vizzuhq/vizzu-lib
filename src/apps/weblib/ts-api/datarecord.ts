import * as Data from './types/data.js'
import { CRecord } from './module/cdata.js'

export class DataRecord implements Data.Record {
	[seriesName: Data.SeriesName]: Data.Value

	constructor(cRecord: CRecord) {
		return new Proxy(this, {
			get: (_target, columnName: string): Data.Value => {
				return cRecord.getValue(columnName)
			}
		})
	}
}
