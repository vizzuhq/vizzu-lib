type CPointer = number
type CString = CPointer
type CException = CPointer
type CTypeInfo = CPointer
type CSnapshotPtr = CPointer
type CAnimationPtr = CPointer
type CChartPtr = CPointer
type CCanvasPtr = CPointer
type CEventPtr = CPointer
type CFunction<T> = CPointer
type CRecordPtr = CPointer
type CRecordValue = CPointer
type CPointPtr = CPointer
type CArrayPtr = CPointer

type PtrType =
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

interface CExceptionInfo {
  get_type(): CTypeInfo
}
interface CExceptionInfoConstructor {
  new(ptr: CPointer): CExceptionInfo
}

interface ModuleOptions {
  locateFile?: (path: string) => string
}

interface Renderer {
  canvas(): HTMLCanvasElement
  dc(): CanvasRenderingContext2D
  frameBegin()
  frameEnd()
  lineWidthNotification(width: number)
  noneZeroLineWidth(): boolean
  startPolygonNotification(): boolean
  endPolygonNotification()
}

interface CVizzu {
  // decorations
  callback: (task: CFunction<(CPointer) => void>, obj: CPointer) => void
  renders: { [key: CPointer]: Renderer }

  // members
  HEAPU8: Uint8Array

  // exported runtime functions
  _malloc(len: number): CPointer
  _free(cpath: CPointer): void
  getValue(ptr: CPointer, type: PtrType): number
  setValue(ptr: CPointer, value: number, type: PtrType): void
  stringToUTF8(str: string, buffer: CPointer, len: number): void
  UTF8ToString(ptr: CString): string
  addFunction<T>(func: T, sig: string): CFunction<T>
  removeFunction(cfunc: CFunction<any>): void
  ExceptionInfo: CExceptionInfoConstructor

  // exported functions
  _callback(task: CFunction<(CPointer) => void>, obj: CPointer): void
  _vizzu_createChart(): CChartPtr
  _vizzu_createCanvas(): CCanvasPtr
  _vizzu_pointerDown(
    chart: CChartPtr,
    canvas: CCanvasPtr,
    pointerId: number,
    x: number,
    y: number
  ): void
  _vizzu_pointerUp(chart: CChartPtr, canvas: CCanvas, pointerId: number, x: number, y: number): void
  _vizzu_pointerMove(
    chart: CChartPtr,
    canvas: CCanvas,
    pointerId: number,
    x: number,
    y: number
  ): void
  _vizzu_pointerLeave(chart: CChartPtr, canvas: CCanvas, pointerId: number): void
  _vizzu_wheel(chart: CChartPtr, canvas: CCanvas, delta: number): void
  _vizzu_setLogging(enable: boolean): void
  _vizzu_update(
    chart: CChartPtr,
    canvas: CCanvas,
    width: number,
    height: number,
    renderControl: number
  ): void
  _vizzu_errorMessage(exceptionPtr: CException, typeinfo: CTypeInfo): CString
  _vizzu_version(): CString
  _data_addDimension(chart: CChartPtr, name: CString, categories: CArrayPtr, count: number): void
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
  _chart_setFilter(
    chart: CChartPtr,
    filter: CFunction<(CRecordPtr) => boolean>,
    deleter: CFunction<(filter: CFunction<(CRecordPtr) => boolean>) => void>
  ): void
  _chart_animate(chart: CChartPtr, callback: CFunction<(bool) => void>): void
  _chart_relToCanvasCoords(chart: CChartPtr, rx: number, ry: number): CPointPtr
  _chart_canvasToRelCoords(chart: CChartPtr, x: number, y: number): CPointPtr
  _chart_setKeyframe(chart: CChartPtr): void
  _chart_markerData(chart: CChartPtr, id: number): CString
  _addEventListener(
    chart: CChartPtr,
    name: CString,
    callback: CFunction<(CEventPtr, CString) => void>
  ): void
  _removeEventListener(
    chart: CChartPtr,
    name: CString,
    callback: CFunction<(CEventPtr, CString) => void>
  ): void
  _event_preventDefault(event: CEventPtr): void
  _anim_control(chart: CChartPtr, command: CString, param: CString): void
  _anim_setValue(chart: CChartPtr, path: CString, value: CString): void
}

declare function VizzuModule(options: ModuleOptions): Promice<CVizzu>

export default VizzuModule
