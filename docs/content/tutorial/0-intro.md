## Intro

This is the tutorial of Vizzu - a free, open-source Javascript library for 
building animated charts, data stories, and interactive explorers. 
It is an excellent place to start using Vizzu, as it walks you through the 
installation and initialization of the library, introduces the logic it employs 
and the different settings to control how your charts look and behave. 
By scrolling through the tutorial, you can see code snippets, and when you 
click on them, you can see how they affect the chart on the top of the screen.

### Installation

To show a Vizzu chart create a placeholder element that will contain the 
rendered chart and import the library into a JavaScript file or script tag 
in your web page:

```html
<div id="myVizzu" style="width:800px; height:480px;"></div>

<script type="module">
import Vizzu from 'https://cdn.jsdelivr.net/npm/vizzu@0.3.0/vizzu.js';
</script>
```

Alternatively, you can use npm (npm install vizzu) or load files from your own domain. 

### Initialization

Create a new object instance of the Vizzu class, add it to a variable, 
initialize the library, and then you can start setting up the first chart by 
calling the animate method.

```javascript { "run": false }
let chart = new Vizzu('myVizzu')

chart.initializing.then(
  chart => chart.animate({ /* ... */ }) 
)
```

### The basic logic of Vizzu

In Vizzu the animated charts are created via a promise chain where the chain 
elements are the states of the charts - described by the animate method - and 
Vizzu animates between these states. By each call, the animate method will 
modify the previously set state.

<img src="images/api-overview.svg" width="100%" alt="Illustration of Vizzu's promise chain">

The animate method has two parameters in total. The first parameter sets the 
chart, and the second determines how Vizzu should animate to that state. 

The first parameter has the following three properties:
- *data* - this is where you add the data that you want to put on the charts.
- *config* - this is where you can add or remove series on the channels and 
  set the general settings of the chart like the chart title, the geometry, the alignment etc.
- *style* - this is where you can set how your chart looks.

### Data types

Vizzu currently supports two types of data series: dimensions and measures. 
Dimensions slice the data cube Vizzu uses, whereas measures are values within the cube.

Dimensions are categorical series that can contain strings and numbers, but 
both will be treated as strings. Temporal data such as dates or timestamps 
should also be added as dimensions. Vizzu will draw the elements on the chart 
in the order they are provided in the data set by default. Thus we suggest 
adding temporal data in a sorted format from oldest to newest.

Measure in the current beta phase can only be numerical.

### Adding data

There are multiple ways you can add data to Vizzu:
- Specified by series - column after column if you think of a spreadsheet
- Specified by records - row after row.
- Data cube form 

Elements with a missing value should contain the number zero. ‘null’, 
‘undefined’ and empty cells will result in an error. In case of dimensions, 
add ‘’ as a value to have a category without a name. 

In all cases, data has to be in first normal form. Here is an example of it:
**Ide kéne táblázat?**

In the type parameter, you can set if a series is a dimension or a measure. 
Adding the type parameter is optional. If omitted, Vizzu will automatically 
select the type depending on the contents of the values array using the typeof 
operator. If all items are numbers, it will be declared as a measure, in any 
other case, a dimension.

Data specified by series:

```javascript { "run": false }
data: {
  series: [
    {
      name: 'Genres', 
      type: 'dimension', 
      values: ['Pop', 'Rock', 'Jazz', 'Metal']
    },
    {
      name: 'Types', 
      type: 'dimension', 
      values: ['Hard', 'Smooth', 'Experimental']
    },
    {
      name: 'Popularity', 
      type: 'measure', 
      values: [ 78, 96, 114, 52, 174, 36, 56, 121, 94, 83, 127, 58 ]
    }
  ]
}
```

Data specified by records:

```javascript  { "run": false }
data: {
  series: [
    { name: 'Genres', type: 'dimension' },
    { name: 'Types', type: 'dimension' },
    { name: 'Popularity', type: 'measure' }
  ],
  records: [
    ['Pop', 'Hard', 78],
    ['Rock', 'Hard', 96],
    ['Jazz', 'Hard', 114],
    ['Metal', 'Hard', 52],
    ['Pop', 'Smooth', 174],
    ['Rock', 'Smooth', 36],
    ['Jazz', 'Smooth', 56],
    ['Metal', 'Smooth', 121],
    ['Pop', 'Experimental', 94],
    ['Rock', 'Experimental', 83],
    ['Jazz', 'Experimental', 127],
    ['Metal', 'Experimental', 58],
  ]
}
```

Data cube:

```javascript { "run": false }
data: {
	dimensions: [ 
		{ name: 'Genres', values: [ 'Pop', 'Rock', 'Jazz', 'Metal' ] },
		{ name: 'Types', values: [ 'Hard', 'Smooth', 'Experimental' ] }
	],
	measures: [
		{
			name: 'Popularity',
			values: [
				[ 78, 96, 114, 52 ],
				[ 174, 36, 56, 121 ],
				[ 94, 83, 127, 58 ]
			]
		}
	]
}
```




