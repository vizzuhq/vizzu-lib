In `HTML`, create a placeholder element that will contain the rendered chart.

```html
<html>
 <body>
  <div id="myVizzu">
  </div>
 </body>
</html>
```

In `JavaScript`, initialize and configure the chart:

```javascript
import Vizzu from 'https://cdn.jsdelivr.net/npm/vizzu@xLIB_MINOR_VERSIONx/dist/vizzu.min.js'
import data from 'https://lib.vizzuhq.com/xLIB_MINOR_VERSIONx/assets/data/music_data.js'

let chart = new Vizzu('myVizzu')

await chart.initializing

chart.animate({
    data
})
```
