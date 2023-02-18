# Data

## Data types

`Vizzu` currently supports two types of data series: dimensions and measures.
Dimensions slice the data cube `Vizzu` uses, whereas measures are values within
the cube.

Dimensions are categorical series that can contain strings and numbers, but both
will be treated as strings. Temporal data such as dates or timestamps should
also be added as dimensions. By default, `Vizzu` will draw the elements on the
chart in the order they are provided in the data set. Thus we suggest adding
temporal data in a sorted format from oldest to newest.

Measures at the moment can only be numerical.

## Adding data

There are multiple ways you can add data to `Vizzu`:

- Specify data by series - column after column if you think of a spreadsheet
- Specify data by records - row after row
- Using data cube form

Elements with a missing value should contain the number zero. `null`,
`undefined` and empty cells will result in an error. In case of dimensions, add
`''` as a value to have a category without a name.

In the first two cases, data has to be in first normal form. Here is an example
of that:

| Genres | Kinds        | Popularity |
| ------ | ------------ | ---------- |
| Pop    | Hard         | 114        |
| Rock   | Hard         | 96         |
| Jazz   | Hard         | 78         |
| Metal  | Hard         | 52         |
| Pop    | Smooth       | 56         |
| Rock   | Smooth       | 36         |
| Jazz   | Smooth       | 174        |
| Metal  | Smooth       | 121        |
| Pop    | Experimental | 127        |
| Rock   | Experimental | 83         |
| Jazz   | Experimental | 94         |
| Metal  | Experimental | 58         |

In the `type` parameter, you can set if a series is a dimension or a measure.
Adding the `type` parameter is optional. If omitted, `Vizzu` will automatically
select the type depending on the first element of the values array using the
`typeof` operator. If the value is a number, it will be declared as a measure,
in any other case, a dimension.

Specify data by series:

```javascript
let data = {
    series: [{
        name: 'Genres',
        type: 'dimension',
        values: [
            'Pop', 'Rock', 'Jazz', 'Metal',
            'Pop', 'Rock', 'Jazz', 'Metal',
            'Pop', 'Rock', 'Jazz', 'Metal'
        ]
    }, {
        name: 'Kinds',
        type: 'dimension',
        values: [
            'Hard', 'Hard', 'Hard', 'Hard',
            'Smooth', 'Smooth', 'Smooth', 'Smooth',
            'Experimental', 'Experimental', 'Experimental', 'Experimental'
        ]
    }, {
        name: 'Popularity',
        type: 'measure',
        values: [114, 96, 78, 52, 56, 36, 174, 121, 127, 83, 94, 58]
    }]
};
```

Specify data by records:

```javascript
let data = {
    series: [{
        name: 'Genres',
        type: 'dimension'
    }, {
        name: 'Kinds',
        type: 'dimension'
    }, {
        name: 'Popularity',
        type: 'measure'
    }],
    records: [
        ['Pop', 'Hard', 114],
        ['Rock', 'Hard', 96],
        ['Jazz', 'Hard', 78],
        ['Metal', 'Hard', 52],
        ['Pop', 'Smooth', 56],
        ['Rock', 'Smooth', 36],
        ['Jazz', 'Smooth', 174],
        ['Metal', 'Smooth', 121],
        ['Pop', 'Experimental', 127],
        ['Rock', 'Experimental', 83],
        ['Jazz', 'Experimental', 94],
        ['Metal', 'Experimental', 58],
    ]
};
```

Using data cube form:

<table>
  <tbody><tr><th colspan="2" rowspan="2"></th><th colspan="4" style="text-align:center">Genres</th></tr>
  <tr>
      <td>Pop</td><td>Rock</td><td>Jazz</td><td>Metal</td>
  </tr>
  <tr><th rowspan="3" style="text-align:center">Kinds</th>
      <td style="text-align:center">Hard</td>
      <td>114</td><td>96</td><td>78</td><td>52</td>
  </tr>
  <tr><td style="text-align:center">Smooth</td>
      <td>56</td><td>36</td><td>74</td><td>121</td>
  </tr>
  <tr>
      <td style="text-align:center">Experimental</td>
      <td>127</td><td>83</td><td>94</td><td>58</td>
  </tr>
  <tr><td colspan="2"></td><th colspan="4" style="text-align:center">Popularity</th></tr>
</tbody></table>

```javascript
let data = {
    dimensions: [{
        name: 'Genres',
        values: ['Pop', 'Rock', 'Jazz', 'Metal']
    }, {
        name: 'Kinds',
        values: ['Hard', 'Smooth', 'Experimental']
    }],
    measures: [{
        name: 'Popularity',
        values: [
            [114, 96, 78, 52],
            [56, 36, 174, 121],
            [127, 83, 94, 58]
        ]
    }]
};
```

You should set the data in the first animate call:

```javascript
chart.animate({
    data
});
```
