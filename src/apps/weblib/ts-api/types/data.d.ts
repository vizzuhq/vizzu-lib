/* eslint-disable @typescript-eslint/no-unused-vars, lines-between-class-members, no-use-before-define */

export namespace Data {
  /** Name of the data series. */
  type SeriesName = string
  /** Type of the data series:
    - 'dimension' - categorical data containing strings 
      (dates should also be added as strings); 
    - 'measure' - continuous data containing numbers. */
  type SeriesType = 'dimension' | 'measure'
  /** Additional info about a data series besides the contained data. */
  interface SeriesMetaInfo {
    /** Name of the data series. It will be the unique id of the series to 
    reference it in various parts of the API, mainly in {@link Config.Channel} and
    {@link Data.Record}. This name will also be used by default for Axis and 
    Legend title. */
    name: SeriesName
    /** Type of the data series.
    If not set, the library will attempt to determine the type based on 
    the type of the first value. Number type will result in measure, 
    string type will result in dimension. */
    type?: SeriesType
    /** Unit of the data series. */
    unit?: string
  }
  interface AbstractSeriesInfo extends SeriesMetaInfo {
    /** Count of values in the series. */
    length: number
  }
  /** Meta data about dimension data series */
  interface DimensionSeriesInfo extends AbstractSeriesInfo {
    /** Distinct values in the series */
    categories: string[]
  }
  interface Range {
    /** Minimal value in the series */
    min: number
    /** Maximal value in the series */
    max: number
  }
  /** Meta data about measure data series */
  interface MeasureSeriesInfo extends AbstractSeriesInfo {
    range: Range
  }
  type SeriesInfo = DimensionSeriesInfo | MeasureSeriesInfo
  /** Meta data about the data set */
  interface Metainfo {
    series: SeriesInfo[]
  }
  /** Represents a categorical or data value */
  type Value = string | number
  /** List of data values in a series. */
  type Values = string[] | number[]
  /** Defines a data series of the data set, and contains a particular variable's
    values in the data set and meta info about the variable. */
  interface Series extends SeriesMetaInfo {
    /** The array that contains the values of the data series. The value types 
    should match {@link Data.SeriesMetaInfo.type}. If the data series
    is shorter than the longest data series defined, it will be internally 
    extended with empty values. */
    values?: Values
  }
  /** A record of the data set, containing one value of each data series 
    corresponding to the same index. */
  interface Record {
    /** Properties are provided for each data series, providing access to the value within
    the record referenced by its {@link Data.Series.name|name}. */
    [seriesName: SeriesName]: Value
  }
  type FilterCallback = (record: Record) => boolean
  interface Filter {
    /** A filter callback is called on each record of the dataset on chart
    generation. If the callback returns false, the record will not be shown on the chart. */
    filter?: FilterCallback | null
  }
  /** Data table specified by series. */
  interface TableBySeries extends Filter {
    /** The series that make up the the data set. */
    series: Series[]
  }
  /** Values of a data record in the order of the series in the dataset. */
  type ValueArray = Value[]
  /** Data table specified by records. */
  interface TableByRecords extends Filter {
    /** The information about the data series in the records of the data set. 
    Note: not needed if it was previously specified. */
    series?: SeriesMetaInfo[]
    /** The array of data records that make up the data set. */
    records: (ValueArray | Record)[]
  }
  type CubeRow = Values | CubeRow[]
  /** Defines a data series of the data cube, and contains a particular variable's
    values in the data cube and meta info about that variable. */
  interface CubeData extends SeriesMetaInfo {
    /** A nested array that contains the values of the data series. Nesting 
    level should match the number of {@link Data.Cube.dimensions}. */
    values: CubeRow
  }
  /** N dimensional data cude */
  interface Cube extends Filter {
    /** The list of the dimensions of the data cube. */
    dimensions: Series[]
    /** The list of measures of the data cube. */
    measures: CubeData[]
  }
  /** Data set is a collection of related {@link Data.Series|data series}. 
    Each chart works on a single data set. */
  type Set = TableBySeries | TableByRecords
  /** Types of the different available data series aggregators:
    
    - sum: sum of the values (default)
    - count: count of the values
    - min: minima of the values
    - max: maxima of the values
    - mean: average/mean of the values
    - distinct: number of different values */
  type AggregatorType = 'sum' | 'count' | 'min' | 'max' | 'mean' | 'distinct'
  /** The name of a series either alone or combined with an aggregator function. */
  type SeriesDescriptor = SeriesName | `${AggregatorType}(${SeriesName})`
  type SeriesList = SeriesDescriptor[] | SeriesDescriptor
}
