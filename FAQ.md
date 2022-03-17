# FAQ

## How to use Vizzu with Webpack, React or other bundlers/frameworks?

Here you can find a tutorial and a complete example on how to use Vizzu with different bundlers/frameworks:

https://github.com/vizzuhq/vizzu-lib/blob/main/docs/content/tutorial/0-intro.md#working-with-bundlers-changing-the-location-of-the-wasm-module

## Why on(), off(), feature() methods fail after constructor call?

Vizzu constructor have to load Vizzu's WebAssembly module, which is done in an assyncron manner to prevent blocking the whole page. 
Because of this, any method except animate() will throw an exception while the library is not initialized. 

There is an `initializing` promise in the Vizzu class which will resolve when initialization is finished. All methods can be called after
this promise is resolved.

## How to load data from CSV into Vizzu?

This example reads the data from CSV: https://lib.vizzuhq.com/0.3.0/content/howtos/csv-input/

You can find the source here: https://github.com/vizzuhq/vizzu-lib-doc/tree/main/docs/0.3.0/content/howtos/csv-input

The CSV loading part is in the csv-input.js file.
