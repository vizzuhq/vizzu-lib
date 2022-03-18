# FAQ

## How to use Vizzu with Webpack, React, Vite and other bundlers/frameworks?

Here's a tutorial and an example:

https://github.com/vizzuhq/vizzu-lib/blob/main/docs/content/tutorial/0-intro.md#working-with-bundlers-changing-the-location-of-the-wasm-module

## Why on(), off(), feature() methods fail after constructor call?

The Vizzu constructor has to load Vizzu's WebAssembly module asynchronously to prevent blocking the whole page. 
Because of this, any method except `animate()` will throw an exception while the library is not initialized. 

There is an `initializing` promise in the Vizzu class, which will resolve when initialization is finished. You can call all methods after
this promise is resolved.

## How to load data from CSV into Vizzu?

This example reads the data from a CSV: https://lib.vizzuhq.com/0.3.0/content/howtos/csv-input/

You can find the source here; the CSV loading part is in the csv-input.js file: https://github.com/vizzuhq/vizzu-lib-doc/tree/main/docs/0.3.0/content/howtos/csv-input
