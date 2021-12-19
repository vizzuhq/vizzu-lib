## Color palette & fonts

This and the next chapter provide a quick intro to the styling of the charts. You can either use the style property like in the following examples or use CSS. By using CSS, it's easier to set the same style for multiple charts on one page or re-use style settings.

If you use CSS, don't change the set parameters later on via the style property, but only with the chart.style.setProperty method.

The font sizes automatically adjust to the chart size to help 
readability, and can also be set separately or for specific groups.

The actual style settings of the chart can be accessed via the style property.

```javascript { "run": false }
console.log(chart.style); 
```


The color palette is changed to the colors we add here. The order of the dimension’s 
items in the data set determine which color belongs to which item as the colors are added one-by-one. 
If you want to use the same setting via CSS, you should add ```--vizzu-plot-marker-colorPalette: #9355e8FF #123456FF #BDAF10FF;```

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
CSS version: ```--vizzu-title-fontSize: 50;```

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
