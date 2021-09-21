## Color palette & fonts

This and the next chapter provide a quick intro to the styling of the charts using the style object. 
The style settings are similar to CSS. The font sizes automatically adjust to the chart size to help 
readability, but they can also be set separately or for specific groups.

The actual style settings of the chart can be accessed via the style property.

```javascript { "run": false }
console.log(chart.style); 
```


The color palette is changed to the colors we add here. The order of the dimension’s 
items in the data set determine which color belongs to which item as the colors are added one-by-one. 

```javascript { "title": "Color palette" }
chart.animate({
	style: {
		plot: {
			marker: {
				colorPalette: '#9355e8FF #123456FF #BDAF10FF'
			}
		}
	}
})
```

Changing the title font size will only affect the title; all other font sizes remain the same.

```javascript { "title": "Title font size" }
chart.animate({
	style: {
		title: {
			fontSize: 50
		}
	}
})
```

This is how to set the font size back to its default value

```javascript { "title": "Title font size - back to default" }
chart.animate({
	style: {
		title: {
			fontSize: null
		}
	}
})
```

In case you change the font size of the whole chart with the top-level fontSize 
parameter then every font on the chart will grow/shrink proportionally. 
The size refers to the font size of the axis labels by default.

```javascript { "title": "Setting all font sizes in one step" }
chart.animate({
	style: {
		fontSize: 20
	}
})
```

You can reset styles to default on any levels by setting them to null. 

```javascript { "title": "Setting all style settings back to default" }
chart.animate({
	style: null
})
```
