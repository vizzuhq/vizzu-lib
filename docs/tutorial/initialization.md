# Initialization

To show a `Vizzu` chart create a placeholder element that will contain the
rendered chart.

```html
<html>
 <body>
  <div id="myVizzu" style="width:800px; height:480px;">
  </div>
 </body>
</html>

```

Create a new object instance of the `Vizzu` class, add it to a variable,
initialize the library, and then you can start setting up the first chart by
calling the animate method.

```javascript
let chart = new Vizzu('myVizzu')

chart.initializing.then(
    chart => chart.animate({
        /* ... */
    })
)
```

## Working with bundlers & frameworks

The published `Vizzu` package contains a `JavaScript` (`vizzu.min.js`) and a
`WebAssembly` (`cvizzu.wasm`) module. By default, the `JavaScript` module tries
to load the wasm module from the same path it was imported from.

This behavior won't work with bundlers if they move the wasm file to a different
folder. To handle this issue, you can manually set the wasm module's location.

```javascript
Vizzu.options({
    wasmUrl: "/path/to/cvizzu.wasm"
})
```

!!! note
    You should set this path before the `Vizzu` constructor is called because it
    loads the wasm module.

Different bundlers will need additional, specific settings to recognize the wasm
file as a dependency and handle it correctly. For setting up `Vizzu` with a
specific bundler or framework, check the corresponding how-to/example below:

- [Webpack](https://github.com/vizzuhq/vizzu-webpack-demo)
- [React](https://github.com/vizzuhq/vizzu-react-example)
- [Vite](https://github.com/simzer/vizzu-vite-demo)
