## Align & range

> Vizzu offers different options to align your chart elements and to set the range 
> of values shown on the axes. Alignment can be used to create charts like a 
> stream chart where the elements are vertically centered . A good example for 
> using range is when you fix the y-axis so that it would not adapt to the data 
> being shown.

### Align: center

> Centered alignment. The effect of this parameter depends on the orientation of 
> the chart. For example, on a column chart, elements will be vertically centered, 
> whereas on a bar chart, horizontally.
> 
> ```javascript
> chart.animate({
> 	config: {
> 		align:'center'
> 	}
> })
> ```

### Align: stretch = % view

> Stretched alignment. This way the elements will proportionally fill the entire 
> plot area, effectively showing proportions in stacked charts. This is why the 
> scale will also switch from values to percentages when used. 
> 
> ```javascript
> chart.animate({
> 	config: {
> 		align:'stretch'
> 	}
> })
> ```

### Align: none - default

> Getting back to the default alignment
> 
> ```javascript
> chart.animate({
> 	config: {
> 		align:'none'
> 	}
> })
> ```

### Axis range set proportionally to shown values

> You can set the range of an axis by setting the minimum and maximum values of 
> it. Both parameters are optional so that you can set only one of those, and you 
> either set specific values or a relative value by adding the % sign. In this 
> example, we set the range of the y-axis in a way that the max value is 150% of 
> the biggest element’s value.
> 
> ```javascript
> chart.animate({
> 	config: {
> 		channels: {
> 			y: { range: { max: '150%' } }
> 		}
> 	}
> })
> ```

### Axis range set explicitly on an axis with discrete series

> You can also set the range for an axis with a dimension on it. You can even use 
> this feature to filter certain elements, just like in the following example.
> 
> ```javascript
> chart.animate({
> 	config: {
> 		channels: {
> 			x: { range: { min: -2, max: 3 } }
> 		}
> 	}
> })
> ```
