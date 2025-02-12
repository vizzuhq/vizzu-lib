import { type Canvas } from './module/canvas'
import { type Chart } from './module/chart'

export type CPointer = number
export type CString = CPointer
export type CException = CPointer
export type CTypeInfo = CPointer
export type CSnapshotPtr = CPointer
export type CAnimationPtr = CPointer
export type CDataPtr = CPointer
export type CChartPtr = CPointer
export type CCanvasPtr = CPointer
export type CEventPtr = CPointer
export type CFunction = CPointer
export type CRecordPtr = CPointer
export type CRecordValue = CPointer
export type CPointPtr = CPointer
export type CArrayPtr = CPointer
export type CColorGradientPtr = CPointer

export type PtrType =
	| 'i1'
	| 'i8'
	| 'i16'
	| 'i32'
	| 'i64'
	| 'float'
	| 'double'
	| 'i1*'
	| 'i8*'
	| 'i16*'
	| 'i32*'
	| 'i64*'
	| 'float*'
	| 'double*'

export interface CExceptionInfo {
	get_type(): CTypeInfo
}
export interface CExceptionInfoConstructor {
	new (ptr: CPointer): CExceptionInfo
}

export interface ModuleOptions {
	locateFile?: (path: string) => string
}

export interface CVizzu {
	// decorations
	canvases: { [key: CPointer]: Canvas }
	charts: { [key: CPointer]: Chart }
	measureCanvas: CanvasRenderingContext2D

	// members
	HEAPU8: Uint8Array

	// exported runtime functions
	_malloc(len: number): CPointer
	_free(cpath: CPointer): void
	getValue(ptr: CPointer, type: PtrType): number
	setValue(ptr: CPointer, value: number, type: PtrType): void
	stringToUTF8(str: string, buffer: CPointer, len: number): void
	UTF8ToString(ptr: CString): string
	addFunction<T>(func: T, sig: string): CFunction
	removeFunction(cfunc: CFunction): void
	ExceptionInfo: CExceptionInfoConstructor

	// exported functions
	_vizzu_createData(): CDataPtr
	_vizzu_createExternalData(
		stringDeleter : CFunction,
		seriesMeta : CFunction,
		seriesInfo : CFunction,
		aggregator : CFunction,
		deleter : CFunction
	): CDataPtr
	_vizzu_createChart(data : CDataPtr): CChartPtr
	_vizzu_createCanvas(): CCanvasPtr
	_vizzu_pointerDown(
		chart: CChartPtr,
		canvas: CCanvasPtr,
		pointerId: number,
		x: number,
		y: number
	): void
	_vizzu_pointerUp(
		chart: CChartPtr,
		canvas: CCanvasPtr,
		pointerId: number,
		x: number,
		y: number
	): void
	_vizzu_pointerMove(
		chart: CChartPtr,
		canvas: CCanvasPtr,
		pointerId: number,
		x: number,
		y: number
	): void
	_vizzu_pointerLeave(chart: CChartPtr, canvas: CCanvasPtr, pointerId: number): void
	_vizzu_wheel(chart: CChartPtr, canvas: CCanvasPtr, delta: number): void
	_vizzu_setLogging(enable: boolean): void
	_vizzu_update(chart: CChartPtr, time: number): void
	_vizzu_setLineResolution(canvas: CCanvasPtr, distanceMax: number, curveHeightMax: number): void
	_vizzu_render(chart: CChartPtr, canvas: CCanvasPtr, width: number, height: number): void

	_vizzu_errorMessage(exceptionPtr: CException, typeinfo: CTypeInfo): CString
	_vizzu_version(): CString
	_data_addDimension(
		chart: CChartPtr,
		name: CString,
		categories: CArrayPtr,
		categoriesCount: number,
		categoryIndices: CArrayPtr,
		categoryIndicesCount: number,
		isContiguous: boolean
	): void
	_data_addMeasure(
		chart: CChartPtr,
		name: CString,
		unit: CString,
		values: CArrayPtr,
		count: number
	): void
	_data_addRecord(chart: CChartPtr, cells: CArrayPtr, count: number): void
	_data_metaInfo(chart: CChartPtr): CString
	_record_getValue(record: CRecordPtr, column: CString): CRecordValue
	_chart_store(chart: CChartPtr): CSnapshotPtr
	_chart_restore(chart: CChartPtr, snapshot: CSnapshotPtr): void
	_chart_anim_store(chart: CChartPtr): CAnimationPtr
	_chart_anim_restore(chart: CChartPtr, anim: CAnimationPtr): void
	_object_free(handle: CPointer): void
	_style_getList(): CString
	_style_setValue(chart: CChartPtr, path: CString, value: CString): void
	_style_getValue(chart: CChartPtr, path: CString, computed: boolean): CString
	_chart_getList(): CString
	_chart_getValue(chart: CChartPtr, path: CString): CString
	_chart_setValue(chart: CChartPtr, path: CString, value: CString): void
	_chart_setFilter(chart: CChartPtr, filter: CFunction, deleter: CFunction): void
	_chart_animate(chart: CChartPtr, callback: CFunction): void
	_chart_relToCanvasCoords(chart: CChartPtr, rx: number, ry: number): CPointPtr
	_chart_canvasToRelCoords(chart: CChartPtr, x: number, y: number): CPointPtr
	_chart_setKeyframe(chart: CChartPtr): void
	_addEventListener(chart: CChartPtr, name: CString, callback: CFunction): void
	_removeEventListener(chart: CChartPtr, name: CString, callback: CFunction): void
	_event_preventDefault(event: CEventPtr): void
	_anim_control_setValue(chart: CChartPtr, command: CString, param: CString): void
	_anim_control_getValue(chart: CChartPtr, command: CString): CString
	_anim_setValue(chart: CChartPtr, path: CString, value: CString): void
}

export declare const Module: CVizzu
