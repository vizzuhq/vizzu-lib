```javascript
let data = {
  series: [
    { name: 'Dimension', values: ['Alice', 'Bob', 'Ted'] },
    { name: 'Measure', values: [4, 3, 2] }
  ]
};
```
```javascript id="axis-demo-0"
chart.animate({ x: Discrete })
```
```javascript id="axis-demo-1"
chart.animate({ x: Measure })
```
```javascript id="axis-demo-2"
chart.animate({ x: [Measure, Discrete] })
```
<div id="axis-demo" style="width:500px; height:125px"></div>
