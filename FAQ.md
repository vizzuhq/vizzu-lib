# FAQ

## How to use Vizzu with Webpack, React, Vite and other bundlers/frameworks?

Here's
[a tutorial and an example](https://lib.vizzuhq.com/latest/tutorial/initialization/#working-with-bundlers-frameworks).

## Why on(), off(), feature() methods fail after constructor call?

The Vizzu constructor has to load Vizzu's WebAssembly module asynchronously to
prevent blocking the whole page. Because of this, any method except `animate()`
will throw an exception while the library is not initialized.

There is an `initializing` promise in the Vizzu class, which will resolve when
initialization is finished. You can call all methods after this promise is
resolved.

## How to load data from CSV into Vizzu?

This [example](https://lib.vizzuhq.com/0.3.0/content/howtos/csv-input/) reads
the data from a CSV.

You can find the source
[here](https://github.com/vizzuhq/vizzu-lib-doc/tree/gh-pages/0.3.0/content/howtos/csv-input);
the CSV loading part is in the csv-input.js file.

## How to remove fraction digits on integer labels during animation?

The maximum number of fraction digits can be set for marker and axis labels with
`maxFractionDigits` style parameter.
